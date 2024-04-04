/**
 * @file HighResolutionTimerCalibrator.cpp
 * @brief Source file for class HighResolutionTimerCalibrator
 * @date 20/07/2015
 * @author Giuseppe Ferro
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

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <Windows.h>
#include <time.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimerCalibrator.h"
#include "HighResolutionTimer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

HighResolutionTimerCalibrator calibratedHighResolutionTimer;

HighResolutionTimerCalibrator::HighResolutionTimerCalibrator() {

    struct timeval initialTime;
    time((time_t *) &initialTime.tv_sec);

    //The precision is at the millisecond!
    SYSTEMTIME forMs;
    GetSystemTime(&forMs);
    initialSecs = initialTime.tv_sec;
    initialUSecs = forMs.wMilliseconds * 1000;

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

bool HighResolutionTimerCalibrator::GetTimeStamp(TimeStamp &timeStamp) {

    uint64 ticks = HighResolutionTimer::Counter() - initialTicks;

    //Use HRT
    uint32 secHRT = static_cast<uint32>(ticks * period);
    uint32 uSecHRT = static_cast<uint32>((ticks * period - secHRT) * 1e6);

    //Add HRT to the the initial time saved in the calibration.
    time_t sec = static_cast<time_t>(initialSecs + secHRT);
    timeStamp.SetMicroseconds(initialUSecs + uSecHRT);

    //Check the overflow
    if (timeStamp.GetMicroseconds() >= 1e6) {
        timeStamp.SetMicroseconds(timeStamp.GetMicroseconds() - 1e6);
        sec++;
    }

    time_t secondsFromEpoch32 = static_cast<time_t>(sec);

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
