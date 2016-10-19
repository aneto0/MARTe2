/**
 * @file StateMachineTest.cpp
 * @brief Source file for class StateMachineTest
 * @date 15/10/2016
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
 * the class StateMachineTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "CLASSMETHODREGISTER.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "StandardParser.h"
#include "StateMachine.h"
#include "StateMachineEvent.h"
#include "StateMachineTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class StateMachineTestMessageReceiver: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

StateMachineTestMessageReceiver    () {
        MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter = MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter>(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        InstallMessageFilter(filter);
        flag = 0;
    }

    MARTe::ErrorManagement::ErrorType ReceiverMethod(MARTe::ReferenceContainer& ref) {
        flag++;
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::int32 flag;

};
CLASS_REGISTER(StateMachineTestMessageReceiver, "1.0")
CLASS_METHOD_REGISTER(StateMachineTestMessageReceiver, ReceiverMethod)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool StateMachineTest::TestDefaultConstructor() {
    using namespace MARTe;
    StateMachine stateMachine;
    bool ok = (stateMachine.Size() == 0u);
    return ok;
}

bool StateMachineTest::TestInitialise() {
    using namespace MARTe;
    const char8 * const config1 = ""
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +A = {"
            "        Class = ReferenceContainer"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"B\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +ENTER = {"
            "            Class = ReferenceContainer"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "    +E = {"
            "        Class = ReferenceContainer"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "}";

    StreamString configStream = config1;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StreamString parserErr;
    StandardParser parser(configStream, cdb, &parserErr);
    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");
    ok &= stateMachine.IsValid();
    return ok;
}

bool StateMachineTest::TestInitialise_False_NoStates() {
    using namespace MARTe;
    const char8 * const config1 = ""
            "+StateMachine = {"
            "    Class = StateMachine"
            "}";

    StreamString configStream = config1;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StreamString parserErr;
    StandardParser parser(configStream, cdb, &parserErr);
    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    return !ok;
}

bool StateMachineTest::TestInitialise_False_NoEvents() {
    using namespace MARTe;
    const char8 * const config1 = ""
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +A = {"
            "        Class = ReferenceContainer"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"B\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +ENTER = {"
            "            Class = ReferenceContainer"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "    }"
            "    +E = {"
            "        Class = ReferenceContainer"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "}";

    StreamString configStream = config1;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StreamString parserErr;
    StandardParser parser(configStream, cdb, &parserErr);
    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    return !ok;
}

bool StateMachineTest::TestInitialise_False_NextState() {
    using namespace MARTe;
    const char8 * const config1 = ""
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +A = {"
            "        Class = ReferenceContainer"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"C\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +ENTER = {"
            "            Class = ReferenceContainer"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "    +E = {"
            "        Class = ReferenceContainer"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "}";

    StreamString configStream = config1;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StreamString parserErr;
    StandardParser parser(configStream, cdb, &parserErr);
    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    return !ok;
}

bool StateMachineTest::TestInitialise_False_NextStateError() {
    using namespace MARTe;
    const char8 * const config1 = ""
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +A = {"
            "        Class = ReferenceContainer"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"B\""
            "            NextStateError = \"ERROR\""
            "            Timeout = 0"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +ENTER = {"
            "            Class = ReferenceContainer"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "    +E = {"
            "        Class = ReferenceContainer"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "}";

    StreamString configStream = config1;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StreamString parserErr;
    StandardParser parser(configStream, cdb, &parserErr);
    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    return !ok;
}

bool StateMachineTest::TestEventTriggered() {
    using namespace MARTe;
    const char8 * const config1 = ""
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +A = {"
            "        Class = ReferenceContainer"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"B\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver1\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver2\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +ENTER = {"
            "            Class = ReferenceContainer"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver3\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver4\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver4\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "    }"
            "    +E = {"
            "        Class = ReferenceContainer"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "}";

    StreamString configStream = config1;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StreamString parserErr;
    StandardParser parser(configStream, cdb, &parserErr);
    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachineTestMessageReceiver> receiver1 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver2 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver3 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver4 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    receiver1->SetName("Receiver1");
    receiver2->SetName("Receiver2");
    receiver3->SetName("Receiver3");
    receiver4->SetName("Receiver4");

    ObjectRegistryDatabase::Instance()->Insert(receiver1);
    ObjectRegistryDatabase::Instance()->Insert(receiver2);
    ObjectRegistryDatabase::Instance()->Insert(receiver3);
    ObjectRegistryDatabase::Instance()->Insert(receiver4);

    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");

    ReferenceT<StateMachineEvent> stateMachineEventA = ObjectRegistryDatabase::Instance()->Find("StateMachine.A.E1");

    ok &= stateMachine.IsValid();
    ok &= stateMachineEventA.IsValid();
    if (ok) {
        ok = (stateMachine->EventTriggered(stateMachineEventA) == ErrorManagement::NoError);
    }
    if (ok) {
        ok = (receiver1->flag == 1);
        ok = (receiver2->flag == 1);
        ok = (receiver3->flag == 1);
        ok = (receiver4->flag == 1);
    }

    ReferenceT<StateMachineEvent> stateMachineEventB = ObjectRegistryDatabase::Instance()->Find("StateMachine.B.E1");

    ok &= stateMachineEventB.IsValid();
    if (ok) {
        ok = (stateMachine->EventTriggered(stateMachineEventB) == ErrorManagement::NoError);
    }
    if (ok) {
        ok = (receiver1->flag == 1);
        ok = (receiver2->flag == 1);
        ok = (receiver3->flag == 1);
        ok = (receiver4->flag == 2);
    }

    return ok;
}

bool StateMachineTest::TestEventTriggered_SendMessage() {
    using namespace MARTe;
    const char8 * const config1 = ""
            "+StateMachine = {"
            "    Class = StateMachine"
            "    +A = {"
            "        Class = ReferenceContainer"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"B\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver1\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver2\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +ENTER = {"
            "            Class = ReferenceContainer"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver3\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver4\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "        +E1 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver4\""
            "                Function = \"ReceiverMethod\""
            "            }"
            "        }"
            "    }"
            "    +E = {"
            "        Class = ReferenceContainer"
            "        +E2 = {"
            "            Class = StateMachineEvent"
            "            NextState = \"A\""
            "            NextStateError = \"E\""
            "            Timeout = 0"
            "        }"
            "    }"
            "}";

    StreamString configStream = config1;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StreamString parserErr;
    StandardParser parser(configStream, cdb, &parserErr);
    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->CleanUp();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachineTestMessageReceiver> receiver1 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver2 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver3 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver4 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    receiver1->SetName("Receiver1");
    receiver2->SetName("Receiver2");
    receiver3->SetName("Receiver3");
    receiver4->SetName("Receiver4");

    ObjectRegistryDatabase::Instance()->Insert(receiver1);
    ObjectRegistryDatabase::Instance()->Insert(receiver2);
    ObjectRegistryDatabase::Instance()->Insert(receiver3);
    ObjectRegistryDatabase::Instance()->Insert(receiver4);

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;
    cdb2.Write("Destination", "StateMachine");
    cdb2.Write("Function", "E1");
    ok &= msg->Initialise(cdb2);
    MessageI::SendMessage(msg, NULL);

    ok = false;
    uint32 counter = 0;
    while (!ok && (counter < 100)) {
        ok = (receiver1->flag == 1);
        ok &= (receiver2->flag == 1);
        ok &= (receiver3->flag == 1);
        ok &= (receiver4->flag == 1);
        counter++;
        Sleep::MSec(10);
    }

    ReferenceT<Message> msg2 = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb3;
    cdb3.Write("Destination", "StateMachine");
    cdb3.Write("Function", "E1");
    ok &= msg2->Initialise(cdb3);
    MessageI::SendMessage(msg2, NULL);

    if (ok) {
        ok = !ok;
    }
    counter = 0;
    while (!ok && (counter < 100)) {
        ok = (receiver1->flag == 1);
        ok = (receiver2->flag == 1);
        ok = (receiver3->flag == 1);
        ok = (receiver4->flag == 2);
        counter++;
        Sleep::MSec(10);
    }

    return ok;
}

