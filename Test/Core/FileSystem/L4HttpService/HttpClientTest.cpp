/**
 * @file HttpClientTest.cpp
 * @brief Source file for class HttpClientTest
 * @date 05/10/2018
 * @author Giuseppe Ferro
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
 * the class HttpClientTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Base64Encoder.h"
#include "ConfigurationDatabase.h"

#include "HttpClient.h"
#include "HttpClientTest.h"
#include "HttpDataExportI.h"
#include "HttpService.h"
#include "Md5Encrypt.h"
#include "MemoryDataSourceI.h"

#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"

#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StringHelper.h"
#include "StringHelper.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static bool SearchKey(const char8 * const key, const char8 * const name, StreamString &value) {
    bool ret = value.SetSize(0ULL);
    if (ret) {
        ret = (key != NULL_PTR(const char8 * const));
        if (ret) {
            ret = (name != NULL_PTR(const char8 * const));
        }

        if (ret) {
            const char8 *p = StringHelper::SearchString(key, name);
            ret = (p != NULL_PTR(const char8 *));
            if (ret) {
                uint32 gap = StringHelper::Length(name);
                p = &p[gap];
                ret = ((p[0] == '=') && (p[1] == '"'));
                if (ret) {
                    p = &p[2];
                    while (p[0] != '"') {
                        value += p[0];
                        p = &(p[1]);
                    }
                }
            }
        }
    }
    return ret;
}

static bool SearchKey1(const char8 * const key, const char8 * const name, StreamString &value) {
    bool ret = value.SetSize(0ULL);
    if (ret) {
        ret = (key != NULL_PTR(const char8 * const));
        if (ret) {
            ret = (name != NULL_PTR(const char8 * const));
        }

        if (ret) {
            const char8 *p = StringHelper::SearchString(key, name);
            ret = (p != NULL_PTR(const char8 *));
            if (ret) {
                uint32 gap = StringHelper::Length(name);
                p = &p[gap];
                while (p[0] != ',') {
                    value += p[0];
                    p = &(p[1]);
                }
            }
        }
    }
    return ret;
}

class HttpClientTestObj: public HttpClient {
public:
    HttpClientTestObj();

    virtual ~HttpClientTestObj();

    bool GetReConnect();

    const char8* GetAuthorization();

    const char8* GetAuthorizationKey();

    uint32 GetLastOperationId();

};

HttpClientTestObj::HttpClientTestObj() {

}

HttpClientTestObj::~HttpClientTestObj() {

}

bool HttpClientTestObj::GetReConnect() {
    return reConnect;
}

const char8* HttpClientTestObj::GetAuthorization() {
    return authorisation.Buffer();
}

const char8* HttpClientTestObj::GetAuthorizationKey() {
    return authorisationKey.Buffer();
}

uint32 HttpClientTestObj::GetLastOperationId() {
    return lastOperationId;
}

class HttpClientTestClassTest1: public ReferenceContainer, public HttpDataExportI {
public:
    CLASS_REGISTER_DECLARATION()

HttpClientTestClassTest1    ();

    virtual ~HttpClientTestClassTest1();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

};

HttpClientTestClassTest1::HttpClientTestClassTest1() {

}

HttpClientTestClassTest1::~HttpClientTestClassTest1() {

}

bool HttpClientTestClassTest1::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {
    HttpDataExportI::GetAsStructuredData(data, protocol);
    data.CreateAbsolute("NodeA.NodeB");
    uint32 var1 = 1;
    data.Write("var1", var1);
    data.CreateAbsolute("NodeA.NodeC");
    int32 var2 = -1;
    data.Write("var2", var2);
    data.MoveToRoot();

    return true;
}

bool HttpClientTestClassTest1::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    HttpDataExportI::GetAsText(stream, protocol);
    StreamString hString;
    StreamString *hStream = (&hString);
    hStream->SetSize(0);
    hStream->Printf("<html><head><TITLE>%s</TITLE>"
                    "</head><BODY BGCOLOR=\"#ffffff\"><H1>%s</H1><UL>",
                    "HttpServiceTestClassTest1", "HttpServiceTestClassTest1");
    hStream->Printf("%s", "</UL></BODY>");
    hStream->Printf("%s", "</html>");
    hStream->Seek(0);
    uint32 stringSize = hStream->Size();
    stream.Write(hStream->Buffer(), stringSize);

    //protocol.WriteHeader(true, HttpDefinition::HSHCReplyOK, hStream, NULL);
    return true;
}

CLASS_REGISTER(HttpClientTestClassTest1, "1.0")

class HttpClientTestClassTest2: public ReferenceContainer, public HttpDataExportI, public HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION()

HttpClientTestClassTest2    ();

    virtual ~HttpClientTestClassTest2();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

    virtual bool Validate(const char8 * const key,
            const int32 command,
            const uint32 ipNumber);

    virtual bool DigestSecurityNeeded();

    virtual bool GetAuthenticationRequest(StreamString &message);
};

HttpClientTestClassTest2::HttpClientTestClassTest2() {

}

HttpClientTestClassTest2::~HttpClientTestClassTest2() {

}

bool HttpClientTestClassTest2::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {

    protocol.Write("Content-Type", "text/html");
    data.CreateAbsolute("NodeA.NodeB");
    uint32 var1 = 1;
    data.Write("var1", var1);
    data.CreateAbsolute("NodeA.NodeC");
    int32 var2 = -1;
    data.Write("var2", var2);
    data.MoveToRoot();

    return true;
}

bool HttpClientTestClassTest2::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    StreamString hString;
    StreamString *hStream = (&hString);

    HttpDataExportI::GetAsText(stream, protocol);

    hStream->SetSize(0);

    hStream->Printf("<html><head><TITLE>%s</TITLE>"
                    "</head><BODY BGCOLOR=\"#ffffff\"><H1>%s</H1><UL>",
                    "HttpServiceTestClassTest1", "HttpServiceTestClassTest1");
    hStream->Printf("%s", "</UL></BODY>");
    hStream->Printf("%s", "</html>");
    hStream->Seek(0LLU);
    uint32 stringSize = hStream->Size();
    stream.Write(hStream->Buffer(), stringSize);
    return true;
}

bool HttpClientTestClassTest2::Validate(const char8 * const key, const int32 command, const uint32 ipNumber) {
    StreamString user, passwd, realm, nonce, uri, cnonce, response, opaque, nc, qop;
    SearchKey(key, "username", user);
    SearchKey(key, "realm", realm);
    SearchKey(key, "nonce", nonce);
    SearchKey(key, "uri", uri);
    SearchKey(key, "cnonce", cnonce);
    SearchKey(key, "response", response);
    SearchKey(key, "opaque", opaque);
    SearchKey1(key, "nc=", nc);
    SearchKey1(key, "qop=", qop);

    StreamString HA1;
    StreamString toEncode;
    passwd = "1234";
    toEncode.Printf("%s:%s:%s", user.Buffer(), realm.Buffer(), passwd.Buffer());
    uint32 toEncodeLen = static_cast<uint32>(toEncode.Size());

    uint8 buffer[16];
    MemoryOperationsHelper::Set(buffer, 0, 16);
    Md5Encrypt::Md5(reinterpret_cast<uint8 *>(toEncode.BufferReference()), toEncodeLen, &buffer[0]);
    for (uint32 i = 0u; (i < 16u); i++) {
        HA1.Printf("%02x", buffer[i]);
    }

    StreamString HA2;
    StreamString commandStr = HttpDefinition::GetErrorCodeString(command);
    MemoryOperationsHelper::Set(buffer, 0, 16);
    toEncode.SetSize(0);
    toEncode.Printf("%s:%s", commandStr.Buffer(), uri);
    toEncodeLen = static_cast<uint32>(toEncode.Size());
    Md5Encrypt::Md5(reinterpret_cast<uint8*>(toEncode.BufferReference()), toEncodeLen, &buffer[0]);
    for (uint32 i = 0u; (i < 16u); i++) {
        HA2.Printf("%02x", buffer[i]);
    }

    StreamString expectedResponse;
    MemoryOperationsHelper::Set(buffer, 0, 16);
    MemoryOperationsHelper::Set(buffer, 0, 16);
    toEncode.SetSize(0);
    printf("md5(%s:%s:%s:%s:%s:%s)", HA1.Buffer(), nonce.Buffer(), nc.Buffer(), cnonce.Buffer(), qop.Buffer(), HA2.Buffer());

    toEncode.Printf("%s:%s:%s:%s:%s:%s", HA1.Buffer(), nonce.Buffer(), nc.Buffer(), cnonce.Buffer(), qop.Buffer(), HA2.Buffer());
    toEncodeLen = static_cast<uint32>(toEncode.Size());
    Md5Encrypt::Md5(reinterpret_cast<uint8*>(toEncode.BufferReference()), toEncodeLen, &buffer[0]);
    for (uint32 i = 0u; (i < 16u); i++) {
        expectedResponse.Printf("%02x", buffer[i]);
    }

    bool ret = (user == "gferro");
    ret &= (realm == "1234");
    ret &= (nonce == "4321");
    ret &= (uri == "Test1");
    ret &= (response == expectedResponse);
    ret &= (opaque == "true");

    return ret;
}

bool HttpClientTestClassTest2::DigestSecurityNeeded() {
    return false;
}

bool HttpClientTestClassTest2::GetAuthenticationRequest(StreamString &message) {

    message = "Digest realm=\"1234\" nonce=\"4321\" opaque=\"true\"";
    return true;
}

CLASS_REGISTER(HttpClientTestClassTest2, "1.0")

class HttpClientTestClassTest3: public ReferenceContainer, public HttpDataExportI, public HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION()

HttpClientTestClassTest3    ();

    virtual ~HttpClientTestClassTest3();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

    virtual bool Validate(const char8 * const key,
            const int32 command,
            const uint32 ipNumber);

    virtual bool DigestSecurityNeeded();

    virtual bool GetAuthenticationRequest(StreamString &message);
};

HttpClientTestClassTest3::HttpClientTestClassTest3() {

}

HttpClientTestClassTest3::~HttpClientTestClassTest3() {

}

bool HttpClientTestClassTest3::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {

    protocol.Write("Content-Type", "text/html");
    data.CreateAbsolute("NodeA.NodeB");
    uint32 var1 = 1;
    data.Write("var1", var1);
    data.CreateAbsolute("NodeA.NodeC");
    int32 var2 = -1;
    data.Write("var2", var2);
    data.MoveToRoot();

    return true;
}

bool HttpClientTestClassTest3::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    StreamString hString;
    StreamString *hStream = (&hString);
    HttpDataExportI::GetAsText(stream, protocol);

    hStream->SetSize(0);

    hStream->Printf("<html><head><TITLE>%s</TITLE>"
                    "</head><BODY BGCOLOR=\"#ffffff\"><H1>%s</H1><UL>",
                    "HttpServiceTestClassTest1", "HttpServiceTestClassTest1");
    hStream->Printf("%s", "</UL></BODY>");
    hStream->Printf("%s", "</html>");
    hStream->Seek(0);
    uint32 stringSize = hStream->Size();
    stream.Write(hStream->Buffer(), stringSize);
    //For the timeout test
    Sleep::Sec(0.1);
    //protocol.WriteHeader(true, HttpDefinition::HSHCReplyOK, hStream, NULL);
    return true;
}

bool HttpClientTestClassTest3::Validate(const char8 * const key, const int32 command, const uint32 ipNumber) {

    StreamString keyStr = key;
    StreamString auth;
    Base64Encoder::Decode(keyStr, auth);

    bool ret = (auth == "gferro:1234");
    return ret;
}

bool HttpClientTestClassTest3::DigestSecurityNeeded() {
    return false;
}

bool HttpClientTestClassTest3::GetAuthenticationRequest(StreamString &message) {

    StreamString auth = "gferro:1234";
    auth.Seek(0);
    StreamString encoded;
    Base64Encoder::Encode(auth, encoded);
    message.Printf("Basic %s", encoded.Buffer());
    message.Seek(0);
    return true;
}

CLASS_REGISTER(HttpClientTestClassTest3, "1.0")

class HttpClientTestClassTest4: public ReferenceContainer, public HttpDataExportI, public HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION()

HttpClientTestClassTest4    ();

    virtual ~HttpClientTestClassTest4();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

    virtual bool Validate(const char8 * const key,
            const int32 command,
            const uint32 ipNumber);

    virtual bool DigestSecurityNeeded();

    virtual bool GetAuthenticationRequest(StreamString &message);
};

HttpClientTestClassTest4::HttpClientTestClassTest4() {

}

HttpClientTestClassTest4::~HttpClientTestClassTest4() {

}

bool HttpClientTestClassTest4::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {

    protocol.Write("Content-Type", "text/html");
    data.CreateAbsolute("NodeA.NodeB");
    uint32 var1 = 1;
    data.Write("var1", var1);
    data.CreateAbsolute("NodeA.NodeC");
    int32 var2 = -1;
    data.Write("var2", var2);
    data.MoveToRoot();

    return true;
}

bool HttpClientTestClassTest4::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    StreamString hString;
    StreamString *hStream = (&hString);
    HttpDataExportI::GetAsText(stream, protocol);

    hStream->SetSize(0);

    hStream->Printf("<html><head><TITLE>%s</TITLE>"
                    "</head><BODY BGCOLOR=\"#ffffff\"><H1>%s</H1><UL>",
                    "HttpServiceTestClassTest1", "HttpServiceTestClassTest1");
    hStream->Printf("%s", "</UL></BODY>");
    hStream->Printf("%s", "</html>");
    hStream->Seek(0);
    uint32 stringSize = hStream->Size();
    stream.Write(hStream->Buffer(), stringSize);

    //protocol.WriteHeader(true, HttpDefinition::HSHCReplyOK, hStream, NULL);
    return true;
}

bool HttpClientTestClassTest4::Validate(const char8 * const key, const int32 command, const uint32 ipNumber) {

    StreamString keyStr = key;
    StreamString auth;
    Base64Encoder::Decode(keyStr, auth);

    bool ret = auth == "gferro:1234";
    return ret;
}

bool HttpClientTestClassTest4::DigestSecurityNeeded() {
    return false;
}

bool HttpClientTestClassTest4::GetAuthenticationRequest(StreamString &message) {

    StreamString auth = "gferro:1234";
    auth.Seek(0);
    StreamString encoded;
    Base64Encoder::Encode(auth, encoded);
    message.Printf("Invalid %s", encoded.Buffer());
    message.Seek(0);
    return true;
}

CLASS_REGISTER(HttpClientTestClassTest4, "1.0")

//#if 0
/**
 * Helper function to setup a MARTe execution environment
 */
static bool InitialiseMemoryMapInputBrokerEnviroment(const char8 * const config) {

    HeapManager::AddHeap(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    ConfigurationDatabase cdb;
    StreamString configStream = config;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ok = parser.Parse();

    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    if (ok) {
        god->Purge();
        ok = god->Initialise(cdb);
    }
    ReferenceT<RealTimeApplication> application;
    if (ok) {
        application = god->Find("Application");
        ok = application.IsValid();
    }
    if (ok) {
        ok = application->ConfigureApplication();
    }
    return ok;
}
//#endif

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

HttpClientTest::HttpClientTest() {
}

HttpClientTest::~HttpClientTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool HttpClientTest::TestConstructor() {
    HttpClientTestObj test;
    bool ret = test.GetReConnect();

    StreamString urlUri;
    test.GetServerUri(urlUri);
    ret &= (urlUri == "");
    StreamString urlHost;
    test.GetServerAddress(urlHost);
    ret &= (urlHost == "");
    ret &= test.GetServerPort() == 0;

    ret &= StringHelper::Compare(test.GetAuthorization(), "") == 0;

    ret &= StringHelper::Compare(test.GetAuthorizationKey(), "") == 0;

    ret &= test.GetLastOperationId() == 0;

    return ret;
}

bool HttpClientTest::TestInitialise() {
    ConfigurationDatabase cdb;

    StreamString address = "127.0.0.1";
    cdb.Write("ServerAddress", address.Buffer());
    uint32 port = 4444;
    cdb.Write("ServerPort", port);
    StreamString uri = "localhost";
    cdb.Write("ServerUri", uri.Buffer());
    StreamString authorization = "1234";
    cdb.Write("Authorization", authorization.Buffer());

    HttpClientTestObj test;
    bool ret = test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestInitialise_FalseNoServerAddress() {
    ConfigurationDatabase cdb;

    uint32 port = 4444;
    cdb.Write("ServerPort", port);
    StreamString uri = "localhost";
    cdb.Write("ServerUri", uri.Buffer());
    StreamString authorization = "1234";
    cdb.Write("Authorization", authorization.Buffer());

    HttpClientTestObj test;
    bool ret = !test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestInitialise_FalseNoServerPort() {
    ConfigurationDatabase cdb;

    StreamString address = "127.0.0.1";
    cdb.Write("ServerAddress", address.Buffer());
    StreamString uri = "localhost";
    cdb.Write("ServerUri", uri.Buffer());
    StreamString authorization = "1234";
    cdb.Write("Authorization", authorization.Buffer());

    HttpClientTestObj test;
    bool ret = !test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestInitialise_FalseNoServerUri() {
    ConfigurationDatabase cdb;

    StreamString address = "127.0.0.1";
    cdb.Write("ServerAddress", address.Buffer());
    uint32 port = 4444;
    cdb.Write("ServerPort", port);
    StreamString authorization = "1234";
    cdb.Write("Authorization", authorization.Buffer());

    HttpClientTestObj test;
    bool ret = !test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestInitialise_DefaultAuthorization() {
    ConfigurationDatabase cdb;

    StreamString address = "127.0.0.1";
    cdb.Write("ServerAddress", address.Buffer());
    uint32 port = 4444;
    cdb.Write("ServerPort", port);
    StreamString uri = "localhost";
    cdb.Write("ServerUri", uri.Buffer());

    HttpClientTestObj test;
    bool ret = test.Initialise(cdb);

    return ret;
}

bool HttpClientTest::TestGetHttpProtocol() {
    HttpClientTestObj test;
    HttpProtocol* protocol = test.GetHttpProtocol();
    return (protocol != NULL);
}

bool HttpClientTest::TestHttpExchange() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +WebRoot = {"
            "       Class = HttpObjectBrowser"
            "       Root = \".\""
            "       +ClassLister = {"
            "           Class = HttpServiceTestClassLister"
            "       }"
            "       +Test1 = {"
            "           Class = HttpClientTestClassTest1"
            "       }"
            "   }"
            "   +HttpServerTest = {"
            "       Class = HttpService"
            "       WebRoot = \"Application.WebRoot\""
            "       Port=9094"
            "       ListenMaxConnections = 255"
            "       Timeout = 0"
            "       AcceptTimeout = 100"
            "       MaxNumberOfThreads=100"
            "       MinNumberOfThreads=1"
            "   }"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HttpServiceTestGAM"
            "             InputSignals = {"
            "                Counter = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "             }"
            "             OutputSignals = {"
            "                CounterOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                TimeOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +Input = {"
            "            Class = HttpServiceTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    HttpClient test;

    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("Test1");

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    ret = service.IsValid();

    if (ret) {
        ret = service->Start();
    }

    if (ret) {
        StreamString readOut;

        ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 1000u);
        if (ret) {
            ret = (readOut == "20\r\n"
                    "<html><head><TITLE>HttpServiceTe\r\n"
                    "20\r\n"
                    "stClassTest1</TITLE></head><BODY\r\n"
                    "20\r\n"
                    " BGCOLOR=\"#ffffff\"><H1>HttpServi\r\n"
                    "20\r\n"
                    "ceTestClassTest1</H1><UL></UL></\r\n"
                    "C\r\n"
                    "BODY></html>\r\n"
                    "0\r\n\r\n");
        }

    }

    if (ret) {
        ret = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpClientTest::TestHttpExchange_Authorization_Digest() {

    const char8 *config = ""
            "+Realm1 = {"
            "   Class = HttpClientTestClassTest2"
            "}"
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpObjectBrowser"
            "           Root = \".\""
            "           Realm = Realm1\n"
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpClientTestClassTest2"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpService"
            "           WebRoot = \"Application.WebRoot\""
            "           Port=9094"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout = 100"
            "           MaxNumberOfThreads=100"
            "           MinNumberOfThreads=1"
            "       }"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HttpServiceTestGAM"
            "             InputSignals = {"
            "                Counter = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "             }"
            "             OutputSignals = {"
            "                CounterOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                TimeOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +Input = {"
            "            Class = HttpServiceTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    HttpClient test;

    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("Test1");

    StreamString userPass = "gferro:1234";
    StreamString encodedUserPass;
    Base64Encoder::Encode(userPass, encodedUserPass);
    test.SetAuthorisation(encodedUserPass.Buffer());

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    ret = service.IsValid();

    if (ret) {
        ret = service->Start();
    }

    StreamString readOut;
    if (ret) {
        ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 20000u);
        readOut = "";
    }
    if (ret) {
        ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 2000u);
    }
    if (ret) {
        ret = (readOut == "20\r\n"
                "<html><head><TITLE>HttpServiceTe\r\n"
                "20\r\n"
                "stClassTest1</TITLE></head><BODY\r\n"
                "20\r\n"
                " BGCOLOR=\"#ffffff\"><H1>HttpServi\r\n"
                "20\r\n"
                "ceTestClassTest1</H1><UL></UL></\r\n"
                "C\r\n"
                "BODY></html>\r\n"
                "0\r\n\r\n");
    }

    if (ret) {
        ret = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpClientTest::TestHttpExchange_Authorization_Basic() {

    const char8 *config = ""
            "+Realm1 = {"
            "   Class = HttpClientTestClassTest3"
            "}"
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +WebRoot = {"
            "       Class = HttpObjectBrowser"
            "       Root = \".\""
            "       Realm = Realm1\n"
            "       +ClassLister = {"
            "           Class = HttpServiceTestClassLister"
            "       }"
            "       +Test1 = {"
            "           Class = HttpClientTestClassTest3"
            "       }"
            "   }"
            "   +HttpServerTest = {"
            "       Class = HttpService"
            "       WebRoot = \"Application.WebRoot\""
            "       Port=9094"
            "       ListenMaxConnections = 255"
            "       Timeout = 0"
            "       AcceptTimeout = 100"
            "       MaxNumberOfThreads=100"
            "       MinNumberOfThreads=1"
            "   }"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HttpServiceTestGAM"
            "             InputSignals = {"
            "                Counter = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "             }"
            "             OutputSignals = {"
            "                CounterOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                TimeOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +Input = {"
            "            Class = HttpServiceTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    HttpClient test;

    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("Test1");

    StreamString userPass = "gferro:1234";
    StreamString encodedUserPass;
    Base64Encoder::Encode(userPass, encodedUserPass);
    test.SetAuthorisation(encodedUserPass.Buffer());

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    ret = service.IsValid();

    if (ret) {
        ret = service->Start();
    }

    StreamString readOut;
    if (ret) {
        ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 20000u);
    }
    if (ret) {
        ret = (readOut == "20\r\n"
                "<html><head><TITLE>HttpServiceTe\r\n"
                "20\r\n"
                "stClassTest1</TITLE></head><BODY\r\n"
                "20\r\n"
                " BGCOLOR=\"#ffffff\"><H1>HttpServi\r\n"
                "20\r\n"
                "ceTestClassTest1</H1><UL></UL></\r\n"
                "C\r\n"
                "BODY></html>\r\n"
                "0\r\n\r\n");
    }

    if (ret) {
        ret = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpClientTest::TestHttpExchange_Authorization_FalseInvalidAuthType() {

    const char8 *config = ""
            "+Realm1 = {\n"
            "   Class = HttpClientTestClassTest4\n"
            "}\n"
            "$Application = {\n"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpObjectBrowser"
            "           Realm = Realm1"
            "           Root = \".\""
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpClientTestClassTest4"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpService"
            "           WebRoot = \"Application.WebRoot\""
            "           Port=9094"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout = 100"
            "           MaxNumberOfThreads=100"
            "           MinNumberOfThreads=1"
            "       }"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HttpServiceTestGAM"
            "             InputSignals = {"
            "                Counter = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "             }"
            "             OutputSignals = {"
            "                CounterOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                TimeOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +Input = {"
            "            Class = HttpServiceTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    HttpClient test;

    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("Test1");

    StreamString userPass = "gferro:4567";
    StreamString encodedUserPass;
    Base64Encoder::Encode(userPass, encodedUserPass);
    test.SetAuthorisation(encodedUserPass.Buffer());

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    ret = service.IsValid();

    if (ret) {
        ret = service->Start();
    }

    if (ret) {
        StreamString readOut;

        ret = !test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 1000u);

    }

    if (ret) {
        ret = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpClientTest::TestHttpExchange_Authorization_FalseTimeout() {

    const char8 *config = ""
            "+Realm1 = {\n"
            "   Class = HttpClientTestClassTest3\n"
            "}\n"
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpObjectBrowser"
            "           Realm = Realm1"
            "           Root = \".\""
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpClientTestClassTest3"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpService"
            "           WebRoot = \"Application.WebRoot\""
            "           Port=9094"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout = 100"
            "           MaxNumberOfThreads=100"
            "           MinNumberOfThreads=1"
            "       }"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HttpServiceTestGAM"
            "             InputSignals = {"
            "                Counter = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "             }"
            "             OutputSignals = {"
            "                CounterOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                TimeOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +Input = {"
            "            Class = HttpServiceTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    HttpClient test;

    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("Test1");

    StreamString userPass = "gferro:4567";
    StreamString encodedUserPass;
    Base64Encoder::Encode(userPass, encodedUserPass);
    test.SetAuthorisation(encodedUserPass.Buffer());
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = service.IsValid();
    }
    if (ret) {
        ret = service->Start();
    }
    StreamString readOut;
    if (ret) {
        ret = !test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 0u);
    }
    if (service.IsValid()) {
        (void) service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpClientTest::TestHttpExchange_Authorization_FalseReplyCommand() {

    const char8 *config = ""
            "+Realm1 = {\n"
            "   Class = HttpClientTestClassTest3\n"
            "}\n"
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpObjectBrowser"
            "           Root = \".\""
            "           Realm = Realm1"
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpClientTestClassTest3"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpService"
            "           WebRoot = \"Application.WebRoot\""
            "           Port=9094"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout = 100"
            "           MaxNumberOfThreads=100"
            "           MinNumberOfThreads=1"
            "       }"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HttpServiceTestGAM"
            "             InputSignals = {"
            "                Counter = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "             }"
            "             OutputSignals = {"
            "                CounterOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                TimeOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +Input = {"
            "            Class = HttpServiceTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    HttpClient test;

    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("Test1");

    StreamString userPass = "gferro:4567";
    StreamString encodedUserPass;
    Base64Encoder::Encode(userPass, encodedUserPass);
    test.SetAuthorisation(encodedUserPass.Buffer());

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    ret = service.IsValid();

    if (ret) {
        ret = service->Start();
    }

    if (ret) {
        StreamString readOut;

        ret = !test.HttpExchange(readOut, HttpDefinition::HSHCReplyOK, NULL, 1000u);

    }

    if (ret) {
        ret = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpClientTest::TestHttpExchange_Authorization_Digest_KeepAlive() {

    const char8 *config = ""
            "+Realm1 = {\n"
            "   Class = HttpClientTestClassTest2\n"
            "}\n"
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpObjectBrowser"
            "           Realm = Realm1"
            "           Root = \".\""
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpClientTestClassTest2"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpService"
            "           WebRoot = \"Application.WebRoot\""
            "           Port=9094"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout = 100"
            "           MaxNumberOfThreads=100"
            "           MinNumberOfThreads=1"
            "           CloseOnAuthFail = 0"
            "       }"
            "   +Functions = {"
            "       Class = ReferenceContainer"
            "       +GAM1 = {"
            "           Class = HttpServiceTestGAM"
            "             InputSignals = {"
            "                Counter = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                }"
            "                Time = {"
            "                    DataSource = Input"
            "                    Type = uint32"
            "                    Frequency = 1000"
            "                }"
            "             }"
            "             OutputSignals = {"
            "                CounterOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "                TimeOnDDB = {"
            "                    DataSource = DDB1"
            "                    Type = uint32"
            "                }"
            "             }"
            "        }"
            "    }"
            "    +Data = {"
            "        Class = ReferenceContainer"
            "        DefaultDataSource = DDB1"
            "        +DDB1 = {"
            "            Class = GAMDataSource"
            "        }"
            "        +Timings = {"
            "            Class = TimingDataSource"
            "        }"
            "        +Input = {"
            "            Class = HttpServiceTestDS"
            "        }"
            "    }"
            "    +States = {"
            "        Class = ReferenceContainer"
            "        +Idle = {"
            "            Class = RealTimeState"
            "            +Threads = {"
            "                Class = ReferenceContainer"
            "                +Thread1 = {"
            "                    Class = RealTimeThread"
            "                    CPUs = 2"
            "                    Functions = { GAM1 }"
            "                }"
            "            }"
            "         }"
            "     }"
            "     +Scheduler = {"
            "         Class = GAMScheduler"
            "         TimingDataSource = Timings"
            "     }"
            "}";

    HttpClient test;

    test.SetServerAddress("127.0.0.1");
    test.SetServerPort(9094);
    test.SetServerUri("Test1");

    StreamString userPass = "gferro:1234";
    StreamString encodedUserPass;
    Base64Encoder::Encode(userPass, encodedUserPass);
    test.SetAuthorisation(encodedUserPass.Buffer());

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    ret = service.IsValid();

    if (ret) {
        ret = service->Start();
    }
    StreamString readOut;

    if (ret) {
        ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 20000u);
        readOut = "";
    }
    if (ret) {

        ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 2000u);
    }
    printf("\n%s\n", readOut.Buffer());
    if (ret) {
        ret = (readOut == "20\r\n"
                "<html><head><TITLE>HttpServiceTe\r\n"
                "20\r\n"
                "stClassTest1</TITLE></head><BODY\r\n"
                "20\r\n"
                " BGCOLOR=\"#ffffff\"><H1>HttpServi\r\n"
                "20\r\n"
                "ceTestClassTest1</H1><UL></UL></\r\n"
                "C\r\n"
                "BODY></html>\r\n"
                "0\r\n\r\n");
    }

    if (ret) {
        ret = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpClientTest::TestSetServerAddress() {
    HttpClientTestObj test;
    StreamString serverAddress;
    test.GetServerAddress(serverAddress);
    bool ret = (serverAddress == "");
    test.SetServerAddress("127.0.0.1");
    test.GetServerAddress(serverAddress);
    ret &= (serverAddress == "127.0.0.1");
    return ret;

}

bool HttpClientTest::TestSetServerPort() {
    HttpClientTestObj test;
    uint32 port = test.GetServerPort();
    bool ret = (port == 0);
    test.SetServerPort(8080);
    port = test.GetServerPort();
    ret &= (port == 8080);
    return ret;
}

bool HttpClientTest::TestSetServerUri() {
    HttpClientTestObj test;
    StreamString serverUri;
    test.GetServerUri(serverUri);
    bool ret = (serverUri == "");
    test.SetServerUri("127.0.0.1");
    test.GetServerUri(serverUri);
    ret &= (serverUri == "127.0.0.1");
    return ret;
}

bool HttpClientTest::TestGetServerAddress() {
    return TestSetServerAddress();
}

bool HttpClientTest::TestGetServerPort() {
    return TestSetServerPort();
}

bool HttpClientTest::TestGetServerUri() {
    return TestSetServerUri();

}

bool HttpClientTest::TestGetAuthorisation() {
    HttpClientTestObj test;
    StreamString auth;
    test.GetAuthorisation(auth);
    bool ret = (auth == "");
    StreamString authToSet = "gferro:1234";
    test.SetAuthorisation(authToSet.Buffer());
    test.GetAuthorisation(auth);
    ret &= (auth == "gferro:1234");
    return ret;
}

bool HttpClientTest::TestSetAuthorisation() {
    return TestGetAuthorisation();
}

