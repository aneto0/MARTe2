/**
 * @file LoggerServiceExample1.cpp
 * @brief Source file for class LoggerServiceExample1
 * @date 04/04/2018
 * @author Andre' Neto
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
 * the class LoggerServiceExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ClassRegistryDatabase.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "JsonParser.h"
#include "Matrix.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "Reference.h"
#include "ReferenceT.h"
#include "StandardParser.h"
#include "StreamString.h"
#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

/**
 * @brief A MARTe::Object that will print some logging information.
 */
class LoggerEx1: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()

LoggerEx1    () {
        using namespace MARTe;
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Constructing an instance of LoggerEx1");
    }

    virtual ~LoggerEx1 () {
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(MARTe::ErrorManagement::Debug, "Destroying an instance of LoggerEx1");
        }
    }

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        using namespace MARTe;
        bool ok = Object::Initialise(data);

        REPORT_ERROR(MARTe::ErrorManagement::Information, "Initialised called");

        float32 a;
        if (ok) {
            ok = data.Read("a", a);
        }
        if (ok) {
            REPORT_ERROR(MARTe::ErrorManagement::Information, "a = %f", a);
        }
        else {
            REPORT_ERROR(MARTe::ErrorManagement::ParametersError, "Could not read the value of a");
        }

        return ok;
    }

};
CLASS_REGISTER(LoggerEx1, "")
}

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    //This will be later overridden by the LoggerService
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+LoggerService = {\n"
            "    Class = LoggerService\n"
            "    CPUs = 0x1\n"
            "    StackSize = 32768\n"
            "    NumberOfLogPages = 128\n"
            "    +ConsoleLogger1 = {\n"
            "        Class = ConsoleLogger\n"
            "        Format = \"EtOoFmC\"\n"
            "        PrintKeys = 1\n"
            "    }\n"
            "}"
            "+LEx1 = {\n"
            "    Class = LoggerEx1\n"
            "    a=5.3\n"
            "}\n"
            "+LEx2 = {\n"
            "   Class = LoggerEx1\n"
            "   a = 2"
            "}\n";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s",
                        configurationCfg.Buffer());
    ConfigurationDatabase cdb;
    StreamString err;
    //Force the string to be seeked to the beginning.
    configurationCfg.Seek(0LLU);
    StandardParser parser(configurationCfg, cdb, &err);
    bool ok = parser.Parse();
    if (ok) {
        //After parsing the tree is pointing at the last leaf
        cdb.MoveToRoot();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    else {
        StreamString errPrint;
        errPrint.Printf("Failed to parse %s", err.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                            errPrint.Buffer());
    }

    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information,
                            "Successfully loaded the configuration file");
    }

    //Purge all the Objects!
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return 0;
}

