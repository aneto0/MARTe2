/**
 * @file HttpDataMonitorTest.cpp
 * @brief Source file for class HttpDataMonitorTest
 * @date 05/08/2025
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
 * the class HttpDataMonitorTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HttpDataMonitorTest.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
#include "StreamStructuredData.h"
#include "JsonPrinter.h"
#include "TCPSocket.h"
#include "HttpProtocol.h"
#include "ObjectRegistryDatabase.h"
#include "HttpService.h"
#include "HttpClient.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class HttpDataMonitorTestObject: public MARTe::Object {
public:CLASS_REGISTER_DECLARATION()

    HttpDataMonitorTestObject() {
    }

    bool ExportData(StructuredDataI &data) {
        if (data.CreateAbsolute("Signals")) {
            data.Write("Signal1", "1");
            data.MoveToAncestor(1u);
        }
        return true;
    }

};
CLASS_REGISTER(HttpDataMonitorTestObject, "1.0")

HttpDataMonitorTest::HttpDataMonitorTest() {

}

HttpDataMonitorTest::~HttpDataMonitorTest() {

}

bool HttpDataMonitorTest::TestConstructor() {
    HttpDataMonitor *test = new HttpDataMonitor;
    bool ret = (test != NULL);
    if (ret) {
        delete test;
    }
    return ret;
}

bool HttpDataMonitorTest::TestDestructor() {
    return TestConstructor();
}

bool HttpDataMonitorTest::TestInitialise() {

    StreamString cfg = ""
            "Plot1 = {\n"
            "    Test@Functions@GAM1@OutputSignals@Counter = {\n"
            "        Signal = OutputSignals.Counter\n"
            "        Component = Test.Functions.GAM1\n"
            "    }\n"
            "}\n"
            "Plot2 = {\n"
            "    Test@Functions@Constant@OutputSignals = {\n"
            "        Signal = OutputSignals\n"
            "        Component = Test.Functions.Constant\n"
            "    }\n"
            "}\n"
            "Plot3 = {\n"
            "    Test@Data@DDB1@Signals@Value0 = {"
            "        Signal = Signals.Value0\n"
            "        Component = Test.Data.DDB1\n"
            "    }"
            "    Test@Data@DDB1@Signals@Value1 = {"
            "        Signal = Signals.Value1\n"
            "        Component = Test.Data.DDB1\n"
            "    }"
            "}";
    ConfigurationDatabase data;

    cfg.Seek(0ull);
    StandardParser parser(cfg, data);
    bool ret = parser.Parse();
    if (ret) {
        data.MoveToRoot();
        HttpDataMonitor test;
        ret = test.Initialise(data);
    }
    return ret;
}

bool HttpDataMonitorTest::TestInitialise_False_NoSignals() {
    StreamString cfg = ""
            "Plot1 = {\n"
            "    Signal1 = {\n"
            "        Component = \"Test.Functions.GAM1\"\n"
            "    }\n"
            "}";
    ConfigurationDatabase data;

    cfg.Seek(0ull);
    StandardParser parser(cfg, data);
    bool ret = parser.Parse();
    if (ret) {
        data.MoveToRoot();
        HttpDataMonitor test;
        ret = !test.Initialise(data);
    }
    return ret;
}

bool HttpDataMonitorTest::TestInitialise_False_NoComponents() {
    StreamString cfg = ""
            "Plot1 = {\n"
            "    Signal1 = {\n"
            "        Signal = \"OutputSignals.Counter\"\n"
            "    }\n"
            "}";
    ConfigurationDatabase data;

    cfg.Seek(0ull);
    StandardParser parser(cfg, data);
    bool ret = parser.Parse();
    HttpDataMonitor test;
    if (ret) {
        data.MoveToRoot();
        ret = !test.Initialise(data);
    }

    return ret;
}


bool HttpDataMonitorTest::TestGetAsStructuredData() {
    StreamString cfg = ""
            "+TestObject = {\n"
            "    Class = HttpDataMonitorTestObject"
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
            "    +HttpMonitor = {\n"
            "        Class = HttpDataMonitor\n"
            "        Plot1 = {\n"
            "            TestObjectSignals@Signal1 = {\n"
            "                Component = \"TestObject\"\n"
            "                Signal = \"Signals.Signal1\"\n"
            "            }\n"
            "        }\n"
            "    }"
            "}\n";

    ConfigurationDatabase data;

    cfg.Seek(0ull);
    StandardParser parser(cfg, data);
    bool ret = parser.Parse();
    if (ret) {
        data.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Initialise(data);
        ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
        ret = service.IsValid();
        if (ret) {
            ret = service->Start();
            if (ret) {
                HttpClient test;
                test.SetServerAddress("127.0.0.1");
                test.SetServerPort(9094);
                test.SetServerUri("/HttpMonitor");
                StreamString reply;
                ret = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);

                //we can check till the timestamp block
                StreamString expectedReply = "18\r\n{\n\r\"Name\": \"HttpMonitor\"\r\n"
                        "1D\r\n,\n\r\"Class\": \"HttpDataMonitor\"\r\nD\r\n\n\r,\"Plots\": {\r\n"
                        "C\r\n\n\r\"Plot1\": {\r\nE\r\n\n\r\"Signals\": {\r\n"
                        "20\r\n\n\r\"TestObjectSignals@Signal1\": \"\r\n2\r\n1\"\r\n"
                        "3\r\n\n\r}\r\n";

                for (uint32 i = 0u; i < expectedReply.Size(); i++) {
                    if (reply[i] != expectedReply[i]) {
                        ret = false;
                        printf("reply[%d]=|%c,%u| - expected[%d]=|%c,%u|\n", i, reply[i], (uint32) reply[i], i, expectedReply[i], (uint32) expectedReply[i]);
                    }
                }
            }
            service->Stop();
            ObjectRegistryDatabase::Instance()->Purge();
        }
    }
    return ret;
}

bool HttpDataMonitorTest::TestGetAsStructuredData_Dir(){
    StreamString cfg = ""
            "+TestObject = {\n"
            "    Class = HttpDataMonitorTestObject"
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
            "    +HttpMonitor = {\n"
            "        Class = HttpDataMonitor"
            "        Plot1 = {\n"
            "            TestObjectSignals = {\n"
            "                Component = \"TestObject\"\n"
            "                Signal = \"Signals\"\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n";

    ConfigurationDatabase data;

    cfg.Seek(0ull);
    StandardParser parser(cfg, data);
    bool ret = parser.Parse();
    if (ret) {
        data.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Initialise(data);
        ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
        ret = service.IsValid();
        if (ret) {
            ret = service->Start();
            if (ret) {
                HttpClient test;
                test.SetServerAddress("127.0.0.1");
                test.SetServerPort(9094);
                test.SetServerUri("/HttpMonitor");
                StreamString reply;
                ret = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);

                //we can check till the timestamp block
                StreamString expectedReply = "18\r\n{\n\r\"Name\": \"HttpMonitor\"\r\n"
                        "1D\r\n,\n\r\"Class\": \"HttpDataMonitor\"\r\nD\r\n\n\r,\"Plots\": {\r\n"
                        "C\r\n\n\r\"Plot1\": {\r\nE\r\n\n\r\"Signals\": {\r\n"
                        "20\r\n\n\r\"TestObjectSignals@Signal1\": \"\r\n2\r\n1\"\r\n"
                        "3\r\n\n\r}\r\n";

                for (uint32 i = 0u; i < expectedReply.Size(); i++) {
                    if (reply[i] != expectedReply[i]) {
                        ret = false;
                        printf("reply[%d]=|%c,%u| - expected[%d]=|%c,%u|\n", i, reply[i], (uint32) reply[i], i, expectedReply[i], (uint32) expectedReply[i]);
                    }
                }
            }
            service->Stop();
            ObjectRegistryDatabase::Instance()->Purge();
        }
    }
    return ret;
}


bool HttpDataMonitorTest::TestGetAsStructuredData_False_InvalidComponent() {
    StreamString cfg = ""
            "+TestObject = {\n"
            "    Class = HttpDataMonitorTestObject"
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
            "    +HttpMonitor = {\n"
            "        Class = HttpDataMonitor"
            "        Plot1 = {\n"
            "            TestObjectInvalid@Signals@Signal1 = {\n"
            "                Component = \"TestObjectInvalid\"\n"
            "                Signal = \"Signals.Signal1\"\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n";

    ConfigurationDatabase data;

    cfg.Seek(0ull);
    StandardParser parser(cfg, data);
    bool ret = parser.Parse();
    if (ret) {
        data.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Initialise(data);
        ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
        ret = service.IsValid();
        if (ret) {
            ret = service->Start();
            if (ret) {
                HttpClient test;
                test.SetServerAddress("127.0.0.1");
                test.SetServerPort(9094);
                test.SetServerUri("/HttpMonitor");
                StreamString reply;
                ret = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);

                //we can check till the timestamp block
                StreamString expectedReply = "18\r\n{\n\r\"Name\": \"HttpMonitor\"\r\n"
                        "1D\r\n,\n\r\"Class\": \"HttpDataMonitor\"\r\nD\r\n\n\r,\"Plots\": {\r\n"
                        "3\r\n\n\r}\r\n1\r\n}\r\n0\r\n\r\n";
                ret = (reply == expectedReply);
            }
            service->Stop();
            ObjectRegistryDatabase::Instance()->Purge();
        }
    }
    return ret;
}

bool HttpDataMonitorTest::TestGetAsText() {
    StreamString cfg = ""
            "+TestObject = {\n"
            "    Class = HttpDataMonitorTestObject"
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
            "    +HttpMonitor = {\n"
            "        Class = HttpDataMonitor"
            "        Plot1 = {\n"
            "            TestObject@Signals@Signal1 = {\n"
            "                Component = \"TestObject\"\n"
            "                Signal = \"Signals.Signal1\"\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "}\n";

    ConfigurationDatabase data;

    cfg.Seek(0ull);
    StandardParser parser(cfg, data);
    bool ret = parser.Parse();
    if (ret) {
        data.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Initialise(data);
        ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("HttpService1");
        ret = service.IsValid();
        if (ret) {
            ret = service->Start();
            if (ret) {
                HttpClient test;
                test.SetServerAddress("127.0.0.1");
                test.SetServerPort(9094);
                test.SetServerUri("/HttpMonitor");
                StreamString reply;
                ret = test.HttpExchange(reply, HttpDefinition::HSHCGet, NULL, 1000u);

                //we can check till the timestamp block
                StreamString expectedReply = "0\r\n\r\n";

                ret = (reply == expectedReply);
            }
            service->Stop();
            ObjectRegistryDatabase::Instance()->Purge();
        }
    }
    return ret;
}

