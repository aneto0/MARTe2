/**
 * @file BasicGAMSchedulerTest.h
 * @brief Header file for class BasicGAMSchedulerTest
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

 * @details This header file contains the declaration of the class BasicGAMSchedulerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICGAMSCHEDULERTEST_H_
#define BASICGAMSCHEDULERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicGAMScheduler.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the BasicGAMScheduler functions.
 */
class BasicGAMSchedulerTest {

public:

    /**
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function initialises correctly the object
     * from a StructuredDataI.
     */
    bool TestInitialise();

    /**
     * @brief Tests if the function performs correctly an execution of a two GAMs thread
     * cycle.
     */
    bool TestStartExecution();

    /**
     * @brief Tests if is this function is called inside a thread the execution will terminate.
     */
    bool TestStopExecution();

    /**
     * @brief Tests if the scheduler writes the absolute execution time of each GAM.
     */
    bool TestGAMAbsoluteExecutionTime();

    /**
     * @brief Tests if the scheduler writes the relative execution time of each GAM.
     */
    bool TestGAMRelativeExecutionTime();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICGAMSCHEDULERTEST_H_ */

