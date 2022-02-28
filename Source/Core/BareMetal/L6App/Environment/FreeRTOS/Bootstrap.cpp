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
#include <stdio.h>
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"


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

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
extern void MARTe2HardwareInitialise();
extern void HardwarePrintf(const char8 * const msg);

extern "C" {
    TaskHandle_t marte2MainTask;
    /**
    *
    */
    void PreLoader(void (*_loader)(void*)) {      
        //The preloader suspends itself until the hw initialisation ends
    	printf("Waiting resume signal from initialisation\r\n");
        vTaskSuspend(NULL);  
        printf("Resume signal received\r\n");
        int (*loader) (MARTe::int32 argc, MARTe::char8** argv) = (int (*) (MARTe::int32 argc, MARTe::char8** argv))_loader;
        printf("Starting PreLoader\r\n");
        loader(0, NULL);

        printf("PreLoader terminated\r\n");

        vTaskDelete(NULL);
    }
}

namespace MARTe {

//Note that some methods are implemented in FileSystem/L6App/Environment/Linux/Bootstrap.cpp

Bootstrap::Bootstrap() {

}

Bootstrap::~Bootstrap() {

}

void Bootstrap::Printf(const char8 * const msg) {
    HardwarePrintf(msg);
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

void Bootstrap::Main(int (*loader)(int32 argc, char8** argv), int32 argc, char8** argv) {
    //TODO: Check if is acceptable this kind of approach

    MARTe2HardwareInitialise(); //Handle to initialise hardware
    BaseType_t xReturned;
    //TaskHandle_t xHandle = NULL;

    //TODO CHECK Priority and stack size as parameter
    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    PreLoader,                          /* Function that implements the task. */
                    "Main",                             /* Text name for the task. */
                    4 * THREADS_DEFAULT_STACKSIZE,      /* Stack size in words, not bytes. */
                    (void*)loader,                      /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,                   /* Priority at which the task is created. */
                    &marte2MainTask);                   /* Used to pass out the created task's handle. */

    vTaskStartScheduler(); //Start FreeRTOS Scheduler

    for(;;); //We should never reach here
}

}
