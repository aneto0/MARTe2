/**
 * @file MultiThreadService.cpp
 * @brief Source file for class MultiThreadService
 * @date Sep 2, 2016
 * @author fsartori
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
 * the class MultiThreadService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "MultiThreadService.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MultiThreadService::~MultiThreadService() {
    Stop();
}

bool MultiThreadService::Initialise(StructuredDataI &data) {
    ErrorManagement::ErrorType err;
    threadPool.SetName("ThreadPool");
    err.parametersError = !data.Read("NumberOfPoolThreads", numberOfPoolThreads);
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("Timeout", msecTimeout);
    }
    if (err.ErrorsCleared()) {
        if (msecTimeout == 0u) {
            msecTimeout = TTInfiniteWait.GetTimeoutMSec();
        }
    }
    return err;
}

ErrorManagement::ErrorType MultiThreadService::Start() {
    ErrorManagement::ErrorType err;
    err.illegalOperation = (threadPool.Size() >= numberOfPoolThreads);
    uint32 n = 0u;
    while ((threadPool.Size() < numberOfPoolThreads) && (err.ErrorsCleared())) {
        ReferenceT<SingleThreadService> service(new (NULL) SingleThreadService(method));
        err.fatalError = !service.IsValid();
        if (err.ErrorsCleared()) {
            service->SetThreadNumber(n);
            service->SetTimeout(msecTimeout);
            err = service->Start();
        }
        if (err.ErrorsCleared()) {
            threadPool.Insert(service);
        }
        n++;
    }
    return err;
}

ErrorManagement::ErrorType MultiThreadService::Stop() {
    ErrorManagement::ErrorType err;
    uint32 i;
    for (i = 0u; i < threadPool.Size(); i++) {
        ReferenceT<SingleThreadService> service = threadPool.Get(i);
        if (service.IsValid()) {
            err = service->Stop();
            if (!err.ErrorsCleared()) {
                REPORT_ERROR_PARAMETERS(err, "Could not Stop SingleThreadService(%d)", i)
            }
        }
    }
    // perform kill if necessary
    for (i = 0; i < threadPool.Size(); i++) {
        ReferenceT<SingleThreadService> service = threadPool.Get(i);
        if (service.IsValid()) {
            err = service->Stop();
            if (!err.ErrorsCleared()) {
                REPORT_ERROR_PARAMETERS(err, "Could not Kill SingleThreadService(%d)", i)
            }
        }
    }
    // remove dead threads
    i = 0;
    while ((i < threadPool.Size()) && (err.ErrorsCleared())) {
        ReferenceT<SingleThreadService> service = threadPool.Get(i);
        if (service.IsValid()) {
            if (service->GetStatus() == EmbeddedServiceI::OffState) {
                threadPool.Delete(service);
            }
            else {
                i++;
            }
        }
        else {
            // some unexpected content or something seriously wrong!!
            err.internalSetupError = true;
        }
    }
    if (err.ErrorsCleared() && (threadPool.Size() > 0)) {
        // some service die hard
        err.timeout = true;
    }

    return err;

}

EmbeddedServiceI::States MultiThreadService::GetStatus(uint32 threadIdx) {
    EmbeddedServiceI::States status = EmbeddedServiceI::OffState;
    if (threadIdx < threadPool.Size()) {
        ReferenceT<SingleThreadService> service = threadPool.Get(threadIdx);
        status = service->GetStatus();
    }

    return status;
}

ThreadIdentifier MultiThreadService::GetThreadId(uint32 threadIdx) {
    ThreadIdentifier tid = 0u;
    if (threadIdx < threadPool.Size()) {
        ReferenceT<SingleThreadService> service = threadPool.Get(threadIdx);
        tid = service->GetThread().GetThreadId();
    }

    return tid;
}

uint32 MultiThreadService::GetNumberOfPoolThreads() const {
    return numberOfPoolThreads;
}

void MultiThreadService::SetNumberOfPoolThreads(const uint32 numberOfPoolThreadsIn) {
    numberOfPoolThreads = numberOfPoolThreadsIn;
}

void MultiThreadService::SetTimeout(TimeoutType msecTimeoutIn) {
    msecTimeout = msecTimeoutIn.GetTimeoutMSec();
    uint32 i;
    for (i = 0u; i < threadPool.Size(); i++) {
        ReferenceT<SingleThreadService> service = threadPool.Get(i);
        if (service.IsValid()) {
            service->SetTimeout(msecTimeoutIn);
        }
    }
}

TimeoutType MultiThreadService::GetTimeout() const {
    return msecTimeout;
}

}
