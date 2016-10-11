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

}

StateMachine::~StateMachine() {
    ErrorManagement::ErrorType err = Stop();
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::FatalError, "Could not Stop the StateMachine");
    }
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
                        ReferenceT<ReferenceContainer> nextState = Find(event->GetNextState());
                        ok = nextState.IsValid();
                        if (!ok) {
                            REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "In event (%s) the next state (%s) does not exist", event->GetName(), event->GetNextState().GetList())
                        }
                    }
                }
                if (ok) {
                    ok = found;
                    if (!ok) {
                        REPORT_ERROR_PARAMETERS(ErrorManagement::ParametersError, "In state (%s) no events were defined", state->GetName())
                    }
                }
            }
        }
    }
    //Install the event listeners for the first state
    ReferenceT<ReferenceContainer> nextStateRef;
    if (err.ErrorsCleared()) {
        nextStateRef = Get(0u);
        err.fatalError = !nextStateRef.IsValid();
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::FatalError, "No state at position zero was defined");
        }
    }
    if (err.ErrorsCleared()) {
        uint32 z;
        for (z = 0u; (z < nextStateRef->Size()) && (ok); z++) {
            ReferenceT<StateMachineEvent> nextStateEventJ = nextStateRef->Get(z);
            if (nextStateEventJ.IsValid()) {
                err = InstallMessageFilter(nextStateEventJ);
                ok = err.ErrorsCleared();
            }
        }
    }
    if (err.ErrorsCleared()) {
        err = Start();
    }
    return err;
}

CLASS_REGISTER(StateMachine, "1.0")
}

