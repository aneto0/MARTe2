/**
 * @file ReplyMessageCatcherMessageFilter.h
 * @brief Header file for class ReplyMessageCatcherMessageFilter
 * @date Aug 19, 2016
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

 * @details This header file contains the declaration of the class ReplyMessageCatcherMessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_REPLYMESSAGECATCHERMESSAGEFILTER_H_
#define L4MESSAGES_REPLYMESSAGECATCHERMESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
* Allows catching a specific message reply
* The filters is removed after catching the reply
* */
class ReplyMessageCatcherMessageFilter: public MessageFilter{
public:
    CLASS_REGISTER_DECLARATION()

/**
 * TODO
 * Initialises basic search filter
 *
 */
ReplyMessageCatcherMessageFilter();

/**
 * TODO
 */
void SetMessageToCatch(const ReferenceT<Message> &message);

/**
 * TODO
 * Initialises basic search filter
 *
 */
virtual ~ReplyMessageCatcherMessageFilter();

/**
 * TODO
 * to be overridden to implement different synchronisation mechanism
 * Wait for caught
 */
virtual ErrorManagement::ErrorType Wait(const TimeoutType &maxWait = TTInfiniteWait, const uint32 pollingTimeUsec=1000);

protected:

    /**
     * TODO
     * on every match will also consume the message and handles both reply mechanisms
    */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

    /**
     * TODO
     * to be overridden to implement synchronisation mechanisms
     */
    virtual void HandleReplyMessage(ReferenceT<Message> &replyMessage){
        caught = true;
    }

    /*
     * TODO
     */
    bool ReplyCaught(){
        return caught;
    }

private:

    /**
     * TODO
     */
    bool caught;

    /**
     * TODO
     */
    ReferenceT<Message> originalMessage ;

};

}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4MESSAGES_REPLYMESSAGECATCHERMESSAGEFILTER_H_ */
	
