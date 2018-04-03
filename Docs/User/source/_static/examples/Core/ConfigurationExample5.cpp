/**
 * @file ConfigurationExample4.cpp
 * @brief Source file for class ConfigurationExample4
 * @date 14/03/2018
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
 * the class ConfigurationExample4 (public, protected, and private). Be aware that some
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

int main(int argc, char **argv) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+A = {\n"
            "    Class = ReferenceContainer\n"
            "    $B = {\n"
            "        Class = ReferenceContainer\n"
            "        +C = {"
            "            Class = ReferenceContainer\n"
            "            +F = {"
            "                Class = ReferenceContainer\n"
            "            }\n"
            "        }\n"
            "        +G = {"
            "            Class = ReferenceContainer\n"
            "            +H = {"
            "                Class = ReferenceContainer\n"
            "            }\n"
            "        }\n"
            "    }\n"
            "    +D = {\n"
            "        Class = ReferenceContainer\n"
            "        +E = {\n"
            "            Class = ReferenceContainer\n"
            "        }\n"
            "    }\n"
            "}\n";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s",
                        configurationCfg.Buffer());
    ConfigurationDatabase cdb;
    StreamString err;
    //Force the string to be seeked to the beginning.
    configurationCfg.Seek(0LLU);
    StandardParser parser(configurationCfg, cdb, &err);
    bool ok = parser.Parse();
    ObjectRegistryDatabase *ord = ObjectRegistryDatabase::Instance();
    if (ok) {
        //After parsing the tree is pointing at the last leaf
        cdb.MoveToRoot();
        ok = ord->Initialise(cdb);
    }
    else {
        StreamString errPrint;
        errPrint.Printf("Failed to parse %s", err.Buffer());
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError,
                            errPrint.Buffer());
    }

    if (ok) {
        //Find A.D.E
        const char8 * const path = "A.D.E";
        ReferenceT<ReferenceContainer> rc = ord->Find(path);
        ok = rc.IsValid();
        if (ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Found %s", path);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                                "Could not find %s !", path);
        }
    }
    if (ok) {
        //Find A.B.E
        const char8 * const path = "A.B.E";
        ReferenceT<ReferenceContainer> rc = ord->Find(path);
        ok = !rc.IsValid();
        if (ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Information,
                                "Could not find %s as expected", path);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                                "Should not have found %s !", path);
        }
    }
    if (ok) {
        //Find D.E as relative path to F
        const char8 * const path = "A.B.C.F";
        ReferenceT<ReferenceContainer> rc = ord->Find(path);
        ok = rc.IsValid();
        if (ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Found %s", path);
            //: Moves up to the next domain (which $B)
            const char8 * const path = ":G.H";
            //Note that the search is relative to rc (which is currently @ A.B.C.F)
            rc = ord->Find(path, rc);
            ok = rc.IsValid();
            if (ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Found %s",
                                    path);
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                                    "Could not find %s !", path);
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                                "Could not find %s !", path);
        }
    }

    return 0;
}

