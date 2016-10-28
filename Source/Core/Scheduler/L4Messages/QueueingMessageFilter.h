/**
 * @file QueueingMessageFilter.h
 * @brief Header file for class QueueingMessageFilter
 * @date 22/08/2016
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

 * @details This header file contains the declaration of the class MessageI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef QUEUEINGMESSAGEFILTER_H_
#define QUEUEINGMESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageFilter.h"
#include "EventSem.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Adds message to a queue.
 * @details Messages consumed by this filter are added to a queue. The queue is consumed by calling the GetMessage method.
 */
class DLL_API QueueingMessageFilter: public MessageFilter, public Object {
public:

    /**
     * @brief Constructor. Initialises the semaphores.
     */
    QueueingMessageFilter();

    /**
     * @brief Destructor.
     */
    virtual ~QueueingMessageFilter();

    /**
     * @brief Adds the message to the message queue.
     * @param[in] messageToTest The message to add to the queue.
     * @return ErrorManagement::NoError if the message can be successfully added to the queue.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

    /**
     * @brief Gets the oldest message from the queue or waits for a message to be available.
     * @param[out] message The oldest message available on the queue.
     * @param[out] timeout The maximum time to wait for a message to be available on the queue.
     * @return ErrorManagement::NoError if the message can be successfully retrieved from the queue with-in the specified timeout.
     */
    ErrorManagement::ErrorType GetMessage(ReferenceT<Message> &message, const TimeoutType &timeout = TTInfiniteWait);

private:

    /**
     * Holds the messages consumed by this QueueingMessageFilter
     */
    ReferenceContainer messageQ;

    /**
     * Locks the adding/removing of messages to the queue
     */
    FastPollingMutexSem mutexSemQ;

    /**
     * Wakes threads waiting on the queue
     */
    EventSem newMessagesAlarm;


};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}


#endif /* QUEUEINGMESSAGEFILTER_H_ */

