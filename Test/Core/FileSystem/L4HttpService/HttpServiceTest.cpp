/**
 * @file HttpServiceTest.cpp
 * @brief Source file for class HttpServiceTest
 * @date 12/09/2018
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
 * the class HttpServiceTest (public, protected, and private). Be aware that some 
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

#include "HttpDataExportI.h"
#include "HttpDefinition.h"
#include "HttpProtocol.h"
#include "HttpRealmI.h"
#include "HttpServiceTest.h"
#include "JsonPrinter.h"

#include "MemoryDataSourceI.h"
#include "MemoryMapInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"

#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StreamStructuredData.h"
#include "StringHelper.h"
#include "ThreadInformation.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HttpServiceTestGAM: public GAM {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestGAM    ();

    virtual ~HttpServiceTestGAM();

    virtual bool Execute();

    virtual bool Setup();

};

HttpServiceTestGAM::HttpServiceTestGAM() {

}

HttpServiceTestGAM::~HttpServiceTestGAM() {

}

bool HttpServiceTestGAM::Execute() {

    return true;
}

bool HttpServiceTestGAM::Setup() {
    return true;
}

CLASS_REGISTER(HttpServiceTestGAM, "1.0")

class HttpServiceTestDS: public MemoryDataSourceI {

public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestDS    ();

    virtual ~HttpServiceTestDS();

    const char8 *GetBrokerName(StructuredDataI &data,const SignalDirection direction);

    virtual bool PrepareNextState(const char8 * const currentStateName,
            const char8 * const nextStateName);

    virtual bool Synchronise();

};

HttpServiceTestDS::HttpServiceTestDS() {

}

HttpServiceTestDS::~HttpServiceTestDS() {

}

const char8 *HttpServiceTestDS::GetBrokerName(StructuredDataI &data, const SignalDirection direction) {
    const char8* brokerName = NULL_PTR(const char8 *);

    if (direction == InputSignals) {

        float32 frequency = 0.F;
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
        }
        uint32 trigger = 0u;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0u) {
                brokerName = "MemoryMapSynchronisedInputBroker";
            }
        }
        if (brokerName == NULL_PTR(const char8 *)) {
            brokerName = "MemoryMapInputBroker";
        }
    }

    if (direction == OutputSignals) {

        float32 frequency = 0.F;
        if (data.Read("Frequency", frequency)) {
            if (frequency > 0.) {
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
        }
        uint32 trigger = 0u;
        if (data.Read("Trigger", trigger)) {
            if (trigger > 0u) {
                brokerName = "MemoryMapSynchronisedOutputBroker";
            }
        }
        if (brokerName == NULL_PTR(const char8 *)) {
            brokerName = "MemoryMapOutputBroker";
        }
    }

    return brokerName;
}

bool HttpServiceTestDS::PrepareNextState(const char8 * const currentStateName, const char8 * const nextStateName) {
    return true;
}

bool HttpServiceTestDS::Synchronise() {
    return true;
}

CLASS_REGISTER(HttpServiceTestDS, "1.0")

class HttpServiceTestClassLister: public ReferenceContainer, public HttpDataExportI {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestClassLister    ();

    virtual ~HttpServiceTestClassLister();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

};

HttpServiceTestClassLister::HttpServiceTestClassLister() {

}

HttpServiceTestClassLister::~HttpServiceTestClassLister() {

}

bool HttpServiceTestClassLister::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {
    printf("\nCall Struct\n");
    return false;
}

bool HttpServiceTestClassLister::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    printf("\nCall Text\n");
    StreamString hString;
    StreamString *hStream = (&hString);

    StreamString className;
    protocol.MoveAbsolute("InputCommands");
    protocol.Read("Class", className);

    printf("\nclass name = %s\n", className.Buffer());
    hStream->SetSize(0);
    if (!protocol.MoveAbsolute("OutputOptions")) {
        protocol.CreateAbsolute("OutputOptions");
    }
    protocol.Write("Content-Type", "text/html");

    {
        hStream->Printf("<HTML>\n"
                        "<HEAD><TITLE>%s</TITLE></HEAD>\n"
                        "<STYLE type=\"text/css\">\n"
                        "  BODY { background: black; color: green}\n"
                        "  A:link { color: red }\n"
                        "  A:visited { color: maroon }\n"
                        "  A:active { color: fuchsia }\n"
                        "</STYLE>\n"
                        "<H1>%s</H1><UL>\n",
                        "HttpServiceTestClassLister", "HttpServiceTestClassLister");

        hStream->Printf("%s", "B = Object can be built by name; A = Allocated objects; D = code from DLL ;S = structure public\n");
        hStream->Printf("%s", "<TABLE border=2>");
        hStream->Printf("%s", "<TR>");
        hStream->Printf("%s", "<TD>");
        hStream->Printf("%s", "Flags");
        hStream->Printf("%s", "</TD>");
        hStream->Printf("%s", "<TD>");
        hStream->Printf("%s", "Name");
        hStream->Printf("%s", "</TD>");
        hStream->Printf("%s", "<TD>");
        hStream->Printf("%s", "Version");
        hStream->Printf("%s", "</TD>");
        hStream->Printf("%s", "<TD>");
        hStream->Printf("%s", "Size");
        hStream->Printf("%s", "</TD>");
        hStream->Printf("%s", "<TD>");
        hStream->Printf("%s", "Allocated");
        hStream->Printf("%s", "</TD>");
        hStream->Printf("%s", "<TD>");
        hStream->Printf("%s", "</TD>");
        hStream->Printf("%s", "</TR>");

        ClassRegistryDatabase *crdb = ClassRegistryDatabase::Instance();

        uint32 nClasses = crdb->GetSize();
        for (uint32 i = 0u; i < nClasses; i++) {

            ClassRegistryItem *item = (ClassRegistryItem *) (crdb->Peek(i));
            if (item != NULL) {

                hStream->Printf("%s", "<TR>");

                hStream->Printf("%s", "<TD>");
                hStream->Printf("%s", "<SPAN STYLE=\"color: green;background-color: black;\">");
                if (item->GetObjectBuilder() != NULL) {
                    hStream->Printf("%s", "B");
                }
                if (item->GetLoadableLibrary() != NULL) {
                    hStream->Printf("%s", "L");
                }
                if (item->GetNumberOfInstances() > 0) {
                    hStream->Printf("%s", "A");
                }
                if (item->GetIntrospection() != NULL) {
                    hStream->Printf("%s", "S");
                }

                hStream->Printf("%s", "</TD>");
                hStream->Printf("%s", "<TD align=left>");

                ClassProperties properties;
                item->GetClassPropertiesCopy(properties);

                hStream->Printf("%s", "<SPAN STYLE=\"color: red;background-color: black;\">");
                hStream->Printf("%s", properties.GetName());

                hStream->Printf("%s", "</TD>");
                hStream->Printf("%s", "<TD>");

                hStream->Printf("%s", "<SPAN STYLE=\"color: darkred;background-color: black;\">");
                hStream->Printf("%s", properties.GetVersion());

                hStream->Printf("%s", "</TD>");
                hStream->Printf("%s", "<TD>");

                hStream->Printf("%s", "<SPAN STYLE=\"color: darkred;background-color: black;\">");
                hStream->Printf("%i", properties.GetSize());

                hStream->Printf("%s", "</TD>");
                hStream->Printf("%s", "<TD>");
                if (item->GetNumberOfInstances() > 0) {

                    hStream->Printf("%s", "<SPAN STYLE=\"color: darkred;background-color: black;\">");

                    hStream->Printf("%i", item->GetNumberOfInstances());
                }
                hStream->Printf("%s", "</TD>");
                hStream->Printf("%s", "<TD>");
                Introspection *introspection = (Introspection *) (item->GetIntrospection());
                if (introspection != NULL) {

                    uint32 numberOfMembers = introspection->GetNumberOfMembers();
                    if (numberOfMembers > 0u) {
                        if (StringHelper::Compare(className.Buffer(), properties.GetName()) == 0) {

                            hStream->Printf("%s", "<TABLE>");

                            for (uint32 j = 0u; j < numberOfMembers; j++) {
                                const IntrospectionEntry introEntry = (*introspection)[j];
                                hStream->Printf("%s", "<TR>");

                                hStream->Printf("%s", "<TD>");
                                hStream->Printf("%s", "<SPAN STYLE=\"color: green;background-color: black;\">");
                                hStream->Printf("%s %s", introEntry.GetMemberTypeName(), introEntry.GetMemberModifiers());
                                hStream->Printf("%s", "</TD>");

                                hStream->Printf("%s", "<TD>");
                                hStream->Printf("%s", "<SPAN STYLE=\"color: green;background-color: black;\">");
                                hStream->Printf("%s", introEntry.GetMemberName());
                                for (uint32 k = 0; k < introEntry.GetNumberOfDimensions(); k++) {
                                    hStream->Printf("[%i]", introEntry.GetNumberOfElements(k));
                                }
                                hStream->Printf("%s", "</TD>");

                                hStream->Printf("%s", "</TR>");
                            }

                            hStream->Printf("%s", "</TABLE>");
                        }
                        else {
                            hStream->Printf("%s", "<SPAN STYLE=\"color: red;background-color: black;\">");
                            StreamString urlT;
                            protocol.GetId(urlT);
                            printf("\nurl = %s, prop = %s\n", urlT.Buffer(), properties.GetName());
                            StreamString className = properties.GetName();
                            hStream->Printf("<A HREF=/%s?Class=%s NAME=+>", urlT.Buffer(), className.Buffer());
                            hStream->Printf("%s", "+");
                            hStream->Printf("%s", "</A>");
                        }
                    }
                }

                //hStream->Printf("%30s %20s %x\n",item->ClassName(),item->Version(),item->Size());
                hStream->Printf("%s", "</TD>");
                hStream->Printf("%s", "</TR>");

            }
        }

        hStream->Printf("%s", "</TABLE>");
    }

    hStream->Printf("%s", "</BODY></HTML>\n");
    hStream->Seek(0);

    //copy to the client
    //protocol.WriteHeader(true, HttpDefinition::HSHCReplyOK, hStream, NULL);
    uint32 stringSize = hStream->Size();
    stream.Write(hStream->Buffer(), stringSize);

    return true;
}

CLASS_REGISTER(HttpServiceTestClassLister, "1.0")

class HttpServiceTestClassTest1: public ReferenceContainer, public HttpDataExportI {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestClassTest1    ();

    virtual ~HttpServiceTestClassTest1();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

};

HttpServiceTestClassTest1::HttpServiceTestClassTest1() {

}

HttpServiceTestClassTest1::~HttpServiceTestClassTest1() {

}

bool HttpServiceTestClassTest1::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {
    HttpDataExportI::GetAsStructuredData(data, protocol);
    //Print the opening {
    StreamStructuredData<JsonPrinter> *sdata = dynamic_cast<StreamStructuredData<JsonPrinter> *>(&data);
    if (sdata != NULL_PTR(StreamStructuredData<JsonPrinter> *)) {
        sdata->GetPrinter()->PrintBegin();
        data.CreateAbsolute("NodeA.NodeB");
        uint32 var1 = 1;
        data.Write("var1", var1);
        data.CreateAbsolute("NodeA.NodeC");
        int32 var2 = -1;
        data.Write("var2", var2);
        data.MoveToRoot();
        sdata->GetPrinter()->PrintEnd();
    }
    return true;
}

bool HttpServiceTestClassTest1::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    StreamString hString;
    StreamString *hStream = (&hString);
    HttpDataExportI::GetAsText(stream, protocol);

    hStream->Printf("<html><head><TITLE>%s</TITLE>"
                    "</head><BODY BGCOLOR=\"#ffffff\"><H1>%s</H1><UL>",
                    "HttpServiceTestClassTest1", "HttpServiceTestClassTest1");
    hStream->Printf("%s", "</UL></BODY>");
    hStream->Printf("%s", "</html>");
    hStream->Seek(0);
    uint32 stringSize = hStream->Size();
    stream.Write(hStream->Buffer(), stringSize);

    return true;
}

CLASS_REGISTER(HttpServiceTestClassTest1, "1.0")

class HttpServiceTestClassTest2: public ReferenceContainer, public HttpDataExportI, public HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestClassTest2    ();

    virtual ~HttpServiceTestClassTest2();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

    virtual bool Validate(const char8 * const key,
            const int32 command,
            const uint32 ipNumber);

    virtual bool DigestSecurityNeeded();

    virtual bool GetAuthenticationRequest(StreamString &message);
};

HttpServiceTestClassTest2::HttpServiceTestClassTest2() {

}

HttpServiceTestClassTest2::~HttpServiceTestClassTest2() {

}

bool HttpServiceTestClassTest2::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {

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

bool HttpServiceTestClassTest2::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    StreamString hString;
    StreamString *hStream = (&hString);

    HttpDataExportI::GetAsText(stream, protocol);

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

bool HttpServiceTestClassTest2::Validate(const char8 * const key, const int32 command, const uint32 ipNumber) {

    StreamString keyStr = key;
    StreamString auth;
    Base64Encoder::Decode(keyStr, auth);

    bool ret = auth == "gferro:1234";
    return ret;
}

bool HttpServiceTestClassTest2::DigestSecurityNeeded() {
    return false;
}

bool HttpServiceTestClassTest2::GetAuthenticationRequest(StreamString &message) {

    StreamString auth = "gferro:1234";
    auth.Seek(0);
    StreamString encoded;
    Base64Encoder::Encode(auth, encoded);
    message.Printf("Basic %s", encoded.Buffer());
    message.Seek(0);
    return true;
}

CLASS_REGISTER(HttpServiceTestClassTest2, "1.0")

class HttpServiceTestClassTest3: public ReferenceContainer, public HttpDataExportI {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestClassTest3    ();

    virtual ~HttpServiceTestClassTest3();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol);

    virtual bool GetAsText(StreamI &stream, HttpProtocol &protocol);

};

HttpServiceTestClassTest3::HttpServiceTestClassTest3() {

}

HttpServiceTestClassTest3::~HttpServiceTestClassTest3() {

}

bool HttpServiceTestClassTest3::GetAsStructuredData(StreamStructuredDataI &data, HttpProtocol &protocol) {
    HttpDataExportI::GetAsStructuredData(data, protocol);
    data.Write("Test", "test");
    Sleep::Sec(2.0);
    return true;
}

bool HttpServiceTestClassTest3::GetAsText(StreamI &stream, HttpProtocol &protocol) {
    HttpDataExportI::GetAsText(stream, protocol);
    return true;
}

CLASS_REGISTER(HttpServiceTestClassTest3, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
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

const char8 *config = ""
        "$Application = {"
        "   Class = RealTimeApplication"
        "       +WebRoot = {"
        "           Class = HttpObjectBrowser"
        "           Root = \".\""
        "           +ClassLister = {"
        "               Class = HttpServiceTestClassLister"
        "           }"
        "           +Test1 = {"
        "               Class = HttpServiceTestClassTest1"
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

HttpServiceTest::HttpServiceTest() {
}

HttpServiceTest::~HttpServiceTest() {
    ObjectRegistryDatabase::Instance()->Purge();
}

bool HttpServiceTest::TestConstructor() {
    HttpService httpserver;
    bool ret = (httpserver.GetPort() == 0u);

    ret &= httpserver.GetMaxConnections() == 0;
    ret &= !httpserver.GetWebRoot().IsValid();
    return ret;

}

bool HttpServiceTest::TestInitialise() {
    StreamString configStream = ""
            "+WebRoot = {\n"
            "  Class = HttpObjectBrowser\n"
            "  Root=\".\""
            "}\n"
            "+HttpServerTest = {\n"
            "  Class = HttpService\n"
            "  WebRoot=\"WebRoot\"\n"
            "  Port=9094\n"
            "  ListenMaxConnections = 3\n"
            "  HttpRelayURL = \"www.google.it\"\n"
            "  Timeout = 0\n"
            "  MaxNumberOfThreads=100\n"
            "  MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    cdb.MoveToRoot();
    if (ret) {
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("HttpServerTest");
    }
    if (ret) {
        ret = test.IsValid();
    }
    if (ret) {
        ret = (test->Start() == ErrorManagement::NoError);
    }
    if (ret) {
        ret = (test->GetPort() == 9094);
        ret &= (test->GetMaxConnections() == 3);
        ret &= (test->GetWebRoot().IsValid());
    }
    if (test.IsValid()) {
        test->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestInitialise_WebRoot() {
    StreamString configStream = ""
            "+HttpServerTest = {\n"
            "  Class = HttpService\n"
            "  Port=9094\n"
            "  ListenMaxConnections = 3\n"
            "  HttpRelayURL = \"www.google.it\"\n"
            "  Timeout = 0\n"
            "  MaxNumberOfThreads=100\n"
            "  MinNumberOfThreads=1\n"
            "  +WebRoot = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root=\".\""
            "  }\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    cdb.MoveToRoot();
    if (ret) {
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("HttpServerTest");
    }
    if (ret) {
        ret = test.IsValid();
    }
    if (ret) {
        ret = (test->GetPort() == 9094);
        ret &= (test->GetMaxConnections() == 3);
        ret &= (test->GetWebRoot().IsValid());
    }
    if (test.IsValid()) {
        test->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestInitialise_FalseNoWebRoot() {
    StreamString configStream = ""
            "+HttpServerTest = {\n"
            "  Class = HttpService\n"
            "  Port=9094\n"
            "  ListenMaxConnections = 3\n"
            "  HttpRelayURL = \"www.google.it\"\n"
            "  Timeout = 0\n"
            "  MaxNumberOfThreads=100\n"
            "  MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    cdb.MoveToRoot();
    if (ret) {
        ret = !ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestInitialise_FalseBadWebRoot() {
    StreamString configStream = ""
            "+HttpServerTest = {\n"
            "  Class = HttpService\n"
            "  Port=9094\n"
            "  ListenMaxConnections = 3\n"
            "  HttpRelayURL = \"www.google.it\"\n"
            "  Timeout = 0\n"
            "  MaxNumberOfThreads=100\n"
            "  MinNumberOfThreads=1\n"
            "  +WebRoot = {\n"
            "    Class = ReferenceContainer\n"
            "    Root=\".\""
            "  }\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    cdb.MoveToRoot();
    if (ret) {
        ret = !ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestInitialise_FalseBadWebRootRef() {
    StreamString configStream = ""
            "+WebRoot = {\n"
            "  Class = ReferenceContainer\n"
            "  Root=\".\""
            "}\n"
            "+HttpServerTest = {\n"
            "  Class = HttpService\n"
            "  WebRoot=\"WebRoot\"\n"
            "  Port=9094\n"
            "  ListenMaxConnections = 3\n"
            "  HttpRelayURL = \"www.google.it\"\n"
            "  Timeout = 0\n"
            "  MaxNumberOfThreads=100\n"
            "  MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    cdb.MoveToRoot();
    if (ret) {
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("HttpServerTest");
    }
    if (ret) {
        ret = test.IsValid();
    }
    if (ret) {
        ret = (test->Start() != ErrorManagement::NoError);
    }
    if (test.IsValid()) {
        test->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultNListenConnections() {
    StreamString configStream = ""
            "+HttpServerTest = {\n"
            "  Class = HttpService\n"
            "  WebRoot=\"WebRoot\"\n"
            "  Port=9094\n"
            "  HttpRelayURL = \"www.google.it\"\n"
            "  Timeout = 0\n"
            "  MaxNumberOfThreads=100\n"
            "  MinNumberOfThreads=1\n"
            "  +WebRoot = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root=\".\""
            "  }\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    cdb.MoveToRoot();
    if (ret) {
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("HttpServerTest");
    }
    if (ret) {
        ret = test.IsValid();
    }
    if (ret) {
        ret = (test->GetPort() == 9094);
        ret &= (test->GetMaxConnections() == 255);
        ret &= (test->GetWebRoot().IsValid());
    }
    if (test.IsValid()) {
        test->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultPort() {
    StreamString configStream = ""
            "+HttpServerTest = {\n"
            "  Class = HttpService\n"
            "  WebRoot=\"WebRoot\"\n"
            "  HttpRelayURL = \"www.google.it\"\n"
            "  Timeout = 0\n"
            "  MaxNumberOfThreads=100\n"
            "  MinNumberOfThreads=1\n"
            "  +WebRoot = {\n"
            "    Class = HttpObjectBrowser\n"
            "    Root=\".\""
            "  }\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();
    cdb.MoveToRoot();
    if (ret) {
        ret = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    ReferenceT<HttpService> test;
    if (ret) {
        test = ObjectRegistryDatabase::Instance()->Find("HttpServerTest");
    }
    if (ret) {
        ret = test.IsValid();
    }
    if (ret) {
        ret = (test->GetPort() == 80);
        ret &= (test->GetMaxConnections() == 255);
        ret &= (test->GetWebRoot().IsValid());
    }
    if (test.IsValid()) {
        test->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestStart() {

    const char8 *config1 = ""
            "+WebRoot = {"
            "  Class = HttpObjectBrowser"
            "  Root = \".\""
            "  +ClassLister = {"
            "     Class = HttpServiceTestClassLister"
            "  }"
            "  +Test1 = {"
            "     Class = HttpServiceTestClassTest1"
            "  }"
            "}"
            "$Application = {"
            "   Class = RealTimeApplication"
            "   +HttpServerTest = {"
            "       Class = HttpService"
            "       WebRoot = \"WebRoot\""
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = test->Start();
        }
    }
    if (ret) {
        ReferenceT<ReferenceContainer> webroot = test->GetWebRoot();
        ret = webroot.IsValid();
    }
    if (test.IsValid()) {
        ret = test->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool HttpServiceTest::TestStart_InvalidWebRoot() {

    const char8 *config1 = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpObjectBrowser"
            "           Root = \".\""
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpServiceTestClassTest1"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpService"
            "           WebRoot = \"Application.Invalid\""
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

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config1);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = !test->Start();
        }
    }

    if (test.IsValid()) {
        ret = test->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestClientService_Text_Interactive() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = test->Start();
        }
    }

    while (1)
        ;
    return ret;
}

bool HttpServiceTest::TestClientService_Text() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
    }
    if (ret) {
        ret = test->Start();
    }

    InternetHost source(9094, "127.0.0.1");
    InternetHost destination(9094, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    if (ret) {
        ret = socket.Open();
    }
    if (ret) {
        ret = socket.Connect("127.0.0.1", 9094);
    }

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GET /Test1/ HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:9094\r\n");
    socket.Printf("%s", "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\r\n");
    socket.Printf("%s", "Accept: text/html\r\n");
    socket.Printf("%s", "Accept-Encoding: gzip, deflate\r\n");
    socket.Printf("%s", "Connection: keep-alive\r\n\r\n");
    socket.Flush();

    stream.ReadHeader();
    StreamString respBody;
    stream.CompleteReadOperation(&respBody, 1000);

    printf("\n%s\n", respBody.Buffer());

    if (ret) {
        ret = respBody == "20\r\n"
                "<html><head><TITLE>HttpServiceTe\r\n"
                "20\r\n"
                "stClassTest1</TITLE></head><BODY\r\n"
                "20\r\n"
                " BGCOLOR=\"#ffffff\"><H1>HttpServi\r\n"
                "20\r\n"
                "ceTestClassTest1</H1><UL></UL></\r\n"
                "C\r\n"
                "BODY></html>\r\n"
                "0\r\n\r\n";
    }

    if (test.IsValid()) {
        ret = test->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

    //return true;
}

bool HttpServiceTest::TestClientService_Structured() {
    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = test->Start();
        }
    }
    InternetHost source(9094, "127.0.0.1");
    InternetHost destination(9094, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    socket.Connect("127.0.0.1", 9094);

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GET /Test1/?TextMode=0 HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:9094\r\n");
    socket.Printf("%s", "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\r\n");
    socket.Printf("%s", "Accept: text/html\r\n");
    socket.Printf("%s", "Accept-Encoding: gzip, deflate\r\n");
    socket.Printf("%s", "Connection: keep-alive\r\n\r\n");
    socket.Flush();

    stream.ReadHeader();
    StreamString respBody;
    stream.CompleteReadOperation(&respBody, 1000);

    if (ret) {
        ret = (respBody == ""
                "19\r\n"
                "{\n\r"
                "\"NodeA\": {\n\r"
                "\"NodeB\": {\r\n"
                "B\r\n\n\r"
                "\"var1\": 1\r\n"
                "10\r\n\n\r"
                "},\n\r"
                "\"NodeC\": {\r\n"
                "C\r\n\n\r"
                "\"var2\": -1\r\n"
                "6\r\n\n\r"
                "}\n\r"
                "}\r\n"
                "1\r\n"
                "}\r\n"
                "0\r\n\r\n");
    }
    if (test.IsValid()) {
        ret = test->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool HttpServiceTest::TestClientService_CloseConnection() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = test->Start();
        }
    }

    InternetHost source(9094, "127.0.0.1");
    InternetHost destination(9094, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    socket.Connect("127.0.0.1", 9094);

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GET /Test1/ HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:9094\r\n");
    socket.Printf("%s", "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\r\n");
    socket.Printf("%s", "Accept: text/html\r\n");
    socket.Printf("%s", "Accept-Encoding: gzip, deflate\r\n");
    socket.Printf("%s", "Connection: close\r\n\r\n");
    socket.Flush();

    stream.ReadHeader();
    StreamString respBody;
    stream.CompleteReadOperation(&respBody, 1000);

    printf("\n%s\n", respBody.Buffer());

    if (ret) {
        ret = respBody == "20\r\n"
                "<html><head><TITLE>HttpServiceTe\r\n"
                "20\r\n"
                "stClassTest1</TITLE></head><BODY\r\n"
                "20\r\n"
                " BGCOLOR=\"#ffffff\"><H1>HttpServi\r\n"
                "20\r\n"
                "ceTestClassTest1</H1><UL></UL></\r\n"
                "C\r\n"
                "BODY></html>\r\n"
                "0\r\n\r\n";
    }

    if (test.IsValid()) {
        ret = test->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;

}

bool HttpServiceTest::TestClientService_Authorisation() {

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpObjectBrowser"
            "           Root = \".\""
            "           Realm = Test1"
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpServiceTestClassTest2"
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

        ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 1000u);
        if (ret) {
            StreamString userPass = "gferro:1234";
            StreamString encodedUserPass;
            Base64Encoder::Encode(userPass, encodedUserPass);
            test.SetAuthorisation(encodedUserPass.Buffer());

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

    }

    if (service.IsValid()) {
        ret = service->Stop();
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestClientService_FailReadHeader() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = test->Start();
        }
    }

    InternetHost source(9094, "127.0.0.1");
    InternetHost destination(9094, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    socket.Connect("127.0.0.1", 9094);

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GOT /TestFake HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:9094\r\n");
    socket.Printf("%s", "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\r\n");
    socket.Printf("%s", "Accept: text/html\r\n");
    socket.Printf("%s", "Accept-Encoding: gzip, deflate\r\n");
    socket.Printf("%s", "Connection: close\r\n\r\n");
    socket.Flush();

    if (test.IsValid()) {
        ret = test->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestServerCycle() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = test->Start();
        }
    }

    uint32 numberOfClients = 10u;

    for (uint32 i = 0u; (i < numberOfClients) && (ret); i++) {

        InternetHost source(9094 + i, "127.0.0.1");
        InternetHost destination(9094, "127.0.0.1");

        TCPSocket socket;

        socket.SetSource(source);
        socket.SetDestination(destination);
        ret = socket.Open();
        if (ret) {
            ret = socket.Connect("127.0.0.1", 9094);
        }

        HttpProtocol stream(socket);

        StreamString payload;

        socket.Printf("%s", "GET /Test1/ HTTP/1.1\r\n");
        socket.Printf("%s", "Host: localhost:9094\r\n");
        socket.Printf("%s", "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\r\n");
        socket.Printf("%s", "Accept: text/html\r\n");
        socket.Printf("%s", "Accept-Encoding: gzip, deflate\r\n");
        socket.Printf("%s", "Connection: close\r\n\r\n");
        if (ret) {
            ret = socket.Flush();
        }
        if (ret) {
            ret = stream.ReadHeader();
        }
        StreamString respBody;
        if (ret) {
            ret = stream.CompleteReadOperation(&respBody, 1000);
        }

        printf("\n%s\n", respBody.Buffer());

        if (ret) {
            ret = respBody == "20\r\n"
                    "<html><head><TITLE>HttpServiceTe\r\n"
                    "20\r\n"
                    "stClassTest1</TITLE></head><BODY\r\n"
                    "20\r\n"
                    " BGCOLOR=\"#ffffff\"><H1>HttpServi\r\n"
                    "20\r\n"
                    "ceTestClassTest1</H1><UL></UL></\r\n"
                    "C\r\n"
                    "BODY></html>\r\n"
                    "0\r\n\r\n";
        }

    }

    if (test.IsValid()) {
        ret = test->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}

bool HttpServiceTest::TestGetPort() {
    return TestInitialise();
}

bool HttpServiceTest::TestGetMaxConnections() {
    return TestInitialise();
}

bool HttpServiceTest::TestGetWebRoot() {
    return TestInitialise();
}

bool HttpServiceTest::TestClientService_NoMoreThreads() {

    const char8 *config = ""
            "$Application = {"
            "   Class = ReferenceContainer"
            "   +WebRoot = {"
            "       Class = HttpObjectBrowser"
            "       Root = \".\""
            "       +Test3 = {"
            "           Class = HttpServiceTestClassTest3"
            "       }"
            "   }"
            "   +HttpServerTest = {"
            "       Class = HttpService"
            "       WebRoot = \"Application.WebRoot\""
            "       Port=9094"
            "       ListenMaxConnections = 255"
            "       IsTextMode = 0"
            "       Timeout = 0"
            "       AcceptTimeout = 100"
            "       MaxNumberOfThreads=2"
            "       MinNumberOfThreads=1"
            "   }"
            "}";

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();

    ReferenceT<HttpService> service = god->Find("Application.HttpServerTest");
    ret = service.IsValid();

    if (ret) {
        ret = service->Start();
    }

    HttpClient test1;
    if (ret) {
        test1.SetServerAddress("127.0.0.1");
        test1.SetServerPort(9094);
        test1.SetServerUri("Test3");
        StreamString readOut;
        ret = test1.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ret) {
        HttpClient test2;
        test2.SetServerAddress("127.0.0.1");
        test2.SetServerPort(9094);
        test2.SetServerUri("Test3");
        StreamString readOut;
        ret = test2.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 1000u);
    }
    if (ret) {
        //Force the closing of the connection with the server
        test1.SetServerUri("TestDoesNotExist");
        StreamString readOut;
        (void) test1.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 1000u);
    }

    Sleep::Sec(2.0);
    if (ret) {
        HttpClient test2;
        test2.SetServerAddress("127.0.0.1");
        test2.SetServerPort(9094);
        test2.SetServerUri("Test3");
        StreamString readOut;
        ret = test2.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 1000u);
        if (ret) {
            ret = (readOut == "10\r\n\n\r\"Test\": \"test\"\r\n");
        }
    }
    if (service.IsValid()) {
        ret = service->Stop();
    }

    ObjectRegistryDatabase::Instance()->Purge();
    return ret;
}
