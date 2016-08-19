/**
 * @file RegisteredMethodsMessageFilter.h
 * @brief Header file for class RegisteredMethodsMessageFilter
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

 * @details This header file contains the declaration of the class RegisteredMethodsMessageFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4MESSAGES_REGISTEREDMETHODSMESSAGEFILTER_H_
#define L4MESSAGES_REGISTEREDMETHODSMESSAGEFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MessageFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



namespace MARTe {

/**
* TODO
* Allows activating registered methods
* The filters remains permanently active until expressly removed
*/
class RegisteredMethodsMessageFilter: public MessageFilter{
public:
    CLASS_REGISTER_DECLARATION()


/**
 * TODO
 * Initialises basic search filter
 *
 */
RegisteredMethodsMessageFilter(Object *destination);

/**
 * TODO
 * Initialises basic search filter
 *
 */
virtual ~RegisteredMethodsMessageFilter();

protected:

    /**
     * TODO
     */
    Object *destinationObject;

    /**
     * TODO
     * on every match will also consume the message and handles both reply mechanisms
    */
    virtual ErrorManagement::ErrorType TestMessage(ReferenceT<Message> &messageToTest);

    /**
     * TODO
     * Interpret errors to say whether the message was consumed
     */
    virtual bool Matched(ErrorManagement::ErrorType ret);

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4MESSAGES_REGISTEREDMETHODSMESSAGEFILTER_H_ */
	
