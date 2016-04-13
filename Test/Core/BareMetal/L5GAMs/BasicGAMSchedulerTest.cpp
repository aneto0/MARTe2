/**
 * @file BasicGAMSchedulerTest.cpp
 * @brief Source file for class BasicGAMSchedulerTest
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
 * the class BasicGAMSchedulerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicGAMSchedulerTest.h"
#include "ConfigurationDatabase.h"
#include "GAMTestHelper.h"
#include "Threads.h"
#include "RealTimeGenericDataDef.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static float64 sleepTime=2.5;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool BasicGAMSchedulerTest::TestConstructor() {

    BasicGAMScheduler scheduler;

    return true;
}

bool BasicGAMSchedulerTest::TestInitialise() {
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfCycles", "1");
    cdb.Write("SleepTime", "1e-6");
    BasicGAMScheduler scheduler;
    return scheduler.Initialise(cdb);
}

bool BasicGAMSchedulerTest::TestStartExecution() {
    ConfigurationDatabase cdb = GetCDB3();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }

    if (!app->ConfigureDataSource()) {
        return false;
    }

    if (!app->ValidateDataSource()) {
        return false;
    }
    if (!app->AllocateDataSource()) {
        return false;
    }
    if (!app->ConfigureDataSourceLinks()) {
        return false;
    }
    if (!app->ValidateDataSourceLinks()) {
        return false;
    }

    // already an execution
    if (!app->PrepareNextState("+State1")) {
        return false;
    }

    ReferenceT<BasicGAMScheduler> scheduler = app->Find("+Scheduler");

    scheduler->StartExecution(0);

    BasicRealTimeDataSourceInputReader reader;
    reader.SetApplication(*app.operator ->());
    reader.AddVariable(app->Find("+Functions.+GAM1.+Input.+Counter"));

    reader.Finalise();

    reader.Read(0);

    uint32 *val = (uint32*) reader.GetData(0);
    ObjectRegistryDatabase::Instance()->CleanUp();

    return *val == 12;

}


static void StopEx(BasicGAMScheduler &scheduler) {
    Sleep::Sec(sleepTime);
    printf("\nstopping...\n");
    scheduler.StopExecution();
    Threads::EndThread();
}

bool BasicGAMSchedulerTest::TestStopExecution() {
    ConfigurationDatabase cdb = GetCDB4();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }

    if (!app->ConfigureDataSource()) {
        return false;
    }

    if (!app->ValidateDataSource()) {
        return false;
    }
    if (!app->AllocateDataSource()) {
        return false;
    }
    if (!app->ConfigureDataSourceLinks()) {
        return false;
    }
    if (!app->ValidateDataSourceLinks()) {
        return false;
    }

    ReferenceT<BasicGAMScheduler> scheduler = app->Find("+Scheduler");
    Threads::BeginThread((ThreadFunctionType) StopEx, scheduler.operator ->());

    // already an execution
    if (!app->PrepareNextState("+State1")) {
        return false;
    }

    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }

    BasicRealTimeDataSourceInputReader reader;
    reader.SetApplication(*app.operator ->());
    reader.AddVariable(app->Find("+Functions.+GAM1.+Input.+Counter"));

    reader.Finalise();

    reader.Read(0);

    uint32 *val = (uint32*) reader.GetData(0);

    ObjectRegistryDatabase::Instance()->CleanUp();

    return *val != 0;
}

bool BasicGAMSchedulerTest::TestGAMAbsoluteExecutionTime() {
    ConfigurationDatabase cdb = GetCDB4();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }

    if (!app->ConfigureDataSource()) {
        return false;
    }

    if (!app->ValidateDataSource()) {
        return false;
    }
    if (!app->AllocateDataSource()) {
        return false;
    }
    if (!app->ConfigureDataSourceLinks()) {
        return false;
    }
    if (!app->ValidateDataSourceLinks()) {
        return false;
    }

    ReferenceT<BasicGAMScheduler> scheduler = app->Find("+Scheduler");
    Threads::BeginThread((ThreadFunctionType) StopEx, scheduler.operator ->());

    // already an execution
    if (!app->PrepareNextState("+State1")) {
        return false;
    }

    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }

    BasicRealTimeDataSourceInputReader reader;
    reader.SetApplication(*app.operator ->());

    ConfigurationDatabase absTime1;
    absTime1.Write("Class", "RealTimeGenericDataDef");
    absTime1.Write("Type", "uint64");
    absTime1.Write("IsFinal", "true");
    absTime1.Write("Path", "GAM_Times.+GAM1.AbsoluteUsecTime");

    RealTimeGenericDataDef def1;
    if (!def1.Initialise(absTime1)) {
        return false;
    }

    if(!reader.AddVariable(ReferenceT<RealTimeGenericDataDef>(&def1))){
        return false;
    }

    ConfigurationDatabase absTime2;
    absTime2.Write("Class", "RealTimeGenericDataDef");
    absTime2.Write("Type", "uint64");
    absTime2.Write("IsFinal", "true");
    absTime2.Write("Path", "GAM_Times.+GAM2.AbsoluteUsecTime");

    RealTimeGenericDataDef def2;
    if (!def2.Initialise(absTime2)) {
        return false;
    }
    if(!reader.AddVariable(ReferenceT<RealTimeGenericDataDef>(&def2))){
        return false;
    }

    if(!reader.Finalise()){
        return false;
    }

    reader.Read(0);

    uint64 *val1 = (uint64*) reader.GetData(0);
    uint64 *val2 = (uint64*) reader.GetData(1);

    printf("\n abs1=%d abs2=%d\n", *val1, *val2);

    ObjectRegistryDatabase::Instance()->CleanUp();

    return (*val1>0) && (*val2>0);
}

bool BasicGAMSchedulerTest::TestGAMRelativeExecutionTime() {
    ConfigurationDatabase cdb = GetCDB4();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("$Application1");

    if (!app->ConfigureArchitecture()) {
        return false;
    }

    if (!app->ConfigureDataSource()) {
        return false;
    }

    if (!app->ValidateDataSource()) {
        return false;
    }
    if (!app->AllocateDataSource()) {
        return false;
    }
    if (!app->ConfigureDataSourceLinks()) {
        return false;
    }
    if (!app->ValidateDataSourceLinks()) {
        return false;
    }

    ReferenceT<BasicGAMScheduler> scheduler = app->Find("+Scheduler");

    Threads::BeginThread((ThreadFunctionType) StopEx, scheduler.operator ->());

    // already an execution
    if (!app->PrepareNextState("+State1")) {
        return false;
    }

    while (Threads::NumberOfThreads() > 0) {
        Sleep::MSec(10);
    }

    BasicRealTimeDataSourceInputReader reader;
    reader.SetApplication(*app.operator ->());

    ConfigurationDatabase absTime1;
    absTime1.Write("Class", "RealTimeGenericDataDef");
    absTime1.Write("Type", "uint64");
    absTime1.Write("IsFinal", "true");
    absTime1.Write("Path", "GAM_Times.+GAM1.RelativeUsecTime");

    RealTimeGenericDataDef def1;
    if (!def1.Initialise(absTime1)) {
        return false;
    }

    if(!reader.AddVariable(ReferenceT<RealTimeGenericDataDef>(&def1))){
        return false;
    }

    ConfigurationDatabase absTime2;
    absTime2.Write("Class", "RealTimeGenericDataDef");
    absTime2.Write("Type", "uint64");
    absTime2.Write("IsFinal", "true");
    absTime2.Write("Path", "GAM_Times.+GAM2.RelativeUsecTime");

    RealTimeGenericDataDef def2;
    if (!def2.Initialise(absTime2)) {
        return false;
    }
    if(!reader.AddVariable(ReferenceT<RealTimeGenericDataDef>(&def2))){
        return false;
    }

    if(!reader.Finalise()){
        return false;
    }

    reader.Read(0);

    uint64 *val1 = (uint64*) reader.GetData(0);
    uint64 *val2 = (uint64*) reader.GetData(1);

    printf("\n rel1=%d rel2=%d\n", *val1, *val2);

    ObjectRegistryDatabase::Instance()->CleanUp();
    return (*val1>0) && (*val2>0);
}
