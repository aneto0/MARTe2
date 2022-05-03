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

RealTimeLoader::RealTimeLoader() :
        Loader() {
}

RealTimeLoader::~RealTimeLoader() {

}

ErrorManagement::ErrorType RealTimeLoader::Configure(StructuredDataI& data, StreamI &configuration) {
    ErrorManagement::ErrorType ret = Loader::Configure(data, configuration);

    ObjectRegistryDatabase *objDb = ObjectRegistryDatabase::Instance();
    uint32 nOfObjs = objDb->Size();
    uint32 found = 0u;
    //start all the rtapp
    for (uint32 n = 0u; (ret) && (n < nOfObjs); n++) {
        ReferenceT<RealTimeApplication> rtApp = objDb->Get(n);
        if (rtApp.IsValid()) {
            ret.initialisationError = !rtApps.Insert(rtApp);
            if (ret.ErrorsCleared()) {
                ret.initialisationError = !rtApp->ConfigureApplication();
                if (!ret) {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to Configure RealTimeApplication");
                }
            }
            found++;
        }
    }
    if (found > 0u) {
        if (data.Read("FirstState", firstState)) {
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Could not find a RealTimeApplication");
        ret = ErrorManagement::ParametersError;
    }

    return ret;
}

ErrorManagement::ErrorType RealTimeLoader::Start() {
    ErrorManagement::ErrorType err;

    uint32 nApps = rtApps.Size();
    (void) firstState.Seek(0ull);
    if (firstState.Size() > 0u) {
        for (uint32 i = 0u; (err.ErrorsCleared()) && (i < nApps); i++) {
            ReferenceT<RealTimeApplication> rtApp = rtApps.Get(i);
            StreamString destination;
            char8 term;
            err.fatalError = !firstState.GetToken(destination, ":", term);
            if (err.ErrorsCleared()) {
                REPORT_ERROR_STATIC(ErrorManagement::Information, "Preparing state %s ", destination.Buffer());
                err.initialisationError = !rtApp->PrepareNextState(destination.Buffer());
                if (err.ErrorsCleared()) {
                    err = rtApp->StartNextStateExecution();
                    if (err.ErrorsCleared()) {
                        REPORT_ERROR_STATIC(ErrorManagement::Information, "Started application in state %s ", destination.Buffer());
                    }
                    else {
                        REPORT_ERROR_STATIC(err, "Failed to StartNextStateExecution for state %s ", destination.Buffer());
                    }
                }
                else {
                    REPORT_ERROR_STATIC(err, "Failed to PrepareNextState for state %s ", destination.Buffer());
                }
            }
            else {
                const uint32 ii = i;
                REPORT_ERROR_STATIC(err, "Failed to get the first state for the application %d ", ii);
            }
        }
    }
    else {
        err = Loader::Start();
    }
    return err;
}

ErrorManagement::ErrorType RealTimeLoader::Stop() {
    ErrorManagement::ErrorType ret = true;
    uint32 nApps = rtApps.Size();

    if (ret.ErrorsCleared()) {
        for (uint32 i = 0u; (ret.ErrorsCleared()) && (i < nApps); i++) {
            ReferenceT<RealTimeApplication> rtApp = rtApps.Get(i);
            ret = rtApp->StopCurrentStateExecution();
            if (!ret.ErrorsCleared()) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Failed to StopCurrentStateExecution");
            }
        }

    }
    if (ret.ErrorsCleared()) {
        ret = Loader::Stop();
    }
    return ret;
}

CLASS_REGISTER(RealTimeLoader, "")

}

