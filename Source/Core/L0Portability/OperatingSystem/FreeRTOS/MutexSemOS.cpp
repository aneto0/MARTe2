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

/*lint -e{9109} forward declaration in MutexSem.h is required to define the class*/
struct MutexSemOSProperties {

    /**
     * Mutex Handle
     */
    SemaphoreHandle_t mutexHandle;

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
    osProperties = new MutexSemOSProperties();
    osProperties->closed = true;
    osProperties->recursive = false;
    osProperties->references = 1u;
    osProperties->referencesMux = 0;
}

MutexSem::MutexSem(MutexSem &source) {
    osProperties = source.GetOSProperties();
    while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
    }
    //Capture the case that it got the osProperties while the source semaphore
    //was already being destructed...
    if (osProperties == static_cast<MutexSemOSProperties *>(NULL)) {
        MutexSem();
    }
    else {
        osProperties->references++;
        osProperties->referencesMux = 0;
    }
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
MutexSem::~MutexSem() {
    if (osProperties != static_cast<MutexSemOSProperties *>(NULL)) {
        while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
        }
        if (osProperties->references == 1u) {
            if (!osProperties->closed) {
                /*lint -e{534} possible closure failure is not handled in the destructor.*/
                Close();
            }
            /*lint -esym(1578, MutexSem::osProperties) the variable is correctly freed here when this is the last reference alive.*/
            delete osProperties;
            osProperties = static_cast<MutexSemOSProperties *>(NULL);
        }
        else {
            osProperties->references--;
            osProperties->referencesMux = 0;
        }
    }
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
bool MutexSem::Create(const bool &recursive) {
    while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
    }
    osProperties->closed = false;
    if (recursive) {
        osProperties->mutexHandle = xSemaphoreCreateMutex();
    }
    else {
        osProperties->mutexHandle = xSemaphoreCreateRecursiveMutex();
    }
    osProperties->referencesMux = 0;
    return (osProperties->mutexHandle != NULL);
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
bool MutexSem::Close() {
    if (!osProperties->closed) {
        osProperties->closed = true;
        /*lint -e{534} the UnLock is allowed to fail (and it will if the semaphore was never used).
         *The semaphore has to be closed whatever the result.*/
        vSemaphoreDelete(osProperties->mutexHandle);
    }
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
/*lint -e{454} -e{456} false positive, the semaphore will be unlocked by the UnLock function*/
ErrorType MutexSem::Lock() {
    ErrorType err = NoError;
    if (!osProperties->closed) {
        portBASE_TYPE ret = xSemaphoreTake(osProperties->mutexHandle, portMAX_DELAY);
        if (ret != pdTRUE) {
            err = OSError;
        }
    }
    else {
        err = OSError;
    }
    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
ErrorType MutexSem::Lock(const TimeoutType &timeout) {
    bool ok = !osProperties->closed;
    ErrorType err = OSError;
    if (timeout == TTInfiniteWait) {
        err = Lock();
    }
    else if (ok) {
        if (!osProperties->closed) {
            err = NoError;
            portBASE_TYPE ret = xSemaphoreTake(osProperties->mutexHandle, (timeout.GetTimeoutMSec() * configTICK_RATE_HZ) / 1000L);
            if (ret != pdTRUE) {
                err = Timeout;
            }
        }
        else {
            err = OSError;
        }
    }
    else {
        err = OSError;
    }
    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
bool MutexSem::UnLock() {
    bool ok = false;
    if (!osProperties->closed) {
        ok = (xSemaphoreGive(osProperties->mutexHandle) == pdTRUE);
    }
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
bool MutexSem::IsRecursive() const {
    return osProperties->recursive;
}

MutexSemOSProperties * MutexSem::GetOSProperties() {
    return osProperties;
}

bool MutexSem::IsClosed() const {
    bool ok = true;
    if (osProperties != static_cast<MutexSemOSProperties *>(NULL)) {
        ok = osProperties->closed;
    }
    return ok;
}
