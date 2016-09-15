/**
 * @file RegisteredMethodsMessageFilter.h
 * @brief Header file for class RegisteredMethodsMessageFilter
 * @date 19/08/2016
 * @author Filippo Sartori
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

#ifndef REGISTEREDMETHODSMESSAGEFILTER_H_
#define REGISTEREDMETHODSMESSAGEFILTER_H_

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
 * @brief Calls the registered method in the messages trapped by this filter instance.
 * @details The filters remains permanently active until expressly removed.
 */
class RegisteredMethodsMessageFilter: public MessageFilter {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. Initialises the MessageFilter(true).
     * @post
     *   IsPermanentFilter() == true
     */
    RegisteredMethodsMessageFilter();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~RegisteredMethodsMessageFilter();

    /**
     * @brief Sets the destination object where the registered method is to be called.
     * @param[in] destination the object where the registered method is to be called.
     */
    void SetDestination(Object *destination);
protected:

    /**
     * @brief Calls on the destination object (see SetDestination) the registered method with name messageToTest->GetFunction().
     * @param[in] messageToTest The message holding the function to call.
     * @return ErrorManagement::NoError if the method (messageToTest->GetFunction()) can be successfully called on the destination object.
     */
    virtual ErrorManagement::ErrorType ConsumeMessage(ReferenceT<Message> &messageToTest);

    /**
     * Object where the registered method is to be called.
     */
    Object *destinationObject;
};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REGISTEREDMETHODSMESSAGEFILTER_H_ */

