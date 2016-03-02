/**
 * @file MessageBroker.cpp
 * @brief Source file for class MessageBroker
 * @date 26/02/2016
 * @author Ivan Herrero
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
 * the class MessageBroker (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MessageBroker.h"
#include "Reference.h"
#include "GlobalObjectsDatabase.h"
#include "ObjectRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MessageBroker *MessageBroker::Instance() {

    static MessageBroker *instance = NULL_PTR(MessageBroker *);
    if (instance == NULL) {
        instance=new MessageBroker();
        GlobalObjectsDatabase::Instance()->Add(instance, NUMBER_OF_GLOBAL_OBJECTS - 4u);
    }
    return instance;
}

MessageBroker::MessageBroker() {

}

MessageBroker::~MessageBroker() {

}

void MessageBroker::SendMessage(const Envelope &envelope, MessageI &reply) {
    Reference ref;

    ref = ObjectRegistryDatabase::Instance()->Find(envelope.GetAddress());

    if (ref.IsValid()) {
        bool ret;
        ret = ref->ProcessMessage(envelope.GetMessage(), reply);
    }
}

}
