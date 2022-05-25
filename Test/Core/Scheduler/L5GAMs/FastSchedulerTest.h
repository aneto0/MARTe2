/**
 * @file FastSchedulerTest.h
 * @brief Header file for class FastSchedulerTest
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

 * @details This header file contains the declaration of the class FastSchedulerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTSCHEDULERTEST_H_
#define FASTSCHEDULERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "FastScheduler.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the FastScheduler functions
 */
class FastSchedulerTest {
private:
/**
* @brief Keeps the number of threads which were running before the test execution.
*        This accomodates the needs of some OSs (e.g. FreeRTOS) which have a minimum number of running tests
*/
uint32 numOfThreadsBefore;
public:

    /**
     * @brief Constructor
     */
    FastSchedulerTest();

    /**
     * @brief Destructor
     */
    virtual ~FastSchedulerTest();

    /**
     * @brief Initialises the ObjectRegistryDatabase
     */
    bool Init(StreamString &config);

    /**
     * @brief Tests the scheduler constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests the execution of a multi-thread MARTe application
     */
    bool TestStartNextStateExecution();

    /**
     * @brief Tests that starting the execution of a multi-thread MARTe application without Preparing the state returns false
     */
    bool TestStartNextStateExecution_False_PrepareNextState();

    /**
     * @brief Tests the execution of a multi-thread MARTe application
     */
    bool TestStopCurrentStateExecution();

    /**
     * @brief Tests the Initialise method
     */
    bool TestInitialise();

    /**
     * @brief Tests the Initialise method with an ErrorMessage
     */
    bool TestInitialise_ErrorMessage();

    /**
     * @brief Tests the Initialise method with more than one message defined
     */
    bool TestInitialise_False_MoreThanOneErrorMessage();

    /**
     * @brief Tests the Initialise method with an invalid message
     */
    bool TestInitialise_False_InvalidMessage();

    /**
     * @brief Tests GAMScheduler integrated with the StateMachine
     */
    bool TestIntegrated();

    /**
     * @brief Tests GAMScheduler integrated with the StateMachine
     */
    bool TestIntegrated_TriggerErrorMessage();

    /**
     * @brief Tests GAMScheduler integrated with NoWait = 0
     */
    bool TestIntegrated_WaitForAll();

    /**
     * @brief Tests GAMScheduler integrated with NoWait = 1
     */
    bool TestIntegrated_NoWait();

    /**
     * @brief Tests that the purge kills all threads
     */
    bool TestPurge();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MARTE2_TEST_CORE_SCHEDULER_L5GAMS_FASTSCHEDULERTEST_H_ */

