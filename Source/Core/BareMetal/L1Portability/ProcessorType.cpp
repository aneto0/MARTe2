/**
 * @file ProcessorType.cpp
 * @brief Source file for class ProcessorType
 * @date 17/06/2015
 * @author Giuseppe Ferro
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
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include "ProcessorType.h"

namespace MARTe{

#if ProcessorTypeDefaultCPUs
BitSet ProcessorType::defaultCPUs = ProcessorTypeDefaultCPUs;
#else
BitSet ProcessorType::defaultCPUs = 0u;
#endif


ProcessorType ProcessorType::GetDefaultCPUs() {
    return ProcessorType(ProcessorType::defaultCPUs);
}

void ProcessorType::SetDefaultCPUs(const BitSet &mask) {
    ProcessorType::defaultCPUs = mask;
}

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void ProcessorType::SetMask(const BitSet mask) {
    processorMask = mask;
}

void ProcessorType::AddCPU(const uint32 cpuNumber) {
    // NOTE no test to ensure that cpuNumber is smaller than the maximum size
    // of the bitmask.
    processorMask.Set(cpuNumber - 1u, true);
}

const bool ProcessorType::CPUEnabled(const uint32 cpuNumber) const {
    return processorMask.Bit(cpuNumber -1u);
}

const uint32 ProcessorType::GetCPUsNumber() const {
    return processorMask.GetNumberOfElements() * 32u;
}    

const uint32 ProcessorType::GetLegacyUint32Mask() const {
    return static_cast<uint32>(processorMask);
}

void ProcessorType::operator=(const BitSet cpuMask) {
    processorMask = cpuMask;
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

void ProcessorType::operator|=(const ProcessorType &pt) {
    processorMask |= pt.processorMask;
}

bool ProcessorType::operator==(const ProcessorType &pt) const {
    return processorMask == pt.processorMask;
}

bool ProcessorType::operator!=(const ProcessorType &pt) const {
    return processorMask != pt.processorMask;
}

ProcessorType::ProcessorType(const BitSet cpuMask) {
    processorMask = cpuMask;
}

ProcessorType::ProcessorType(const ProcessorType &pt) {
    processorMask = pt.processorMask;
}

ProcessorType::ProcessorType(const uint32 &cpuMask){
    processorMask = cpuMask;
}

BitSet ProcessorType::GetProcessorMask() const{
    return processorMask;
}


}
