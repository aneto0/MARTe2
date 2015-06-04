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
 * @file FastPollingMutexTest.h
 * @brief Tests the MutexSem and associated functions.
 *
 * The test consists in concurrent threads incrementing a shared variable.
 * These will only be allowed to change this variable value when the semaphore is locked.
 */

#ifndef FAST_POLLING_MUTEX_TEST_H
#define FAST_POLLING_MUTEX_TEST_H

#include "FastPollingMutexSem.h"
#include "Threads.h"
#include "Sleep.h"

#define N_MUTEX 2

/** @brief Class for testing of FastPollingMutexSem functions. */
class FastPollingMutexTest {

private:

public:

    /**One mutex is used for normal tests, the other is necessary to implement the deadlock condition**/
    FastPollingMutexSem fastMutexSem[N_MUTEX];

    /**Shared Variable between Threads**/
    int32 sharedVariable;

    /**It's a shared variable to allows to the main process to understand when all threads are terminated**/
    int32 exitCondition;

    /**A boolean for the tests with mutex with timeouts. It begins true when the mutex fail because the timeout**/
    bool expired;

    /**It's used to check if it's possible that a thread unlock a mutex locked by another thread**/
    bool free;

    /**It's used by a function to cause the deadlock condition**/
    bool deadlock;
    TimeoutType timeout;

    /** @brief Constructor. */
    FastPollingMutexTest() {
        sharedVariable = 0;
        exitCondition = 0;
        timeout = TTInfiniteWait;
        expired = false;
        free = false;
        deadlock = false;
        for (int32 i = 0; i < N_MUTEX; i++) {
            fastMutexSem[i].Create(False);
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
     * Launchs nOfThreads threads and each of them tries a lock on a mutex with the timeout passed by argument. The first thread that locks the mutex goes in an infinite loop, while other threads after 
     * the lock fail because the timeout, increment a variable. Also in this case it returns true if the shared variable is equal to nOfThreads-1, otherwise if the timeout does not happen, the function 
     * return false. 
     *
     * @param timeout is the timeout in ms for the mutex, nOfThreads is the number of threads to launch. 
     * @return true if after timeout the mutex fail, false if too much time is elapsed. **/
    bool TestSyncTimeout(TimeoutType timeout, int32 nOfThreads);

    /**
     * @brief Checks if it is possible that a thread can unlock the mutex locked by another thread. This should be possible.
     * @return true if the unlock works, false otherwise. **/
    bool FakeLock();

    /**
     * @brief Implements a DeadLock. Checks if it is possible a deadlock situation.
     * @return true if a there is a deadlock condition, this means that threads remains in an infinite loop. **/
    bool DeadLock();

    /**
     * @brief Checks what happen when a thread were killed while it locks a mutex. 
     * @return true if the tread is killed after the unlock, because otherwise we can obtain a program crash. **/
    bool KillWithLock();

};

#endif
