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
#include "AdvancedErrorManagement.h"
#include "ExecutionInfo.h"
#include "SingleThreadService.h"

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
}

/*lint -e{1551} the only reason why this could throw an exception is if
 * the callback method throws an exception while stopping (but this is not
 * caught anyway). */
SingleThreadService::~SingleThreadService() {
    ErrorManagement::ErrorType err = SingleThreadService::Stop();
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(err, "Could not Stop all the EmbeddedThreadI instances (first time).");
    }
    err = SingleThreadService::Stop();
    if (!err.ErrorsCleared()) {
        REPORT_ERROR(err, "Could not Stop all the EmbeddedThreadI instances (second time).");
    }
}

ErrorManagement::ErrorType SingleThreadService::Start() {
    embeddedThread.SetName(GetName());
    return embeddedThread.Start();
}

ErrorManagement::ErrorType SingleThreadService::Stop() {
    return embeddedThread.Stop();
}

EmbeddedThreadI::States SingleThreadService::GetStatus() {
    return embeddedThread.GetStatus();
}

void SingleThreadService::SetTimeout(const TimeoutType &msecTimeoutIn) {
    EmbeddedServiceI::SetTimeout(msecTimeoutIn);
    embeddedThread.SetTimeout(msecTimeoutIn);
}

void SingleThreadService::SetPriorityClass(const Threads::PriorityClassType priorityClassIn) {
    if (GetStatus() == EmbeddedThreadI::OffState) {
        EmbeddedServiceI::SetPriorityClass(priorityClassIn);
        embeddedThread.SetPriorityClass(priorityClassIn);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority class cannot be changed if the service is running");
    }
}

void SingleThreadService::SetPriorityLevel(const uint8 priorityLevelIn) {
    if (GetStatus() == EmbeddedThreadI::OffState) {
        EmbeddedServiceI::SetPriorityLevel(priorityLevelIn);
        embeddedThread.SetPriorityLevel(priorityLevelIn);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Priority level cannot be changed if the service is running");
    }
}

void SingleThreadService::SetStackSize(const uint32 stackSizeIn) {
    if (GetStatus() == EmbeddedThreadI::OffState) {
        EmbeddedServiceI::SetStackSize(stackSizeIn);
        embeddedThread.SetStackSize(stackSizeIn);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "Stack size cannot be changed if the service is running");
    }
}

void SingleThreadService::SetCPUMask(const ProcessorType& cpuMaskIn) {
    if (GetStatus() == EmbeddedThreadI::OffState) {
        EmbeddedServiceI::SetCPUMask(cpuMaskIn);
        embeddedThread.SetCPUMask(cpuMaskIn);
    }
    else {
        REPORT_ERROR(ErrorManagement::ParametersError, "CPUMask cannot be changed if the service is running");
    }
}

}
