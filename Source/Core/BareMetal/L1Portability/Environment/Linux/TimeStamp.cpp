/**
 * @file TimeStamp.cpp
 * @brief Source file for class TimeStamp
 * @date 11/11/2015
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

 * @details This source file contains the definition of all the methods for
 * the class TimeStamp (public, protected, and private). Be aware that some 
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
#include "../../TimeStamp.h"
#include "ErrorManagement.h"
#include "../../OSInitializer.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

TimeStamp::TimeStamp() {
    microseconds = 0u;
    seconds = 0u;
    minutes = 0u;
    hours = 0u;
    days = 0u;
    month = 0u;
    year = 1900u;
}

bool TimeStamp::GetTime() {

    uint64 ticksFromStart = HighResolutionTimer::Counter() - OSInitializer::initialTicks;

    //Use HRT
    float64 secondsFromStart = static_cast<float64>(ticksFromStart) * OSInitializer::period;
    float64 uSecondsFromStart = (secondsFromStart - floor(secondsFromStart)) * 1e6;

    //Add HRT to the the initial time saved in the calibration.
    float64 secondsFromEpoch = static_cast<float64>(OSInitializer::initialSecs) + secondsFromStart;
    float64 uSecondsFromEpoch = static_cast<float64>(OSInitializer::initialUSecs) + uSecondsFromStart;

    uint32 microseconds = static_cast<uint32>(uSecondsFromEpoch);

    //Check the overflow
    if (microseconds >= 1000000u) {
        microseconds -= 1000000u;
        secondsFromEpoch++;
    }

    SetMicroseconds(microseconds);

    //fill the time structure
    time_t secondsFromEpoch32 = static_cast<time_t>(secondsFromEpoch);

    bool ret = ConvertFromEpoch(secondsFromEpoch32);

    return ret;
}


bool TimeStamp::ConvertFromEpoch(const oslong secondsFromEpoch) {

    //fill the time structure
    const struct tm *tValues = localtime(&secondsFromEpoch);
    bool ret = (tValues != NULL);
    if (ret) {
        seconds = static_cast<uint32>(tValues->tm_sec);
        minutes = static_cast<uint32>(tValues->tm_min);
        hours = static_cast<uint32>(tValues->tm_hour);
        days = static_cast<uint32>(tValues->tm_mday) - 1u;
        month = static_cast<uint32>(tValues->tm_mon);
        year = static_cast<uint32>(tValues->tm_year) + 1900u;
    }

    else {
        REPORT_ERROR(ErrorManagement::OSError, "Error: localtime()");
    }
    return ret;
}

}
