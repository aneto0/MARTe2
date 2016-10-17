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
