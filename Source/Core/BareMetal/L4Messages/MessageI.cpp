/**
 * @file messageI.cpp
 * @brief Source file for class messageI
 * @date Apr 14, 2016
 * @author fsartori
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MessageI::MessageI() {

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
        ret.parametersError= true;
        // TODO produce error message
    }

    // compute actual message parameters
    if (ret.NoError()) {
        // is this is a reply (must be a late reply)
        if (message->IsReplyMessage()) {

            if (!message->LateReplyExpected()) {
                ret.communicationError = true;
                // TODO produce error message
            }
            else {
                message->MarkLateReplyExpected(false);
                //{message->IsReplyMessage() and message->LateReplyExpected()}
                // if it is a reply then the destination is the original sender
                destination = message->GetSender();
            }

            // not a reply
        }
        else {

            // if it is a reply then the destination is the original sender
            destination = message->GetDestination();

            // assigns the sender
            if (sender != NULL) {
                message->SetSender(sender->GetName());
            }
            else {
                // no Object ==> no reply possible
                if (message->ReplyExpected()) {
                    // TODO produce error message
                    ret.parametersError = true;
                }
            }
        }
    }

    if (ret.NoError()) {
        ReferenceT<MessageI> destinationObject = FindDestination(destination);

        if (destinationObject.IsValid()) {
            ret = destinationObject->ReceiveMessage(message);
        }
        else {
            ret.unsupportedFeature = true;
            // TODO produce error message
        }
    }

    if (ret.NoError()) {
        // if we wanted an immediate reply then we should have one
        bool isImmediateReplyExpected=message->ImmediateReplyExpected();
        bool isReply=message->IsReplyMessage();
        if ((isImmediateReplyExpected) && (!isReply)) {
            ret.communicationError = true;
            // TODO produce error message
        }
    }

    return ret;
}

ErrorManagement::ErrorType MessageI::SendMessageAndWaitReply(ReferenceT<Message> &message,
                                                             const Object * const sender,
                                                             const TimeoutType &maxWait) {
    ErrorManagement::ErrorType ret(true);

    /*
     * TODO: Verify all the error conditions at the beginning:
     * !message.IsValid() => error
     * message->IsReplyMessage() => error
     */

    if (!message.IsValid()) {
        ret.parametersError = true;
        // TODO produce error message
    }

    if (ret.NoError()) {
        // reply to a reply NOT possible
        if (message->IsReplyMessage()) {
            // TODO emit error
            ret.communicationError = true;
        }
    }

    if (ret.NoError()) {
        // true means immediate reply
        message->MarkImmediateReplyExpected();
        message->SetReplyTimeout(maxWait);

        ret = SendMessage(message, sender);
    }
    return ret;
}

/**
 * TODO
 * Finds the target object
 * Calls the ReceiveMessage function of the target
 * Reply is expected but does not Waits for a reply and returns
 * */
ErrorManagement::ErrorType MessageI::SendMessageAndExpectReplyLater(ReferenceT<Message> &message,
                                                                    const Object * const sender) {
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

    if (ret.NoError()) {
        // reply to a reply NOT possible
        if (message->IsReplyMessage()) {
            // TODO emit error
            ret.communicationError = true;
        }
    }

    if (ret.NoError()) {
        // false means decoupled reply
        message->MarkLateReplyExpected();

        ret = SendMessage(message, sender);
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ErrorManagement::ErrorType MessageI::ReceiveMessage(ReferenceT<Message> &message) {
    return SortMessage(message);
}

/**
 * TODO
 * Default message sorting mechanism
 * By default checks if there are usable registered methods
 * Otherwise calls HandleMessage
 * */
ErrorManagement::ErrorType MessageI::SortMessage(ReferenceT<Message> &message) {

    ErrorManagement::ErrorType ret;

    /*
     * TODO: Verify all the error conditions at the beginning:
     * !message.IsValid() => error
     * thisAsObject == NULL_PTR(Object *) => error
     */

    Object *thisAsObject = dynamic_cast<Object *>(this);

    // why? The Send already controls this-
    if (!message.IsValid()) {
        ret.parametersError = true;
        // TODO produce error message
    }

    //if this is an Object derived class then we can look for a registered method to call

    // why? If the Send finds in the ORD, it returns a Reference which points always to an Object, then there is
    // no need of this check
    if (thisAsObject == NULL_PTR(Object *)) {
        ret.parametersError = true;
        // TODO produce error message
    }

    if (ret.NoError()) {
        CCString function = message->GetFunction();
        if (message->IsReplyMessage()) {
            function = "HandleReply";
        }

        /*lint -e{613} .NULL check has been done before entering here*/
        ret = thisAsObject->CallRegisteredMethod(function, *(message.operator->()));
        bool isReplyExpected=message->ReplyExpected();
        // automatically mark the message as reply
        if (ret && isReplyExpected) {
            message->MarkAsReply();
        }

    }

    // check if errors are only of function mismatch
    if (!ret.NoError()) {
        ErrorManagement::ErrorType saveRet = ret;
        // try resetting the "good" errors
        ret.unsupportedFeature = false;
        ret.parametersError = false;
        if (ret.NoError()) {
            ret = HandleMessage(message);
        }
        else {
            ret = saveRet;
        }
    }

    // shall we send a reply?
    bool isLateReplyExpected=(message->LateReplyExpected());
    bool isReply=(message->IsReplyMessage());
    if ((ret.NoError()) && (isLateReplyExpected) && (isReply)) {
        ret = MessageI::SendMessage(message);
    }

    return ret;
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Default implementation does not concern about input parameters */
ErrorManagement::ErrorType MessageI::HandleMessage(ReferenceT<Message> &message) {
    return ErrorManagement::UnsupportedFeature;
}

}

