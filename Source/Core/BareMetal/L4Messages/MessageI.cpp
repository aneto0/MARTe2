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

ReferenceT<MessageI> MessageI::FindDestination(CCString destination){
    ReferenceT<MessageI> destinationObject_MessageI;
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();

    if (ord != NULL_PTR(ObjectRegistryDatabase *)){
        Reference destinationObject = ord->Find(destination);
        if (destinationObject.IsValid()){
            destinationObject_MessageI = destinationObject;
        }
    }
    return destinationObject_MessageI;
}


bool MessageI::SendMessage( ReferenceT<Message> &message,Object *sender){
    CCString destination = "";
    bool ret = (message.IsValid());

    if (ret){
        if (message->IsReplyMessage()){

            // if it is a reply then the destination is the original sender
            destination = message->GetSender();
        } else {

            // if it is a reply then the destination is the original sender
            destination = message->GetDestination();

            if (sender != NULL){
                message->SetSender(sender->GetName());
            } else {
                // no Object ==> no reply possible
                if (message->ReplyExpected()){
                    // TODO produce error message
                    ret = false;
                }
            }
        }
    }

    if (ret){
        ReferenceT<MessageI> destinationObject = FindDestination(destination);

        if (destinationObject.IsValid()){
            ret = destinationObject->ReceiveMessage(message);
        }
    }

    return ret;
}



bool MessageI::SendMessageAndWaitReply(ReferenceT<Message> &message,Object *sender,TimeoutType maxWait){
    bool ret = (message.IsValid());

    if (ret){
        // reply to a reply NOT possible
        if (message->IsReplyMessage()){
            // TODO emit error
            ret = false;
        }
    }

    if (ret){
        // true means immediate reply
        message->MarkReplyExpected(true);
        message->SetReplyTimeout(maxWait);
    }

    if (ret){
        ret = SendMessage(message,sender);
    }
    return ret;
}

/**
 * TODO
 * Finds the target object
 * Calls the ReceiveMessage function of the target
 * Reply is expected but does not Waits for a reply and returns
 * */
bool MessageI::SendMessageAndExpectReplyLater(ReferenceT<Message> &message,Object *sender){
    bool ret = (message.IsValid());

    if (ret){
        // reply to a reply NOT possible
        if (message->IsReplyMessage()){
            // TODO emit error
            ret = false;
        }
    }

    if (ret){
        // false means decoupled reply
        message->MarkReplyExpected(false);
    }

    if (ret){
        ret = SendMessage(message,sender);
    }
    return ret;
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


bool MessageI::ReceiveMessage(ReferenceT<Message> &message) {
    return SortMessage(message);
}

/**
 * TODO
 * Default message sorting mechanism
 * By default checks if there are usable registered methods
 * Otherwise calls HandleMessage
 * */
bool MessageI::SortMessage(ReferenceT<Message> &message){
    // the return of the function
    bool ret = true;
    // whether we found a function
    bool done = false;

    Object *thisAsObject = dynamic_cast<Object *> (this);

    //if this is an Object derived class then we can look for a registered method to call
    if (thisAsObject != NULL){

        // we will not emit errors here is the message is invalid.
        if (message.IsValid()){

            CCString function = message->GetFunction();
            if (message->IsReplyMessage()){
                function = "HandleReply";
            }

            ClassMethodReturn fret = thisAsObject->CallRegisteredMethod(function,*(message.operator->()));

            done = (fret.methodFound && fret.prototypeMismatch);
            if (done){
                ret = fret.AllOk();
            }
        }
    }

    // if no registered method found
    if (!done) {
        ret = HandleMessage(message);
    }

    return ret;
}

bool MessageI::HandleMessage(ReferenceT<Message> &message){
    return false;
}


}

	
