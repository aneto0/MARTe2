/**
 * @file Sleep.h
 * @brief Header file for module Sleep
 * @date 17/06/2015
 * @author Giuseppe Ferrò
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "License")
 * You may not use this work except in compliance with the License.
 * You may obtain a copy of the License at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License permissions and limitations under the License.

 * @details This header file contains the declaration of the module Sleep
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SLEEP_H_
#define SLEEP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace Sleep {

/**
 * @brief Retrieve the time as seconds from the start of time.
 * @return the seconds elapsed from 00:00 of Jan 1, 1970.
 */
DLL_API int32 GetDateSeconds(void);

/**
 * @brief Sleeps the requested time trying not to sleep more.
 * @details This function uses HighResolutionTimer functions. It adopts a mix between a busy-sleep and a OS yield one.
 * @param[in] usec is the time in micro-seconds to sleep (no more). Max sleep time is 4000 seconds.
 * @param[in] margin is the number of OS schedule granularities to leave to the busy-sleep part.
 */
DLL_API void PreciseUsec(uint32 usec, uint32 margin=1);

/**
 * @brief Sleeps for sec seconds (float32 value).
 * @details This function uses HighResolutionTimer functions. It adopts a mix between a busy-sleep and a OS yield one.
 * @param[in] seconds is the time to sleep. Max sleep time is 4E6 seconds. Below 4000 a precise sleep algorithm is used.
 * @param[in] margin is the number of OS schedule granularities to leave to the busy-sleep part.
 */
DLL_API void PreciseSeconds(const float32 seconds, uint32 margin=1);


/**
 * @brief Sleep without yield cpu.
 * @details This function uses HighResolutionTimer functions.
 * @param[in] sec is the seconds to sleep.
 */
inline void Busy(float32 seconds);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void Sleep::Busy(const float32 seconds) {
    uint64 startCounter = HighResolutionTimer::Counter();
    float64 frequencyF = static_cast<float64>(HighResolutionTimer::Frequency());
    float64 secondsAsTicksF = (seconds * frequencyF);
    uint64 secondsAsTicks = static_cast<uint64>(secondsAsTicksF);
    uint64 sleepUntilCounter = startCounter + secondsAsTicks;

    // this will overflow - but it is ok
    uint64 toSleepLeft = (HighResolutionTimer::Counter() - sleepUntilCounter);
    while (toSleepLeft < secondsAsTicks) {
        toSleepLeft = (HighResolutionTimer::Counter() - sleepUntilCounter);
    }
}

} // Sleep

} // MARTe
#endif /* SLEEP_H_ */
