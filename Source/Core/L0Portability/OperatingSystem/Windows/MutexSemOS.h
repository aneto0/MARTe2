/**
 * @file MutexSemOS.h
 * @brief Header file for class MutexSemOS
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

 * @details This header file contains the declaration of the class MutexSemOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUTEXSEMOS_H_
#define 		MUTEXSEMOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../../TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief System dependent implementation of a mutual exclusive semaphore.
 */
class MutexSemOS {

public:

    /**
     * @brief Create a mutex semafore with a given initial state
     * @details Called by MutexSem::Create.
     * @param[in,out] semH is the mutex semaphore handle in return.
     * @param[in] locked defines the initial state (true = locked, false = unlocked).
     * @return false if the new or Init fail, true otherwise
     */
    static inline bool Create(HANDLE &semH,
                              bool locked) {
        semH = CreateMutex(NULL, (locked == True), NULL);
        return (semH != NULL);
    }

    /**
     * @brief Close the semafore handle.
     * @details Called by MutexSem::Close
     * @param[in,out] semH is the mutex semaphore handle.
     * @return true.
     */
    static inline bool Close(HANDLE &semH) {
        if (semH == (HANDLE) NULL)
            return True;
        if (CloseHandle(semH) == FALSE) {
            return False;
        }
        semH = NULL;
        return True;
    }

    /**
     * @brief Lock the semafore.
     * @details Called by MutexSem::Lock
     * @param[in,out] semH is the mutex semaphore handle.
     * @param[in] msecTimeout is the desired timeout.
     * @param[out] error specified if the error is because of the timeout or not.
     * @return the result of PrivateMutexSemStruct::Lock
     */
    static inline bool Lock(HANDLE &semH,
                            TimeoutType msecTimeout,
                            Error &error) {
        DWORD ret = WaitForSingleObject(semH, msecTimeout.msecTimeout);
        if (ret == WAIT_FAILED) {
            error = OSError;
            return False;
        }
        if (ret == WAIT_TIMEOUT) {
            error = Timeout;
            return False;
        }
        return True;
    }

    /**
     * @brief Unlock the semaphore.
     * @details MutexSem::UnLock
     * @param[in,out] semH is the mutex semaphore handle.
     * @return the return of PrivateMutexSemStruct::UnLock.
     */
    static inline bool UnLock(HANDLE &semH) {
        if (ReleaseMutex(semH) == FALSE) {
            return False;
        }
        return True;
    }

    /**
     * @brief MutexSem::Lock MutexSemOS::Lock.
     */
    static inline bool FastLock(HANDLE &semH,
                                TimeoutType msecTimeout,
                                Error &error) {
        int ret = WaitForSingleObject(semH, msecTimeout.msecTimeout);
        if (ret == WAIT_FAILED) {
            error = OSError;
            return False;
        }
        if (ret == WAIT_TIMEOUT) {
            error = Timeout;
            return False;
        }
    }

    /**
     * @brief MutexSem::UnLock MutexSemOS::UnLock.
     */
    static inline bool FastUnLock(HANDLE &semH) {
        return (ReleaseMutex(semH) == TRUE);
    }

    /**
     * @brief Fast return in case of locked semaphore.
     * @details called by MutexSem::FastTryLock.
     * @param[in,out] semH is the mutex semaphore handle.
     * @return the result of PrivateMutexSemStruct::FastLock.
     */
    static inline bool FastTryLock(HANDLE &semH) {
        int ret = WaitForSingleObject(semH, 0);
        return ((ret != (int) WAIT_FAILED) && (ret != (int) WAIT_TIMEOUT));
    }
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXSEMOS_H_ */

