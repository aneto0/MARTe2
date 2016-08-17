/**
 * @file QueuedMessageI.h
 * @brief Header file for class QueuedMessageI
 * @date Apr 22, 2016
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
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * TODO
 * */
class QueuedMessageI: public MessageI{
public:

    /**
       * TODO
       * the main job is to add messages to the message Q
       * first checks for any message filter. If message matches then adds message to filter, removes and trigger the filter
       * if no filter adds to main Q
      * */
    virtual ErrorManagement::ErrorType ReceiveMessage(ReferenceT<Message> &message);

    /**
     * TODO
     * called by the main thread to sort messages taken from Q
     *
     * */
    virtual ErrorManagement::ErrorType SortMessage(ReferenceT<Message> &message);


    /**
     *     sets all up and starts the message handler thread
     */
    QueuedMessageI();

    /**
     * TODO
     *     kills the message handler thread
     */
   virtual ~QueuedMessageI();

private:

   void MessageProcessingThread();

   /*
    * to protect access to both queues
    */
   MutexSem           queuesAccessControl;

   /*
    * This is where the messages are piled up
    */
   ReferenceContainer messageProcessQueue;

   /*
    * triggered whenever a new message is inserted
    */
   EventSem           newMessageInQueue;

   /*
    * This is where the senders may wait for a reply
    */
   ReferenceContainer messageReplyWaitList;

   /*
    * triggered whenever a new reply is available
    */
   EventSem           newReplyInWaitList;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* QUEUEDMESSAGEI_H_ */
	
