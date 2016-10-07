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
#if 0
ErrorManagement::ErrorType StateMachineState::ActOn(CCString instruction) {
    ReferenceT<StateMachineMessage> enterInstruction = Find(instruction);
    // Instruction exists?
    bool found = enterInstruction.IsValid();

    if (found) {
        //SENDSTATE
        GCRTemplate<Message> gcrtm = me->Find("SENDSTATE");
        if (gcrtm.IsValid()) {
            GCRTemplate < Message > gcrtm(GCFT_Create);
            if (!gcrtm.IsValid()) {
                AssertErrorCondition(FatalError, "ActOn: Failed creating message for SENDSTATE");
                return SMS_Error;
            }
            if (sm.currentState.IsValid()) {
                gcrtm->Init(sm.currentState->StateCode(), sm.currentState->Name());
            }
            else {
                AssertErrorCondition(FatalError, "ActOn: sm->currentState is not valid! using this state instead ");
                gcrtm->Init(this->StateCode(), this->Name());
            }

            if (sm.verboseLevel >= 10) {
                AssertErrorCondition(Information, "ActOn: sending state(%i,%s) to %s", gcrtm->GetMessageCode().Code(), gcrtm->Content(), me->Destination());
            }

            GCRTemplate < MessageEnvelope > mec(GCFT_Create);
            if (!mec.IsValid()) {
                AssertErrorCondition(FatalError, "ActOn: Failed creating copy for SENDSTATE");
                return SMS_Error;
            }
            mec->PrepareMessageEnvelope(gcrtm, me->Destination(), MDRF_None, &sm);
            // replace with copy
            me = mec;
        }
        else {
            // associate sender
            me->SetSender(sm);
        }

        MessageHandler::SendMessage (me);
        return SMS_Ok;
    }

    GCRTemplate < MessageDeliveryRequest > mdr = enterInstruction;
    if (mdr.IsValid()) {

        //SENDSTATE
        GCRTemplate<Message> gcrtm = mdr->GetMessage();
        if (gcrtm.IsValid()) {
            if (strcmp(gcrtm->Name(), "SENDSTATE") == 0) {
                GCRTemplate < Message > gcrtm(GCFT_Create);

                if (!gcrtm.IsValid()) {
                    AssertErrorCondition(FatalError, "Trigger: Failed creating mdr message for SENDSTATE");
                    return SMS_Error;
                }

                if (sm.currentState.IsValid()) {
                    gcrtm->Init(sm.currentState->StateCode(), sm.currentState->Name());
                }
                else {
                    AssertErrorCondition(FatalError, "ActOn: sm->currentState is not valid! using this state instead ");
                    gcrtm->Init(this->StateCode(), this->Name());
                }

                if (sm.verboseLevel >= 10) {
                    AssertErrorCondition(Information, "ActOn: sending state(%i,%s) to %s", gcrtm->GetMessageCode().Code(), gcrtm->Content(),
                            mdr->Destinations());
                }

                GCRTemplate < MessageDeliveryRequest > mdrc(GCFT_Create);
                if (!mdrc.IsValid()) {
                    AssertErrorCondition(FatalError, "Trigger: Failed creating mdr copy for SENDSTATE");
                    return SMS_Error;
                }
                mdrc->PrepareMDR(gcrtm, mdr->Destinations(), mdr->Flags(), &sm, mdr->MsecTimeout());
                // replace with copy
                mdr = mdrc;
            }
            else {
                // associate sender
                mdr->SetSender(sm);
            }
        }

        if (GMDSendMessageDeliveryRequest (mdr)) {
            return SMS_Ok;
        }
        else {
            return SMS_Error;
        }
    }

    AssertErrorCondition(FatalError, "ActOn: %s object is of type %s (not MessageEnvelope or MDR)", name, enterInstruction->ClassName());
    return SMS_Error;

}
#endif

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
        //Fire all the events
        uint32 i = 0u;
        ReferenceT<StateMachineMessage> event = Get(i);
        if (event.IsValid()) {
            if (event->ExpectsReply()) {
                err = MessageI::SendMessageAndWaitReply(event, stateMachine.operator ->(), event->GetTimeout());
            }
            else if (event->ExpectsIndirectReply()) {
                err = stateMachine->SendMessageAndWaitIndirectReply(event, event->GetTimeout());
            }
            else {
                err = MessageI::SendMessage(event, stateMachine.operator ->());
            }
        }

        return err;
    }

    CLASS_REGISTER(StateMachineState, "1.0")
}

