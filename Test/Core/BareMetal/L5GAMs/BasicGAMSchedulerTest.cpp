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
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

BasicGAMSchedulerTest::BasicGAMSchedulerTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
    ConfigurationDatabase cdb = GetCDB3();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
}

BasicGAMSchedulerTest::~BasicGAMSchedulerTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();

}

bool BasicGAMSchedulerTest::TestConstructor() {

    BasicGAMScheduler scheduler;

    return true;
}

bool BasicGAMSchedulerTest::TestInitialise() {
    ConfigurationDatabase cdb;
    cdb.Write("NumberOfCycles", "1");
    BasicGAMScheduler scheduler;
    return scheduler.Initialise(cdb);
}

bool BasicGAMSchedulerTest::TestStartExecution() {

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

    return *val==12;

}

bool BasicGAMSchedulerTest::TestStopExecution() {
    return true;
}
