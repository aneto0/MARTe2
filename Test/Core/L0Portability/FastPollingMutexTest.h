/**
 * @file FastPollingMutexTest.h
 * @brief Header file for class FastPollingMutexTest
 * @date 26/06/2015
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

 * @details This header file contains the declaration of the class FastPollingMutexTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTPOLLINGMUTEXTEST_H_
#define 		FASTPOLLINGMUTEXTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "FastPollingMutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/**
 * @brief A class to test FastPollingMutexSem functions
 */
class FastPollingMutexTest {

public:

    /**
     * The semaphore object.
     */
    FastPollingMutexSem fastSem;

    /**
     * @brief Constructor.
     */
    FastPollingMutexTest();

    /**
     * @brief Tests the FastPollingMutexSem::FastLock function.
     * @details The second lock should return false after the timeout.
     * @param[in] timeout is the desired timeout.
     * @return true if successful, false otherwise.
     */
    bool TestLock(TimeoutType timeout);

    /**
     * @brief Tests the FastPollingMutexSem::FasUnLock function.
     * @details After the unlock the lock function should have success.
     * @return true if successful, false otherwise.
     */
    bool TestUnLock();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FASTPOLLINGMUTEXTEST_H_ */

