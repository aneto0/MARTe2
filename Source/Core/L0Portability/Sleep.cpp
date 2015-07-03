/**
 * @file Sleep.cpp
 * @brief Source file for class Sleep
 * @date 17/06/2015
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
 * the class Sleep (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Sleep.h"

int32 GetDateSeconds() {
    return SleepOS::GetDateSeconds();
}

void SleepAtLeast(const float64 sec) {
    SleepOS::AtLeast(sec);
}

void SleepNoMore(const float64 sec) {
    SleepOS::NoMore(sec);
}

void SleepSecDouble(const float64 sec) {
    SleepOS::SecDouble(sec);
}

void SleepSec(const float32 sec) {
    SleepOS::SecFloat(sec);
}

void SleepMSec(const int32 msec) {
    SleepOS::MSec(msec);
}

void SleepBusy(const float64 sec) {
    int64 startCounter = HighResolutionTimer::Counter();
    int64 endCounter = static_cast<int64>(sec) * HighResolutionTimer::Frequency();
    int64 sleepUntilCounter = startCounter + endCounter;
    while (HighResolutionTimer::Counter() < sleepUntilCounter) {
    }
}

void SleepSemiBusy(const float64 totalSleepSec,
                   const float64 nonBusySleepSec) {
    SleepOS::SemiBusy(totalSleepSec, nonBusySleepSec);
}

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int32 Sleep::GetDateSeconds(void) {
    return ::GetDateSeconds();
}

void Sleep::AtLeast(const float64 sec) {
    SleepAtLeast(sec);
}

void Sleep::NoMore(const float64 sec) {
    SleepNoMore(sec);
}

void Sleep::Sec(const float64 sec) {
    SleepSecDouble(sec);
}

void Sleep::Sec(const float32 sec) {
    SleepSec(sec);
}

void Sleep::MSec(const int32 msec) {
    SleepMSec(msec);
}

void Sleep::Busy(const float64 sec) {
    SleepBusy(sec);
}

void Sleep::SemiBusy(const float64 totalSleepSec, const float64 nonBusySleepSec) {
    SleepSemiBusy(totalSleepSec, nonBusySleepSec);
}
