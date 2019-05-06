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
#include <unistd.h>
#include <circle/actled.h>
#include <circle/logger.h>
#include <circle/memory.h>
#include <circle/actled.h>
#include <circle/koptions.h>
#include <circle/devicenameservice.h>
#include <circle/screen.h>
#include <circle/serial.h>
#include <circle/exceptionhandler.h>
#include <circle/interrupt.h>
#include <circle/timer.h>
#include <circle/logger.h>
#include <circle/usb/dwhcidevice.h>
#include <circle/sched/scheduler.h>
#include <circle/net/netsubsystem.h>
#include <circle/types.h>
#include <circle/multicore.h>
#include <circle/string.h>
#include <circle/startup.h>
#include <circle/gpioclock.h>
#include <circle/gpiopin.h>
#include <circle/pwmoutput.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "ConfigurationDatabase.h"
#include "StructuredDataI.h"
#include "MemoryDataSourceI.h"
#include "GAM.h"
#include "GAMSchedulerI.h"
#include "RealTimeApplication.h"
#include "BasicTCPSocket.h"
#include "CMultiCoreSupport1.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe {

/**
 * The list of linux MARTe applications.
 */
static const char8 * const arguments =
        "Arguments are -l LOADERCLASS -f FILENAME [-p xml|json|cdb] [-s FIRST_STATE | -m MSG_DESTINATION:MSG_FUNCTION] [-c DEFAULT_CPUS] [-t BUILD_TOKENS]";

#define INCLUDE_CFG_FILE(x) QUOTE(x)



/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//Note that some methods are implemented in FileSystem/L6App/Environment/Linux/Bootstrap.cpp

Bootstrap::Bootstrap() {

    // cannot return here because some destructors used in CKernel are not implemented
}

Bootstrap::~Bootstrap() {

}

ErrorManagement::ErrorType Bootstrap::ReadParameters(int32 argc,
                                                     char8 **argv,
                                                     StructuredDataI &loaderParameters) {

    ErrorManagement::ErrorType ret;
    ret.parametersError = !loaderParameters.Write("Loader", "RealTimeLoader");
    if (ret) {
        ret.parametersError = !loaderParameters.Write("FirstState", "State1");
    }
    if (ret) {
        ret.parametersError = !loaderParameters.Write("Parser", "cdb");
    }
    return ret;
}

void Bootstrap::Printf(const char8 * const msg) {
    CLogger::Get()->Write("bootstrap", LogNotice, "%s\n", msg);
}

}
