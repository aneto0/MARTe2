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
//This refers to the underlying Xilinx printf
#include "xil_printf.h"

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
namespace MARTe {

//Note that some methods are implemented in FileSystem/L6App/Environment/Linux/Bootstrap.cpp

Bootstrap::Bootstrap() {

}

Bootstrap::~Bootstrap() {

}

ErrorManagement::ErrorType Bootstrap::ReadParameters(int32 argc, char8 **argv, StructuredDataI &loaderParameters) {
    //TODO finishe me
    ErrorManagement::ErrorType ret;
    ret.parametersError = !loaderParameters.Write("Loader", "RealTimeLoader");
    if (ret) {
        //ret.parametersError = !loaderParameters.Write("FirstState", "State1");
        ret.parametersError = !loaderParameters.Write("MessageDestination", "StateMachine");
        if (ret) {
            ret.parametersError = !loaderParameters.Write("MessageFunction", "START");
        }
    }
    if (ret) {
        ret.parametersError = !loaderParameters.Write("Parser", "cdb");
    }
    return ret;
}

extern "C" {
    void HardwarePrintf(const char8 * const msg);
}
void Bootstrap::Printf(const char8 * const msg) {
    //TODO HARDWAREPRINTF
    xil_printf("%s\r\n", msg);
}
extern void MARTe2HardwareInitialise();

extern "C" {
    void PreLoader(const void *_loader) {
        int (*loader) (MARTe::int32 argc, MARTe::char8** argv) = (int (*) (MARTe::int32 argc, MARTe::char8** argv))_loader;
        loader(0, NULL);
    }
}

void Bootstrap::Main(int (*loader)(int32 argc, char8** argv), int32 argc, char8** argv) {
    MARTe2HardwareInitialise(); //Handle to initialise hardware
    BaseType_t xReturned;
    TaskHandle_t xHandle = NULL;

    //TODO CHECK Priority and stack size as parameter
    /* Create the task, storing the handle. */
    xReturned = xTaskCreate(
                    PreLoader,                          /* Function that implements the task. */
                    "Main",                             /* Text name for the task. */
                    4 * THREADS_DEFAULT_STACKSIZE,      /* Stack size in words, not bytes. */
                    loader,                             /* Parameter passed into the task. */
                    tskIDLE_PRIORITY,                   /* Priority at which the task is created. */
                    &xHandle );                         /* Used to pass out the created task's handle. */

    if( xReturned == pdPASS )
    {
        /* The task was created.  Use the task's handle to delete the task. */
        vTaskDelete( xHandle );
    }

    vTaskStartScheduler(); //Start FreeRTOS Scheduler

    for(;;); //We should never reach here
}

}
