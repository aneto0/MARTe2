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
 * the class HighResolutionTimerCalibrator (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <FreeRTOS.h>
#include <timers.h>
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimer.h"
#include "HighResolutionTimerCalibrator.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/*lint -e{9141} constant that can be reused by other classes*/
HighResolutionTimerCalibrator calibratedHighResolutionTimer;

HighResolutionTimerCalibrator::HighResolutionTimerCalibrator() {
    initialTicks = HighResolutionTimer::Counter();
    frequency = 0;
    period = 0.;

    frequency = configTICK_RATE_HZ;
    period = 1.0 / frequency;
}

//This time is implemented as relative and not absolute (as per interface specification).
bool HighResolutionTimerCalibrator::GetTimeStamp(TimeStamp &timeStamp) const {
    uint32 microSecondsFromStart = static_cast<uint32>(HighResolutionTimer::Counter() * HighResolutionTimer::Period() * 1000000);
    timeStamp.SetMicroseconds((microSecondsFromStart % 1000000) * 1000000);
    uint32 secondsFromStart = static_cast<uint32>(HighResolutionTimer::Counter() * HighResolutionTimer::Period());
    timeStamp.SetDay(secondsFromStart / 86400000);
    timeStamp.SetHour(secondsFromStart / 3600);
    timeStamp.SetMinutes((secondsFromStart % 3600) / 60);
    timeStamp.SetSeconds((secondsFromStart % 3600) % 60);
    return true;
}

int64 HighResolutionTimerCalibrator::GetFrequency() const {
    return configTICK_RATE_HZ;
}

int64 HighResolutionTimerCalibrator::GetInitialTicks() const {
    return initialTicks;
}

float64 HighResolutionTimerCalibrator::GetPeriod() const {
    return period;
}

}
