/**
 * @file SleepOS.cpp
 * @brief Source file for class SleepOS
 * @date 20/06/2015
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
 * the class SleepOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <time.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Sleep.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static const uint32 winSleepFreq = 1000;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void Sleep::AtLeast(float64 sec) {
    int32 ticks = (int32) (winSleepFreq * sec + 0.9999);
    if (ticks < 0) {
        return;
    }

    ::Sleep(ticks);
}

void Sleep::NoMore(float64 sec) {
    int ticks = (int) (winSleepFreq * sec);
    if (ticks < 0)
        return;

    ::Sleep(ticks);
}

void Sleep::Sec(float64 sec) {
    if (sec < 0)
        return;

    ::Sleep((unsigned long) (sec * 1000.0 + 0.5));
}

void Sleep::MSec(int32 msec) {
    if (msec < 0)
        return;

    ::Sleep(msec);
}

void Sleep::SemiBusy(float64 totalSleepSec,
                     float64 nonBusySleepSec) {
    NoMore(totalSleepSec);
}

int32 Sleep::GetDateSeconds() {
    return (int32) time((time_t *) NULL);
}
