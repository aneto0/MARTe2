/**
 * @file MessageI.cpp
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

#include "AdvancedErrorManagement.h"
#include "MessageI.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "ReferenceContainerFilterReferences.h"
#include "ReplyMessageCatcherMessageFilter.h"
#include "ReplyMessageCatcherMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
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
        else {
            /*lint -e{1793} GetList() is used to allow CCString to be passed to the REPORT_ERROR_STATIC*/
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "The destination object with name %s does not exist", destination.GetList());
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
    Reference destination;
    ErrorManagement::ErrorType ret;

    if (!message.IsValid()) {
        ret.parametersError = true;
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "Invalid message.");
    }

    // compute actual message parameters
    if (ret.ErrorsCleared()) {

        // is this is a reply (must be an indirect reply)
        // a direct reply does not need sending
        // therefore we will refuse sending it
        if (message->IsReply()) {

            if (!message->ExpectsIndirectReply()) {
                ret.communicationError = true;
                REPORT_ERROR_STATIC_0(ErrorManagement::CommunicationError, "Message does not expect and indirect reply as it should.");
            }
            else {
                // if it is a reply then the destination is the original sender
                // Check if it exists in the tree!
                Reference ref(const_cast<Object *>(message->GetSender()));
                ReferenceContainer result;
                ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, ref);
                ReferenceContainer *ord = dynamic_cast<ReferenceContainer*>(ObjectRegistryDatabase::Instance());
                if (ord != NULL_PTR(ReferenceContainer *)) {
                    ord->Find(result, filter);
                }
                if (result.Size() > 0u) {
                    destination = result.Get(0u);
                }
            }

        }
        else { // not a reply

            // if it is not a reply then use the proper destination
            destination = FindDestination(message->GetDestination());

            // assigns the sender
            if (sender != NULL) {
                message->SetSender(sender);
            }
            else {
                // no Object ==> no reply possible
                // therefore refuse sending
                if (message->ExpectsReply()) {
                    REPORT_ERROR_STATIC_0(ErrorManagement::CommunicationError, "Message expects reply but no sender was set.");
                    ret.parametersError = true;
                }
            }
        }
    }

    if (ret.ErrorsCleared()) {
        // implicit dynamic cast here
        ReferenceT<MessageI> destinationObject = destination;

        if (destinationObject.IsValid()) {
            ret = destinationObject->messageFilters.ReceiveMessage(message);
        }
        else {
            /*lint -e{1793} GetList() is used to allow CCString to be passed to the REPORT_ERROR_STATIC*/
            REPORT_ERROR_STATIC(ErrorManagement::UnsupportedFeature, "The destination object with name %s does not have a MessageI interface.", message->GetDestination().GetList());
            ret.unsupportedFeature = true;
        }
    }

    return ret;
}

ErrorManagement::ErrorType MessageI::WaitForReply(const ReferenceT<Message> &message,
                                                  const TimeoutType &maxWait,
                                                  const uint32 pollingTimeUsec) {
    ErrorManagement::ErrorType err(true);

    if (!message.IsValid()) {
        err.parametersError = true;
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "Invalid message.");
    }

    if (err.ErrorsCleared()) {
        // no reply expected. why am I here?
        if (!message->ExpectsReply()) {
            REPORT_ERROR_FULL(ErrorManagement::CommunicationError, "No reply expected as it should.");
            err.communicationError = true;
        }
    }

    uint64 start = HighResolutionTimer::Counter();
    float32 pollingTime = static_cast<float32>(pollingTimeUsec);
    pollingTime *= static_cast<float32>(1.0e-6);
    bool isReply = false;
    if(err.ErrorsCleared()){
        isReply = message->IsReply();
    }
    while ((err.ErrorsCleared()) && (!isReply)) {
        Sleep::NoMore(pollingTime);
        if (maxWait != TTInfiniteWait) {
            uint64 deltaT = HighResolutionTimer::Counter() - start;
            err.timeout = maxWait.HighResolutionTimerTicks() > deltaT;
        }
        isReply = message->IsReply();
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
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "Invalid message.");
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

ErrorManagement::ErrorType MessageI::InstallMessageFilter(ReferenceT<MessageFilter> messageFilter,
                                                          const int32 position) {

    return messageFilters.Insert(messageFilter, position);
}

ErrorManagement::ErrorType MessageI::RemoveMessageFilter(ReferenceT<MessageFilter> messageFilter) {
    return messageFilters.Delete(messageFilter);

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
        REPORT_ERROR_STATIC_0(ErrorManagement::ParametersError, "Invalid message.");
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

        ret = InstallMessageFilter(messageCatcher);
    }

    if (ret.ErrorsCleared()) {
        /*lint -e{740} [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: It is expected that the final class inherits both from MessageI and from Object. */
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

