/**
 * @file QueuedMessageI.h
 * @brief Header file for class QueuedMessageI
 * @date 22/04/2016
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

 * @details This header file contains the declaration of the class QueuedMessageI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef QUEUEDMESSAGEI_H_
#define QUEUEDMESSAGEI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageI.h"
#include "MutexSem.h"
#include "EventSem.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class represents a queue of messages
 * @details This class creates a thread for handling messages of the queue
 * without blocking the sender. The lifecycle of this thread is managed by
 * the class itself (created on construction and deleted on destruction)
 */
class DLL_API QueuedMessageI: public MessageI {

public:

    /**
     * @brief Default constructor
     */
    QueuedMessageI();

    /**
     * @brief Destructor
     */
    virtual ~QueuedMessageI();

protected:

    /**
     * @brief TODO
     * @details
     * + Default message handling mechanism
     * + Handles the reception of a message
     * + By default simply calls SortMessage
     * + Can be overridden to implement message Queues etc...
     * + in the case of a specialised method where queued message handling is implemented
     * + when the immediate return message is requested then the wait is performed here and a timeout+communication error may be produced here
     * */
    virtual ErrorManagement::ErrorType ReceiveMessage(ReferenceT<Message> &message);

private:

    /**
     * @brief TODO
     */
    void MessageProcessingThread();

    /*
     * A mutex semaphore to protect access to both queues
     */
    MutexSem queuesAccessControl;

    /*
     * A reference container where the messages are piled up
     */
    ReferenceContainer messageProcessQueue;

    /*
     * An event sempahore triggered whenever a new message is inserted
     */
    EventSem newMessageInQueue;

    /*
     * A reference container where the senders may wait for a reply
     */
    ReferenceContainer messageReplyWaitList;

    /*
     * An event semaphore triggered whenever a new reply is available
     */
    EventSem newReplyInWaitList;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

#endif /* QUEUEDMESSAGEI_H_ */

