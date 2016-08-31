/**
 * @file QueueingMessageFilter.cpp
 * @brief Source file for class QueueingMessageFilter
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

 * @details This source file contains the definition of all the methods for
 * the class QueueingMessageFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "QueueingMessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

QueueingMessageFilter::QueueingMessageFilter():MessageFilter(true){

}

/**
 * @brief Destructor.
 */
QueueingMessageFilter::~QueueingMessageFilter(){

}

ErrorManagement::ErrorType QueueingMessageFilter::ConsumeMessage(ReferenceT<Message> &messageToTest){
    ErrorManagement::ErrorType err;
    err.timeout = messageQ.Lock();
    if (err.ErrorsCleared()){
        err.fatalError = messageQ.Insert(messageToTest,-1);

        // handle the case of an element to an empty Q
        if (err.ErrorsCleared()  && messageQ.Size() == 1){
            newMessagesAlarm.Post();
        }

        messageQ.UnLock();
    }
    return err;
}

/**
 * TODO
*/
ErrorManagement::ErrorType QueueingMessageFilter::GetMessage(ReferenceT<Message> &message,const TimeoutType &timeout){
    ErrorManagement::ErrorType err;
    bool locked = false;
    err.timeout = !messageQ.Lock();
    locked = !err.timeout;

    // handle the empty Q case
    if (messageQ.Size() == 0){
        if (err.ErrorsCleared()){
            err.fatalError = newMessagesAlarm.Reset();
        }
        if (locked && err.ErrorsCleared()){
            messageQ.UnLock();
            locked = false;
        }
        if (err.ErrorsCleared()){
            err.timeout = newMessagesAlarm.Wait(timeout);
        }
        if (err.ErrorsCleared()){
            err.timeout = !messageQ.Lock();
            locked = !err.timeout;
        }
    }

    if (err.ErrorsCleared()){
        Reference ref = messageQ.Get(0);

        // TODO diversify error -- unexpectedError
        err.fatalError = !ref.IsValid();

        if (err.ErrorsCleared()){
            message = ref;
            // TODO diversify error -- failedCast
            err.fatalError = !message.IsValid();
        }
    }

    if (locked) {
         messageQ.UnLock();
   }
    return err;

}



}
