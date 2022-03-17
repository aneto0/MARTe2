/**
 * @file Sleep.h
 * @brief Header file for module Sleep
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

/**
 * @brief MARTe Sleep functions.
 */
class DLL_API Sleep {
public:
    /**
     * @brief Retrieve the time as seconds from the start of time.
     * @return the seconds elapsed from 00:00 of Jan 1, 1970.
     */
    static int32 GetDateSeconds(void);

    /**
     * @brief Sleeps no more than the requested time.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the time in seconds to sleep (no more).
     */
    static inline void NoMore(const float32 sec);

    /**
     * @brief Sleeps for sec seconds (float64 value).
     * @param[in] sec is the time to sleep.
     */
    static inline void Sec(const float32 sec);

    /**
     * @brief Sleeps for msec milliseconds.
     * @param[in] msec is the number of milliseconds to sleep.
     */
    static inline void MSec(const uint32 msec);

    /**
     * @brief Sleep without yield cpu.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the seconds to sleep.
     */
    static inline void Busy(float32 sec);

    /**
     * @brief Sleep yielding cpu for nonBusySleepSec.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] totalSleepSec is the total time in seconds to sleep.
     * @param[in] nonBusySleepSec is the time to sleep without use cpu.
     */
    static inline void SemiBusy(const float32 totalSleepSec,
            const float32 nonBusySleepSec);

    /**
     * @brief Gets the scheduler granularity (i.e. any requests to sleep no more than this value, will busy sleep).
     * @return the scheduler granularity in micro-seconds.
     */
    static uint32 GetSchedulerGranularity();

    /**
     * @brief Sets the scheduler granularity (i.e. any requests to sleep no more than this value, will busy sleep).
     * @param[in] granularity the scheduler granularity in micro-seconds.
     */
    static void SetSchedulerGranularity(const uint32 &granularity);

private:

    /**
     * @brief Function to be used by all the other methods
     * @param[in] totalUsecTime is the total time in micro-seconds to sleep.
     * @param[in[ nonBusyUsecTime is the time to sleep using the operating system sleep function.
     */
    static inline void MicroSeconds(uint32 totalUsecTime, uint32 nonBusyUsecTime);

    /**
     * @brief Wraps the operating system sleep call
     * @param[in] usecTime is the time to sleep in micro-seconds
     */
    static void OsUsleep(uint32 usecTime);

    /**
     * The scheduler granularity (i.e. any requests to sleep no more than this value, will busy sleep).
     */
    static uint32 schedulerGranularity;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void Sleep::NoMore(const float32 sec) {

    uint32 uSec = static_cast<uint32>((sec * 1e6) + 0.5);

    uint32 toGrow = schedulerGranularity;
    while (toGrow < uSec) {
        toGrow += schedulerGranularity;
    }
    toGrow -= schedulerGranularity;

    MicroSeconds(uSec, toGrow);

}

void Sleep::Sec(const float32 sec) {
    uint32 usecTime = static_cast<uint32>((sec * 1e6) + 0.5);
    MicroSeconds(usecTime, usecTime);
}

void Sleep::MSec(const uint32 msec) {
    uint32 usec = (msec * 1000u);
    MicroSeconds(usec, usec);
}

void Sleep::Busy(float32 sec) {
    uint32 usecTime = static_cast<uint32>((sec * 1e6) + 0.5);
    MicroSeconds(usecTime, 0u);
}

void Sleep::SemiBusy(const float32 totalSleepSec,
                     const float32 nonBusySleepSec) {
    uint32 usecTotalTime = static_cast<uint32>((totalSleepSec * 1e6) + 0.5);
    uint32 nonBusyTime = static_cast<uint32>((nonBusySleepSec * 1e6) + 0.5);
    MicroSeconds(usecTotalTime, nonBusyTime);
}

void Sleep::MicroSeconds(uint32 totalUsecTime,
                         uint32 nonBusyUsecTime) {
    
    uint64 startCounter = HighResolutionTimer::Counter();
    uint64 deltaTicks = static_cast<uint64>(totalUsecTime * static_cast<float64>(HighResolutionTimer::Frequency()) / 1e6);

    OsUsleep(nonBusyUsecTime);
    //Never try to busy sleep if not requested by the caller. This is to avoid issues with times that might be on the past
    if (totalUsecTime != nonBusyUsecTime) {
        while ((HighResolutionTimer::Counter() - startCounter) < deltaTicks) {
        }
    }
}

}
#endif /* SLEEP_H_ */
