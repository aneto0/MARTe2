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

    virtual MARTe::ErrorManagement::ErrorType StartNextStateExecution();

    virtual MARTe::ErrorManagement::ErrorType StopCurrentStateExecution();

    virtual void CustomPrepareNextState(){

    }

};

RealTimeThreadTestScheduler1::RealTimeThreadTestScheduler1() :
        GAMSchedulerI() {

}

MARTe::ErrorManagement::ErrorType RealTimeThreadTestScheduler1::StartNextStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

MARTe::ErrorManagement::ErrorType RealTimeThreadTestScheduler1::StopCurrentStateExecution() {
    return MARTe::ErrorManagement::NoError;
}

CLASS_REGISTER(RealTimeThreadTestScheduler1, "1.0")

/**
 * @brief GAM empty implementation to support the RealTimeThread tests
 */
class RealTimeThreadTestGAM1: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeThreadTestGAM1    ();

    virtual bool Setup();

    virtual bool Execute();
};

RealTimeThreadTestGAM1::RealTimeThreadTestGAM1() :
        GAM() {
}

bool RealTimeThreadTestGAM1::Setup() {
    return true;
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

    virtual bool PrepareNextState(const char8 * currentStateName, const char8 * nextStateName);
};

RealTimeThreadTestGAMGroup1::RealTimeThreadTestGAMGroup1() :
        GAMGroup() {
}

bool RealTimeThreadTestGAMGroup1::PrepareNextState(const char8 * currentStateName, const char8 * nextStateName) {
    return true;
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
        "                    Functions = { GAM1 GAM2 }"
        "                }"
        "                +Thread2 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { GAMGroup1 }"
        "                }"
        "            }"
        "        }"
        "        +State2 = {"
        "            Class = RealTimeState"
        "            +Threads = {"
        "                Class = ReferenceContainer"
        "                +Thread1 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { GAM1 GAM2 }"
        "                }"
        "                +Thread2 = {"
        "                    Class = RealTimeThread"
        "                    Functions = { GAMGroup1 GAMContainer }"
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
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
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
        "                    Functions = { ::GAM0 GAM1 GAM2 }"
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
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
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
        "                    Functions = { GAM1 GAM2 }"
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
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
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
        "                    Functions = { GAM1 GAM2a }"
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
        "        +Timings = {"
        "            Class = TimingDataSource"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        TimingDataSource = Timings"
        "        Class = RealTimeThreadTestScheduler1"
        "    }"
        "}";
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeThreadTest::RealTimeThreadTest() {
}

RealTimeThreadTest::~RealTimeThreadTest() {
    ObjectRegistryDatabase::Instance()->Purge();
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
    const char8 * const functionNames[numberOfThreads][maxFunctions] = { { "GAM1", "GAM2" }, { "GAMGroup1", NULL }, {
            "GAM1", "GAM2" }, { "GAMGroup1", "GAMContainer" } };

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
    const char8 * const gamNames[numberOfThreads][maxGAMs] = { { "GAM1", "GAM2", NULL, NULL }, { "GAM3", "GAM4", NULL,
    NULL }, { "GAM1", "GAM2", NULL, NULL }, { "GAM3", "GAM4", "GAM5", "GAM6" } };

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

bool RealTimeThreadTest::TestGetStackSize() {

    ConfigurationDatabase tcdb;
    tcdb.Write("Class", "RealTimeThread");
    const char8 *functions[1] = { "??" };
    tcdb.Write("Functions", functions);
    tcdb.Write("StackSize", "12345");
    tcdb.MoveToRoot();

    RealTimeThread thread;
    bool ok = (thread.GetStackSize() == THREADS_DEFAULT_STACKSIZE);
    if (ok) {
        ok = thread.Initialise(tcdb);
    }
    if (ok) {
        ok = (thread.GetStackSize() == 12345);
    }
    return ok;
}

bool RealTimeThreadTest::TestGetCPU() {
    ConfigurationDatabase tcdb;
    tcdb.Write("Class", "RealTimeThread");
    const char8 *functions[1] = { "??" };
    tcdb.Write("Functions", functions);
    tcdb.Write("CPUs", "0xfeff");
    tcdb.MoveToRoot();

    RealTimeThread thread;
    bool ok = (thread.GetCPU() == ProcessorType::GetDefaultCPUs());
    if (ok) {
        ok = thread.Initialise(tcdb);
    }
    if (ok) {
        ok = (thread.GetCPU() == 0xfeffu);
    }

    return ok;
}

bool RealTimeThreadTest::TestToStructuredData() {
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
    if (ret) {
        ConfigurationDatabase printedStructuredData;
        ret = thread1S1->ToStructuredData(printedStructuredData);
        if (ret) {
            StreamString value;
            ret = printedStructuredData.MoveToRoot();
            ret &= printedStructuredData.MoveAbsolute("+Thread1");
            ret &= printedStructuredData.Read("Class", value);
            ret &= (value == "RealTimeThread");
            value = "";
            AnyType functionsArray = printedStructuredData.GetType("Functions");
            if (ret) {
                ret &= (functionsArray.GetDataPointer() != NULL);
            }
            uint32 numberOfFunctions = 0u;
            if (ret) {
                numberOfFunctions = functionsArray.GetNumberOfElements(0u);
                ret &= (numberOfFunctions == 2u);
            }
            if (ret) {
                StreamString *functions = new StreamString[numberOfFunctions];
                Vector<StreamString> functionVector(functions, numberOfFunctions);
                ret &= (printedStructuredData.Read("Functions", functionVector));
                ret &= (functions[0] == "GAM1");
                ret &= (functions[1] == "GAM2");
                delete[] functions;
            }
            uint32 numberOfGAMs = 0u;
            if (ret) {
                numberOfGAMs = thread1S1->GetNumberOfGAMs();
                ret = (numberOfGAMs == 2u);
            }

            if (ret) {
                StreamString *gams = new StreamString[numberOfGAMs];
                Vector<StreamString> gamsVector(gams, numberOfGAMs);
                ret &= (printedStructuredData.Read("GAMs", gamsVector));
                ret &= (gams[0] == "GAM1");
                ret &= (gams[1] == "GAM2");
                delete[] gams;
            }
        }
    }
    if (ret) {
        ConfigurationDatabase printedStructuredData;
        ret = thread2S1->ToStructuredData(printedStructuredData);
        if (ret) {
            StreamString value;
            ret = printedStructuredData.MoveToRoot();
            ret &= printedStructuredData.MoveAbsolute("+Thread2");
            ret &= printedStructuredData.Read("Class", value);
            ret &= (value == "RealTimeThread");
            value = "";
            AnyType functionsArray = printedStructuredData.GetType("Functions");
            if (ret) {
                ret &= (functionsArray.GetDataPointer() != NULL);
            }
            uint32 numberOfFunctions = 0u;
            if (ret) {
                numberOfFunctions = functionsArray.GetNumberOfElements(0u);
                ret &= (numberOfFunctions == 1u);
            }
            if (ret) {
                StreamString *functions = new StreamString[numberOfFunctions];
                Vector<StreamString> functionVector(functions, numberOfFunctions);
                ret &= (printedStructuredData.Read("Functions", functionVector));
                ret &= (functions[0] == "GAMGroup1");
                delete[] functions;
            }
            uint32 numberOfGAMs = 0u;
            if (ret) {
                numberOfGAMs = thread2S1->GetNumberOfGAMs();
                ret = (numberOfGAMs == 2u);
            }

            if (ret) {
                StreamString *gams = new StreamString[numberOfGAMs];
                Vector<StreamString> gamsVector(gams, numberOfGAMs);
                ret &= (printedStructuredData.Read("GAMs", gamsVector));
                ret &= (gams[0] == "GAM3");
                ret &= (gams[1] == "GAM4");
                delete[] gams;
            }
        }
    }
    if (ret) {
        ConfigurationDatabase printedStructuredData;
        ret = thread1S2->ToStructuredData(printedStructuredData);
        if (ret) {
            StreamString value;
            ret = printedStructuredData.MoveToRoot();
            ret &= printedStructuredData.MoveAbsolute("+Thread1");
            ret &= printedStructuredData.Read("Class", value);
            ret &= (value == "RealTimeThread");
            value = "";
            AnyType functionsArray = printedStructuredData.GetType("Functions");
            if (ret) {
                ret &= (functionsArray.GetDataPointer() != NULL);
            }
            uint32 numberOfFunctions = 0u;
            if (ret) {
                numberOfFunctions = functionsArray.GetNumberOfElements(0u);
                ret &= (numberOfFunctions == 2u);
            }
            if (ret) {
                StreamString *functions = new StreamString[numberOfFunctions];
                Vector<StreamString> functionVector(functions, numberOfFunctions);
                ret &= (printedStructuredData.Read("Functions", functionVector));
                ret &= (functions[0] == "GAM1");
                ret &= (functions[1] == "GAM2");
                delete[] functions;
            }
            uint32 numberOfGAMs = 0u;
            if (ret) {
                numberOfGAMs = thread1S2->GetNumberOfGAMs();
                ret = (numberOfGAMs == 2u);
            }

            if (ret) {
                StreamString *gams = new StreamString[numberOfGAMs];
                Vector<StreamString> gamsVector(gams, numberOfGAMs);
                ret &= (printedStructuredData.Read("GAMs", gamsVector));
                ret &= (gams[0] == "GAM1");
                ret &= (gams[1] == "GAM2");
                delete[] gams;
            }
        }
    }
    if (ret) {
        ConfigurationDatabase printedStructuredData;
        ret = thread2S2->ToStructuredData(printedStructuredData);
        if (ret) {
            StreamString value;
            ret = printedStructuredData.MoveToRoot();
            ret &= printedStructuredData.MoveAbsolute("+Thread2");
            ret &= printedStructuredData.Read("Class", value);
            ret &= (value == "RealTimeThread");
            value = "";
            AnyType functionsArray = printedStructuredData.GetType("Functions");
            if (ret) {
                ret &= (functionsArray.GetDataPointer() != NULL);
            }
            uint32 numberOfFunctions = 0u;
            if (ret) {
                numberOfFunctions = functionsArray.GetNumberOfElements(0u);
                ret &= (numberOfFunctions == 2u);
            }
            if (ret) {
                StreamString *functions = new StreamString[numberOfFunctions];
                Vector<StreamString> functionVector(functions, numberOfFunctions);
                ret &= (printedStructuredData.Read("Functions", functionVector));
                ret &= (functions[0] == "GAMGroup1");
                ret &= (functions[1] == "GAMContainer");
                delete[] functions;
            }
            uint32 numberOfGAMs = 0u;
            if (ret) {
                numberOfGAMs = thread2S2->GetNumberOfGAMs();
                ret = (numberOfGAMs == 4u);
            }

            if (ret) {
                StreamString *gams = new StreamString[numberOfGAMs];
                Vector<StreamString> gamsVector(gams, numberOfGAMs);
                ret &= (printedStructuredData.Read("GAMs", gamsVector));
                ret &= (gams[0] == "GAM3");
                ret &= (gams[1] == "GAM4");
                ret &= (gams[2] == "GAM5");
                ret &= (gams[3] == "GAM6");
                delete[] gams;
            }
        }
    }

    return ret;
}

