/**
 * @file FastSchedulerTest.cpp
 * @brief Source file for class FastSchedulerTest
 * @date 11/04/2021
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
 * the class FastSchedulerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FastSchedulerTest.h"
#include "GAMTestHelper.h"
#include "MemoryDataSourceI.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StateMachine.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * @brief GAM which returns false on Execute
 */
class FastSchedulerTestGAMFalse: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    FastSchedulerTestGAMFalse() {

    }

    virtual ~FastSchedulerTestGAMFalse() {

    }

    virtual bool Setup() {
        return true;
    }

    virtual bool Initialise(StructuredDataI & data) {
        return GAM::Initialise(data);
    }

    virtual bool Execute() {
        return false;
    }

};
CLASS_REGISTER(FastSchedulerTestGAMFalse, "1.0")


class FastSchedulerTestGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    FastSchedulerTestGAM() {

    }

    virtual ~FastSchedulerTestGAM() {

    }

    virtual bool Setup() {
        return true;
    }

    virtual bool Execute() {
        return true;
    }

    uint8 *GetInputMem() {
        return (uint8*)GetInputSignalsMemory();
    }

    uint8 *GetOutputMem() {
        return (uint8*)GetOutputSignalsMemory();
    }
};
CLASS_REGISTER(FastSchedulerTestGAM, "1.0")


class FastSchedulerTestDSTest: public MemoryDataSourceI {
public:
    CLASS_REGISTER_DECLARATION()

    FastSchedulerTestDSTest();

    virtual ~FastSchedulerTestDSTest();

    virtual const char8 *GetBrokerName(StructuredDataI &data,
                                       const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
                                  const char8 * const nextStateName);

    virtual bool Synchronise();

    virtual EventSem *GetSem();

    uint8 stop;
private:
    EventSem eventSem;

};

FastSchedulerTestDSTest::FastSchedulerTestDSTest() {
    eventSem.Create();
    stop = 0;
}

FastSchedulerTestDSTest::~FastSchedulerTestDSTest() {
    eventSem.Close();
}

const char8 *FastSchedulerTestDSTest::GetBrokerName(StructuredDataI &data,
                                                    const SignalDirection direction) {
    const char8 *brokerName = NULL_PTR(const char8 *);
    if (direction == InputSignals) {
        float32 frequency = 0.F;
        uint32 trigger = 0u;
        if (!data.Read("Frequency", frequency)) {
            frequency = -1.F;
        }
        if (!data.Read("Trigger", trigger)) {
            trigger = 0u;
        }
        if ((frequency >= 0.F) || (trigger > 0u)) {
            brokerName = "MemoryMapSynchronisedInputBroker";
        }
        else {
            brokerName = "MemoryMapInputBroker";
        }
    }
    else {
        float32 frequency = 0.F;
        uint32 trigger = 0u;
        if (!data.Read("Frequency", frequency)) {
            frequency = -1.F;
        }
        if (!data.Read("Trigger", trigger)) {
            trigger = 0u;
        }
        if ((frequency >= 0.F) || (trigger > 0u)) {
            brokerName = "MemoryMapSynchronisedOutputBroker";
        }
        else {
            brokerName = "MemoryMapOutputBroker";
        }
    }
    return brokerName;
}

bool FastSchedulerTestDSTest::PrepareNextState(const char8 * const currentStateName,
                                               const char8 * const nextStateName) {
    return true;
}

bool FastSchedulerTestDSTest::Synchronise() {
    if (!stop) {
        printf("before ResWait %s\n", GetName());
        eventSem.ResetWait(0xFFFFFFFF);
        printf("after ResWait %s\n", GetName());
        (*(uint32*) memory)++;
    }
    return true;
}

EventSem *FastSchedulerTestDSTest::GetSem() {
    return &eventSem;
}

CLASS_REGISTER(FastSchedulerTestDSTest, "1.0")

static StreamString configFull = ""
        "+StateMachine = {"
        "    Class = StateMachine"
        "    +StopState = {"
        "        Class = ReferenceContainer"
        "        +Start = {"
        "            Class = StateMachineEvent"
        "            NextState = \"RunningState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +ChangeToState1Msg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Mode = \"ExpectsReply\""
        "                Function = PrepareNextState"
        "                +Parameters = {"
        "                    Class = ConfigurationDatabase"
        "                    param1 = \"State1\""
        "                }"
        "            }"
        "            +StartNextStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StartNextStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "    }"
        "    +RunningState = {"
        "        Class = ReferenceContainer"
        "        +ChangeToState1 = {"
        "            Class = StateMachineEvent"
        "            NextState = \"RunningState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +ChangeToState1Msg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Mode = \"ExpectsReply\""
        "                Function = PrepareNextState"
        "                +Parameters = {"
        "                    Class = ConfigurationDatabase"
        "                    param1 = \"State1\""
        "                }"
        "            }"
        "            +StopCurrentStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StopCurrentStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "            +StartNextStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StartNextStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "        +ChangeToState2 = {"
        "            Class = StateMachineEvent"
        "            NextState = \"RunningState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +ChangeToState2Msg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Mode = \"ExpectsReply\""
        "                Function = PrepareNextState"
        "                +Parameters = {"
        "                    Class = ConfigurationDatabase"
        "                    param1 = \"State2\""
        "                }"
        "            }"
        "            +StopCurrentStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StopCurrentStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "            +StartNextStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StartNextStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "        +Stop = {"
        "            Class = StateMachineEvent"
        "            NextState = \"StopState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +StopCurrentStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StopCurrentStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "    }"
        "    +ErrorState = {"
        "        Class = ReferenceContainer"
        "        +ENTER = {"
        "            Class = ReferenceContainer"
        "            +StopCurrentStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StopCurrentStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "        +Stop = {"
        "            Class = StateMachineEvent"
        "            NextState = \"StopState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "        }"
        "    }"
        "}"
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
        "        Class = FastScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +Start = {"
        "        Class = Message"
        "        Destination = \"StateMachine\""
        "        Function = Start"
        "    }"
        "    +Stop = {"
        "        Class = Message"
        "        Destination = \"StateMachine\""
        "        Function = Stop"
        "    }"
        "    +State1 = {"
        "        Class = Message"
        "        Destination = \"StateMachine\""
        "        Function = ChangeToState1"
        "    }"
        "    +State2 = {"
        "        Class = Message"
        "        Destination = \"StateMachine\""
        "        Function = ChangeToState2"
        "    }"
        "}"
        "";

static StreamString configFullError = ""
        "+StateMachine = {"
        "    Class = StateMachine"
        "    +StopState = {"
        "        Class = ReferenceContainer"
        "        +Start = {"
        "            Class = StateMachineEvent"
        "            NextState = \"RunningState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +ChangeToState1Msg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Mode = \"ExpectsReply\""
        "                Function = PrepareNextState"
        "                +Parameters = {"
        "                    Class = ConfigurationDatabase"
        "                    param1 = \"State1\""
        "                }"
        "            }"
        "            +StartNextStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StartNextStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "    }"
        "    +RunningState = {"
        "        Class = ReferenceContainer"
        "        +ChangeToState1 = {"
        "            Class = StateMachineEvent"
        "            NextState = \"RunningState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +ChangeToState1Msg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Mode = \"ExpectsReply\""
        "                Function = PrepareNextState"
        "                +Parameters = {"
        "                    Class = ConfigurationDatabase"
        "                    param1 = \"State1\""
        "                }"
        "            }"
        "            +StopCurrentStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StopCurrentStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "            +StartNextStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StartNextStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "        +ChangeToState2 = {"
        "            Class = StateMachineEvent"
        "            NextState = \"RunningState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +ChangeToState2Msg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Mode = \"ExpectsReply\""
        "                Function = PrepareNextState"
        "                +Parameters = {"
        "                    Class = ConfigurationDatabase"
        "                    param1 = \"State2\""
        "                }"
        "            }"
        "            +StopCurrentStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StopCurrentStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "            +StartNextStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StartNextStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "        +Stop = {"
        "            Class = StateMachineEvent"
        "            NextState = \"StopState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +StopCurrentStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StopCurrentStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "        +Error = {"
        "            Class = StateMachineEvent"
        "            NextState = \"ErrorState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "            +StopCurrentStateExecutionMsg = {"
        "                Class = Message"
        "                Destination = \"Fibonacci\""
        "                Function = StopCurrentStateExecution"
        "                Mode = \"ExpectsReply\""
        "            }"
        "        }"
        "    }"
        "    +ErrorState = {"
        "        Class = ReferenceContainer"
        "        +Stop = {"
        "            Class = StateMachineEvent"
        "            NextState = \"StopState\""
        "            NextStateError = \"ErrorState\""
        "            Timeout = 0"
        "        }"
        "    }"
        "}"
        "$Fibonacci = {"
        "    Class = RealTimeApplication"
        "    +Functions = {"
        "        Class = ReferenceContainer"
        "        +GAMError = {"
        "            Class = FastSchedulerTestGAMFalse"
        "            InputSignals = {"
        "                SignalIn1 = {"
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Alias = add1"
        "                    Default = 1"
        "                }"
        "            }"
        "        }"
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
        "                    Functions = {GAMA, GAMB, GAMError}"
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
        "        Class = FastScheduler"
        "        TimingDataSource = Timings"
        "        +Error = {"
        "            Class = Message"
        "            Destination = \"StateMachine\""
        "            Function = Error"
        "        }"
        "    }"
        "}"
        "+TestMessages = {"
        "    Class = ReferenceContainer"
        "    +Start = {"
        "        Class = Message"
        "        Destination = \"StateMachine\""
        "        Function = Start"
        "    }"
        "    +Stop = {"
        "        Class = Message"
        "        Destination = \"StateMachine\""
        "        Function = Stop"
        "    }"
        "    +State1 = {"
        "        Class = Message"
        "        Destination = \"StateMachine\""
        "        Function = ChangeToState1"
        "    }"
        "    +State2 = {"
        "        Class = Message"
        "        Destination = \"StateMachine\""
        "        Function = ChangeToState2"
        "    }"
        "}"
        "";

static StreamString configSimple = ""
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
        "        Class = FastScheduler"
        "        TimingDataSource = Timings"
        "    }"
        "}";

static StreamString configSimpleOneMessage = ""
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
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Alias = add1"
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
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FastScheduler"
        "        TimingDataSource = Timings"
        "        +Error = {"
        "            Class = Message"
        "            Destination = \"StateMachine\""
        "            Function = Stop"
        "        }"
        "    }"
        "}";

static StreamString configSimpleTwoMessages = ""
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
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Alias = add1"
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
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FastScheduler"
        "        TimingDataSource = Timings"
        "        +Error = {"
        "            Class = Message"
        "            Destination = \"StateMachine\""
        "            Function = Stop"
        "        }"
        "        +Error2 = {"
        "            Class = Message"
        "            Destination = \"StateMachine\""
        "            Function = Stop"
        "        }"
        "    }"
        "}";

StreamString configSimpleInvalidMessage = ""
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
        "                    DataSource = DDB1"
        "                    Type = uint32"
        "                    Alias = add1"
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
        "                    Functions = {GAMA}"
        "                }"
        "            }"
        "        }"
        "    }"
        "    +Scheduler = {"
        "        Class = FastScheduler"
        "        TimingDataSource = Timings"
        "        +Error = {"
        "            Class = ReferenceContainer"
        "        }"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FastSchedulerTest::FastSchedulerTest() {
    numOfThreadsBefore = Threads::NumberOfThreads();
}

FastSchedulerTest::~FastSchedulerTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool FastSchedulerTest::Init(StreamString &config) {
    config.Seek(0u);
    ConfigurationDatabase cdb;
    StandardParser parser(config, cdb);
    if (!parser.Parse()) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    if (!ObjectRegistryDatabase::Instance()->Initialise(cdb)) {
        return false;
    }
    return true;
}

bool FastSchedulerTest::TestConstructor() {
    FastScheduler test;
    return true;
}

bool FastSchedulerTest::TestInitialise() {
    ConfigurationDatabase config;
    config.Write("TimingDataSource", "Timings");
    FastScheduler scheduler;
    bool ok = scheduler.Initialise(config);

    return ok;
}

bool FastSchedulerTest::TestInitialise_ErrorMessage() {
    return Init(configSimpleOneMessage);
}

bool FastSchedulerTest::TestInitialise_False_MoreThanOneErrorMessage() {
    return !Init(configSimpleTwoMessages);
}

bool FastSchedulerTest::TestInitialise_False_InvalidMessage() {
    return !Init(configSimpleInvalidMessage);
}

bool FastSchedulerTest::TestIntegrated() {
    bool ok = Init(configFull);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ok) {
        ok = app->ConfigureApplication();
    }
    if (ok) {
        ReferenceT<Message> messageStart = ObjectRegistryDatabase::Instance()->Find("TestMessages.Start");
        if (!messageStart.IsValid()) {
            return false;
        }
        ok = MessageI::SendMessage(messageStart, NULL);
    }
    ReferenceT<GAM1> gams[8];
    gams[0] = app->Find("Functions.GAMA");
    gams[1] = app->Find("Functions.GAMB");
    gams[2] = app->Find("Functions.GAMC");
    gams[3] = app->Find("Functions.GAMD");
    gams[4] = app->Find("Functions.GAME");
    gams[5] = app->Find("Functions.GAMF");
    gams[6] = app->Find("Functions.GAMG");
    gams[7] = app->Find("Functions.GAMH");
    bool testExecuted1[] = { true, true, true, true, true, true, false, false };
    uint32 counter = 0;
    if (ok) {
        ok = false;
        while (!ok) {
            ok = true;
            for (uint32 i = 0u; (i < 8u) && (ok); i++) {
                ok = ((gams[i]->numberOfExecutions > 0u) == testExecuted1[i]);
            }
            counter++;
            if (!ok) {
                Sleep::Sec(0.1);
            }
            if (counter > 20) {
                break;
            }
        }
    }
    if (ok) {
        ReferenceT<Message> messageState2 = ObjectRegistryDatabase::Instance()->Find("TestMessages.State2");
        if (!messageState2.IsValid()) {
            return false;
        }
        ok = MessageI::SendMessage(messageState2, NULL);
    }
    bool testExecuted2[] = { true, true, true, true, true, true, true, true };
    counter = 0;
    if (ok) {
        ok = false;
        while (!ok) {
            ok = true;
            for (uint32 i = 0u; (i < 8u) && (ok); i++) {
                ok = ((gams[i]->numberOfExecutions > 0u) == testExecuted2[i]);
            }
            counter++;
            if (!ok) {
                Sleep::Sec(0.1);
            }
            if (counter > 20) {
                break;
            }
        }
    }
    if (ok) {
        for (uint32 i = 0u; (i < 8u); i++) {
            gams[i]->numberOfExecutions = 0;
        }
        ReferenceT<Message> messageState1 = ObjectRegistryDatabase::Instance()->Find("TestMessages.State1");
        if (!messageState1.IsValid()) {
            return false;
        }
        ok = MessageI::SendMessage(messageState1, NULL);
    }
    bool testExecuted3[] = { true, true, true, true, true, true, false, false };
    counter = 0;
    if (ok) {
        ok = false;
        while (!ok) {
            ok = true;
            for (uint32 i = 0u; (i < 8u) && (ok); i++) {
                ok = ((gams[i]->numberOfExecutions > 0u) == testExecuted3[i]);
            }
            counter++;
            if (!ok) {
                Sleep::Sec(0.1);
            }
            if (counter > 20) {
                break;
            }
        }
    }
    ReferenceT<Message> messageStop = ObjectRegistryDatabase::Instance()->Find("TestMessages.Stop");
    if (!messageStop.IsValid()) {
        return false;
    }
    ok &= (MessageI::SendMessage(messageStop, NULL) == ErrorManagement::NoError);
    Sleep::Sec(1.);
    ObjectRegistryDatabase::Instance()->Purge();
    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::Sec(0.1);
    }

    return ok;
}

bool FastSchedulerTest::TestIntegrated_TriggerErrorMessage() {
    bool ok = Init(configFullError);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ok) {
        ok = app->ConfigureApplication();
    }
    if (ok) {
        ReferenceT<Message> messageStart = ObjectRegistryDatabase::Instance()->Find("TestMessages.Start");
        if (!messageStart.IsValid()) {
            return false;
        }
        ok = MessageI::SendMessage(messageStart, NULL);
    }

    ReferenceT<StateMachine> sm = ObjectRegistryDatabase::Instance()->Find("StateMachine");

    if (ok) {
        ok = sm.IsValid();
    }
    Reference state;
    bool running = !ok;
    while (!running) {
        state = sm->GetCurrentState();
        ok = state.IsValid();
        if (ok) {
            running = (StringHelper::Compare(state->GetName(), "RunningState") == 0);
        }
    }
    bool stopped = !ok;
    while (!stopped) {
        state = sm->GetCurrentState();
        ok = state.IsValid();
        if (ok) {
            stopped = (StringHelper::Compare(state->GetName(), "ErrorState") == 0);
        }
        if (stopped) {
            stopped = (sm->GetCurrentStateStatus() == StateMachine::Executing);
        }
    }
    //Wait for the StateMachine to be the only thread alive.
    Sleep::Sec(1.);
    ObjectRegistryDatabase::Instance()->Purge();
    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::Sec(0.1);
    }

    return ok;
}

bool FastSchedulerTest::TestPurge() {
    bool ok = Init(configFull);
    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (ok) {
        ok = app->ConfigureApplication();
    }
    if (ok) {
        ReferenceT<Message> messageStart = ObjectRegistryDatabase::Instance()->Find("TestMessages.Start");
        if (!messageStart.IsValid()) {
            return false;
        }
        ok = MessageI::SendMessage(messageStart, NULL);
    }
    ReferenceT<GAM1> gams[8];
    gams[0] = app->Find("Functions.GAMA");
    gams[1] = app->Find("Functions.GAMB");
    gams[2] = app->Find("Functions.GAMC");
    gams[3] = app->Find("Functions.GAMD");
    gams[4] = app->Find("Functions.GAME");
    gams[5] = app->Find("Functions.GAMF");
    gams[6] = app->Find("Functions.GAMG");
    gams[7] = app->Find("Functions.GAMH");
    bool testExecuted1[] = { true, true, true, true, true, true, false, false };
    uint32 counter = 0;
    if (ok) {
        ok = false;
        while (!ok) {
            ok = true;
            for (uint32 i = 0u; (i < 8u) && (ok); i++) {
                ok = ((gams[i]->numberOfExecutions > 0u) == testExecuted1[i]);
            }
            counter++;
            if (!ok) {
                Sleep::Sec(0.1);
            }
            if (counter > 20) {
                break;
            }
        }
    }
    if (ok) {
        ReferenceT<Message> messageState2 = ObjectRegistryDatabase::Instance()->Find("TestMessages.State2");
        if (!messageState2.IsValid()) {
            return false;
        }
        ok = MessageI::SendMessage(messageState2, NULL);
    }
    bool testExecuted2[] = { true, true, true, true, true, true, true, true };
    counter = 0;
    if (ok) {
        ok = false;
        while (!ok) {
            ok = true;
            for (uint32 i = 0u; (i < 8u) && (ok); i++) {
                ok = ((gams[i]->numberOfExecutions > 0u) == testExecuted2[i]);
            }
            counter++;
            if (!ok) {
                Sleep::Sec(0.1);
            }
            if (counter > 20) {
                break;
            }
        }
    }
    ReferenceT<Message> messageStop = ObjectRegistryDatabase::Instance()->Find("TestMessages.Stop");
    if (!messageStop.IsValid()) {
        return false;
    }
    ok &= (MessageI::SendMessage(messageStop, NULL) == ErrorManagement::NoError);

    ObjectRegistryDatabase::Instance()->Purge();
    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::Sec(0.1);
    }

    return ok;
}

static uint32 done = 0;
static void PrepareNext(RealTimeApplication &app) {
    // the start execution is inside the prepare next state
    if (!app.PrepareNextState("State2")) {
        printf("\nFailed @ app.PrepareNextState!!!\n");
    }
    app.StopCurrentStateExecution();
    done = 1;
}

bool FastSchedulerTest::TestStartNextStateExecution() {
    if (!Init(configSimple)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app->ConfigureApplication()) {
        return false;
    }

    // the start execution is inside the prepare next state
    if (!app->PrepareNextState("State1")) {
        return false;
    }

    ReferenceT<FastScheduler> sched = app->Find("Scheduler");

    ErrorManagement::ErrorType err = app->StartNextStateExecution();
    if (err.ErrorsCleared()) {
        Sleep::Sec(1.0);
        done = 0;
        Threads::BeginThread((ThreadFunctionType) PrepareNext, app.operator ->());

        while (!done) {
            Sleep::MSec(10);
        }
        err = app->StartNextStateExecution();
        Sleep::Sec(1.0);

        if (err.ErrorsCleared()) {
            ReferenceT<GAM1> gam = app->Find("Functions.GAMA");
            uint32 tic = (gam->numberOfExecutions);

            Sleep::MSec(500);
            uint32 toc = gam->numberOfExecutions;
            err = (toc == tic);
            if (err.ErrorsCleared()) {
                app->StopCurrentStateExecution();
                Sleep::MSec(200);

                tic = (gam->numberOfExecutions);

                Sleep::MSec(500);
                toc = gam->numberOfExecutions;
                err = (toc != tic);
            }

        }
        ObjectRegistryDatabase::Instance()->Purge();

        while (Threads::NumberOfThreads() > numOfThreadsBefore) {
            Sleep::MSec(10);
        }
    }
    return err.ErrorsCleared();
}

bool FastSchedulerTest::TestStartNextStateExecution_False_PrepareNextState() {
    if (!Init(configSimple)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app->ConfigureApplication()) {
        return false;
    }

    ReferenceT<FastScheduler> sched = app->Find("Scheduler");

    ErrorManagement::ErrorType err = app->StartNextStateExecution();
    app->StopCurrentStateExecution();
    ObjectRegistryDatabase::Instance()->Purge();

    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::MSec(10);
    }
    return !err.ErrorsCleared();
}

bool FastSchedulerTest::TestStopCurrentStateExecution() {
    return TestStartNextStateExecution();
}

bool FastSchedulerTest::TestIntegrated_WaitForAll() {

    static StreamString configTestIntegrated_WaitForAll = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = FastSchedulerTestGAM"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = TestDS1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Trigger = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Trigger = 1"
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
            "            Class = FastSchedulerTestGAM"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = TestDS2"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Trigger = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Trigger = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
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
            "        +TestDS1 = {"
            "            Class = FastSchedulerTestDSTest"
            "        }"
            "        +TestDS2= {"
            "            Class = FastSchedulerTestDSTest"
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
            "                +Thread2 = {"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = FastScheduler"
            "        TimingDataSource = Timings"
            "        NoWait = 0"
            "    }"
            "}";

    if (!Init(configTestIntegrated_WaitForAll)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app->ConfigureApplication()) {
        return false;
    }

    // the start execution is inside the prepare next state
    if (!app->PrepareNextState("State1")) {
        return false;
    }

    ReferenceT<FastScheduler> sched = app->Find("Scheduler");
    ReferenceT<FastSchedulerTestDSTest> testDS1 = app->Find("Data.TestDS1");
    ReferenceT<FastSchedulerTestDSTest> testDS2 = app->Find("Data.TestDS2");
    ReferenceT<FastSchedulerTestGAM> testgama = app->Find("Functions.GAMA");
    ReferenceT<FastSchedulerTestGAM> testgamb = app->Find("Functions.GAMB");

    if (!sched.IsValid() || !testDS1.IsValid() || !testDS2.IsValid() || !testgama.IsValid() || !testgamb.IsValid()) {
        return false;
    }

    EventSem *sem1 = testDS1->GetSem();
    EventSem *sem2 = testDS2->GetSem();
    ErrorManagement::ErrorType err = app->StartNextStateExecution();

    bool ret = true;
    if (err.ErrorsCleared()) {
        uint32 *mem1 = (uint32 *) testgama->GetInputMem();
        uint32 *mem2 = (uint32 *) testgamb->GetInputMem();

        printf("mem1=%d, mem2=%d\n", *mem1, *mem2);
        if ((*mem1 != 0u) || (*mem2 != 0u)) {
            return false;
        }
        //be sure threads are waiting
        Sleep::MSec(1000u);

        sem1->Post();
        sem2->Post();

        uint32 cnt = 0u;
        while (((*mem1 != 1u) || (*mem2 != 1u)) && (cnt < 3u)) {
            Sleep::MSec(500u);
            cnt++;
        }
        printf("mem1=%d, mem2=%d\n", *mem1, *mem2);
        if ((*mem1 != 1u) || (*mem2 != 1u)) {
            ret = false;
        }
        if (ret) {
            //give the time to wait again
            Sleep::MSec(1000u);

            //never do stop-start, always prepare-stop-start,
            app->PrepareNextState("State1");
            app->StopCurrentStateExecution();
            app->StartNextStateExecution();

            //this works from the prev cycle
            sem1->Post();
            cnt = 0u;
            while (((*mem1 != 2u) || (*mem2 != 1u)) && (cnt < 3u)) {
                Sleep::MSec(500u);
                cnt++;
            }
            printf("mem1=%d, mem2=%d\n", *mem1, *mem2);
            if ((*mem1 != 2u) || (*mem2 != 1u)) {
                ret = false;
            }

            if (ret) {
                //this is not working because blocked by the other
                sem1->Post();

                Sleep::MSec(1000);
                printf("mem1=%d, mem2=%d\n", *mem1, *mem2);
                if ((*mem1 != 2u) || (*mem2 != 1u)) {
                    ret = false;
                }
            }

            if (ret) {
                sem2->Post();
                Sleep::MSec(200u);

                //needed because of the ResetWait
                sem1->Post();

                cnt = 0u;
                while (((*mem1 != 3u) || (*mem2 != 2u)) && (cnt < 3u)) {
                    Sleep::MSec(500u);
                    cnt++;
                }
                printf("mem1=%d, mem2=%d\n", *mem1, *mem2);
                if ((*mem1 != 3u) || (*mem2 != 2u)) {
                    ret = false;
                }
            }
        }

    }
    testDS1->stop = 1;
    testDS2->stop = 1;
    sem1->Post();
    sem2->Post();

    ObjectRegistryDatabase::Instance()->Purge();

    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::MSec(10);
    }

    return ret;
}

bool FastSchedulerTest::TestIntegrated_NoWait() {

    static StreamString configTestIntegrated_NoWait = ""
            "$Fibonacci = {"
            "    Class = RealTimeApplication"
            "    +Functions = {"
            "        Class = ReferenceContainer"
            "        +GAMA = {"
            "            Class = FastSchedulerTestGAM"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = TestDS1"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Trigger = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Trigger = 1"
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
            "            Class = FastSchedulerTestGAM"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = TestDS2"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Trigger = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Trigger = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
            "                    Alias = add2"
            "                    Type = uint32"
            "                }"
            "            }"
            "        }"
            "        +GAMC = {"
            "            Class = FastSchedulerTestGAM"
            "            InputSignals = {"
            "                SignalIn1 = {"
            "                    DataSource = TestDS2"
            "                    Type = uint32"
            "                    Alias = add1"
            "                    Trigger = 1"
            "                }"
            "                SignalIn2 = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                    Alias = add2"
            "                    Trigger = 1"
            "                }"
            "            }"
            "            OutputSignals = {"
            "                SignalOut = {"
            "                    DataSource = DDB1"
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
            "        +TestDS1 = {"
            "            Class = FastSchedulerTestDSTest"
            "        }"
            "        +TestDS2= {"
            "            Class = FastSchedulerTestDSTest"
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
            "                    CPUs = 0x1"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 0x1"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMB}"
            "                }"
            "            }"
            "        }"
            "        +State2 = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    CPUs = 0x1"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMA}"
            "                }"
            "                +Thread2 = {"
            "                    CPUs = 0x2"
            "                    Class = RealTimeThread"
            "                    Functions = {GAMC}"
            "                }"
            "            }"
            "        }"
            "    }"
            "    +Scheduler = {"
            "        Class = FastScheduler"
            "        TimingDataSource = Timings"
            "        NoWait = 1"
            "    }"
            "}";

    if (!Init(configTestIntegrated_NoWait)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app->ConfigureApplication()) {
        return false;
    }

    // the start execution is inside the prepare next state
    if (!app->PrepareNextState("State1")) {
        return false;
    }

    ReferenceT<FastScheduler> sched = app->Find("Scheduler");
    ReferenceT<FastSchedulerTestDSTest> testDS1 = app->Find("Data.TestDS1");
    ReferenceT<FastSchedulerTestDSTest> testDS2 = app->Find("Data.TestDS2");
    ReferenceT<FastSchedulerTestGAM> testgama = app->Find("Functions.GAMA");
    ReferenceT<FastSchedulerTestGAM> testgamb = app->Find("Functions.GAMB");
    ReferenceT<FastSchedulerTestGAM> testgamc = app->Find("Functions.GAMC");

    if (!sched.IsValid() || !testDS1.IsValid() || !testDS2.IsValid() || !testgama.IsValid() || !testgamb.IsValid()) {
        return false;
    }

    EventSem *sem1 = testDS1->GetSem();
    EventSem *sem2 = testDS2->GetSem();
    ErrorManagement::ErrorType err = app->StartNextStateExecution();

    bool ret = true;
    if (err.ErrorsCleared()) {
        uint32 *mem1 = (uint32 *) testgama->GetInputMem();
        uint32 *mem2 = (uint32 *) testgamb->GetInputMem();
        uint32 *mem3 = (uint32 *) testgamc->GetInputMem();

        printf("mem1=%d, mem2=%d\n", *mem1, *mem2);
        if ((*mem1 != 0u) || (*mem2 != 0u)) {
            return false;
        }
        //be sure threads are waiting
        Sleep::MSec(1000u);

        sem1->Post();
        sem2->Post();

        uint32 cnt = 0u;
        while (((*mem1 != 1u) || (*mem2 != 1u)) && (cnt < 3u)) {
            Sleep::MSec(500u);
            cnt++;
        }
        printf("mem1=%d, mem2=%d\n", *mem1, *mem2);
        if ((*mem1 != 1u) || (*mem2 != 1u)) {
            ret = false;
        }
        if (ret) {
            //give the time to wait again
            Sleep::MSec(1000u);

            //never do stop-start, always prepare-stop-start,
            app->PrepareNextState("State2");
            app->StopCurrentStateExecution();
            app->StartNextStateExecution();

            //this works from the prev cycle
            sem1->Post();
            //this is going to copy to GAMB still
            sem2->Post();

            cnt = 0u;
            while (((*mem1 != 2u) || (*mem3 != 0u)) && (cnt < 3u)) {
                Sleep::MSec(500u);
                cnt++;
            }
            printf("mem1=%d, mem2=%d\n", *mem1, *mem3);
            if ((*mem1 != 2u) || (*mem3 != 0u)) {
                ret = false;
            }

            if (ret) {
                sem1->Post();

                cnt = 0u;
                while (((*mem1 != 3u) || (*mem3 != 0u)) && (cnt < 3u)) {
                    Sleep::MSec(500u);
                    cnt++;
                }
                printf("mem1=%d, mem2=%d\n", *mem1, *mem3);
                if ((*mem1 != 3u) || (*mem3 != 0u)) {
                    ret = false;
                }
            }

            if (ret) {

                sem2->Post();
                //needed because of the ResetWait
                sem1->Post();

                cnt = 0u;
                while (((*mem1 != 4u) || (*mem3 != 3u)) && (cnt < 3u)) {
                    Sleep::MSec(500u);
                    cnt++;
                }
                printf("mem1=%d, mem2=%d\n", *mem1, *mem3);
                if ((*mem1 != 4u) || (*mem3 != 3u)) {
                    ret = false;
                }
            }
        }

    }
    testDS1->stop = 1;
    testDS2->stop = 1;
    sem1->Post();
    sem2->Post();

    ObjectRegistryDatabase::Instance()->Purge();

    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::MSec(10);
    }

    return ret;
}

