/**
 * @file StateMachineEvent.h
 * @brief Header file for class StateMachineEvent
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

 * @details This header file contains the declaration of the class StateMachineEvent
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATEMACHINEEVENT_H_
#define STATEMACHINEEVENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageFilter.h"
#include "ReferenceContainer.h"
#include "StateMachineEvent.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief The StateMachine event definition.
 * @details When this event is triggered (i.e. when a message whose Function is the Name() of this
 *  StateMachineEvent instance) the StateMachine will change to the state given by GetNextState()
 *  and all the Message instances that belong to this container will be triggered.
 */
class StateMachineEvent: public ReferenceContainer, public MessageFilter {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @post
     *   IsPermanentFilter() == false &&
     *   GetTransitionTimeout() == TTInfiniteWait
     */
    StateMachineEvent();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~StateMachineEvent();

    /**
     * @brief Gets the maximum timeout to wait for all the replies against a given event.
     * @return the maximum timeout to wait for all the replies against a given event.
     */
    TimeoutType GetTransitionTimeout() const;

    /**
     * @brief Gets the state to follow the trigger of this event.
     * @return the state to follow the trigger of this event.
     */
    CCString GetNextState();

    /**
     * @brief Gets the following state in case of an error when triggering this event.
     * @return the following state in case of an error when triggering this event.
     */
    CCString GetNextStateError();

    /**
     * @brief Sets the StateMachine.
     * @param[in] stateMachine the StateMachine to set.
     */
    void SetStateMachine(Reference stateMachine);

    /**
     * @brief Reads the event code and the maximum timeout to wait for replies.
     * @param[in] data shall contain a parameter named "NextState" with the name of the state to which the state machine shall move if
     * this event is triggered.
     * If a parameter named "Timeout" is defined it will be used to define the maximum time to wait for all the replies to arrive.
     * If "Timeout=0" => Timeout = TTInfiniteWait.
     * If a parameter named "NextStateError" exists, it will be used to define the name of the state to where the state machine
     *  will move in case of an error when moving away from this event.
     * @return true if the "Code" and "NextState" parameters are available.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Checks if the messageToTest->GetFunction() == GetName() and if so requests a state change to the state machine.
     * @param[in] messageToTest the message with the target event name encoded in the messageToTest->GetFunction().
     * @return ErrorManagement::NoError if messageToTest->GetFunction() == GetName() and the event can be successfully triggered in the state machine.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

    /**
     * @brief Allow to send the message after a new state transition is concluded.
     */
    void Reset();

private:

    /**
     * Reference to the state-machine which holds this event
     */
    Reference stateMachineIn;

    /**
     * Maximum timeout to wait for a reply from all the triggered actions which require a reply.
     */
    TimeoutType timeout;

    /**
     * The name of the next state
     */
    StreamString nextState;

    /**
     * The name of the state to go in case of error
     */
    StreamString nextStateError;

    /**
     * Only Consume the message once per state transistion.
     */
    bool reset;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATEMACHINEEVENT_H_ */

