/**
 * @file ProcessorType.cpp
 * @brief Source file for class ProcessorType
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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
 * the class ProcessorType (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ProcessorType.h"

#if ProcessorTypeDefaultCPUs
/*lint -e{9141} required to allow the setting the default CPUs at compilation time*/
uint32 ProcessorType::defaultCPUs = ProcessorTypeDefaultCPUs;
#else
uint32 ProcessorType::defaultCPUs = 0;
#endif


/*lint -e{9141} required to allow the setting the default CPUs at compilation time*/
uint32 ProcessorType::GetDefaultCPUs() {
    return ProcessorType::defaultCPUs;
}

/*lint -e{9141} required to allow the setting the default CPUs at compilation time*/
void ProcessorType::SetDefaultCPUs(const uint32 &mask) {
    ProcessorType::defaultCPUs = mask;
}

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void ProcessorType::SetMask(const uint32 mask) {
    processorMask = mask;
}

void ProcessorType::AddCPU(const uint32 cpuNumber) {
    uint32 cpuMask = 1u;
    cpuMask = cpuMask << (cpuNumber - 1u);
    processorMask |= cpuMask;
}

void ProcessorType::operator=(const uint32 cpuMask) {
    processorMask = cpuMask;
}

ProcessorType& ProcessorType::operator=(const ProcessorType &pt) {
    if (this != &pt) {
        processorMask = pt.processorMask;
    }
    return *this;
}

void ProcessorType::operator|=(const uint32 cpuMask) {
    processorMask |= cpuMask;
}

void ProcessorType::operator|=(const ProcessorType &pt) {
    processorMask |= pt.processorMask;
}

bool ProcessorType::operator==(const ProcessorType &pt) const {
    return processorMask == pt.processorMask;
}

bool ProcessorType::operator==(const uint32 mask) const {
    return processorMask == mask;
}

bool ProcessorType::operator!=(const ProcessorType &pt) const {
    return processorMask != pt.processorMask;
}

bool ProcessorType::operator!=(const uint32 mask) const {
    return processorMask != mask;
}

uint32 ProcessorType::GetProcessorMask() const {
    return processorMask;
}

ProcessorType::ProcessorType(const uint32 cpuMask) {
    processorMask = cpuMask;
}

ProcessorType::ProcessorType(const ProcessorType &pt) {
    processorMask = pt.processorMask;
}
