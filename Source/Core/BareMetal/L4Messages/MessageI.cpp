/**
 * @file messageI.cpp
 * @brief Source file for class messageI
 * @date 14/04/2016
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
 * the class messageI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MessageI.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReplyMessageCatcherMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MessageI::MessageI() {
    messageFilters.SetTimeout(TTInfiniteWait);

}

MessageI::~MessageI() {

}

ReferenceT<MessageI> MessageI::FindDestination(CCString destination) {
    ReferenceT<MessageI> destinationObject_MessageI;
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    // simple search for named object
    if (ord != NULL_PTR(ObjectRegistryDatabase *)) {
        Reference destinationObject = ord->Find(destination);
        if (destinationObject.IsValid()) {
            destinationObject_MessageI = destinationObject;
        }
    }

    // if (!destinationObject_MessageI.IsValid())
    // TODO search via brokers
    // partial find of brokers within the specified path
    // build and return reference to remote object handler

    return destinationObject_MessageI;
}

ErrorManagement::ErrorType MessageI::SendMessage(ReferenceT<Message> &message,
                                                 const Object * const sender) {
    CCString destination = "";
    ErrorManagement::ErrorType ret;

    /*
     * TODO: Verify all the error conditions at the beginning:
     *
     * !message.IsValid() => error
     * message->IsReplyMessage() && !message->LateReplyExpected() => error
     * !message->IsReplyMessage() && sender == NULL && message->ReplyExpected() => error
     * !destinationObject.IsValid() => error
     * message->ImmediateReplyExpected() && !message->IsReplyMessage() => error
     */

    if (!message.IsValid()) {
        ret.parametersError = true;
        // TODO produce error message
    }

    // compute actual message parameters
    if (ret.ErrorsCleared()) {

        // is this is a reply (must be an indirect reply)
        // a direct reply does not need sending
        // therefore we will refuse sending it
        if (message->IsReply()) {

            if (!message->ExpectsIndirectReply()) {
                ret.communicationError = true;
                // TODO produce error message
            }
            else {

                //{message->IsReply() and message->ExpectsIndirectReply()}
                // if it is a reply then the destination is the original sender
                destination = message->GetSender();
            }

        }
        else { // not a reply

            // if it is not a reply then use the proper destination
            destination = message->GetDestination();

            // assigns the sender
            if (sender != NULL) {
                message->SetSender(sender->GetName());
            }
            else {
                // no Object ==> no reply possible
                // therefore refuse sending
                if (message->ExpectsReply()) {
                    // TODO produce error message
                    ret.parametersError = true;
                }
            }
        }
    }

    if (ret.ErrorsCleared()) {
        // implicit dynamic cast here
        ReferenceT<MessageI> destinationObject = FindDestination(destination);

        if (destinationObject.IsValid()) {
            ret = destinationObject->messageFilters.ReceiveMessage(message);
        }
        else {
            ret.unsupportedFeature = true;
            // TODO produce error message
            // the object being addressed does not have the MessageI interface
        }
    }

    return ret;
}

ErrorManagement::ErrorType MessageI::WaitForReply(ReferenceT<Message> &message,
                                                  const TimeoutType &maxWait,
                                                  const uint32 pollingTimeUsec) {

    uint64 start = HighResolutionTimer::Counter();
    float32 pollingTime = (float) pollingTimeUsec * 1.0e-6;
    ErrorManagement::ErrorType err(true);

    if (!message.IsValid()) {
        err.parametersError = true;
        // TODO produce error message
    }

    // no reply expected. why am I here?
    if (!message->ExpectsReply()) {
        // TODO emit error
        err.communicationError = true;
    }

    while (err.ErrorsCleared() && !message->IsReply()) {
        Sleep::NoMore(pollingTime);
        if (maxWait != TTInfiniteWait) {
            uint64 deltaT = HighResolutionTimer::Counter() - start;
            err.timeout = maxWait.HighResolutionTimerTicks() > deltaT;
        }
    }

    return err;
}

ErrorManagement::ErrorType MessageI::SendMessageAndWaitReply(ReferenceT<Message> &message,
                                                             const Object * const sender,
                                                             const TimeoutType &maxWait,
                                                             const uint32 pollingTimeUsec) {
    ErrorManagement::ErrorType ret(true);

    /*
     * Verify all the error conditions at the beginning (Ivan's proposal):
     * !message.IsValid() => error
     * message->IsReplyMessage() => error
     */

    if (!message.IsValid()) {
        ret.parametersError = true;
        // TODO produce error message
    }

    if (ret.ErrorsCleared()) {

        // mark that reply is expected
        message->SetExpectsReply(true);

        ret = SendMessage(message, sender);
    }

    if (ret.ErrorsCleared()) {

        ret = WaitForReply(message, maxWait, pollingTimeUsec);

    }

    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ErrorManagement::ErrorType MessageI::InstallMessageFilter(ReferenceT<MessageFilter> messageFilter,
                                                          CCString name,
                                                          int32 position) {

    return messageFilters.InstallMessageFilter(messageFilter, name, position);
}

ErrorManagement::ErrorType MessageI::RemoveMessageFilter(ReferenceT<MessageFilter> messageFilter) {
    return messageFilters.RemoveMessageFilter(messageFilter);

}

ErrorManagement::ErrorType MessageI::RemoveMessageFilter(CCString name) {
    return messageFilters.RemoveMessageFilter(name);
}

ErrorManagement::ErrorType MessageI::SendMessageAndWaitIndirectReply(ReferenceT<Message> &message,
                                                                     const TimeoutType &maxWait,
                                                                     const uint32 pollingTimeUsec) {

    ErrorManagement::ErrorType ret(true);

    /*
     * Verify all the error conditions at the beginning (Ivan's proposal):
     * !message.IsValid() => error
     */

    if (!message.IsValid()) {
        ret.parametersError = true;
        // TODO produce error message

        message->SetExpectsIndirectReply();
        message->SetExpectsReply();
    }

    ReferenceT<ReplyMessageCatcherMessageFilter> replyMessageCatcher;
    ReferenceT<MessageFilter> messageCatcher;
    if (ret.ErrorsCleared()) {
        //Install message catcher
        ReferenceT<ReplyMessageCatcherMessageFilter> rmc(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        replyMessageCatcher = rmc;

        ret.fatalError = !replyMessageCatcher.IsValid();
    }

    if (ret.ErrorsCleared()) {
        replyMessageCatcher->SetMessageToCatch(message);

        messageCatcher = replyMessageCatcher;

        ret = InstallMessageFilter(messageCatcher, "");
    }

    if (ret.ErrorsCleared()) {
        Object *thisObject = dynamic_cast<Object *>(this);

        if (thisObject != NULL) {
            ret = SendMessage(message, thisObject);
        }

        if (ret.ErrorsCleared()) {
            ret = replyMessageCatcher->Wait(maxWait, pollingTimeUsec);
        }

        // try remove the message filter in any case whatever happened ..
        ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, messageCatcher);
        ReferenceContainer result;
        //Locking is already done inside the Find
        messageFilters.Find(result, filter);
        if (result.Size() > 0u) {
            ret = static_cast<uint32>(ret) | static_cast<uint32>(RemoveMessageFilter(messageCatcher));
        }
    }
    return ret;
}

}

