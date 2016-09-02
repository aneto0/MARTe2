/**
 * @file RegisteredMethodsMessageFilter.cpp
 * @brief Source file for class RegisteredMethodsMessageFilter
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

 * @details This source file contains the definition of all the methods for
 * the class RegisteredMethodsMessageFilter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RegisteredMethodsMessageFilter.h"
#include "MessageI.h"


namespace MARTe {
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RegisteredMethodsMessageFilter::RegisteredMethodsMessageFilter() :MessageFilter(true){
    destinationObject = NULL_PTR(Object *);
}

RegisteredMethodsMessageFilter::~RegisteredMethodsMessageFilter(){
}

void RegisteredMethodsMessageFilter::SetDestination(Object *destination){
    destinationObject = destination;
}

ErrorManagement::ErrorType RegisteredMethodsMessageFilter::ConsumeMessage(ReferenceT<Message> &messageToTest){

    ErrorManagement::ErrorType ret;

    //This filter does not handle replies...
    if ((destinationObject != NULL_PTR(Object *)) && (messageToTest.IsValid()) && (!messageToTest->IsReply())){

        // try calling the method
        ret = destinationObject->CallRegisteredMethod(messageToTest->GetFunction(), *(messageToTest.operator->()));

        // the registered method has no responsibility to handle the reply mechanism
        // therefore it is handled here
        if (MessageConsumed(ret) && messageToTest->ExpectsReply()){
            messageToTest->SetAsReply();

            // TODO handling other error messages resulting from the call
            // store them in the reply
            // handles indirect reply
            if (messageToTest->ExpectsIndirectReply()){
                // TODO handle error messages from SendMessage
                // simply produce a warning
                // destination in reply is known so should not be set
                MessageI::SendMessage(messageToTest, NULL);
            }
        }
    } else {
        ret.unsupportedFeature = true;
    }

    return ret;

}

CLASS_REGISTER(RegisteredMethodsMessageFilter, "1.0")

}


	
