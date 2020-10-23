/**
 * @file MultiThreadService.cpp
 * @brief Source file for class MultiThreadService
 * @date 02/09/2016
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
#include "StreamString.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MultiThreadService::MultiThreadService(EmbeddedServiceMethodBinderI &binder) :
        EmbeddedServiceI(), method(binder) {
    numberOfPoolThreads = 1u;
}

/*lint -e{1551} the only reason why this could throw an exception is if
 * the callback method throws an exception while stopping (but this is not
 * caught anyway). */
MultiThreadService::~MultiThreadService() {
    ErrorManagement::ErrorType err = MultiThreadService::Stop();
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(err, "Could not Stop all the EmbeddedThreadI instances.");
    }
}

bool MultiThreadService::Initialise(StructuredDataI &data) {
    threadPool.SetName("ThreadPool");
    ErrorManagement::ErrorType err = EmbeddedServiceI::Initialise(data);
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("NumberOfPoolThreads", numberOfPoolThreads);
        if (!err.ErrorsCleared()) {
            REPORT_ERROR(ErrorManagement::ParametersError, "NumberOfPoolThreads has to be specified.");
        }
    }
    if (err.ErrorsCleared()) {
        err = CreateThreads();
    }
    if (err.ErrorsCleared()) {
        AnyType individualThreadPrioritiesClass = data.GetType("PrioritiesClass");
        if (!individualThreadPrioritiesClass.IsVoid()) {
            uint32 dims = individualThreadPrioritiesClass.GetNumberOfDimensions();
            bool ok = (dims == 2u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "PrioritiesClass must be a matrix.");
            }
            uint32 numberOfColumns = 0u;
            uint32 numberOfRows = 0u;
            if (ok) {
                numberOfColumns = individualThreadPrioritiesClass.GetNumberOfElements(0u);
                numberOfRows = individualThreadPrioritiesClass.GetNumberOfElements(1u);
                ok = (numberOfColumns == 2u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "PrioritiesClass must have two columns.");
                }
            }
            Matrix<StreamString> priorityClassMat(numberOfRows, numberOfColumns);
            if (ok) {
                ok = data.Read("PrioritiesClass", priorityClassMat);
            }
            uint32 r;
            for (r = 0u; (r < numberOfRows) && (ok); r++) {
                uint32 priorityClassIdx;
                StreamString strToConvert = priorityClassMat(r, 0u);
                ok = TypeConvert(priorityClassIdx, strToConvert);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "Failed to TypeConvert string to integer.");
                }
                StreamString priorityClassStr = priorityClassMat(r, 1u);
                if (ok) {
                    ok = (priorityClassIdx < GetNumberOfPoolThreads());
                    if (!ok) {
                        REPORT_ERROR(ErrorManagement::ParametersError, "priorityClassIdx must be < GetNumberOfPoolThreads()");
                    }
                }
                if (ok) {
                    if (priorityClassStr == "IdlePriorityClass") {
                        SetPriorityClassThreadPool(Threads::IdlePriorityClass, priorityClassIdx);
                    }
                    else if (priorityClassStr == "NormalPriorityClass") {
                        SetPriorityClassThreadPool(Threads::NormalPriorityClass, priorityClassIdx);
                    }
                    else if (priorityClassStr == "RealTimePriorityClass") {
                        SetPriorityClassThreadPool(Threads::RealTimePriorityClass, priorityClassIdx);
                    }
                    else {
                        REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported PriorityClass.");
                        ok = false;
                    }
                }
            }
            if (!ok) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        AnyType individualThreadPrioritiesLevel = data.GetType("PrioritiesLevel");
        if (!individualThreadPrioritiesLevel.IsVoid()) {
            uint32 dims = individualThreadPrioritiesLevel.GetNumberOfDimensions();
            bool ok = (dims == 2u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "PrioritiesLevel must be a matrix.");
            }
            uint32 numberOfColumns = 0u;
            uint32 numberOfRows = 0u;
            if (ok) {
                numberOfColumns = individualThreadPrioritiesLevel.GetNumberOfElements(0u);
                numberOfRows = individualThreadPrioritiesLevel.GetNumberOfElements(1u);
                ok = (numberOfColumns == 2u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "PrioritiesLevel must have two columns.");
                }
            }
            Matrix<uint32> priorityLevelMat = Matrix<uint32>(numberOfRows, numberOfColumns);
            if (ok) {
                ok = data.Read("PrioritiesLevel", priorityLevelMat);
            }
            uint32 r;
            for (r = 0u; (r < numberOfRows) && (ok); r++) {
                uint32 priorityLevelIdx = priorityLevelMat(r, 0u);
                uint8 prioLevel = static_cast<uint8>(priorityLevelMat(r, 1u));
                ok = (priorityLevelIdx < GetNumberOfPoolThreads());
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "priorityLevelIdx must be < GetNumberOfPoolThreads()");
                }
                if (ok) {
                    SetPriorityLevelThreadPool(prioLevel, priorityLevelIdx);
                }
            }
            if (!ok) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        AnyType individualThreadStackSizes = data.GetType("StackSizes");
        if (!individualThreadStackSizes.IsVoid()) {
            uint32 dims = individualThreadStackSizes.GetNumberOfDimensions();
            bool ok = (dims == 2u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "StackSizes must be a matrix.");
            }
            uint32 numberOfColumns = 0u;
            uint32 numberOfRows = 0u;
            if (ok) {
                numberOfColumns = individualThreadStackSizes.GetNumberOfElements(0u);
                numberOfRows = individualThreadStackSizes.GetNumberOfElements(1u);
                ok = (numberOfColumns == 2u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "StackSizes must have two columns.");
                }
            }
            Matrix<uint32> stackSizeMat = Matrix<uint32>(numberOfRows, numberOfColumns);
            if (ok) {
                ok = data.Read("StackSizes", stackSizeMat);
            }
            uint32 r;
            for (r = 0u; (r < numberOfRows) && (ok); r++) {
                uint32 stackSizeIdx = stackSizeMat(r, 0u);
                uint32 stackSizes = static_cast<uint32>(stackSizeMat(r, 1u));
                ok = (stackSizeIdx < GetNumberOfPoolThreads());
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "stackSizeIdx must be < GetNumberOfPoolThreads()");
                }
                if (ok) {
                    SetStackSizeThreadPool(stackSizes, stackSizeIdx);
                }
            }
            if (!ok) {
                err.parametersError = true;
            }
        }
    }
    if (err.ErrorsCleared()) {
        AnyType individualThreadCPUMask = data.GetType("CPUMasks");
        if (!individualThreadCPUMask.IsVoid()) {
            uint32 dims = individualThreadCPUMask.GetNumberOfDimensions();
            bool ok = (dims == 2u);
            if (!ok) {
                REPORT_ERROR(ErrorManagement::ParametersError, "CPUMasks must be a matrix.");
            }
            uint32 numberOfColumns = 0u;
            uint32 numberOfRows = 0u;
            if (ok) {
                numberOfColumns = individualThreadCPUMask.GetNumberOfElements(0u);
                numberOfRows = individualThreadCPUMask.GetNumberOfElements(1u);
                ok = (numberOfColumns == 2u);
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "CPUMasks must have two columns.");
                }
            }
            Matrix<uint32> cpuMasksMat(numberOfRows, numberOfColumns);
            if (ok) {
                ok = data.Read("CPUMasks", cpuMasksMat);
            }
            uint32 r;
            for (r = 0u; (r < numberOfRows) && (ok); r++) {
                uint32 cpuMaskIdx = cpuMasksMat(r, 0u);
                uint32 cpuMaskForIdx = cpuMasksMat(r, 1u);
                ok = (cpuMaskIdx < GetNumberOfPoolThreads());
                if (!ok) {
                    REPORT_ERROR(ErrorManagement::ParametersError, "cpuMaskIdx must be < GetNumberOfPoolThreads()");
                }
                if (ok) {
                    SetCPUMaskThreadPool(cpuMaskForIdx, cpuMaskIdx);
                }
            }
            if (!ok) {
                err.parametersError = true;
            }
        }
    }
    return err;
}

ErrorManagement::ErrorType MultiThreadService::CreateThreads() {
    ErrorManagement::ErrorType err;
    err.illegalOperation = (threadPool.Size() > 0u);
    bool errorsCleared = err.ErrorsCleared();
    uint16 threadNumber = 0u;
    while ((threadPool.Size() < numberOfPoolThreads) && (errorsCleared)) {
        ReferenceT<EmbeddedThread> thread(new (NULL) EmbeddedThread(method, threadNumber));
        err.fatalError = !thread.IsValid();
        if (err.ErrorsCleared()) {
            thread->SetPriorityClass(GetPriorityClass());
            thread->SetPriorityLevel(GetPriorityLevel());
            thread->SetCPUMask(GetCPUMask());
            thread->SetTimeout(GetTimeout());
            StreamString tname;
            (void) tname.Printf("%s_%d", GetName(), threadNumber);
            thread->SetName(tname.Buffer());
        }
        if (err.ErrorsCleared()) {
            err.fatalError = !threadPool.Insert(thread);
        }
        threadNumber++;
        errorsCleared = err.ErrorsCleared();
    }
    return err;
}

ErrorManagement::ErrorType MultiThreadService::Start() {
    bool allStop = true;
    uint32 i;
    ErrorManagement::ErrorType err;
    if (threadPool.Size() == 0u) {
        err = CreateThreads();
    }
    else {
        if (err.ErrorsCleared()) {
            for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
                allStop = (GetStatus(i) == EmbeddedThreadI::OffState);
            }
        }
    }
    if (allStop) {
        bool ok = err.ErrorsCleared();
        for (i = 0u; (i < threadPool.Size()) && (ok); i++) {
            ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
            if (thread.IsValid()) {
                err = thread->Start();
            }
            else {
                err.fatalError = true;
            }
            ok = err.ErrorsCleared();
        }
    }
    else {
        err.illegalOperation = true;
        REPORT_ERROR(ErrorManagement::IllegalOperation, "Service must be stopped before starting.");
    }
    return err;
}

ErrorManagement::ErrorType MultiThreadService::Stop() {
    ErrorManagement::ErrorType err;
    uint32 i;
    for (i = 0u; i < threadPool.Size(); i++) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        if (thread.IsValid()) {
            if (thread->GetStatus() != EmbeddedThreadI::OffState) {
                err = thread->Stop();
                if (!err.ErrorsCleared()) {
                    uint32 threadNumber = i;
                    REPORT_ERROR(err, "Could not Stop EmbeddedThreadI(%d)", threadNumber);
                }
            }
        }
    }
// perform kill if necessary
    for (i = 0u; i < threadPool.Size(); i++) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        if (thread.IsValid()) {
            if (thread->GetStatus() != EmbeddedThreadI::OffState) {
                uint32 threadNumber = i;
                err = thread->Stop();
                if (!err.ErrorsCleared()) {
                    REPORT_ERROR(err, "Could not Kill EmbeddedThreadI(%d)", threadNumber);
                }
                else {
                    REPORT_ERROR(err, "Killed EmbeddedThreadI(%d)", threadNumber);
                }
            }
        }
    }
// remove dead threads
    threadPool.Purge();
    if (err.ErrorsCleared()) {
        // some service died hard
        err.timeout = (threadPool.Size() > 0u);
    }

    return err;

}

EmbeddedThreadI::States MultiThreadService::GetStatus(const uint32 threadIdx) {
    EmbeddedThreadI::States status = EmbeddedThreadI::OffState;
    if (threadIdx < threadPool.Size()) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            status = thread->GetStatus();
        }
    }

    return status;
}

uint32 MultiThreadService::GetNumberOfPoolThreads() const {
    return numberOfPoolThreads;
}

void MultiThreadService::SetNumberOfPoolThreads(const uint32 numberOfPoolThreadsIn) {
    if (threadPool.Size() == 0u) {
        numberOfPoolThreads = numberOfPoolThreadsIn;
    }
}

void MultiThreadService::SetTimeout(const TimeoutType & msecTimeoutIn) {
    EmbeddedServiceI::SetTimeout(msecTimeoutIn);
    uint32 i;
    for (i = 0u; i < threadPool.Size(); i++) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
        if (thread.IsValid()) {
            thread->SetTimeout(msecTimeoutIn);
        }
    }
}

void MultiThreadService::SetPriorityClass(const Threads::PriorityClassType priorityClassIn) {
    bool allStop = true;
    uint32 i;
    for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
        allStop = (GetStatus(i) == EmbeddedThreadI::OffState);
    }
    if (allStop) {
        EmbeddedServiceI::SetPriorityClass(priorityClassIn);
        for (i = 0u; i < threadPool.Size(); i++) {
            ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
            if (thread.IsValid()) {
                thread->SetPriorityClass(priorityClassIn);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority class cannot be changed if the service is running");
    }
}

void MultiThreadService::SetPriorityLevel(const uint8 priorityLevelIn) {
    bool allStop = true;
    uint32 i;
    for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
        allStop = (GetStatus(i) == EmbeddedThreadI::OffState);
    }
    if (allStop) {
        EmbeddedServiceI::SetPriorityLevel(priorityLevelIn);
        for (i = 0u; i < threadPool.Size(); i++) {
            ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
            if (thread.IsValid()) {
                thread->SetPriorityLevel(priorityLevelIn);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority level cannot be changed if the service is running");
    }
}

void MultiThreadService::SetStackSize(const uint32 stackSizeIn) {
    bool allStop = true;
    uint32 i;
    for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
        allStop = (GetStatus(i) == EmbeddedThreadI::OffState);
    }
    if (allStop) {
        EmbeddedServiceI::SetStackSize(stackSizeIn);
        for (i = 0u; i < threadPool.Size(); i++) {
            ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
            if (thread.IsValid()) {
                thread->SetStackSize(stackSizeIn);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Stack size level cannot be changed if the service is running");
    }
}

void MultiThreadService::SetCPUMask(const ProcessorType& cpuMaskIn) {
    bool allStop = true;
    uint32 i;
    for (i = 0u; (i < threadPool.Size()) && (allStop); i++) {
        allStop = (GetStatus(i) == EmbeddedThreadI::OffState);
    }
    if (allStop) {
        EmbeddedServiceI::SetCPUMask(cpuMaskIn);
        for (i = 0u; i < threadPool.Size(); i++) {
            ReferenceT<EmbeddedThreadI> thread = threadPool.Get(i);
            if (thread.IsValid()) {
                thread->SetCPUMask(cpuMaskIn);
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask cannot be changed if the service is running");
    }
}

Threads::PriorityClassType MultiThreadService::GetPriorityClassThreadPool(const uint32 threadIdx) {
    Threads::PriorityClassType prioClass = Threads::UnknownPriorityClass;
    if (threadIdx < threadPool.Size()) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            prioClass = thread->GetPriorityClass();
        }
    }
    return prioClass;
}

uint8 MultiThreadService::GetPriorityLevelThreadPool(const uint32 threadIdx) {
    uint8 prioLevel = 0u;
    if (threadIdx < threadPool.Size()) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            prioLevel = thread->GetPriorityLevel();
        }
    }
    return prioLevel;
}

uint32 MultiThreadService::GetStackSizeThreadPool(const uint32 threadIdx) {
    uint32 stackSizeAtIdx = 0u;
    if (threadIdx < threadPool.Size()) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            stackSizeAtIdx = thread->GetStackSize();
        }
    }
    return stackSizeAtIdx;
}

const char8 * const MultiThreadService::GetThreadNameThreadPool(const uint32 threadIdx) {
    const char8 * threadName = NULL_PTR(const char8 * const);
    if (threadIdx < threadPool.Size()) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            threadName = thread->GetName();
        }
    }
    return threadName;
}


ProcessorType MultiThreadService::GetCPUMaskThreadPool(const uint32 threadIdx) {
    ProcessorType cpuMaskForIdx = UndefinedCPUs;
    if (threadIdx < threadPool.Size()) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            cpuMaskForIdx = thread->GetCPUMask();
        }
    }
    return cpuMaskForIdx;
}

void MultiThreadService::SetPriorityClassThreadPool(const Threads::PriorityClassType priorityClassIn, const uint32 threadIdx) {

    if (GetStatus(threadIdx) == EmbeddedThreadI::OffState) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            thread->SetPriorityClass(priorityClassIn);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority class cannot be changed if the service is running");
    }
}

void MultiThreadService::SetPriorityLevelThreadPool(const uint8 priorityLevelIn, const uint32 threadIdx) {
    if (GetStatus(threadIdx) == EmbeddedThreadI::OffState) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            thread->SetPriorityLevel(priorityLevelIn);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority level cannot be changed if the service is running");
    }
}

void MultiThreadService::SetStackSizeThreadPool(const uint32 stackSizeIn, const uint32 threadIdx) {
    if (GetStatus(threadIdx) == EmbeddedThreadI::OffState) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            thread->SetStackSize(stackSizeIn);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Stack size cannot be changed if the service is running");
    }
}

void MultiThreadService::SetThreadNameThreadPool(const char8 * const threadName, const uint32 threadIdx) {
    if (GetStatus(threadIdx) == EmbeddedThreadI::OffState) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            thread->SetName(threadName);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Thread name cannot be changed if the service is running");
    }
}


void MultiThreadService::SetCPUMaskThreadPool(const ProcessorType& cpuMaskIn, const uint32 threadIdx) {
    if (GetStatus(threadIdx) == EmbeddedThreadI::OffState) {
        ReferenceT<EmbeddedThreadI> thread = threadPool.Get(threadIdx);
        if (thread.IsValid()) {
            thread->SetCPUMask(cpuMaskIn);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask cannot be changed if the service is running");
    }
}

}

