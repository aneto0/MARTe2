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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "BasicFile.h"
#include "ConfigurationDatabase.h"
#include "StreamMemoryReference.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
extern const char * configFile;

namespace MARTe {

/**
 * True while the application is to be running.
 */
static bool keepRunning = true;
}


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */

/*---------------------------------------------------------------------------*/

namespace MARTe {

StreamMemoryReference *staticConfigurationStream = NULL;

ErrorManagement::ErrorType Bootstrap::GetConfigurationStream(StructuredDataI &loaderParameters, StreamI *&configurationStream) {
    const char8 * const marteConfig = configFile;
    uint32 marteConfigLength = StringHelper::Length(marteConfig) + 1;
    if (staticConfigurationStream == NULL) {
        staticConfigurationStream = new StreamMemoryReference(marteConfig, marteConfigLength);
    }
    configurationStream = staticConfigurationStream;
    return true;
}

ErrorManagement::ErrorType Bootstrap::Run() {
    
    while (keepRunning) {
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
    if (staticConfigurationStream != NULL) {
        delete staticConfigurationStream;
    }

    return true;
}

}
