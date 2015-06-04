/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id:  $
 *
 **/

/**
 * @file
 * @brief Access to the high resolution counters. 
 *
 * These routines enable
 * the calculation of high resolution timings.
 */
 
#ifndef HIGH_RESOLUTION_TIMER_H
#define HIGH_RESOLUTION_TIMER_H

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,HighResolutionTimerA.h)

extern "C"
{
    /** @brief The frequency of the HighResolutionTimer Clock.
      * @return the clock frequency of the cpu. */
    int64 HighResolutionTimerFrequency();

    /** @brief the HighResolutionTimer Clock period in seconds.
      * @return the cpu clock period in seconds (it must be the inverse of the frequency). */
    double HighResolutionTimerPeriod();

    /** @brief How many ticks in a msec for the HRT.
      * @return the number of the cpu ticks during a millisecond. */
    uint32 HighResolutionTimerMSecTics();
}

/** 
  * @brief This class implements useful functions for high resolution timing using the cpu clock. 
  *
  * These functions are used a lot in each functions that need timeouts implementation like for examples semaphores. 
  * Most of the implementation is delegated to HighResolutionTimerA.h which use very low level code (assembly)
  * for the Counter functions, while the Frequency and Period functions are delegated to HighResolutionTimerCalibratorOs.h
  * reading on a system file that returns the current cpu frequency. */

class HighResolutionTimer {

public:

    /** @brief An high resolution time counter. Only valid on pentiums CPUs and above.
      * Reads the cpu ticks on an 64 bits integer. */
    static inline int64 Counter() {
        return HighResolutionTimerRead64();
    }

    /** @brief An high resolution time counter. Only valid on pentiums CPUs and above 
      * Reads the cpu ticks on an 32 bits integer. */
    static inline uint32 Counter32() {
        return HighResolutionTimerRead32();
    }

    /** @brief To interpret the value returned by HRTCounter.
      * @see HighResolutionTimerFrequency(). 
      * @return the current frequency of the cpu. */
    static inline int64 Frequency() {
        return HighResolutionTimerFrequency();
    }

    /** @brief The length of a clock period in seconds.
      * @see HighResolutionTimerPeriod().
      * @return the current period of the cpu. */
    static inline double Period() {
        return HighResolutionTimerPeriod();
    }

    /** @brief Converts HRT ticks to time.
      * @param tStop is the final ticks number.
      * @param tStart is the initial ticks number.
      * @return the time elapsed in TStop-TStart ticks in seconds */
    static inline double TicksToTime(int64 tStop, int64 tStart = 0) {
        int64 dT = tStop - tStart;
        return dT * Period();
    }
};

#endif

