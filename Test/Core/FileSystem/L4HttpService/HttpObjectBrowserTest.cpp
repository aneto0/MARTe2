/**
 * @file HttpObjectBrowserTest.cpp
 * @brief Source file for class HttpObjectBrowserTest
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
 * the class HttpObjectBrowserTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Directory.h"
#include "File.h"
#include "HttpClient.h"
#include "HttpObjectBrowser.h"
#include "HttpObjectBrowserTest.h"
#include "HttpService.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HttpObjectBrowserTestRealm: public MARTe::Object, public MARTe::HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION();

    HttpObjectBrowserTestRealm();
    virtual ~HttpObjectBrowserTestRealm();

    virtual bool Validate(const MARTe::char8 * key, MARTe::int32 command, MARTe::uint32 ipNumber);

    virtual bool DigestSecurityNeeded();

    virtual bool GetAuthenticationRequest(MARTe::StreamString &message);

    bool valid;
};

HttpObjectBrowserTestRealm::HttpObjectBrowserTestRealm() {
    valid = false;
}

HttpObjectBrowserTestRealm::~HttpObjectBrowserTestRealm() {

}

bool HttpObjectBrowserTestRealm::Validate(const MARTe::char8 * key, MARTe::int32 command, MARTe::uint32 ipNumber) {
    return valid;
}

bool HttpObjectBrowserTestRealm::DigestSecurityNeeded() {
    return true;
}

bool HttpObjectBrowserTestRealm::GetAuthenticationRequest(MARTe::StreamString &message) {
    message = "Basic 12345678";
    message.Seek(0LLU);
    return true;
}

CLASS_REGISTER(HttpObjectBrowserTestRealm, "1.0")
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool HttpObjectBrowserTest::TestInitialise() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", ".");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_ObjectRegistryDatabase_Root() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", "/");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_Self_Root() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", ".");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_Other_Root() {
    using namespace MARTe;
    StreamString cfg = ""
            "+ARoot = {\n"
            "    Class = ReferenceContainer\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = ARoot"
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

bool HttpObjectBrowserTest::TestInitialise_False_No_Root() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = !httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_False_Bad_Root_1() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", "?");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = !httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_False_Bad_Root_2() {
    using namespace MARTe;
    ConfigurationDatabase cdb;
    cdb.Write("Root", "HttpObjectBrowserTestARoot");

    ReferenceT<HttpObjectBrowser> httpObjectBrowser(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    bool ok = !httpObjectBrowser->Initialise(cdb);
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestInitialise_Realm() {
    using namespace MARTe;
    StreamString cfg = ""
            "+ARoot = {\n"
            "    Class = ReferenceContainer\n"
            "}\n"
            "+ARealm = {\n"
            "    Class = HttpObjectBrowserTestRealm\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = ARoot\n"
            "    Realm = ARealm\n"
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

bool HttpObjectBrowserTest::TestInitialise_False_Realm() {
    using namespace MARTe;
    StreamString cfg = ""
            "+ARoot = {\n"
            "    Class = ReferenceContainer\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = ARoot\n"
            "    Realm = ARoot\n"
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

bool HttpObjectBrowserTest::TestGetAsStructuredData_This_Self() {
    using namespace MARTe;
    StreamString cfg = ""
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
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
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
    test.SetServerUri("/");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = ""
            "1F\r\n"
            "{\n\r"
            "\"Name\": \"HttpObjectBrowser1\"\r\n"
            "1F\r\n"
            ",\n\r"
            "\"Class\": \"HttpObjectBrowser\"\r\n"
            "9\r\n"
            "\n\r"
            ",\"0\": {\r\n"
            "12\r\n"
            "\n\r"
            "\"Name\": \"AChild\"\r\n"
            "20\r\n"
            ",\n\r"
            "\"Class\": \"ReferenceContainer\"\r\n"
            "13\r\n"
            ",\n\r"
            "\"IsContainer\": 1\r\n"
            "3\r\n"
            "\n\r"
            "}\r\n"
            "1\r\n"
            "}\r\n"
            "0\r\n"
            "\r\n";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsStructuredData_This_ObjectRegistryDatabase() {
    using namespace MARTe;
    StreamString cfg = ""
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
            "    Root = \"/\""
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
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
    test.SetServerUri("HttpObjectBrowser1");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = ""
            "1F\r\n"
            "{\n\r"
            "\"Name\": \"HttpObjectBrowser1\"\r\n"
            "1F\r\n"
            ",\n\r"
            "\"Class\": \"HttpObjectBrowser\"\r\n"
            "9\r\n"
            "\n\r"
            ",\"0\": {\r\n"
            "18\r\n"
            "\n\r"
            "\"Name\": \"HttpService1\"\r\n"
            "19\r\n"
            ",\n\r"
            "\"Class\": \"HttpService\"\r\n"
            "13\r\n"
            ",\n\r"
            "\"IsContainer\": 1\r\n"
            "3\r\n"
            "\n\r"
            "}\r\n"
            "9\r\n"
            ",\n\r"
            "\"1\": {\r\n"
            "1E\r\n"
            "\n\r"
            "\"Name\": \"HttpObjectBrowser1\"\r\n"
            "1F\r\n"
            ",\n\r"
            "\"Class\": \"HttpObjectBrowser\"\r\n"
            "13\r\n"
            ",\n\r"
            "\"IsContainer\": 1\r\n"
            "3\r\n"
            "\n\r"
            "}\r\n"
            "1\r\n"
            "}\r\n"
            "0\r\n"
            "\r\n";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsStructuredData_This_Other() {
    using namespace MARTe;
    StreamString cfg = ""
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser2\n"
            "}\n"
            "+RC1 = {\n"
            "    Class = ReferenceContainer\n"
            "    +HttpObjectBrowser1 = {\n"
            "        Class = HttpObjectBrowser"
            "        Root = \".\""
            "        +AChild = {\n"
            "             Class = ReferenceContainer\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+HttpObjectBrowser2 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \"RC1\""
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
    test.SetServerUri("HttpObjectBrowser1");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = ""
            "1F\r\n"
            "{\n\r"
            "\"Name\": \"HttpObjectBrowser1\"\r\n"
            "1F\r\n"
            ",\n\r"
            "\"Class\": \"HttpObjectBrowser\"\r\n"
            "9\r\n"
            "\n\r"
            ",\"0\": {\r\n"
            "12\r\n"
            "\n\r"
            "\"Name\": \"AChild\"\r\n"
            "20\r\n"
            ",\n\r"
            "\"Class\": \"ReferenceContainer\"\r\n"
            "13\r\n"
            ",\n\r"
            "\"IsContainer\": 1\r\n"
            "3\r\n"
            "\n\r"
            "}\r\n"
            "1\r\n"
            "}\r\n"
            "0\r\n"
            "\r\n";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsStructuredData_Other_Not_HttpObjectBrowser() {
    using namespace MARTe;
    StreamString cfg = ""
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser2\n"
            "}\n"
            "+RC1 = {\n"
            "    Class = ReferenceContainer\n"
            "    +HttpObjectBrowser1 = {\n"
            "        Class = HttpObjectBrowser"
            "        Root = \".\""
            "        +AChild = {\n"
            "             Class = ReferenceContainer\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+HttpObjectBrowser2 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \"RC1\""
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
    test.SetServerUri("HttpObjectBrowser1/AChild");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = ""
            "13\r\n"
            "{\n\r"
            "\"Name\": \"AChild\""
            "\r\n"
            "20\r\n"
            ",\n\r"
            "\"Class\": \"ReferenceContainer\"\r\n"
            "1\r\n"
            "}\r\n"
            "0\r\n"
            "\r\n";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsStructuredData_Other_Not_HttpObjectBrowser_Long_Path() {
    using namespace MARTe;
    StreamString cfg = ""
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser2\n"
            "}\n"
            "+RC1 = {\n"
            "    Class = ReferenceContainer\n"
            "    +HttpObjectBrowser1 = {\n"
            "        Class = HttpObjectBrowser"
            "        Root = \".\""
            "        +AChild = {\n"
            "             Class = ReferenceContainer\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+HttpObjectBrowser2 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \"RC1\""
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
    test.SetServerUri("HttpObjectBrowser1/AChild/ANonExistentChild");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = ""
            "13\r\n"
            "{\n\r"
            "\"Name\": \"AChild\""
            "\r\n"
            "20\r\n"
            ",\n\r"
            "\"Class\": \"ReferenceContainer\"\r\n"
            "1\r\n"
            "}\r\n"
            "0\r\n"
            "\r\n";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsStructuredData_InvalidTarget() {
    using namespace MARTe;
    StreamString cfg = ""
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 0\n"
            "    WebRoot = HttpObjectBrowser2\n"
            "}\n"
            "+RC1 = {\n"
            "    Class = ReferenceContainer\n"
            "    +HttpObjectBrowser1 = {\n"
            "        Class = HttpObjectBrowser"
            "        Root = \".\""
            "        +AChild = {\n"
            "             Class = ReferenceContainer\n"
            "        }\n"
            "    }\n"
            "}\n"
            "+HttpObjectBrowser2 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \"RC1\""
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
    test.SetServerUri("ANonExistentChild");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = "";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsText_This() {
    using namespace MARTe;
    StreamString cfg = ""
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
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
            "    }\n"
            "    +DirectoryResource = {\n"
            "        Class = HttpDirectoryResource\n"
            "        BaseDir = \".\""
            "    }\n"
            "}\n";

    File f;
    f.Open("HttpObjectBrowserTest.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
    f.Printf("Test %d", 1);
    f.Flush();
    f.Close();
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
    test.SetServerUri("/?path=HttpObjectBrowserTest.txt");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = "Test 1";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    Directory d("HttpObjectBrowserTest.txt");
    d.Delete();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsText_This_FileNotFound() {
    using namespace MARTe;
    StreamString cfg = ""
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
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
            "    }\n"
            "    +DirectoryResource = {\n"
            "        Class = HttpDirectoryResource\n"
            "        BaseDir = \".\""
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
    test.SetServerUri("/?path=HttpObjectBrowserTestNotFound.txt");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (test.GetHttpProtocol()->GetHttpCommand() == HttpDefinition::HSHCReplyNotFound);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsText_Other() {
    using namespace MARTe;
    StreamString cfg = ""
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 1\n"
            "    WebRoot = HttpObjectBrowser2\n"
            "}\n"
            "+HttpObjectBrowser2 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +HttpObjectBrowser1 = {\n"
            "        Class = HttpObjectBrowser\n"
            "        Root = \".\""
            "        +AChild = {\n"
            "            Class = ReferenceContainer\n"
            "        }\n"
            "        +DirectoryResource = {\n"
            "             Class = HttpDirectoryResource\n"
            "             BaseDir = \".\""
            "        }\n"
            "    }\n"
            "}\n";

    File f;
    f.Open("HttpObjectBrowserTest.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
    f.Printf("Test %d", 2);
    f.Flush();
    f.Close();
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
    test.SetServerUri("/HttpObjectBrowser1?path=HttpObjectBrowserTest.txt");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = "Test 2";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    Directory d("HttpObjectBrowserTest.txt");
    d.Delete();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsText_Other_Not_HttpDataExportI() {
    using namespace MARTe;
    StreamString cfg = ""
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9094\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 1\n"
            "    WebRoot = HttpObjectBrowser2\n"
            "}\n"
            "+HttpObjectBrowser2 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +HttpObjectBrowser1 = {\n"
            "        Class = HttpObjectBrowser\n"
            "        Root = \".\""
            "        +AChild = {\n"
            "            Class = ReferenceContainer\n"
            "        }\n"
            "        +DirectoryResource = {\n"
            "             Class = HttpDirectoryResource\n"
            "             BaseDir = \".\""
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
    test.SetServerUri("/HttpObjectBrowser1/AChild?path=HttpObjectBrowserTest.txt");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (test.GetHttpProtocol()->GetHttpCommand() == HttpDefinition::HSHCReplyNotFound);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpObjectBrowserTest::TestGetAsText_InvalidObject() {
    using namespace MARTe;
    StreamString cfg = ""
            "+HttpService1 = {\n"
            "    Class = HttpService\n"
            "    Port = 9096\n"
            "    Timeout = 0\n"
            "    AcceptTimeout = 100"
            "    MinNumberOfThreads = 1\n"
            "    MaxNumberOfThreads = 8\n"
            "    ListenMaxConnections = 255\n"
            "    IsTextMode = 1\n"
            "    WebRoot = HttpObjectBrowser2\n"
            "}\n"
            "+HttpObjectBrowser2 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\""
            "    +HttpObjectBrowser1 = {\n"
            "        Class = HttpObjectBrowser\n"
            "        Root = \".\""
            "        +AChild = {\n"
            "            Class = ReferenceContainer\n"
            "        }\n"
            "        +DirectoryResource = {\n"
            "             Class = HttpDirectoryResource\n"
            "             BaseDir = \".\""
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
    test.SetServerPort(9096);
    test.SetServerUri("/NotExistent");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (test.GetHttpProtocol()->GetHttpCommand() == HttpDefinition::HSHCReplyNotFound);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool HttpObjectBrowserTest::TestGetAsStructuredData_Realm() {
    using namespace MARTe;
    StreamString cfg = ""
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
            "+ARealm = {\n"
            "    Class = HttpObjectBrowserTestRealm\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\"\n"
            "    Realm = ARealm\n"
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
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
    ReferenceT<HttpObjectBrowserTestRealm> realm = ObjectRegistryDatabase::Instance()->Find("ARealm");
    realm->valid = true;

    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/");
    test.SetAuthorisation("ignore");
    StreamString reply;
    if (ok) {
        //Authenticate
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
        reply = "";
    }
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = ""
            "1F\r\n"
            "{\n\r"
            "\"Name\": \"HttpObjectBrowser1\"\r\n"
            "1F\r\n"
            ",\n\r"
            "\"Class\": \"HttpObjectBrowser\"\r\n"
            "9\r\n"
            "\n\r"
            ",\"0\": {\r\n"
            "12\r\n"
            "\n\r"
            "\"Name\": \"AChild\"\r\n"
            "20\r\n"
            ",\n\r"
            "\"Class\": \"ReferenceContainer\"\r\n"
            "13\r\n"
            ",\n\r"
            "\"IsContainer\": 1\r\n"
            "3\r\n"
            "\n\r"
            "}\r\n"
            "1\r\n"
            "}\r\n"
            "0\r\n"
            "\r\n";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsText_Realm() {
    using namespace MARTe;
    StreamString cfg = ""
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
            "+ARealm = {\n"
            "    Class = HttpObjectBrowserTestRealm\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\"\n"
            "    Realm = ARealm\n"
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
            "    }\n"
            "    +DirectoryResource = {\n"
            "        Class = HttpDirectoryResource\n"
            "        BaseDir = \".\""
            "    }\n"
            "}\n";

    File f;
    f.Open("HttpObjectBrowserTest.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
    f.Printf("Test %d", 1);
    f.Flush();
    f.Close();
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
    ReferenceT<HttpObjectBrowserTestRealm> realm = ObjectRegistryDatabase::Instance()->Find("ARealm");
    realm->valid = true;
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/?path=HttpObjectBrowserTest.txt");
    test.SetAuthorisation("ignore");
    StreamString reply;
    if (ok) {
        //Authenticate
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
        reply = "";
    }
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    const char8 * expectedReply = "Test 1";
    if (ok) {
        ok = (reply == expectedReply);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    Directory d("HttpObjectBrowserTest.txt");
    d.Delete();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsStructuredData_Realm_False() {
    using namespace MARTe;
    StreamString cfg = ""
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
            "+ARealm = {\n"
            "    Class = HttpObjectBrowserTestRealm\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\"\n"
            "    Realm = ARealm\n"
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
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
    ReferenceT<HttpObjectBrowserTestRealm> realm = ObjectRegistryDatabase::Instance()->Find("ARealm");
    realm->valid = false;

    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (test.GetHttpProtocol()->GetHttpCommand() == HttpDefinition::HSHCReplyAUTH);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool HttpObjectBrowserTest::TestGetAsText_Realm_False() {
    using namespace MARTe;
    StreamString cfg = ""
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
            "+ARealm = {\n"
            "    Class = HttpObjectBrowserTestRealm\n"
            "}\n"
            "+HttpObjectBrowser1 = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root = \".\"\n"
            "    Realm = ARealm\n"
            "    +AChild = {\n"
            "        Class = ReferenceContainer\n"
            "    }\n"
            "    +DirectoryResource = {\n"
            "        Class = HttpDirectoryResource\n"
            "        BaseDir = \".\""
            "    }\n"
            "}\n";

    File f;
    f.Open("HttpObjectBrowserTest.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_W);
    f.Printf("Test %d", 1);
    f.Flush();
    f.Close();
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
    ReferenceT<HttpObjectBrowserTestRealm> realm = ObjectRegistryDatabase::Instance()->Find("ARealm");
    realm->valid = false;
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
    ok = service.IsValid();

    if (ok) {
        ok = service->Start();
    }
    HttpClient test;
    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("/?path=HttpObjectBrowserTest.txt");
    StreamString reply;
    if (ok) {
        ok = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ok) {
        ok = (test.GetHttpProtocol()->GetHttpCommand() == HttpDefinition::HSHCReplyAUTH);
    }
    if (ok) {
        ok = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    Directory d("HttpObjectBrowserTest.txt");
    d.Delete();
    return ok;
}
