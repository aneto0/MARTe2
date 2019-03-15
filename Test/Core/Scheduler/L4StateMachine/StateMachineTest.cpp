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
#include "AdvancedErrorManagement.h"
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

class StateMachineTestMessageReceiverError: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

StateMachineTestMessageReceiverError    () {
        MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter = MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter>(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        InstallMessageFilter(filter);
        flag = 0;
    }

    MARTe::ErrorManagement::ErrorType ReceiverMethod(MARTe::ReferenceContainer& ref) {
        flag++;
        return MARTe::ErrorManagement::FatalError;
    }

    MARTe::int32 flag;

};
CLASS_REGISTER(StateMachineTestMessageReceiverError, "1.0")
CLASS_METHOD_REGISTER(StateMachineTestMessageReceiverError, ReceiverMethod)

class StateMachineTestMessageReceiverTimeout: public MARTe::Object, public MARTe::QueuedMessageI {
public:
    CLASS_REGISTER_DECLARATION()StateMachineTestMessageReceiverTimeout() {
        MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter> filter = MARTe::ReferenceT<MARTe::RegisteredMethodsMessageFilter>(MARTe::GlobalObjectsDatabase::Instance()->GetStandardHeap());
        filter->SetDestination(this);
        InstallMessageFilter(filter);
        flag = 0;
        Start();
    }

    virtual ~StateMachineTestMessageReceiverTimeout() {
        Stop();
        Stop();
    }

    MARTe::ErrorManagement::ErrorType ReceiverMethod(MARTe::ReferenceContainer& ref) {
        flag = 1;
        while(flag == 1) {
            MARTe::Sleep::Sec(0.1);
        }
        return MARTe::ErrorManagement::NoError;
    }

    MARTe::int32 flag;

};
CLASS_REGISTER(StateMachineTestMessageReceiverTimeout, "1.0")
CLASS_METHOD_REGISTER(StateMachineTestMessageReceiverTimeout, ReceiverMethod)
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");
    ok &= stateMachine.IsValid();
    stateMachine->Stop();
    stateMachine->Stop();
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
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

    stateMachine->Stop();
    stateMachine->Stop();
    return ok;
}

bool StateMachineTest::TestGetState() {
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
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
    ReferenceT<ReferenceContainer> stateA = ObjectRegistryDatabase::Instance()->Find("StateMachine.A");
    ReferenceT<StateMachineEvent> stateMachineEventA = ObjectRegistryDatabase::Instance()->Find("StateMachine.A.E1");

    ok &= stateMachine.IsValid();
    ok &= stateA.IsValid();
    ok &= stateMachineEventA.IsValid();
    if (ok) {
        ok &= (stateMachine->GetCurrentState() == stateA);
        ok &= (stateMachine->GetCurrentStateStatus() == StateMachine::Executing);
    }
    if (ok) {
        ok = (stateMachine->EventTriggered(stateMachineEventA) == ErrorManagement::NoError);
    }
    if (ok) {
        ok = (receiver1->flag == 1);
        ok = (receiver2->flag == 1);
        ok = (receiver3->flag == 1);
        ok = (receiver4->flag == 1);
    }

    ReferenceT<ReferenceContainer> stateB = ObjectRegistryDatabase::Instance()->Find("StateMachine.B");
    ReferenceT<StateMachineEvent> stateMachineEventB = ObjectRegistryDatabase::Instance()->Find("StateMachine.B.E1");

    ok &= stateMachineEventB.IsValid();
    if (ok) {
        ok &= (stateMachine->GetCurrentState() == stateB);
        ok &= (stateMachine->GetCurrentStateStatus() == StateMachine::Executing);
    }
    if (ok) {
        ok = (stateMachine->EventTriggered(stateMachineEventB) == ErrorManagement::NoError);
    }
    if (ok) {
        ok = (receiver1->flag == 1);
        ok = (receiver2->flag == 1);
        ok = (receiver3->flag == 1);
        ok = (receiver4->flag == 2);
    }

    stateMachine->Stop();
    stateMachine->Stop();
    return ok;
}

bool StateMachineTest::TestGetStateStatus() {
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
            "                Mode = \"ExpectsReply\""
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
            "                Mode = \"ExpectsReply\""
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
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver5\""
            "                Function = \"ReceiverMethod\""
            "            }"
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachineTestMessageReceiver> receiver1 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiverTimeout> receiver2 = ReferenceT<StateMachineTestMessageReceiverTimeout>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiverTimeout> receiver3 = ReferenceT<StateMachineTestMessageReceiverTimeout>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver4 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver5 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    receiver1->SetName("Receiver1");
    receiver2->SetName("Receiver2");
    receiver3->SetName("Receiver3");
    receiver4->SetName("Receiver4");
    receiver5->SetName("Receiver5");

    ObjectRegistryDatabase::Instance()->Insert(receiver1);
    ObjectRegistryDatabase::Instance()->Insert(receiver2);
    ObjectRegistryDatabase::Instance()->Insert(receiver3);
    ObjectRegistryDatabase::Instance()->Insert(receiver4);
    ObjectRegistryDatabase::Instance()->Insert(receiver5);

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;
    cdb2.Write("Destination", "StateMachine");
    cdb2.Write("Function", "E1");
    ok &= msg->Initialise(cdb2);
    MessageI::SendMessage(msg, NULL);

    bool done = false;
    uint32 counter = 0;
    while (!done && (counter < 500)) {
        done = (receiver2->flag == 1);
        counter++;
        Sleep::MSec(10);
    }
    ok &= done;

    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");
    ReferenceT<ReferenceContainer> stateA = ObjectRegistryDatabase::Instance()->Find("StateMachine.A");

    ok &= (stateMachine->GetCurrentState() == stateA);
    ok &= (stateMachine->GetCurrentStateStatus() == StateMachine::Exiting);
    receiver2->flag = 2;
    done = false;
    counter = 0;
    while (!done && (counter < 500)) {
        done = (receiver3->flag == 1);
        counter++;
        Sleep::MSec(10);
    }
    ok &= done;
    ReferenceT<ReferenceContainer> stateB = ObjectRegistryDatabase::Instance()->Find("StateMachine.B");
    ok &= (stateMachine->GetCurrentState() == stateB);
    ok &= (stateMachine->GetCurrentStateStatus() == StateMachine::Entering);
    receiver3->flag = 2;
    done = false;
    counter = 0;
    while (!done && (counter < 500)) {
        done = (receiver4->flag == 1);
        counter++;
        Sleep::MSec(10);
    }
    ok &= done;
    done = false;
    counter = 0;
    while (!done && (counter < 500)) {
        done = (stateMachine->GetCurrentState() == stateB);
        done &= (stateMachine->GetCurrentStateStatus() == StateMachine::Executing);
    }
    ok &= done;

    receiver2->Stop();
    receiver2->Stop();
    receiver3->Stop();
    receiver3->Stop();
    stateMachine->Stop();
    stateMachine->Stop();
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
            "                ReplyTimeout = 10000"
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
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
    while (!ok && (counter < 500)) {
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
    while (!ok && (counter < 500)) {
        ok = (receiver1->flag == 1);
        ok &= (receiver2->flag == 1);
        ok &= (receiver3->flag == 1);
        ok &= (receiver4->flag == 2);
        counter++;
        Sleep::MSec(10);
    }

    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");
    stateMachine->Stop();
    stateMachine->Stop();
    return ok;
}

bool StateMachineTest::TestEventTriggered_SendMessage_WaitReply() {
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
            "                Mode = \"ExpectsReply\""
            "            }"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver2\""
            "                Function = \"ReceiverMethod\""
            "                Mode = \"ExpectsReply\""
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
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
    while (!ok && (counter < 500)) {
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
    while (!ok && (counter < 500)) {
        ok = (receiver1->flag == 1);
        ok &= (receiver2->flag == 1);
        ok &= (receiver3->flag == 1);
        ok &= (receiver4->flag == 2);
        counter++;
        Sleep::MSec(10);
    }

    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");
    stateMachine->Stop();
    stateMachine->Stop();
    return ok;
}

bool StateMachineTest::TestEventTriggered_SendMessage_GoToError() {
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
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver5\""
            "                Function = \"ReceiverMethod\""
            "            }"
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachineTestMessageReceiverError> receiver1 = ReferenceT<StateMachineTestMessageReceiverError>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiverError> receiver2 = ReferenceT<StateMachineTestMessageReceiverError>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiverError> receiver3 = ReferenceT<StateMachineTestMessageReceiverError>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiverError> receiver4 = ReferenceT<StateMachineTestMessageReceiverError>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver5 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    receiver1->SetName("Receiver1");
    receiver2->SetName("Receiver2");
    receiver3->SetName("Receiver3");
    receiver4->SetName("Receiver4");
    receiver5->SetName("Receiver5");

    ObjectRegistryDatabase::Instance()->Insert(receiver1);
    ObjectRegistryDatabase::Instance()->Insert(receiver2);
    ObjectRegistryDatabase::Instance()->Insert(receiver3);
    ObjectRegistryDatabase::Instance()->Insert(receiver4);
    ObjectRegistryDatabase::Instance()->Insert(receiver5);

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;
    cdb2.Write("Destination", "StateMachine");
    cdb2.Write("Function", "E1");
    ok &= msg->Initialise(cdb2);
    MessageI::SendMessage(msg, NULL);

    bool done = false;
    uint32 counter = 0;
    while (!done && (counter < 500)) {
        done = (receiver1->flag == 1);
        counter++;
        Sleep::MSec(10);
    }
    ok &= done;
    ReferenceT<Message> msg2 = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb3;
    cdb3.Write("Destination", "StateMachine");
    cdb3.Write("Function", "E2");
    ok &= msg2->Initialise(cdb3);
    MessageI::SendMessage(msg2, NULL);

    counter = 0;
    done = false;
    while (!done && (counter < 500)) {
        done = (receiver5->flag == 1);
        counter++;
        Sleep::MSec(10);
    }
    ok &= done;

    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");
    stateMachine->Stop();
    stateMachine->Stop();
    return ok;
}

bool StateMachineTest::TestEventTriggered_SendMessage_GoToError_Timeout() {
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
            "            Timeout = 10"
            "            +M1 = {"
            "                Class = Message"
            "                Destination = \"Receiver1\""
            "                Function = \"ReceiverMethod\""
            "                Mode = \"ExpectsReply\""
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
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver5\""
            "                Function = \"ReceiverMethod\""
            "            }"
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachineTestMessageReceiverTimeout> receiver1 = ReferenceT<StateMachineTestMessageReceiverTimeout>(
            GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver2 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver3 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver4 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver5 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    receiver1->SetName("Receiver1");
    receiver2->SetName("Receiver2");
    receiver3->SetName("Receiver3");
    receiver4->SetName("Receiver4");
    receiver5->SetName("Receiver5");

    ObjectRegistryDatabase::Instance()->Insert(receiver1);
    ObjectRegistryDatabase::Instance()->Insert(receiver2);
    ObjectRegistryDatabase::Instance()->Insert(receiver3);
    ObjectRegistryDatabase::Instance()->Insert(receiver4);
    ObjectRegistryDatabase::Instance()->Insert(receiver5);

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;
    cdb2.Write("Destination", "StateMachine");
    cdb2.Write("Function", "E1");
    ok &= msg->Initialise(cdb2);
    MessageI::SendMessage(msg, NULL);

    bool done = false;
    uint32 counter = 0;
    while (!done && (counter < 500)) {
        done = (receiver1->flag == 1);
        counter++;
        Sleep::MSec(10);
    }
    ok &= done;
    done = false;
    counter = 0;
    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");
    ReferenceT<ReferenceContainer> errorState = ObjectRegistryDatabase::Instance()->Find("StateMachine.E");
    while (!done && (counter < 500)) {
        done = (stateMachine->GetCurrentState() == errorState);
        done &= (stateMachine->GetCurrentStateStatus() == StateMachine::Executing);
        counter++;
        Sleep::MSec(10);
    }

    ReferenceT<Message> msg2 = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb3;
    cdb3.Write("Destination", "StateMachine");
    cdb3.Write("Function", "E2");
    ok &= msg2->Initialise(cdb3);
    MessageI::SendMessage(msg2, NULL);
    ok &= done;
    done = false;
    counter = 0;
    while (!done && (counter < 500)) {
        done = (receiver5->flag == 1);
        counter++;
        Sleep::MSec(10);
    }
    ok &= done;
    receiver1->flag = 2;
    Sleep::MSec(100);

    receiver1->Stop();
    receiver1->Stop();
    stateMachine->Stop();
    stateMachine->Stop();

    return ok;
}

bool StateMachineTest::TestEventTriggered_SendMessage_PingPong() {
    using namespace MARTe;
    const char8 * const config1 = ""
            "+StateMachineA = {"
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
            "                Destination = \"StateMachineB\""
            "                Function = \"E1\""
            "            }"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +ENTER = {"
            "            Class = ReferenceContainer"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver1\""
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
            "                Destination = \"StateMachineB\""
            "                Function = \"E1\""
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
            "}"
            "+StateMachineB = {"
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
            "                Destination = \"StateMachineA\""
            "                Function = \"E1\""
            "            }"
            "        }"
            "    }"
            "    +B = {"
            "        Class = ReferenceContainer"
            "        +ENTER = {"
            "            Class = ReferenceContainer"
            "            +M2 = {"
            "                Class = Message"
            "                Destination = \"Receiver2\""
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachineTestMessageReceiver> receiver1 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ReferenceT<StateMachineTestMessageReceiver> receiver2 = ReferenceT<StateMachineTestMessageReceiver>(GlobalObjectsDatabase::Instance()->GetStandardHeap());

    receiver1->SetName("Receiver1");
    receiver2->SetName("Receiver2");

    ObjectRegistryDatabase::Instance()->Insert(receiver1);
    ObjectRegistryDatabase::Instance()->Insert(receiver2);

    ReferenceT<Message> msg = ReferenceT<Message>(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb2;
    cdb2.Write("Destination", "StateMachineA");
    cdb2.Write("Function", "E1");
    ok &= msg->Initialise(cdb2);
    MessageI::SendMessage(msg, NULL);

    ok = false;
    uint32 counter = 0;
    ReferenceT<StateMachine> stateMachineA = ObjectRegistryDatabase::Instance()->Find("StateMachineA");
    ReferenceT<StateMachine> stateMachineB = ObjectRegistryDatabase::Instance()->Find("StateMachineB");
    ReferenceT<ReferenceContainer> stateMachineAStateA = ObjectRegistryDatabase::Instance()->Find("StateMachineA.A");
    ReferenceT<ReferenceContainer> stateMachineBStateA = ObjectRegistryDatabase::Instance()->Find("StateMachineB.A");
    while (!ok && (counter < 500)) {
        ok = (receiver1->flag == 1);
        ok &= (receiver2->flag == 1);
        ok &= (stateMachineA->GetCurrentState() == stateMachineAStateA);
        ok &= (stateMachineB->GetCurrentState() == stateMachineBStateA);
        counter++;
        Sleep::MSec(10);
    }
    stateMachineA->Stop();
    stateMachineA->Stop();
    stateMachineB->Stop();
    stateMachineB->Stop();
    return ok;
}

bool StateMachineTest::TestExportData() {
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
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, parserErr.Buffer());
        return false;
    }

    ObjectRegistryDatabase::Instance()->Purge();
    ok &= ObjectRegistryDatabase::Instance()->Initialise(cdb);
    ReferenceT<StateMachine> stateMachine = ObjectRegistryDatabase::Instance()->Find("StateMachine");

    ConfigurationDatabase cdbe;
    stateMachine->ExportData(cdbe);
    cdbe.MoveToRoot();
    StreamString currentStateName;
    ok &= cdbe.Read("CurrentState", currentStateName);
    ok &= (currentStateName == "A");

    stateMachine->Stop();
    stateMachine->Stop();
    return ok;
}

