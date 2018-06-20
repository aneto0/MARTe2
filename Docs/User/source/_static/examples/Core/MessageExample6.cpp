/**
 * @file MessageExample6.cpp
 * @brief Source file for class MessageExample6
 * @date 17/04/2018
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
 * the class MessageExample6 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "CLASSMETHODREGISTER.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "MessageI.h"
#include "MessageFilter.h"
#include "Object.h"
#include "ObjectRegistryDatabase.h"
#include "RegisteredMethodsMessageFilter.h"
#include "Sleep.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {

void DumpObjectRegistryDatabase(MARTe::ReferenceContainer &rc, MARTe::uint32 depth) {
    MARTe::uint32 i;
    for (i = 0u; i < depth; i++) {
        printf("    ");
    }
    for (i = 0u; i < rc.Size(); i++) {
        MARTe::ReferenceT<MARTe::Object> rco = rc.Get(i);
        if (rco.IsValid()) {
            printf("[%s]\n", rco->GetName());
        }
        MARTe::ReferenceT<MARTe::ReferenceContainer> rcn = rc.Get(i);
        if (rcn.IsValid()) {
            DumpObjectRegistryDatabase(*rcn.operator ->(), depth + 1);
        }
    }
    printf("\n");
}
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);
    StreamString err;

    bool ok = true;
    {
        StreamString configurationCfg = ""
                "+ConfigurationManager = {\n"
                "    Class = ObjectRegistryDatabaseMessageI\n"
                "}";
        //Force the string to be seeked to the beginning.
        configurationCfg.Seek(0LLU);

        REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s", configurationCfg.Buffer());
        ConfigurationDatabase cdb;

        StandardParser parser(configurationCfg, cdb, &err);
        parser.Parse();
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
    }
    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully loaded the configuration file");
        DumpObjectRegistryDatabase(*ObjectRegistryDatabase::Instance(), 0u);
    }
    //Reload a new configuration
    if (ok) {
        StreamString app1Cfg = ""
                "+App1 = {\n"
                "    Class = ReferenceContainer\n"
                "    +A = {\n"
                "        Class = ReferenceContainer\n"
                "        +B = {\n"
                "            Class = ReferenceContainer\n"
                "        }\n"
                "    }\n"
                "}";
        ReferenceT<ConfigurationDatabase> cdbApp1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        app1Cfg.Seek(0LLU);
        StandardParser parserApp1(app1Cfg, *cdbApp1.operator ->(), &err);
        ok = parserApp1.Parse();
        if (ok) {
            //After parsing the tree is pointing at the last leaf
            ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ConfigurationDatabase cdbMsg;
            cdbMsg.Write("Function", "load");
            cdbMsg.Write("Destination", "ConfigurationManager");
            cdbApp1->MoveToRoot();
            msg->Initialise(cdbMsg);
            msg->Insert(cdbApp1);
            ErrorManagement::ErrorType err = MessageI::SendMessage(msg);
            ok = err.ErrorsCleared();
            if (ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully loaded the app1 configuration");
                DumpObjectRegistryDatabase(*ObjectRegistryDatabase::Instance(), 0u);
            }
        }
    }
    //Change the configuration (purge first)
    if (ok) {
        ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        ConfigurationDatabase cdbMsg;
        cdbMsg.Write("Function", "purge");
        cdbMsg.Write("Destination", "ConfigurationManager");
        ReferenceT<ConfigurationDatabase> cdbMsgPayload(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        cdbMsgPayload->Write("Root", "App1");
        msg->Initialise(cdbMsg);
        msg->Insert(cdbMsgPayload);
        ErrorManagement::ErrorType err = MessageI::SendMessage(msg);
        ok = err.ErrorsCleared();
        if (ok) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully purged the app1 configuration");
            DumpObjectRegistryDatabase(*ObjectRegistryDatabase::Instance(), 0u);
        }
    }
    if (ok) {
        StreamString app2Cfg = ""
                "+App1 = {\n"
                "    Class = ReferenceContainer\n"
                "    +A = {\n"
                "        Class = ReferenceContainer\n"
                "        +C = {\n"
                "            Class = ReferenceContainer\n"
                "            +B = {\n"
                "                Class = ReferenceContainer\n"
                "            }\n"
                "        }\n"
                "    }\n"
                "}";

        ReferenceT<ConfigurationDatabase> cdbApp2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
        app2Cfg.Seek(0LLU);
        StandardParser parserApp2(app2Cfg, *cdbApp2.operator ->(), &err);
        ok = parserApp2.Parse();
        if (ok) {
            //After parsing the tree is pointing at the last leaf
            ReferenceT<Message> msg(GlobalObjectsDatabase::Instance()->GetStandardHeap());
            ConfigurationDatabase cdbMsg;
            cdbMsg.Write("Function", "load");
            cdbMsg.Write("Destination", "ConfigurationManager");
            msg->Initialise(cdbMsg);
            msg->Insert(cdbApp2);
            ErrorManagement::ErrorType err = MessageI::SendMessage(msg);
            ok = err.ErrorsCleared();
            if (ok) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Successfully loaded the app2 configuration");
                DumpObjectRegistryDatabase(*ObjectRegistryDatabase::Instance(), 0u);
            }
        }
    }
    //Purge all the Objects!
    MARTe::ObjectRegistryDatabase::Instance()->Purge();
    return 0;
}

