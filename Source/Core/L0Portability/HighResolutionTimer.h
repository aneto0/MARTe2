/**
 * @file HighResolutionTimer.h
 * @brief Header file for class HighResolutionTimer
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

 * @details This header file contains the declaration of the class HighResolutionTimer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIME_H_
#define 		HIGHRESOLUTIONTIME_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,HighResolutionTimerA.h)

/**
 * @brief A structure containing the time stamp informations.
 */
struct TimeValues {

    /** nanoseconds 0-999999 */
    uint32 microseconds;

    /** seconds 0-59 */
    uint32 seconds;

    /** minutes 0-59 */
    uint32 minutes;

    /** hours 0-23 */
    uint32 hours;

    /** days 1-31 */
    uint32 days;

    /** month 0-11 */
    uint32 month;

    /** year since 1900 */
    uint32 year;
};

#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,HighResolutionTimerCalibratorOS.h)

extern "C" {
/**
 * @brief The frequency of the HighResolutionTimer Clock.
 * @return the clock frequency of the cpu.
 */
int64 HighResolutionTimerFrequency();

/**
 * @brief the HighResolutionTimer Clock period in seconds.
 * @return the cpu clock period in seconds (it must be the inverse of the frequency).
 */
double HighResolutionTimerPeriod();

/**
 * @brief How many ticks in a millisecond for the HRT.
 * @return the number of the cpu ticks during a millisecond.
 */
uint32 HighResolutionTimerMSecTics();

/**
 * @brief Get the current time stamp [microseconds, seconds, minutes, hour, day, month, year].
 * @see TimeValues.
 * @param[out] date is a TimeValues structure which must be filled by this method.
 */
bool HighResolutionTimerGetTimeStamp(TimeValues &date);

}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief This class implements useful functions for high resolution timing using the cpu clock.
 *
 * @details These functions are used a lot in each functions that need timeouts implementation like for examples semaphores.\n
 * Most of the implementation is delegated to HighResolutionTimerA.h which use very low level code (assembly)
 * for the Counter functions, while the Frequency and Period functions are delegated to HighResolutionTimerCalibratorOs.h
 * reading on a system file that returns the current cpu frequency.
 */
class HighResolutionTimer {

public:

    static HighResolutionTimerCalibratorOS highResolutionTimerCalibratorOS;

    /**
     * @brief An high resolution time counter. Only valid on pentiums CPUs and above.
     * @details Reads the cpu ticks on an 64 bits integer.
     */
    static inline int64 Counter();

    /**
     * @brief An high resolution time counter.
     * @details Reads the cpu ticks on an 32 bits integer.
     */
    static inline uint32 Counter32();

    /**
     * @brief To interpret the value returned by HRTCounter.
     * @see HighResolutionTimerFrequency().
     * @return the current frequency of the cpu.
     */
    static inline int64 Frequency();
    /**
     * @brief The length of a clock period in seconds.
     * @see HighResolutionTimerPeriod().
     * @return the current period of the cpu.
     */
    static inline double Period();

    /**
     * @brief Converts HRT ticks to time.
     * @param[in] tStop is the final ticks number.
     * @param[in] tStart is the initial ticks number.
     * @return the time elapsed in TStop-TStart ticks in seconds
     */
    static inline double TicksToTime(int64 tStop,
                                     int64 tStart = 0);

    /**
     * @brief Get the current time stamp [microseconds, seconds, minutes, hour, day, month, year].
     * @see TimeValues.
     * @param[out] date is a TimeValues structure which must be filled by this method.
     */
    static inline bool GetTimeStamp(TimeValues &date);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

int64 HighResolutionTimer::Counter() {
    return HighResolutionTimerA::Read64();
}

uint32 HighResolutionTimer::Counter32() {
    return HighResolutionTimerA::Read32();
}

int64 HighResolutionTimer::Frequency() {
    return HighResolutionTimerFrequency();
}

double HighResolutionTimer::Period() {
    return HighResolutionTimerPeriod();
}

double HighResolutionTimer::TicksToTime(int64 tStop,
                                        int64 tStart) {
    int64 dT = tStop - tStart;
    return dT * Period();
}

bool HighResolutionTimer::GetTimeStamp(TimeValues &date) {
    return HighResolutionTimerGetTimeStamp(date);
}

#endif /* HIGHRESOLUTIONTIME_H_ */

