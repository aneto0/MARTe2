/**
 * @file QueuedMessageI.cpp
 * @brief Source file for class QueuedMessageI
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

 * @details This source file contains the definition of all the methods for
 * the class QueuedMessageI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

//#include <old/GenericVoidMethodCallerT.h>
#include "QueuedMessageI.h"
#include "ErrorType.h"
//#include "EmbeddedThreadMethodCaller.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {



/**
 * TODO
 *     kills the message handler thread
 */
QueuedMessageI::~QueuedMessageI(){

}



/**
 *     sets all up and starts the message handler thread
 */
QueuedMessageI::QueuedMessageI():
        queue(GlobalObjectsDatabase::Instance()->GetStandardHeap()),
        queueProcessingThread(binder),
        binder(*this, &QueuedMessageI::QueueProcessing)
{
//        queueProcessingThread(EmbeddedServiceI::MethodBinderT<QueuedMessageI> (*this, &QueuedMessageI::QueueProcessing)){
    ErrorManagement::ErrorType err;

    err.fatalError = !queue.IsValid();

    if (err.ErrorsCleared()){
        // installs as last in the filter queue
        err = MessageI::InstallMessageFilter(queue,"QUEUE",-1);
    }
}



ErrorManagement::ErrorType QueuedMessageI::Start(){
    ErrorManagement::ErrorType err;

    err.fatalError = !queue.IsValid();

    if (err.ErrorsCleared()){

        err = queueProcessingThread.Start();

    }

    return err;

}

ErrorManagement::ErrorType QueuedMessageI::Stop(){
    ErrorManagement::ErrorType err;

    err = queueProcessingThread.Stop();

    if (err.timeout){

        err = queueProcessingThread.Stop();

    }

    return err;

}

ErrorManagement::ErrorType QueuedMessageI::QueueProcessing(EmbeddedServiceI::ExecutionInfo info){
    ErrorManagement::ErrorType err;
    ReferenceT<Message> message;
    const TimeoutType timeout = 1000;

    err.unsupportedFeature = !queue.IsValid();

    if (err.ErrorsCleared()){

        err = queue->GetMessage(message,timeout);

        if (err.ErrorsCleared()){
            err = queuedMessageFilters.ReceiveMessage(message);
        }
    }

    // not a reason to abort this thread
    err.timeout = false;

    return err;

}


ErrorManagement::ErrorType QueuedMessageI::InstallMessageFilter(ReferenceT<MessageFilter> messageFilter,CCString name,int32 position,bool afterQueue){
    ErrorManagement::ErrorType err;
    if (afterQueue) {
        err = queuedMessageFilters.InstallMessageFilter(messageFilter,name,position);
    } else {
        err = messageFilters.InstallMessageFilter(messageFilter,name,position);
    }

    return err;
}


ErrorManagement::ErrorType QueuedMessageI::RemoveMessageFilter(ReferenceT<MessageFilter> messageFilter){
    ErrorManagement::ErrorType err;

    err = messageFilters.RemoveMessageFilter(messageFilter);

    if (!err.ErrorsCleared()){
        err = queuedMessageFilters.RemoveMessageFilter(messageFilter);
    }
    return err;
}

ErrorManagement::ErrorType QueuedMessageI::RemoveMessageFilter(CCString name){
    ErrorManagement::ErrorType err;

    err = messageFilters.RemoveMessageFilter(name);

    if (!err.ErrorsCleared()){
        err = queuedMessageFilters.RemoveMessageFilter(name);
    }
    return err;
}



}

	
