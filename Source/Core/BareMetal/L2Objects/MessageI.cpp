/**
 * @file MessageI.cpp
 * @brief Source file for class MessageI
 * @date 24/feb/2016
 * @author pc
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
 * the class MessageI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MessageI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MessageI::MessageI() {
    requestCode = 0u;
    uniqueId = 0;

}


MessageI::~MessageI(){

}


bool MessageI::SetContent(StructuredDataI & data) {
    //TODO
    return true;
}

StructuredDataI *MessageI::GetContent(){
    //TODO
    return NULL_PTR(StructuredDataI*);
}



void MessageI::SetUniqueId(int32 id) {
    uniqueId = id;
}

void MessageI::SetRequestCode(uint32 code) {
    requestCode = code;
}

uint32 MessageI::GetRequestCode() const {
    return requestCode;
}

uint32 MessageI::GetUniqueId() const {
    return uniqueId;
}


}
