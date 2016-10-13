/**
 * @file StateMachineEvent.cpp
 * @brief Source file for class StateMachineEvent
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
 * the class StateMachineEvent (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

StateMachineEvent::StateMachineEvent() :
        ReferenceContainer(),
        MessageFilter(false) {
    timeout = TTInfiniteWait;
}

StateMachineEvent::~StateMachineEvent() {

}


TimeoutType StateMachineEvent::GetTimeout() const {
    return timeout;
}

CCString StateMachineEvent::GetNextState() {
    return nextState.Buffer();
}

CCString StateMachineEvent::GetNextStateError() {
    return nextStateError.Buffer();
}

void StateMachineEvent::SetStateMachine(Reference stateMachine) {
    stateMachineIn = stateMachine;
}

bool StateMachineEvent::Initialise(StructuredDataI &data) {
    ErrorManagement::ErrorType err;
    err.parametersError = !ReferenceContainer::Initialise(data);
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("NextState", nextState);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NextState was not specified");
        }
    }
    if (!data.Read("NextStateError", nextStateError)) {
        REPORT_ERROR(ErrorManagement::Warning, "NextStateError was not specified. Using ERROR.");
        nextStateError = "ERROR";
    }
    uint32 msecTimeout;
    if (err.ErrorsCleared()) {
        if (!data.Read("Timeout", msecTimeout)) {
            REPORT_ERROR(ErrorManagement::Warning, "Timeout was not specified. Using TTInfiniteWait");
            msecTimeout = 0u;
        }
    }
    if (err.ErrorsCleared()) {
        if (msecTimeout == 0u) {
            timeout = TTInfiniteWait.GetTimeoutMSec();
        }
        else {
            timeout = msecTimeout;
        }
    }

    return err.ErrorsCleared();
}

ErrorManagement::ErrorType StateMachineEvent::ConsumeMessage(ReferenceT<Message> &messageToTest) {

    ReferenceT<StateMachine> stateMachine = stateMachineIn;

    ErrorManagement::ErrorType err;
    err.fatalError = !messageToTest.IsValid();

    bool found = false;
    //Check if the destination of this message is this event
    if (err.ErrorsCleared()) {
        /*found = (msg->GetCode() == GetCode());
         if (!found) {
         if (msg->GetContent() != NULL_PTR(CCString)) {
         found = (StringHelper::Compare(msg->GetContent(), GetName()) == 0u);
         }
         }*/
        if (messageToTest->GetFunction() != NULL_PTR(CCString)) {
            found = (StringHelper::Compare(messageToTest->GetFunction(), GetName()) == 0u);
        }

    }
    //Found.
    if (found) {
        REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "Changing from state (%s) to state (%s)", GetName(), nextState)
        err = SendMultipleMessagesAndWaitReply(*this);
        //Install the next state event filters...
        ReferenceT<ReferenceContainer> nextStateRef;
        if (err.ErrorsCleared()) {
            if (nextState.Size() > 0u) {
                nextStateRef = stateMachine->Find(nextState.Buffer());
                err.fatalError = !nextStateRef.IsValid();
            }
            else {
                REPORT_ERROR_PARAMETERS(err, "In state (%s) the NextState is not defined", GetName())
            }
        }
        else {
            REPORT_ERROR_PARAMETERS(err, "In state (%s) could not send all the event messages. Moving to error state (%s)", GetName(), nextStateError)
            if (nextStateError.Size() > 0u) {
                nextStateRef = stateMachine->Find(nextStateError.Buffer());
                err.fatalError = !nextStateRef.IsValid();
            }
            else {
                REPORT_ERROR_PARAMETERS(err, "In state (%s) the NextStateError is not defined", GetName())
            }
            if (err.ErrorsCleared()) {
                //Clear errors to allow to install the ERROR
                err = false;
            }
        }
        if (err.ErrorsCleared()) {
            uint32 j;
            bool ok = true;
            for (j = 0u; (j < nextStateRef->Size()) && (ok); j++) {
                ReferenceT<StateMachineEvent> nextStateEventJ = nextStateRef->Get(j);
                if (nextStateEventJ.IsValid()) {
                    err = stateMachine->InstallMessageFilter(nextStateEventJ);
                    ok = err.ErrorsCleared();
                }
            }
        }
        else {
            REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "In state (%s) the next state is not valid", GetName())
        }
        //Check if the next state there are messages to be fired at ENTER
        if (err.ErrorsCleared()) {
            ReferenceT<ReferenceContainer> enterMessages = nextStateRef->Find("ENTER");
            if (enterMessages.IsValid()) {
                //Check if it is a single message
                ReferenceT<ReferenceContainer> enterMessage = enterMessages;
                if (enterMessage.IsValid()) {
                    ReferenceContainer messagesToSend;
                    messagesToSend.Insert(enterMessage);
                    err = SendMultipleMessagesAndWaitReply(messagesToSend);
                }
                else {
                    err = SendMultipleMessagesAndWaitReply(*(enterMessages.operator ->()));
                }
            }
        }

    }
    return err;
}

ErrorManagement::ErrorType StateMachineEvent::SendMultipleMessagesAndWaitReply(ReferenceContainer messagesToSend) {

    ErrorManagement::ErrorType err;
    ReferenceT<StateMachine> stateMachine = stateMachineIn;

    //Semaphore to wait for replies from events which require a reply
    EventSem waitSem;
    bool ok = waitSem.Create();
    if (ok) {
        ok = waitSem.Reset();
    }

    //Prepare to wait for eventual replies
    ReferenceContainer eventReplyContainer;
    uint32 i;

    //Only accept indirect replies
    for (i = 0u; i < Size(); i++) {
        ReferenceT<Message> eventMsg = messagesToSend.Get(i);
        if (eventMsg.IsValid()) {
            if (eventMsg->ExpectsReply()) {
                eventMsg->SetExpectsIndirectReply(true);
            }
            if (eventMsg->ExpectsIndirectReply()) {
                eventReplyContainer.Insert(eventMsg);
            }
        }
    }

    //Prepare the filter which will wait for all the replies
    if (eventReplyContainer.Size() > 0u) {
        ReferenceT<QueuedReplyMessageCatcherFilter> filter(new (NULL) QueuedReplyMessageCatcherFilter());
        filter->SetMessagesToCatch(eventReplyContainer);
        filter->SetEventSemaphore(waitSem);
        stateMachine->MessageI::InstallMessageFilter(filter, 0);
    }

    ok = err.ErrorsCleared();
    for (i = 0u; (i < Size()) && (ok); i++) {
        ReferenceT<Message> stateMachineMsg = messagesToSend.Get(i);
        if (stateMachineMsg.IsValid()) {
            err = MessageI::SendMessage(stateMachineMsg, stateMachine.operator ->());
            REPORT_ERROR_PARAMETERS(ErrorManagement::Information, "While changing from state (%s) triggered message (%s)", GetName(),
                                    stateMachineMsg->GetName())
        }
        ok = err.ErrorsCleared();
    }
    //Wait for all the replies to arrive...
    if (ok) {
        if (eventReplyContainer.Size() > 0u) {
            err = waitSem.Wait(timeout);
        }
    }

    return err;
}

CLASS_REGISTER(StateMachineEvent, "1.0")

}

