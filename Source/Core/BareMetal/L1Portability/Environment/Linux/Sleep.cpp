/**
 * @file Sleep.cpp
 * @brief Source file for module Sleep
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
 * the module Sleep (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <string.h>
#include <math.h>
#include <errno.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HighResolutionTimer.h"
#include "Sleep.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace Sleep {

void AtLeast(const float64 sec) {
    int32 nsecRemainder = -1;
    struct timespec timesValues;
    struct timespec remTimesValues;
    float64 roundValue = floor(sec);
    float64 nsecSleep = (sec - roundValue) * 1e9;
    timesValues.tv_sec = static_cast<time_t>(roundValue);
    timesValues.tv_nsec = static_cast<oslong>(nsecSleep);
    uint64 hrtCounter = HighResolutionTimer::Counter();
    while (nsecRemainder < 0) {
        while (nanosleep(&timesValues, &remTimesValues) == -1) {
            if (errno != EINTR) {
                REPORT_ERROR(ErrorManagement::OSError, "Sleep: Failed nanosleep()");
                break;
            }
            REPORT_ERROR(ErrorManagement::warning, "Sleep: nanosleep() interrupted by signal");
            memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
        }

        uint64 reminderCounter = HighResolutionTimer::Counter() - hrtCounter;
        float64 reminderSec = (static_cast<float64>(reminderCounter) * HighResolutionTimer::Period()) - sec;
        float64 reminderNanoSec = reminderSec * 1e9;
        nsecRemainder = static_cast<int32>(reminderNanoSec);

        timesValues.tv_sec = 0;
        timesValues.tv_nsec = nsecRemainder;
    }
}

void NoMore(const float64 sec) {
    uint32 linuxSleepNoMoreMinUsecTime = 5000u;
    uint64 secCounts = static_cast<uint64>(sec) * HighResolutionTimer::Frequency();

    float64 secNoMore = sec;
    secNoMore -= static_cast<float64>(linuxSleepNoMoreMinUsecTime) * 1e-6;
    uint64 start = HighResolutionTimer::Counter();
    if (secNoMore > 0.) {
        struct timespec timesValues;
        struct timespec remTimesValues;
        float64 roundValue = floor(secNoMore);
        float64 nsecSleep = (secNoMore - roundValue) * 1e9;
        timesValues.tv_sec = static_cast<time_t>(roundValue);
        timesValues.tv_nsec = static_cast<oslong>(nsecSleep);
        while (nanosleep(&timesValues, &remTimesValues) == -1) {
            if (errno != EINTR) {
                REPORT_ERROR(ErrorManagement::OSError, "Sleep: Failed nanosleep()");
                break;
            }
            REPORT_ERROR(ErrorManagement::warning, "Sleep: nanosleep() interrupted by signal");
            memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
        }
    }
    uint64 sleepUntil = secCounts + start;
    while (HighResolutionTimer::Counter() < sleepUntil) {
    }

}

void Sec(const float64 sec) {
    if (sec > 0.0) {
        struct timespec timesValues;
        struct timespec remTimesValues;
        float64 roundValue = floor(sec);
        timesValues.tv_sec = static_cast<time_t>(roundValue);
        float64 nsec = (sec - roundValue) * 1e9;
        /*lint -e{970} exception to Rule 3-9-2 in order to guarantee that the type of tv_nsec is always consistent with nanoSeconds in 32 and 64 bit architectures*/
        timesValues.tv_nsec = static_cast<long>(nsec);
        while (nanosleep(&timesValues, &remTimesValues) == -1) {
            if (errno != EINTR) {
                REPORT_ERROR(ErrorManagement::OSError, "Sleep: Failed nanosleep()");
                break;
            }
            REPORT_ERROR(ErrorManagement::warning, "Sleep: nanosleep() interrupted by signal");
            memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
        }
    }
}

void MSec(const int32 msec) {
    if (msec > 0) {
        int32 sec = 0;
        int32 nsecSleep = 0;
        if (msec >= 1000) {
            sec = static_cast<int32>(msec / 1000);
            nsecSleep = (msec - (sec * 1000)) * 1000000;
        }
        else {
            sec = 0;
            nsecSleep = msec * 1000000;
        }
        struct timespec timesValues;
        struct timespec remTimesValues;
        timesValues.tv_sec = static_cast<time_t>(sec);
        timesValues.tv_nsec = static_cast<oslong>(nsecSleep);
        while (nanosleep(&timesValues, &remTimesValues) == -1) {
            if (errno != EINTR) {
                REPORT_ERROR(ErrorManagement::OSError, "Sleep: Failed nanosleep()");
                break;
            }
            REPORT_ERROR(ErrorManagement::warning, "Sleep: nanosleep() interrupted by signal");
            memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
        }
    }
}

void SemiBusy(const float64 totalSleepSec,
              const float64 nonBusySleepSec) {
    uint64 startCounter = HighResolutionTimer::Counter();
    float64 endCounterF = totalSleepSec * static_cast<float64>(HighResolutionTimer::Frequency());
    uint64 sleepUntilCounter = startCounter + static_cast<uint64>(endCounterF);

    if ((nonBusySleepSec < totalSleepSec) && (nonBusySleepSec > 0.0)) {
        struct timespec timesValues;
        struct timespec remTimesValues;
        float64 roundValue = floor(nonBusySleepSec);
        float64 nsecSleep = (nonBusySleepSec - roundValue) * 1E9;
        timesValues.tv_sec = static_cast<time_t>(roundValue);
        timesValues.tv_nsec = static_cast<oslong>(nsecSleep);
        while (nanosleep(&timesValues, &remTimesValues) == -1) {
            if (errno != EINTR) {
                REPORT_ERROR(ErrorManagement::OSError, "Sleep: Failed nanosleep()");
                break;
            }
            REPORT_ERROR(ErrorManagement::warning, "Sleep: nanosleep() interrupted by signal");
            memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
        }
    }
    while (HighResolutionTimer::Counter() < sleepUntilCounter) {
    }
}

int32 GetDateSeconds() {
    return static_cast<int32>(time(static_cast<time_t *>(NULL)));
}

}

}
