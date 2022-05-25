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

#include "FreeRTOS.h"
#include "semphr.h"
#include "queue.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <ErrorType.h>
#include "EventSem.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*lint -e{9109} forward declaration in EventSem.h is required to define the class*/
struct EventSemProperties {

    /**
     * The counting semaphore handle
     */
    SemaphoreHandle_t semHandle;

    /**
     * The number of handle references pointing at this structure.
     */
    int32 references;

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
}

EventSem::EventSem(EventSem &source) {
    handle = source.GetProperties();
    //Capture the case that it got the handle reference while the source semaphore
    //was already being destructed...
    if (handle == static_cast<EventSemProperties *>(NULL)) {
        EventSem();
    }
    else {
        Atomic::Increment(&handle->references);
    }
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
EventSem::~EventSem() {
    if (handle != static_cast<EventSemProperties *>(NULL)) {
        if (handle->references == 1u) {
            if (!handle->closed) {
                /*lint -e{534} possible closure failure is not handled in the destructor.*/
                Close();
            }
            /*lint -esym(1578, EventSem::handle) the variable is correctly freed here when this is the last reference alive.*/
            delete handle;
            handle = static_cast<EventSemProperties *>(NULL);
        }
        else {
            Atomic::Decrement(&handle->references);
        }
    }
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Create() {
    handle->closed = false;
   // handle->semHandle = xSemaphoreCreateMutex();
    handle->semHandle = xSemaphoreCreateCounting(1,0);

    return (handle->semHandle != NULL);
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Close() {
    if (!handle->closed) {
        handle->closed = true;
        /*lint -e{534} the post is allowed to fail (and it will if the semaphore was never used).
         *The semaphore has to be closed whatever the result.*/
        Post();
        vSemaphoreDelete(handle->semHandle);
    }
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
ErrorManagement::ErrorType EventSem::Wait() {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (!handle->closed) {
        portBASE_TYPE ret = xSemaphoreTake(handle->semHandle, portMAX_DELAY);
        if (ret == pdTRUE) {
            xSemaphoreGive(handle->semHandle);
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
ErrorManagement::ErrorType EventSem::Wait(const TimeoutType &timeout) {
    ErrorManagement::ErrorType err = ErrorManagement::NoError;
    if (timeout == TTInfiniteWait) {
        err = Wait();
    }
    else {
        if (!handle->closed) {
            portBASE_TYPE ret = xSemaphoreTake(handle->semHandle, timeout.GetTimeoutMSec() /** configTICK_RATE_HZ) / 1000L*/);
            if (ret == pdTRUE) {
                xSemaphoreGive(handle->semHandle);
            }
            else {
                err = ErrorManagement::Timeout;
            }
        }
        else {
            err = ErrorManagement::OSError;
        }
    }
    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Post() {
    bool ok = false;
    if (!handle->closed) {
        //    ok = (xSemaphoreGive(handle->semHandle) == pdTRUE);
        BaseType_t xHigherPriorityTaskWoken=pdFALSE;
        xSemaphoreGiveFromISR(handle->semHandle, &xHigherPriorityTaskWoken);
        //TODO: Actually it seems it has been removed and refers to an old tutorial
        //Check why was here and why it was needed
        //portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
        //portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        ok = true;//(xHigherPriorityTaskWoken == pdTRUE);
    }
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Reset() {
    bool ok = false;
    if (!handle->closed) {
        ok = true;
        xQueueReset(handle->semHandle);
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
