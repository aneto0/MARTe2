/**
 * @file EventSemOS.h
 * @brief Header file for class EventSemOS
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

 * @details This header file contains the declaration of the class EventSemOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTSEMOS_H_
#define EVENTSEMOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#ifndef LINT
#include <pthread.h>
#include <math.h>
#include <sys/timeb.h>
#endif
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "../../TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Private event semaphore class used for Solaris and Linux when using pThread.
 *
 * @details The event semaphore is implemented using pthread_cond functions, but this class
 * use also a pthread_mutex to assure consistency of critical operations on the event semaphore
 * shared by threads.
 */
class PrivateEventSemStruct {

private:
    /**
     * @brief Constructor.
     */
    PrivateEventSemStruct() {
        stop = true;
        closed = false;
        references = 1;
    }
    /**
     * @brief Destructor.
     */
    ~PrivateEventSemStruct() {
    }

    /**
     * @brief Initialize the semaphore with the right attributes.
     * @return false if something wrong with pthread_mutex and pthread_cond initializations.
     */
    bool Init() {
        stop = true;
        closed = false;
        if (pthread_mutexattr_init(&mutexAttributes) != 0) {
            return false;
        }
        if (pthread_mutex_init(&mutexHandle, &mutexAttributes) != 0) {
            return false;
        }
        if (pthread_cond_init(&eventVariable, NULL) != 0) {
            return false;
        }
        return true;
    }

    /**
     * @brief Destroy the semaphore.
     * @return false if something wrong in pthread_mutex and pthread_cond destructions.
     */
    bool Close() {
        if (closed) {
            return true;
        }
        Post();
        closed = true;
        if (pthread_mutexattr_destroy(&mutexAttributes) != 0) {
            return false;
        }
        if (pthread_mutex_destroy(&mutexHandle) != 0) {
            return false;
        }
        if (pthread_cond_destroy(&eventVariable) != 0) {
            return false;
        }
        return true;
    }

    /**
     * @brief Wait until a post condition or until the timeout expire.
     * @param[in] msecTimeout is the desired timeout.
     * @param[out] error is the error type.
     * @return false if lock or wait functions fail or if the timeout causes the wait fail.
     */
    bool Wait(TimeoutType msecTimeout, Error &error) {
        if (closed) {
            return false;
        }
        if (msecTimeout == TTInfiniteWait) {
            if (pthread_mutex_lock(&mutexHandle) != 0) {
                error = OSError;
                return false;
            }
            if (stop == true) {
                if (pthread_cond_wait(&eventVariable, &mutexHandle) != 0) {
                    pthread_mutex_unlock(&mutexHandle);
                    error = OSError;
                    return false;
                }
            }
            if (pthread_mutex_unlock(&mutexHandle) != 0) {
                error = OSError;
                return false;
            }
        }
        else {
            struct timespec timesValues;
            timeb tb;
            ftime(&tb);

            float64 sec = ((msecTimeout.msecTimeout + tb.millitm) * 1e-3
                    + tb.time);

            float64 roundValue = floor(sec);
            timesValues.tv_sec = (int) roundValue;
            timesValues.tv_nsec = (int) ((sec - roundValue) * 1E9);
            if (pthread_mutex_timedlock(&mutexHandle, &timesValues) != 0) {
                error = Timeout;
                return false;
            }
            if (stop == true) {

                if (pthread_cond_timedwait(&eventVariable, &mutexHandle, &timesValues) != 0) {
                    pthread_mutex_unlock(&mutexHandle);
                    error = Timeout;
                    return false;
                }
            }
            if (pthread_mutex_unlock(&mutexHandle) != 0) {
                error = OSError;
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Post condition. Free all threads stopped in a wait condition.
     * @return true if the eventVariable is set to zero.
     */
    bool Post() {
        if (closed) {
            return false;
        }
        if (pthread_mutex_lock(&mutexHandle) != 0) {
            return false;
        }
        stop = false;
        if (pthread_mutex_unlock(&mutexHandle) != 0) {
            return false;
        }
        return (pthread_cond_broadcast(&eventVariable) == 0);
    }

    /**
     * @brief Reset the semaphore for a new possible wait condition.
     * @return false if the mutex lock fails.
     */
    bool Reset() {
        if (closed) {
            return false;
        }
        if (pthread_mutex_lock(&mutexHandle) != 0) {
            return false;
        }
        stop = true;
        if (pthread_mutex_unlock(&mutexHandle) != 0) {
            return false;
        }
        return stop;
    }

    /**
     * @brief Adds an handle reference.
     */
    void AddReference() {
        references++;
    }

    /**
     * @brief Removes an handle reference.
     * @return true if the nuumber of handle references is equal to zero.
     */
    bool RemoveReference() {
        if (--references < 0) {
            references = 0;
        }
        return references == 0;
    }

private:
    /** Mutex Handle */
    pthread_mutex_t mutexHandle;

    /** Mutex Attributes */
    pthread_mutexattr_t mutexAttributes;

    /** Conditional Variable */
    pthread_cond_t eventVariable;

    /** boolean semaphore */
    bool stop;

    /** The number of handle references. */
    int32 references;

    /** This flag is set to true when the semaphore is closed.
     * We need this because when we use shared semaphores (created with
     * the copy constructor) once the semaphore has been closed by the
     * operating system, calling a second close (for the copy constructed
     * semaphore) has an undefined behavior.
     */
    bool closed;

    friend class EventSemOS;

};

/**
 * @brief System dependent implementation of an event semaphore.
 *
 * @details These function implements the event semaphore using pthread library.
 */
class EventSemOS {

public:

    /**
     * @brief Create a new event semaphore handle.
     * @details Called by EventSem::Create
     * @param[in,out] semH is the semaphore handle in return.
     * @return false if the new or the Init functions fail, true otherwise.
     */
    static bool Create(const HANDLE &semH) {
        if (semH != (HANDLE) NULL) {
            if (((PrivateEventSemStruct*) semH)->RemoveReference()) {
                delete (PrivateEventSemStruct *) semH;
            }
        }
        semH = (HANDLE) new PrivateEventSemStruct;
        if (semH == (HANDLE) NULL) {
            return false;
        }
        // Initialize the Semaphore
            bool ret = ((PrivateEventSemStruct *) semH)->Init();
            if (!ret) {
                delete (PrivateEventSemStruct *) semH;
                semH = (HANDLE) NULL;
                return false;
            }
            return true;
        }

        /**
         * @brief Destroy the event semaphore.
         * @details Called by EventSem::Create
         * @param[in,out] semH is the semaphore handle.
         * @return true if the Close function has success, false otherwise.
         */
        static bool Close(const HANDLE &semH) {
            if (semH == (HANDLE) NULL) {
                return true;
            }
            bool ret = ((PrivateEventSemStruct *) semH)->Close();
            if (((PrivateEventSemStruct*) semH)->RemoveReference()) {
                delete (PrivateEventSemStruct *) semH;
            }
            semH = (HANDLE) NULL;
            return ret;
        }

        /**
         * @brief Wait condition.
         * @details Called by EventSem::Create
         * @param[in,out] semH is the semaphore handle.
         * @param[in] msecTimeout is the desired timeout.
         * @param[out] error is the error type.
         * @return the result of PrivateEventSemStruct::Wait.
         */
        static inline bool Wait(const HANDLE const &semH, TimeoutType msecTimeout, Error &error) {
            if (semH == (HANDLE) NULL) {
                return false;
            }
            return ((PrivateEventSemStruct *) semH)->Wait(msecTimeout, error);
        }

        /**
         * @brief Post condition.
         * @details Called by EventSem::Post
         * @param[in,out] semH is the semaphore handle.
         * @return the result of PrivateEventSemStruct::Post.
         */
        static inline bool Post(const HANDLE const &semH) {
            if (semH == (HANDLE) NULL) {
                return false;
            }
            return (((PrivateEventSemStruct *) semH)->Post());
        }

        /**
         * @brief Reset the semaphore for a new wait condition.
         * @details Called by EventSem::Reset
         * @param[in,out] semH is the semaphore handle.
         * @return the result of PrivateEventSemStruct::Reset.
         */
        static inline bool Reset(const HANDLE const &semH) {
            if (semH == (HANDLE) NULL) {
                return false;
            }
            return ((PrivateEventSemStruct *) semH)->Reset();
        }

        /**
         * @brief Reset and then perform a wait condition of the event semaphore.
         * @details Called by EventSem::Reset
         * @param[in,out] semH is a pointer to the event semaphore.
         * @param[in] msecTimeout is the desired timeout.
         * @param[out] error is the error type.
         * @return the result of PrivateEventSemStruct::Wait.
         */
        static inline bool ResetWait(const HANDLE const &semH, TimeoutType msecTimeout, Error &error) {
            Reset(semH);
            return Wait(semH, msecTimeout, error);
        }

        /**
         * @brief Adds an handle reference.
         */
        static inline void DuplicateHandle(const HANDLE const &semH) {
            ((PrivateEventSemStruct*) semH)->AddReference();
        }
    };

    /*---------------------------------------------------------------------------*/
    /*                        Inline method definitions                          */
    /*---------------------------------------------------------------------------*/
#endif /* EVENTSEMOS_H_ */

