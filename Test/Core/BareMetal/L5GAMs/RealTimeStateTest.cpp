/**
 * @file RealTimeStateTest.cpp
 * @brief Source file for class RealTimeStateTest
 * @date 07/mar/2016
 * @author pc
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
 * the class RealTimeStateTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeStateTest.h"
#include "GAMTestHelper.h"
#include "ReferenceContainerFilterReferencesTemplate.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class RealTimeStateTestGAMGroup: public GAMGroup {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeStateTestGAMGroup    ();

    virtual bool PrepareNextState(const char8 * currentStateName,
            const char8 * nextStateName);

};

RealTimeStateTestGAMGroup::RealTimeStateTestGAMGroup() :
        GAMGroup() {
}

bool RealTimeStateTestGAMGroup::PrepareNextState(const char8 * currentStateName,
                                                 const char8 * nextStateName) {
    return true;

}

CLASS_REGISTER(RealTimeStateTestGAMGroup, "1.0")
//////////////////
//////////////////
//////////////////
//////////////////
class RealTimeStateTestContext1: public Object {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeStateTestContext1    ();

    int32 GetProperty1() const;

    float32 GetProperty2()const;

private:
    int32 propertyToBeShared1;

    float32 propertyToBeShared2;
};

RealTimeStateTestContext1::RealTimeStateTestContext1() :
        Object() {
    propertyToBeShared1 = 1;
    propertyToBeShared2 = 2;
}

int32 RealTimeStateTestContext1::GetProperty1() const {
    return propertyToBeShared1;
}

float32 RealTimeStateTestContext1::GetProperty2() const {
    return propertyToBeShared2;
}

CLASS_REGISTER(RealTimeStateTestContext1, "1.0")

//////////////////
//////////////////
//////////////////
//////////////////

class RealTimeStateStateLessGAM: public GAM {

public:
    CLASS_REGISTER_DECLARATION()

RealTimeStateStateLessGAM    ();

    virtual bool Execute();

    virtual bool SetContext(ConstReference contextIn);

    virtual ConstReference GetContext();

private:
    ConstReference context;

};

RealTimeStateStateLessGAM::RealTimeStateStateLessGAM() :
        GAM() {
}

bool RealTimeStateStateLessGAM::Execute() {
    return true;
}

bool RealTimeStateStateLessGAM::SetContext(ConstReference contextIn) {
    context = contextIn;
    return true;
}

ConstReference RealTimeStateStateLessGAM::GetContext() {
    return context;
}

CLASS_REGISTER(RealTimeStateStateLessGAM, "1.0")

//////////////////
//////////////////
//////////////////
//////////////////

class RealTimeStateStateFulGAM: public GAM, public StatefulI {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeStateStateFulGAM    ();

    virtual bool Execute();

    virtual bool SetContext(ConstReference contextIn);

    virtual uint32 GetContext1();

    virtual ConstReference GetContext2();


    virtual bool PrepareNextState(const char8 * current, const char8 * next);

private:
    uint32 context;
    ConstReference context2;

};

RealTimeStateStateFulGAM::RealTimeStateStateFulGAM() :
        GAM() {
    context = 0u;
}

bool RealTimeStateStateFulGAM::Execute() {
    return true;
}

bool RealTimeStateStateFulGAM::SetContext(ConstReference contextIn) {
    context2 = contextIn;
    return true;
}

uint32 RealTimeStateStateFulGAM::GetContext1() {
    return context;
}

ConstReference RealTimeStateStateFulGAM::GetContext2() {
    return context2;
}


bool RealTimeStateStateFulGAM::PrepareNextState(const char8 * current,
                                           const char8 * next) {
    context++;
    return true;
}
CLASS_REGISTER(RealTimeStateStateFulGAM, "1.0")

//////////////////
//////////////////
//////////////////
//////////////////


class RealTimeStateTestScheduler: public GAMSchedulerI {
public:

    CLASS_REGISTER_DECLARATION()

RealTimeStateTestScheduler    ();

    virtual void StartExecution();

    virtual void StopExecution();

};

RealTimeStateTestScheduler::RealTimeStateTestScheduler() :
        GAMSchedulerI() {
}
void RealTimeStateTestScheduler::StartExecution() {

}
void RealTimeStateTestScheduler::StopExecution() {
}
CLASS_REGISTER(RealTimeStateTestScheduler, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeStateTest::RealTimeStateTest() {

    static StreamString config = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMGroup1 = {"
            "            Class = RealTimeStateTestGAMGroup"
            "            +GAMA = {"
            "                Class = RealTimeStateStateFulGAM"
            "                InputSignals = {"
            "                    SignalIn1 = {"
            "                        DataSource = DDB1"
            "                        Type = uint32"
            "                        Alias = add1"
            "                        Default = 1"
            "                    }"
            "                    SignalIn2 = {"
            "                        DataSource = DDB2"
            "                        Type = uint32"
            "                        Alias = add2"
            "                        Default = 2"
            "                    }"
            "                }"
            "                OutputSignals = {"
            "                    SignalOut = {"
            "                        DataSource = DDB1"
            "                        Alias = add1"
            "                        Type = uint32"
            "                    }"
            "                }"
            "            }"
            "            +GAMB = {"
            "                Class = RealTimeStateStateLessGAM"
            "                InputSignals = {"
            "                    SignalIn1 = {"
            "                        DataSource = DDB2"
            "                        Type = uint32"
            "                        Alias = add2"
            "                    }"
            "                    SignalIn2 = {"
            "                        DataSource = DDB1"
            "                        Type = uint32"
            "                        Alias = add1"
            "                    }"
            "                }"
            "                OutputSignals = {"
            "                    SignalOut = {"
            "                        DataSource = DDB2"
            "                        Alias = add2"
            "                        Type = uint32"
            "                    }"
            "                }"
            "            }"
            "            +GAMGroup2 = {"
            "                Class = RealTimeStateTestGAMGroup"
            "                +GAMC = {"
            "                    Class = RealTimeStateStateLessGAM"
            "                    InputSignals = {"
            "                        SignalIn1 = {"
            "                            DataSource = DDB1"
            "                            Type = uint32"
            "                            Alias = add3"
            "                            Default = 1"
            "                        }"
            "                        SignalIn2 = {"
            "                            DataSource = DDB2"
            "                            Type = uint32"
            "                            Alias = add4"
            "                            Default = 2"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        SignalOut = {"
            "                            DataSource = DDB1"
            "                            Alias = add3"
            "                            Type = uint32"
            "                        }"
            "                    }"
            "                }"
            "                +GAMD = {"
            "                    Class = RealTimeStateStateLessGAM"
            "                    InputSignals = {"
            "                        SignalIn1 = {"
            "                            DataSource = DDB2"
            "                            Type = uint32"
            "                            Alias = add4"
            "                        }"
            "                        SignalIn2 = {"
            "                            DataSource = DDB1"
            "                            Type = uint32"
            "                            Alias = add3"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        SignalOut = {"
            "                            DataSource = DDB2"
            "                            Alias = add4"
            "                            Type = uint32"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "        }"
            "        +GAMGroup3 = {"
            "            Class = RealTimeStateTestGAMGroup"
            "            +Container = {"
            "                Class = ReferenceContainer"
            "                +GAMGroup4 = {"
            "                    Class = RealTimeStateTestGAMGroup"
            "                    +GAME = {"
            "                        Class = RealTimeStateStateLessGAM"
            "                        InputSignals = {"
            "                            SignalIn1 = {"
            "                                DataSource = DDB1"
            "                                Type = uint32"
            "                                Alias = add5"
            "                                Default = 1"
            "                            }"
            "                            SignalIn2 = {"
            "                                DataSource = DDB2"
            "                                Type = uint32"
            "                                Alias = add6"
            "                                Default = 2"
            "                            }"
            "                        }"
            "                        OutputSignals = {"
            "                            SignalOut = {"
            "                                DataSource = DDB1"
            "                                Alias = add5"
            "                                Type = uint32"
            "                            }"
            "                        }"
            "                    }"
            "                }"
            "                +GAMF = {"
            "                    Class = RealTimeStateStateLessGAM"
            "                    InputSignals = {"
            "                        SignalIn1 = {"
            "                            DataSource = DDB2"
            "                            Type = uint32"
            "                            Alias = add6"
            "                        }"
            "                        SignalIn2 = {"
            "                            DataSource = DDB1"
            "                            Type = uint32"
            "                            Alias = add5"
            "                        }"
            "                    }"
            "                    OutputSignals = {"
            "                        SignalOut = {"
            "                            DataSource = DDB2"
            "                            Alias = add6"
            "                            Type = uint32"
            "                        }"
            "                    }"
            "                }"
            "            }"
            "            +GAMG = {"
            "                Class = RealTimeStateStateLessGAM"
            "                InputSignals = {"
            "                    SignalIn1 = {"
            "                        DataSource = DDB1"
            "                        Type = uint32"
            "                        Alias = add7"
            "                        Default = 1"
            "                    }"
            "                    SignalIn2 = {"
            "                        DataSource = DDB2"
            "                        Type = uint32"
            "                        Alias = add8"
            "                        Default = 2"
            "                    }"
            "                }"
            "                OutputSignals = {"
            "                    SignalOut = {"
            "                        DataSource = DDB1"
            "                        Alias = add7"
            "                        Type = uint32"
            "                    }"
            "                }"
            "            }"
            "            +GAMH = {"
            "                Class = RealTimeStateStateFulGAM"
            "                InputSignals = {"
            "                    SignalIn1 = {"
            "                        DataSource = DDB2"
            "                        Type = uint32"
            "                        Alias = add8"
            "                    }"
            "                    SignalIn2 = {"
            "                        DataSource = DDB1"
            "                        Type = uint32"
            "                        Alias = add7"
            "                    }"
            "                }"
            "                OutputSignals = {"
            "                    SignalOut = {"
            "                        DataSource = DDB2"
            "                        Alias = add8"
            "                        Type = uint32"
            "                    }"
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
            "                    Functions = {GamGroup1 GamGroup1}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GamGroup1.GamGroup2}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMGroup3.Container}"
            "                }"
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMGroup3.Container.GAMGroup4}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = RealTimeStateTestScheduler"
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
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        printf("\nFAILED INITIALISATION\n");
    }
}

RealTimeStateTest::~RealTimeStateTest() {
    ObjectRegistryDatabase::Instance()->CleanUp();
}

bool RealTimeStateTest::TestConstructor() {
    RealTimeState test;
    return (test.GetNumberOfStatefuls() == 0);
}

bool RealTimeStateTest::TestAddStatefuls() {
    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Fibonacci.States.State1");
    if (!state1.IsValid()) {
        return false;
    }
    ReferenceContainer statefuls;
    ReferenceT<ReferenceContainer> functions = ObjectRegistryDatabase::Instance()->Find("Fibonacci.Functions");
    if (!functions.IsValid()) {
        return false;
    }

    ReferenceT<StatefulI> stateful = functions;

    //Look for all the GAMs inside the RealTimeApplication
    ReferenceContainerFilterReferencesTemplate<StatefulI> gamFilter(-1, ReferenceContainerFilterMode::RECURSIVE);

    functions->Find(statefuls, gamFilter);
    if (!state1->AddStatefuls(statefuls)) {
        return false;
    }

    printf("\n#statefuls = %d\n", state1->GetNumberOfStatefuls());
    return state1->GetNumberOfStatefuls() == 6;
}

bool RealTimeStateTest::TestPrepareNextState() {
    return true;
}

bool RealTimeStateTest::TestGetNumberOfStatefuls() {
    return true;
}

#if 0

bool RealTimeStateTest::TestConfigureArchitecture() {

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

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!state1->ConfigureArchitecture(*app.operator->())) {
        return false;
    }

    ReferenceT<PIDGAMGroup> group1 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup1");

    if (state1->GetNumberOfGAMGroups() != 1) {
        return false;
    }
    if (state1->GetGAMGroups()[0] != group1) {
        return false;
    }
    return true;

}

bool RealTimeStateTest::TestConfigureArchitecture_SingleInGAMGroup() {
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

    ReferenceT<RealTimeState> state2 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State2");

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    if (!state2->ConfigureArchitecture(*app.operator->())) {
        return false;
    }

    ReferenceT<PIDGAMGroup> group2 = ObjectRegistryDatabase::Instance()->Find("Application1.Functions.PIDGroup2");

    if (state2->GetNumberOfGAMGroups() != 1) {
        return false;
    }
    if (state2->GetGAMGroups()[0] != group2) {
        return false;
    }
    return true;

}

bool RealTimeStateTest::TestConfigureArchitectureFalse_NoThreads() {

    ConfigurationDatabase cdb;
    StreamString conf = ""
    "$Application1 = {\n" "\n"
    "    Class = RealTimeApplication\n"
    "    +States = {\n"
    "        Class = ReferenceContainer\n"
    "        +State1 = {\n"
    "            Class = RealTimeState\n"
    "            +Threadss = {\n"
    "                Class = ReferenceContainer\n"
    "                +Thread1 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {:Functions.GAM1 :Functions.GAM2}\n"
    "                }\n"
    "                +Thread2 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {:Functions.PIDGroup1}\n"
    "                }\n"
    "            }\n"
    "        }\n"
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

    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Application1");

    return (!state1->ConfigureArchitecture(*app.operator->()));
}

bool RealTimeStateTest::TestInsertFunction() {
    RealTimeState state;

    if (state.Size() != 0) {
        return false;
    }

    ReferenceT<PIDGAM> function1 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    function1->SetName("function1");

    ReferenceT<PIDGAM> function2 = ReferenceT<PIDGAM>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    function1->SetName("function2");

    if (!state.InsertFunction(function1)) {
        return false;
    }
    if (!state.InsertFunction(function2)) {
        return false;
    }

    if (state.Size() != 1) {
        return false;
    }

    ReferenceT<ReferenceContainer> functionsContainer = state.Get(0);
    if (StringHelper::Compare(functionsContainer->GetName(), "Functions") != 0) {
        return false;
    }

    return functionsContainer->Size() == 2;

}

bool RealTimeStateTest::TestAddGAMGroup() {
    RealTimeState state;

    const uint32 size = 32;
    ReferenceT<PIDGAMGroup> gamGroup[size];
    for (uint32 i = 0; i < size; i++) {
        gamGroup[i] = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        state.AddGAMGroup(gamGroup[i]);
        // add it two times
        state.AddGAMGroup(gamGroup[i]);
    }

    if (state.GetNumberOfGAMGroups() != size) {
        return false;
    }

    ReferenceT<GAMGroup> *ret = state.GetGAMGroups();

    for (uint32 i = 0; i < size; i++) {
        if (ret[i] != gamGroup[i]) {
            return false;
        }
    }
    return true;
}

bool RealTimeStateTest::TestGetGAMGroups() {
    RealTimeState state;

    const uint32 size = 32;
    ReferenceT<PIDGAMGroup> gamGroup[size];
    for (uint32 i = 0; i < size; i++) {
        gamGroup[i] = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        state.AddGAMGroup(gamGroup[i]);
    }

    ReferenceT<GAMGroup> *ret = state.GetGAMGroups();

    for (uint32 i = 0; i < size; i++) {
        if (ret[i] != gamGroup[i]) {
            return false;
        }
    }
    return true;
}

bool RealTimeStateTest::TestGetNumberOfGAMGroups() {
    RealTimeState state;

    const uint32 size = 32;
    ReferenceT<PIDGAMGroup> gamGroup[size];
    for (uint32 i = 0; i < size; i++) {
        gamGroup[i] = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        state.AddGAMGroup(gamGroup[i]);
    }

    return (state.GetNumberOfGAMGroups() == size);
}

bool RealTimeStateTest::TestPrepareState() {
    RealTimeState state;

    ConfigurationDatabase empty;
    const uint32 size = 32;
    ReferenceT<PIDGAMGroup> gamGroup[size];
    for (uint32 i = 0; i < size; i++) {
        gamGroup[i] = ReferenceT<PIDGAMGroup>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        state.AddGAMGroup(gamGroup[i]);
        gamGroup[i]->Initialise(empty);
    }

    RealTimeStateInfo info;
    info.activeBuffer = 0;
    info.currentState = "state1";
    info.nextState = "state2";

    state.PrepareState(info);

    for (uint32 i = 0; i < size; i++) {
        if (gamGroup[i]->GetContext() != 2) {
            return false;
        }
    }

    info.activeBuffer = 1;
    info.currentState = "state2";
    info.nextState = "state1";

    // change again
    state.PrepareState(info);

    for (uint32 i = 0; i < size; i++) {
        if (gamGroup[i]->GetContext() != 1) {
            return false;
        }
    }
    return true;
}

bool RealTimeStateTest::TestValidateDataSourceLinks() {
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
    "                    Cycles = 1" // sync in thread 1
    "                }\n"
    "            }\n"
    "        }\n"
    "        +GAM3 = {\n"
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
    "                    Cycles = 2"// sync in thread 2
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
    "                +Thread2 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {:Functions.GAM3 }\n"
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

    ReferenceT<RealTimeState> state = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

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

    bool ret = state->ValidateDataSourceLinks();
    ObjectRegistryDatabase::Instance()->CleanUp();
    return ret;
}

bool RealTimeStateTest::TestValidateDataSourceLinksFalse_MoreThanOneSyncPerThread() {
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
    "                    Cycles = 2" // sync in thread 1
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
    "                    Cycles = 1"// sync in thread 1
    "                }\n"
    "            }\n"
    "        }\n"
    "        +GAM3 = {\n"
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
    "                    Cycles = 2"// sync in thread 2
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
    "                +Thread2 = {\n"
    "                    Class = RealTimeThread\n"
    "                    Functions = {:Functions.GAM3 }\n"
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

    ReferenceT<RealTimeState> state = ObjectRegistryDatabase::Instance()->Find("Application1.States.State1");

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

    bool ret = !state->ValidateDataSourceLinks();
    ObjectRegistryDatabase::Instance()->CleanUp();
    return ret;
}
#endif
