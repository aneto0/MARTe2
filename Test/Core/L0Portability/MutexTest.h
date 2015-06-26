/**
 * @file MutexTest.h
 * @brief Header file for class MutexTest
 * @date 25/06/2015
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

 * @details This header file contains the declaration of the class MutexTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUTEXTEST_H_
#define 		MUTEXTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the MutexSem functions
 */
class MutexTest {
public:

    /**
     * Mutex object.
     */
    MutexSem mutexSem;

    /**
     * @brief Constructor.
     * @details Creates the mutex handle.
     */
    MutexTest();

    /**
     * @brief Destructor.
     * @details Closes the mutex handle.
     */
    ~MutexTest();

    /**
     * @brief Tests the MutexSem::Lock function.
     * @details If the mutex is not recursive  a double lock causes a deadlock. In this case the second lock function should fail because of the timeout.
     * @param[in] timeout is the desired timeout.
     */
    bool TestLock(TimeoutType timeout);

    /**
     * @brief Tests the MutexSem::UnLock function.
     * @details After the unlock function, the thread should be able to lock again without errors.
     */
    bool TestUnLock();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXTEST_H_ */

