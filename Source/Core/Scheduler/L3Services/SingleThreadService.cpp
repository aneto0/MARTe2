/**
 * @file SingleThreadService.cpp
 * @brief Source file for class SingleThreadService
 * @date 23/08/2016
 * @author Filippo Sartori
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
 * the class SingleThreadService (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <SingleThreadService.h>
#include "ExecutionInfo.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

SingleThreadService::SingleThreadService(EmbeddedServiceMethodBinderI &binder) :
        EmbeddedServiceI(),
        embeddedThread(binder) {
    SetTimeout(TTInfiniteWait);
}

SingleThreadService::~SingleThreadService() {
    Stop();
    Stop();
}

bool SingleThreadService::Initialise(StructuredDataI &data) {
    uint32 msecTimeout;
    ErrorManagement::ErrorType err;
    err.parametersError = !data.Read("Timeout", msecTimeout);
    if (err.ErrorsCleared()) {
        if (msecTimeout == 0u) {
            msecTimeout = TTInfiniteWait.GetTimeoutMSec();
        }
        SetTimeout(msecTimeout);
    }

    return err;
}

void SingleThreadService::SetTimeout(TimeoutType msecTimeoutIn) {
    embeddedThread.SetTimeout(msecTimeoutIn);
}

TimeoutType SingleThreadService::GetTimeout() const {
    return embeddedThread.GetTimeout();
}

ErrorManagement::ErrorType SingleThreadService::Start() {
    return embeddedThread.Start();
}

ErrorManagement::ErrorType SingleThreadService::Stop() {
    return embeddedThread.Stop();
}

}
