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
#include "BasicGAMScheduler.h"

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
     * @brief Tests the constructor
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function inserts a new GAMSchedulerRecord.
     */
    bool TestInsertRecord();

    /**
     * @brief Tests if the function creates the accelerator to the next GAMSchedulerRecord.
     */
    bool TestPrepareNextState();

    /**
     * @brief Tests if the function fails when the next state name is not found.
     */
    bool TestPrepareNextStateFalse_InvalidNextState();

    /**
     * @brief Tests if the function fails if the application is not set.
     */
    bool TestPrepareNextStateFalse_NoAppSet();

    /**
     * @brief Tests if the function sets the application.
     */
    bool TestSetApplication();

    /**
     * @brief Tests if the function stops the current execution and starts a new one.
     */
    bool TestChangeState();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULERITEST_H_ */

