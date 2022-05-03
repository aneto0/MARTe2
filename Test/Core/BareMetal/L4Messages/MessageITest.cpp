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

#include "ConfigurationDatabase.h"
#include "Message.h"
#include "MessageITest.h"
#include "ObjectRegistryDatabase.h"
#include "ObjectWithMessages.h"
#include "ReplyMessageCatcherMessageFilter.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
class ObjectWithMessagesCatcher: public ReplyMessageCatcherMessageFilter {
public:
    CLASS_REGISTER_DECLARATION()ObjectWithMessagesCatcher() {

    }

    void SetObjectToHandle(ReferenceT<ObjectWithMessages> objectToHandleIn) {
        objectToHandle = objectToHandleIn;
    }

    virtual void HandleReplyMessage(ReferenceT<Message> &replyMessage) {
        ReplyMessageCatcherMessageFilter::HandleReplyMessage(replyMessage);
        objectToHandle->HandleReply();
    }

private:
    ReferenceT<ObjectWithMessages> objectToHandle;
};

CLASS_REGISTER(ObjectWithMessagesCatcher, "1.0")
}

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

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, sender.operator->());
    if (status != ErrorManagement::NoError) {
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

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, NULL);
    if (status != ErrorManagement::NoError) {
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
    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, NULL);

    result = (status == ErrorManagement::ParametersError);

    return result;
}

bool MessageITest::TestSendMessage_False_NotExpectedIndirectReply() {
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

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, sender.operator->());

    result = (status == ErrorManagement::CommunicationError);

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

    mess->SetExpectsReply(true);

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, NULL);

    result = (status == ErrorManagement::ParametersError);

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

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, NULL);

    result = (status == ErrorManagement::ParametersError);

    return result;

}

bool MessageITest::TestSendMessage_False_InvalidDestination() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<Object> receiver = ReferenceT<Object>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "ReceiverMethod");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, sender.operator->());

    result = (status == ErrorManagement::UnsupportedFeature);
    ObjectRegistryDatabase::Instance()->Purge();
    return result;
}

bool MessageITest::TestSendMessage_False_InvalidDestination_Not_Exist() {
    using namespace MARTe;
    bool result = false;
    ErrorManagement::ErrorType status;
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<Object> receiver = ReferenceT<Object>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("receiver");

    ReferenceT<Message> mess = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiverDoesNotExist");
    cdb.Write("Function", "ReceiverMethod");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, sender.operator->());

    result = (status == ErrorManagement::UnsupportedFeature);
    ObjectRegistryDatabase::Instance()->Purge();
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

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessage(mess, sender.operator->());

    result = (status == ErrorManagement::UnsupportedFeature);

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
    cdb.Write("Mode", "ExpectsReply");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());
    if (status != ErrorManagement::NoError) {
        return false;
    }
    if (!mess->IsReply()) {
        return false;
    }
    if (mess->ExpectsIndirectReply()) {
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

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());

    result = (status == ErrorManagement::ParametersError);

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
    cdb.Write("Mode", "ExpectsReply");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    mess->SetAsReply();

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = MessageI::SendMessageAndWaitReply(mess, sender.operator->());

    result = (status == ErrorManagement::CommunicationError);

    return result;
}

bool MessageITest::TestSendMessage_ExpectsIndirectReply() {
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
    cdb.Write("Mode", "ExpectsIndirectReply");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    ReferenceT<ObjectWithMessagesCatcher> catcherFilter = ReferenceT<ObjectWithMessagesCatcher>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (!catcherFilter.IsValid()) {
        return false;
    }
    catcherFilter->SetMessageToCatch(mess);
    catcherFilter->SetObjectToHandle(sender);
    sender->InstallMessageFilter(catcherFilter);

    status = MessageI::SendMessage(mess, sender.operator->());
    if (status != ErrorManagement::NoError) {
        return false;
    }
    status = catcherFilter->Wait(TTInfiniteWait);
    if (status != ErrorManagement::NoError) {
        return false;
    }

    result = ((sender->Flag() == 2) && (receiver->Flag() == 0));

    return result;
}

bool MessageITest::TestSendMessageAndWaitIndirectReply() {
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
    cdb.Write("Mode", "ExpectsIndirectReply");

    if (!mess->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    status = sender->SendMessageAndWaitIndirectReply(mess);
    if (status != ErrorManagement::NoError) {
        return false;
    }
    ReferenceT<Object> replyObj = mess->Get(0);
    if (!replyObj.IsValid()) {
        return false;
    }
    if (StringHelper::Compare(replyObj->GetName(), "REPLY") != 0) {
        return false;
    }

    result = (receiver->Flag() == 0);

    return result;
}

bool MessageITest::TestSendMessageAndWaitIndirectReply_InvalidMessage() {
    using namespace MARTe;
    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<Message> msg;
    ErrorManagement::ErrorType status = sender->SendMessageAndWaitIndirectReply(msg);

    return status == ErrorManagement::ParametersError;
}

bool MessageITest::TestSendMessageAndWaitIndirectReply_InvalidDestination() {
    using namespace MARTe;

    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ObjectWithMessages> receiver = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->SetName("sender");
    receiver->SetName("does.not.exist");

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    cdb.Write("Destination", "receiver");
    cdb.Write("Function", "ReceiverMethod");
    cdb.Write("Mode", "ExpectsIndirectReply");

    if (!msg->Initialise(cdb)) {
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Insert(sender);
    ObjectRegistryDatabase::Instance()->Insert(receiver);

    ErrorManagement::ErrorType status = sender->SendMessageAndWaitIndirectReply(msg);

    return (status == ErrorManagement::UnsupportedFeature);
}

bool MessageITest::TestWaitForReply() {
    using namespace MARTe;

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    msg->SetAsReply(true);
    msg->SetExpectsReply(true);
    ErrorManagement::ErrorType status = MessageI::WaitForReply(msg, 1, 1);
    return status;
}

bool MessageITest::TestWaitForReply_InvalidMessage() {
    using namespace MARTe;

    ReferenceT<Message> msg;
    ErrorManagement::ErrorType status = MessageI::WaitForReply(msg, 1, 1);
    return status == ErrorManagement::ParametersError;
}

bool MessageITest::TestWaitForReply_NotExpectsReply() {
    using namespace MARTe;

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    msg->SetAsReply(true);
    msg->SetExpectsReply(false);
    ErrorManagement::ErrorType status = MessageI::WaitForReply(msg, 1, 1);
    return status == ErrorManagement::CommunicationError;
}

bool MessageITest::TestWaitForReply_Timeout() {
    using namespace MARTe;

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    msg->SetExpectsReply(true);
    ErrorManagement::ErrorType status = MessageI::WaitForReply(msg, 1, 1);
    return status == ErrorManagement::Timeout;
}

bool MessageITest::TestInstallMessageFilter() {
    using namespace MARTe;

    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ReplyMessageCatcherMessageFilter> filter = ReferenceT<ReplyMessageCatcherMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    return sender->InstallMessageFilter(filter);
}

bool MessageITest::TestRemoveMessageFilter() {
    using namespace MARTe;

    ReferenceT<ObjectWithMessages> sender = ReferenceT<ObjectWithMessages>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<ReplyMessageCatcherMessageFilter> filter = ReferenceT<ReplyMessageCatcherMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    sender->InstallMessageFilter(filter);
    return sender->RemoveMessageFilter(filter);
}
