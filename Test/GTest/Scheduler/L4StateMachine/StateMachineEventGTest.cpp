/**
 * @file StateMachineEventGTest.cpp
 * @brief Source file for class StateMachineEventGTest
 * @date 11/10/2016
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
 * the class MessageGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include "MARTe2UTest.h"
#include "StateMachineEventTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestConstructor) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestConsumeMessage) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestConsumeMessage());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestGetNextState) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestGetNextState());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestGetNextStateError) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestGetNextStateError());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestGetTransitionTimeout) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestGetTransitionTimeout());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestInitialise) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestInitialise_False_NoNextState) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestInitialise_False_NoNextState());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestInitialise_NoZeroTimeout) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestInitialise_NoZeroTimeout());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestInitialise_True_NoNextStateError) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestInitialise_True_NoNextStateError());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestInitialise_True_NoTimeout) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestInitialise_True_NoTimeout());
}

TEST(Scheduler_L4StateMachine_StateMachineEventGTest,TestSetStateMachine) {
    StateMachineEventTest target;
    ASSERT_TRUE(target.TestSetStateMachine());
}


