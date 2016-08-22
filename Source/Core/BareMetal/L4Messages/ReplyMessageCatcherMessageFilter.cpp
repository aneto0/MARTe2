/**
 * @file ReplyMessageCatcherMessageFilter.cpp
 * @brief Source file for class ReplyMessageCatcherMessageFilter
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

 * @details This source file contains the definition of all the methods for
 * the class ReplyMessageCatcherMessageFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReplyMessageCatcherMessageFilter.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


ReplyMessageCatcherMessageFilter::ReplyMessageCatcherMessageFilter(){
    caught = false;
}



void ReplyMessageCatcherMessageFilter::SetMessageToCatch(const ReferenceT<Message> &message ):MessageFilter(false){
    originalMessage = message;
}

ReplyMessageCatcherMessageFilter::~ReplyMessageCatcherMessageFilter(){
}



ErrorManagement::ErrorType ReplyMessageCatcherMessageFilter::ProcessMessage(ReferenceT<Message> &messageToTest,MessageI *receiver){

    ErrorManagement::ErrorType ret(true);


    if (messageToTest == originalMessage){

        //check reply flag
        if (!messageToTest->IsReply()){
            // TODO produce warning!
        }

        HandleReplyMessage(messageToTest);

        ret = true;
    } else {
        ret.unsupportedFeature = true;
    }

    return ret;

}

ErrorManagement::ErrorType ReplyMessageCatcherMessageFilter::Wait(const TimeoutType &maxWait, const uint32 pollingTimeUsec){
    ErrorManagement::ErrorType  err(true);

    uint64 start = HighResolutionTimer::Counter();
    float32 pollingTime = (float)pollingTimeUsec * 1.0e-6;

    while(err.ErrorsCleared() && !caught ){
        Sleep::NoMore(pollingTime);
        if (maxWait != TTInfiniteWait){
            uint64 deltaT = HighResolutionTimer::Counter() - start;
            err.timeout = maxWait.HighResolutionTimerTicks() > deltaT;
        }
    }

    return err;
}



}

	
