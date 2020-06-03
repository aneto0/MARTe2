/**
 * @file HttpExample1.cpp
 * @brief Source file for class HttpExample1
 * @date 26/10/2018
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
 * the class HttpExample1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <math.h>
#include <signal.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "BasicConsole.h"
#include "ClassRegistryDatabase.h"
#include "ClassRegistryItemT.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "HighResolutionTimer.h"
#include "HttpDataExportI.h"
#include "HttpDefinition.h"
#include "HttpService.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "StandardParser.h"
#include "StreamString.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static bool keepRunning = true;
static MARTe::float32 timeStepPeriod = 1e-2;
static void StopApp(int sig) {
    keepRunning = false;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe2Tutorial {
/**
 * @brief A MARTe::Object class that will expose its properties using the DataExportI interface.
 */
class HttpObjectEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
    HttpObjectEx1() : Object() {
        counter = 10u;
    }

    virtual ~HttpObjectEx1 () {
        if (GetName() != NULL_PTR(const MARTe::char8 *)) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    virtual bool ExportData(MARTe::StructuredDataI & data) {
        bool ok = Object::ExportData(data);
        counter++;
        if (counter == 30u) {
            counter = 10u;
        }
        if (ok) {
            ok = data.Write("Counter", counter);
        }
        return ok;
    }

private:
    MARTe::uint32 counter;

};
CLASS_REGISTER(HttpObjectEx1, "1.0")

/**
 * @brief A MARTe::Object class that will expose its properties using the HttpDataExportI interface.
 */
class HttpObjectEx2: public MARTe::Object, public MARTe::HttpDataExportI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
    HttpObjectEx2() : Object() {
        counter = 10u;
    }

    virtual ~HttpObjectEx2 () {
        if (GetName() != NULL_PTR(const MARTe::char8 *)) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    virtual bool GetAsStructuredData(MARTe::StreamStructuredDataI &data, MARTe::HttpProtocol &protocol) {
        bool ok = HttpDataExportI::GetAsStructuredData(data, protocol);
        if (ok) {
            ok = MARTe::Object::ExportData(data);
        }
        counter++;
        if (counter == 30u) {
            counter = 10u;
        }
        if (ok) {
            ok = data.Write("Counter", counter);
        }
        return ok;

    }

    virtual bool GetAsText(MARTe::StreamI &stream, MARTe::HttpProtocol &protocol) {
        bool ok = HttpDataExportI::GetAsText(stream, protocol);
        MARTe::StreamString txt;
        if (ok) {
            ok = txt.Printf("<html><body><h1>Counter = %d</h1></body></html>\n", counter);
        }
        if (ok) {
            MARTe::uint32 nBytes = txt.Size();
            ok = stream.Write(txt.Buffer(), nBytes);
        }
        return ok;
    }

private:
    MARTe::uint32 counter;

};
CLASS_REGISTER(HttpObjectEx2, "1.0")


/**
 * @brief A MARTe::Object class that will expose its properties using the DataExportI interface.
 */
class HttpPendulumEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief NOOP.
     */
    HttpPendulumEx1() : Object() {
        velocity = 0;
        angle = 3.1415 / 3;
        gravity = -9.8;
        length = 1;
        timeStepCounter = 0;
    }

    virtual ~HttpPendulumEx1 () {
        if (GetName() != NULL_PTR(const MARTe::char8 *)) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

    virtual bool ExportData(MARTe::StructuredDataI & data) {
        bool ok = Object::ExportData(data);
        if (ok) {
            ok = data.Write("angle", angle);
        }
        return ok;
    }

    void Execute() {
        MARTe::float32 acceleration = gravity / length * sinf(angle);
        velocity += acceleration * timeStepPeriod;
        angle += velocity * timeStepPeriod;
        timeStepCounter++;
        //REPORT_ERROR_STATIC(MARTe::ErrorManagement::Debug, "Time = %f Angle = %f", timeStepPeriod, angle);
    }

private:
    MARTe::float32 length;
    MARTe::float32 velocity;
    MARTe::float32 angle;
    MARTe::float32 gravity;
    MARTe::uint32 timeStepCounter;

};
CLASS_REGISTER(HttpPendulumEx1, "1.0")
}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString config = ""
            "+WebRoot = {"
            "    Class = HttpObjectBrowser"
            "    Root = \".\""
            "    +TheRoot = {"
            "        Root = \"/\""
            "        Class = HttpObjectBrowser"
            "    }"
            "    +ARootObj1 = {"
            "        Class = HttpObjectBrowser"
            "        Root = \".\""
            "        +AChildObj1 = {"
            "            Class = HttpObjectEx1"
            "        }"
            "        +Obj2 = {"
            "            Class = HttpObjectEx1"
            "        }"
            "    }"
            "    +ARootObj2 = {"
            "        Class = HttpObjectEx1"
            "    }"
            "    +ARootObj3 = {"
            "        Class = HttpObjectEx1"
            "    }"
            "    +Pendulum = {"
            "        Class = HttpPendulumEx1"
            "    }"
            "    +ResourcesHtml = {"
            "        Class = HttpDirectoryResource"
            "        BaseDir = \"../../../../../../Resources/HTTP/\""
            "    }"
            "    +ResourcesJS = {"
            "        Class = HttpDirectoryResource"
            "        BaseDir = \"../../../../../../Resources/HTTP/JS/\""
            "    }"
            "    +ResourcesJS2 = {"
            "        Class = HttpDirectoryResource"
            "        BaseDir = \"../Resources/HTTP/\""
            "    }"
            "    +ThreadsInformationQuery = {"
            "        Class = ThreadsInformationQuery"
            "    }"
            "}"
            "+WebServer = {"
            "    Class = HttpService"
            "    Port = 8084"
            "    WebRoot = WebRoot"
            "    Timeout = 0"
            "    ListenMaxConnections = 255"
            "    AcceptTimeout = 1000"
            "    MaxNumberOfThreads = 64"
            "    MinNumberOfThreads = 1"
            "}";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s", config.Buffer());
    ConfigurationDatabase cdb;
    StreamString err;
    //Force the string to be seeked to the beginning.
    config.Seek(0LLU);
    StandardParser parser(config, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        //After parsing the tree is pointing at the last leaf
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    else {
        StreamString errPrint;
        errPrint.Printf("Failed to parse %s", err.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, errPrint.Buffer());
    }

    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully loaded the configuration file");
    }
    ReferenceT<HttpService> service = ObjectRegistryDatabase::Instance()->Find("WebServer");
    if (ok) {
        ok = service.IsValid();
    }
    if (ok) {
        ok = service->Start();
    }
    StreamString msg = "Type Ctrl + C to exit\n";
    BasicConsole con;
    con.Open(BasicConsoleMode::PerformCharacterInput);
    con.SetSceneSize(64, 1);
    uint32 msgSize = msg.Size();
    con.Write(msg.Buffer(), msgSize);
    ReferenceT<HttpPendulumEx1> pendulum = ObjectRegistryDatabase::Instance()->Find("WebRoot.Pendulum");
    signal(SIGTERM, StopApp);
    signal(SIGINT, StopApp);
    if (ok) {
        ok = pendulum.IsValid();
    }
    if (ok) {
        while (keepRunning) {
            pendulum->Execute();
            Sleep::Sec(timeStepPeriod);
        }
    }
    if (ok) {
        ok = service->Stop();
    }
    return 0;
}

