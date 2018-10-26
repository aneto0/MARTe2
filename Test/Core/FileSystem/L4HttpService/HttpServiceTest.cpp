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

#include "HttpServiceTest.h"
#include "ConfigurationDatabase.h"

#include "MemoryOperationsHelper.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"
#include "StringHelper.h"
#include "MemoryDataSourceI.h"

#include "MemoryMapInputBroker.h"
#include "MemoryMapSynchronisedInputBroker.h"
#include "MemoryMapOutputBroker.h"
#include "MemoryMapSynchronisedOutputBroker.h"

#include "DataExportI.h"
#include "HttpDefinition.h"
#include "HttpProtocol.h"
#include "HttpRealmI.h"
#include "HttpClient.h"
#include "Base64Encoder.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

class HttpServiceTestService: public HttpService {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestService    ();

    virtual ~HttpServiceTestService();

    uint32 GetPort();

    ReferenceT<ReferenceContainer> GetWebRoot();

    const char8 *GetWebRootPath();

    uint32 GetListenMaxConnections();
};

HttpServiceTestService::HttpServiceTestService() {

}

HttpServiceTestService::~HttpServiceTestService() {

}

uint32 HttpServiceTestService::GetPort() {
    return port;
}

ReferenceT<ReferenceContainer> HttpServiceTestService::GetWebRoot() {
    return webRoot;
}

const char8 *HttpServiceTestService::GetWebRootPath() {
    return webRootPath.Buffer();
}

uint32 HttpServiceTestService::GetListenMaxConnections() {
    return listenMaxConnections;
}

CLASS_REGISTER(HttpServiceTestService, "1.0")

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

const char8 *HttpServiceTestDS::GetBrokerName(StructuredDataI &data,
                                              const SignalDirection direction) {
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

bool HttpServiceTestDS::PrepareNextState(const char8 * const currentStateName,
                                         const char8 * const nextStateName) {
    return true;
}

bool HttpServiceTestDS::Synchronise() {
    return true;
}

CLASS_REGISTER(HttpServiceTestDS, "1.0")

class HttpServiceTestWebRoot: public ReferenceContainer, public DataExportI {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestWebRoot    ();

    virtual ~HttpServiceTestWebRoot();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol);

    virtual bool GetAsText(StreamI &stream, ProtocolI &protocol);

    virtual int32 GetReplyCode(ProtocolI &data);

private:
    void RecCallback(ReferenceT<ReferenceContainer> ref,
            BufferedStreamI *hStream);

};

HttpServiceTestWebRoot::HttpServiceTestWebRoot() {

}

HttpServiceTestWebRoot::~HttpServiceTestWebRoot() {

}

void HttpServiceTestWebRoot::RecCallback(ReferenceT<ReferenceContainer> ref,
                                         BufferedStreamI *hStream) {

    if (ref.IsValid()) {
        const char8* className = ref->GetClassProperties()->GetName();
        const char8* name = ref->GetName();

        hStream->Printf("%s", "<TR>\n");
        hStream->Printf("<TD>%s</TD><TD><A HREF=\"%s/\">%s</A></TD>\n", className, name, name);
        hStream->Printf("%s", "</TR>\n");
        uint32 numberOfElements = ref->Size();
        for (uint32 i = 0u; i < numberOfElements; i++) {
            ReferenceT<ReferenceContainer> child = ref->Get(i);
            RecCallback(ref, hStream);
        }
    }

}

bool HttpServiceTestWebRoot::GetAsStructuredData(StreamStructuredDataI &data,
                                                 ProtocolI &protocol) {
    return false;
}

bool HttpServiceTestWebRoot::GetAsText(StreamI &stream,
                                       ProtocolI &protocol) {

    StreamString hString;
    StreamString *hStream = (&hString);

    hStream->SetSize(0);
    if (!protocol.MoveAbsolute("OutputOptions")) {
        protocol.CreateAbsolute("OutputOptions");
    }
    protocol.Write("Content-Type", "text/html");

    hStream->SetSize(0);

    hStream->Printf("<html><head><TITLE>%s</TITLE>"
                    "</head><BODY BGCOLOR=\"#ffffff\"><H1>%s</H1><UL>",
                    "HttpServiceTestWebRoot", "HttpServiceTestWebRoot");
    hStream->Printf("%s", "<TABLE>\n");
    uint32 numberOfElements = Size();
    for (uint32 i = 0u; i < numberOfElements; i++) {
        ReferenceT<ReferenceContainer> ref = Get(i);
        RecCallback(ref, hStream);
    }
    hStream->Printf("%s", "</TABLE>\n");
    hStream->Printf("%s", "</UL></BODY>\n");
    hStream->Printf("%s", "</html>\n");
    hStream->Seek(0);

    uint32 stringSize = hStream->Size();
    stream.Write(hStream->Buffer(), stringSize);

    //protocol.WriteHeader(true, HttpDefinition::HSHCReplyOK, hStream, NULL);
    return true;
}

int32 HttpServiceTestWebRoot::GetReplyCode(ProtocolI &data) {
    return HttpDefinition::HSHCReplyOK;
}



CLASS_REGISTER(HttpServiceTestWebRoot, "1.0")

class HttpServiceTestClassLister: public ReferenceContainer, public DataExportI {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestClassLister    ();

    virtual ~HttpServiceTestClassLister();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol);

    virtual bool GetAsText(StreamI &stream, ProtocolI &protocol);

    virtual int32 GetReplyCode(ProtocolI &data);

};

HttpServiceTestClassLister::HttpServiceTestClassLister() {

}

HttpServiceTestClassLister::~HttpServiceTestClassLister() {

}

int32 HttpServiceTestClassLister::GetReplyCode(ProtocolI &data) {
    return HttpDefinition::HSHCReplyOK;
}

bool HttpServiceTestClassLister::GetAsStructuredData(StreamStructuredDataI &data,
                                                     ProtocolI &protocol) {
    printf("\nCall Struct\n");
    return false;
}

bool HttpServiceTestClassLister::GetAsText(StreamI &stream,
                                           ProtocolI &protocol) {
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

                hStream->Printf("%s","<TR>");

                hStream->Printf("%s","<TD>");
                hStream->Printf("%s","<SPAN STYLE=\"color: green;background-color: black;\">");
                if (item->GetObjectBuilder() != NULL) {
                    hStream->Printf("%s","B");
                }
                if (item->GetLoadableLibrary() != NULL) {
                    hStream->Printf("%s","L");
                }
                if (item->GetNumberOfInstances() > 0) {
                    hStream->Printf("%s","A");
                }
                if (item->GetIntrospection() != NULL) {
                    hStream->Printf("%s","S");
                }

                hStream->Printf("%s","</TD>");
                hStream->Printf("%s","<TD align=left>");

                ClassProperties properties;
                item->GetClassPropertiesCopy(properties);

                hStream->Printf("%s","<SPAN STYLE=\"color: red;background-color: black;\">");
                hStream->Printf("%s",properties.GetName());

                hStream->Printf("%s","</TD>");
                hStream->Printf("%s","<TD>");

                hStream->Printf("%s","<SPAN STYLE=\"color: darkred;background-color: black;\">");
                hStream->Printf("%s",properties.GetVersion());

                hStream->Printf("%s","</TD>");
                hStream->Printf("%s","<TD>");

                hStream->Printf("%s","<SPAN STYLE=\"color: darkred;background-color: black;\">");
                hStream->Printf("%i",properties.GetSize());

                hStream->Printf("%s","</TD>");
                hStream->Printf("%s","<TD>");
                if (item->GetNumberOfInstances() > 0) {

                    hStream->Printf("%s","<SPAN STYLE=\"color: darkred;background-color: black;\">");

                    hStream->Printf("%i",item->GetNumberOfInstances());
                }
                hStream->Printf("%s","</TD>");
                hStream->Printf("%s","<TD>");
                Introspection *introspection=(Introspection *)(item->GetIntrospection());
                if (introspection !=NULL) {

                    uint32 numberOfMembers=introspection->GetNumberOfMembers();
                    if(numberOfMembers>0u) {
                        if (StringHelper::Compare(className.Buffer(),properties.GetName())==0) {

                            hStream->Printf("%s","<TABLE>");

                            for(uint32 j=0u; j<numberOfMembers; j++) {
                                const IntrospectionEntry introEntry=(*introspection)[j];
                                hStream->Printf("%s","<TR>");

                                hStream->Printf("%s","<TD>");
                                hStream->Printf("%s","<SPAN STYLE=\"color: green;background-color: black;\">");
                                hStream->Printf("%s %s",introEntry.GetMemberTypeName(),introEntry.GetMemberModifiers());
                                hStream->Printf("%s","</TD>");

                                hStream->Printf("%s","<TD>");
                                hStream->Printf("%s","<SPAN STYLE=\"color: green;background-color: black;\">");
                                hStream->Printf("%s",introEntry.GetMemberName());
                                for (uint32 k = 0;k <introEntry.GetNumberOfDimensions(); k++) {
                                    hStream->Printf("[%i]",introEntry.GetNumberOfElements(k));
                                }
                                hStream->Printf("%s","</TD>");

                                hStream->Printf("%s","</TR>");
                            }

                            hStream->Printf("%s","</TABLE>");
                        }
                        else {
                            hStream->Printf("%s","<SPAN STYLE=\"color: red;background-color: black;\">");
                            StreamString urlT;
                            protocol.GetId(urlT);
                            printf("\nurl = %s, prop = %s\n", urlT.Buffer(),properties.GetName());
                            StreamString className=properties.GetName();
                            hStream->Printf("<A HREF=/%s?Class=%s NAME=+>",urlT.Buffer(),className.Buffer());
                            hStream->Printf("%s","+");
                            hStream->Printf("%s","</A>");
                        }
                    }
                }

                //hStream->Printf("%30s %20s %x\n",item->ClassName(),item->Version(),item->Size());
                hStream->Printf("%s","</TD>");
                hStream->Printf("%s","</TR>");

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

class HttpServiceTestClassTest1: public ReferenceContainer, public DataExportI {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestClassTest1    ();

    virtual ~HttpServiceTestClassTest1();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol);

    virtual bool GetAsText(StreamI &stream, ProtocolI &protocol);

    virtual int32 GetReplyCode(ProtocolI &data);
};

HttpServiceTestClassTest1::HttpServiceTestClassTest1() {

}

HttpServiceTestClassTest1::~HttpServiceTestClassTest1() {

}

bool HttpServiceTestClassTest1::GetAsStructuredData(StreamStructuredDataI &data,
                                                    ProtocolI &protocol) {

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

bool HttpServiceTestClassTest1::GetAsText(StreamI &stream,
                                          ProtocolI &protocol) {
    StreamString hString;
    StreamString *hStream = (&hString);

    hStream->SetSize(0);
    if (!protocol.MoveAbsolute("OutputOptions")) {
        protocol.CreateAbsolute("OutputOptions");
    }
    protocol.Write("Content-Type", "text/html");

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

int32 HttpServiceTestClassTest1::GetReplyCode(ProtocolI &data) {
    return HttpDefinition::HSHCReplyOK;
}



CLASS_REGISTER(HttpServiceTestClassTest1, "1.0")



class HttpServiceTestClassTest2: public ReferenceContainer, public DataExportI, public HttpRealmI {
public:
    CLASS_REGISTER_DECLARATION()

HttpServiceTestClassTest2    ();

    virtual ~HttpServiceTestClassTest2();

    virtual bool GetAsStructuredData(StreamStructuredDataI &data, ProtocolI &protocol);

    virtual bool GetAsText(StreamI &stream, ProtocolI &protocol);

    virtual int32 GetReplyCode(ProtocolI &data);

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

bool HttpServiceTestClassTest2::GetAsStructuredData(StreamStructuredDataI &data,
                                                   ProtocolI &protocol) {

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

bool HttpServiceTestClassTest2::GetAsText(StreamI &stream,
                                         ProtocolI &protocol) {
    StreamString hString;
    StreamString *hStream = (&hString);

    hStream->SetSize(0);
    if (!protocol.MoveAbsolute("OutputOptions")) {
        protocol.CreateAbsolute("OutputOptions");
    }
    protocol.Write("Content-Type", "text/html");

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

int32 HttpServiceTestClassTest2::GetReplyCode(ProtocolI &data) {
    return HttpDefinition::HSHCReplyOK;
}

bool HttpServiceTestClassTest2::Validate(const char8 * const key,
                                        const int32 command,
                                        const uint32 ipNumber) {

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
//#endif

const char8 *config = ""
        "$Application = {"
        "   Class = RealTimeApplication"
        "       +WebRoot = {"
        "           Class = HttpServiceTestWebRoot"
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
        "           Port=4444"
        "           ListenMaxConnections = 255"
        "           Timeout = 0"
        "           AcceptTimeout=1000"
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
    // Auto-generated constructor stub for HttpServiceTest
    // TODO Verify if manual additions are needed
}

HttpServiceTest::~HttpServiceTest() {
    // Auto-generated destructor stub for HttpServiceTest
    // TODO Verify if manual additions are needed
}

bool HttpServiceTest::TestConstructor() {
    HttpServiceTestService httpserver;
    bool ret = (httpserver.GetPort() == 0u);

    ret &= (StringHelper::Compare(httpserver.GetWebRootPath(), "") == 0);
    ret &= httpserver.GetListenMaxConnections() == 0;
    ret &= !httpserver.GetWebRoot().IsValid();
    return ret;

}

bool HttpServiceTest::TestInitialise() {
    StreamString configStream = "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "WebRoot=\"WebRoot\"\n"
            "Port=4444\n"
            "ListenMaxConnections = 3\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 4444;
        }
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "WebRoot") == 0);
        ret &= test.GetListenMaxConnections() == 3;

        ret &= !test.GetWebRoot().IsValid();
    }

    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultWebRoot() {
    StreamString configStream = "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "Port=4444\n"
            "ListenMaxConnections = 3\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 4444;
        }
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "") == 0);
        ret &= test.GetListenMaxConnections() == 3;

        ret &= test.GetWebRoot() == test.Get(0);
    }

    return ret;
}

bool HttpServiceTest::TestInitialise_FalseNoDefaultWebRoot() {
    StreamString configStream = "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "Port=4444\n"
            "ListenMaxConnections = 3\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = !test.Initialise(cdb);
        }
    }
    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultNListenConnections() {
    StreamString configStream = "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "WebRoot=\"WebRoot\"\n"
            "Port=4444\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 4444;
        }
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "WebRoot") == 0);
        ret &= test.GetListenMaxConnections() == 255;

        ret &= !test.GetWebRoot().IsValid();
    }

    return ret;
}

bool HttpServiceTest::TestInitialise_DefaultPort() {
    StreamString configStream = "+WebRoot = {\n"
            "Class = ReferenceContainer\n"
            "}\n"
            "+HttpServerTest = {\n"
            "Class = HttpService\n"
            "WebRoot=\"WebRoot\"\n"
            "ListenMaxConnections = 3\n"
            "HttpRelayURL = \"www.google.it\"\n"
            "Timeout = 0\n"
            "MaxNumberOfThreads=100\n"
            "MinNumberOfThreads=1\n"
            "}";

    ConfigurationDatabase cdb;
    configStream.Seek(0);
    StandardParser parser(configStream, cdb);

    bool ret = parser.Parse();

    HttpServiceTestService test;

    if (ret) {
        ret = cdb.MoveAbsolute("+HttpServerTest");
        if (ret) {
            ret = test.Initialise(cdb);
        }
        if (ret) {
            ret = test.GetPort() == 80;
        }
        ret &= (StringHelper::Compare(test.GetWebRootPath(), "WebRoot") == 0);
        ret &= test.GetListenMaxConnections() == 3;

        ret &= !test.GetWebRoot().IsValid();
    }

    return ret;
}

bool HttpServiceTest::TestStart() {

    const char8 *config1 = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpServiceTestWebRoot"
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpServiceTestClassTest1"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpServiceTestService"
            "           WebRoot = \"Application.WebRoot\""
            "           Port=4444"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout=1000"
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
    ReferenceT<HttpServiceTestService> test = god->Find("Application.HttpServerTest");
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

    if (ret) {
        ret = test->Stop();
    }

    return ret;

}

bool HttpServiceTest::TestStart_InvalidWebRoot() {

    const char8 *config1 = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpServiceTestWebRoot"
            "           +ClassLister = {"
            "               Class = HttpServiceTestClassLister"
            "           }"
            "           +Test1 = {"
            "               Class = HttpServiceTestClassTest1"
            "           }"
            "       }"
            "       +HttpServerTest = {"
            "           Class = HttpServiceTestService"
            "           WebRoot = \"Application.Invalid\""
            "           Port=4444"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout=1000"
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
    ReferenceT<HttpServiceTestService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = !test->Start();
        }
    }

    if (ret) {
        ret = test->Stop();
    }

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
        if (ret) {
            ret = test->Start();
        }
    }

    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    socket.Connect("127.0.0.1", 4444);

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GET /Test1/ HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:4444\r\n");
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

    if (ret) {
        ret = test->Stop();
    }

    Sleep::Sec(1);
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

    /*    while (1)
     ;
     */
    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    socket.Connect("127.0.0.1", 4444);

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GET /Test1/?TextMode=0 HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:4444\r\n");
    socket.Printf("%s", "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\r\n");
    socket.Printf("%s", "Accept: text/html\r\n");
    socket.Printf("%s", "Accept-Encoding: gzip, deflate\r\n");
    socket.Printf("%s", "Connection: keep-alive\r\n\r\n");
    socket.Flush();

    stream.ReadHeader();
    StreamString respBody;
    stream.CompleteReadOperation(&respBody, 1000);

    printf("\n|%s|\n|%s|\n", respBody.Buffer(), "18\r\n\n\r"
           "\"NodeA\": {\n\r"
           "\"NodeB\": {\n\r"
           "C\r\n\n\r"
           "\"var1\": +1\r\n"
           "10\r\n\n\r"
           "},\n\r"
           "\"NodeC\": {\r\n"
           "C\r\n\n\r"
           "\"var2\": -1\r\n"
           "6\r\n\n\r"
           "}\n\r"
           "}\r\n"
           "0\r\n\r\n");

    if (ret) {
        ret = respBody == "18\r\n\n\r"
                "\"NodeA\": {\n\r"
                "\"NodeB\": {\r\n"
                "C\r\n\n\r"
                "\"var1\": +1\r\n"
                "10\r\n\n\r"
                "},\n\r"
                "\"NodeC\": {\r\n"
                "C\r\n\n\r"
                "\"var2\": -1\r\n"
                "6\r\n\n\r"
                "}\n\r"
                "}\r\n"
                "0\r\n\r\n";
    }

    if (ret) {
        ret = test->Stop();
    }

    Sleep::Sec(1);
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

    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    socket.Connect("127.0.0.1", 4444);

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GET /Test1/ HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:4444\r\n");
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

    if (ret) {
        ret = test->Stop();
    }

    Sleep::Sec(1);
    return ret;

}

bool HttpServiceTest::TestClientService_InvalidInterface() {

    bool ret = InitialiseMemoryMapInputBrokerEnviroment(config);
    ObjectRegistryDatabase *god = ObjectRegistryDatabase::Instance();
    ReferenceT<HttpService> test = god->Find("Application.HttpServerTest");
    if (ret) {
        ret = test.IsValid();
        if (ret) {
            ret = test->Start();
        }
    }

    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    socket.Connect("127.0.0.1", 4444);

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GET /TestFake HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:4444\r\n");
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
                "stWebRoot</TITLE></head><BODY BG\r\n"
                "20\r\n"
                "COLOR=\"#ffffff\"><H1>HttpServiceT\r\n"
                "20\r\n"
                "estWebRoot</H1><UL><TABLE>\n"
                "<TR>\n\r\n"
                "20\r\n"
                "<TD>HttpServiceTestClassLister</\r\n"
                "20\r\n"
                "TD><TD><A HREF=\"ClassLister/\">Cl\r\n"
                "20\r\n"
                "assLister</A></TD>\n"
                "</TR>\n"
                "<TR>\n"
                "<T\r\n"
                "20\r\n"
                "D>HttpServiceTestClassTest1</TD>\r\n"
                "20\r\n"
                "<TD><A HREF=\"Test1/\">Test1</A></\r\n"
                "20\r\n"
                "TD>\n"
                "</TR>\n"
                "</TABLE>\n"
                "</UL></BODY>\n\r\n"
                "8\r\n"
                "</html>\n\r\n"
                "0\r\n\r\n";

    }

    if (ret) {
        ret = test->Stop();
    }

    Sleep::Sec(1);
    return ret;

}

bool HttpServiceTest::TestClientService_Authorisation(){

    const char8 *config = ""
            "$Application = {"
            "   Class = RealTimeApplication"
            "       +WebRoot = {"
            "           Class = HttpServiceTestWebRoot"
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
            "           Port=4444"
            "           ListenMaxConnections = 255"
            "           Timeout = 0"
            "           AcceptTimeout=1000"
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
    test.SetServerPort(4444);
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

        ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 20000u);

        if (ret) {
            StreamString userPass = "gferro:1234";
            StreamString encodedUserPass;
            Base64Encoder::Encode(userPass, encodedUserPass);
            test.SetAuthorisation(encodedUserPass.Buffer());

            StreamString readOut;

            ret = test.HttpExchange(readOut, HttpDefinition::HSHCGet, NULL, 10000u);
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

        }

    }

    if (ret) {
        ret = service->Stop();
    }
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

    InternetHost source(4444, "127.0.0.1");
    InternetHost destination(4444, "127.0.0.1");

    TCPSocket socket;

    socket.SetSource(source);
    socket.SetDestination(destination);
    socket.Open();
    socket.Connect("127.0.0.1", 4444);

    HttpProtocol stream(socket);

    StreamString payload;

    socket.Printf("%s", "GOT /TestFake HTTP/1.1\r\n");
    socket.Printf("%s", "Host: localhost:4444\r\n");
    socket.Printf("%s", "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:62.0) Gecko/20100101 Firefox/62.0\r\n");
    socket.Printf("%s", "Accept: text/html\r\n");
    socket.Printf("%s", "Accept-Encoding: gzip, deflate\r\n");
    socket.Printf("%s", "Connection: close\r\n\r\n");
    socket.Flush();


    if (ret) {
        ret = test->Stop();
    }

    Sleep::Sec(1);
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

        InternetHost source(4444 + i, "127.0.0.1");
        InternetHost destination(4444, "127.0.0.1");

        TCPSocket socket;

        socket.SetSource(source);
        socket.SetDestination(destination);
        socket.Open();
        socket.Connect("127.0.0.1", 4444);

        HttpProtocol stream(socket);

        StreamString payload;

        socket.Printf("%s", "GET /Test1/ HTTP/1.1\r\n");
        socket.Printf("%s", "Host: localhost:4444\r\n");
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

    }

    if (ret) {
        ret = test->Stop();
    }

    Sleep::Sec(1);
    return ret;

    return true;
}
