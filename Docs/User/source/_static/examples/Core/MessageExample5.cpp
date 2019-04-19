/**
 * @file MessageExample5.cpp
 * @brief Source file for class MessageExample5
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
 * the class MessageExample5 (public, protected, and private). Be aware that some
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
#include "CLASSMETHODREGISTER.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "MessageI.h"
#include "MessageFilter.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "Sleep.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

/**
 * @brief A MARTe::Object class that registers a set of RPC functions that can be called
 * with messages.
 */
class MessageEx1: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Install the RegisteredMethodsMessageFilter filter.
     */
    MessageEx1 () : MARTe::Object(), MARTe::MessageI() {
        using namespace MARTe;
        filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        MessageI::InstallMessageFilter(filter);
    }

    virtual ~MessageEx1 () {
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    virtual void Purge(MARTe::ReferenceContainer &purgeList) {
        RemoveMessageFilter(filter);
    }

    MARTe::ErrorManagement::ErrorType Function0 () {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Function0 called.");
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Function1 (MARTe::uint32 a, MARTe::float32 b) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Received %u %f.", a, b);
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Function2 (MARTe::int32 &a, MARTe::float32 &b) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Received %u %f.", a, b);
        a = -a;
        b = -b;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Function3 (MARTe::StreamString a) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Received %s.", a.Buffer());
        return MARTe::ErrorManagement::NoError;
    }

private:
    MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter;
};

CLASS_REGISTER(MessageEx1, "")
CLASS_METHOD_REGISTER(MessageEx1, Function0)
CLASS_METHOD_REGISTER(MessageEx1, Function1)
CLASS_METHOD_REGISTER(MessageEx1, Function2)
CLASS_METHOD_REGISTER(MessageEx1, Function3)

/**
 * @brief A MARTe::Object class that will send indirect reply messages and waits for the reply.
 * Note that the method SendMessageAndWaitIndirectReply will automatically register a ReplyMessageCatcherMessageFilter filter
 */
class MessageEx2: public MARTe::ReferenceContainer, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP
     */
    MessageEx2 () : MARTe::ReferenceContainer(), MARTe::MessageI() {
        using namespace MARTe;
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
                if (!msg->ExpectsIndirectReply()) {
                    msg->SetExpectsIndirectReply(true);
                }
                err = SendMessageAndWaitIndirectReply(msg);
                REPORT_ERROR(err, "Message %s sent", msg->GetName());
            }
            if (err.ErrorsCleared()) {
                if (msg->IsReply()) {
                    REPORT_ERROR(err, "Message %s is now a reply as expected", msg->GetName());
                    ReferenceT<ConfigurationDatabase> params;
                    if (msg->Size() > 0) {
                        params = msg->Get(0);
                    }
                    if (params.IsValid()) {
                        REPORT_ERROR(err, "Replied parameters %!", *(params.operator ->()));
                    }
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
            "+MsgSender1= {\n"
            "    Class = MessageEx2\n"
            "    +Msg0 = {\n"
            "        Class = Message\n"
            "        Destination = MsgRec1"
            "        Function = \"Function0\""
            "        Mode = \"ExpectsIndirectReply\""
            "    }"
            "    +Msg1 = {\n"
            "        Class = Message\n"
            "        Destination = MsgRec1"
            "        Function = \"Function1\""
            "        Mode = \"ExpectsIndirectReply\""
            "        +Parameters = {"
            "            Class = ConfigurationDatabase"
            "            param1 = 2"
            "            param2 = 3.14"
            "        }"
            "    }"
            "    +Msg2 = {\n"
            "        Class = Message\n"
            "        Destination = MsgRec1"
            "        Function = \"Function2\""
            "        Mode = \"ExpectsIndirectReply\""
            "        +Parameters = {"
            "            Class = ConfigurationDatabase"
            "            param1 = 2"
            "            param2 = 3.14"
            "        }"
            "    }"
            "    +Msg3 = {\n"
            "        Class = Message\n"
            "        Destination = MsgRec1"
            "        Function = \"Function3\""
            "        Mode = \"ExpectsIndirectReply\""
            "        +Parameters = {"
            "            Class = ConfigurationDatabase"
            "            param1 = \"This is a string\""
            "        }"
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
    ReferenceT<MessageEx2> msgSender1 = ObjectRegistryDatabase::Instance()->Find("MsgSender1");

    if ((msgSender1.IsValid()) && (msgRec1.IsValid())) {
        msgSender1->SendMessages();

        while (!msgSender1->replyReceived) {
            Sleep::MSec(100);
        }
    }
    //Purge all the Objects!
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return 0;
}

