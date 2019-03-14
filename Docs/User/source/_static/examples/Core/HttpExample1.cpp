/**
 * @file HttpExample1.cpp
 * @brief Source file for class HttpExample1
 * @date Oct 26, 2018
 * @author aneto
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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "BasicConsole.h"
#include "ClassRegistryDatabase.h"
#include "ClassRegistryItemT.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "HttpDataExportI.h"
#include "HttpDefinition.h"
#include "HttpService.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "StandardParser.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

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
HttpObjectEx1    () : Object() {

    }

    virtual ~HttpObjectEx1 () {
        if (GetName() != NULL_PTR(const MARTe::char8 *)) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Information, "No more references pointing at %s [%s]. "
                    "The Object will be safely deleted.", GetName(), GetClassProperties()->GetName());
        }
    }

};
CLASS_REGISTER(HttpObjectEx1, "1.0")
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
            "    +ResourcesHtml = {"
            "        Class = HttpDirectoryResource"
            "        BaseDir = \"../../../../../../Resources/HTTP/\""
            "    }"
            "    +ResourcesJS = {"
            "        Class = HttpDirectoryResource"
            "        BaseDir = \"../../../../../../Resources/HTTP/JS/\""
            "    }"
            "}"
            "+WebServer = {"
            "    Class = HttpService"
            "    Port = 8084"
            "    WebRoot = WebRoot"
            "    Timeout = 0"
            "    ListenMaxConnections = 255"
            "    AcceptTimeout = 1000"
            "    MaxNumberOfThreads = 8"
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
    StreamString msg = "Hit any key to exit\n";
    BasicConsole con;
    con.Open(BasicConsoleMode::PerformCharacterInput);
    con.SetSceneSize(64, 1);
    uint32 msgSize = msg.Size();
    con.Write(msg.Buffer(), msgSize);
    char8 ignore;
    msgSize = 1u;
    (void) con.Read(&ignore, msgSize);
    return 0;
}

