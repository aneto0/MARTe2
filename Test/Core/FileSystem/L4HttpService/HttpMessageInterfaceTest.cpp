/**
 * @file HttpMessageInterfaceTest.cpp
 * @brief Source file for class HttpMessageInterfaceTest
 * @date 18/03/2019
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
 * the class HttpMessageInterfaceTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CLASSMETHODREGISTER.h"
#include "Directory.h"
#include "File.h"
#include "HttpClient.h"
#include "HttpMessageInterface.h"
#include "HttpMessageInterfaceTest.h"
#include "HttpObjectBrowser.h"
#include "HttpService.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class HttpMessageInterfaceTestObject: public MARTe::Object, public MARTe::MessageI {
public:
    CLASS_REGISTER_DECLARATION()

HttpMessageInterfaceTestObject    () {
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
CLASS_REGISTER(HttpMessageInterfaceTestObject, "1.0")
CLASS_METHOD_REGISTER(HttpMessageInterfaceTestObject, ReceiverMethod)

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool HttpMessageInterfaceTest::TestInitialise() {
    using namespace MARTe;
    StreamString cfg = ""
            "+TestObj = {\n"
            "    Class = HttpMessageInterfaceTestObject\n"
            "}\n"
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +MessageInterface1 = {\n"
            "        Class = HttpMessageInterface\n"
            "        +Msg1 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "        +Msg2 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpMessageInterfaceTest::TestInitialise_False_Not_Only_Messages() {
    using namespace MARTe;
    StreamString cfg = ""
            "+TestObj = {\n"
            "    Class = HttpMessageInterfaceTestObject\n"
            "}\n"
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +MessageInterface1 = {\n"
            "        Class = HttpMessageInterface\n"
            "        +Msg1 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "        +Msg2 = {\n"
            "            Class = Object\n"
            "        }\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = !ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpMessageInterfaceTest::TestGetAsStructuredData() {
    using namespace MARTe;
    StreamString cfg = ""
            "+TestObj = {\n"
            "    Class = HttpMessageInterfaceTestObject\n"
            "}\n"
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +MessageInterface1 = {\n"
            "        Class = HttpMessageInterface\n"
            "        +Msg1 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "            Mode = ExpectsReply"
            "        }\n"
            "        +Msg2 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/MessageInterface1");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = "1E\r\n{\n\r\"Name\": \"MessageInterface1\"\r\n"
            "20\r\n,\n\r\"Class\": \"HttpMessageInterfac\r\n2\r\ne\"\r\n"
            "9\r\n\n\r,\"0\": {\r\n10\r\n\n\r\"Name\": \"Msg1\"\r\n"
            "15\r\n,\n\r\"Class\": \"Message\"\r\n"
            "13\r\n,\n\r\"IsContainer\": 1\r\n3\r\n\n\r}\r\n"
            "9\r\n,\n\r\"1\": {\r\n10\r\n\n\r\"Name\": \"Msg2\"\r\n"
            "15\r\n,\n\r\"Class\": \"Message\"\r\n"
            "13\r\n,\n\r\"IsContainer\": 1\r\n3\r\n\n\r}\r\n1\r\n}\r\n0\r\n\r\n";
    if (ok) {
        ok = (reply == expectedReply);
    }

    ReferenceT<HttpMessageInterfaceTestObject> target = ObjectRegistryDatabase::Instance()->Find("TestObj");
    if (ok) {
        ok = target.IsValid();
    }
    test.SetServerUri("/MessageInterface1?msg=Msg1");
    StreamString reply2;
    if (ok) {
        ok = test.HttpExchange(reply2, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (reply2 == "A\r\n{\n\r\"OK\": 1\r\n1\r\n}\r\n0\r\n\r\n");
    }
    if (ok) {
        ok = (target->flag == 1);
    }
    test.SetServerUri("/MessageInterface1?msg=Msg2");
    StreamString reply3;
    if (ok) {
        ok = test.HttpExchange(reply3, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (reply3 == "A\r\n{\n\r\"OK\": 1\r\n1\r\n}\r\n0\r\n\r\n");
    }
    if (ok) {
        ok = (target->flag == 2);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpMessageInterfaceTest::TestGetAsText() {
    using namespace MARTe;
    StreamString cfg = ""
            "+TestObj = {\n"
            "    Class = HttpMessageInterfaceTestObject\n"
            "}\n"
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 1\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +MessageInterface1 = {\n"
            "        Class = HttpMessageInterface\n"
            "        +Msg1 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "        +Msg2 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/MessageInterface1");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = "0\r\n\r\n";
    if (ok) {
        ok = (reply == expectedReply);
    }

    ReferenceT<HttpMessageInterfaceTestObject> target = ObjectRegistryDatabase::Instance()->Find("TestObj");
    if (ok) {
        ok = target.IsValid();
    }
    test.SetServerUri("/MessageInterface1?msg=Msg1");
    StreamString reply2;
    if (ok) {
        ok = test.HttpExchange(reply2, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (reply2 == "0\r\n\r\n");
    }
    if (ok) {
        ok = (target->flag == 1);
    }
    test.SetServerUri("/MessageInterface1?msg=Msg2");
    StreamString reply3;
    if (ok) {
        ok = test.HttpExchange(reply3, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (reply3 == "0\r\n\r\n");
    }
    if (ok) {
        ok = (target->flag == 2);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpMessageInterfaceTest::TestGetAsStructuredData_Message_Cannot_Be_Sent() {
    using namespace MARTe;
    StreamString cfg = ""
            "+TestObj = {\n"
            "    Class = HttpMessageInterfaceTestObject\n"
            "}\n"
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +MessageInterface1 = {\n"
            "        Class = HttpMessageInterface\n"
            "        +Msg1 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "        +Msg2 = {\n"
            "            Class = Message\n"
            "            Destination = TestObjThatDoesNotExist\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/MessageInterface1?msg=Msg2");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (reply == "A\r\n{\n\r\"OK\": 0\r\n1\r\n}\r\n0\r\n\r\n");
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpMessageInterfaceTest::TestGetAsText_Message_Cannot_Be_Sent() {
    using namespace MARTe;
    StreamString cfg = ""
            "+TestObj = {\n"
            "    Class = HttpMessageInterfaceTestObject\n"
            "}\n"
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 1\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +MessageInterface1 = {\n"
            "        Class = HttpMessageInterface\n"
            "        +Msg1 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "        +Msg2 = {\n"
            "            Class = Message\n"
            "            Destination = TestObjThatDoesNotExist\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/MessageInterface1?msg=Msg2");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (reply == "0\r\n\r\n");
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpMessageInterfaceTest::TestGetAsStructuredData_Message_Does_Not_Exist() {
    using namespace MARTe;
    StreamString cfg = ""
            "+TestObj = {\n"
            "    Class = HttpMessageInterfaceTestObject\n"
            "}\n"
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +MessageInterface1 = {\n"
            "        Class = HttpMessageInterface\n"
            "        +Msg1 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "        +Msg2 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/MessageInterface1?msg=Msg3");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (reply == "A\r\n{\n\r\"OK\": 0\r\n1\r\n}\r\n0\r\n\r\n");
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpMessageInterfaceTest::TestGetAsText_Message_Does_Not_Exist() {
    using namespace MARTe;
    StreamString cfg = ""
            "+TestObj = {\n"
            "    Class = HttpMessageInterfaceTestObject\n"
            "}\n"
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 1\n"
            "    WebRoot = HttpObjectBrowser1\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +MessageInterface1 = {\n"
            "        Class = HttpMessageInterface\n"
            "        +Msg1 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "        +Msg2 = {\n"
            "            Class = Message\n"
            "            Destination = TestObj\n"
            "            Function = ReceiverMethod\n"
            "        }\n"
            "    }\n"
            "}\n";

    cfg.Seek(0LLU);
    StreamString err;
    ConfigurationDatabase cdb;
    StandardParser parser(cfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        ok = cdb.MoveToRoot();
    }
    if (ok) {
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/MessageInterface1?msg=Msg3");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (reply == "0\r\n\r\n");
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}
