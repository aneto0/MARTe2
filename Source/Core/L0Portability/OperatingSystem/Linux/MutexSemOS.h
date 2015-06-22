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

#include "../../TimeoutType.h"
#include <pthread.h>
#include <math.h>
#include <sys/timeb.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Private mutex semaphore class used for Solaris and Linux when using pThread.
 */
class PrivateMutexSemStruct {

public:
    /**
     * @brief Constructor.
     */
    PrivateMutexSemStruct() {
    }
    /**
     * @brief Destructor.
     */
    ~PrivateMutexSemStruct() {
    }

    /**
     * @brief Initialize the semaphore with the right attributes.
     * @return false if something wrong with pthread_mutex initialization.
     */
    bool Init() {
        if (pthread_mutexattr_init(&mutexAttributes) != 0)
            return False;
        if (pthread_mutexattr_setprotocol(&mutexAttributes, PTHREAD_PRIO_INHERIT) != 0)
            return False;
        //This was pthread PTHREAD_MUTEX_RECURSIVE but it was crashing when a deadlock was forced on purpose
        //with PTHREAD_MUTEX_NORMAL the same thread cannot lock the semaphore without unlocking it first.
        if (pthread_mutexattr_settype(&mutexAttributes, PTHREAD_MUTEX_NORMAL) != 0)
            return False;
        if (pthread_mutex_init(&mutexHandle, &mutexAttributes) != 0)
            return False;
        return True;
    }

    /**
     * @brief Destroy the semaphore.
     * @return false if something wrong in pthread_mutex destruction.
     */
    bool Close() {
        if (!pthread_mutexattr_destroy(&mutexAttributes))
            return False;
        if (!pthread_mutex_destroy(&mutexHandle))
            return False;
        return True;
    }

    /**
     * @brief Lock the semaphore until an unlock or the timeout expire.
     * @details The thread that locks a semaphore cannot be killed in the critical region.
     * @param[in] msecTimeout is the desired timeout.
     * @return false if lock fails also because the expire of the timeout, true otherwise.
     */
    bool Lock(TimeoutType msecTimeout = TTInfiniteWait) {
        if (msecTimeout == TTInfiniteWait) {
            if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0)
                return False;

            if (pthread_mutex_lock(&mutexHandle) != 0)
                return False;
        }
        else {
            struct timespec timesValues;
            timeb tb;
            ftime(&tb);
            double sec = ((msecTimeout.msecTimeout + tb.millitm) * 1e-3 + tb.time);
            double roundValue = floor(sec);
            timesValues.tv_sec = (int) roundValue;
            timesValues.tv_nsec = (int) ((sec - roundValue) * 1E9);
            int err = 0;
            if ((err = pthread_mutex_timedlock(&mutexHandle, &timesValues)) != 0) {
                return False;
            }
        }
        return True;
    }

    /**
     * @brief Unlock the semaphore.
     * @return true if the unlock has success.
     * @details Enable the possibility to kill the thread after the unlock.
     */
    bool UnLock() {
        bool condition = (pthread_mutex_unlock(&mutexHandle) == 0);
        return condition && pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) == 0;
    }

    /**
     * @brief Fast return in case of locked semaphore.
     * @return true if the semaphore was unlocked and locks it, otherwise return false.
     */
    bool TryLock() {
        return (pthread_mutex_trylock(&mutexHandle) == 0);
    }

private:
    /**  Mutex Handle */
    pthread_mutex_t mutexHandle;
    /** Mutex Attributes */
    pthread_mutexattr_t mutexAttributes;

};

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
    static bool Create(HANDLE &semH,
                       bool locked) {
        if (semH != (HANDLE) NULL) {
            delete (PrivateMutexSemStruct *) semH;
        }
        // Create the Structure
        semH = (HANDLE) new PrivateMutexSemStruct();
        if (semH == (HANDLE) NULL) {
            return False;
        }
        // Initialize the Semaphore
        bool ret = ((PrivateMutexSemStruct *) semH)->Init();
        if (!ret) {
            delete (PrivateMutexSemStruct *) semH;
            semH = (HANDLE) NULL;
            return False;
        }
        if (locked == True) {
            ((PrivateMutexSemStruct *) semH)->Lock(TTInfiniteWait);
        }

        return True;
    }

    /**
     * @brief Close the semafore handle.
     * @details Called by MutexSem::Close
     * @param[in,out] semH is the mutex semaphore handle.
     * @return true.
     */
    static inline bool Close(HANDLE &semH) {
        if (semH == (HANDLE) NULL) {
            return True;
        }
        semH = (HANDLE) NULL;
        return True;
    }

    /**
     * @brief Lock the semafore.
     * @details Called by MutexSem::Lock
     * @param[in,out] semH is the mutex semaphore handle.
     * @param[in] msecTimeout is the desired timeout.
     * @return the result of PrivateMutexSemStruct::Lock
     */
    static inline bool Lock(HANDLE &semH,
                            TimeoutType msecTimeout) {
        if (semH == (HANDLE) NULL) {
            return False;
        }
        return ((PrivateMutexSemStruct *) semH)->Lock(msecTimeout);
    }

    /**
     * @brief Unlock the semaphore.
     * @details MutexSem::UnLock
     * @param[in,out] semH is the mutex semaphore handle.
     * @return the return of PrivateMutexSemStruct::UnLock.
     */
    static inline bool UnLock(HANDLE &semH) {
        if (semH == (HANDLE) NULL) {
            return False;
        }
        return ((PrivateMutexSemStruct *) semH)->UnLock();
    }

    /**
     * @see MutexSem::Lock
     * @see MutexSemOSLock.
     */
    static inline bool FastLock(HANDLE &semH,
                                TimeoutType msecTimeout) {
        if (semH == (HANDLE) NULL) {
            return False;
        }
        return ((PrivateMutexSemStruct *) semH)->Lock(msecTimeout);
    }

    /**
     * @see MutexSem::UnLock
     * @see MutexSemOS::UnLock.
     */
    static inline bool FastUnLock(HANDLE &semH) {
        if (semH == (HANDLE) NULL) {
            return False;
        }
        return ((PrivateMutexSemStruct *) semH)->UnLock();
    }

    /**
     * @brief Fast return in case of locked semaphore.
     * @details called by MutexSem::FastTryLock.
     * @param[in,out] semH is the mutex semaphore handle.
     * @return the result of PrivateMutexSemStruct::FastLock.
     */
    static inline bool FastTryLock(HANDLE &semH) {
        if (semH == (HANDLE) NULL) {
            return False;
        }
        return ((PrivateMutexSemStruct *) semH)->TryLock();
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXSEMOS_H_ */

