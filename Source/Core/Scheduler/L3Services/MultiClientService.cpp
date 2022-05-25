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
#include "AdvancedErrorManagement.h"
#include "MultiClientEmbeddedThread.h"
#include "MultiClientService.h"
#include "ReferenceT.h"
#include "StreamString.h"

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

    ErrorManagement::ErrorType err = EmbeddedServiceI::Initialise(data);
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("MaxNumberOfThreads", maxNumberOfThreads);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MaxNumberOfThreads was not specified");
        }
    }
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("MinNumberOfThreads", minNumberOfThreads);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "MinNumberOfThreads was not specified");
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
        if (err.ErrorsCleared()) {
            thread->SetPriorityClass(GetPriorityClass());
            thread->SetPriorityLevel(GetPriorityLevel());
            thread->SetCPUMask(GetCPUMask());
            thread->SetTimeout(GetTimeout());
            thread->SetThreadNumber(static_cast<uint16>(threadPool.Size()));
            StreamString tname;
            (void)tname.Printf("%s_%d", GetName(), HighResolutionTimer::Counter32());
            thread->SetName(tname.Buffer());
            err = thread->Start();
        }

        if (err.ErrorsCleared()) {
            err.fatalError = !threadPool.Insert(thread);
        }

    }
    return err;
}

ErrorManagement::ErrorType MultiClientService::RemoveThread(const ThreadIdentifier threadId) {
    uint32 i = 0u;
    ErrorManagement::ErrorType err;
    bool found = false;
    while ((i < threadPool.Size()) && (!found)) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        found = (thread->GetThreadId() == threadId);
        if (found) {
            err.recoverableError = !threadPool.Delete(thread);
        }
        i++;
    }
    err.fatalError = !found;
    return err;
}

uint16 MultiClientService::GetNumberOfActiveThreads() {
    uint32 numberOfThreads = threadPool.Size();
    uint16 numberOfAliveThreads = 0u;
    uint32 i;
    for (i = 0u; (i < numberOfThreads); i++) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        if (thread.IsValid()) {
            if (thread->GetStatus() != EmbeddedThreadI::OffState) {
                numberOfAliveThreads++;
            }
        }
    }

    return numberOfAliveThreads;
}

ErrorManagement::ErrorType MultiClientService::Start() {
    ErrorManagement::ErrorType err;
    err.illegalOperation = (threadPool.Size() > 0u);
    bool errorsCleared = err.ErrorsCleared();
    while ((threadPool.Size() < minNumberOfThreads) && (errorsCleared)) {
        ReferenceT<MultiClientEmbeddedThread> thread(new (NULL) MultiClientEmbeddedThread(method, *this));
        err.fatalError = !thread.IsValid();
        if (err.ErrorsCleared()) {
            thread->SetPriorityClass(GetPriorityClass());
            thread->SetPriorityLevel(GetPriorityLevel());
            thread->SetCPUMask(GetCPUMask());
            thread->SetTimeout(GetTimeout());
            thread->SetName(GetName());
            err = thread->Start();
        }
        if (err.ErrorsCleared()) {
            err.fatalError = !threadPool.Insert(thread);
        }
        errorsCleared = err.ErrorsCleared();
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
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Maximum number of pool threads cannot be changed if the service is running");
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
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Minimum number of pool threads cannot be changed if the service is running");
    }
}

void MultiClientService::SetPriorityClass(const Threads::PriorityClassType priorityClassIn) {
    if (threadPool.Size() == 0u) {
        EmbeddedServiceI::SetPriorityClass(priorityClassIn);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority class cannot be changed if the service is running");
    }
}

void MultiClientService::SetPriorityLevel(const uint8 priorityLevelIn) {
    if (threadPool.Size() == 0u) {
        EmbeddedServiceI::SetPriorityLevel(priorityLevelIn);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority level cannot be changed if the service is running");
    }
}

void MultiClientService::SetCPUMask(const ProcessorType& cpuMaskIn) {
    if (threadPool.Size() == 0u) {
        EmbeddedServiceI::SetCPUMask(cpuMaskIn);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask cannot be changed if the service is running");
    }

}

}

