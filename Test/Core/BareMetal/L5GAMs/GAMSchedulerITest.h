/**
 * @file GAMSchedulerITest.h
 * @brief Header file for class GAMSchedulerITest
 * @date 04/04/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class GAMSchedulerITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSCHEDULERITEST_H_
#define GAMSCHEDULERITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMSchedulerI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the GAMSchedulerI functions
 */
class GAMSchedulerITest {

public:

	/**
	 * @brief Initialise ObjectRegistryDatabase
	 */
    GAMSchedulerITest();

	/**
	 * @brief Destroys ObjectRegistryDatabase
	 */
    ~GAMSchedulerITest();

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function configures correctly the scheduler
     */
    bool TestConfigureScheduler();

    /**
     * @brief Tests if the scheduler configuration fails if not a RealTimeState is present
     * in States container
     */
    bool TestConfigureSchedulerFalse_InvalidState();

    /**
     * @brief Tests if the function returns the number of executables in the specified state
     */
    bool TestGetNumberOfExecutables();

    /**
     * @brief Tests if the function prepares the set of threads to be executed in the next state
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests if the function executes a cycle of Executables defined in a RealTimeThreads
     */
    bool TestExecuteSingleCycle();

    /**
     * @brief Tests that the ExecuteSingleCycle fails if a broker returns false (increase the coverage).
     */
    bool TestExecuteSingleCycle_False();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULERITEST_H_ */

