/**
 * @file GAMSchedulerITest.cpp
 * @brief Source file for class GAMSchedulerITest
 * @date 04/04/2016
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class GAMSchedulerITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSchedulerITest.h"
#include "GAMTestHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool GAMSchedulerITest::TestConstructor() {
    DummyScheduler sched;
    return true;
}

bool GAMSchedulerITest::TestInsertRecord() {

    const uint32 size = 32;
    ReferenceT<RealTimeThread> threadsS1[size];
    ReferenceT<RealTimeThread> threadsS2[size];
    DummyScheduler sched;
    for (uint32 i = 0u; i < size; i++) {
        threadsS1[i] = ReferenceT<RealTimeThread>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        threadsS2[i] = ReferenceT<RealTimeThread>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

        sched.InsertRecord("state1", threadsS1[i]);
        sched.InsertRecord("state2", threadsS2[i]);
    }

    ReferenceT<GAMSchedulerRecord> rec1 = sched.Get(0);
    ReferenceT<GAMSchedulerRecord> rec2 = sched.Get(1);

    if (StringHelper::Compare(rec1->GetName(), "state1") != 0) {
        return false;
    }

    if (StringHelper::Compare(rec2->GetName(), "state2") != 0) {
        return false;
    }

    for (uint32 i = 0u; i < size; i++) {
        ReferenceT<RealTimeThread> test = rec1->Peek(i);
        if (test != threadsS1[i]) {
            return false;
        }
        test = rec2->Peek(i);
        if (test != threadsS2[i]) {
            return false;
        }
    }
    return (rec1->GetNumberOfThreads() == size) && (rec2->GetNumberOfThreads() == size);
}

bool GAMSchedulerITest::TestPrepareNextState() {

    StreamString conf = cdbStr3;
    conf.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");
    ReferenceT<DummyScheduler> sched = ObjectRegistryDatabase::Instance()->Find("Scheduler");
    sched->SetApplication(*app.operator ->());

    if (!app->ConfigureArchitecture()) {
        return false;
    }

    if (!app->ConfigureDataSource()) {
        return false;
    }

    if (!app->AllocateDataSource()) {
        return false;
    }
    RealTimeStateInfo info;
    info.activeBuffer = 0;
    info.currentState = "";
    info.nextState = "State1";
    bool ret(sched->PrepareNextState(info));
    ObjectRegistryDatabase::Instance()->CleanUp();

    return ret;
}

bool GAMSchedulerITest::TestPrepareNextStateFalse_NoAppSet() {
    RealTimeStateInfo info;
    info.activeBuffer = 0;
    info.currentState = "";
    info.nextState = "state1";
    ReferenceT<RealTimeThread> threadsS1 = ReferenceT<RealTimeThread>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    DummyScheduler sched;
    sched.InsertRecord("state1", threadsS1);

    return (!sched.PrepareNextState(info));

}

bool GAMSchedulerITest::TestPrepareNextStateFalse_InvalidNextState() {
    RealTimeStateInfo info;
    info.activeBuffer = 0;
    info.currentState = "state1";
    info.nextState = "state2";
    ReferenceT<RealTimeThread> threadsS1 = ReferenceT<RealTimeThread>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    DummyScheduler sched;
    sched.InsertRecord("state1", threadsS1);

    RealTimeApplication app;
    sched.SetApplication(app);

    return (!sched.PrepareNextState(info));
}

bool GAMSchedulerITest::TestSetApplication() {
    DummyScheduler sched;
    RealTimeApplication app;
    sched.SetApplication(app);
    return true;
}

bool GAMSchedulerITest::TestChangeState() {
    DummyScheduler sched;

    // stops and starts again the execution
    sched.ChangeState(0);

    return (sched.numberOfExecutions == 2);
}

bool GAMSchedulerITest::TestExecuteSingleCycle() {
    StreamString conf = cdbStr4;
    conf.Seek(0);
    ConfigurationDatabase cdb;
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");
    ReferenceT<DummyScheduler> sched = ObjectRegistryDatabase::Instance()->Find("Scheduler");
    sched->SetApplication(*app.operator ->());

    if (!app->ConfigureArchitecture()) {
        return false;
    }

    if (!app->ConfigureDataSource()) {
        return false;
    }

    if (!app->AllocateDataSource()) {
        return false;
    }

    if (!app->ConfigureDataSourceLinks()) {
        return false;
    }
    RealTimeStateInfo info;
    info.activeBuffer = 1;
    info.currentState = "";
    info.nextState = "State1";

    app->PrepareNextState("State1");

    if (!sched->PrepareNextState(info)) {
        return false;
    }

    uint32 actBuffer = 0;
    uint32 threadId = 0;

    sched->ExecuteSingleCycle(threadId, actBuffer);

    ReferenceT<DummyGAM> gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAM1");
    ReferenceT<DummyGAM> gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAM2");

    bool ret = (gam1->flag == 1) && (gam2->flag == 2);
    sched->ExecuteSingleCycle(threadId, actBuffer);

    ObjectRegistryDatabase::Instance()->CleanUp();

    return ret;
}

