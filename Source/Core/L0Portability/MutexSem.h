/**
 * @file MutexSem.h
 * @brief Header file for class MutexSem
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class MutexSem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUTEXSEM_H_
#define 		MUTEXSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,MutexSemOS.h)
#include "SemCore.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Mutual exclusive semaphore functions to allow creation, destruction and lock-unlock of the semaphore.
 *
 * @details This type of semaphore is very useful for thread synchronization due to protect critical sections of code
 * like for example multiple accesses to the same shared memory location avoiding race conditions or not consistent datas.\n
 * Mutex semaphores are generally used in multi-threading applications to guarantee exclusive access in critical code sections
 * shared by different threads.
 *
 * @details Most of the implementation is delegated to MutexSemOS.h which provides system level functions for mutex semaphores.
 *
 */
class MutexSem : public SemCore {

public:
    /**
     * @brief Copy constructor.
     * @param[in] h is the semaphore handle to be copied in this handle.
     */
    MutexSem(HANDLE h);

    /**
     * @brief Default constructor.
     */
    MutexSem();

    /**
     * @brief Destructor.
     */
    ~MutexSem();

    /**
     * @brief Open the semaphore with a given initial state.
     * @param[in] locked is the desired initial state: true = locked, false = unlocked.
     * @return false if something in the system level mutex initialization goes wrong.
     */
    bool Create(bool locked=False);

    /**
     * @brief Close the semaphore handle.
     * @return true.
     */
    bool Close();

    /**
     * @brief Lock the semaphore until the desired timeout.
     * @param[in] msecTimeout is the desired timeout.
     * @return false if something in the lock system level function goes wrong.
     */
    bool Lock(TimeoutType msecTimeout = TTInfiniteWait);

    /**
     * @brief Unlock the semaphore.
     * @return true if the unlock system level function return true.
     */
    bool UnLock();

    /**
     * @brief Fast lock.
     * @return true if successful, false otherwise.
     */
    inline bool FastLock(TimeoutType msecTimeout = TTInfiniteWait);

    /**
     * @brief Fast unlock.
     * @return true if successful, false otherwise.
     */
    inline bool FastUnLock();

    /**
     * @brief Tries to lock the semaphore and if it is already locked returns immediately.
     * @return true if the semaphore was unlocked and the function locks it, false if it was already locked.
     */
    inline bool FastTryLock();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool MutexSem::FastLock(TimeoutType msecTimeout) {
    return MutexSemOS::FastLock(semH, msecTimeout);
}

bool MutexSem::FastUnLock() {
    return MutexSemOS::FastUnLock(semH);
}

bool MutexSem::FastTryLock() {
    return MutexSemOS::FastTryLock(semH);
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXSEM_H_ */

