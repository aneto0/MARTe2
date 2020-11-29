/**
 * @file QueuedMessageI.cpp
 * @brief Source file for class QueuedMessageI
 * @date 22/04/2016
 * @author Filippo Sartori
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
 * the class QueuedMessageI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "QueuedMessageI.h"
#include "ErrorType.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

QueuedMessageI::QueuedMessageI() :
        MessageI(), queue(GlobalObjectsDatabase::Instance()->GetStandardHeap()), queueProcessingThread(binder), binder(*this, &QueuedMessageI::QueueProcessing) {

    ErrorManagement::ErrorType err;

    err.fatalError = !queue.IsValid();

    if (err.ErrorsCleared()) {
        // installs as last in the filter queue
        queue->SetName("QUEUE");
        err = MessageI::InstallMessageFilter(queue, -1);
    }
    if (!err.ErrorsCleared()) {
        REPORT_ERROR_STATIC(ErrorManagement::InitialisationError, "Failed to install message filter");
    }

}

QueuedMessageI::~QueuedMessageI() {
}


void QueuedMessageI::SetQueueName(const char8 * const name) {
    queueProcessingThread.SetName(name);
}

ErrorManagement::ErrorType QueuedMessageI::Start() {
    ErrorManagement::ErrorType err;

    err.fatalError = !queue.IsValid();

    if (err.ErrorsCleared()) {
        err = queueProcessingThread.Start();

    }

    return err;

}

ErrorManagement::ErrorType QueuedMessageI::Stop() {
    ErrorManagement::ErrorType err;

    err = queueProcessingThread.Stop();

    if (err.timeout.operator bool()) {

        err = queueProcessingThread.Stop();

    }

    return err;

}

/*lint -e{1764} EmbeddedServiceMethodBinderI callback method pointer prototype requires a non constant ExecutionInfo*/
ErrorManagement::ErrorType QueuedMessageI::QueueProcessing(ExecutionInfo &info) {
    ErrorManagement::ErrorType err;
    ReferenceT<Message> message;
    const TimeoutType timeout = 1000;

    // do not handle other stages
    if (info.GetStage() == ExecutionInfo::MainStage) {

        err.unsupportedFeature = !queue.IsValid();

        if (err.ErrorsCleared()) {

            err = queue->GetMessage(message, timeout);

            if (err.ErrorsCleared()) {
                err = queuedMessageFilters.ReceiveMessage(message);
                if (!message->IsReply()) {
                    if (message->ExpectsReply()) {
                        message->SetAsReply(true);
                        // handles indirect reply
                        if (message->ExpectsIndirectReply()) {
                            // simply produce a warning
                            // destination in reply is known so should not be set
                            (void) MessageI::SendMessage(message, NULL_PTR(Object *));
                        }
                    }
                }
            }
        }

        // not a reason to abort this thread
        err.timeout = false;
    }

    return err;

}

void QueuedMessageI::PurgeFilters() {
    queuedMessageFilters.Purge();
}

ErrorManagement::ErrorType QueuedMessageI::InstallMessageFilter(ReferenceT<MessageFilter> messageFilter, const int32 position) {
    ErrorManagement::ErrorType err;
    err.parametersError = !messageFilter.IsValid();
    if (err.ErrorsCleared()) {
        err.fatalError = !queuedMessageFilters.Insert(messageFilter, position);
    }

    return err;
}

ErrorManagement::ErrorType QueuedMessageI::RemoveMessageFilter(ReferenceT<MessageFilter> messageFilter) {
    ErrorManagement::ErrorType err;
    err.parametersError = !messageFilter.IsValid();
    if (err.ErrorsCleared()) {
        err.fatalError = !queuedMessageFilters.Delete(messageFilter);
    }
    return err;
}

}

