/**
 * @file HighResolutionTimer.cpp
 * @brief Source file for class HighResolutionTimer
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

 * @details This source file contains the definition of all the methods for
 * the class HighResolutionTimerCalibrator (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#ifndef LINT
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimerCalibrator.h"
#include "HighResolutionTimer.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{9141} constant that can be reused by other classes*/
HighResolutionTimerCalibrator calibratedHighResolutionTimer;

HighResolutionTimerCalibrator::HighResolutionTimerCalibrator() {
    struct timespec initTime;
    memset(&initTime, 0, sizeof(struct timespec));
    // From man clock_gettime(2)
    //    CLOCK_MONOTONIC_RAW (since Linux 2.6.28; Linux-specific)
    //       Similar to CLOCK_MONOTONIC, but provides access to a raw
    //       hardware-based time that is not subject to NTP adjustments
    //       or the incremental adjustments performed by adjtime(3).
    //       This clock does not count time that the system is
    //       suspended.
    int32 ret = clock_gettime(CLOCK_MONOTONIC_RAW, &initTime);

    initialTicks = HighResolutionTimer::Counter();
    //We already know that using gettime, the granularity is in nanoseconds
    //and so the frequency is expressed in GHz
    period = 1.0e-9;
    frequency = 1000000000u;
    
    initialSecs = initTime.tv_sec;
    initialUSecs = initTime.tv_nsec / 1000;

    if(ret == 0) {
        REPORT_ERROR_STATIC_0(ErrorManagement::OSError, "HighResolutionTimerCalibrator: clock_gettime()");
    }

}

bool HighResolutionTimerCalibrator::GetTimeStamp(TimeStamp &timeStamp) const {

    uint64 ticksFromStart = HighResolutionTimer::Counter() - initialTicks;

    //Use HRT
    float64 secondsFromStart = static_cast<float64>(ticksFromStart) * period;
    float64 uSecondsFromStart = (secondsFromStart - floor(secondsFromStart)) * 1e6;

    //Add HRT to the the initial time saved in the calibration.
    float64 secondsFromEpoch = static_cast<float64>(initialSecs) + secondsFromStart;
    float64 uSecondsFromEpoch = static_cast<float64>(initialUSecs) + uSecondsFromStart;

    uint32 microseconds = static_cast<uint32>(uSecondsFromEpoch);

    //Check the overflow
    if (microseconds >= 1000000u) {
        microseconds -= 1000000u;
        secondsFromEpoch++;
    }

    timeStamp.SetMicroseconds(microseconds);

    //fill the time structure
    time_t secondsFromEpoch32 = static_cast<time_t>(secondsFromEpoch);

    bool ret = timeStamp.ConvertFromEpoch(secondsFromEpoch32);

    return ret;
}

uint64 HighResolutionTimerCalibrator::GetFrequency() const {
    return frequency;
}

float64 HighResolutionTimerCalibrator::GetPeriod() const {
    return period;
}


}
