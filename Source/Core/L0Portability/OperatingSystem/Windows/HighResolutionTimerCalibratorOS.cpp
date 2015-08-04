/**
 * @file HighResolutionTimerCalibratorOS.cpp
 * @brief Source file for class HighResolutionTimerCalibratorOS
 * @date 20/07/2015
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

 * @details This source file contains the definition of all the methods for
 * the class HighResolutionTimerCalibratorOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimer.h"
#include "HighResolutionTimerCalibratorOS.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
HighResolutionTimerCalibratorOS calibratedHighResolutionTimer;


HighResolutionTimerCalibratorOS::HighResolutionTimerCalibratorOS() {
    time((time_t *) &initialTime.tv_sec);

    //The precision is at the millisecond!
    SYSTEMTIME forMs;
    GetSystemTime(&forMs);
    initialTime.tv_usec = forMs.wMilliseconds * 1000;

    initialTicks = HighResolutionTimer::Counter();

    LARGE_INTEGER tt0, tt1, tt2, tt3, tt4, tt5;
    uint64 dTa, dTb;
    dTa = 0;
    dTb = 0;
    for (int i = 0; i < 50; i++) {
        tt2.QuadPart = HighResolutionTimer::Counter();
        QueryPerformanceCounter(&tt0);
        tt3.QuadPart = HighResolutionTimer::Counter();
        tt4 = tt3;
        while ((tt4.QuadPart - tt3.QuadPart) < 100000)
            tt4.QuadPart = HighResolutionTimer::Counter(); // .5 ms at 200 Mhz

        QueryPerformanceCounter(&tt1);
        tt5.QuadPart = HighResolutionTimer::Counter();
        dTa += (tt1.QuadPart - tt0.QuadPart);
        dTb += ((tt5.QuadPart + tt4.QuadPart) - (tt3.QuadPart + tt2.QuadPart)) / 2;
    }
    QueryPerformanceFrequency(&tt0);
    frequency = tt0.QuadPart;
    frequency *= dTb;
    frequency /= dTa;

    frequency += 999999;
    frequency /= 2000000;
    frequency *= 2000000;

    period = 1.0 / frequency;
}

bool HighResolutionTimerCalibratorOS::GetTimeStamp(TimeValues &timeStamp) {

    uint64 ticks = HighResolutionTimer::Counter() - initialTicks;

    //Use HRT
    uint32 secHRT = static_cast<uint32>(ticks * period);
    uint32 uSecHRT = static_cast<uint32>((ticks * period - secHRT) * 1e6);

    //Add HRT to the the initial time saved in the calibration.
    time_t sec = static_cast<time_t>(initialTime.tv_sec + secHRT);
    timeStamp.microseconds = initialTime.tv_usec + uSecHRT;

    //Check the overflow
    if (timeStamp.microseconds >= 1e6) {
        timeStamp.microseconds -= 1e6;
        sec++;
    }

    //fill the time structure
    struct tm tValues;
    if (localtime_s(&tValues, static_cast<const time_t*>(&sec)) != 0u) {
        return false;
    }
    timeStamp.seconds = tValues.tm_sec;
    timeStamp.minutes = tValues.tm_min;
    timeStamp.hours = tValues.tm_hour;
    timeStamp.days = tValues.tm_mday;
    timeStamp.month = tValues.tm_mon;
    timeStamp.year = tValues.tm_year;
    return true;
}

int64 HighResolutionTimerCalibratorOS::GetFrequency() const {
    return frequency;
}

int64 HighResolutionTimerCalibratorOS::GetInitialTicks() const {
    return initialTicks;
}

const struct timeval& HighResolutionTimerCalibratorOS::GetInitialTime() const {
    return initialTime;
}

float64 HighResolutionTimerCalibratorOS::GetPeriod() const {
    return period;
}
