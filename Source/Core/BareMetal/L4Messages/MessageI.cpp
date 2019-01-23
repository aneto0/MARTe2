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
#include "Ticks.h"
#include "MicroSeconds.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

MessageI::MessageI() {
    messageFilters.SetTimeout(MilliSeconds::Infinite);

}

MessageI::~MessageI() {

}

ReferenceT<MessageI> MessageI::FindDestination(CCString destination) {
    ReferenceT<MessageI> destinationObject_MessageI;

    // simple search for named object
    Reference destinationObject = ObjectRegistryDatabase::Find(destination);
    if (destinationObject.IsValid()) {
        destinationObject_MessageI = destinationObject;
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
        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid message.");
    }

    // compute actual message parameters
    if (ret.ErrorsCleared()) {

        // is this is a reply (must be an indirect reply)
        // a direct reply does not need sending
        // therefore we will refuse sending it
        if (message->IsReply()) {

            if (!message->ExpectsIndirectReply()) {
                ret.communicationError = true;
                REPORT_ERROR(ErrorManagement::CommunicationError, "Message does not expect and indirect reply as it should.");
            }
            else {
                // if it is a reply then the destination is the original sender
                // Check if it exists in the tree!
                Reference ref(const_cast<Object *>(message->GetSender()));
                ReferenceContainer result;
                ReferenceContainerFilterReferences filter(1, ReferenceContainerFilterMode::RECURSIVE, ref);


                ReferenceT<ReferenceContainer> ord = ObjectRegistryDatabase::Access();
                if (ord.IsValid()){
                    ord->Find(result, filter);
                    if (result.Size() > 0u) {
                        destination = result.Get(0u);
                    }
                }
/*
                ReferenceContainer *ord = dynamic_cast<ReferenceContainer*>(ObjectRegistryDatabase::Access());
                if (ord != NULL_PTR(ReferenceContainer *)) {
                    ord->Find(result, filter);
                }
                if (result.Size() > 0u) {
                    destination = result.Get(0u);
                }
*/
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
                    REPORT_ERROR(ErrorManagement::CommunicationError, "Message expects reply but no sender was set.");
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
            REPORT_ERROR(ErrorManagement::UnsupportedFeature, "The destination object does not have a MessageI interface.");
            ret.unsupportedFeature = true;
        }
    }

    return ret;
}

ErrorManagement::ErrorType MessageI::WaitForReply(const ReferenceT<Message> &message,
                                                  const Ticks &maxWaitT,
                                                  const uint32 pollingTimeUsec) {
    ErrorManagement::ErrorType ok;

    uint64 endT = HighResolutionTimer::Counter();
    if (maxWaitT.IsValid() ) {
        endT += maxWaitT.GetTimeRaw();
    }

    ok.parametersError = !message.IsValid();
    REPORT_ERROR(ok, "Invalid message.");

    if (ok) {
        // no reply expected. why am I here?
        ok.communicationError = !message->ExpectsReply();
        REPORT_ERROR(ok, "No reply expected as it should.");
    }

    bool isReply = false;
    if (ok){
        isReply = message->IsReply();
    }

    MicroSeconds pollingTime(pollingTimeUsec,Units::us);
    while (ok && (!isReply)) {
        Sleep::NoMore(pollingTime);
        if (maxWaitT.IsValid() ) {
            ok.timeout = (HighResolutionTimer::Counter() > endT);
        }
        isReply = message->IsReply();
    }

    return ok;
}

ErrorManagement::ErrorType MessageI::SendMessageAndWaitReply(ReferenceT<Message> &message,
                                                             const Object * const sender,
                                                             const Ticks &maxWaitT,
                                                             const uint32 pollingTimeUsec) {
    ErrorManagement::ErrorType ret(true);

    /*
     * Verify all the error conditions at the beginning (Ivan's proposal):
     * !message.IsValid() => error
     * message->IsReplyMessage() => error
     */

    if (!message.IsValid()) {
        ret.parametersError = true;
        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid message.");
    }

    if (ret.ErrorsCleared()) {

        // mark that reply is expected
        message->SetExpectsReply(true);

        ret = SendMessage(message, sender);
    }

    if (ret.ErrorsCleared()) {

        ret = WaitForReply(message, maxWaitT, pollingTimeUsec);

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
                                                                     const MilliSeconds &maxWait,
                                                                     const uint32 pollingTimeUsec) {

    ErrorManagement::ErrorType ret(true);

    /*
     * Verify all the error conditions at the beginning (Ivan's proposal):
     * !message.IsValid() => error
     */

    if (!message.IsValid()) {
        ret.parametersError = true;
        REPORT_ERROR(ErrorManagement::ParametersError, "Invalid message.");
    }

    ReferenceT<ReplyMessageCatcherMessageFilter> replyMessageCatcher;
    ReferenceT<MessageFilter> messageCatcher;
    if (ret.ErrorsCleared()) {
        //Install message catcher
        ReferenceT<ReplyMessageCatcherMessageFilter> rmc(HeapManager::standardHeapId);
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

