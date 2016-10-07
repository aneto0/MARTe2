/**
 * @file StateMachineState.h
 * @brief Header file for class StateMachineState
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

 * @details This header file contains the declaration of the class StateMachineState
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATEMACHINESTATE_H_
#define STATEMACHINESTATE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * @brief TODO
 */
class DLL_API StateMachineState: public MessageFilter, public ReferenceContainer {

public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     * @post
     *    GetCode() == 0
     */
    StateMachineState();

    /**
     * @brief Gets the code that identifies the state.
     * @return the code that identifies the state.
     */
    uint32 GetCode() const;


    /**
     * TODO.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);
private:
    /**
     * TODO
     */
    ErrorManagement::ErrorType ActOn(CCString instruction);

    /**
     * A numerical code which identifies the state.
     */
    uint32 code;

    /**
     * Reference to the state-machine which holds this state
     */
    Reference stateMachineIn;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATEMACHINESTATE_H_ */

