/**
 * @file StateMachineTest.h
 * @brief Header file for class StateMachineTest
 * @date 15/10/2016
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

 * @details This header file contains the declaration of the class StateMachineTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATEMACHINETEST_H_
#define STATEMACHINETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StateMachine.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * Tests the StateMachineEvent public methods.
 */
class StateMachineTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method without defining any states.
     */
    bool TestInitialise_False_NoStates();

    /**
     * @brief Tests the Initialise method with a state without event.
     */
    bool TestInitialise_False_NoEvents();

    /**
     * @brief Tests the Initialise method with a NextState that does not exist.
     */
    bool TestInitialise_False_NextState();

    /**
     * @brief Tests the Initialise method with a NextStateError that does not exist.
     */
    bool TestInitialise_False_NextStateError();

    /**
     * @brief Tests the GetState method.
     */
    bool TestGetState();

    /**
     * @brief Tests the GetStatus method.
     */
    bool TestGetStateStatus();

    /**
     * @brief Tests the EventTriggered method.
     */
    bool TestEventTriggered();

    /**
     * @brief Tests the EventTriggered method through the SendMessage interface.
     */
    bool TestEventTriggered_SendMessage();

    /**
     * @brief Tests the EventTriggered method through the SendMessage interface and wait for a replay.
     */
    bool TestEventTriggered_SendMessage_WaitReply();

    /**
     * @brief Tests the EventTriggered method through the SendMessage and verifies that it goes into an error state.
     */
    bool TestEventTriggered_SendMessage_GoToError();

    /**
     * @brief Tests the EventTriggered method through the SendMessage and verifies that it goes into an error state after a timeout.
     */
    bool TestEventTriggered_SendMessage_GoToError_Timeout();

    /**
     * @brief Tests the EventTriggered method through the SendMessage interface and by forcing two state machines to interchange information between them.
     */
    bool TestEventTriggered_SendMessage_PingPong();

    /**
     * @brief Tests the GetState method.
     */
    bool TestExportData();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATEMACHINETEST_H_ */

