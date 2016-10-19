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


#include "gtest/gtest.h"
#include "StateMachineTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(StateMachineGTest,TestConstructor) {
    StateMachineTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(StateMachineGTest,TestInitialise) {
    StateMachineTest target;
    ASSERT_TRUE(target.TestInitialise());
}

TEST(StateMachineGTest,TestInitialise_False_NoStates) {
    StateMachineTest target;
    ASSERT_TRUE(target.TestInitialise_False_NoStates());
}

TEST(StateMachineGTest,TestInitialise_False_NoEvents) {
    StateMachineTest target;
    ASSERT_TRUE(target.TestInitialise_False_NoEvents());
}

TEST(StateMachineGTest,TestInitialise_False_NextState) {
    StateMachineTest target;
    ASSERT_TRUE(target.TestInitialise_False_NextState());
}

TEST(StateMachineGTest,TestInitialise_False_NextStateError) {
    StateMachineTest target;
    ASSERT_TRUE(target.TestInitialise_False_NextStateError());
}

TEST(StateMachineGTest,TestEventTriggered) {
    StateMachineTest target;
    ASSERT_TRUE(target.TestEventTriggered());
}

TEST(StateMachineGTest,TestEventTriggered_SendMessage) {
    StateMachineTest target;
    ASSERT_TRUE(target.TestEventTriggered_SendMessage());
}
