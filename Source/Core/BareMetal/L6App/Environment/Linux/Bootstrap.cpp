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
#include <signal.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "ConfigurationDatabase.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * The list of linux MARTe applications.
 */
static const char8 * const arguments = "Arguments are -l LOADERCLASS -f FILENAME [-p xml|json|cdb] [-s FIRST_STATE | -m MSG_DESTINATION:MSG_FUNCTION] [-c DEFAULT_CPUS] [-t BUILD_TOKENS] [-g SCHEDULER_GRANULARITY_US] [-k STOP_MSG_DESTINATION:STOP_MSG_FUNCTION]";

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

//Note that some methods are implemented in FileSystem/L6App/Environment/Linux/Bootstrap.cpp

Bootstrap::Bootstrap() {

}

Bootstrap::~Bootstrap() {

}

ErrorManagement::ErrorType Bootstrap::ReadParameters(int32 argc, char8 **argv, StructuredDataI &loaderParameters) {
    ErrorManagement::ErrorType ret = (argv != NULL_PTR(char8 **));

    //Write all the parameters into a cdb
    ConfigurationDatabase argsConfiguration;
    if (ret) {
        int32 i;
        for (i = 1; (i < (argc - 1)); i += 2) {
            argsConfiguration.Write(argv[i], argv[i + 1]);
            //Also write them into the loaderParameters so that they can be use generally by any loader
            loaderParameters.Write(argv[i], argv[i + 1]);
        }
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Arguments:\n%!", argsConfiguration);
    }
    if (ret) {
        StreamString loaderClass;
        ret.parametersError = !argsConfiguration.Read("-l", loaderClass);
        if (ret) {
            ret.parametersError = !loaderParameters.Write("Loader", loaderClass.Buffer());
        }
        else {
            REPORT_ERROR_STATIC(ret, arguments);
        }
    }
    if (ret) {
        StreamString filename;
        ret.parametersError = !argsConfiguration.Read("-f", filename);
        if (ret) {
            ret.parametersError = !loaderParameters.Write("Filename", filename.Buffer());
        }
        else {
            REPORT_ERROR_STATIC(ret, arguments);
        }
    }
    if (ret) {
        StreamString precompiledRealTimeFunctionsFilename;
        if (argsConfiguration.Read("-pf", precompiledRealTimeFunctionsFilename)) {
            ret.parametersError = !loaderParameters.Write("PrecompiledFunctions", precompiledRealTimeFunctionsFilename.Buffer());
        }
    }
    if (ret) {
        StreamString precompiledRealTimeDataFilename;
        if (argsConfiguration.Read("-pd", precompiledRealTimeDataFilename)) {
            ret.parametersError = !loaderParameters.Write("PrecompiledData", precompiledRealTimeDataFilename.Buffer());
        }
    }
    if (ret) {
        uint32 precompileRealTimeApp = 0;
        if (argsConfiguration.Read("-pc", precompileRealTimeApp)) {
            ret.parametersError = !loaderParameters.Write("PrecompileRealTimeApp", precompileRealTimeApp);
        }
    }

    if (ret) {
        uint32 defaultCPUs = 0x1;
        (void) argsConfiguration.Read("-c", defaultCPUs);
        ret.parametersError = !loaderParameters.Write("DefaultCPUs", defaultCPUs);
    }
    if (ret) {
        uint32 schedulerGranularity;
        if(argsConfiguration.Read("-g", schedulerGranularity)) {
            ret.parametersError = !loaderParameters.Write("SchedulerGranularity", schedulerGranularity);
        }
    }
    if (ret) {
        StreamString parserType;
        if (!argsConfiguration.Read("-p", parserType)) {
            parserType = "cdb";
        }
        ret.parametersError = !loaderParameters.Write("Parser", parserType.Buffer());
    }
    if (ret) {
        StreamString firstState;
        (void) argsConfiguration.Read("-s", firstState);
        if (firstState.Size() > 0u) {
            ret.parametersError = !loaderParameters.Write("FirstState", firstState.Buffer());
        }
    }
    if (ret) {
        StreamString buildTokens;
        (void) argsConfiguration.Read("-t", buildTokens);
        if (buildTokens.Size() > 0u) {
            ret.parametersError = !loaderParameters.Write("BuildTokens", buildTokens.Buffer());
        }
    }
    if (ret) {
        StreamString domainTokens;
        (void) argsConfiguration.Read("-d", domainTokens);
        if (domainTokens.Size() > 0u) {
            ret.parametersError = !loaderParameters.Write("DomainTokens", domainTokens.Buffer());
        }
    }
    if (ret) {
        StreamString messageArgs;
        (void) argsConfiguration.Read("-m", messageArgs);
        if (messageArgs.Size() > 0u) {
            char8 term;
            messageArgs.Seek(0LLU);
            StreamString messageDestination;
            StreamString messageFunction;
            ret.parametersError = !messageArgs.GetToken(messageDestination, ":", term);
            if (ret) {
                ret.parametersError = !messageArgs.GetToken(messageFunction, ":", term);
            }
            if (ret) {
                ret.parametersError = !loaderParameters.Write("MessageDestination", messageDestination.Buffer());
            }
            if (ret) {
                ret.parametersError = !loaderParameters.Write("MessageFunction", messageFunction.Buffer());
            }
        }
    }
    if (ret) {
        StreamString messageArgs;
        (void) argsConfiguration.Read("-k", messageArgs);
        if (messageArgs.Size() > 0u) {
            char8 term;
            messageArgs.Seek(0LLU);
            StreamString messageDestination;
            StreamString messageFunction;
            ret.parametersError = !messageArgs.GetToken(messageDestination, ":", term);
            if (ret) {
                ret.parametersError = !messageArgs.GetToken(messageFunction, ":", term);
            }
            if (ret) {
                ret.parametersError = !loaderParameters.Write("StopMessageDestination", messageDestination.Buffer());
            }
            if (ret) {
                ret.parametersError = !loaderParameters.Write("StopMessageFunction", messageFunction.Buffer());
            }
        }
    }

    if (ret) {
        REPORT_ERROR_STATIC(ErrorManagement::Information, "Loader parameters:\n%!", loaderParameters);
    }
    return ret;
}

void Bootstrap::Printf(const char8 * const msg) {
    printf("%s\n", msg);
}

void Bootstrap::Main(int (*loader)(int32 argc, char8** argv), int32 argc, char8** argv) {
    loader(argc, argv);
}

}
