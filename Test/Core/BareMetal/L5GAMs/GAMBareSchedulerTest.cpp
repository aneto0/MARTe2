/**
 * @file GAMBareSchedulerTest.cpp
 * @brief Source file for class GAMBareSchedulerTest
 * @date 18/06/2021
 * @author Giuseppe Avon
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
 * the class GAMBareSchedulerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMBareSchedulerTest.h"
#include "GAMSchedulerI.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "ObjectRegistryDatabase.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
GAMBareSchedulerTest::GAMBareSchedulerTest() {

    static StreamString config = ""
            "$TestRTApp = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Default = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add1"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +State1 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA }"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = GAMBareScheduler"
            "        TimingDataSource = Timings"
            "        MaxCycles = 1"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure during configuration file parsing");
    }

    cdb.MoveToRoot();
    ObjectRegistryDatabase::Instance()->Purge();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failure during configuration file parsing");
    }
}

GAMBareSchedulerTest::~GAMBareSchedulerTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool GAMBareSchedulerTest::TestConstructor() {
    //The constructor of the bare scheduler is almost empty, only sets a value to NULL
    GAMBareScheduler gamScheduler;
    return true;
}

bool GAMBareSchedulerTest::PrepareApplication() {
    bool retVal = true;

    realTimeApplication = ObjectRegistryDatabase::Instance()->Find("TestRTApp");
    retVal = realTimeApplication.IsValid();
    if(!retVal) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "RealTimeApplication reference is not valid!");
    }

    if (retVal) {
        retVal = realTimeApplication->ConfigureApplication();
        if(!retVal) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "RealTimeApplication configuration failure!");
        }
    }

    if(retVal) {
        scheduler = realTimeApplication->Find("Scheduler");
        retVal = scheduler.IsValid();
        if(!retVal) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Scheduler reference is not valid!");
        }
    }

    if(retVal) {
        retVal = realTimeApplication->PrepareNextState("State1");
        if(!retVal) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Scheduler PrepareNextState failed!");
        }
    }

    if(retVal) {
        uint32 numberOfExecutables = scheduler->GetNumberOfExecutables("State1", "Thread1");
        retVal = (numberOfExecutables == 3);
        if(!retVal) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Wrong number of executables %d != 1", numberOfExecutables);
        }
    }
    
    return retVal;
}

bool GAMBareSchedulerTest::TestInitialise() {
    bool retVal = PrepareApplication();

    return retVal;
}

bool GAMBareSchedulerTest::TestConfigureScheduler() {
    bool retVal = PrepareApplication();

    return retVal;
}

bool GAMBareSchedulerTest::TestStartCurrentStateExecution() {
    bool retVal = PrepareApplication();

    if(retVal) {
        retVal = realTimeApplication->StartNextStateExecution();
    }

    retVal = realTimeApplication->StopCurrentStateExecution();

    return retVal;
}

bool GAMBareSchedulerTest::TestStopCurrentStateExecution() {
    bool retVal = PrepareApplication();

    if(retVal) {
        retVal = realTimeApplication->StopCurrentStateExecution();
    }
    
    return retVal;
}
