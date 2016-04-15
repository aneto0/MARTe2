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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ReferenceT<MessageI> MessageI::FindDestination(CCString destination){
    ReferenceT<MessageI> destinationObject_MessageI;

    Reference destinationObject = ObjectRegistryDatabase::Instance()->Find(destination);
    if (destinationObject.IsValid()){
        ReferenceT<MessageI> destinationObject_MessageI = destinationObject;
    }
    return destinationObject_MessageI;
}


bool MessageI::SendMessage( ReferenceT<Message> &message,Object *sender){
    bool ret = (message.IsValid());

    if (ret){

        if (sender != NULL){
            message->SetSender(sender->GetName());
        }

        ReferenceT<MessageI> destinationObject_MessageI = FindDestination(message->GetDestination());

        if (destinationObject_MessageI.IsValid()){
            ret = destinationObject_MessageI->ReceiveMessage(message);
        }
    }

    return ret;
}


bool MessageI::SendMessageAndWaitReply(ReferenceT<Message> &message,Object *sender){
    bool ret = (message.IsValid());

    if (ret){
        message->MarkReplyExpected();

        if (sender != NULL){
            message->SetSender(sender->GetName());
        }

        ReferenceT<MessageI> destinationObject_MessageI = FindDestination(message->GetDestination());

        if (destinationObject_MessageI.IsValid()){
            ret = destinationObject_MessageI->ReceiveMessage(message);
        }

        // TODO
        // check for isReply set.
        if (!message->IsReplyMessage()){
            ret = false;
            // TODO handle errors
        }

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
        message->MarkReplyExpected(true);

        if (sender != NULL){
            message->SetSender(sender->GetName());
        }

        ReferenceT<MessageI> destinationObject_MessageI = FindDestination(message->GetDestination());

        if (destinationObject_MessageI.IsValid()){
            ret = destinationObject_MessageI->ReceiveMessage(message);
        }

        // TODO
        // check for isReply set.
        if (!message->ReplyMessagePlanned() ){
            ret = false;
            // TODO handle errors
        }

    }

    return false;

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

    Object *this_Object = dynamic_cast<Object *> (this);


    //TODO look for a method matching the name
    if (this_Object != NULL){

    }

   //check if Object derived and look for registered method
   //check if ..
   //otherwise
   return HandleMessage(message);
}

bool MessageI::HandleMessage(ReferenceT<Message> &message){
    return false;
}


}

	
