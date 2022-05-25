/**
 * @file HighResolutionTimer.h
 * @brief Header file for module HighResolutionTimer
 * @date 05/07/2015
 * @author André Neto
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

 * @details This header file contains the declaration of the module HighResolutionTimer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMER_ENV_H_
#define HIGHRESOLUTIONTIMER_ENV_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "../../HighResolutionTimer.h"
#include "HighResolutionTimerCalibrator.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

extern HighResolutionTimerCalibrator calibratedHighResolutionTimer;

namespace HighResolutionTimer {

inline uint64 Frequency() {
    return calibratedHighResolutionTimer.GetFrequency();
}

inline float64 Period() {
    return calibratedHighResolutionTimer.GetPeriod();
}

inline float64 TicksToTime(const uint64 tStop,
                           const uint64 tStart) {
    uint64 dT = (tStop - tStart);
    return static_cast<float64>(dT) * Period();
}

inline bool GetTimeStamp(TimeStamp &date) {
    return calibratedHighResolutionTimer.GetTimeStamp(date);
}

inline uint32 Counter32() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return static_cast<uint32>((ts.tv_sec * 1e9) + ts.tv_nsec);

    /* Modified by Giuseppe Avon, below original
    * Justification: Move to POSIX compliant due to ARM porting
    volatile uint64 perf = 0LLU;
    uint32 *pperf = (uint32 *) &perf;
    asm(
            "\n"
            "        rdtsc        \n"
            : "=a"(pperf[0]) , "=d"(pperf[1])
    );

    return (uint32) perf;
    */
}

inline uint64 Counter() {
        struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return static_cast<uint64>((ts.tv_sec * 1e9) + ts.tv_nsec);
    /* Modified by Giuseppe Avon, below original
    * Justification: Move to POSIX compliant due to ARM porting
    volatile uint64 perf = 0LLU;
    uint32 *pperf = (uint32 *) &perf;
    asm volatile(
            "\n"
            "        rdtsc        \n"
            : "=a"(pperf[0]) , "=d"(pperf[1])
    );
    return perf;
    */
}

}

}
#endif /* HIGHRESOLUTIONTIMER_ENV_H_ */
