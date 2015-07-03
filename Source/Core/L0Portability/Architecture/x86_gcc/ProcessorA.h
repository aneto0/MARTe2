/**
 * @file ProcessorA.h
 * @brief Header file for class ProcessorA
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

 * @details This header file contains the declaration of the class ProcessorA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROCESSORA_H_
#define 		PROCESSORA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Platform dependent functions to get the processor's informations.
 */
class ProcessorA {

public:

    /**
     * @brief Implementation of the cpuid function for x86 and gcc
     * @param[in] info drives the type of request being asked, e.g. 0 for Vendor, 1 for Family and Model, ...
     * @param[out] eax the CPU EAX register
     * @param[out] ebx the CPU EBX register
     * @param[out] ecx the CPU ECX register
     * @param[out] edx the CPU EDX register
     */
    static inline void CPUID(uint32 info,
                             uint32 &eax,
                             uint32 &ebx,
                             uint32 &ecx,
                             uint32 &edx) {
        __asm__(
                "cpuid;" /* assembly code */
                :"=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) /* outputs */
                :"a" (info) /* input: info into eax */
                /* clobbers: none */
        );
    }

    /**
     * @brief Get the cpu family.
     * @details Called by Processor::Family()
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
    static const char8 *VendorId() {

        uint32 eax = 0;
        CPUID(0, eax, (uint32 &) processorVendorId[0], (uint32 &) processorVendorId[8], (uint32 &) processorVendorId[4]);
        processorVendorId[12] = 0;
        return &(processorVendorId[0]);
    }

private:

    /** Buffer used to store the processor identifier */
    static char8 processorVendorId[13];

};

char8 ProcessorA::processorVendorId[13] = { 0 };

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROCESSORA_H_ */

