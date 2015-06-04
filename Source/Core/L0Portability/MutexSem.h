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
 * $Id: $
 *
 **/

/**
 * @file
 * @brief Mutex semaphore implementation.
 */

#ifndef MUTEX_SEM
#define MUTEX_SEM

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,MutexSemOS.h)
#include "SemCore.h"


/**
 * @brief Mutex Semaphore with functions to allows creation, destruction and lock-unlock of the semaphore.
 * 
 * This type of semaphore is very useful for thread syncronization due to protect critical sections of code
 * like for example multiple accesses to the same shared memory location avoiding race conditions or not consistent datas.
 *
 * Most of the implementation is delegated to MutexSemOS.h which provides system level functions for mutex semaphores.
 *
 * Mutex semaphores are generally used in multithreading applications to guarantee exclusive access in critical code sections
 * shared by different threads.
 */


/** a mutual exclusion semaphore */
class MutexSem : public SemCore {

public:
    /** @brief Constructor.
      * @param h is a pointer to the semaphore structure. */
    MutexSem(HANDLE h) {
        Init(h);
    }

    /** @brief Default constructor. */
    MutexSem() {
    }

    /** @brief Destructor. */
    ~MutexSem() {
        Close();
    }

    /** @brief Open the semafore with a given initial state.
      * @param locked is the desired initial state: true = locked, false = unlocked.
      * @return false if something in the system level mutex initialization goes wrong. */
    bool Create(bool locked=False) {
        return MutexSemOSCreate(semH, locked);
    }

    /** @brief Close the semafore handle. 
      * @return true. */
    bool Close() {
        return MutexSemOSClose(semH);
    }

    /** @brief Lock the semafore.
      * @return false if something in the lock system level function goes wrong. */
    bool Lock(TimeoutType msecTimeout = TTInfiniteWait) {
        return MutexSemOSLock(semH, msecTimeout);
    }

    /** @brief Unlock the semaphore.
      * @return true if the unlock system level function return true. */
    bool UnLock() {
        return MutexSemOSUnLock(semH);
    }

    /** @see Lock() */
    inline bool FastLock(TimeoutType msecTimeout = TTInfiniteWait) {
        return MutexSemOSFastLock(semH, msecTimeout);
    }

    /** @see UnLock() */
    inline bool FastUnLock() {
        return MutexSemOSFastUnLock(semH);
    }

    /** @brief Try to lock the semaphore and if it is already locked returning immediately.
      * @return true if the semaphore was unlocked and the function locks it, false if it was already locked. */
    inline bool FastTryLock() {
        return MutexSemOSFastTryLock(semH);
    }
};
#endif

