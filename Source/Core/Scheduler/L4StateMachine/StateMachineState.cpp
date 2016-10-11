/**
 * @file StateMachineState.cpp
 * @brief Source file for class StateMachineState
 * @date 30/09/2016
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

 * @details This source file contains the definition of all the methods for
 * the class StateMachineState (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "QueuedReplyMessageCatcherFilter.h"
#include "StateMachine.h"
#include "StateMachineEvent.h"
#include "StateMachineMessage.h"
#include "StateMachineState.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ErrorManagement::ErrorType StateMachineState::ConsumeMessage(ReferenceT<Message> &messageToTest) {

    ReferenceT<StateMachineMessage> msg = messageToTest;
    ReferenceT<StateMachine> stateMachine = stateMachineIn;

    ErrorManagement::ErrorType err;
    err.fatalError = !msg.IsValid();
    bool found = false;
    if (err.ErrorsCleared()) {
        found = (msg->GetCode() == GetCode());
        if (!found) {
            if (msg->GetContent() != NULL_PTR(CCString)) {
                found = (StringHelper::Compare(msg->GetContent(), GetName()) == 0u);
            }
        }
    }
    if (found) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Changing from state (%s) with code (%d)", GetName(), GetCode())
        //Prepare to wait for eventual replies
        ReferenceContainer eventReplyContainer;
        uint32 i;

        for (i = 0u; i < Size(); i++) {
            ReferenceT<StateMachineMessage> event = Get(i);
            if (event.IsValid()) {
                if (event->ExpectsReply()) {
                    event->SetExpectsIndirectReply(true);
                }
                if (event->ExpectsIndirectReply()) {
                    eventReplyContainer.Insert(event);
                }
            }
        }
        EventSem waitSem;
        if (eventReplyContainer.Size() > 0u) {
            ReferenceT<QueuedReplyMessageCatcherFilter> filter(new (NULL) QueuedReplyMessageCatcherFilter());
            waitSem.Create();
            waitSem.Reset();
            filter->SetMessagesToCatch(eventReplyContainer);
            filter->SetEventSemaphore(waitSem);
            stateMachine->MessageI::InstallMessageFilter(filter, 0);
        }

        bool ok = err.ErrorsCleared();
        for (i = 0u; (i < Size()) && (ok); i++) {
            ReferenceT<StateMachineMessage> stateMachineMsg = Get(i);
            if (stateMachineMsg.IsValid()) {
                err = MessageI::SendMessage(stateMachineMsg, stateMachine.operator ->());
                REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "While changing from state (%s) triggered message (%s)", GetName(), stateMachineMsg->GetName())
            }
            ok = err.ErrorsCleared();
        }
        //Wait for all the replies to arrive...
        if(ok){
            if (eventReplyContainer.Size() > 0u) {
                err = waitSem.Wait(timeout);
            }
        }
    }
    //Install the next state filter...


    return err;
}

CLASS_REGISTER(StateMachineState, "1.0")
}

