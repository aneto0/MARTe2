/**
 * @file RegisteredMethodsMessageFilter.cpp
 * @brief Source file for class RegisteredMethodsMessageFilter
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

RegisteredMethodsMessageFilter::RegisteredMethodsMessageFilter(Object * object):MessageFilter(true){
    destinationObject = object;
}

RegisteredMethodsMessageFilter::~RegisteredMethodsMessageFilter(){
}

ErrorManagement::ErrorType RegisteredMethodsMessageFilter::ProcessMessage(ReferenceT<Message> &messageToTest){

    ErrorManagement::ErrorType ret;

//    Object *destinationObject = dynamic_cast<Object *>(receiver);

    if ((destinationObject != NULL_PTR(Object *)) && (messageToTest.IsValid())){

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
                MessageI::SendMessage(messageToTest,destinationObject);
            }
        }
    } else {
        ret.unsupportedFeature = true;
    }

    return ret;

}

}


	
