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
#include "StateMachine.h"
#include "StateMachineEvent.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

StateMachineEvent::StateMachineEvent() :
        ReferenceContainer(),
        MessageFilter(true) {
    timeout = TTInfiniteWait;
    reset = true;
}

/*lint -e{1551} references are destroyed by design in the destructor*/
StateMachineEvent::~StateMachineEvent() {
    stateMachineIn = Reference();
}

TimeoutType StateMachineEvent::GetTransitionTimeout() const {
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
    uint32 msecTimeout = 0u;
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
        CCString function = messageToTest->GetFunction();
        if (function.GetList() != NULL_PTR(const char8 *)) {
            found = (StringHelper::Compare(function, GetName()) == 0);
        }

    }
    //Found.
    if (found) {
        if (reset) {
            reset = false;
            err = stateMachine->EventTriggered(this);
        }
    }
    else {
        err.recoverableError = true;
    }
    return err;
}

void StateMachineEvent::Reset() {
    reset = true;
}

CLASS_REGISTER(StateMachineEvent, "1.0")

}

