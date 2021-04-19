/**
 * @file ProcessorA.h
 * @brief Header file for module ProcessorA
 * @date 17/06/2015
 * @author Giuseppe Ferro
 * @author Luca Boncagni
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

 * @details This header file contains the declaration of the module ProcessorA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROCESSORA_H_
#define PROCESSORA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#ifndef LINT
#include <string.h>
#endif

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Processor.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace Processor {

/**
 * Buffer used to store the processor identifier
 */
extern char8 processorVendorId[13];

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
#if defined( i386 ) && defined ( PIC )
    /* in case of PIC under 32-bit EBX cannot be clobbered */
    __asm__ volatile ("movl %ebx, %%edi \n\t cpuid \n\t xchgl %ebx, %%edi" : "=D" (ebx), "+a" (eax), "+c" (ecx), "=d" (edx));
#else
    __asm__(
            "cpuid;" /* assembly code */
            :"=a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx) /* outputs */
            :"a" (info) /* input: info into eax */
            /* clobbers: none */
    );
#endif
}

inline uint32 Family() {
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

inline uint32 Model() {
    uint32 eax = 0;
    uint32 ebx = 0;
    uint32 ecx = 0;
    uint32 edx = 0;
    CPUID(1, eax, ebx, ecx, edx);
    uint32 model = (eax >> 4) & 0xf;
    uint32 family = Processor::Family();
    if ((family == 6) || (family == 15)) {
        uint32 extendedModel = (eax >> 16) & 0xf;
        model += (extendedModel << 4);
    }
    return model;
}

inline const char8 *VendorId() {
    uint32 eax = 0;
    uint32 ebx = 0;
    uint32 ecx = 0;
    uint32 edx = 0;

    CPUID(0, eax, ebx, ecx, edx);
    memcpy(&processorVendorId[0], &ebx, 4u);
    memcpy(&processorVendorId[8], &ecx, 4u);
    memcpy(&processorVendorId[4], &edx, 4u);
    processorVendorId[12] = 0;
    return &(processorVendorId[0]);
}

}

}
#endif /* PROCESSORA_H_ */
