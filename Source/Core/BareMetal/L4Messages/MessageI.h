/**
 * @file MessageI.h
 * @brief Header file for class MessageI
 * @date Apr 5, 2016
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

 * @details This header file contains the declaration of the class MessageI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGEI_H_
#define MESSAGEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Message.h"
#include "TimeoutType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * TODO
 * */
class MessageI  {
public:
    /**
     * TODO
     * Finds the target object
     * Calls the ReceiveMessage function of the target
     * The sync/async behaviour depends on the flags set in the message
     * by default: Reply is not expected here. Returns whenever possible
     * if Message::flags.expectsReply= true          - Then reply message will requested but not waited for
     * if Message::flags.expectsImmediateReply= true - Then reply message will be waited for at the destination
     * */
    static bool SendMessage( ReferenceT<Message> &message,Object *sender = NULL);

    /**
     * TODO
     * Sets Message::maxWait
     * Sets Message::flags.expectsImmediateReply and calls SendMessage
     * */
    static bool SendMessageAndWaitReply(ReferenceT<Message> &message,Object *sender = NULL,TimeoutType maxWait);

    /**
     * TODO
     * Sets Message::flags.expectsReply and calls SendMessage
     * */
    static bool SendMessageAndExpectReplyLater(ReferenceT<Message> &message,Object *sender = NULL);


    virtual ~MessageI(){};

protected:

    /**
      * TODO
      * Default message handling mechanism
      * Handles the reception of a message
      * By default simply calls SortMessage
      * Can be overridden to implement message Queues etc...
      * true - message is accepted
      * false - message is rejected
     * */
     virtual bool ReceiveMessage(ReferenceT<Message> &message);

     /**
      * TODO
      * Default message sorting mechanism
      * By default checks if there are usable registered methods
      * Otherwise calls HandleMessage
      * */
     virtual bool SortMessage(ReferenceT<Message> &message);

    /**
      * TODO
      * Default message handling mechanism
      * By default refuses messages returning false
     * */
     virtual bool HandleMessage(ReferenceT<Message> &message);

private:

     static ReferenceT<MessageI> FindDestination(CCString destination);


};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



#endif /* MESSAGEI_H_ */
	
