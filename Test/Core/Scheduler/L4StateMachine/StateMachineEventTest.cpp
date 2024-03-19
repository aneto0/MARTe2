/**
 * @file StateMachineEventTest.cpp
 * @brief Source file for class StateMachineEventTest
 * @date 11/10/2016
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
 * the class StateMachineEventTest (public, protected, and private). Be aware that some 
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
#include "CLASSMETHODREGISTER.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "StandardParser.h"
#include "StateMachine.h"
#include "StateMachineEvent.h"
#include "StateMachineEventTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class StateMachineEventTestMessageReceiver: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

StateMachineEventTestMessageReceiver    () {
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
CLASS_REGISTER(StateMachineEventTestMessageReceiver, "1.0")
CLASS_METHOD_REGISTER(StateMachineEventTestMessageReceiver, ReceiverMethod)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool StateMachineEventTest::TestDefaultConstructor() {
    using namespace MARTe;
    StateMachineEvent event;
    bool ok = (event.GetTransitionTimeout() == TTInfiniteWait);
    return ok;
}

bool StateMachineEventTest::TestGetTransitionTimeout() {
    return TestInitialise();
}

bool StateMachineEventTest::TestGetNextState() {
    return TestInitialise();
}

bool StateMachineEventTest::TestSetStateMachine() {
    return TestConsumeMessage();
}

bool StateMachineEventTest::TestInitialise() {
    using namespace MARTe;
    StateMachineEvent event;
    ConfigurationDatabase cdb;
    cdb.Write("NextState", "B");
    cdb.Write("NextStateError", "E");
    cdb.Write("Timeout", 0);
    bool ok = event.Initialise(cdb);
    ok &= (StringHelper::Compare(event.GetNextState(), "B") == 0);
    ok &= (StringHelper::Compare(event.GetNextStateError(), "E") == 0);
    ok &= (event.GetTransitionTimeout() == TTInfiniteWait);
    return ok;
}

bool StateMachineEventTest::TestInitialise_NoZeroTimeout() {
    using namespace MARTe;
    StateMachineEvent event;
    ConfigurationDatabase cdb;
    cdb.Write("NextState", "B");
    cdb.Write("NextStateError", "E");
    cdb.Write("Timeout", 10);
    bool ok = event.Initialise(cdb);
    ok &= (StringHelper::Compare(event.GetNextState(), "B") == 0);
    ok &= (StringHelper::Compare(event.GetNextStateError(), "E") == 0);
    ok &= (event.GetTransitionTimeout() == 10);
    return ok;
}

bool StateMachineEventTest::TestGetNextStateError() {
    return TestInitialise();
}

bool StateMachineEventTest::TestInitialise_False_NoNextState() {
    using namespace MARTe;
    StateMachineEvent event;
    ConfigurationDatabase cdb;
    cdb.Write("NextStateError", "E");
    cdb.Write("Timeout", 10);
    return !event.Initialise(cdb);
}

bool StateMachineEventTest::TestInitialise_True_NoNextStateError() {
    using namespace MARTe;
    StateMachineEvent event;
    ConfigurationDatabase cdb;
    cdb.Write("NextState", "B");
    cdb.Write("Timeout", 10);
    bool ok = event.Initialise(cdb);
    ok &= (StringHelper::Compare(event.GetNextState(), "B") == 0);
    ok &= (StringHelper::Compare(event.GetNextStateError(), "ERROR") == 0);
    ok &= (event.GetTransitionTimeout() == 10);
    return ok;
}

bool StateMachineEventTest::TestInitialise_True_NoTimeout() {
    using namespace MARTe;
    StateMachineEvent event;
    ConfigurationDatabase cdb;
    cdb.Write("Code", 0xf);
    cdb.Write("NextState", "B");
    cdb.Write("NextStateError", "E");
    bool ok = event.Initialise(cdb);
    ok &= (StringHelper::Compare(event.GetNextState(), "B") == 0);
    ok &= (StringHelper::Compare(event.GetNextStateError(), "E") == 0);
    ok &= (event.GetTransitionTimeout() == TTInfiniteWait);
    return ok;
}

bool StateMachineEventTest::TestConsumeMessage() {
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

    ReferenceT<StateMachineEventTestMessageReceiver> receiver = ReferenceT<StateMachineEventTestMessageReceiver>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    receiver->SetName("Receiver");

    StreamString configStream = config1;
    configStream.Seek(0);
    ConfigurationDatabase cdb;
    StreamString parserErr;
    StandardParser parser(configStream, cdb, &parserErr);
    bool ok = parser.Parse();
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Insert(receiver);
    ReferenceT<StateMachineEvent> event = ObjectRegistryDatabase::Instance()->Find("StateMachine.A.E1");

    ReferenceT<Message> stateMachineMessage = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase msgCdb;
    msgCdb.Write("Destination", "StateMachine");
    msgCdb.Write("Function", "E1");
    ok &= stateMachineMessage->Initialise(msgCdb);
    ok &= (event->ConsumeMessage(stateMachineMessage) == ErrorManagement::NoError);
    ok &= (receiver->flag == 2);

    ReferenceT<Message> stateMachineMessage2 = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase msgCdb2;
    msgCdb2.Write("Destination", "StateMachine");
    msgCdb2.Write("Function", "E2");
    ok &= stateMachineMessage2->Initialise(msgCdb2);
    ok &= (event->ConsumeMessage(stateMachineMessage2) == ErrorManagement::RecoverableError);

    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");
    stateMachine->Stop();
    stateMachine->Stop();
    return ok;

}
