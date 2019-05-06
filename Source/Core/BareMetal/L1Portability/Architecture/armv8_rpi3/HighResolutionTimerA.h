/**
 * @file HighResolutionTimerA.h
 * @brief Header file for module HighResolutionTimerA
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

 * @details This header file contains the declaration of the module HighResolutionTimerA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMERA_H_
#define HIGHRESOLUTIONTIMERA_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


namespace MARTe {

namespace HighResolutionTimer {

uint64 Frequency();




inline uint64 Counter() {

#ifdef _USE_HRT_PERF_MODULE

    volatile uint32 cc = 0;
    __asm__ volatile ("mrc p15, 0, %0, c9, c13, 0":"=r" (cc));

    return ((uint64)(cc))<<6;
#else
    struct timespec now;
    clock_gettime(CLOCK_REALTIME, &now);

    double counter_x=((now.tv_sec*1000000000. + now.tv_nsec)/1e9)*(HighResolutionTimer::Frequency());
    return (uint64)counter_x;
#endif

}

inline uint32 Counter32() {
#ifdef _USE_HRT_PERF_MODULE
    volatile uint32 cc = 0;
    __asm__ volatile ("mrc p15, 0, %0, c9, c13, 0":"=r" (cc));
    return cc<<6;
#else
  return (uint32)Counter();
#endif

}


}
}
#endif /* HIGHRESOLUTIONTIMERA_H_ */
