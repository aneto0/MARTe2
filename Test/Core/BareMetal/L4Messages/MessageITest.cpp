/**
 * @file MessageITest.cpp
 * @brief Source file for class MessageITest
 * @date 11/05/2016
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
 * the class MessageITest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MessageITest.h"
#include "ObjectWithMessages.h"
#include "Message.h"
#include "ConfigurationDatabase.h"
#include "ObjectRegistryDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//using namespace MARTe;
bool MessageITest::TestDefaultConstructor() {
    MessageI mess;
    return true;
}

bool MessageITest::TestSendMessage() {
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "ReceiverMethod");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    if (sender->SendMessage(mess, sender.operator->())!=ErrorManagement::NoError) {
        return false;
    }
    return (receiver->Flag() == 0);
}


bool TestSendMessage_NULL_Source(){
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "ReceiverMethod");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    if (sender->SendMessage(mess, NULL)!=ErrorManagement::NoError) {
        return false;
    }
    return (receiver->Flag() == 0);

}
