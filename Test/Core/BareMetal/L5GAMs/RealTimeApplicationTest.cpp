/**
 * @file RealTimeApplicationTest.cpp
 * @brief Source file for class RealTimeApplicationTest
 * @date 26/02/2016
 * @author Giuseppe Ferro
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
 * the class RealTimeApplicationTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeApplicationTest.h"

#include <DataSourceI.h>
#include <GAMDataSource.h>
#include "RealTimeState.h"
#include "RealTimeThread.h"
#include "ObjectRegistryDatabase.h"
#include "GAMTestHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class RealTimeApplicationTestScheduler: public GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

RealTimeApplicationTestScheduler    ();

    virtual void StartExecution();

    virtual void StopExecution();

    void SetThreadToExecute(uint32 tid);

    void ExecuteThreadCycle(uint32 threadId);

    bool Started();

    const char8 *GetStateName();
private:
    uint32 threadId;
    bool started;

};

RealTimeApplicationTestScheduler::RealTimeApplicationTestScheduler() :
        GAMSchedulerI() {
    threadId = 0u;
    started = false;
}

bool RealTimeApplicationTestScheduler::Started() {
    return started;
}
void RealTimeApplicationTestScheduler::StartExecution() {
    started = true;
    ExecuteSingleCycle(GetSchedulableStates()[RealTimeApplication::GetIndex()]->threads[threadId].executables,
                       GetSchedulableStates()[RealTimeApplication::GetIndex()]->threads[threadId].numberOfExecutables);

}

void RealTimeApplicationTestScheduler::StopExecution() {
    started = false;

}

const char8 *RealTimeApplicationTestScheduler::GetStateName() {
    return GetSchedulableStates()[RealTimeApplication::GetIndex()]->name;
}

CLASS_REGISTER(RealTimeApplicationTestScheduler, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeApplicationTest::RealTimeApplicationTest() {

    config = ""
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
            "                    Default = 3"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add4"
            "                    Default = 5"
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
            "                    Default = 8"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add6"
            "                    Default = 13"
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
            "                    Default = 21"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB2"
            "                    Type = uint32"
            "                    Alias = add8"
            "                    Default = 34"
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
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";

}

RealTimeApplicationTest::~RealTimeApplicationTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeApplicationTest::TestConstructor() {
    RealTimeApplication app;

    return app.GetIndex() == 1;
}

bool RealTimeApplicationTest::Init() {
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    bool ret = parser.Parse();

    if (ret) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->CleanUp();
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
        if (!ret) {
        }
    }
    return ret;
}

bool RealTimeApplicationTest::TestInitialise() {

    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    if (!app.Initialise(cdb)) {
        return false;
    }

    return true;

}

bool RealTimeApplicationTest::TestInitialiseFalse_NoStatesFound() {

    StreamString config = ""
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
            "    +Scheduler = {"
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    return (!app.Initialise(cdb));
}

bool RealTimeApplicationTest::TestInitialiseFalse_NoDataFound() {

    StreamString config = ""
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
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    return (!app.Initialise(cdb));
}

bool RealTimeApplicationTest::TestInitialiseFalse_NoFunctionsFound() {
    StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
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
            "        Class = RealTimeApplicationTestScheduler"
            "        TimingDataSource = Timings"
            "    }"
            "}";
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    return (!app.Initialise(cdb));
}

bool RealTimeApplicationTest::TestInitialiseFalse_NoSchedulerFound() {
    StreamString config = ""
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
            "}";
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    cdb.MoveAbsolute("$Fibonacci");

    RealTimeApplication app;

    return (!app.Initialise(cdb));
}

bool RealTimeApplicationTest::TestConfigureApplication() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    return true;

}

bool RealTimeApplicationTest::TestConfigureApplicationNoInit() {
    if (!Init()) {
        return false;
    }
    config.Seek(0ull);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }
    cdb.MoveAbsolute("$Fibonacci");
    RealTimeApplicationConfigurationBuilder builder(cdb, "DDB1");
    builder.ConfigureBeforeInitialisation();
    ConfigurationDatabase fcdb;
    ConfigurationDatabase dcdb;

    builder.Copy(fcdb, dcdb);
    fcdb.MoveToRoot();
    dcdb.MoveToRoot();

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication(fcdb, dcdb)) {
        return false;
    }
    return true;

}

static uint32 GetDsDefault(ReferenceT<DataSourceI> ddb,
                           const char8 *signalName) {
    uint32 signalIndex;
    if (!ddb->GetSignalIndex(signalIndex, signalName)) {
        return false;
    }

    uint32 *addPtr;
    if (!ddb->GetSignalMemoryBuffer(signalIndex, 0, (void*&) addPtr)) {
        return false;
    }
    return *addPtr;

}

bool RealTimeApplicationTest::TestPrepareNextState() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }
    if (app->GetIndex() != 1) {
        return false;
    }
    ReferenceT<DataSourceI> ddb1 = app->Find("Data.DDB1");
    ReferenceT<DataSourceI> ddb2 = app->Find("Data.DDB2");
    ReferenceT<GAM1> gams[8];
    gams[0] = app->Find("Functions.GAMA");
    gams[1] = app->Find("Functions.GAMB");
    gams[2] = app->Find("Functions.GAMC");
    gams[3] = app->Find("Functions.GAMD");
    gams[4] = app->Find("Functions.GAME");
    gams[5] = app->Find("Functions.GAMF");
    gams[6] = app->Find("Functions.GAMG");
    gams[7] = app->Find("Functions.GAMH");

    if (!app->PrepareNextState("State1")) {
        return false;
    }

    uint32 testContext1[] = { 1, 1, 1, 1, 1, 1, 0, 0 };
    for (uint32 i = 0u; i < 8u; i++) {
        if (gams[i]->context != testContext1[i]) {
            return false;
        }
    }

    if ((!ddb1.IsValid()) || (!ddb2.IsValid())) {
        return false;
    }

    if (GetDsDefault(ddb1, "add1") != 1) {
        return false;
    }

    if (GetDsDefault(ddb2, "add2") != 2) {
        return false;
    }

    if (GetDsDefault(ddb1, "add3") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add4") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add5") != 8) {
        return false;
    }

    if (GetDsDefault(ddb2, "add6") != 13) {
        return false;
    }

    app->StartExecution();

    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }

    if (StringHelper::Compare(scheduler->GetStateName(), "State1") != 0) {
        return false;
    }

    if (!app->PrepareNextState("State2")) {
        return false;
    }

    uint32 testContext2[] = { 2, 2, 2, 2, 1, 1, 1, 1 };
    for (uint32 i = 0u; i < 8u; i++) {
        if (gams[i]->context != testContext2[i]) {
            return false;
        }
    }

    if (GetDsDefault(ddb1, "add1") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add2") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add3") != 3) {
        return false;
    }

    if (GetDsDefault(ddb2, "add4") != 5) {
        return false;
    }

    if (GetDsDefault(ddb1, "add7") != 21) {
        return false;
    }

    if (GetDsDefault(ddb2, "add8") != 34) {
        return false;
    }

    app->StartExecution();

    if (StringHelper::Compare(scheduler->GetStateName(), "State2") != 0) {
        return false;
    }
    return true;

}

bool RealTimeApplicationTest::TestStartExecution() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }
    if (app->GetIndex() != 1) {
        return false;
    }

    if (!app->PrepareNextState("State1")) {
        return false;
    }
    app->StartExecution();
    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }
    if (!scheduler->Started()) {
        return false;
    }

    return app->GetIndex() == 0;

}

bool RealTimeApplicationTest::TestStopExecution() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    if (!app->PrepareNextState("State1")) {
        return false;
    }
    app->StartExecution();
    ReferenceT<RealTimeApplicationTestScheduler> scheduler = app->Find("Scheduler");
    if (!scheduler.IsValid()) {
        return false;
    }
    if (!scheduler->Started()) {
        return false;
    }

    app->StopExecution();
    return !scheduler->Started();
}

bool RealTimeApplicationTest::TestGetStates() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    ReferenceContainer states;
    if (!app->GetStates(states)) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    if (!app->GetStates(states)) {
        return false;
    }

    return (states.Size() == 2);
}

bool RealTimeApplicationTest::TestGetIndex() {
    if (!Init()) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app.IsValid()) {
        return false;
    }

    if (!app->ConfigureApplication()) {
        return false;
    }

    if (app->GetIndex() != 0u) {
        return false;
    }

    if (!app->PrepareNextState("State1")) {
        return false;
    }

    return (app->GetIndex() == 1u);
}

