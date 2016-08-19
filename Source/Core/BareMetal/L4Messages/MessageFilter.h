/**
 * @file MessageFilter.h
 * @brief Header file for class MessageFilter
 * @date Aug 17, 2016
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

 * @details This header file contains the declaration of the class MessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_MESSAGEFILTER_H_
#define L4MESSAGES_MESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "ReferenceContainerFilter.h"
#include "CString.h"
#include "Message.h"
#include "StringHelper.h"





namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief class to implement a filter on Messages.
 */
class MessageFilter: public Object,public ReferenceContainerFilter{
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * TODO
     * Initialises basic search filter
     *
     */
    MessageFilter(bool isPermanentFilter);

    /**
     * TODO
     * Initialises basic search filter
     *
     */
    virtual ~MessageFilter();

    /**
     * TODO
     */
    inline bool IsPermanentFilter(){
        return permanentFilter;
    }

protected:

    /**
     * TODO
     * Single test of a message.
     * Also try consuming (uses and does not delete it) the message if matched
    */
    virtual ErrorManagement::ErrorType TestMessage(ReferenceT<Message> &messageToTest)= 0;

    /**
     * TODO
     * Was the message consumed?
    */
    virtual bool Matched(ErrorManagement::ErrorType ret){
        return bool(ret);
    }

    /**
     * TODO
     * Allows handling multiple messages in a container
       */
    virtual bool Test(ReferenceContainer &previouslyFound,Reference &referenceToTest){

        ReferenceT<Message> &messageToTest = referenceToTest;
        return Matched(TestMessage(messageToTest));
    }

private:

    /**
     * True if it remains active after a successful match
     */
    bool permanentFilter;

};

/**
 * TODO
   */
MessageFilter::MessageFilter(bool isPermanentFilter):ReferenceContainerFilter(-1,ReferenceContainerFilterMode::REMOVE){
    permanentFilter = isPermanentFilter;
}

/**
 * TODO
   */
MessageFilter::~MessageFilter(){

}


} // namespace
#if 0

class GenericMessageFilter: public MessageFilter{
public:
    CLASS_REGISTER_DECLARATION()


/**
 * TODO
 * Initialises basic search filter
 *
 */
GenericMessageFilter();

/**
 * TODO
 * Initialises basic search filter
 *
 */
virtual ~GenericMessageFilter();

protected:

    /**
     * TODO
     * if set to NULL this search criteria always passes
     * Allows selecting messages on the base of the sender
     */
    CCString senderFilter;

    /**
     * TODO
     * if set to true replies are selected
     * if set to false this check is waivered
     */
    bool replyFilter;

    /**
     * TODO
     * if set to NULL this search criteria always passes
     * Allows selecting messages on the base of the sender
     */
    CCString functionName;

    /**
     * TODO
     * if set to invalid
     * Allows intercepting returning messages as reply
     */
    ReferenceT<Message> &originalMessage;

    /**
     * TODO
       */
    virtual ErrorManagement::ErrorType GenericMessageFilter::TestMessage(ReferenceT<Message> &messageToTest);

};


GenericMessageFilter::GenericMessageFilter():originalMessage(){
    replyFilter = false;
    functionName = NULL_PTR(CCString);
    senderFilter = NULL_PTR(CCString);
}


/**
 * TODO
*/
ErrorManagement::ErrorType GenericMessageFilter::TestMessage(ReferenceT<Message> &messageToTest){
    bool matched = true;

    if (senderFilter != NULL_PTR(CCString)) {
        matched &= (StringHelper::Compare(senderFilter,messageToTest->GetSender()) );
    }

    if (matched && replyFilter){
        matched &= messageToTest->IsReply() ;
    }

    if (matched && (functionName != NULL_PTR(CCString) ) ) {
        matched &= (StringHelper::Compare(functionName,messageToTest->GetFunction() ) );
    }

    if (matched && originalMessage.IsValid() ) {
        matched &= (originalMessage == messageToTest );
    }

    return matched;

}

#endif



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4MESSAGES_MESSAGEFILTER_H_ */
	
