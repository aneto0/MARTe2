/**
 * @file Message.h
 * @brief Header file for class Message
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

 * @details This header file contains the declaration of the class Message
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "CString.h"
#include "StreamString.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 *
 * */
class DLL_API Message: public ReferenceContainer{

    /**
     * who is the originator of the message
     * empty means anonymous
     * */
    StreamString sender;

    /**
     * who is the destination of the message
     * addressed from GODB forward
     * */
    StreamString destination;

    /**
     * what is the function of this message
     * */
    StreamString function;

    /**
     * in case of synchronous communication, how long to wait
     * */
    TimeoutType maxWait;

    struct MessageFlags {
        /**
         * init:false
         * true if reply is required
         * set by the sender
         */
        bool expectsReply:1;

        /**
         * init:false
         * true if reply is required
         * and I am going to wait for it in the call itself
         * set by the sender
         */
        bool expectsImmediateReply:1;


        /**
         * init:false
         * true for a reply message, false for a normal message
         * in a reply message sender and destination are implicitly flipped
         * set by the recipient:HandleMessage or by the caller of the recipient::SortMessage
        */
        bool isReply:1;

        /**
         * init:false
         * true for a reply message that has been/ will be sent asynchronously
         * the variable function is not used and instead HandleReply will be called on the sender to handle the reply
         * set by recipient::ReceiveMessage
         */
        bool willBeAsyncReply:1;

        /**
         * init:false
         * true if the reply is from the call mechanism and not the actual user code
         * isReply is assumed true;
         * set by the caller of the recipient::SortMessage
         */
        bool missingReply:1;

        MessageFlags(){
            expectsReply          = false;
            expectsImmediateReply = false;
            isReply               = false;
            willBeAsyncReply      = false;
            missingReply          = false;
        }

    } mode;
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * TODO
     * */
    Message(){}

    /**
     * TODO
     * */
    virtual ~Message(){

    }

    /**
     * TODO
     * sets payload, Destination and Function
     * */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * TODO
     * marked by send when requiring reply
     * */
    void MarkReplyExpected(bool immediate = false){
        mode.expectsReply = true;
        mode.expectsImmediateReply = true;
    }

    /**
     * TODO
     * */
    bool IsReplyMessage(){
        return mode.isReply;
    }

    /**
     * TODO
     * */
    bool ReplyMessagePlanned(){
        return mode.willBeAsyncReply;
    }

    /**
     * TODO
     * */
    CCString GetDestination(){
        return destination.Buffer();
    }

    /**
     * TODO
     * */
    CCString GetSender(){
        return sender.Buffer();
    }

    /**
     * TODO
     * */
    void SetSender(CCString senderName){
        sender = senderName;
    }

    /**
     * TODO
     * */
    CCString GetFunction(){
        return function.Buffer();
    }

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/




#endif /* MESSAGE_H_ */
	
