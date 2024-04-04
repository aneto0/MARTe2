/**
 * @file EmbeddedServiceI.cpp
 * @brief Source file for class EmbeddedServiceI
 * @date 05/09/2016
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
 * the class EmbeddedServiceI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "EmbeddedServiceI.h"
#include "StreamString.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
EmbeddedServiceI::EmbeddedServiceI() :
        ReferenceContainer() {
    priorityLevel = 0u;
    priorityClass = Threads::NormalPriorityClass;
    msecTimeout = TTInfiniteWait;
    cpuMask = UndefinedCPUs;
    stackSize = THREADS_DEFAULT_STACKSIZE;
}

EmbeddedServiceI::~EmbeddedServiceI() {
}

bool EmbeddedServiceI::Initialise(StructuredDataI &data) {
    uint32 msecTimeoutRead = 0u;
    ErrorManagement::ErrorType err = ReferenceContainer::Initialise(data);
    if (err.ErrorsCleared()) {
        err.parametersError = !data.Read("Timeout", msecTimeoutRead);
    }
    if (err.ErrorsCleared()) {
        if (msecTimeoutRead == 0u) {
            SetTimeout(TTInfiniteWait);
        }
        else {
            SetTimeout(msecTimeoutRead);
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Timeout has to be specified.");
    }
    uint8 priorityLevelRead = 0u;
    if (data.Read("PriorityLevel", priorityLevelRead)) {
        SetPriorityLevel(priorityLevelRead);
    }
    uint32 cpuMaskRead = 0u;
    if (data.Read("CPUMask", cpuMaskRead)) {
        SetCPUMask(cpuMaskRead);
    }
    uint32 stackSizeRead = 0u;
    if (data.Read("StackSize", stackSizeRead)) {
        SetStackSize(stackSizeRead);
    }
    StreamString priorityClassStr;
    if (data.Read("PriorityClass", priorityClassStr)) {
        if (priorityClassStr == "IdlePriorityClass") {
            SetPriorityClass(Threads::IdlePriorityClass);
        }
        else if (priorityClassStr == "NormalPriorityClass") {
            SetPriorityClass(Threads::NormalPriorityClass);
        }
        else if (priorityClassStr == "RealTimePriorityClass") {
            SetPriorityClass(Threads::RealTimePriorityClass);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Unsupported PriorityClass.");
            err.parametersError = true;
        }
    }

    return err;
}

void EmbeddedServiceI::SetTimeout(const TimeoutType &msecTimeoutIn) {
    msecTimeout = msecTimeoutIn;
}

TimeoutType EmbeddedServiceI::GetTimeout() const {
    return msecTimeout;
}

Threads::PriorityClassType EmbeddedServiceI::GetPriorityClass() const {
    return priorityClass;
}

void EmbeddedServiceI::SetPriorityClass(const Threads::PriorityClassType priorityClassIn) {
    priorityClass = priorityClassIn;
}

uint8 EmbeddedServiceI::GetPriorityLevel() const {
    return priorityLevel;
}

void EmbeddedServiceI::SetPriorityLevel(const uint8 priorityLevelIn) {
    priorityLevel = priorityLevelIn;
}

uint32 EmbeddedServiceI::GetStackSize() const {
    return stackSize;
}

void EmbeddedServiceI::SetStackSize(const uint32 stackSizeIn) {
    stackSize = stackSizeIn;
}

ProcessorType EmbeddedServiceI::GetCPUMask() const {
    return cpuMask;
}

void EmbeddedServiceI::SetCPUMask(const ProcessorType& cpuMaskIn) {
    cpuMask = cpuMaskIn;
}

}
