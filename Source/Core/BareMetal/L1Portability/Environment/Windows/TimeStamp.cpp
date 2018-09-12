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
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <ctime>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "TimeStamp.h"
#include "ErrorManagement.h"
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

    uint64 ticks = HighResolutionTimer::Counter() - OSInitializer::initialTicks;

    //Use HRT
    uint32 secHRT = static_cast<uint32>(ticks * OSInitializer::period);
    uint32 uSecHRT = static_cast<uint32>((ticks * OSInitializer::period - secHRT) * 1e6);

    //Add HRT to the the initial time saved in the calibration.
    time_t sec = static_cast<time_t>(OSInitializer::initialSecs + secHRT);
    SetMicroseconds(OSInitializer::initialUSecs + uSecHRT);

    //Check the overflow
    if (GetMicroseconds() >= 1e6) {
        SetMicroseconds(GetMicroseconds() - 1e6);
        sec++;
    }

    time_t secondsFromEpoch32 = static_cast<time_t>(sec);

    bool ret = ConvertFromEpoch(secondsFromEpoch32);

    return ret;
}


bool TimeStamp::ConvertFromEpoch(const oslong secondsFromEpoch) {
    //fill the time structure
    struct tm tValues;
    time_t secondsFromEpochTimeT = secondsFromEpoch;
    if (localtime_s(&tValues, static_cast<const time_t*>(&secondsFromEpochTimeT)) != 0) {
        return false;
    }
    seconds = tValues.tm_sec;
    minutes = tValues.tm_min;
    hours = tValues.tm_hour;
    days = tValues.tm_mday - 1u;
    month = tValues.tm_mon;
    year = tValues.tm_year + 1900u;

    return true;
}
}

