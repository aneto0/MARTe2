/**
 * @file StateMachine.cpp
 * @brief Source file for class StateMachine
 * @date 11/10/2016
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
 * the class StateMachine (public, protected, and private). Be aware that some 
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

StateMachine::StateMachine() :
        ReferenceContainer(),
        QueuedMessageI() {
    currentStateStatus = Entering;
}

/*lint -e{1551} the destructor must guarantee that the QueuedMessageI SingleThreadService is stopped.*/
StateMachine::~StateMachine() {
    ReferenceContainer c;
    StateMachine::Purge(c);
}

void StateMachine::Purge(ReferenceContainer &purgeList) {
    ErrorManagement::ErrorType err = Stop();
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not Stop the StateMachine. Retrying...");
        err = Stop();
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Could not Stop the StateMachine.");
        }
    }
    bool ok = true;
    if (currentState.IsValid()) {
        uint32 j;
        for (j = 0u; (j < currentState->Size()) && (ok); j++) {
            ReferenceT<StateMachineEvent> currentStateEventJ = currentState->Get(j);
            if (currentStateEventJ.IsValid()) {
                ok = (RemoveMessageFilter(currentStateEventJ) == ErrorManagement::NoError);
            }
        }
    }
    uint32 i;
    for (i = 0u; (i < Size()) && (ok); i++) {
        ReferenceT<ReferenceContainer> state = Get(i);
        if (state.IsValid()) {
            uint32 j;
            for (j = 0u; (j < state->Size()) && (ok); j++) {
                ReferenceT<StateMachineEvent> event = state->Get(j);
                if (event.IsValid()) {
                    event->SetStateMachine(Reference());
                }
            }
        }
    }
    PurgeFilters();
    ReferenceContainer::Purge(purgeList);
}

bool StateMachine::ExportData(StructuredDataI & data) {
    bool ok = ReferenceContainer::ExportData(data);
    if (ok) {
        StreamString currentStateName = "";
        if (currentState.IsValid()) {
            currentStateName = currentState->GetName();
        }
        ok = data.Write("CurrentState", currentStateName.Buffer());
    }
    return ok;
}


bool StateMachine::Initialise(StructuredDataI &data) {
    ErrorManagement::ErrorType err;
    err.parametersError = !ReferenceContainer::Initialise(data);
    bool ok = true;
    if (err.ErrorsCleared()) {
        //Loop on all the states
        uint32 i;
        for (i = 0u; (i < Size()) && (ok); i++) {
            ReferenceT<ReferenceContainer> state = Get(i);
            if (state.IsValid()) {
                uint32 j;
                bool found = false;
                for (j = 0u; (j < state->Size()) && (ok); j++) {
                    ReferenceT<StateMachineEvent> event = state->Get(j);
                    if (event.IsValid()) {
                        event->SetStateMachine(this);
                        found = true;
                        //Check if the NextState exists
                        CCString nextStateStr = event->GetNextState();
                        ReferenceT<ReferenceContainer> nextState = Find(nextStateStr);
                        ok = nextState.IsValid();
                        if (!ok) {
                            err.parametersError = true;
                            REPORT_ERROR(ErrorManagement::ParametersError, "In event (%s) the next state (%s) does not exist", event->GetName(),
                                                    nextStateStr.GetList());
                        }
                        //Check if the NextStateError exists
                        if (ok) {
                            CCString nextStateErrorStr = event->GetNextStateError();
                            ReferenceT<ReferenceContainer> nextStateError = Find(nextStateErrorStr);
                            ok = nextStateError.IsValid();
                            if (!ok) {
                                err.parametersError = true;
                                REPORT_ERROR(ErrorManagement::ParametersError, "In event (%s) the next state error (%s) does not exist",
                                                        event->GetName(), nextStateErrorStr.GetList());
                            }
                        }
                    }
                }
                if (ok) {
                    ok = found;
                    if (!ok) {
                        err.parametersError = true;
                        REPORT_ERROR(ErrorManagement::ParametersError, "In state (%s) no events were defined", state->GetName());
                    }
                }
            }
        }
    }
    //Install the event listeners for the first state
    if (err.ErrorsCleared()) {
        currentState = Get(0u);
        err.fatalError = !currentState.IsValid();
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "No state at position zero was defined");
        }
    }
    if (err.ErrorsCleared()) {
        uint32 z;
        for (z = 0u; (z < currentState->Size()) && (ok); z++) {
            ReferenceT<StateMachineEvent> nextStateEventJ = currentState->Get(z);
            if (nextStateEventJ.IsValid()) {
                err = InstallMessageFilter(nextStateEventJ);
                ok = err.ErrorsCleared();
            }
        }
    }
    if (err.ErrorsCleared()) {
        QueuedMessageI::SetQueueName(GetName());
        err = Start();
    }
    if (err.ErrorsCleared()) {
        currentStateStatus = Executing;
    }
    return err;
}

ErrorManagement::ErrorType StateMachine::EventTriggered(ReferenceT<StateMachineEvent> event) {
    ErrorManagement::ErrorType err;
    ErrorManagement::ErrorType errSend = false;
    StreamString nextStateError;
    StreamString nextState;

    currentStateStatus = Exiting;
    err.fatalError = !event.IsValid();
    if (err.ErrorsCleared()) {
        err.fatalError = !currentState.IsValid();
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "The current state is not valid!");
        }
    }

    if (err.ErrorsCleared()) {
        nextStateError = event->GetNextStateError();
        nextState = event->GetNextState();
        REPORT_ERROR(ErrorManagement::Information, "Changing from state (%s) to state (%s)", currentState->GetName(), nextState.Buffer());
        errSend = SendMultipleMessagesAndWaitReply(*(event.operator ->()), event->GetTransitionTimeout());
    }

    StreamString currentStateName=currentState->GetName();
    if (err.ErrorsCleared()) {
        if (nextState != currentStateName) {
            //Remove all the filters related to the previous event (except this one which will be removed by the MessageFilter).
            uint32 j;
            bool ok = true;
            for (j = 0u; (j < currentState->Size()) && (ok); j++) {
                ReferenceT<StateMachineEvent> currentStateEventJ = currentState->Get(j);
                if (currentStateEventJ.IsValid()) {
                    if (currentStateEventJ != event) {
                        err = RemoveMessageFilter(currentStateEventJ);
                        ok = err.ErrorsCleared();
                    }
                }
            }
        }
    }
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Error removing StateMachineEvent filters");
    }

    //Install the next state event filters...
    if (errSend.ErrorsCleared()) {
        if (nextState.Size() > 0u) {
            currentState = Find(nextState.Buffer());
            currentStateStatus = Entering;
            err.fatalError = !currentState.IsValid();
        }
        else {
            REPORT_ERROR(err, "In state (%s) the NextState is not defined", currentState->GetName());
        }
    }
    else {
        REPORT_ERROR(err, "In state (%s) could not send all the event messages. Moving to error state (%s)", currentState->GetName(),
                                nextStateError.Buffer());
        if (nextStateError.Size() > 0u) {
            currentState = Find(nextStateError.Buffer());
            err.fatalError = !currentState.IsValid();
        }
        else {
            REPORT_ERROR(err, "In state (%s) the NextStateError is not defined", currentState->GetName());
        }
    }
    if (err.ErrorsCleared()) {
        uint32 j;
        bool ok = true;
        for (j = 0u; (j < currentState->Size()) && (ok); j++) {
            ReferenceT<StateMachineEvent> nextStateEventJ = currentState->Get(j);
            if (nextStateEventJ.IsValid()) {
                nextStateEventJ->Reset();
                if (nextState != currentStateName) {
                    err = InstallMessageFilter(nextStateEventJ);
                    ok = err.ErrorsCleared();
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "In state (%s) the next state is not valid", currentState->GetName());
    }
    //Check if the next state there are messages to be fired at ENTER.
    if (err.ErrorsCleared()) {
        ReferenceT<ReferenceContainer> enterMessages = currentState->Find("ENTER");
        //Compute the highest timeout
        if (enterMessages.IsValid()) {
            uint32 msecTimeout = 0u;
            uint32 n;
            bool maxTimeoutFound = false;
            for (n = 0u; (n < enterMessages->Size()) && (!maxTimeoutFound); n++) {
                ReferenceT<Message> enterMessage = enterMessages->Get(n);
                if (enterMessage.IsValid()) {
                    if (enterMessage->GetReplyTimeout() == TTInfiniteWait) {
                        maxTimeoutFound = true;
                        msecTimeout = TTInfiniteWait.GetTimeoutMSec();
                    }
                    else {
                        if (enterMessage->GetReplyTimeout().GetTimeoutMSec() > msecTimeout) {
                            msecTimeout = enterMessage->GetReplyTimeout().GetTimeoutMSec();
                        }
                    }
                }
            }
            err = SendMultipleMessagesAndWaitReply(*(enterMessages.operator ->()), msecTimeout);
        }
    }
    if (err.ErrorsCleared()) {
        currentStateStatus = Executing;
    }
    return err;
}

ErrorManagement::ErrorType StateMachine::SendMultipleMessagesAndWaitReply(ReferenceContainer messagesToSend,
                                                                          const TimeoutType &timeout) {

    ErrorManagement::ErrorType err;
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
    for (i = 0u; i < messagesToSend.Size(); i++) {
        ReferenceT<Message> eventMsg = messagesToSend.Get(i);
        if (eventMsg.IsValid()) {
            if (eventMsg->ExpectsReply()) {
                eventMsg->SetExpectsIndirectReply(true);
            }
            if (eventMsg->ExpectsIndirectReply()) {
                err = !eventReplyContainer.Insert(eventMsg);
            }
        }
    }

    if (ok) {
        ok = err.ErrorsCleared();
    }
    //Prepare the filter which will wait for all the replies
    if ((eventReplyContainer.Size() > 0u) && (ok)) {
        ReferenceT<QueuedReplyMessageCatcherFilter> filter(new (NULL) QueuedReplyMessageCatcherFilter());
        filter->SetMessagesToCatch(eventReplyContainer);
        filter->SetEventSemaphore(waitSem);
        err = MessageI::InstallMessageFilter(filter, 0);
    }

    ok = err.ErrorsCleared();
    for (i = 0u; (i < messagesToSend.Size()) && (ok); i++) {
        ReferenceT<Message> eventMsg = messagesToSend.Get(i);
        if (eventMsg.IsValid()) {
            eventMsg->SetAsReply(false);
            REPORT_ERROR(ErrorManagement::Information, "In state (%s) triggered message (%s)", currentState->GetName(), eventMsg->GetName());
            err = MessageI::SendMessage(eventMsg, this);
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

Reference StateMachine::GetCurrentState() const {
    return currentState;
}

StateMachine::StateStatus StateMachine::GetCurrentStateStatus() const {
    return currentStateStatus;
}

CLASS_REGISTER(StateMachine, "1.0")
}

