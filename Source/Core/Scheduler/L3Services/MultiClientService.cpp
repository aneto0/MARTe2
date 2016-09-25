/**
 * @file MultiClientEmbeddedThread.cpp
 * @brief Source file for class MultiClientService
 * @date Sep 5, 2016
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
 * the class MultiClientService (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <MultiClientEmbeddedThread.h>
#include "MultiClientService.h"
#include "ReferenceT.h"

namespace MARTe {
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MultiClientService::~MultiClientService() {

}

MultiClientService::MultiClientService(EmbeddedServiceMethodBinderI &binder) :
        MultiThreadService(binder) {
    minNumberOfThreads = 1u;
    maxNumberOfThreads = 3u;
}

bool MultiClientService::Initialise(StructuredDataI &data) {

    ErrorManagement::ErrorType err;
    err.parametersError = !data.Read("MaxNumberOfThreads", maxNumberOfThreads);
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(ErrorManagement::ParametersError, "MaxNumberOfThreads was not specified");
    }
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("MinNumberOfThreads", minNumberOfThreads);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MinNumberOfThreads was not specified");
        }
    }
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("Timeout", msecTimeout);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Timeout was not specified");
        }
    }
    if (err.ErrorsCleared()) {
        if (msecTimeout == 0u) {
            msecTimeout = TTInfiniteWait.GetTimeoutMSec();
        }
    }
    if (err.ErrorsCleared()) {
        err.parametersError = (maxNumberOfThreads <= minNumberOfThreads);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MaxNumberOfThreads must be > MinNumberOfThreads");
        }
    }
    if (err.ErrorsCleared()) {
        err.parametersError = (minNumberOfThreads == 0u);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MinNumberOfThreads must be > 0");
        }
    }

    return err;
}

ErrorManagement::ErrorType MultiClientService::AddThread() {
    ErrorManagement::ErrorType err;
    err.illegalOperation = (threadPool.Size() >= maxNumberOfThreads);
    if (err.ErrorsCleared()) {
        ReferenceT<MultiClientEmbeddedThread> thread(new (NULL) MultiClientEmbeddedThread(method, *this));
        err.fatalError = !thread.IsValid();
        uint16 threadNumber = 0u;
        //Check for dead threads... TODO discuss with FISA + unique identifier.
        uint32 i = 0u;
        i = 0;
        while ((i < threadPool.Size()) && (err.ErrorsCleared())) {
            ReferenceT<EmbeddedThreadI> deadThreadCheck = threadPool.Get(i);
            if (deadThreadCheck.IsValid()) {
                if (deadThreadCheck->GetStatus() == EmbeddedThreadI::OffState) {
                    //Recycle thread number
                    if (threadNumber == 0u) {
                        threadNumber = deadThreadCheck->GetThreadNumber();
                    }
                    threadPool.Delete(deadThreadCheck);
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
        if (err.ErrorsCleared()) {
            thread->SetThreadNumber(threadNumber);
            thread->SetTimeout(msecTimeout);
            if (err.ErrorsCleared()) {
                err = thread->Start();
            }

            if (err.ErrorsCleared()) {
                threadPool.Insert(thread);
            }
        }
    }
    return err;
}

uint16 MultiClientService::GetNumberOfActiveThreads() {
    uint32 numberOfThreads = threadPool.Size();
    uint32 numberOfAliveThreads = 0u;
    uint32 i;
    for (i = 0u; (i < numberOfThreads); i++) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        if (thread->GetStatus() != EmbeddedThreadI::OffState) {
            numberOfAliveThreads++;
        }
    }

    return numberOfAliveThreads;
}

ErrorManagement::ErrorType MultiClientService::Start() {
    ErrorManagement::ErrorType err;
    err.illegalOperation = (threadPool.Size() > 0);
    uint32 n = 0u;
    while ((threadPool.Size() < minNumberOfThreads) && (err.ErrorsCleared())) {
        ReferenceT<MultiClientEmbeddedThread> thread(new (NULL) MultiClientEmbeddedThread(method, *this));
        err.fatalError = !thread.IsValid();
        if (err.ErrorsCleared()) {
            thread->SetThreadNumber(static_cast<uint16>(n));
            thread->SetTimeout(msecTimeout);
            err = thread->Start();
        }
        if (err.ErrorsCleared()) {
            threadPool.Insert(thread);
        }
        n++;
    }
    return err;
}

uint16 MultiClientService::GetMaximumNumberOfPoolThreads() const {
    return maxNumberOfThreads;
}

uint16 MultiClientService::GetMinimumNumberOfPoolThreads() const {
    return minNumberOfThreads;
}

void MultiClientService::SetMaximumNumberOfPoolThreads(const uint16 maxNumberOfThreadsIn) {
    if (threadPool.Size() == 0u) {
        if (maxNumberOfThreadsIn > minNumberOfThreads) {
            maxNumberOfThreads = maxNumberOfThreadsIn;
        }
    }
}

void MultiClientService::SetMinimumNumberOfPoolThreads(const uint16 minNumberOfThreadsIn) {
    if (threadPool.Size() == 0u) {
        if (maxNumberOfThreads > minNumberOfThreadsIn) {
            if (minNumberOfThreadsIn > 0u) {
                minNumberOfThreads = minNumberOfThreadsIn;
            }
        }
    }
}

}

