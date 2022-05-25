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

/**
 * @brief A generic MARTe application which uses a Bootstrap to read the input parameters and to setup the execution environment.
 * Note that the Loader to be used is one of the user parameters, so that this main should be sufficiently generic for most use-cases.
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
#include "ReferenceT.h"
#include "StreamI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static MARTe::Bootstrap bootstrap;

/**
 * @brief The default error processing function for the main program.
 * @param[in] errorInfo information about the error.
 * @param[in] errorDescription error textual description.
 */
void MainErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
    using namespace MARTe;
    static const uint32 MAX_ERROR_CODE_STR_SIZE = 16u;
    static char8 errCodeBuffer[MAX_ERROR_CODE_STR_SIZE + 1u];
    (void) MemoryOperationsHelper::Set(&errCodeBuffer[0u], '\0', MAX_ERROR_CODE_STR_SIZE);
    static StreamMemoryReference errCodeStr(errCodeBuffer, MAX_ERROR_CODE_STR_SIZE);
    (void) errCodeStr.Seek(0LLU);
    ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errCodeStr);
    errCodeBuffer[MAX_ERROR_CODE_STR_SIZE] = '\0';

    static char8 errBuffer[MAX_ERROR_MESSAGE_SIZE + 1u];
    (void) MemoryOperationsHelper::Set(&errBuffer[0], '\0', MAX_ERROR_MESSAGE_SIZE);
    static StreamMemoryReference errStr(&errBuffer[0], MAX_ERROR_MESSAGE_SIZE);
    (void) errStr.Seek(0LLU);
    (void) errStr.Printf("[%s - %s:%d]: %s", errCodeBuffer, errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    errBuffer[MAX_ERROR_MESSAGE_SIZE] = '\0';
    bootstrap.Printf(errBuffer);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
extern "C" {
    int _main(int argc, char **argv) {
        using namespace MARTe;
        SetErrorProcessFunction(&MainErrorProcessFunction);

        ConfigurationDatabase loaderParameters;
        StreamI *configurationStream = NULL_PTR(StreamI *);

        ErrorManagement::ErrorType ret = bootstrap.ReadParameters(argc, argv, loaderParameters);

        
        if (ret) {
            ret = bootstrap.GetConfigurationStream(loaderParameters, configurationStream);
            if (ret) {
                ret.fatalError = (configurationStream == NULL_PTR(StreamI *));
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not GetConfigurationStream.");
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not ReadParameters.");
        }

        StreamString loaderClass;
        if (ret) {
            ret.fatalError = !loaderParameters.Read("Loader", loaderClass);
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Loader not specified");
            }
        }

        //Try to instantiate a new loader.
        ReferenceT<Loader> loaderRef;
        if (ret) {
            loaderRef = Reference(loaderClass.Buffer(), GlobalObjectsDatabase::Instance()->GetStandardHeap());
            if (loaderRef.IsValid()) {
                ret = loaderRef->Configure(loaderParameters, *configurationStream);
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not Initialise the loader with name %s", loaderClass.Buffer());
                }
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Could not instantiate loader with name %s", loaderClass.Buffer());
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

        loaderRef = Reference();
        MARTe::ObjectRegistryDatabase::Instance()->Purge();
        loaderParameters.Purge();
        ClassRegistryDatabase *crd = ClassRegistryDatabase::Instance();
        uint32 numberOfItems = crd->GetSize();
        uint32 i;
        //List all the classes and the related number of instances that are still alive
        for (i=0u; i<numberOfItems; i++) {
            const ClassRegistryItem *cri = crd->Peek(i);
            if (cri != NULL_PTR(const ClassRegistryItem *)) {
                const ClassProperties *cp = cri->GetClassProperties();
                if (cp != NULL_PTR(const ClassProperties *)) {
                    ErrorManagement::ErrorType err = ErrorManagement::Information;
                    if (cri->GetNumberOfInstances() != 0u) {
                        err = ErrorManagement::Warning;
                    }
                    REPORT_ERROR_STATIC(err, "[%s] - instances: %d", cp->GetName(), cri->GetNumberOfInstances());
                }
            }
        }
        return 0;
    }
}
/**
 * @brief Main function.
 * @param[in] argc the number of arguments to be parsed by the MARTe::Bootstrap.
 * @param[in] argv the value of the arguments to be parsed by the MARTe::Bootstrap.
 * @return 0
 */
int main(int argc, char **argv) {
   bootstrap.Main(_main, argc, argv);
   return 0;
}
