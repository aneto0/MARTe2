/**
 * @file StateMachineEventTest.h
 * @brief Header file for class StateMachineEventTest
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

 * @details This header file contains the declaration of the class StateMachineEventTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATEMACHINEEVENTTEST_H_
#define STATEMACHINEEVENTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * Tests the StateMachineEvent public methods.
 */
class StateMachineEventTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the GetTimeout method.
     */
    bool TestGetTransitionTimeout();

    /**
     * @brief Tests the GetNextState method.
     */
    bool TestGetNextState();

    /**
     * @brief Tests the GetNextStateError method.
     */
    bool TestGetNextStateError();

    /**
     * @brief Tests the SetStateMachine method.
     */
    bool TestSetStateMachine();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with a timeout that is not zero.
     */
    bool TestInitialise_NoZeroTimeout();

    /**
     * @brief Tests the Initialise method without specifying the NextState.
     */
    bool TestInitialise_False_NoNextState();

    /**
     * @brief Tests the Initialise method without specifying the NextStateError.
     */
    bool TestInitialise_True_NoNextStateError();

    /**
     * @brief Tests the Initialise method without specifying the timeout.
     */
    bool TestInitialise_True_NoTimeout();

    /**
     * @brief Tests the ConsumeMessage method.
     */
    bool TestConsumeMessage();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
#endif /* STATEMACHINEEVENTTEST_H_ */

