/**
 * @file EventSem.cpp
 * @brief Source file for class EventSem
 * @date 06/07/2015
 * @author André Neto
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

 * @details This source file contains the definition of all the methods for
 * the class EventSem (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <pthread.h>
#include <math.h>
#include <sys/time.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Atomic.h"
#include "ErrorManagement.h"
#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{9109} forward declaration in EventSem.h is required to define the class*/
struct EventSemProperties {

    /**
     * Mutex Handle
     */
    pthread_mutex_t mutexHandle;

    /**
     * Mutex Attributes
     */
    pthread_mutexattr_t mutexAttributes;

    /**
     * Conditional Variable
     */
    pthread_cond_t eventVariable;

    /**
     * The number of handle references pointing at this structure.
     */
    uint32 references;

    /**
     * Implementation of the barrier in Linux. When true the EventSem will stop.
     */
    bool stop;

    /**
     * Is the semaphore closed?
     */
    bool closed;

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EventSem::EventSem() {
    /*lint -e{1732} -e{1733} no default assignment and no default copy constructor.
     *This is safe since none of the struct members point to dynamically allocated memory*/
    /*lint -e{1713} with the meaning of the () to initialise a struct is clear*/
    handle = new EventSemProperties();
    handle->closed = true;
    handle->references = 1u;
    handle->stop = true;
    mux.Create();
}

EventSem::EventSem(EventSem &source) {
    handle = source.GetProperties();
    if (mux.FastLock() == ErrorManagement::NoError) {
        //Capture the case that it got the handle reference while the source semaphore
        //was already being destructed...
        if (handle == static_cast<EventSemProperties *>(NULL)) {
            EventSem();
        }
        else {
            handle->references++;
        }
    }
    mux.FastUnLock();
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
EventSem::~EventSem() {
    if (handle != static_cast<EventSemProperties *>(NULL)) {
        if (mux.FastLock() == ErrorManagement::NoError) {
            if (handle->references == 1u) {
                if (!handle->closed) {
                    /*lint -e{534} possible closure failure is not handled in the destructor.*/
                    Close();
                }
                handle->references--;
                /*lint -esym(1579, MARTe::EventSem::handle) the variable is correctly freed here when this is the last reference alive.*/
                delete handle;
                handle = static_cast<EventSemProperties *>(NULL);
            }
            else {
                handle->references--;
            }
        }
        mux.FastUnLock();
    }
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Create() {
    bool ok = false;
    if (mux.FastLock() == ErrorManagement::NoError) {
        handle->closed = false;
        handle->stop = true;
        ok = (pthread_mutexattr_init(&handle->mutexAttributes) == 0);
        if (ok) {
            ok = (pthread_mutex_init(&handle->mutexHandle, &handle->mutexAttributes) == 0);
            if (ok) {
                ok = (pthread_cond_init(&handle->eventVariable, static_cast<const pthread_condattr_t *>(NULL)) == 0);
                if (!ok) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_cond_init()");
                }
            }
            else {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_mutex_init()");
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_mutexattr_init()");
        }
    }
    mux.FastUnLock();
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Close() {
    bool ok = true;
    if (!handle->closed) {
        handle->closed = true;
        /*lint -e{534} the post is allowed to fail (and it will if the semaphore was never used).
         *The semaphore has to be closed whatever the result.*/
        Post();
        ok = (pthread_mutexattr_destroy(&handle->mutexAttributes) == 0);
        if (ok) {
            ok = (pthread_mutex_destroy(&handle->mutexHandle) == 0);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_mutex_destroy()");
            }
        }
        else {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_mutexattr_destroy()");
        }
        if (ok) {
            ok = (pthread_cond_destroy(&handle->eventVariable) == 0);
            if (!ok) {
                REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_cond_destroy()");
            }
        }
    }
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
ErrorManagement::ErrorType EventSem::Wait() {
    bool ok = false;
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (!handle->closed) {
        bool okLock = (pthread_mutex_lock(&handle->mutexHandle) == 0);
        if (!okLock) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_mutex_lock()");
        }

        bool okWait = true;
        if (handle->stop) {
            okWait = (pthread_cond_wait(&handle->eventVariable, &handle->mutexHandle) == 0);
        }
        if (!okWait) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_cond_wait()");
        }

        bool okUnLock = (pthread_mutex_unlock(&handle->mutexHandle) == 0);
        if (!okUnLock) {
            REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "Error: pthread_mutex_unlock()");
        }
        ok = (okLock && okWait && okUnLock);
        if (!ok) {
            err = ErrorManagement::OSError;
        }
    }
    else {
        err = ErrorManagement::FatalError;
        REPORT_ERROR_STATIC_0(err, "Error: the semaphore handle is closed");
    }

    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
ErrorManagement::ErrorType EventSem::Wait(const TimeoutType &timeout) {
    bool ok = !handle->closed;
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (timeout == TTInfiniteWait) {
        err = Wait();
    }
    else {
        if (ok) {
            struct timespec timesValues;
            struct timeval tv;
	    struct timezone tz;
            ok = (gettimeofday(&tv, &tz) == 0);

            if (ok) {
                float64 sec = static_cast<float64>(timeout.GetTimeoutUSec());
                sec += static_cast<float64>(tv.tv_usec);
                sec *= 1e-6;
                sec += static_cast<float64>(tv.tv_sec);

                float64 roundValue = floor(sec);
                timesValues.tv_sec = static_cast<int32>(roundValue);
                float64 nSec = (sec - roundValue);
                nSec *= 1e9;
                timesValues.tv_nsec = static_cast<int32>(nSec);
                ok = (pthread_mutex_timedlock(&handle->mutexHandle, &timesValues) == 0);
                if (ok) {
                    if (handle->stop) {
                        ok = (pthread_cond_timedwait(&handle->eventVariable, &handle->mutexHandle, &timesValues) == 0);
                        if (!ok) {
                            err = ErrorManagement::Timeout;
                    //        REPORT_ERROR_STATIC_0(err, "Information: timeout occurred");
                        }
                    }
                }

                /*lint -e{455} false positive from that does not understand that pthread_mutex_timedlock locks the semaphore*/
                bool okOs = (pthread_mutex_unlock(&handle->mutexHandle) == 0);
                if (!okOs) {
                    err = ErrorManagement::OSError;
                    REPORT_ERROR_STATIC_0(err, "Error: pthread_mutex_unlock()");
                }
            }
            else {
                err = ErrorManagement::OSError;
                REPORT_ERROR_STATIC_0(err, "Error: ftime()");
            }
        }
        else {
            err = ErrorManagement::FatalError;
            REPORT_ERROR_STATIC_0(err, "Error: the semaphore handle is closed");
        }
    }
    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Post() {
    bool ok = false;
    if (!handle->closed) {
        bool okLock = (pthread_mutex_lock(&handle->mutexHandle) == 0);
        handle->stop = false;
        bool okUnLock = (pthread_mutex_unlock(&handle->mutexHandle) == 0);
        bool okBroadcast = (pthread_cond_broadcast(&handle->eventVariable) == 0);
        ok = (okLock && okUnLock && okBroadcast);
    }

    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Reset() {
    bool ok = false;
    if (!handle->closed) {
        bool okLock = (pthread_mutex_lock(&handle->mutexHandle) == 0);
        bool okUnLock = (pthread_mutex_unlock(&handle->mutexHandle) == 0);
        ok = (okLock && okUnLock);
        handle->stop = true;
    }
    return ok;
}

ErrorManagement::ErrorType EventSem::ResetWait(const TimeoutType &timeout) {
    bool ok = Reset();
    ErrorManagement::ErrorType err = ErrorManagement::OSError;
    if (ok) {
        err = Wait(timeout);
    }
    return err;
}

EventSemProperties *EventSem::GetProperties() {
    return handle;
}

bool EventSem::IsClosed() const {
    bool ok = true;
    if (handle != static_cast<EventSemProperties *>(NULL)) {
        ok = handle->closed;
    }
    return ok;
}
}
