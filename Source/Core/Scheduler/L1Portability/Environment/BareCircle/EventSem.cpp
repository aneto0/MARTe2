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
#include <sys/timeb.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Atomic.h"
#include "ErrorManagement.h"
#include "EventSem.h"
#include "FastPollingEventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{9109} forward declaration in EventSem.h is required to define the class*/
struct EventSemProperties {

    FastPollingEventSem sem;

};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EventSem::EventSem() {
    handle=new EventSemProperties;

}

EventSem::EventSem(EventSem &source) {
    *handle = *(source.handle);
}

/*lint -e{1551} only C calls are performed. No exception can be raised*/
EventSem::~EventSem() {
    delete handle;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Create() {
    handle->sem.Create();
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Close() {
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
ErrorManagement::ErrorType EventSem::Wait() {


    return !(handle->sem.FastWait(TTInfiniteWait, 1e-3));
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
ErrorManagement::ErrorType EventSem::Wait(const TimeoutType &timeout) {
    return !(handle->sem.FastWait(timeout, 1e-3));
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Post() {
    handle->sem.FastPost();
    return true;
}

/*lint -e{613} guaranteed by design that it is not possible to call this function with a NULL
 * reference to handle*/
bool EventSem::Reset() {
    handle->sem.Reset();
    return true;
}

ErrorManagement::ErrorType EventSem::ResetWait(const TimeoutType &timeout) {
    return handle->sem.FastResetWait(timeout, 1e-3);
}

EventSemProperties *EventSem::GetProperties() {
    return handle;
}

bool EventSem::IsClosed() const {
    return false;
}
}
