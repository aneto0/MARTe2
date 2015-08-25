/**
 * @file MutexSemOS.cpp
 * @brief Source file for class MutexSemOS
 * @date 08/07/2015
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
 * the class MutexSemOS (public, protected, and private). Be aware that some 
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

#include "MutexSem.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*lint -e{9109} forward declaration in MutexSem.h is required to define the class*/
struct MutexSemOSProperties {

    /**
     * Mutex Handle
     */
    pthread_mutex_t mutexHandle;

    /**
     * Mutex Attributes
     */
    pthread_mutexattr_t mutexAttributes;

    /**
     * The number of handle references pointing at this structure.
     */
    uint32 references;

    /**
     * To protect the reference handling
     */
    int32 referencesMux;

    /**
     * Is the semaphore closed?
     */
    bool closed;

    /**
     * Is the semaphore recursive
     */
    bool recursive;

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MutexSem::MutexSem() {
    /*lint -e{1732} -e{1733} no default assignment and no default copy constructor.
     *This is safe since none of the struct members point to dynamically allocated memory*/
    /*lint -e{1713} with the meaning of the () to initialise a struct is clear*/
    handle = new MutexSemOSProperties();
    handle->closed = true;
    handle->recursive = false;
    handle->references = 1u;
    handle->referencesMux = 0;
}

MutexSem::MutexSem(MutexSem &source) {
    handle = source.GetOSProperties();
    while (!Atomic::TestAndSet(&handle->referencesMux)) {
    }
    //Capture the case that it got the handle while the source semaphore
    //was already being destructed...
    if (handle == static_cast<MutexSemOSProperties *>(NULL)) {
        MutexSem();
    }
    else {
        handle->references++;
        handle->referencesMux = 0;
    }
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
MutexSem::~MutexSem() {
    if (handle != static_cast<MutexSemOSProperties *>(NULL)) {
        while (!Atomic::TestAndSet(&handle->referencesMux)) {
        }
        if (handle->references == 1u) {
            if (!handle->closed) {
                /*lint -e{534} possible closure failure is not handled in the destructor.*/
                Close();
            }
            /*lint -esym(1578, MutexSem::handle) the variable is correctly freed here when this is the last reference alive.*/
            delete handle;
            handle = static_cast<MutexSemOSProperties *>(NULL);
        }
        else {
            handle->references--;
            handle->referencesMux = 0;
        }
    }
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::Create(const bool &recursive) {
    while (!Atomic::TestAndSet(&handle->referencesMux)) {
    }
    handle->closed = false;
    bool ok = (pthread_mutexattr_init(&handle->mutexAttributes) == 0);
    if (ok) {
        ok = (pthread_mutexattr_setprotocol(&handle->mutexAttributes, PTHREAD_PRIO_INHERIT) == 0);

        if (ok) {
            if (recursive) {
                //The deadlock condition causes a crash at operating system level.
                ok = (pthread_mutexattr_settype(&handle->mutexAttributes, PTHREAD_MUTEX_RECURSIVE) == 0);
                handle->recursive = true;
            }
            else {
                //This was pthread PTHREAD_MUTEX_RECURSIVE but it was crashing when a deadlock was forced on purpose
                //with PTHREAD_MUTEX_NORMAL the same thread cannot lock the semaphore without unlocking it first.
                ok = (pthread_mutexattr_settype(&handle->mutexAttributes, PTHREAD_MUTEX_NORMAL) == 0);
                handle->recursive = false;
            }
        }
        if (ok) {
            ok = (pthread_mutex_init(&handle->mutexHandle, &handle->mutexAttributes) == 0);
        }
    }
    handle->referencesMux = 0;
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::Close() {
    bool ok = true;
    if (!handle->closed) {
        handle->closed = true;
        /*lint -e{534} the UnLock is allowed to fail (and it will if the semaphore was never used).
         *The semaphore has to be closed whatever the result.*/
        UnLock();
        ok = (pthread_mutexattr_destroy(&handle->mutexAttributes) == 0);
        if (ok) {
            ok = (pthread_mutex_destroy(&handle->mutexHandle) == 0);
        }
    }
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
/*lint -e{454} -e{456} false positive, the semaphore will be unlocked by the UnLock function*/
ErrorType MutexSem::Lock() {
    ErrorType err = NoError;
    if (!handle->closed) {
        bool okCancel = (pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, static_cast<int32 *>(NULL)) == 0);
        bool okLock = (pthread_mutex_lock(&handle->mutexHandle) == 0);
        if ((!okCancel) || (!okLock)) {
            err = OSError;
        }
    }
    else {
        err = OSError;
    }

    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
ErrorType MutexSem::Lock(const TimeoutType &timeout) {
    bool ok = !handle->closed;
    ErrorType err = OSError;
    if (timeout == TTInfiniteWait) {
        err = Lock();
    }
    else if (ok) {
        struct timespec timesValues;
        timeb tb;
        ok = (ftime(&tb) == 0);

        if (ok) {
            float64 sec = static_cast<float64>(timeout.GetTimeoutMSec());
            sec += static_cast<float64>(tb.millitm);
            sec *= 1e-3;
            sec += static_cast<float64>(tb.time);

            float64 roundValue = floor(sec);
            timesValues.tv_sec = static_cast<int32>(roundValue);
            float64 nSec = (sec - roundValue);
            nSec *= 1e9;
            timesValues.tv_nsec = static_cast<int32>(nSec);

            ok = (pthread_mutex_timedlock(&handle->mutexHandle, &timesValues) == 0);
            if (!ok) {
                err = Timeout;
            }
            else {
                err = NoError;
            }
        }
    }
    else {
        err = OSError;
    }
    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::UnLock() {
    bool ok = false;
    if (!handle->closed) {
        /*lint -e{455} false positive, locked by the Lock function*/
        ok = (pthread_mutex_unlock(&handle->mutexHandle) == 0);
        if (ok) {
            ok = (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, static_cast<int32 *>(NULL)) == 0);
        }
    }
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::IsRecursive() const {
    return handle->recursive;
}

MutexSemOSProperties * MutexSem::GetOSProperties() {
    return handle;
}

bool MutexSem::IsClosed() const {
    bool ok = true;
    if (handle != static_cast<MutexSemOSProperties *>(NULL)) {
        ok = handle->closed;
    }
    return ok;
}
