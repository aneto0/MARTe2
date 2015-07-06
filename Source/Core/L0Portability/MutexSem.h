/**
 * @file MutexSem.h
 * @brief Header file for class MutexSem
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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
#define MUTEXSEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,MutexSemOS.h)
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
 * @details The Lock functions uses an ErrorType object defined in GeneralDefinition.h that could be used by the user to
 * know if an eventual lock error happened because of the timeout or for other reasons.
 *
 * @details Most of the implementation is delegated to MutexSemOS.h which provides system level functions for mutex semaphores.
 *
 */
class MutexSem {

public:
    /**
     * @brief Copy constructor.
     * @param[in] h is the semaphore handle to be copied in this handle.
     */
    MutexSem(const MutexSem &source);

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
     * @details We can specify using the "recursive" parameter if we want to create a recursive or notmal mutex.
     * Anyway not all the operating systems supports both this modes, then the
     * function could ignore the "recursive" argument. Use MutexSem::IsRecursive to know if the created mutex is recursive or not.
     * @param[in] locked is the desired initial state: true = locked, false = unlocked.
     * @param[in] recursive specifies if the mutex should be created recursive or not.
     * @return false if something in the system level mutex initialization fails.
     */
    bool Create(bool locked=false, bool recursive=false);

    /**
     * @brief Close the semaphore handle.
     * @return true if successful, false otherwise.
     */
    bool Close();

    /**
     * @brief Lock the semaphore until the desired timeout.
     * @param[in] msecTimeout is the desired timeout.
     * @param[out] error specifies the error type in case of errors (timeout fail or other error types).
     * @return false if something in the lock system level function goes wrong.
     */
    bool Lock(const TimeoutType &msecTimeout = TTInfiniteWait, Error &error=Global::errorType) const;

    /**
     * @brief Unlock the semaphore.
     * @return true if the unlock system level function return true, false otherwise.
     */
    bool UnLock() const;

    /**
     * @brief Fast lock.
     * @param[in] msecTimeout is the desired timeout.
     * @param[out] error specifies the error type in case of errors (timeout fail or other error types).
     * @return true if successful, false otherwise.
     */
    inline bool FastLock(const TimeoutType &msecTimeout = TTInfiniteWait, Error &error=Global::errorType) const;

    /**
     * @brief Fast unlock.
     * @return true if successful, false otherwise.
     */
    inline bool FastUnLock() const;

    /**
     * @brief Tries to lock the semaphore and if it is already locked returns immediately.
     * @return true if the semaphore was unlocked and the function locks it, false if it was already locked.
     */
    inline bool FastTryLock() const;

    /**
     * @brief Returns true if the mutex is recursive, false otherwise.
     * @details The mutex implementation could be different for each operating system. If the created mutex is not recursive, this means that
     * if the same thread locks it twice it triggers a deadlock condition.
     * @return true if the mutex is recursive, false otherwise.
     */
    inline bool IsRecursive() const;

    /**
     * @brief Getter for the semaphore handle.
     * @return the semaphore internal handle.
     */
    const HANDLE GetHandle() const;

private:

    /** true if the mutex is recursive, false otherwise. */
    bool isRecursive;

    /** The handle associated to the semaphore. */
    const HANDLE semH;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool MutexSem::FastLock(const TimeoutType &msecTimeout, Error &error) const {
    return MutexSemOS::FastLock(semH, msecTimeout, error);
}

bool MutexSem::FastUnLock() const {
    return MutexSemOS::FastUnLock(semH);
}

bool MutexSem::FastTryLock() const {
    return MutexSemOS::FastTryLock(semH);
}

bool MutexSem::IsRecursive() const {
    return isRecursive;
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXSEM_H_ */

