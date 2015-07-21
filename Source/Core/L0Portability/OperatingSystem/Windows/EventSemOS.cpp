/**
 * @file EventSemOS.cpp
 * @brief Source file for class EventSemOS
 * @date 20/07/2015
 * @author Giuseppe Ferrò
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
#include "Windows.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "EventSem.h"
#include "Atomic.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
struct EventSemOSProperties {
    /**
     * Event Handle
     */
    HANDLE eventHandle;

    /**
     * Is the semaphore closed?
     */
    bool closed;

    /**
     * To protect the reference handling
     */
    int32 referencesMux;

    /**
     * The number of handle references pointing at this structure.
     */
    uint32 references;
};
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EventSem::EventSem() {
    osProperties = new EventSemOSProperties();
    osProperties->eventHandle = NULL;
    osProperties->closed = true;
    osProperties->references = 1u;

    osProperties->referencesMux = 0;

}

bool EventSem::Create() {
    while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
    }

    osProperties->closed = false;

    //If the handle is valid close it
    CloseHandle(osProperties->eventHandle);

    osProperties->eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

    bool ok = (osProperties->eventHandle != NULL);

    osProperties->referencesMux = 0;
    return ok;
}

EventSem::EventSem(EventSem &source) {

    osProperties = source.GetOSProperties();
    if (osProperties == static_cast<EventSemOSProperties *>(NULL)) {
        //Capture the case that it got the osProperties reference while the source semaphore
        //was already being destructed...
        EventSem();
    }
    else {
        osProperties->references++;
        osProperties->referencesMux = 0;
    }
}

bool EventSem::Close() {

    bool ok = true;

    if (!osProperties->closed) {
        osProperties->closed = true;

        if (osProperties->eventHandle != static_cast<EventSemOSProperties *>(NULL)) {
            ok = (CloseHandle(osProperties->eventHandle) == TRUE);

        }

    }

    return ok;
}

EventSem::~EventSem() {
    if (osProperties != static_cast<EventSemOSProperties *>(NULL)) {
        while (!Atomic::TestAndSet(&osProperties->referencesMux)) {
        }

        if (osProperties->references == 1u) {
            if (!osProperties->closed) {
                Close();

            }

            delete osProperties;
            osProperties = static_cast<EventSemOSProperties *>(NULL);
        }
        else {
            osProperties->references--;
            osProperties->referencesMux = 0;

        }
    }

}

ErrorType EventSem::Wait() {
    ErrorType error = NoError;
    int ret = WaitForSingleObject(osProperties->eventHandle, INFINITE);

    if (ret == WAIT_FAILED) {
        error = OSError;
    }

    return error;
}

ErrorType EventSem::Wait(const TimeoutType &timeout) {
    ErrorType error = NoError;
    int ret = WaitForSingleObject(osProperties->eventHandle, timeout.GetTimeoutMSec());

    if (ret == WAIT_FAILED) {
        error = OSError;
    }

    if (ret == WAIT_TIMEOUT) {
        error = Timeout;
    }

    return error;
}

bool EventSem::Post() {

    return (SetEvent(osProperties->eventHandle) == TRUE);
}

bool EventSem::Reset() {

    return (ResetEvent(osProperties->eventHandle) == TRUE);
}

ErrorType EventSem::ResetWait(const TimeoutType &timeout) {
    Reset();
    return Wait(timeout);
}

EventSemOSProperties * EventSem::GetOSProperties() {
    return osProperties;
}

bool EventSem::IsClosed() const {
    bool ok = true;
    if (osProperties != static_cast<EventSemOSProperties *>(NULL)) {
        ok = osProperties->closed;
    }
    return ok;
}
