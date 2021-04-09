/**
 * @file TimingDataSourceTest.cpp
 * @brief Source file for class TimingDataSourceTest
 * @date 28/07/2016
 * @author Andre Neto
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
 * the class TimingDataSourceTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "TimingDataSourceTest.h"
#include "GAM.h"
#include "GAMSchedulerI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static bool InitialiseTimingDataSourceEnviroment(const char8 * const config) {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Application1");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}

class TimingDataSourceTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

TimingDataSourceTestScheduler1    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    virtual void CustomPrepareNextState(){

    }

};

TimingDataSourceTestScheduler1::TimingDataSourceTestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType TimingDataSourceTestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType TimingDataSourceTestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

CLASS_REGISTER(TimingDataSourceTestScheduler1, "1.0")

class TimingDataSourceTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

TimingDataSourceTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();

    void *GetInputSignalsMemory();

    void *GetOutputSignalsMemory();
};

TimingDataSourceTestGAM1::TimingDataSourceTestGAM1() :
        GAM() {

}

bool TimingDataSourceTestGAM1::Setup() {
    return true;
}

bool TimingDataSourceTestGAM1::Execute() {
    return true;
}

void* TimingDataSourceTestGAM1::GetInputSignalsMemory() {
    return GAM::GetInputSignalsMemory();
}

void* TimingDataSourceTestGAM1::GetOutputSignalsMemory() {
    return GAM::GetOutputSignalsMemory();
}

CLASS_REGISTER(TimingDataSourceTestGAM1, "1.0")

static const char8 * const config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = TimingDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = TimingDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = TimingDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = TimingDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
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
        "                    Functions = {GAMB GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMB GAMA GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = TimingDataSourceTestScheduler1"
        "    }"
        "}";

//Adds consumers to the Timings node, which is illegal.
static const char8 * const config2 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = TimingDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMB = {"
        "            Class = TimingDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMC = {"
        "            Class = TimingDataSourceTestGAM1"
        "            OutputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Timings"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "        +GAMD = {"
        "            Class = TimingDataSourceTestGAM1"
        "            InputSignals = {"
        "               Signal1 = {"
        "                   DataSource = Timings"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 1"
        "               }"
        "               Signal2 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "               Signal3 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "                   NumberOfElements = 10"
        "                   NumberOfDimensions = 2"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
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
        "                    Functions = {GAMB GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMB GAMA GAMD GAMC}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = TimingDataSourceTestScheduler1"
        "    }"
        "}";

static const char8 * const configTestCurrentStateSignal = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMA = {"
        "            Class = TimingDataSourceTestGAM1"
        "            InputSignals = {"
        "                CurrentState = {"
        "                   DataSource = Timings"
        "                   Type = uint32"
        "                }"
        "            }"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   DataSource = DDB1"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        Class = ReferenceContainer"
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
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = GAMScheduler"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool TimingDataSourceTest::TestConstructor() {
    bool ret = InitialiseTimingDataSourceEnviroment(config1);
    if (ret) {
        ReferenceT<TimingDataSource> timingDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Timings");
        ret = timingDataSource.IsValid();
    }
    return ret;
}

bool TimingDataSourceTest::TestSetConfiguredDatabase() {
    bool ret = InitialiseTimingDataSourceEnviroment(config1);
    ReferenceT<TimingDataSource> timingDataSource;
    if (ret) {
        timingDataSource = ObjectRegistryDatabase::Instance()->Find("Application1.Data.Timings");
        ret = timingDataSource.IsValid();
    }
    return ret;
}

bool TimingDataSourceTest::TestCurrentStateValue() {
    ConfigurationDatabase cdb;
    StreamString configStream = configTestCurrentStateSignal;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }

    ReferenceT<RealTimeApplication> application;
    ReferenceT<TimingDataSourceTestGAM1> gama;

    if (ok) {
        application = god->Find("Application1");
        ok = application.IsValid();
    }

    if (ok) {
        gama = application->Find("Functions.GAMA");
        ok = gama.IsValid();
    }
    
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Cannot find GAMA");
    }

    if (ok) {
        ok = application->ConfigureApplication();
    }

    uint32* currentStateValue = (uint32*)gama->GetInputSignalsMemory();

    if(currentStateValue == NULL_PTR(uint32*)) {
        ok = false;
    }

    //Round 1 - Going to State1 and ensuring we have the index to 0
    if (ok) {
        ok = application->PrepareNextState("State1");
    }

    if (ok) {
        ok = application->StartNextStateExecution();
    }

    Sleep::MSec(10);
        
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Null pointer for the currentStateValue");
    }

    if (ok) {
        ok = (*currentStateValue == 0u);
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "[A] Current state value should be 0 instead of %d", *currentStateValue);
    }

    //Round 2 - Going to State2 and ensuring we have the index to 1
    if (ok) {
        ok = application->PrepareNextState("State2");
    }

    if (ok) {
        ok = application->StartNextStateExecution();
    }

    Sleep::MSec(10);
        
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }

    if (ok) {
        ok = (*currentStateValue == 1u);
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "[B] Current state value should be 1 instead of %d", *currentStateValue);
    }

    //Round 3 - Going to back again to State1 and ensuring we have the index to 0
    if (ok) {
        ok = application->PrepareNextState("State1");
    }

    if (ok) {
        ok = application->StartNextStateExecution();
    }

    Sleep::MSec(10);
        
    if (ok) {
        ok = application->StopCurrentStateExecution();
    }

    if (ok) {
        ok = (*currentStateValue == 0u);
    }

    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "[C] Current state value should be 0 instead of %d", *currentStateValue);
    }

    god->Purge();
    return ok;
}

bool TimingDataSourceTest::TestSetConfiguredDatabase_False_Producers() {
    ConfigurationDatabase config;
    config.CreateAbsolute("Signals.0");
    config.Write("QualifiedName", "TestSignal");

    config.CreateAbsolute("Signals.0.States.State1");
    StreamString *producers = new StreamString[1];
    producers[0] = "TestProducer";
    Vector<StreamString> vec(producers, 1);
    config.Write("GAMNamesProducers", vec);
    producers[0] = "0";
    config.Write("GAMProducers", vec);
    delete [] producers;
    config.MoveToRoot();
    TimingDataSource timingDataSource;
    return !timingDataSource.SetConfiguredDatabase(config);
}
