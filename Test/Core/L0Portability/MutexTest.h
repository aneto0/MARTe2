/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:$
 *
 **/
/**
 * @file MutexTest.h
 * @brief Tests the MutexSem and associated functions.
 *
 * The test consists in concurrent threads incrementing a shared variable.
 * These will only be allowed to change this variable value when the semaphore is locked.
 */

#ifndef MUTEX_TEST_H
#define MUTEX_TEST_H

#include "MutexSem.h"
#include "Threads.h"
#include "Sleep.h"

#define N_MUTEX 2

/** @brief A class for testing of MutexSem functions. */
class MutexTest {

private:

public:

    /**One mutex is used for normal tests, the other is necessary to implement the deadlock condition**/
    MutexSem mutexSem[N_MUTEX];

    /**Shared Variable between Threads**/
    int32 sharedVariable;

    /**It's a shared variable to allows to the main process to understand when all threads are terminated**/
    int32 exitCondition;

    /**A boolean for the tests with mutex with timeouts. It begins true when the mutex fail because the timeout**/
    bool expired;

    /**It's true when we want use fast mutex**/
    bool usingFast;

    /**It's used to check if it's possible that a thread unlock a mutex locked by another thread**/
    bool free;

    /**It's used by a function to cause the deadlock condition**/
    bool deadlock;
    TimeoutType timeout;

    MutexTest() {
        sharedVariable = 0;
        exitCondition = 0;
        timeout = TTInfiniteWait;
        usingFast = false;
        expired = false;
        free = false;
        deadlock = false;
        for (int32 i = 0; i < N_MUTEX; i++) {
            mutexSem[i].Create(False);
        }
    }

    /**
     * @brief Tests the semaphore without timeout.
     *
     * Launchs nOfThreads threads that increment of 10 a shared variable. During this operation a mux is used and for each increment the thread controls that the shared variable is consistent, otherwise   
     * return. It returns true if the shared variable is equal to 10*nOfThreads. 
     *
     * @param nOfThreads is the number of threads to launch. 
     * @return true if the shared variable remains consistent between locks and unlocks. **/
    bool TestSync(int32 nOfThreads);

    /**
     * @brief Tests the semaphore with timeout.
     * 
     * Launchs nOfThreads threads and each of them tries a lock on a mutex with the timeout passed by argument. The first thread that lock the mutex loops until the timeout is expired, after that, 
     * the lock fail and all threads call the increment function. Also in this case it returns true if the shared variable is equal to 10*nOfThreads, otherwise if the timeout does not happen, the first 
     * thread exit from the cycle modifying the shared variable, so the return value is not correct and the function return false. 
     *
     * @param timeout is the timeout in ms for the mutex, nOfThreads is the number of threads to launch. 
     * @return true if after timeout the mutex fail, false if too much time is elapsed. **/
    bool TestSyncTimeout(TimeoutType timeout, int32 nOfThreads);

    /**
     * @brief The same of TestSync() with FastLock() and FastUnLock() functions. 
     * @param nOfThreads is the number of threads to launch. 
     * @return true if the shared variable remains consistent between locks and unlocks. **/
    bool TestSyncFast(int32 nOfThreads);

    /**
     * @brief The same of TestSyncTimeout() with FastLock() and FastUnLock() functions.
     * @param timeout is the timeout in ms for the mutex, nOfThreads is the number of threads to launch. 
     * @return true if after timeout the mutex fail, false if too much time is elapsed. **/
    bool TestSyncTimeoutFast(TimeoutType, int32);

    /**
     * @brief Checks if it is possible that a thread can unlock the mutex locked by another thread. 
     * @return true if the unlock fail, false otherwise. **/
    bool FakeLock();

    /**
     * @brief Implements a DeadLock. Check if it's possible a deadlock situation.
     * @return true if a there is a deadlock condition, this means that threads remains in an infinite loop. **/
    bool DeadLock();

    /**
     * @brief Checks what happen when a thread were killed while it lock a mutex. 
     * @return true if the tread is killed after the unlock, because otherwise we can obtain a program crash. **/
    bool KillWithLock();

};

#endif
