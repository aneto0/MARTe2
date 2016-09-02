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
#include "ReferenceContainer.h"
#include "EmbeddedThread.h"
#include "QueueingMessageFilter.h"
//#include "EmbeddedThreadMethodCaller.h"

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
     *     sets all up and starts the message handler thread
     */
    QueuedMessageI();

    /**
     * TODO
     *     kills the message handler thread
     */
   virtual ~QueuedMessageI();

   /**
    * TODO
    *     installs message queue
    *     starts the thread
    */
   ErrorManagement::ErrorType Start();
   /**
    * TODO
    *     installs message queue
    *     starts the thread
    */
   ErrorManagement::ErrorType Stop();

   /**
    * @brief installs a message filter in a given position
    * TODO
    */
   ErrorManagement::ErrorType InstallMessageFilter(ReferenceT<MessageFilter> messageFilter,CCString name="",int32 position=0,bool afterQueue=true);

   /**
    * TODO
    */
   ErrorManagement::ErrorType RemoveMessageFilter(ReferenceT<MessageFilter> messageFilter);

   /**
    * TODO
    */
   ErrorManagement::ErrorType RemoveMessageFilter(CCString name);


private:

   /**
    * TODO
    * */
   ErrorManagement::ErrorType QueueProcessing(EmbeddedServiceI::ExecutionInfo info);

   /**
    * TODO
    * The message consuming filters
    * used by SortMessage
    *
    * */
   MessageFilterPool queuedMessageFilters;

   /**
    * TODO
    * */
   ReferenceT<QueueingMessageFilter> queue;

   /**
    * TODO
    * The message consuming filters
    * used by SortMessage
    *
    * */
   EmbeddedThread    queueProcessingThread;

   /**
    * TODO
    */
   EmbeddedServiceI::MethodBinderT<QueuedMessageI> binder;
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* QUEUEDMESSAGEI_H_ */
	
