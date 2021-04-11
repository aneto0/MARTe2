/**
 * @file FastSchedulerTest.cpp
 * @brief Source file for class FastSchedulerTest
 * @date Apr 11, 2021 TODO Verify the value and format of the date
 * @author ferrog TODO Verify the name and format of the author
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
    while (Threads::NumberOfThreads() > 0) {
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
    while (Threads::NumberOfThreads() > 0) {
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
    while (Threads::NumberOfThreads() > 0) {
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
                if (!err.ErrorsCleared()) {
                    printf("failed 2 %d %d\n", tic, toc);
                }
            }
            else {
                printf("failed 1 %d %d\n", tic, toc);
            }

        }
        ObjectRegistryDatabase::Instance()->Purge();

        while (Threads::NumberOfThreads() > 0) {
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

    return !err.ErrorsCleared();
}

bool FastSchedulerTest::TestStopCurrentStateExecution() {
    return TestStartNextStateExecution();
}

