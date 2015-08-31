/**
 * @file HighResolutionTimerOS.cpp
 * @brief Source file for class HighResolutionTimerOS
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
 * the class HighResolutionTimerCalibratorOS (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <math.h>
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimerCalibratorOS.h"
#include "HighResolutionTimer.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -e{9141} constant that can be reused by other classes*/
HighResolutionTimerCalibratorOS calibratedHighResolutionTimer;

HighResolutionTimerCalibratorOS::HighResolutionTimerCalibratorOS() {
    const size_t LINUX_CPUINFO_BUFFER_SIZE = 1023u;
    initialTicks = HighResolutionTimer::Counter();
    frequency = 0;
    period = 0.;

    int32 ret = gettimeofday(&initialTime, static_cast<struct timezone *>(NULL));
    if (ret == 0) {
        char8 buffer[LINUX_CPUINFO_BUFFER_SIZE + 1u];
        memset(&buffer[0], 0, LINUX_CPUINFO_BUFFER_SIZE + 1u);

        FILE *f = fopen("/proc/cpuinfo", "r");
        size_t size = LINUX_CPUINFO_BUFFER_SIZE;
        if (f != NULL) {
            size = fread(&buffer[0], static_cast<size_t>(size), static_cast<size_t>(1u), f);
            fclose(f);
        }
        else{
            REPORT_ERROR(ErrorManagement::OSError, "Error: fopen()")
        }

        if (size > 0u) {
            const char8 *pattern = "MHz";
            const char8 *p = StringHelper::SearchString(&buffer[0], pattern);
            if (p != NULL) {
                p = StringHelper::SearchString(p, ":");
                p++;
                float64 freqMHz = strtof(p, static_cast<char8 **>(0));
                if (freqMHz > 0.) {
                    float64 frequencyF = freqMHz *= 1.0e6;
                    period = 1.0 / frequencyF;
                    frequency = static_cast<int64>(frequencyF);
                }
            }
        }
        else{
            REPORT_ERROR(ErrorManagement::OSError, "Error: fread()")
        }
    }
    else{
        REPORT_ERROR(ErrorManagement::OSError, "Error: gettimeofday()")
    }
}

bool HighResolutionTimerCalibratorOS::GetTimeStamp(TimeValues &timeStamp) const {

    int64 ticksFromStart = HighResolutionTimer::Counter() - initialTicks;

    //Use HRT
    float64 secondsFromStart = static_cast<float64>(ticksFromStart) * period;
    float64 uSecondsFromStart = (secondsFromStart - floor(secondsFromStart)) * 1e6;

    //Add HRT to the the initial time saved in the calibration.
    float64 secondsFromEpoch = static_cast<float64>(initialTime.tv_sec) + secondsFromStart;
    float64 uSecondsFromEpoch = static_cast<float64>(initialTime.tv_usec) + uSecondsFromStart;
    timeStamp.microseconds = static_cast<uint32>(uSecondsFromEpoch);

    //Check the overflow
    if (timeStamp.microseconds >= 1000000u) {
        timeStamp.microseconds -= 1000000u;
        secondsFromEpoch++;
    }

    //fill the time structure
    time_t secondsFromEpoch32 = static_cast<time_t>(secondsFromEpoch);
    const struct tm *tValues = localtime(&secondsFromEpoch32);
    bool ret = (tValues != NULL);
    if (ret) {
        timeStamp.seconds = static_cast<uint32>(tValues->tm_sec);
        timeStamp.minutes = static_cast<uint32>(tValues->tm_min);
        timeStamp.hours = static_cast<uint32>(tValues->tm_hour);
        timeStamp.days = static_cast<uint32>(tValues->tm_mday);
        timeStamp.month = static_cast<uint32>(tValues->tm_mon);
        timeStamp.year = static_cast<uint32>(tValues->tm_year);
    }
    else{
        REPORT_ERROR(ErrorManagement::OSError, "Error: localtime()")
    }
    return ret;
}

int64 HighResolutionTimerCalibratorOS::GetFrequency() const {
    return frequency;
}

float64 HighResolutionTimerCalibratorOS::GetPeriod() const {
    return period;
}
