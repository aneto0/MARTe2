/**
 * @file ObjectWithMessages.cpp
 * @brief Source file for class ObjectWithMessages
 * @date 14/06/2016
 * @author Giuseppe Ferro
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
 * the class ObjectWithMessages (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItemT.h"
#include "ObjectWithMessages.h"
#include "RegisteredMethodsMessageFilter.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ObjectWithMessages::ObjectWithMessages() {
    ReferenceT<RegisteredMethodsMessageFilter> registeredMethodsMessageFilter("RegisteredMethodsMessageFilter");
    registeredMethodsMessageFilter->SetDestination(this);

    if (registeredMethodsMessageFilter.IsValid()) {
        InstallMessageFilter(registeredMethodsMessageFilter);
    }
    flag = -1;
}

bool ObjectWithMessages::ReceiverMethod(ReferenceContainer& ref) {
    flag = 0;
    ReferenceT<Object> obj = ReferenceT<Object>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    obj->SetName("REPLY");
    ref.Insert(obj);
    return true;
}

bool ObjectWithMessages::SenderMethod(ReferenceContainer& ref) {
    flag = 1;
    return true;
}

int32 ObjectWithMessages::Flag() {
    return flag;
}

bool ObjectWithMessages::HandleReply() {
    flag = 2;
    return true;
}

CLASS_REGISTER(ObjectWithMessages, "1.0")

CLASS_METHOD_REGISTER(ObjectWithMessages, &ObjectWithMessages::ReceiverMethod, &ObjectWithMessages::SenderMethod, &ObjectWithMessages::HandleReply)

}
