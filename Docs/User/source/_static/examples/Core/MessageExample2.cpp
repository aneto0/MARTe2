/**
 * @file MessageExample2.cpp
 * @brief Source file for class MessageExample2
 * @date 08/04/2018
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
 * @brief Message filter for the MessageEx1 below.
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
 * @brief A simple MARTe::Object class that will receive and reply to messages.
 */
class MessageEx1: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
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
        messageToTest->SetAsReply(true);
        return ErrorManagement::NoError;
    }

    bool messageReceived;
private:
    MARTe::ReferenceT<MessageFilterEx1> filter;
};

CLASS_REGISTER(MessageEx1, "")

MARTe::ErrorManagement::ErrorType MessageFilterEx1::ConsumeMessage(MARTe::ReferenceT<MARTe::Message> &messageToTest) {
    MARTe::ReferenceT<MessageEx1> ownerT = owner;
    MARTe::ErrorManagement::ErrorType err;
    err.fatalError = !owner.IsValid();
    if (err.ErrorsCleared()) {
        err = ownerT->CheckMessage(messageToTest);
    }
    return err;
}

/**
 * @brief A simple MARTe::Object class that will send messages.
 */
class MessageEx2: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
    MessageEx2 () : MARTe::Object(), MARTe::MessageI() {
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
        messageToTest->SetAsReply(true);
        return ErrorManagement::NoError;
    }

    bool messageReceived;
private:
    MARTe::ReferenceT<MessageFilterEx1> filter;
};

CLASS_REGISTER(MessageEx1, "")


}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+MsgEx1 = {\n"
            "    Class = MessageEx1\n"
            "}\n"
            "+MsgEx2 = {\n"
            "    Class = MessageEx1\n"
            "}\n"
            "+MsgEx3 = {\n"
            "    Class = MessageEx1\n"
            "}\n"
            "+Messages = {\n"
            "    Class = ReferenceContainer\n"
            "    +Msg1 = {\n"
            "        Class = Message\n"
            "        Destination = MsgEx3"
            "        Function = \"AFunction\""
            "    }"
            "    +Msg2 = {\n"
            "        Class = Message\n"
            "        Destination = MsgEx2"
            "        Function = \"BFunction\""
            "    }"
            "    +Msg3 = {\n"
            "        Class = Message\n"
            "        Destination = MsgEx1"
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

    ReferenceT<MessageEx1> msgEx1 = ObjectRegistryDatabase::Instance()->Find("MsgEx1");
    ReferenceT<MessageEx1> msgEx2 = ObjectRegistryDatabase::Instance()->Find("MsgEx2");
    ReferenceT<MessageEx1> msgEx3 = ObjectRegistryDatabase::Instance()->Find("MsgEx3");
    ReferenceT<Message> msg1 = ObjectRegistryDatabase::Instance()->Find("Messages.Msg1");
    ReferenceT<Message> msg2 = ObjectRegistryDatabase::Instance()->Find("Messages.Msg2");
    ReferenceT<Message> msg3 = ObjectRegistryDatabase::Instance()->Find("Messages.Msg3");
    MessageI::SendMessage(msg1, NULL_PTR(Object *));
    MessageI::SendMessage(msg2, NULL_PTR(Object *));
    MessageI::SendMessage(msg3, NULL_PTR(Object *));

    while (!msgEx1->messageReceived) {
        Sleep::MSec(100);
    }
    while (!msgEx2->messageReceived) {
        Sleep::MSec(100);
    }
    while (!msgEx3->messageReceived) {
        Sleep::MSec(100);
    }
    //Purge all the Objects!
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return 0;
}

