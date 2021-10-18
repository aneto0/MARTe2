/**
 * @file MutexSem.cpp
 * @brief Source file for class MutexSem
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
 * the class MutexSem (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MutexSem.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{9109} forward declaration in MutexSem.h is required to define the class*/
struct MutexSemProperties {

    /**
     * Mutex Handle
     */
    SemaphoreHandle_t mutexHandle;

    /**
     * The number of handle references pointing at this structure.
     */
    int32 references;

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
    handle = new MutexSemProperties();
    handle->closed = true;
    handle->recursive = false;
    handle->references = 1u;
}

MutexSem::MutexSem(MutexSem &source) {
    handle = source.GetProperties();
    //Capture the case that it got the handle while the source semaphore
    //was already being destructed...
    if (handle == static_cast<MutexSemProperties *>(NULL)) {
        MutexSem();
    }
    else {
        Atomic::Increment(&handle->references);
    }
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
MutexSem::~MutexSem() {
    if (handle != static_cast<MutexSemProperties *>(NULL)) {
        if (handle->references == 1u) {
            if (!handle->closed) {
                /*lint -e{534} possible closure failure is not handled in the destructor.*/
                Close();
            }
            /*lint -esym(1578, MutexSem::handle) the variable is correctly freed here when this is the last reference alive.*/
            delete handle;
            handle = static_cast<MutexSemProperties *>(NULL);
        }
        else {
            Atomic::Decrement(&handle->references);
        }
    }
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::Create(const bool &recursive) {
    handle->closed = false;
    if (recursive) {
        handle->mutexHandle = xSemaphoreCreateMutex();
    }
    else {
        handle->mutexHandle = xSemaphoreCreateRecursiveMutex();
    }
    return (handle->mutexHandle != NULL);
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::Close() {
    if (!handle->closed) {
        handle->closed = true;
        /*lint -e{534} the UnLock is allowed to fail (and it will if the semaphore was never used).
         *The semaphore has to be closed whatever the result.*/
        vSemaphoreDelete(handle->mutexHandle);
    }
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
/*lint -e{454} -e{456} false positive, the semaphore will be unlocked by the UnLock function*/
ErrorManagement::ErrorType MutexSem::Lock() {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (!handle->closed) {
        portBASE_TYPE ret = xSemaphoreTake(handle->mutexHandle, portMAX_DELAY);
        if (ret != pdTRUE) {
            err = ErrorManagement::OSError;
        }
    }
    else {
        err = ErrorManagement::OSError;
    }
    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
ErrorManagement::ErrorType MutexSem::Lock(const TimeoutType &timeout) {
    bool ok = !handle->closed;
    ErrorManagement::ErrorType err = ErrorManagement::OSError;
    if (timeout == TTInfiniteWait) {
        err = Lock();
    }
    else if (ok) {
        if (!handle->closed) {
            err = ErrorManagement::NoError;
            portBASE_TYPE ret = xSemaphoreTake(handle->mutexHandle, (timeout.GetTimeoutMSec() * configTICK_RATE_HZ) / 1000L);
            if (ret != pdTRUE) {
                err = ErrorManagement::Timeout;
            }
        }
        else {
            err = ErrorManagement::OSError;
        }
    }
    else {
        err = ErrorManagement::OSError;
    }
    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::UnLock() {
    bool ok = false;
    if (!handle->closed) {
        ok = (xSemaphoreGive(handle->mutexHandle) == pdTRUE);
    }
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool MutexSem::IsRecursive() const {
    return handle->recursive;
}

MutexSemProperties * MutexSem::GetProperties() {
    return handle;
}

bool MutexSem::IsClosed() const {
    bool ok = true;
    if (handle != static_cast<MutexSemProperties *>(NULL)) {
        ok = handle->closed;
    }
    return ok;
}

}
