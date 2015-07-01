/**
 * @file MutexIntegrationTest.h
 * @brief Header file for class MutexIntegrationTest
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

 * @details This header file contains the declaration of the class MutexIntegrationTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUTEXINTEGRATIONTEST_H_
#define 		MUTEXINTEGRATIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MutexSem.h"
#include "Threads.h"
#include "Sleep.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/




/**
 * @brief A class to testing the MutexSem functions.
 */
class MutexIntegrationTest {

private:

    static const int32 N_MUTEX = 2;


public:

    /** One mutex is used for normal tests, the other is necessary to implement the deadlock condition */
    MutexSem mutexSem[N_MUTEX];

    /** Shared Variable between Threads */
    int32 sharedVariable;

    /** It's a shared variable to allows to the main process to understand when all threads are terminated */
    int32 exitCondition;

    /** A boolean for the tests with mutex with timeouts. It begins true when the mutex fail because the timeout */
    bool expired;

    /** It's true when we want use fast mutex */
    bool usingFast;

    /** It's used to check if it's possible that a thread unlock a mutex locked by another thread */
    bool free;

    /** It's used by a function to cause the deadlock condition */
    bool deadlock;

    /** Variable used to store the timeout value */
    TimeoutType timeout;


    /**
     * @brief Constructor.
     */
    MutexIntegrationTest();

    /**
     * @brief Tests the semaphore without timeout.
     *
     * @details Launchs nOfThreads threads that increment of 10 a shared variable. During this operation a mux is used and for each increment the thread controls that the shared variable is consistent, otherwise
     * return. It returns true if the shared variable is equal to 10*nOfThreads.
     *
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the shared variable remains consistent between locks and unlocks.
     */
    bool TestSync(int32 nOfThreads);

    /**
     * @brief Tests the semaphore with timeout.
     *
     * @details Launchs nOfThreads threads and each of them tries a lock on a mutex with the timeout passed by argument. The first thread that lock the mutex loops until the timeout is expired, after that,
     * the lock fail and all threads call the increment function. Also in this case it returns true if the shared variable is equal to 10*nOfThreads, otherwise if the timeout does not happen, the first
     * thread exit from the cycle modifying the shared variable, so the return value is not correct and the function return false.
     *
     * @param[in] timeout is the timeout in ms for the mutex, nOfThreads is the number of threads to launch.
     * @param[in] nOfThreads is the number of threads to be launched.
     *
     * @return true if after timeout the mutex fail, false if too much time is elapsed.
     */
    bool TestSyncTimeout(TimeoutType timeout, int32 nOfThreads);

    /**
     * @brief The same of TestSync with MutexSem::FastLock and MutexSem::FastUnLock functions.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the shared variable remains consistent between locks and unlocks.
     */
    bool TestSyncFast(int32 nOfThreads);

    /**
     * @brief The same of TestSyncTimeout() with MutexSem::FastLock and MutexSem::FastUnLock functions.
     * @param[in] timeout is the timeout in ms for the mutex, nOfThreads is the number of threads to launch.
     * @param[in] nOfThreads is the number of threads to be launched.
     * @return true if after timeout the mutex fail, false if too much time is elapsed.
     */
    bool TestSyncTimeoutFast(TimeoutType timeout, int32 nOfThreads);

    /**
     * @brief Checks if it is possible that a thread can unlock the mutex locked by another thread.
     * @return true if the unlock fail, false otherwise.
     */
    bool FakeLock();

    /**
     * @brief Implements a DeadLock. Check if it's possible a deadlock situation.
     * @return true if there is a deadlock condition, this means that threads will remain in an infinite loop.
     */
    bool DeadLock();


};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXINTEGRATIONTEST_H_ */

