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


#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "ConfigurationDatabase.h"
#include "StandardParser.h"

extern void MARTe2HardwareInitialise();

namespace MARTe {

    StreamString configurationFile;

    void BareUSErrorProcessFunction(const ErrorManagement::ErrorInformation &errorInfo,
                                    const char8 * const errorDescription) {
        printf(errorDescription);
    }

    Bootstrap::Bootstrap() {
        ErrorManagement::SetErrorProcessFunction(BareUSErrorProcessFunction);
    }

    Bootstrap::~Bootstrap() {

    }   

    ErrorManagement::ErrorType Bootstrap::ReadParameters(int32 argc, char8 **argv, StructuredDataI &loaderParameters) {
        //TODO Check me
        ErrorManagement::ErrorType ret;

        //If we have the compilation variable MARTe2_READPARAMETERS_LOADER defined, then we can use that
        //otherwise we use our default one, the RealTimeLoader
        #ifdef MARTe2_READPARAMETERS_LOADER
            ret.parametersError = !loaderParameters.Write("Loader", MARTe2_READPARAMETERS_LOADER);
        #else
            ret.parametersError = !loaderParameters.Write("Loader", "RealTimeLoader");
        #endif

        if (ret) {
            //If we have the compilation variable MARTe2_READPARAMETERS_FIRSTSTATE defined, then we can use that
            //otherwise we use our default one, Idle
            #ifdef MARTe2_READPARAMETERS_FIRSTSTATE
                ret.parametersError = !loaderParameters.Write("FirstState", MARTe2_READPARAMETERS_FIRSTSTATE);
            #else
                ret.parametersError = !loaderParameters.Write("FirstState", "Idle");
            #endif

            //If we have both MARTe2_READPARAMETERS_FIRSTMESSAGEDESTINATION MARTe2_READPARAMETERS_FIRSTMESSAGEFUNCTION
            //we can send a first message to the state machine startup. Otherwise, nothing will be sent
            #if defined (MARTe2_READPARAMETERS_FIRSTMESSAGEDESTINATION) && defined (MARTe2_READPARAMETERS_FIRSTMESSAGEFUNCTION)
                ret.parametersError = !loaderParameters.Write("MessageDestination", MARTe2_READPARAMETERS_FIRSTMESSAGEDESTINATION);
                if (ret) {
                    ret.parametersError = !loaderParameters.Write("MessageFunction", MARTe2_READPARAMETERS_FIRSTMESSAGEFUNCTION);
                }
            #endif
        }
        if (ret) {
            #ifdef MARTe2_READPARAMETERS_PARSER
                ret.parametersError = !loaderParameters.Write("Parser", MARTe2_READPARAMETERS_PARSER);
            #else
                ret.parametersError = !loaderParameters.Write("Parser", "cdb");
            #endif
        }
        return ret;
    }

    void Bootstrap::Printf(const char8 * const msg) {
        printf("%s\r\n", msg);
    }

    void Bootstrap::Main(int (*loader)(int32 argc, char8** argv), int32 argc, char8** argv) {
        MARTe2HardwareInitialise(); //Handle to initialise hardware
        loader(argc, argv);
    }

}
