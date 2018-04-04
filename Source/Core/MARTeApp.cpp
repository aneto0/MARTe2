/**
 * @file MARTeApp.cpp
 * @brief Source file for class MARTeApp
 * @date 04/04/2018
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
 * the class MARTeApp (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "Loader.h"
#include "ObjectRegistryDatabase.h"
#include "ProcessorType.h"
#include "StreamI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static MARTe::Bootstrap bootstrap;

void MainErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
    MARTe::StreamString errorCodeStr;
    MARTe::ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errorCodeStr);
    MARTe::StreamString err;
    err.Printf("[%s - %s:%d]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    bootstrap.Printf(err.Buffer());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
int main(int argc, char **argv) {
    using namespace MARTe;
    SetErrorProcessFunction(&MainErrorProcessFunction);

    ConfigurationDatabase loaderParameters;
    StreamString init;
    StreamI &configurationStream = init;
    ErrorManagement::ErrorType ret = bootstrap.ReadParameters(argc, argv, loaderParameters, configurationStream);

    StreamString loaderClass;

    if (ret) {
        uint32 defaultCPUs = 0x1;
        if (!loaderParameters.Read("DefaultCPUs", defaultCPUs)) {
            REPORT_ERROR_STATIC(ErrorManagement::Warning, "DefaultCPUs not specified");
        }
        REPORT_ERROR_STATIC(ErrorManagement::Information, "DefaultCPUs set to %d", defaultCPUs);
        ProcessorType::SetDefaultCPUs(defaultCPUs);

        if (!loaderParameters.Read("Loader", loaderClass)) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Loader not specified");
            ret = ErrorManagement::FatalError;
        }
    }

    //Try to instantiate a new loader.
    ReferenceT<Loader> loaderRef;
    if (ret) {
        loaderRef = Reference(loaderClass.Buffer(), GlobalObjectsDatabase::Instance()->GetStandardHeap());
        if (loaderRef.IsValid()) {
            ret = loaderRef->Initialise(loaderParameters, configurationStream);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not Initialise the loader %s", loaderClass.Buffer());
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not instantiate loader %s", loaderClass.Buffer());
            ret = ErrorManagement::FatalError;
        }
    }
    if (ret) {
        ret = loaderRef->Start();
    }
    if (ret) {
        //Expects to run until an operating specific event exits
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Application starting");
        ret = bootstrap.Run();
        REPORT_ERROR_STATIC(ret, "Application terminated");
    }
    if (ret) {
        ret = loaderRef->Stop();
    }
    MARTe::ObjectRegistryDatabase::Instance()->Purge();

    return 0;
}

