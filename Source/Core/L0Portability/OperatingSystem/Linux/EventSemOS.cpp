/**
 * @file EventSemOS.cpp
 * @brief Source file for class EventSemOS
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
 * the class EventSemOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <pthread.h>
#include <math.h>
#include <sys/timeb.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <ErrorType.h>
#include "EventSem.h"
#include "Atomic.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
struct EventSemOSProperties {
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
     * To protect the reference handling
     */
    int32 referencesMux;

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
    osProperties = new EventSemOSProperties();
    if (osProperties != static_cast<EventSemOSProperties *>(NULL)) {
        osProperties->closed = true;
        osProperties->references = 1;
        osProperties->referencesMux = 0;
        osProperties->stop = true;
    }
}

EventSem::EventSem(const EventSem &source) {
    osProperties = source.GetOSProperties();
    while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
    }
    //Capture the case that it got the osProperties while the source semaphore
    //was already being destructed...
    if (osProperties == static_cast<EventSemOSProperties *>(NULL)) {
        EventSem();
    }
    else {
        osProperties->references++;
        osProperties->referencesMux = 0;
    }
}

EventSem::~EventSem() {
    if (osProperties != static_cast<EventSemOSProperties *>(NULL)) {
        while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
        }
        if (osProperties->references == 1) {
            if (!osProperties->closed) {
                Close();
            }
            delete osProperties;
            osProperties = NULL;
        }
        else {
            osProperties->references--;
            osProperties->referencesMux = 0;
        }
    }
}

bool EventSem::Create() {
    while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
    }
    osProperties->closed = false;
    osProperties->stop = true;
    bool ok = (pthread_mutexattr_init(&osProperties->mutexAttributes) == 0);
    if (ok) {
        ok = (pthread_mutex_init(&osProperties->mutexHandle, &osProperties->mutexAttributes) == 0);
        if (ok) {
            ok = (pthread_cond_init(&osProperties->eventVariable, NULL) == 0);
        }
    }
    osProperties->referencesMux = 0;
    return ok;
}

bool EventSem::Close() {
    bool ok = true;
    if (!osProperties->closed) {
        osProperties->closed = true;
        Post();
        if (ok) {
            ok = (pthread_mutexattr_destroy(&osProperties->mutexAttributes) == 0);
            if (ok) {
                ok = (pthread_mutex_destroy(&osProperties->mutexHandle) == 0);
            }
            if (ok) {
                ok = (pthread_cond_destroy(&osProperties->eventVariable) == 0);
            }
        }
    }
    return ok;
}

ErrorType EventSem::Wait() {
    bool ok = !osProperties->closed;
    ErrorType err = NoError;
    if (ok) {
        ok = (pthread_mutex_lock(&osProperties->mutexHandle) == 0);

        if (ok && osProperties->stop) {
            ok = (pthread_cond_wait(&osProperties->eventVariable, &osProperties->mutexHandle) == 0);
            pthread_mutex_unlock(&osProperties->mutexHandle);
        }
        if (ok) {
            ok = (pthread_mutex_unlock(&osProperties->mutexHandle) == 0);
        }
        if (!ok) {
            err = OSError;
        }
    }
    else {
        err = OSError;
    }
    return err;
}

ErrorType EventSem::Wait(const TimeoutType &timeout) {
    bool ok = !osProperties->closed;
    ErrorType err = NoError;
    if (timeout == TTInfiniteWait) {
        err = Wait();
    }
    if (ok) {
        struct timespec timesValues;
        timeb tb;
        ftime(&tb);

        float64 sec = ((timeout.GetTimeoutMSec() + tb.millitm) * 1e-3 + tb.time);

        float64 roundValue = floor(sec);
        timesValues.tv_sec = (int) roundValue;
        timesValues.tv_nsec = (int) ((sec - roundValue) * 1E9);
        ok = (pthread_mutex_timedlock(&osProperties->mutexHandle, &timesValues) == 0);
        if (ok && osProperties->stop) {
            ok = (pthread_cond_timedwait(&osProperties->eventVariable, &osProperties->mutexHandle, &timesValues) == 0);
        }
        if (!ok) {
            err = Timeout;
        }
        bool okOs = (pthread_mutex_unlock(&osProperties->mutexHandle) == 0);
        if (!okOs) {
            err = OSError;
        }
    }
    else {
        err = OSError;
    }
    return err;
}

bool EventSem::Post() {
    bool ok = !osProperties->closed;
    if (ok) {
        ok = (pthread_mutex_lock(&osProperties->mutexHandle) == 0);
    }
    osProperties->stop = false;
    if (ok) {
        ok = (pthread_mutex_unlock(&osProperties->mutexHandle) == 0);
    }
    if (ok) {
        ok = (pthread_cond_broadcast(&osProperties->eventVariable) == 0);
    }
    return ok;
}

bool EventSem::Reset() {
    bool ok = !osProperties->closed;
    if (ok) {
        ok = (pthread_mutex_lock(&osProperties->mutexHandle) == 0);
    }
    if (ok) {
        ok = (pthread_mutex_unlock(&osProperties->mutexHandle) == 0);
        osProperties->stop = true;
    }
    return ok;
}

ErrorType EventSem::ResetWait(const TimeoutType &timeout) {
    bool ok = Reset();
    ErrorType err = OSError;
    if (ok) {
        err = Wait(timeout);
    }
    return err;
}

EventSemOSProperties *EventSem::GetOSProperties() const {
    return osProperties;
}

bool EventSem::IsClosed() const {
    bool ok = true;
    if (osProperties != static_cast<EventSemOSProperties *>(NULL)) {
        ok = osProperties->closed;
    }
    return ok;
}

