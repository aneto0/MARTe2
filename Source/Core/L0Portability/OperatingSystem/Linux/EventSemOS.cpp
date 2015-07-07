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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "../../TimeoutType.h"
#include "../../Errors.h"
#include "EventSemOS.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
EventSemOS::EventSemOS() :
        IEventSem::IEventSem() {
    closed = true;
    pthreadSem = new PThreadSem();
    if ((pthreadSem != static_cast<PThreadSem *>(NULL))) {
        pthreadSem->references = 1;
        pthreadSem->mux = 0;
        pthreadSem->stop = true;
    }
}

EventSemOS::EventSemOS(const EventSemOS &source) :
        IEventSem::IEventSem() {
    closed = source.IsClosed();
    pthreadSem = source.GetHandle();
    if ((pthreadSem != static_cast<PThreadSem *>(NULL))) {
        FlagsType error;
        while (!Atomic::TestAndSet(&pthreadSem->mux))
            ;
        pthreadSem->references++;
        pthreadSem->mux = 0;
    }
}

EventSemOS::~EventSemOS() {
    FlagsType error;
    if ((pthreadSem != static_cast<PThreadSem *>(NULL))) {
        while (!Atomic::TestAndSet(&pthreadSem->mux))
            ;

        if (pthreadSem->references == 1) {
            if (!closed) {
                Close();
            }
            delete pthreadSem;
            pthreadSem = NULL;
        }
        if ((pthreadSem != static_cast<PThreadSem *>(NULL))) {
            pthreadSem->mux = 0;
        }
    }
}

bool EventSemOS::Create() {
    closed = false;
    bool ok = (pthreadSem != static_cast<PThreadSem *>(NULL));
    if (ok) {
        pthreadSem->stop = true;
        ok = (pthread_mutexattr_init(&pthreadSem->mutexAttributes) == 0);
        if (ok) {
            ok = (pthread_mutex_init(&pthreadSem->mutexHandle, &pthreadSem->mutexAttributes) == 0);
            if (ok) {
                ok = (pthread_cond_init(&pthreadSem->eventVariable, NULL) == 0);
            }
        }
    }
    return ok;
}

bool EventSemOS::Close() {
    bool ok = !closed;
    ok &= (pthreadSem != static_cast<PThreadSem *>(NULL));
    if (ok) {
        ok = Post();
        if (ok && (!closed)) {
            ok = (pthread_mutexattr_destroy(&pthreadSem->mutexAttributes) == 0);
            if (ok) {
                ok = (pthread_mutex_destroy(&pthreadSem->mutexHandle) == 0);
            }
            if (ok) {
                ok = (pthread_cond_destroy(&pthreadSem->eventVariable) == 0);
            }
        }
    }
    closed = ok;
    return ok;
}

bool EventSemOS::Wait(FlagsType &error, const TimeoutType &msecTimeout) {
    bool ok = !closed;
    ok &= (pthreadSem != static_cast<PThreadSem *>(NULL));
    if (ok) {
        if (msecTimeout == TTInfiniteWait) {
            ok = (pthread_mutex_lock(&pthreadSem->mutexHandle) == 0);

            if (ok && pthreadSem->stop) {
                ok = (pthread_cond_wait(&pthreadSem->eventVariable, &pthreadSem->mutexHandle) == 0);
                pthread_mutex_unlock(&pthreadSem->mutexHandle);
            }
            if (ok) {
                ok = (pthread_mutex_unlock(&pthreadSem->mutexHandle) == 0);
            }
            if (!ok) {
                error = Errors::OSError;
            }
        }
        else {
            struct timespec timesValues;
            timeb tb;
            ftime(&tb);

            float64 sec = ((msecTimeout.GetTimeoutMSec() + tb.millitm) * 1e-3 + tb.time);

            float64 roundValue = floor(sec);
            timesValues.tv_sec = (int) roundValue;
            timesValues.tv_nsec = (int) ((sec - roundValue) * 1E9);
            ok = (pthread_mutex_timedlock(&pthreadSem->mutexHandle, &timesValues) == 0);
            if (ok && pthreadSem->stop) {
                ok = (pthread_cond_timedwait(&pthreadSem->eventVariable, &pthreadSem->mutexHandle, &timesValues) == 0);
            }
            if (!ok) {
                error = Errors::Timeout;
            }
            bool okOs = (pthread_mutex_unlock(&pthreadSem->mutexHandle) == 0);
            if (!okOs) {
                error = Errors::OSError;
            }
            ok &= okOs;
        }
    }
    return ok;
}

bool EventSemOS::Post() {
    bool ok = !closed;
    ok &= (pthreadSem != static_cast<PThreadSem *>(NULL));
    if (ok) {
        ok = (pthread_mutex_lock(&pthreadSem->mutexHandle) == 0);
    }
    pthreadSem->stop = false;
    if (ok) {
        ok = (pthread_mutex_unlock(&pthreadSem->mutexHandle) == 0);
    }
    if (ok) {
        ok = (pthread_cond_broadcast(&pthreadSem->eventVariable) == 0);
    }
    return ok;
}

bool EventSemOS::Reset() {
    bool ok = !closed;
    ok &= (pthreadSem != static_cast<PThreadSem *>(NULL));
    if (ok) {
        ok = (pthread_mutex_lock(&pthreadSem->mutexHandle) == 0);
        if (ok) {
            ok = (pthread_mutex_unlock(&pthreadSem->mutexHandle) == 0);
            pthreadSem->stop = true;
        }
    }
    return ok;
}

