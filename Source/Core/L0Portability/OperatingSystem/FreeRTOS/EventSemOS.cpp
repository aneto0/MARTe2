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

/*lint -e{9109} forward declaration in EventSem.h is required to define the class*/
struct EventSemOSProperties {

    /**
     * The counting semaphore handle
     */
    SemaphoreHandle_t semHandle;

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

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EventSem::EventSem() {
    /*lint -e{1732} -e{1733} no default assignment and no default copy constructor.
     *This is safe since none of the struct members point to dynamically allocated memory*/
    /*lint -e{1713} with the meaning of the () to initialise a struct is clear*/
    osProperties = new EventSemOSProperties();
    osProperties->closed = true;
    osProperties->references = 1u;
    osProperties->referencesMux = 0;
}

EventSem::EventSem(EventSem &source) {
    osProperties = source.GetOSProperties();
    while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
    }
    //Capture the case that it got the osProperties reference while the source semaphore
    //was already being destructed...
    if (osProperties == static_cast<EventSemOSProperties *>(NULL)) {
        EventSem();
    }
    else {
        osProperties->references++;
        osProperties->referencesMux = 0;
    }
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
EventSem::~EventSem() {
    if (osProperties != static_cast<EventSemOSProperties *>(NULL)) {
        while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
        }
        if (osProperties->references == 1u) {
            if (!osProperties->closed) {
                /*lint -e{534} possible closure failure is not handled in the destructor.*/
                Close();
            }
            /*lint -esym(1578, EventSem::osProperties) the variable is correctly freed here when this is the last reference alive.*/
            delete osProperties;
            osProperties = static_cast<EventSemOSProperties *>(NULL);
        }
        else {
            osProperties->references--;
            osProperties->referencesMux = 0;
        }
    }
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
bool EventSem::Create() {
    while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
    }
    osProperties->closed = false;
    osProperties->semHandle = xSemaphoreCreateMutex();
    ;
    return (osProperties->semHandle != NULL);
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
bool EventSem::Close() {
    if (!osProperties->closed) {
        osProperties->closed = true;
        /*lint -e{534} the post is allowed to fail (and it will if the semaphore was never used).
         *The semaphore has to be closed whatever the result.*/
        Post();
        vSemaphoreDelete(osProperties->semHandle);
    }
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
ErrorType EventSem::Wait() {
    ErrorType err = NoError;
    if (!osProperties->closed) {
        portBASE_TYPE ret = xSemaphoreTake(osProperties->semHandle, portMAX_DELAY);
        if (ret == pdTRUE) {
            xSemaphoreGive(osProperties->semHandle);
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
ErrorType EventSem::Wait(const TimeoutType &timeout) {
    bool ok = !osProperties->closed;
    ErrorType err = NoError;
    if (timeout == TTInfiniteWait) {
        err = Wait();
    }
    if (ok) {
        portBASE_TYPE ret = xSemaphoreTake(osProperties->semHandle, (timeout.GetTimeoutMSec() * configTICK_RATE_HZ) / 1000L);
        if (ret == pdTRUE) {
            xSemaphoreGive(osProperties->semHandle);
        }
        else {
            err = Timeout;
        }
    }
    else {
        err = OSError;
    }
    return err;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
bool EventSem::Post() {
    bool ok = false;
    if (!osProperties->closed) {
        ok = (xSemaphoreGive(osProperties->semHandle) == pdTRUE);
    }
    return ok;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to osProperties*/
bool EventSem::Reset() {
    bool ok = false;
    if (!osProperties->closed) {
        ok = true;
        xQueueReset(osProperties->semHandle);
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

EventSemOSProperties *EventSem::GetOSProperties() {
    return osProperties;
}

bool EventSem::IsClosed() const {
    bool ok = true;
    if (osProperties != static_cast<EventSemOSProperties *>(NULL)) {
        ok = osProperties->closed;
    }
    return ok;
}
