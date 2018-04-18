/**
 * @file MessageExample2.cpp
 * @brief Source file for class MessageExample2
 * @date 17/04/2018
 * @author Andre' Neto
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
 * the class MessageExample2 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "MessageI.h"
#include "MessageFilter.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "Sleep.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

/**
 * @brief Message filter for the MessageEx1 below (which replies to the message sent).
 */
class MessageFilterEx1: public MARTe::Object, public MARTe::MessageFilter {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
MessageFilterEx1    () : MARTe::Object(), MARTe::MessageFilter(true) {
        using namespace MARTe;
    }

    virtual void Purge(MARTe::ReferenceContainer &purgeList) {
        owner = MARTe::Reference();
    }

    virtual ~MessageFilterEx1 () {
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    void SetOwner(MARTe::Reference ownerIn) {
        owner = ownerIn;
    }

    virtual MARTe::ErrorManagement::ErrorType ConsumeMessage(MARTe::ReferenceT<MARTe::Message> &messageToTest);

private:
    MARTe::Reference owner;

};
CLASS_REGISTER(MessageFilterEx1, "")

/**
 * @brief A MARTe::Object class that will receive and reply to messages.
 */
class MessageEx1: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Install the message filter.
     */
MessageEx1    () : MARTe::Object(), MARTe::MessageI() {
        using namespace MARTe;
        filter = ReferenceT<MessageFilterEx1>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetOwner(this);
        ErrorManagement::ErrorType ret = MessageI::InstallMessageFilter(filter);
        messageReceived = false;
    }

    virtual ~MessageEx1 () {
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    virtual void Purge(MARTe::ReferenceContainer &purgeList) {
        filter->SetOwner(MARTe::Reference());
        RemoveMessageFilter(filter);
    }

    /**
     * @brief Print the message contents in the screen.
     */
    MARTe::ErrorManagement::ErrorType CheckMessage(MARTe::ReferenceT<MARTe::Message> &messageToTest) {
        using namespace MARTe;
        CCString sender = messageToTest->GetSender();
        CCString destination = messageToTest->GetDestination();
        CCString function = messageToTest->GetFunction();
        bool expectsReply = messageToTest->ExpectsReply();
        bool expectsIndirectReply = messageToTest->ExpectsIndirectReply();
        bool isReply = messageToTest->IsReply();
        REPORT_ERROR(MARTe::ErrorManagement::Information, "Received message from %s to %s with function %s "
                "(expectsReply? %d expectsIndirectReply? %d isReply? %d", sender.GetList(), destination.GetList(),
                function.GetList(), expectsReply, expectsIndirectReply, isReply);

        messageReceived = true;
        return ErrorManagement::NoError;
    }

    bool messageReceived;
private:
    MARTe::ReferenceT<MessageFilterEx1> filter;
};

CLASS_REGISTER(MessageEx1, "")

//Delegate to the owner the handling of the message (but could be handled by the filter itself as well).
MARTe::ErrorManagement::ErrorType MessageFilterEx1::ConsumeMessage(MARTe::ReferenceT<MARTe::Message> &messageToTest) {
    using namespace MARTe;
    ReferenceT<MessageEx1> ownerT = owner;
    ErrorManagement::ErrorType err;
    err.fatalError = !owner.IsValid();
    if (err.ErrorsCleared()) {
        err = ownerT->CheckMessage(messageToTest);
    }
    //Mark the message as a (direct) reply
    if (err.ErrorsCleared()) {
        messageToTest->SetAsReply(true);
        //And insert an (example) object into
        ReferenceT<Object> example = ReferenceT<Object>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        example->SetName("REPLY");
        messageToTest->Insert(example);
    }
    return err;
}

/**
 * @brief A MARTe::ReferenceContainer class that will send any messages inserted into it.
 * Note that it does not inherit from MessageI.
 */
class MessageEx2: public MARTe::ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
MessageEx2    () : MARTe::ReferenceContainer() {
        replyReceived = false;
    }

    virtual ~MessageEx2 () {
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    void SendMessages() {
        using namespace MARTe;
        uint32 numberOfMessages = Size();
        uint32 i;
        for (i=0u; i<numberOfMessages; i++) {
            ReferenceT<Message> msg = Get(i);
            ErrorManagement::ErrorType err;
            err.fatalError = !msg.IsValid();
            if (err.ErrorsCleared()) {
                err = MessageI::SendMessageAndWaitReply(msg, this);
                REPORT_ERROR(err, "Message %s sent", msg->GetName());
            }
            if (err.ErrorsCleared()) {
                if (msg->IsReply()) {
                    REPORT_ERROR(err, "Message %s is now a reply as expected", msg->GetName());
                }
                if (msg->Size() > 0) {
                    Reference ref = msg->Get(0);
                    REPORT_ERROR(err, "Message %s contains an object in the reply with name %s", msg->GetName(), ref->GetName());
                }
            }
        }
        replyReceived = true;
    }

    bool replyReceived;
};

CLASS_REGISTER(MessageEx2, "")

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+MsgRec1 = {\n"
            "    Class = MessageEx1\n"
            "}\n"
            "+MsgRec2 = {\n"
            "    Class = MessageEx1\n"
            "}\n"
            "+MsgRec3 = {\n"
            "    Class = MessageEx1\n"
            "}\n"
            "+MsgSender1= {\n"
            "    Class = MessageEx2\n"
            "    +Msg1 = {\n"
            "        Class = Message\n"
            "        Destination = MsgRec3"
            "        Function = \"AFunction\""
            "    }"
            "    +Msg2 = {\n"
            "        Class = Message\n"
            "        Destination = MsgRec2"
            "        Function = \"BFunction\""
            "    }"
            "    +Msg3 = {\n"
            "        Class = Message\n"
            "        Destination = MsgRec1"
            "        Function = \"CFunction\""
            "    }"
            "}";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s", configurationCfg.Buffer());
    ConfigurationDatabase cdb;
    StreamString err;
    //Force the string to be seeked to the beginning.
    configurationCfg.Seek(0LLU);
    StandardParser parser(configurationCfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        //After parsing the tree is pointing at the last leaf
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    else {
        StreamString errPrint;
        errPrint.Printf("Failed to parse %s", err.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, errPrint.Buffer());
    }

    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully loaded the configuration file");
    }

    ReferenceT<MessageEx1> msgRec1 = ObjectRegistryDatabase::Instance()->Find("MsgRec1");
    ReferenceT<MessageEx1> msgRec2 = ObjectRegistryDatabase::Instance()->Find("MsgRec2");
    ReferenceT<MessageEx1> msgRec3 = ObjectRegistryDatabase::Instance()->Find("MsgRec3");
    ReferenceT<MessageEx2> msgSender1 = ObjectRegistryDatabase::Instance()->Find("MsgSender1");

    if ((msgSender1.IsValid()) && (msgRec1.IsValid()) && (msgRec2.IsValid()) && (msgRec3.IsValid())) {
        msgSender1->SendMessages();
        while (!msgRec1->messageReceived) {
            Sleep::MSec(100);
        }
        while (!msgRec2->messageReceived) {
            Sleep::MSec(100);
        }
        while (!msgRec3->messageReceived) {
            Sleep::MSec(100);
        }
        while (!msgSender1->replyReceived) {
            Sleep::MSec(100);
        }
    }
    //Purge all the Objects!
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return 0;
}

