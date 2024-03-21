/**
 * @file SingleThreadServiceExample1.cpp
 * @brief Source file for class SingleThreadServiceExample1
 * @date 24/04/2018
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
 * the class SingleThreadServiceExample1 (public, protected, and private). Be aware that some
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
#include "BasicTCPSocket.h"
#include "ConfigurationDatabase.h"
#include "ErrorLoggerExample.h"
#include "EmbeddedServiceMethodBinderT.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "SingleThreadService.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe2Tutorial {
/**
 * @brief a class that contains a single thread service with an embedded binder.
 */
class SingleThreadServiceExample1: public MARTe::Object, public MARTe::EmbeddedServiceMethodBinderT<SingleThreadServiceExample1> {

public:
    CLASS_REGISTER_DECLARATION()SingleThreadServiceExample1() :
    MARTe::Object(), MARTe::EmbeddedServiceMethodBinderT<SingleThreadServiceExample1>(*this, &SingleThreadServiceExample1::ThreadCallback), service(
            *this) {
        counter = 0u;
    }

    virtual ~SingleThreadServiceExample1() {
        using namespace MARTe;
        if (GetName() != NULL) {
            REPORT_ERROR_STATIC(ErrorManagement::Information, "No more references "
                    "pointing at %s [%s]. The Object will "
                    "be safely deleted.",
                    GetName(), GetClassProperties()->GetName());
        }
    }

    virtual bool Initialise(MARTe::StructuredDataI &data) {
        bool ok = MARTe::Object::Initialise(data);
        if (ok) {
            ok = service.Initialise(data);
        }
        return ok;
    }

    MARTe::ErrorManagement::ErrorType ThreadCallback(MARTe::ExecutionInfo &info) {
        using namespace MARTe;
        ErrorManagement::ErrorType err;
        if (info.GetStage() == ExecutionInfo::StartupStage) {
            REPORT_ERROR(ErrorManagement::Information, "Callback called with ExecutionInfo::StartupStage");
        }
        else if (info.GetStage() == ExecutionInfo::MainStage) {
            REPORT_ERROR(ErrorManagement::Information, "Callback called with ExecutionInfo::MainStage");
            if (counter == 0u) {
                err = ErrorManagement::Completed;
            }
            else {
                err = ErrorManagement::FatalError;
            }
        }
        else if (info.GetStage() == ExecutionInfo::TerminationStage) {
            REPORT_ERROR(ErrorManagement::Information, "Callback called with ExecutionInfo::TerminationStage");
            err = ErrorManagement::NoError;
            counter++;
        }
        else if (info.GetStage() == ExecutionInfo::BadTerminationStage) {
            REPORT_ERROR(ErrorManagement::Information, "Callback called with ExecutionInfo::BadTerminationStage");
            counter++;
            //Simulate error to force killing
            while (true) {
                Sleep::Sec(0.2);
            }
        }
        else if (info.GetStage() == ExecutionInfo::AsyncTerminationStage) {
            REPORT_ERROR(ErrorManagement::Information, "Callback called with ExecutionInfo::AsyncTerminationStage");
            err = ErrorManagement::NoError;
        }
        Sleep::Sec(0.2);
        return err;
    }

    MARTe::SingleThreadService service;
    MARTe::uint32 counter;
};

CLASS_REGISTER(SingleThreadServiceExample1, "")
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    using namespace MARTe2Tutorial;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString configurationCfg = ""
            "+SingleThreadServiceExample1 = {\n"
            "    Class = SingleThreadServiceExample1\n"
            "    Timeout = 100\n" //100 ms
            "    CPUMask = 0x1\n"
            "}";

    REPORT_ERROR_STATIC(ErrorManagement::Information, "Loading CFG:\n%s", configurationCfg.Buffer());
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
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, errPrint.Buffer());
    }

    ReferenceT<SingleThreadServiceExample1> singleThreadService;
    if (ok) {
        singleThreadService = ord->Find("SingleThreadServiceExample1");
        ok = singleThreadService.IsValid();
    }
    if (ok) {
        singleThreadService->service.Start();
        while (singleThreadService->counter != 2) {
            Sleep::Sec(0.2);
        }
        singleThreadService->service.Stop();
        Sleep::Sec(0.2);
        //Kill the service
        singleThreadService->service.Stop();
    }
    //Purge all the Objects!
    ObjectRegistryDatabase::Instance()->Purge();
    return 0;
}

