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

MessageITest::MessageITest() {
}

MessageITest::~MessageITest() {
}

bool MessageITest::TestDefaultConstructor() {
    using namespace MARTe;
    MessageI mess;
    return true;
}

bool MessageITest::TestSendMessage() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
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

    status = MessageI::SendMessage(mess, sender.operator->());
    if (status != ErrorManagement::noError) {
        return false;
    }

    result = (receiver->Flag() == 0);

    return result;
}

bool MessageITest::TestSendMessage_NULL_Source() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "ReceiverMethod");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, NULL);
    if (status != ErrorManagement::noError) {
        return false;
    }

    result = (receiver->Flag() == 0);

    return result;

}

bool MessageITest::TestSendMessage_False_InvalidMessage() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    receiver->SetName("receiver");

    ReferenceT<Message> mess;
    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, NULL);

    result = (status == ErrorManagement::parametersError);

    return result;
}

bool MessageITest::TestSendMessage_False_NotExpectedLateReply() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
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

    mess->SetAsReply();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, sender.operator->());

    result = (status == ErrorManagement::communicationError);

    return result;
}

bool MessageITest::TestSendMessage_False_NoDestinationForReply() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "ReceiverMethod");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    mess->SetAsReply();
    mess->ExpectsIndirectReply();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, NULL);

    result = (status == ErrorManagement::unsupportedFeature);

    return result;

}

bool MessageITest::TestSendMessage_False_NoDestinationForExpectedReply() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "ReceiverMethod");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    mess->SetExpectsReply(true);

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, NULL);

    result = (status == ErrorManagement::parametersError);

    return result;

}

bool MessageITest::TestSendMessage_False_InvalidDestination() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "invalid");
    cdb.Write("Function", "ReceiverMethod");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, sender.operator->());

    result = (status == ErrorManagement::unsupportedFeature);

    return result;

}

bool MessageITest::TestSendMessage_False_InvalidFunction() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "Invalid");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, sender.operator->());

    result = (status == ErrorManagement::unsupportedFeature);

    return result;

}

bool MessageITest::TestSendMessageAndWaitReply() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
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

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());
    if (status != ErrorManagement::noError) {
        return false;
    }
    if (!mess->IsReply()) {
        return false;
    }
    if (!mess->ExpectsIndirectReply()) {
        return false;
    }

    result = (receiver->Flag() == 0);

    return result;
}

bool MessageITest::TestSendMessageAndWaitReply_False_InvalidMessage() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("receiver");

    ReferenceT<Message> mess;

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());

    result = (status == ErrorManagement::parametersError);

    return result;
}

bool MessageITest::TestSendMessageAndWaitReply_False_ReplyOfReply() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
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

    mess->SetAsReply();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());

    result = (status == ErrorManagement::communicationError);

    return result;
}

bool MessageITest::TestSendMessageAndExpectReplyLater() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
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

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());
    if (status != ErrorManagement::noError) {
        return false;
    }

    result = ((sender->Flag() == 2) && (receiver->Flag() == 0));

    return result;
}

bool MessageITest::TestSendMessageAndExpectReplyLater_False_InvalidMessage() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("receiver");

    ReferenceT<Message> mess;

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());

    result = (status == ErrorManagement::parametersError);

    return result;
}

bool MessageITest::TestSendMessageAndExpectReplyLater_False_ReplyOfReply() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
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

    mess->SetAsReply();

    ObjectRegistryDatabase::Instance()->CleanUp();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());

    result = (status == ErrorManagement::communicationError);

    return result;
}
