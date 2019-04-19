/**
 * @file StateMachineExample1.cpp
 * @brief Source file for class StateMachineExample1
 * @date 25/04/2018
 * @author Andre Neto
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
 * the class StateMachineExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

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
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "StandardParser.h"
#include "StateMachine.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe2Tutorial {

/**
 * @brief A MARTe::Object class that registers a set of RPC functions that can be called
 * with messages.
 */
class MessageReceiverEx1: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Install the RegisteredMethodsMessageFilter filter.
     */
MessageReceiverEx1    () : MARTe::Object(), MARTe::MessageI() {
        using namespace MARTe;
        filter = ReferenceT<RegisteredMethodsMessageFilter>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        MessageI::InstallMessageFilter(filter);
        fun0Called = false;
        fun1Called = false;
        fun2Called = false;
        fun3Called = false;
    }

    virtual ~MessageReceiverEx1 () {
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
        fun0Called = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Function1 (MARTe::uint32 a, MARTe::float32 b) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Received %u %f.", a, b);
        fun1Called = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Function2 (MARTe::int32 a, MARTe::float32 b, MARTe::uint32 c) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Received %u %f %u.", a, b,c);
        fun2Called = true;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::ErrorManagement::ErrorType Function3 (MARTe::StreamString a) {
        REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "Received %s.", a.Buffer());
        fun3Called = true;
        return MARTe::ErrorManagement::NoError;
    }

    bool fun0Called;
    bool fun1Called;
    bool fun2Called;
    bool fun3Called;

private:
    MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter;
};

CLASS_REGISTER(MessageReceiverEx1, "")
CLASS_METHOD_REGISTER(MessageReceiverEx1, Function0)
CLASS_METHOD_REGISTER(MessageReceiverEx1, Function1)
CLASS_METHOD_REGISTER(MessageReceiverEx1, Function2)
CLASS_METHOD_REGISTER(MessageReceiverEx1, Function3)

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+Receiver1 = {\n"
            "    Class = MessageReceiverEx1\n"
            "}\n"
            "+Receiver2 = {\n"
            "    Class = MessageReceiverEx1\n"
            "}\n"
            "+StateMachineExample1 = {\n"
            "    Class = StateMachine\n"
            "    +STATE1 = {\n"
            "       Class = ReferenceContainer\n"
            "       +GOTOSTATE2 = {\n"
            "          Class = StateMachineEvent\n"
            "          NextState = \"STATE2\"\n"
            "          NextStateError = \"STATE1\"\n"
            "          Timeout = 0\n"
            "          +DoSomethingOnRec1 = {\n"
            "              Class = Message\n"
            "              Destination = Receiver1\n"
            "              Mode = ExpectsReply\n"
            "              Function = Function1\n"
            "              +Parameters = {\n"
            "                   Class = ConfigurationDatabase\n"
            "                   param1 = 2\n"
            "                   param2 = 3.14\n"
            "              }\n"
            "          }\n"
            "          +DoSomethingElseOnRec1 = {\n"
            "              Class = Message\n"
            "              Destination = Receiver1\n"
            "              Mode = ExpectsReply\n"
            "              Function = Function0\n"
            "          }\n"
            "       }\n"
            "       +GOTOSTATE3 = {\n"
            "          Class = StateMachineEvent\n"
            "          NextState = \"STATE3\"\n"
            "          NextStateError = \"STATE1\"\n"
            "          Timeout = 0\n"
            "          +DoSomethingOnRec2 = {\n"
            "              Class = Message\n"
            "              Destination = \"Receiver1\"\n"
            "              Mode = ExpectsReply\n"
            "              Function = Function1\n"
            "              +Parameters = {\n"
            "                   Class = ConfigurationDatabase\n"
            "                   param1 = 2\n"
            "                   param2 = 3.14\n"
            "              }\n"
            "          }\n"
            "          +DoSomethingElseOnRec2 = {\n"
            "              Class = Message\n"
            "              Destination = \"Receiver2\"\n"
            "              Mode = ExpectsReply\n"
            "              Function = Function0\n"
            "          }\n"
            "       }\n"
            "    }\n"
            "    +STATE2 = {\n"
            "       Class = ReferenceContainer\n"
            "       +GOTOSTATE1 = {\n"
            "          Class = StateMachineEvent\n"
            "          NextState = \"STATE1\"\n"
            "          NextStateError = \"STATE1\"\n"
            "          Timeout = 0\n"
            "          +DoSomethingOnRec1 = {\n"
            "              Class = Message\n"
            "              Destination = Receiver1\n"
            "              Mode = ExpectsReply\n"
            "              Function = Function3\n"
            "              +Parameters = {\n"
            "                   Class = ConfigurationDatabase\n"
            "                   param1 = \"BACKTOSTATE1!\"\n"
            "              }\n"
            "          }\n"
            "       }\n"
            "    }\n"
            "    +STATE3 = {\n"
            "       Class = ReferenceContainer\n"
            "       +ENTER = {\n"
            "           Class = ReferenceContainer\n"
            "           +DoSomethingOnRec1 = {\n"
            "              Class = Message\n"
            "              Destination = Receiver1\n"
            "              Mode = ExpectsReply\n"
            "              Function = Function2\n"
            "              +Parameters = {\n"
            "                   Class = ConfigurationDatabase\n"
            "                   param1 = 1"
            "                   param2 = 2"
            "                   param3 = 3"
            "              }\n"
            "          }\n"
            "       }"
            "       +GOTOSTATE2 = {\n"
            "          Class = StateMachineEvent\n"
            "          NextState = \"STATE2\"\n"
            "          NextStateError = \"STATE2\"\n"
            "          Timeout = 0\n"
            "          +DoSomethingOnRec2 = {\n"
            "              Class = Message\n"
            "              Destination = Receiver2\n"
            "              Mode = ExpectsReply\n"
            "              Function = Function3\n"
            "              +Parameters = {\n"
            "                   Class = ConfigurationDatabase\n"
            "                   param1 = \"BACKTOSTATE2!\"\n"
            "              }\n"
            "          }\n"
            "       }\n"
            "    }\n"
            "}\n";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s", configurationCfg.Buffer());
    ConfigurationDatabase cdb;
    StreamString err;
    //Force the string to be seeked to the beginning.
    configurationCfg.Seek(0LLU);
    StandardParser parser(configurationCfg, cdb, &err);
    bool ok = parser.Parse();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        //After parsing the tree is pointing at the last leaf
        cdb.MoveToRoot();
        ok = ord->Initialise(cdb);
    }
    else {
        StreamString errPrint;
        errPrint.Printf("Failed to parse %s", err.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, errPrint.Buffer());
    }
    ReferenceT<MessageReceiverEx1> rec1 = ord->Find("Receiver1");
    ReferenceT<MessageReceiverEx1> rec2 = ord->Find("Receiver2");

    //Send a message to the state machine
    ReferenceT<Message> msg1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase msg1Cdb;
    msg1Cdb.Write("Destination", "StateMachineExample1");
    msg1Cdb.Write("Function", "GOTOSTATE3");
    msg1->Initialise(msg1Cdb);
    MessageI::SendMessage(msg1, NULL);

    while (!rec2->fun0Called) {
        Sleep::Sec(0.1);
    }

    ReferenceT<Message> msg2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase msg2Cdb;
    msg2Cdb.Write("Destination", "StateMachineExample1");
    msg2Cdb.Write("Function", "GOTOSTATE2");
    msg2->Initialise(msg2Cdb);
    MessageI::SendMessage(msg2, NULL);

    while (!rec2->fun3Called) {
        Sleep::Sec(0.1);
    }

    ReferenceT<Message> msg3(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase msg3Cdb;
    msg3Cdb.Write("Destination", "StateMachineExample1");
    msg3Cdb.Write("Function", "GOTOSTATE1");
    msg3->Initialise(msg3Cdb);
    MessageI::SendMessage(msg3, NULL);

    while (!rec1->fun3Called) {
        Sleep::Sec(0.1);
    }
    //Purge all the Objects!
    ObjectRegistryDatabase::Instance()->Purge();
    return 0;
}

