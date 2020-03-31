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
#include "GAMGroup.h"
#include "MemoryDataSourceI.h"
#include "RealTimeApplication.h"
#include "ObjectRegistryDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class GAMSchedulerIDataSourceTest: public MemoryDataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

GAMSchedulerIDataSourceTest    () : MemoryDataSourceI() {

    }

    virtual ~GAMSchedulerIDataSourceTest() {

    }

    virtual const char8 *GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
        return "MemoryMapSynchronisedInputBroker";
    }

    virtual bool Synchronise() {
        return false;
    }

    virtual bool PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
        return true;
    }

    virtual bool GetInputBrokers(ReferenceContainer &inputBrokers, const char8* const functionName, void * const gamMemPtr) {
        bool ok = MemoryDataSourceI::GetInputBrokers(inputBrokers, functionName, gamMemPtr);
        if (ok) {
            ok = (inputBrokers.Size() > 0u);
        }
        ReferenceT<BrokerI> broker;
        if (ok) {
            broker = inputBrokers.Get(0u);
            ok = broker.IsValid();
        }
        if (ok) {
            broker->SetName("TestBroker");
        }
        return ok;
    }
};

CLASS_REGISTER(GAMSchedulerIDataSourceTest, "1.0")

class DummyScheduler: public GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

DummyScheduler    ();

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    bool ExecuteThreadCycle(uint32 threadId);

    virtual bool ConfigureScheduler(Reference realTimeApp);

    virtual void CustomPrepareNextState();

private:

    ScheduledState * const * scheduledStates;
};

DummyScheduler::DummyScheduler() :
        GAMSchedulerI() {
    scheduledStates = NULL_PTR(ScheduledState * const *);
}
MARTe::ErrorManagement::ErrorType DummyScheduler::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

bool DummyScheduler::ConfigureScheduler(Reference realTimeApp) {
    bool ret = GAMSchedulerI::ConfigureScheduler(realTimeApp);
    if (ret) {
        scheduledStates = GetSchedulableStates();
    }
    return ret;
}

bool DummyScheduler::ExecuteThreadCycle(uint32 threadId) {
    ReferenceT<RealTimeApplication> realTimeAppT = realTimeApp;
    return ExecuteSingleCycle(scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].executables,
                       scheduledStates[realTimeAppT->GetIndex()]->threads[threadId].numberOfExecutables);

}
MARTe::ErrorManagement::ErrorType DummyScheduler::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

void DummyScheduler::CustomPrepareNextState() {

}

CLASS_REGISTER(DummyScheduler, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
GAMSchedulerITest::GAMSchedulerITest() {

    static StreamString config = ""
            "$Fibonacci = {"
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
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
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
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMC = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add3"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add3"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMD = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add3"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add4"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAME = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add5"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add5"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMF = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add5"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add6"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMG = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add7"
            "                    Default = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                    Default = 2"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add7"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMH = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add7"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add8"
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
            "        +DDB2 = {"
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
            "                    Functions = {GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD, GAME, GAMF}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMG, GAMH, GAMA, GAMB}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC, GAMD}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = DummyScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        printf("\nFAILED PARSING\n");
    }

    cdb.MoveToRoot();
    ObjectRegistryDatabase::Instance()->Purge();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        printf("\nFAILED INITIALISATION\n");
    }

}

GAMSchedulerITest::~GAMSchedulerITest() {

    ObjectRegistryDatabase::Instance()->Purge();

}

bool GAMSchedulerITest::TestConstructor() {
    DummyScheduler sched;
    return true;
}

bool GAMSchedulerITest::TestConfigureScheduler() {

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    ReferenceT<GAMSchedulerI> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }

    if (!scheduler->ConfigureScheduler(app)) {
        return false;
    }

    uint32 numberOfExecutables = scheduler->GetNumberOfExecutables("State1", "Thread1");

    if (numberOfExecutables != 8) {
        return false;
    }
    numberOfExecutables = scheduler->GetNumberOfExecutables("State1", "Thread2");

    if (numberOfExecutables != 16) {
        return false;
    }
    numberOfExecutables = scheduler->GetNumberOfExecutables("State2", "Thread1");

    if (numberOfExecutables != 16) {
        return false;
    }
    numberOfExecutables = scheduler->GetNumberOfExecutables("State2", "Thread2");

    if (numberOfExecutables != 8) {
        return false;
    }
    return true;
}

bool GAMSchedulerITest::TestConfigureSchedulerFalse_InvalidState() {

    static StreamString config = ""
            "$Fibonacci = {"
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
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Default = 2"
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
            "        +GAMB = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add2"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB2"
            "                    Alias = add2"
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
            "        +DDB2 = {"
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
            "                    Functions = {GAMA GAMB}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = ReferenceContainer"
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
            "        Class = DummyScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveToRoot();
    ObjectRegistryDatabase::Instance()->Purge();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    ReferenceT<GAMSchedulerI> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }

    return (!scheduler->ConfigureScheduler(app));
}

bool GAMSchedulerITest::TestGetNumberOfExecutables() {
    return TestConfigureScheduler();
}

bool GAMSchedulerITest::TestPrepareNextState() {

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    ReferenceT<DummyScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }

    if (!scheduler->ConfigureScheduler(app)) {
        return false;
    }

    if (!scheduler->PrepareNextState("", "State1")) {
        printf("\nFailed pns\n");
        return false;
    }

    ReferenceT<GAM1> gama = app->Find("Functions.GAMA");
    ReferenceT<GAM1> gamb = app->Find("Functions.GAMB");
    ReferenceT<GAM1> gamc = app->Find("Functions.GAMC");
    ReferenceT<GAM1> gamd = app->Find("Functions.GAMD");
    ReferenceT<GAM1> game = app->Find("Functions.GAME");
    ReferenceT<GAM1> gamf = app->Find("Functions.GAMF");
    ReferenceT<GAM1> gamg = app->Find("Functions.GAMG");
    ReferenceT<GAM1> gamh = app->Find("Functions.GAMH");
    app->StartNextStateExecution();

    scheduler->ExecuteThreadCycle(0);

    if (gama->numberOfExecutions != 1u || gamb->numberOfExecutions != 1u) {
        return false;
    }
    scheduler->ExecuteThreadCycle(1);

    if (gamc->numberOfExecutions != 1u || gamd->numberOfExecutions != 1u || game->numberOfExecutions != 1u || gamf->numberOfExecutions != 1u) {
        return false;
    }

    if (!scheduler->PrepareNextState("State1", "State2")) {
        printf("\nFailed pns\n");
        return false;
    }
    app->StartNextStateExecution();

    scheduler->ExecuteThreadCycle(0);

    if (gamg->numberOfExecutions != 1u || gamh->numberOfExecutions != 1u || gama->numberOfExecutions != 2u || gamb->numberOfExecutions != 2u) {
        return false;
    }

    scheduler->ExecuteThreadCycle(1);

    if (gamc->numberOfExecutions != 2u || gamd->numberOfExecutions != 2u) {
        return false;
    }
    return true;
}

bool GAMSchedulerITest::TestExecuteSingleCycle() {
    return TestPrepareNextState();
}

bool GAMSchedulerITest::TestExecuteSingleCycle_False() {
    static StreamString config = ""
            "$TestExecuteSingleCycle_False = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = GAM1"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = GAMSchedulerIDataSourceTest"
            "                    Type = uint32"
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
            "        +GAMSchedulerIDataSourceTest = {"
            "            Class = GAMSchedulerIDataSourceTest"
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
            "    }"
            "    +Scheduler = {"
            "        Class = DummyScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ok = parser.Parse();
    if (!ok) {
        printf("\nFAILED PARSING\n");
    }

    cdb.MoveToRoot();
    ObjectRegistryDatabase::Instance()->Purge();
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
        if (!ok) {
            printf("\nFAILED INITIALISATION\n");
        }

    }
    ReferenceT<RealTimeApplication> app;
    if (ok) {
        app = ObjectRegistryDatabase::Instance()->Find("TestExecuteSingleCycle_False");
        ok = app.IsValid();
    }

    if (ok) {
        ok = app->ConfigureApplication();
    }

    ReferenceT<DummyScheduler> scheduler;
    if (ok) {
        scheduler = app->Find("Scheduler");
        ok = scheduler.IsValid();
    }

    if (ok) {
        ok = scheduler->ConfigureScheduler(app);
    }

    if (ok) {
        ok = scheduler->PrepareNextState("", "State1");
    }

    if (ok) {
        ok = app->StartNextStateExecution();
    }

    if (ok) {
        ok = !scheduler->ExecuteThreadCycle(0);
    }
    return ok;
}
