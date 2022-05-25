/**
 * @file GAMSchedulerTest.cpp
 * @brief Source file for class GAMSchedulerTest
 * @date 09/08/2016
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
 * the class GAMSchedulerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GAMSchedulerTest.h"
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
class GAMSchedulerTestGAMFalse: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

    GAMSchedulerTestGAMFalse() {

    }

    virtual ~GAMSchedulerTestGAMFalse() {

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
CLASS_REGISTER(GAMSchedulerTestGAMFalse, "1.0")

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
        "        Class = GAMScheduler"
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
        "            Class = GAMSchedulerTestGAMFalse"
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
        "        Class = GAMScheduler"
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
        "        Class = GAMScheduler"
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
        "        Class = GAMScheduler"
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
        "        Class = GAMScheduler"
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

static StreamString configSimpleInvalidMessage = ""
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
        "        Class = GAMScheduler"
        "        TimingDataSource = Timings"
        "        +Error = {"
        "            Class = ReferenceContainer"
        "        }"
        "    }"
        "}";

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMSchedulerTest::GAMSchedulerTest() {
    numOfThreadsBefore = Threads::NumberOfThreads();
}

GAMSchedulerTest::~GAMSchedulerTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool GAMSchedulerTest::Init(StreamString &config) {
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

bool GAMSchedulerTest::TestConstructor() {
    GAMScheduler test;
    return true;
}

bool GAMSchedulerTest::TestInitialise() {
    ConfigurationDatabase config;
    config.Write("TimingDataSource", "Timings");
    GAMScheduler scheduler;
    bool ok = scheduler.Initialise(config);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool GAMSchedulerTest::TestInitialise_ErrorMessage() {
    bool ok = Init(configSimpleOneMessage);
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool GAMSchedulerTest::TestInitialise_False_MoreThanOneErrorMessage() {
    bool ok = !Init(configSimpleTwoMessages);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool GAMSchedulerTest::TestInitialise_False_InvalidMessage() {
    bool ok = !Init(configSimpleInvalidMessage);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool GAMSchedulerTest::TestIntegrated() {
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

    while (Threads::NumberOfThreads() > 1 + numOfThreadsBefore) {
        Sleep::Sec(0.1);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::Sec(0.1);
    }

    return ok;
}

bool GAMSchedulerTest::TestIntegrated_TriggerErrorMessage() {
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
    while (Threads::NumberOfThreads() > 1 + numOfThreadsBefore) {
        Sleep::Sec(0.1);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::Sec(0.1);
    }

    return ok;
}

bool GAMSchedulerTest::TestPurge() {
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

bool GAMSchedulerTest::TestStartNextStateExecution() {
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

    ReferenceT<GAMScheduler> sched = app->Find("Scheduler");

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

        app->StopCurrentStateExecution();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    while (Threads::NumberOfThreads() > numOfThreadsBefore) {
        Sleep::MSec(10);
    }
    return err.ErrorsCleared();
}

bool GAMSchedulerTest::TestStartNextStateExecution_False_PrepareNextState() {
    if (!Init(configSimple)) {
        return false;
    }

    ReferenceT<RealTimeApplication> app = ObjectRegistryDatabase::Instance()->Find("Fibonacci");
    if (!app->ConfigureApplication()) {
        return false;
    }

    ReferenceT<GAMScheduler> sched = app->Find("Scheduler");

    ErrorManagement::ErrorType err = app->StartNextStateExecution();
    ObjectRegistryDatabase::Instance()->Purge();

    return !err.ErrorsCleared();
}

bool GAMSchedulerTest::TestStopCurrentStateExecution() {
    return TestStartNextStateExecution();
}
