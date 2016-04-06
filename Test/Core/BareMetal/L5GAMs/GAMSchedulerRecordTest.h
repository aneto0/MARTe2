/**
 * @file GAMSchedulerRecordTest.h
 * @brief Header file for class GAMSchedulerRecordTest
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

 * @details This header file contains the declaration of the class GAMSchedulerRecordTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSCHEDULERRECORDTEST_H_
#define GAMSCHEDULERRECORDTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAMSchedulerRecord.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the GAMSchedulerRecord functions.
 */
class GAMSchedulerRecordTest {

public:

    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the functions add threads correctly.
     */
    bool TestAddThread();

    /**
     * @brief Tests if the function retrieves the thread in the
     * specified position.
     */
    bool TestPeek();

    /**
     * @brief Tests if the function returns an invalid reference trying
     * to access to a position greater than the number of threads inserted.
     */
    bool TestPeekOutOfRange();

    /**
     * @brief Tests if the function returns the correct number of threads inserted.
     */
    bool TestNumberOfThreads();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSCHEDULERRECORDTEST_H_ */

