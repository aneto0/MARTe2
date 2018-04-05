/**
 * @file RealTimeLoader.cpp
 * @brief Source file for class RealTimeLoader
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
 * the class RealTimeLoader (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include "RealTimeLoader.h"
#include "MessageI.h"
#include "ObjectRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

RealTimeLoader::RealTimeLoader() {
}

RealTimeLoader::~RealTimeLoader() {

}

ErrorManagement::ErrorType RealTimeLoader::Initialise(StructuredDataI& data, StreamI &configuration) {
    ErrorManagement::ErrorType ret = Loader::Initialise(data, configuration);

    ObjectRegistryDatabase *objDb = ObjectRegistryDatabase::Instance();
    uint32 nOfObjs = objDb->Size();
    uint32 n;
    bool found = false;
    for (n = 0u; (ret) && (n < nOfObjs) && (!found); n++) {
        rtApp = objDb->Get(n);
        found = rtApp.IsValid();
        if (found) {
            ret.initialisationError = !rtApp->ConfigureApplication();
            if (!ret) {
                REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to Configure RealTimeApplication");
            }
        }
    }
    if (found) {
        (void) data.Read("FirstState", firstState);
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Could not find a RealTimeApplication");
        ret = ErrorManagement::ParametersError;
    }

    return ret;
}

ErrorManagement::ErrorType RealTimeLoader::Start() {
    ErrorManagement::ErrorType ret;
    if (firstState.Size() > 0u) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Preparing state %s ", firstState);
        ret.initialisationError = !rtApp->PrepareNextState(firstState.Buffer());
        if (ret) {
            ret = rtApp->StartNextStateExecution();
            if (ret) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Started application in state %s ", firstState);
            }
            else {
                REPORT_ERROR_STATIC(ret, "Failed to StartNextStateExecution for state %s ", firstState);
            }
        }
        else {
            REPORT_ERROR_STATIC(ret, "Failed to PrepareNextState for state %s ", firstState);
        }
    }
    else {
        ret = Loader::Start();
    }
    return ret;
}

ErrorManagement::ErrorType RealTimeLoader::Stop() {
    ErrorManagement::ErrorType ret = rtApp.IsValid();
    if (ret) {
        ret = rtApp->StopCurrentStateExecution();
        if (ret) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to StopCurrentStateExecution");
        }
    }
    if (ret) {
        ret = Loader::Stop();
    }
    return ret;
}

CLASS_REGISTER(RealTimeLoader, "")

}

