/**
 * @file Bootstrap.cpp
 * @brief Source file for class Bootstrap
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
 * the class Bootstrap (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "platform.h"
#include "xil_printf.h"

#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "ConfigurationDatabase.h"
#include "marte2config.h"
#include "StandardParser.h"


namespace MARTe {

    StreamString configurationFile;

    void BareUSErrorProcessFunction(const ErrorManagement::ErrorInformation &errorInfo,
                                    const char8 * const errorDescription) {
        print(errorDescription);
    }

    Bootstrap::Bootstrap() {
        ErrorManagement::SetErrorProcessFunction(BareUSErrorProcessFunction);
    }

    Bootstrap::~Bootstrap() {

    }

    ErrorManagement::ErrorType Bootstrap::Run() {
        
        return ErrorManagement::NoError;
    }
   

    ErrorManagement::ErrorType Bootstrap::GetConfigurationStream(StructuredDataI &loaderParameters, StreamI *&configurationStream) {
        ErrorManagement::ErrorType retVal = ErrorManagement::NoError;
        configurationFile = configFile;
        configurationFile.Seek(0);
        configurationStream = &configurationFile;

        return retVal;
    }

    ErrorManagement::ErrorType Bootstrap::ReadParameters(int32 argc, char8 **argv, StructuredDataI &loaderParameters) {
        ErrorManagement::ErrorType retVal;

        //Writing defaults in loader parameters
        retVal.parametersError = !loaderParameters.Write("Parser", "cdb");
        if(!retVal) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::ReadParameters() Failure writing default parser (cdb)");
        }

        if(retVal) {
            retVal.parametersError = !loaderParameters.Write("Loader", "RealTimeLoader");
            if(!retVal) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::ReadParameters() Failure writing default loader (RealTimeLoader)");
            }
        }

        if(retVal) {
            retVal.parametersError = !loaderParameters.Write("FirstState", "State1");
            if(!retVal) {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Bootstrap::ReadParameters() Failure setting startup state (State1)");
            }
        }

        return retVal;
    }

    void Bootstrap::Printf(const char8 * const msg) {
        xil_printf("%s\r\n", msg);
    }

}
