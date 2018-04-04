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
#include "BasicFile.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * TODO
 */
static BasicFile inputConfigurationFile;
/**
 * TODO
 */
static bool keepRunning = true;
/**
 * TODO
 */
static bool killApp = false;
/**
 * TODO
 */
static void StopApp(int sig) {
    //Second time this is called? Kill the application.
    if (!killApp) {
        killApp = true;
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Application killed.");
        _exit(0);
    }
    REPORT_ERROR_STATIC(ErrorManagement::Information, "Application successfully stopped.\n");
    keepRunning = false;
}

static const char8 * const arguments =
        "Arguments are -l LOADERCLASS -f FILENAME [-p xml|json|cdb] [-s FIRST_STATE | -m MSG_DESTINATION:MSG_FUNCTION] [-c DEFAULT_CPUS]";
/**
 * TODO
 */
static ErrorManagement::ErrorType ParseParameter(int32 argc, char8 **argv, const char8 * const flag, StreamString &output, bool optional) {
    int32 i;
    bool found = false;
    ErrorManagement::ErrorType ret;
    for (i = 0; (i < (argc - 1)) && (!found); i++) {
        StreamString arg = argv[i];
        found = (arg == flag);
        if (found) {
            output = argv[i + 1];
        }
    }

    if (!found) {
        if (optional) {
            ret = ErrorManagement::Warning;
        }
        else {
            ret = ErrorManagement::ParametersError;
            REPORT_ERROR_STATIC(ret, arguments);
        }
    }
    return ret;
}
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

Bootstrap::Bootstrap() {

}

Bootstrap::~Bootstrap() {

}

ErrorManagement::ErrorType Bootstrap::ReadParameters(int32 argc, char8 **argv, StructuredDataI &loaderParameters, StreamI &configurationStream) {

    StreamString filename;
    StreamString firstState;
    StreamString messageArgs;
    StreamString loaderClass;
    StreamString parserType;
    StreamString defaultCPUs;

    ErrorManagement::ErrorType ret = (argc > 4);
    if (ret) {
        ret = ParseParameter(argc, argv, "-f", filename, false);
    }
    if (ret) {
        ret = ParseParameter(argc, argv, "-l", loaderClass, false);
        if (ret) {
            ret.parametersError = !loaderParameters.Write("Loader", loaderClass.Buffer());
        }
        else {
            ret = ErrorManagement::NoError;
        }
    }
    if (ret) {
        ret = ParseParameter(argc, argv, "-c", defaultCPUs, true);
        if (ret) {
            ret.parametersError = !loaderParameters.Write("DefaultCPUs", defaultCPUs.Buffer());
        }
        else {
            ret = ErrorManagement::NoError;
        }
    }
    if (ret) {
        ret = ParseParameter(argc, argv, "-p", parserType, true);
        if (!ret) {
            parserType = "cdb";
            ret = ErrorManagement::NoError;
        }
        ret.parametersError = !loaderParameters.Write("Parser", parserType.Buffer());
    }
    if (ret) {
        ret = ParseParameter(argc, argv, "-s", firstState, true);
        if (ret == ErrorManagement::Warning) {
            ret = ParseParameter(argc, argv, "-m", messageArgs, true);
            if (ret) {
                StreamString messageDestination;
                StreamString messageFunction;
                char8 term;
                messageArgs.Seek(0LLU);
                ret.parametersError = !messageArgs.GetToken(messageDestination, ":", term);
                if (ret) {
                    ret.parametersError = !messageArgs.GetToken(messageFunction, ":", term);
                    if (ret) {
                        ret.parametersError = !loaderParameters.Write("MessageDestination", messageDestination.Buffer());
                    }
                    if (ret) {
                        ret.parametersError = !loaderParameters.Write("MessageFunction", messageFunction.Buffer());
                    }
                    if (!ret) {
                        REPORT_ERROR_STATIC(ErrorManagement::ParametersError, arguments);
                    }
                }
                else {
                    REPORT_ERROR_STATIC(ErrorManagement::ParametersError, arguments);
                }
            }
            else {
                //Message is optional
                ret = ErrorManagement::NoError;
            }
        }
        else {
            ret.parametersError = !loaderParameters.Write("FirstState", firstState.Buffer());
        }
    }
    //Read the input file to a stream
    if (ret) {
        ret.OSError = !inputConfigurationFile.Open(filename.Buffer(), BasicFile::ACCESS_MODE_R);
        if (ret) {
            ret.OSError = !inputConfigurationFile.Seek(0LLU);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to open the input file %s", filename.Buffer());
        }
        configurationStream = inputConfigurationFile;
    }
    return ret;
}

ErrorManagement::ErrorType Bootstrap::Run() {
    ErrorManagement::ErrorType ret = inputConfigurationFile.Close();
    if (ret) {
        mlockall(MCL_CURRENT | MCL_FUTURE);
        signal(SIGTERM, StopApp);
        signal(SIGINT, StopApp);
        while (keepRunning) {
            Sleep::Sec(1.0);
        }
        munlockall();
    }
    return ret;
}

void Bootstrap::Printf(const char8 * const msg) {
    printf("%s\n", msg);
}

}
