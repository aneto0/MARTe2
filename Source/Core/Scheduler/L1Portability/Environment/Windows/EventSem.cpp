/**
 * @file EventSem.cpp
 * @brief Source file for class EventSem
 * @date 20/07/2015
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

 * @details This source file contains the definition of all the methods for
 * the class EventSem (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "Atomic.h"
#include "Windows.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{


struct EventSemProperties {

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
    handle = new EventSemProperties();
    handle->eventHandle = NULL;
    handle->closed = true;
    handle->references = 1u;

    handle->referencesMux = 0;

}

bool EventSem::Create() {
    while (!Atomic::TestAndSet(&handle->referencesMux)) {
    }

    handle->closed = false;

    //If the handle is valid close it
    CloseHandle(handle->eventHandle);

    handle->eventHandle = CreateEvent(NULL, TRUE, FALSE, NULL);

    bool ok = (handle->eventHandle != NULL);

    handle->referencesMux = 0;
    return ok;
}

EventSem::EventSem(EventSem &source) {

    handle = source.GetProperties();
    if (handle == static_cast<EventSemProperties *>(NULL)) {
        //Capture the case that it got the handle reference while the source semaphore
        //was already being destructed...
        EventSem();
    }
    else {
        handle->references++;
        handle->referencesMux = 0;
    }
}

bool EventSem::Close() {

    bool ok = true;

    if (!handle->closed) {
        handle->closed = true;

        if (handle->eventHandle != static_cast<EventSemProperties *>(NULL)) {
            ok = (CloseHandle(handle->eventHandle) == TRUE);

        }

    }

    return ok;
}

EventSem::~EventSem() {
    if (handle != static_cast<EventSemProperties *>(NULL)) {
        while (!Atomic::TestAndSet(&handle->referencesMux)) {
        }

        if (handle->references == 1u) {
            if (!handle->closed) {
                Close();

            }

            delete handle;
            handle = static_cast<EventSemProperties *>(NULL);
        }
        else {
            handle->references--;
            handle->referencesMux = 0;

        }
    }

}

ErrorManagement::ErrorType EventSem::Wait() {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    int ret = WaitForSingleObject(handle->eventHandle, INFINITE);

    if (ret == WAIT_FAILED) {
        error = ErrorManagement::OSError;
    }

    return error;
}

ErrorManagement::ErrorType EventSem::Wait(const TimeoutType &timeout) {
    ErrorManagement::ErrorType error = ErrorManagement::NoError;
    int ret = WaitForSingleObject(handle->eventHandle, timeout.GetTimeoutMSec());

    if (ret == WAIT_FAILED) {
        error = ErrorManagement::OSError;
    }

    if (ret == WAIT_TIMEOUT) {
        error = ErrorManagement::Timeout;
    }

    return error;
}

bool EventSem::Post() {

    return (SetEvent(handle->eventHandle) == TRUE);
}

bool EventSem::Reset() {

    return (ResetEvent(handle->eventHandle) == TRUE);
}

ErrorManagement::ErrorType EventSem::ResetWait(const TimeoutType &timeout) {
    Reset();
    return Wait(timeout);
}

EventSemProperties * EventSem::GetProperties() {
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
