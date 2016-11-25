/**
 * @file QueuedReplyMessageCatcherFilter.h
 * @brief Header file for class QueuedReplyMessageCatcherFilter
 * @date 7/10/2016
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class QueuedReplyMessageCatcherFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef QUEUEDREPLYMESSAGECATCHERFILTER_H_
#define QUEUEDREPLYMESSAGECATCHERFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "EventSem.h"
#include "MessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Allows catching a container of message replies.
 * @details When all the reply messages are caught an event semaphore is posted.
 * The filter is removed after catching the reply.
 */
class DLL_API QueuedReplyMessageCatcherFilter: public MessageFilter, public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. Initialises the MessageFilter(false).
     * @post
     *   IsPermanentFilter() == false
     */
    QueuedReplyMessageCatcherFilter();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~QueuedReplyMessageCatcherFilter();

    /**
     * @brief Sets the reply messages to be caught.
     * @param[in] messagesToCatchIn the reply messages to be caught.
     */
    void SetMessagesToCatch(ReferenceContainer &messagesToCatchIn);

    /**
     * @brief Sets the event semaphore to fire when all the messages have been caught.
     * @details This assignment is performed by copying the low-level semaphore of \a eventSemIn.
     * @param[in] eventSemIn event semaphore to post when all the messages have been caught.
     */
    void SetEventSemaphore(EventSem &eventSemIn);

    /**
     * @brief Verifies if the \a messageToTest is one of messages to be caught (see SetMessagesToCatch).
     * @param[in] messageToTest The message to test.
     * @return ErrorManagement::NoError if the messageToTest was one of the messages to be caught, otherwise it returns ErrorManagement::UnsupportedFeature.
     * @pre
     *   SetEventSemaphore
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

private:

    /**
     * The message to catch.
     */
    ReferenceContainer messagesToCatch;

    /**
     * The semaphore to post when all the messages have been received.
     */
    EventSem *eventSem;

};

}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* QUEUEDMESSAGEREPLYCATCHERMESSAGEFILTER_H_ */
	
