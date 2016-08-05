/**
 * @file RealTimeStateTest.cpp
 * @brief Source file for class RealTimeStateTest
 * @date 07/03/2016
 * @author Giusepppe Ferro
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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class RealTimeStateTestContext1: public Object {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeStateTestContext1    ();

    int32 GetProperty1() const;

    float32 GetProperty2()const;

    void SetProperty1(int32);

    void SetProperty2(float32);
private:

    int32 propertyToBeShared1;

    float32 propertyToBeShared2;
};

RealTimeStateTestContext1::RealTimeStateTestContext1() :
        Object() {
    propertyToBeShared1 = 0;
    propertyToBeShared2 = 0;
}

int32 RealTimeStateTestContext1::GetProperty1() const {
    return propertyToBeShared1;
}

float32 RealTimeStateTestContext1::GetProperty2() const {
    return propertyToBeShared2;
}

void RealTimeStateTestContext1::SetProperty1(int32 x) {
    propertyToBeShared1 = x;
}

void RealTimeStateTestContext1::SetProperty2(float32 x) {
    propertyToBeShared2 = x;

}

CLASS_REGISTER(RealTimeStateTestContext1, "1.0")

//////////////////
//////////////////
//////////////////
//////////////////
class RealTimeStateTestGAMGroup1: public GAMGroup {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeStateTestGAMGroup1    ();

    virtual bool PrepareNextState(const char8 * currentStateName,
            const char8 * nextStateName);

private:
    ReferenceT<RealTimeStateTestContext1> context;
};

RealTimeStateTestGAMGroup1::RealTimeStateTestGAMGroup1() :
        GAMGroup() {
}

bool RealTimeStateTestGAMGroup1::PrepareNextState(const char8 * currentStateName,
                                                  const char8 * nextStateName) {

    context = ReferenceT<RealTimeStateTestContext1>("RealTimeStateTestContext1");

    context->SetProperty1(1);
    context->SetProperty2(2.0);
    return SetContext(context);

}

CLASS_REGISTER(RealTimeStateTestGAMGroup1, "1.0")

//////////////////
//////////////////
//////////////////
//////////////////
class RealTimeStateTestGAMGroup2: public GAMGroup {
public:
    CLASS_REGISTER_DECLARATION()

RealTimeStateTestGAMGroup2    ();

    virtual bool PrepareNextState(const char8 * currentStateName,
            const char8 * nextStateName);
private:
    ReferenceT<RealTimeStateTestContext1> context;

};

RealTimeStateTestGAMGroup2::RealTimeStateTestGAMGroup2() :
        GAMGroup() {

}

bool RealTimeStateTestGAMGroup2::PrepareNextState(const char8 * currentStateName,
                                                  const char8 * nextStateName) {

    context = ReferenceT<RealTimeStateTestContext1>("RealTimeStateTestContext1");

    context->SetProperty1(3);
    context->SetProperty2(4.0);

    return SetContext(context);

}

CLASS_REGISTER(RealTimeStateTestGAMGroup2, "1.0")

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

    virtual ConstReference GetContext(uint32 index);

private:
    ConstReference context[2];
    uint32 index;
};

RealTimeStateStateLessGAM::RealTimeStateStateLessGAM() :
        GAM() {
    index = 0;
}

bool RealTimeStateStateLessGAM::Execute() {
    return true;
}

bool RealTimeStateStateLessGAM::SetContext(ConstReference contextIn) {

    context[index] = contextIn;
    index++;
    return true;
}

ConstReference RealTimeStateStateLessGAM::GetContext(uint32 index) {
    return context[index];
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

    virtual ConstReference GetContext2(uint32 index);

    virtual bool PrepareNextState(const char8 * current, const char8 * next);

private:
    uint32 context;
    ConstReference context2[2];
    uint32 index;

};

RealTimeStateStateFulGAM::RealTimeStateStateFulGAM() :
        GAM() {
    context = 0u;
    index = 0;

}

bool RealTimeStateStateFulGAM::Execute() {
    return true;
}

bool RealTimeStateStateFulGAM::SetContext(ConstReference contextIn) {

    context2[index] = contextIn;
    index++;
    return true;
}

uint32 RealTimeStateStateFulGAM::GetContext1() {
    return context;
}

ConstReference RealTimeStateStateFulGAM::GetContext2(uint32 index) {
    return context2[index];
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
            "            Class = RealTimeStateTestGAMGroup1"
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
            "                Class = RealTimeStateTestGAMGroup2"
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
            "            Class = RealTimeStateTestGAMGroup2"
            "            +Container = {"
            "                Class = ReferenceContainer"
            "                +GAMGroup4 = {"
            "                    Class = RealTimeStateTestGAMGroup1"
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
    }

    cdb.MoveToRoot();
    ObjectRegistryDatabase::Instance()->CleanUp();

    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
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

    //if we do this twise it will not add nothing becouse they are already inside
    if (!state1->AddStatefuls(statefuls)) {
        return false;
    }

    return state1->GetNumberOfStatefuls() == 6;
}

bool RealTimeStateTest::TestAddStatefulsFalse_InvalidStateful() {
    ReferenceT<RealTimeState> state1 = ObjectRegistryDatabase::Instance()->Find("Fibonacci.States.State1");
    if (!state1.IsValid()) {
        return false;
    }
    ReferenceT<RealTimeStateStateLessGAM> notStateful = ObjectRegistryDatabase::Instance()->Find("Fibonacci.Functions.GAMGroup1.GAMB");
    if (!notStateful.IsValid()) {
        return false;
    }

    ReferenceContainer statefuls;

    statefuls.Insert(notStateful);
    return (!state1->AddStatefuls(statefuls));

}

bool RealTimeStateTest::TestPrepareNextState() {
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

    //if we do this twise it will not add nothing becouse they are already inside
    if (!state1->AddStatefuls(statefuls)) {
        return false;
    }

    if (!state1->PrepareNextState("", "State1")) {
        return false;
    }

    ReferenceT<RealTimeStateStateFulGAM> gama = functions->Find("GAMGroup1.GAMA");
    ReferenceT<RealTimeStateStateLessGAM> gamb = functions->Find("GAMGroup1.GAMB");
    ReferenceT<RealTimeStateStateLessGAM> gamc = functions->Find("GAMGroup1.GAMGroup2.GAMC");
    ReferenceT<RealTimeStateStateLessGAM> gamd = functions->Find("GAMGroup1.GAMGroup2.GAMD");
    ReferenceT<RealTimeStateStateLessGAM> game = functions->Find("GAMGroup3.Container.GAMGroup4.GAME");
    ReferenceT<RealTimeStateStateLessGAM> gamf = functions->Find("GAMGroup3.Container.GAMF");
    ReferenceT<RealTimeStateStateLessGAM> gamg = functions->Find("GAMGroup3.GAMG");
    ReferenceT<RealTimeStateStateFulGAM> gamh = functions->Find("GAMGroup3.GAMH");

    ConstReferenceT(RealTimeStateTestContext1)context[2];
    context[0] = gama->GetContext2(0);
    context[1] = gama->GetContext2(1);
    if (context[0]->GetProperty1() != 1 && context[0]->GetProperty2() != 2.0) {
        return false;
    }
    if (gama->GetContext1() != 1) {
        return false;
    }
    if (context[1].IsValid()) {
        return false;
    }

    context[0] = gamb->GetContext(0);
    context[1] = gamb->GetContext(1);
    if (context[0]->GetProperty1() != 1 && context[0]->GetProperty2() != 2.0) {
        return false;
    }
    if (context[1].IsValid()) {
        return false;
    }

    context[0] = gamc->GetContext(0);
    context[1] = gamc->GetContext(1);
    if (context[0]->GetProperty1() != 1 && context[0]->GetProperty2() != 2.0) {
        return false;
    }
    if (context[1]->GetProperty1() != 3 && context[1]->GetProperty2() != 4.0) {
        return false;
    }

    context[0] = gamd->GetContext(0);
    context[1] = gamd->GetContext(1);
    if (context[0]->GetProperty1() != 1 && context[0]->GetProperty2() != 2.0) {
        return false;
    }
    if (context[1]->GetProperty1() != 3 && context[1]->GetProperty2() != 4.0) {
        return false;
    }

    context[0] = game->GetContext(0);
    context[1] = game->GetContext(1);
    if (context[0]->GetProperty1() != 3 && context[0]->GetProperty2() != 4.0) {
        return false;
    }

    if (context[1]->GetProperty1() != 1 && context[1]->GetProperty2() != 2.0) {
        return false;
    }

    context[0] = gamf->GetContext(0);
    context[1] = gamf->GetContext(1);
    if (context[0]->GetProperty1() != 3 && context[0]->GetProperty2() != 4.0) {
        return false;
    }

    if (context[1].IsValid()) {
        return false;
    }

    context[0] = gamg->GetContext(0);
    context[1] = gamg->GetContext(1);
    if (context[0]->GetProperty1() != 3 && context[0]->GetProperty2() != 4.0) {
        return false;
    }

    if (context[1].IsValid()) {
        return false;
    }

    context[0] = gamh->GetContext2(0);
    context[1] = gamh->GetContext2(1);

    if (context[0]->GetProperty1() != 3 && context[0]->GetProperty2() != 4.0) {
        return false;
    }

    if (context[1].IsValid()) {
        return false;
    }
    if (gamh->GetContext1() != 1) {
        return false;
    }

    return true;
}

bool RealTimeStateTest::TestGetNumberOfStatefuls() {
    return TestAddStatefuls();
}

