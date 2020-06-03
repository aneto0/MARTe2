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
#include "File.h"
#include "ConfigurationDatabase.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * The configuration file.
 */
static File inputConfigurationFile;
/**
 * True while the application is to be running.
 */
static bool keepRunning = true;
/**
 * To be used when the application is to be killed.
 */
static bool killApp = false;
/**
 * Callback function for the signal (see Run below).
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
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ErrorManagement::ErrorType Bootstrap::GetConfigurationStream(StructuredDataI &loaderParameters, StreamI *&configurationStream) {
    StreamString filename;
    ErrorManagement::ErrorType ret = loaderParameters.Read("Filename", filename);
    //Read the input file to a stream
    if (ret) {
        ret.OSError = !inputConfigurationFile.Open(filename.Buffer(), BasicFile::ACCESS_MODE_R);
        if (ret) {
            ret.OSError = !inputConfigurationFile.Seek(0LLU);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::ParametersError, "Failed to open the input file %s", filename.Buffer());
        }
    }
    if (ret) {
        configurationStream = &inputConfigurationFile;
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

}
