/**
 * @file Sleep.cpp
 * @brief Source file for class Sleep
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

int GetDateSeconds() {
    return SleepOS::GetDateSeconds();
}

void SleepAtLeast(double sec) {
    SleepOS::AtLeast(sec);
}

void SleepNoMore(double sec) {
    SleepOS::NoMore(sec);
}

void SleepSecDouble(double sec) {
    SleepOS::SecDouble(sec);
}

void SleepSec(float sec) {
    SleepOS::SecFloat(sec);
}

void SleepMSec(int32 msec) {
    SleepOS::MSec(msec);
}

void SleepBusy(double sec) {
    int64 startCounter = HighResolutionTimer::Counter();
    int64 sleepUntil = startCounter + (int64) (sec * HighResolutionTimer::Frequency());
    while (HighResolutionTimer::Counter() < sleepUntil) {
    }
}

void SleepSemiBusy(double totalSleepSec,
                   double nonBusySleepSec) {
    SleepOS::SemiBusy(totalSleepSec, nonBusySleepSec);
}

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int Sleep::GetDateSeconds() {
    return ::GetDateSeconds();
}

void Sleep::AtLeast(double sec) {
    SleepAtLeast(sec);
}

void Sleep::NoMore(double sec) {
    SleepNoMore(sec);
}

void Sleep::Sec(double sec) {
    SleepSecDouble(sec);
}

void Sleep::Sec(float sec) {
    SleepSec(sec);
}

void Sleep::MSec(int32 msec) {
    SleepMSec(msec);
}

void Sleep::Busy(double sec) {
    SleepBusy(sec);
}

void Sleep::SemiBusy(double totalSleepSec,
                     double nonBusySleepSec) {
    SleepSemiBusy(totalSleepSec, nonBusySleepSec);
}
