/**
 * @file HighResolutionTimerA.h
 * @brief Header file for class HighResolutionTimerA
 * @date 05/ago/2015
 * @author pc
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

 * @details This header file contains the declaration of the class HighResolutionTimerA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMERA_H_
#define HIGHRESOLUTIONTIMERA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
/*
 #include "FreeRTOS.h"
 #include "task.h"*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace HighResolutionTimer {

inline uint64 Counter() {
#if AARCH==64
    uint64 nCNTPCT;
    asm volatile ("mrs %0, CNTPCT_EL0" : "=r" (nCNTPCT));
#else
    uint32 nCNTPCTLow;
    uint32 nCNTPCTHigh;
    asm volatile ("mrrc p15, 0, %0, %1, c14" : "=r" (nCNTPCTLow), "=r" (nCNTPCTHigh));
    uint64 nCNTPCT = (nCNTPCTHigh << 32) | nCNTPCTLow;
#endif
    return nCNTPCT;
}

inline uint32 Counter32() {
#if AARCH==64
    uint64 nCNTPCT;
    asm volatile ("mrs %0, CNTPCT_EL0" : "=r" (nCNTPCT));
#else
    uint32 nCNTPCT;
    uint32 nCNTPCTHigh;
    asm volatile ("mrrc p15, 0, %0, %1, c14" : "=r" (nCNTPCT), "=r" (nCNTPCTHigh));
#endif
    return (uint32) nCNTPCT;
}

}
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HIGHRESOLUTIONTIMERA_H_ */

