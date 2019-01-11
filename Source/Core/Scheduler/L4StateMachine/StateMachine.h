/**
 * @file StateMachine.h
 * @brief Header file for class StateMachine
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

 * @details This header file contains the declaration of the class StateMachine
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "QueuedMessageI.h"
#include "StateMachineEvent.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Implementation of a complete state machine.
 * @details A StateMachine contains one or more states (which are ReferenceContainre instances).
 * A state contains one or more StateMachineEvent instances.
 * A StateMachineEvent instance contains one or Message instances to be triggered when the event is matched.
 * The StateMachine automatically moves to the first state defined. To change state a message with the event
 * name, which contains the next state, must be received.
 *
 * On each state, if a ReferenceContainer named ENTER exists, then all the messages belonging to this container
 *  will be sent upon entering this state.

 *
 * The configuration syntax is (object names are only given as an example):
 * +StateMachine = {
 *     Class = StateMachine
 *     +State1 = {
 *        Class = ReferenceContainer
 *        +Event1 = {
 *            Class = StateMachineEvent
 *            NextState = State2
 *            +Message1 = {
 *               Class = Message
 *               Destination = SomeObject
 *            }
 *        }
 *        +SameState = {
 *            Class = StateMachineEvent
 *            NextState = State1
 *        }
 *     }
 *     +State2 = {
 *        Class = ReferenceContainer
 *        +ENTER = {
 *            Class = ReferenceContainer
 *            +Message1 = {
 *               Class = Message
 *               Destination = SomeObject
 *            }
 *        }
 *        +Event1 = {
 *            Class = StateMachineEvent
 *            NextState = State1
 *        }
 *     }
 *     +ERROR = {
 *        Class = ReferenceContainer
 *        +Event1 = {
 *            Class = StateMachineEvent
 *            NextState = State1
 *        }
 *     }
 * }
 */
class StateMachine: public ReferenceContainer, public QueuedMessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor. NOOP.
     */
StateMachine    ();

    /**
     * @brief Destructor. Calls QueuedMessageI::Stop().
     */
    virtual ~StateMachine();

    /**
     * The list of status where
     */
    enum StateStatus {
        /**
         * Exiting from a given state
         */
        Exiting,

        /**
         * Executing a given state
         */
        Executing,

        /**
         * Entering a given state
         */
        Entering
    };

    /**
     * @brief Initialises the StateMachine (see ReferenceContainer::Initialise)
     * @details Verifies that at least one state shall exist.
     * For every state at least one event shall be defined.
     * For every event in every state the NextState shall be defined and shall exists.
     *  For every event in every state the NextStateError shall exist.
     * Upon successful initialisation it calls QueuedMessageI::Starts and registers the
     * MessageFilter for all the events of the first state.
     * @param[in] data configuration in the form:
     * * +StateMachine = {
     *     Class = StateMachine
     *     +State1 = {
     *        Class = ReferenceContainer
     *        +Event1 = {
     *            Class = StateMachineEvent
     *            NextState = State2
     *            +Message1 = {
     *               Class = Message
     *               Destination = SomeObject
     *            }
     *        }
     *        +SameState = {
     *            Class = StateMachineEvent
     *            NextState = State1
     *        }
     *     }
     *     +State2 = {
     *        Class = ReferenceContainer
     *        +ENTER = {
     *            Class = ReferenceContainer
     *            +Message1 = {
     *               Class = Message
     *               Destination = SomeObject
     *            }
     *        }
     *        +Event1 = {
     *            Class = StateMachineEvent
     *            NextState = State1
     *        }
     *     }
     *     +ERROR = {
     *        Class = ReferenceContainer
     *        +Event1 = {
     *            Class = StateMachineEvent
     *            NextState = State1
     *        }
     *     }
     * }
     * @return true if ReferenceContainer::Initialise(data) returns true and
     *  if the state rules defined above are valid.
     */
    virtual bool Initialise(StructuredDataI &data);


    /**
     * @brief Calls ReferenceContainer::Purge after removing all the StateMachineEvents from the queue
     * @param[in] purgeList (see Object::Purge)
     */
    virtual void Purge(ReferenceContainer &purgeList);

    /**
     * @brief See ReferenceContainer::ExportData
     * @details Exports as ReferenceContainer::ExportData together with the current state.
     * @param[in] data see ReferenceContainer::ExportData.
     * @return see ReferenceContainer::ExportData
     */
    virtual bool ExportData(StructuredDataI & data);

    /**
     * @brief To be called by a StateMachineEvent which has received a relevant event and wishes to change state.
     * @details The current state will be updated to the StateMachineEvent::GetNextState. All the messages associated
     * to the event will be triggered and the state machine will wait for all the relevant replies.
     * If there is an ENTER reference container on the next state all the messages belonging to this container
     *  will also be fired.
     * @param[in] event the state machine event that has received the relevant event.
     * @return ErrorManagement::NoError if the event can be successfully processed and the state machine state changed.
     * ErrorManagement::Timeout if a timeout occurs while sending the message.
     */
    ErrorManagement::ErrorType EventTriggered(ReferenceT<StateMachineEvent> event);

    /**
     * @brief Gets the current state.
     * @return the current State.
     */
    Reference GetCurrentState() const;

    /**
     * @brief Gets the current state status.
     * @return the current state status.
     */
    StateStatus GetCurrentStateStatus() const;

private:
    /**
     * @brief Sends multiple messages and waits for all the replies to arrive.
     * @param[in] messagesToSend container with messages to send.
     * @return ErrorManagement::NoError if all the messages can be successfully send and all the replies are received before timeout.
     */
    virtual ErrorManagement::ErrorType SendMultipleMessagesAndWaitReply(ReferenceContainer messagesToSend, const TimeoutType &timeout);

    /**
     * The state machine current state.
     */
    ReferenceT<ReferenceContainer> currentState;

    /**
     * The current state status.
     */
    StateStatus currentStateStatus;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATEMACHINE_H_ */

