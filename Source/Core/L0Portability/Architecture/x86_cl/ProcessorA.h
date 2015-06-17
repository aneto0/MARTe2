/**
 * @file ProcessorA.h
 * @brief Header file for class ProcessorA
 * @date 17/06/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class ProcessorA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROCESSORA_H_
#define 		PROCESSORA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <intrin.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Basically a global variable used to store the processor vendor
 */
class ProcessorId {
public:
    static char processorVendorId[13];
};

char ProcessorId::processorVendorId[13]={0};

/**
 * @brief Platform dependent functions to get the processor's informations.
 */
class ProcessorA {

public:

    /**
     * @brief Implementation of the cpuid function for x86 and cl
     * @param[in] code drives the type of request being asked, e.g. 0 for Vendor, 1 for Family and Model, ...
     * @param[out] A the CPU A register
     * @param[out] B the CPU B register
     * @param[out] C the CPU C register
     * @param[out] D the CPU D register
     */
    static inline void CPUID(uint32 code,
                             uint32 &A,
                             uint32 &B,
                             uint32 &C,
                             uint32 &D) {

        int32 cpuInfo[4];
        __cpuid(cpuInfo, code);

        A = cpuInfo[0];
        B = cpuInfo[1];
        C = cpuInfo[2];
        D = cpuInfo[3];
    }

    /**
     * @brief Get the cpu family.
     * @details Called by Processor::Family
     * @return the cpu family.
     */
    static uint32 Family() {
        uint32 eax = 0;
        uint32 ebx = 0;
        uint32 ecx = 0;
        uint32 edx = 0;
        CPUID(1, eax, ebx, ecx, edx);
        uint32 family = (eax >> 8) & 0xf;
        if (family == 0xf) {
            family += (eax >> 20) & 0xf;
        }
        return family;
    }

    /**
     * @brief Get the cpu model.
     * @details Called by Processor::Model()
     * @return the cpu model.
     */
    static uint32 Model() {
        uint32 eax = 0;
        uint32 ebx = 0;
        uint32 ecx = 0;
        uint32 edx = 0;
        CPUID(1, eax, ebx, ecx, edx);
        return (eax >> 4) & 0xf;
    }

    /**
     * @brief Get the identifier of the cpu.
     * @details Called by Processor::VendorId()
     * @return the cpu id.
     */
    static const char *VendorId() {

        uint32 eax = 0;
        CPUID(0, eax, (uint32 &) ProcessorId::processorVendorId[0], (uint32 &) ProcessorId::processorVendorId[8], (uint32 &) ProcessorId::processorVendorId[4]);
        ProcessorId::processorVendorId[12] = 0;
        return &(ProcessorId::processorVendorId[0]);
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROCESSORA_H_ */

