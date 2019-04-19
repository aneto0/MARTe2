/**
 * @file QueuedReplyMessageCatcherFilter.cpp
 * @brief Source file for class QueuedReplyMessageCatcherFilter
 * @date 07/10/2016
 * @author Andre Neto
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
 * the class QueuedReplyMessageCatcherFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "QueuedReplyMessageCatcherFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

QueuedReplyMessageCatcherFilter::QueuedReplyMessageCatcherFilter() :
        MessageFilter(false),
        Object() {
    eventSem = NULL_PTR(EventSem *);
}

void QueuedReplyMessageCatcherFilter::SetMessagesToCatch(ReferenceContainer &messagesToCatchIn) {
    messagesToCatch = messagesToCatchIn;
}

void QueuedReplyMessageCatcherFilter::SetEventSemaphore(EventSem &eventSemIn) {
    if (eventSem != NULL_PTR(EventSem *)) {
        delete eventSem;
    }
    eventSem = new EventSem(eventSemIn);
}

/*lint -e{1551} the event semaphore must be destroyed in the destructor*/
QueuedReplyMessageCatcherFilter::~QueuedReplyMessageCatcherFilter() {
    if (eventSem != NULL_PTR(EventSem *)) {
        delete eventSem;
    }
}

ErrorManagement::ErrorType QueuedReplyMessageCatcherFilter::ConsumeMessage(ReferenceT<Message> &messageToTest) {

    ErrorManagement::ErrorType ret(true);
    //check reply flag
    if (!messageToTest->IsReply()) {
        REPORT_ERROR(ErrorManagement::Warning, "The message caught is not a reply %s", messageToTest.operator ->()->GetName());
        ret.warning = true;
    }
    bool ok = ret.ErrorsCleared();
    if (ok) {
        uint32 i;
        bool found = false;
        for (i = 0u; (i < messagesToCatch.Size()) && (ok) && (!found); i++) {
            ReferenceT<Message> message = messagesToCatch.Get(i);
            ok = message.IsValid();
            if (ok) {
                found = (message == messageToTest);
            }
            else {
                ret.unsupportedFeature = true;
            }

            if (found) {
                ok = messagesToCatch.Delete(message);
            }
        }
        if (!found) {
            ret.unsupportedFeature = true;
        }
    }
    ok = ret.ErrorsCleared();
    if (ok) {
        if (messagesToCatch.Size() == 0u) {
            if (eventSem != NULL_PTR(EventSem *)) {
                ret.fatalError = !eventSem->Post();
            }
            else {
                REPORT_ERROR(ErrorManagement::FatalError, "The event semaphore was not set!");
                ret.fatalError = true;
            }
        }
        else {
            //Do not allow the MessageFilter to remove the filter (which is not permanent) before all the messages are caught
            ret.notCompleted = true;
        }
    }
    return ret;

}

CLASS_REGISTER(QueuedReplyMessageCatcherFilter, "1.0")

}

