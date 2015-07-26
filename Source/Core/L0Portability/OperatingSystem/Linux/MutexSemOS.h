/**
 * @file MutexSemOS.h
 * @brief Header file for class MutexSemOS
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

 * @details This header file contains the declaration of the class MutexSemOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MUTEXSEMOS_H_
#define MUTEXSEMOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <pthread.h>
#include <math.h>
#include <sys/timeb.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Private mutex semaphore class used for Solaris and Linux when using pThread.
 */
class PrivateMutexSemStruct {

private:

    /**
     * @brief Default constructor.
     */
    PrivateMutexSemStruct() {
        references = 1;
        closed = false;
    }

    /**
     * @brief Destructor.
     */
    ~PrivateMutexSemStruct() {
    }

    /**
     * @brief Initialize the semaphore with the right attributes.
     * @param[in] recursive specifies if the mutex created is recursive or not
     * @return false if something wrong with pthread_mutex initialization.
     */
    bool Init(bool &recursive) {
        closed = false;
        if (pthread_mutexattr_init(&mutexAttributes) != 0) {
            return false;
        }
        if (pthread_mutexattr_setprotocol(&mutexAttributes,
                                          PTHREAD_PRIO_INHERIT) != 0) {
            return false;
        }
        if (recursive) {
            //The deadlock condition causes a crash at operating system level.
            if (pthread_mutexattr_settype(&mutexAttributes,
                                          PTHREAD_MUTEX_RECURSIVE) != 0) {
                return false;
            }
        }
        else {
            //This was pthread PTHREAD_MUTEX_RECURSIVE but it was crashing when a deadlock was forced on purpose
            //with PTHREAD_MUTEX_NORMAL the same thread cannot lock the semaphore without unlocking it first.
            if (pthread_mutexattr_settype(&mutexAttributes,
                                          PTHREAD_MUTEX_NORMAL) != 0) {
                return false;
            }
        }
        if (pthread_mutex_init(&mutexHandle, &mutexAttributes) != 0) {
            return false;
        }
        return true;
    }

    /**
     * @brief Destroy the semaphore.
     * @return false if something wrong in pthread_mutex destruction.
     */
    bool Close() {
        if (closed) {
            return true;
        }
        UnLock();
        closed = true;
        if (pthread_mutexattr_destroy(&mutexAttributes) != 0) {
            return false;
        }
        if (pthread_mutex_destroy(&mutexHandle) != 0) {
            return false;
        }
        return true;
    }

    /**
     * @brief Lock the semaphore until an unlock or the timeout expire.
     * @details The thread that locks a semaphore cannot be killed in the critical region.
     * @param[in] msecTimeout is the desired timeout.
     * @param[out] error is the error type.
     * @return false if lock fails also because the expire of the timeout, true otherwise.
     */
    bool Lock(TimeoutType msecTimeout, FlagsType &error) {
        if (closed) {
            return false;
        }
        if (msecTimeout == TTInfiniteWait) {
            if (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL) != 0) {
                error = Errors::OSError;
                return false;
            }

            if (pthread_mutex_lock(&mutexHandle) != 0) {
                error = Errors::OSError;
                return false;
            }
        }
        else {
            struct timespec timesValues;
            timeb tb;
            ftime(&tb);
            float64 sec = ((msecTimeout.GetTimeoutMSec() + tb.millitm) * 1e-3
                    + tb.time);
            float64 roundValue = floor(sec);
            timesValues.tv_sec = (int) roundValue;
            timesValues.tv_nsec = (int) ((sec - roundValue) * 1E9);
            int err = 0;
            if ((err = pthread_mutex_timedlock(&mutexHandle, &timesValues))
                    != 0) {
                error = Errors::Timeout;
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Unlock the semaphore.
     * @details Enable the possibility to kill the thread after the unlock.
     * @return true if the unlock has success.
     */
    bool UnLock() {
        if (closed) {
            return false;
        }
        bool condition = (pthread_mutex_unlock(&mutexHandle) == 0);
        return condition
                && pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL) == 0;
    }

    /**
     * @brief Fast return in case of locked semaphore.
     * @return true if the semaphore was unlocked and locks it, otherwise return false.
     */
    bool TryLock() {
        if (closed) {
            return false;
        }
        return (pthread_mutex_trylock(&mutexHandle) == 0);
    }

    /**
     * @brief Adds an handle reference.
     */
    void AddReference() {
        references++;
    }

    /**
     * @brief Removes an handle reference.
     * @return true if the number of handle references is equal to zero.
     */
    bool RemoveReference() {
        if (--references < 0) {
            references = 0;
        }
        return references == 0;
    }

private:

    /**
     * Mutex Handle
     */
    pthread_mutex_t mutexHandle;

    /**
     * Mutex Attributes
     */
    pthread_mutexattr_t mutexAttributes;

    /**
     * Number of handle references
     */
    uint32 references;

    /**
     * This flag is set to true when the semaphore is closed.
     * We need this because when we use shared semaphores (created with
     * the copy constructor) once the semaphore has been closed by the
     * operating system, calling a second close (for the copy constructed
     * semaphore) has an undefined behavior.
     */
    bool closed;

    friend class MutexSemOS;
};

/**
 * @brief System dependent implementation of a mutual exclusive semaphore.
 */
class MutexSemOS {

public:

    /**
     * @brief Create a mutex semaphore with a given initial state
     * @details Called by MutexSem::Create.
     * @param[in,out] semH is the mutex semaphore handle in return.
     * @param[in] locked defines the initial state (true = locked, false = unlocked).
     * @param[in] recursive specifies if the mutex created is recursive or not.
     * @return false if the new or Init fail, true otherwise
     */
    static bool Create(const HANDLE &semH, bool locked, bool &recursive) {
        if (semH != (HANDLE) NULL) {

            if (((PrivateMutexSemStruct*) semH)->RemoveReference()) {
                delete (PrivateMutexSemStruct *) semH;
            }
        }
        // Create the Structure
            semH = (HANDLE) new PrivateMutexSemStruct();
            if (semH == (HANDLE) NULL) {
                return false;
            }
            // Initialize the Semaphore
            bool ret = ((PrivateMutexSemStruct *) semH)->Init(recursive);
            if (!ret) {
                delete (PrivateMutexSemStruct *) semH;
                semH = (HANDLE) NULL;
                return false;
            }

            FlagsType error;
            if (locked == true) {
                ((PrivateMutexSemStruct *) semH)->Lock(TTInfiniteWait, error);
            }

            return error == Errors::Information;
        }

        /**
         * @brief Close the semaphore handle.
         * @details Called by MutexSem::Close
         * @param[in,out] semH is the mutex semaphore handle.
         * @return true.
         */
        static inline bool Close(const HANDLE &semH) {
            if (semH == (HANDLE) NULL) {
                return true;
            }

            bool ret = ((PrivateMutexSemStruct*) semH)->Close();

            if (((PrivateMutexSemStruct*) semH)->RemoveReference()) {
                delete (PrivateMutexSemStruct*) semH;
            }
            semH = (HANDLE) NULL;
            return ret;
        }

        /**
         * @brief Lock the semaphore.
         * @details Called by MutexSem::Lock
         * @param[in,out] semH is the mutex semaphore handle.
         * @param[in] msecTimeout is the desired timeout.
         * @param[out] error is the error type.
         * @return the result of PrivateMutexSemStruct::Lock
         */
        static inline bool Lock(const HANDLE const &semH, TimeoutType msecTimeout, FlagsType &error) {
            if (semH == (HANDLE) NULL) {
                return false;
            }
            return ((PrivateMutexSemStruct *) semH)->Lock(msecTimeout, error);
        }

        /**
         * @brief Unlock the semaphore.
         * @details MutexSem::UnLock
         * @param[in,out] semH is the mutex semaphore handle.
         * @return the return of PrivateMutexSemStruct::UnLock.
         */
        static inline bool UnLock(const HANDLE const &semH) {
            if (semH == (HANDLE) NULL) {
                return false;
            }
            return ((PrivateMutexSemStruct *) semH)->UnLock();
        }

        /**
         * @see MutexSem::Lock
         * @see MutexSemOSLock.
         */
        static inline bool FastLock(const HANDLE const &semH, TimeoutType msecTimeout, FlagsType &error) {
            if (semH == (HANDLE) NULL) {
                return false;
            }
            return ((PrivateMutexSemStruct *) semH)->Lock(msecTimeout, error);
        }

        /**
         * @see MutexSem::UnLock
         * @see MutexSemOS::UnLock.
         */
        static inline bool FastUnLock(const HANDLE const &semH) {

            if (semH == (HANDLE) NULL) {
                return false;
            }
            return ((PrivateMutexSemStruct *) semH)->UnLock();
        }

        /**
         * @brief Fast return in case of locked semaphore.
         * @details called by MutexSem::FastTryLock.
         * @param[in,out] semH is the mutex semaphore handle.
         * @return the result of PrivateMutexSemStruct::FastLock.
         */
        static inline bool FastTryLock(const HANDLE const &semH) {
            if (semH == (HANDLE) NULL) {
                return false;
            }
            return ((PrivateMutexSemStruct *) semH)->TryLock();
        }

        /**
         * @brief Adds an handle reference.
         */
        static inline void DuplicateHandle(const HANDLE const &semH) {
            ((PrivateMutexSemStruct*) semH)->AddReference();
        }
    };

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXSEMOS_H_ */
