/**
 * @file RealTimeThreadTest.cpp
 * @brief Source file for class RealTimeThreadTest
 * @date 07/03/2016
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
 * the class RealTimeThreadTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "GAM.h"
#include "GAMGroup.h"
#include "GAMSchedulerI.h"
#include "RealTimeThread.h"
#include "RealTimeThreadTest.h"
#include "StandardParser.h"

#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAMSchedulerI empty implementation to support the RealTimeThread tests
 */
class RealTimeThreadTestScheduler1: public GAMSchedulerI {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeThreadTestScheduler1    ();

    virtual void StartExecution();

    virtual void StopExecution();
};

RealTimeThreadTestScheduler1::RealTimeThreadTestScheduler1() :
        GAMSchedulerI() {

}

void RealTimeThreadTestScheduler1::StartExecution() {

}

void RealTimeThreadTestScheduler1::StopExecution() {

}

CLASS_REGISTER(RealTimeThreadTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the RealTimeThread tests
 */
class RealTimeThreadTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeThreadTestGAM1    ();

    virtual bool Execute();
};

RealTimeThreadTestGAM1::RealTimeThreadTestGAM1() :
        GAM() {
}

bool RealTimeThreadTestGAM1::Execute() {
    return true;
}
CLASS_REGISTER(RealTimeThreadTestGAM1, "1.0")

/**
 * @brief Empty object implementation to fake a GAM
 */
class RealTimeThreadTestFakeGAM1: public Object {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeThreadTestFakeGAM1    ();

    virtual bool Execute();

};

RealTimeThreadTestFakeGAM1::RealTimeThreadTestFakeGAM1() :
        Object() {
}

bool RealTimeThreadTestFakeGAM1::Execute() {
    return true;
}
CLASS_REGISTER(RealTimeThreadTestFakeGAM1, "1.0")

/**
 * @brief GAMGroup empty implementation to support the RealTimeThread tests
 */
class RealTimeThreadTestGAMGroup1: public GAMGroup {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeThreadTestGAMGroup1    ();

    virtual void PrepareNextState(const RealTimeStateInfo &status);
};

RealTimeThreadTestGAMGroup1::RealTimeThreadTestGAMGroup1() :
        GAMGroup() {
}

void RealTimeThreadTestGAMGroup1::PrepareNextState(const RealTimeStateInfo &status) {

}
CLASS_REGISTER(RealTimeThreadTestGAMGroup1, "1.0")

/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseRealTimeThreadEnviroment(const char8 * const config) {
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->CleanUp();
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

//Standard valid config with GAMs, ReferenceContainers and GAMGroups
static const char8* config1 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = RealTimeThreadTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = RealTimeThreadTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAMGroup1 = {"
        "            Class = RealTimeThreadTestGAMGroup1"
        "            +GAM3 = {"
        "                Class = RealTimeThreadTestGAM1"
        "                OutputSignals = {"
        "                   Signal1 = {"
        "                      Type = uint32"
        "                   }"
        "                }"
        "            }"
        "            +GAM4 = {"
        "                Class = RealTimeThreadTestGAM1"
        "                OutputSignals = {"
        "                   Signal2 = {"
        "                      Type = uint32"
        "                   }"
        "                }"
        "            }"
        "        }"
        "        +GAMContainer = {"
        "            Class = ReferenceContainer"
        "            +GAM5 = {"
        "                Class = RealTimeThreadTestGAM1"
        "                InputSignals = {"
        "                   Signal1 = {"
        "                      Type = uint32"
        "                   }"
        "                   Signal2 = {"
        "                      Type = uint32"
        "                   }"
        "                }"
        "            }"
        "            +GAM6 = {"
        "                Class = RealTimeThreadTestGAM1"
        "                InputSignals = {"
        "                   Signal1 = {"
        "                      Type = uint32"
        "                   }"
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
        "                    Functions = { :Functions.GAM1 :Functions.GAM2 }"
        "                }"
        "                +Thread2 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { :Functions.GAMGroup1 }"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { :Functions.GAM1 :Functions.GAM2 }"
        "                }"
        "                +Thread2 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { :Functions.GAMGroup1 :Functions.GAMContainer }"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        DefaultDataSource = DDB1"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = RealTimeThreadTestScheduler1"
        "    }"
        "}";

//Invalid configuration trying to link to GAM which does not live inside +Functions
static const char8* config2 = ""
        "GAM0 = {"
        "   Class = RealTimeThreadTestGAM1"
        "   InputSignals = {"
        "      Signal0 = {"
        "         Type = uint32"
        "      }"
        "   }"
        "}"
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = RealTimeThreadTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = RealTimeThreadTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
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
        "                    Functions = { ::GAM0 :Functions.GAM1 :Functions.GAM2 }"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        DefaultDataSource = DDB1"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = RealTimeThreadTestScheduler1"
        "    }"
        "}";

//Invalid configuration trying to link a GAM which is not of type GAM
static const char8* config3 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = RealTimeThreadTestFakeGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = RealTimeThreadTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
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
        "                    Functions = { :Functions.GAM1 :Functions.GAM2 }"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        DefaultDataSource = DDB1"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = RealTimeThreadTestScheduler1"
        "    }"
        "}";

//Invalid configuration trying to link a GAM with the wrong path
static const char8* config4 = ""
        "$Application1 = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAM1 = {"
        "            Class = RealTimeThreadTestGAM1"
        "            OutputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
        "            }"
        "        }"
        "        +GAM2 = {"
        "            Class = RealTimeThreadTestGAM1"
        "            InputSignals = {"
        "               Signal0 = {"
        "                   Type = uint32"
        "               }"
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
        "                    Functions = { :Functions.GAM1 :Functions.GAM2a }"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Data = {"
        "        DefaultDataSource = DDB1"
        "        Class = ReferenceContainer"
        "        +DDB1 = {"
        "            Class = GAMDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = RealTimeThreadTestScheduler1"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeThreadTest::RealTimeThreadTest() {
}

RealTimeThreadTest::~RealTimeThreadTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeThreadTest::TestConstructor() {

    RealTimeThread test;
    bool ret = (test.GetNumberOfFunctions() != 0);
    if (ret) {
        ret = (test.GetFunctions() == NULL);
    }
    if (ret) {
        ret = (test.GetNumberOfGAMs() != 0);
    }
    if (ret) {
        ReferenceContainer gamList;
        test.GetGAMs(gamList);
        ret = (gamList.Size() == 0u);
    }
    if (ret) {
        ret = (test.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    }
    if (ret) {
        ret = (test.GetCPU() == ProcessorType::GetDefaultCPUs());
    }
    if (ret) {
        ret = (test.Size() == 0);
    }
    return test.Size() == 0;
}

bool RealTimeThreadTest::TestInitialise() {

    ConfigurationDatabase cdb;

    const char8 * functions[3] = { "a", "b", "c" };
    cdb.Write("Functions", functions);

    RealTimeThread test;
    if (!test.Initialise(cdb)) {
        return false;
    }

    StreamString *functionRet = test.GetFunctions();
    if (functionRet[0] != "a") {
        return false;
    }
    if (functionRet[1] != "b") {
        return false;
    }

    if (functionRet[2] != "c") {
        return false;
    }

    return test.GetNumberOfFunctions() == 3;

}

bool RealTimeThreadTest::TestInitialiseFalse_NoFunctions() {
    ConfigurationDatabase cdb;

    RealTimeThread test;
    return (!test.Initialise(cdb));
}

bool RealTimeThreadTest::TestConfigureArchitecture() {
    bool ret = InitialiseRealTimeThreadEnviroment(config1);
    ReferenceT<RealTimeThread> thread1S1;
    ReferenceT<RealTimeThread> thread2S1;
    ReferenceT<RealTimeThread> thread1S2;
    ReferenceT<RealTimeThread> thread2S2;
    if (ret) {
        thread1S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
        ret = thread1S1.IsValid();
    }
    if (ret) {
        thread2S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
        ret = thread2S1.IsValid();
    }
    if (ret) {
        thread1S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
        ret = thread1S2.IsValid();
    }
    if (ret) {
        thread2S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");
        ret = thread2S2.IsValid();
    }
    return ret;
}

bool RealTimeThreadTest::TestConfigureArchitecture_OrphanGAM() {
    bool ret = InitialiseRealTimeThreadEnviroment(config2);
    return !ret;
}

bool RealTimeThreadTest::TestConfigureArchitecture_InvalidGAMType() {
    bool ret = InitialiseRealTimeThreadEnviroment(config3);
    return !ret;
}

bool RealTimeThreadTest::TestConfigureArchitecture_InvalidGAMPath() {
    bool ret = InitialiseRealTimeThreadEnviroment(config4);
    return !ret;
}

bool RealTimeThreadTest::TestGetNumberOfFunctions() {
    bool ret = InitialiseRealTimeThreadEnviroment(config1);
    ReferenceT<RealTimeThread> thread1S1;
    ReferenceT<RealTimeThread> thread2S1;
    ReferenceT<RealTimeThread> thread1S2;
    ReferenceT<RealTimeThread> thread2S2;
    if (ret) {
        thread1S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
        ret = thread1S1.IsValid();
    }
    if (ret) {
        thread2S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
        ret = thread2S1.IsValid();
    }
    if (ret) {
        thread1S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
        ret = thread1S2.IsValid();
    }
    if (ret) {
        thread2S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");
        ret = thread2S2.IsValid();
    }

    const uint32 numberOfThreads = 4;
    ReferenceT<RealTimeThread> threads[numberOfThreads] = { thread1S1, thread2S1, thread1S2, thread2S2 };
    uint32 numberOfFunctions[numberOfThreads] = { 2, 1, 2, 2 };

    uint32 n;
    for (n = 0u; (n < numberOfThreads) && (ret); n++) {
        ret = (threads[n]->GetNumberOfFunctions() == numberOfFunctions[n]);
    }

    return ret;
}

bool RealTimeThreadTest::TestGetFunctions() {
    bool ret = InitialiseRealTimeThreadEnviroment(config1);
    ReferenceT<RealTimeThread> thread1S1;
    ReferenceT<RealTimeThread> thread2S1;
    ReferenceT<RealTimeThread> thread1S2;
    ReferenceT<RealTimeThread> thread2S2;
    if (ret) {
        thread1S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
        ret = thread1S1.IsValid();
    }
    if (ret) {
        thread2S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
        ret = thread2S1.IsValid();
    }
    if (ret) {
        thread1S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
        ret = thread1S2.IsValid();
    }
    if (ret) {
        thread2S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");
        ret = thread2S2.IsValid();
    }

    const uint32 numberOfThreads = 4;
    const uint32 maxFunctions = 2;
    ReferenceT<RealTimeThread> threads[numberOfThreads] = { thread1S1, thread2S1, thread1S2, thread2S2 };
    const char8 * const functionNames[numberOfThreads][maxFunctions] = { { ":Functions.GAM1", ":Functions.GAM2" }, { ":Functions.GAMGroup1", NULL }, {
            ":Functions.GAM1", ":Functions.GAM2" }, { ":Functions.GAMGroup1", ":Functions.GAMContainer" } };

    uint32 n;
    for (n = 0u; (n < numberOfThreads) && (ret); n++) {
        uint32 numberOfFunctions = threads[n]->GetNumberOfFunctions();
        StreamString *thisThreadFunctionNames = threads[n]->GetFunctions();
        uint32 i;
        for (i = 0u; i < numberOfFunctions; i++) {
            ret = (thisThreadFunctionNames[i] == functionNames[n][i]);
        }
    }

    return ret;
}

bool RealTimeThreadTest::TestGetNumberOfGAMs() {
    bool ret = InitialiseRealTimeThreadEnviroment(config1);
    ReferenceT<RealTimeThread> thread1S1;
    ReferenceT<RealTimeThread> thread2S1;
    ReferenceT<RealTimeThread> thread1S2;
    ReferenceT<RealTimeThread> thread2S2;
    if (ret) {
        thread1S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
        ret = thread1S1.IsValid();
    }
    if (ret) {
        thread2S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
        ret = thread2S1.IsValid();
    }
    if (ret) {
        thread1S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
        ret = thread1S2.IsValid();
    }
    if (ret) {
        thread2S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");
        ret = thread2S2.IsValid();
    }

    const uint32 numberOfThreads = 4;
    ReferenceT<RealTimeThread> threads[numberOfThreads] = { thread1S1, thread2S1, thread1S2, thread2S2 };
    uint32 numberOfGAMs[numberOfThreads] = { 2, 2, 2, 4 };

    uint32 n;
    for (n = 0u; (n < numberOfThreads) && (ret); n++) {
        ret = (threads[n]->GetNumberOfGAMs() == numberOfGAMs[n]);
    }

    return ret;
}

bool RealTimeThreadTest::TestGetGAMs() {
    bool ret = InitialiseRealTimeThreadEnviroment(config1);
    ReferenceT<RealTimeThread> thread1S1;
    ReferenceT<RealTimeThread> thread2S1;
    ReferenceT<RealTimeThread> thread1S2;
    ReferenceT<RealTimeThread> thread2S2;
    if (ret) {
        thread1S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
        ret = thread1S1.IsValid();
    }
    if (ret) {
        thread2S1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
        ret = thread2S1.IsValid();
    }
    if (ret) {
        thread1S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
        ret = thread1S2.IsValid();
    }
    if (ret) {
        thread2S2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");
        ret = thread2S2.IsValid();
    }

    const uint32 numberOfThreads = 4;
    const uint32 maxGAMs = 4;
    ReferenceT<RealTimeThread> threads[numberOfThreads] = { thread1S1, thread2S1, thread1S2, thread2S2 };
    const char8 * const gamNames[numberOfThreads][maxGAMs] = { { "GAM1", "GAM2", NULL, NULL }, { "GAM3", "GAM4", NULL, NULL }, { "GAM1", "GAM2", NULL, NULL }, {
            "GAM3", "GAM4", "GAM5", "GAM6" } };

    uint32 n;
    for (n = 0u; (n < numberOfThreads) && (ret); n++) {
        ReferenceContainer gamList;
        threads[n]->GetGAMs(gamList);
        uint32 i;
        for (i = 0u; i < gamList.Size(); i++) {
            StreamString name = gamList.Get(i)->GetName();
            name.Seek(0u);
            ret = (name == gamNames[n][i]);
        }
    }

    return ret;
}

#if 0
bool RealTimeThreadTest::TestConfigureArchitecture_MoreGAMs() {
    ConfigurationDatabase cdb;
    StreamString conf =
    "$Application1 = {\n"
    "    Class = RealTimeApplication\n"
    "    +Functions = {\n"
    "        Class = ReferenceContainer\n"
    "        +GAM1 = {\n"
    "            Class = PIDGAM\n"
    "        }\n"
    "        +GAM2 = {\n"
    "            Class = PIDGAM\n"
    "        }\n"
    "        +PIDGroup1 = {\n"
    "            Class = PIDGAMGroup\n"
    "            +GAM3 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "            +GAM4 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "        }\n"
    "        +GAMContainer = {\n"
    "            Class = ReferenceContainer\n"
    "            +GAM5 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "            +GAM6 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "        }\n"
    "        +PIDGroup2 = {\n"
    "            Class = PIDGAMGroup\n"
    "            +GAM7 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "            +GAM8 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +States = {\n"
    "        Class = ReferenceContainer\n"
    "        +State1 = {\n"
    "            Class = RealTimeState\n"
    "            +Threads = {\n"
    "                Class = ReferenceContainer\n"
    "                +Thread1 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {:Functions.GAM1 :Functions.GAM2 :Functions.PIDGroup1 :Functions.GAMContainer :Functions.PIDGroup2.GAM7 :Functions.PIDGroup2.GAM8}\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +Data = {\n"
    "        Class = DataSourceContainer\n"
    "    }\n"
    "    +Scheduler = {\n"
    "        Class = DummyScheduler\n"
    "    }\n"
    "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    if (thread1->GetNumberOfGAMs() != 8) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    return true;
}

bool RealTimeThreadTest::TestConfigureArchitecture_GAMGroup() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!thread2->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    ReferenceT<PIDGAM> gam3 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup1.GAM3");
    ReferenceT<PIDGAM> gam4 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup1.GAM4");

    if (gam3->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam3->GetSupportedStates()[0] != "State1") {
        return false;
    }
    if (gam4->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam4->GetSupportedStates()[0] != "State1") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestConfigureArchitecture_ReferenceContainer() {

    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");

    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!thread3->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    ReferenceT<PIDGAM> gam5 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMContainer.GAM5");
    ReferenceT<PIDGAM> gam6 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMContainer.GAM6");

    if (gam5->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam5->GetSupportedStates()[0] != "State2") {
        return false;
    }
    if (gam6->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam6->GetSupportedStates()[0] != "State2") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestConfigureArchitecture_SingleInGAMGroup() {

    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");

    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!thread4->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    ReferenceT<PIDGAM> gam7 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup2.GAM7");
    ReferenceT<PIDGAM> gam8 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup2.GAM8");

    if (gam7->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam7->GetSupportedStates()[0] != "State2") {
        return false;
    }
    if (gam8->GetNumberOfSupportedStates() != 1) {
        return false;
    }

    if (gam8->GetSupportedStates()[0] != "State2") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestConfigureArchitectureFalse_OrphanGAM() {
    ConfigurationDatabase cdb;

    // application
    StreamString conf = ""
    "+GAM1 = {\n"
    "    Class = PIDGAM\n"
    "}\n"
    "$Application1 = {\n"
    "    Class = RealTimeApplication\n"
    "    +Functions = {\n"
    "        Class = ReferenceContainer\n"
    "        +GAM2 = {\n"
    "            Class = PIDGAM\n"
    "        }\n"
    "    }\n"
    "    +States = {\n"
    "        Class = ReferenceContainer\n"
    "        +State1 = {\n"
    "            Class = RealTimeState\n"
    "            +Threads = {\n"
    "                Class = ReferenceContainer\n"
    "                +Thread1 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {::GAM1 :Functions.GAM2 }\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +Data = {\n"
    "        Class = DataSourceContainer\n"
    "    }\n"
    "    +Scheduler = {\n"
    "        Class = DummyScheduler\n"
    "    }\n"
    "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    printf("\nsize ordb= %d\n", ObjectRegistryDatabase::Instance()->Size());
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    return true;
}

bool RealTimeThreadTest::TestConfigureArchitectureFalse_InvalidGAMType() {
    ConfigurationDatabase cdb;

    StreamString conf = ""

    "$Application1 = {\n"
    "    Class = RealTimeApplication\n"
    "    +Functions = {\n"
    "        Class = ReferenceContainer\n"
    "        +GAM1 = {\n"
    "            Class = PIDGAM\n"
    "        }\n"
    "        +GAM2 = {\n"
    "            Class = Object\n"
    "        }\n"
    "    }\n"
    "    +States = {\n"
    "        Class = ReferenceContainer\n"
    "        +State1 = {\n"
    "            Class = RealTimeState\n"
    "            +Threads = {\n"
    "                Class = ReferenceContainer\n"
    "                +Thread1 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {::GAM1 :Functions.GAM2 }\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +Data = {\n"
    "        Class = DataSourceContainer\n"
    "    }\n"
    "    +Scheduler = {\n"
    "        Class = DummyScheduler\n"
    "    }\n"
    "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();

    return true;

}

bool RealTimeThreadTest::TestConfigureArchitectureFalse_InvalidGAMPath() {
    ConfigurationDatabase cdb;
    StreamString conf =
    "$Application1 = {\n"
    "    Class = RealTimeApplication\n"
    "    +Functions = {\n"
    "        Class = ReferenceContainer\n"
    "        +GAM1 = {\n"
    "            Class = PIDGAM\n"
    "        }\n"
    "        +GAM2 = {\n"
    "            Class = PIDGAM\n"
    "        }\n"
    "        +PIDGroup1 = {\n"
    "            Class = PIDGAMGroup\n"
    "            +GAM3 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "            +GAM4 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "        }\n"
    "        +GAMContainer = {\n"
    "            Class = ReferenceContainer\n"
    "            +GAM5 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "            +GAM6 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "        }\n"
    "        +PIDGroup2 = {\n"
    "            Class = PIDGAMGroup\n"
    "            +GAM7 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "            +GAM8 = {\n"
    "                Class = PIDGAM\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +States = {\n"
    "        Class = ReferenceContainer\n"
    "        +State1 = {\n"
    "            Class = RealTimeState\n"
    "            +Threads = {\n"
    "                Class = ReferenceContainer\n"
    "                +Thread1 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {:Functions.GAM1 :Functions.GAM2 :Functions.PIDGroup1 :Functions.GAMContainer :Functions.PIDGroup2.GAM7 :Functions.PIDGroup3.GAM8}\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +Data = {\n"
    "        Class = DataSourceContainer\n"
    "    }\n"
    "    +Scheduler = {\n"
    "        Class = DummyScheduler\n"
    "    }\n"
    "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    return true;
}

bool RealTimeThreadTest::TestGetNumberOfFunctions() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");

    if (thread1->GetNumberOfFunctions() != 2) {
        return false;
    }
    if (thread2->GetNumberOfFunctions() != 1) {
        return false;
    }
    if (thread3->GetNumberOfFunctions() != 1) {
        return false;
    }
    if (thread4->GetNumberOfFunctions() != 2) {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestGetFunctions() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");

    if (thread1->GetFunctions()[0] != ":Functions.GAM1") {
        return false;
    }
    if (thread1->GetFunctions()[1] != ":Functions.GAM2") {
        return false;
    }
    if (thread2->GetFunctions()[0] != ":Functions.PIDGroup1") {
        return false;
    }
    if (thread3->GetFunctions()[0] != ":Functions.GAMContainer") {
        return false;
    }
    if (thread4->GetFunctions()[0] != ":Functions.PIDGroup2.GAM7") {
        return false;
    }
    if (thread4->GetFunctions()[1] != ":Functions.PIDGroup2.GAM8") {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestGetNumberOfGAMs() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");
    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2");
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    if (!thread2->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    if (!thread3->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    if (!thread4->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    if (thread1->GetNumberOfGAMs() != 2) {
        return false;
    }
    if (thread2->GetNumberOfGAMs() != 2) {
        return false;
    }
    if (thread3->GetNumberOfGAMs() != 2) {
        return false;
    }
    if (thread4->GetNumberOfGAMs() != 2) {
        return false;
    }

    return true;
}

bool RealTimeThreadTest::TestGetGAMs() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ReferenceT<RealTimeThread> thread1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");
    ReferenceT<RealTimeThread> thread3 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread1");
    ReferenceT<RealTimeThread> thread4 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2.Threads.Thread2");

    ReferenceT<PIDGAM> gam1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAM1");
    ReferenceT<PIDGAM> gam2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAM2");

    ReferenceT<PIDGAM> gam3 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup1.GAM3");
    ReferenceT<PIDGAM> gam4 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup1.GAM4");

    ReferenceT<PIDGAM> gam5 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMContainer.GAM5");
    ReferenceT<PIDGAM> gam6 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.GAMContainer.GAM6");

    ReferenceT<PIDGAM> gam7 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup2.GAM7");
    ReferenceT<PIDGAM> gam8 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup2.GAM8");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");
    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2");
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!thread1->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }
    if (!thread2->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    if (!thread3->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    if (!thread4->ConfigureArchitecture(*app.operator->(), *state2.operator->())) {
        return false;
    }

    if (thread1->GetGAMs()[0] != gam1) {
        return false;
    }
    if (thread1->GetGAMs()[1] != gam2) {
        return false;
    }
    if (thread2->GetGAMs()[0] != gam3) {
        return false;
    }
    if (thread2->GetGAMs()[1] != gam4) {
        return false;
    }
    if (thread3->GetGAMs()[0] != gam5) {
        return false;
    }
    if (thread3->GetGAMs()[1] != gam6) {
        return false;
    }
    if (thread4->GetGAMs()[0] != gam7) {
        return false;
    }
    if (thread4->GetGAMs()[1] != gam8) {
        return false;
    }
    return true;
}

bool RealTimeThreadTest::TestGetStackSize() {

    ConfigurationDatabase tcdb;
    tcdb.Write("Class", "RealTimeThread");
    const char8 *functions[1] = {"??"};
    tcdb.Write("Functions", functions);
    tcdb.Write("StackSize", "12345");
    tcdb.MoveToRoot();

    RealTimeThread thread;
    if (thread.GetStackSize() != THREADS_DEFAULT_STACKSIZE) {
        return false;
    }

    if (!thread.Initialise(tcdb)) {
        return false;
    }

    return thread.GetStackSize() == 12345;

}

bool RealTimeThreadTest::TestGetCPU() {
    ConfigurationDatabase tcdb;
    tcdb.Write("Class", "RealTimeThread");
    const char8 *functions[1] = {"??"};
    tcdb.Write("Functions", functions);
    tcdb.Write("CPUs", "0xfeff");
    tcdb.MoveToRoot();

    RealTimeThread thread;
    if (thread.GetCPU() != ProcessorType::GetDefaultCPUs()) {
        return false;
    }

    if (!thread.Initialise(tcdb)) {
        return false;
    }

    return thread.GetCPU() == 0xfeff;
}

bool RealTimeThreadTest::TestToStructuredData() {
    ConfigurationDatabase cdb;
    StreamString conf = cdbStr1;
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ReferenceT<RealTimeThread> thread2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread2");

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");
    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2");
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");
    if (!thread2->ConfigureArchitecture(*app.operator->(), *state1.operator->())) {
        return false;
    }

    ConfigurationDatabase outCDB;

    thread2->ToStructuredData(outCDB);
    StreamString display;
    display.Printf("%!", outCDB);

    printf("\n%s\n", display.Buffer());

    StreamString test = "+Thread2 = {\n"
    "Class = \"RealTimeThread\"\n"
    "Functions = { \":Functions.PIDGroup1\" } \n"
    "+PIDGroup1 = {\n"
    "Class = \"PIDGAMGroup\"\n"
    "+GAM3 = {\n"
    "Class = \"PIDGAM\"\n"
    "}\n"
    "+GAM4 = {\n"
    "Class = \"PIDGAM\"\n"
    "}\n"
    "}\n"
    "}\n";

    return test == display;
}

bool RealTimeThreadTest::TestValidateDataSourceLinks() {
    ConfigurationDatabase cdb;
    StreamString conf = ""
    "$Application1 = {\n"
    "    Class = RealTimeApplication\n"
    "    +Functions = {\n"
    "        Class = ReferenceContainer\n"
    "        +GAM1 = {\n"
    "            Class = DummyGAM\n"
    "            +Input = {\n"
    "                Class = GAMSignalsContainer\n"
    "                IsInput = true\n"
    "                IsFinal = true\n"
    "                +Counter = {\n"
    "                    Class = GAMGenericSignal\n"
    "                    Type = uint32\n"
    "                    Default = 0\n"
    "                    Path = DDB.Counter1\n"
    "                    IsFinal = true\n"
    "                }\n"
    "            }\n"
    "            +Output = {\n"
    "                Class = GAMSignalsContainer\n"
    "                IsOutput = true\n"
    "                IsFinal = true\n"
    "                +Counter = {\n"
    "                    Class = GAMGenericSignal\n"
    "                    Type = uint32\n"
    "                    Default = 0\n"
    "                    Path = DDB.Counter2\n"
    "                    IsFinal = true\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "        +GAM2 = {\n"
    "            Class = DummyGAM\n"
    "            +Input = {\n"
    "                Class = GAMSignalsContainer\n"
    "                IsInput = true\n"
    "                IsFinal = true\n"
    "                +Counter = {\n"
    "                    Class = GAMGenericSignal\n"
    "                    Type = uint32\n"
    "                    Default = 0\n"
    "                    Path = DDB.Counter2\n"
    "                    IsFinal = true\n"
    "                }\n"
    "            }\n"
    "            +Output = {\n"
    "                Class = GAMSignalsContainer\n"
    "                IsOutput = true\n"
    "                IsFinal = true\n"
    "                +Counter = {\n"
    "                    Class = GAMGenericSignal\n"
    "                    Type = uint32\n"
    "                    Default = 0\n"
    "                    Path = DDB.Counter1\n"
    "                    IsFinal = true\n"
    "                    Cycles = 1" // the only one sync
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +States = {\n"
    "        Class = ReferenceContainer\n"
    "        +State1 = {\n"
    "            Class = RealTimeState\n"
    "            +Threads = {\n"
    "                Class = ReferenceContainer\n"
    "                +Thread1 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {:Functions.GAM1 :Functions.GAM2}\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +Data = {\n"
    "        Class = DataSourceContainer\n"
    "        +DDB = {\n"
    "            Class = DataSource\n"
    "        }\n"
    "    }\n"
    "    +Scheduler = {\n"
    "        Class = DummyScheduler\n"
    "    }\n"
    "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    ReferenceT<RealTimeThread> thread = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");

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

    bool ret = thread->ValidateDataSourceLinks();
    ObjectRegistryDatabase::Instance()->CleanUp();
    return ret;
}

bool RealTimeThreadTest::TestValidateDataSourceLinksFalse_MoreSync() {
    ConfigurationDatabase cdb;
    StreamString conf = ""
    "$Application1 = {\n"
    "    Class = RealTimeApplication\n"
    "    +Functions = {\n"
    "        Class = ReferenceContainer\n"
    "        +GAM1 = {\n"
    "            Class = DummyGAM\n"
    "            +Input = {\n"
    "                Class = GAMSignalsContainer\n"
    "                IsInput = true\n"
    "                IsFinal = true\n"
    "                +Counter = {\n"
    "                    Class = GAMGenericSignal\n"
    "                    Type = uint32\n"
    "                    Default = 0\n"
    "                    Path = DDB.Counter1\n"
    "                    IsFinal = true\n"
    "                    Cycles = 2\n"
    "                }\n"
    "            }\n"
    "            +Output = {\n"
    "                Class = GAMSignalsContainer\n"
    "                IsOutput = true\n"
    "                IsFinal = true\n"
    "                +Counter = {\n"
    "                    Class = GAMGenericSignal\n"
    "                    Type = uint32\n"
    "                    Default = 0\n"
    "                    Path = DDB.Counter2\n"
    "                    IsFinal = true\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "        +GAM2 = {\n"
    "            Class = DummyGAM\n"
    "            +Input = {\n"
    "                Class = GAMSignalsContainer\n"
    "                IsInput = true\n"
    "                IsFinal = true\n"
    "                +Counter = {\n"
    "                    Class = GAMGenericSignal\n"
    "                    Type = uint32\n"
    "                    Default = 0\n"
    "                    Path = DDB.Counter2\n"
    "                    IsFinal = true\n"
    "                }\n"
    "            }\n"
    "            +Output = {\n"
    "                Class = GAMSignalsContainer\n"
    "                IsOutput = true\n"
    "                IsFinal = true\n"
    "                +Counter = {\n"
    "                    Class = GAMGenericSignal\n"
    "                    Type = uint32\n"
    "                    Default = 0\n"
    "                    Path = DDB.Counter1\n"
    "                    IsFinal = true\n"
    "                    Cycles = 1"
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +States = {\n"
    "        Class = ReferenceContainer\n"
    "        +State1 = {\n"
    "            Class = RealTimeState\n"
    "            +Threads = {\n"
    "                Class = ReferenceContainer\n"
    "                +Thread1 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {:Functions.GAM1 :Functions.GAM2}\n"
    "                }\n"
    "            }\n"
    "        }\n"
    "    }\n"
    "    +Data = {\n"
    "        Class = DataSourceContainer\n"
    "        +DDB = {\n"
    "            Class = DataSource\n"
    "        }\n"
    "    }\n"
    "    +Scheduler = {\n"
    "        Class = DummyScheduler\n"
    "    }\n"
    "}\n";
    conf.Seek(0);
    StandardParser parser(conf, cdb);
    if (!parser.Parse()) {
        return false;
    }
    ObjectRegistryDatabase::Instance()->CleanUp();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    ReferenceT<RealTimeThread> thread = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1.Threads.Thread1");

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

    bool ret = !thread->ValidateDataSourceLinks();
    ObjectRegistryDatabase::Instance()->CleanUp();
    return ret;
}
#endif
