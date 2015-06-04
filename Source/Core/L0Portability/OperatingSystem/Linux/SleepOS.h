/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief Implentation of task sleeping in Linux
 */
#ifndef SLEEP_OS_H
#define SLEEP_OS_H

#include "../../HighResolutionTimer.h"
#include <string.h>
#include <math.h>
#include <errno.h>

#ifndef LINUX_SLEEP_NO_MORE_MIN_USEC_TIME
#define LINUX_SLEEP_NO_MORE_MIN_USEC_TIME 5000
#endif

/**
 * @see Sleep.h SleepAtLeast
 * @brief Sleep more than sec in argument.
 * @param sec defines the minimum number of seconds to sleep.
 */
static inline void SleepOSAtLeast(double sec) {
    int64 hrtCounter;
    int32 nsecRemainder;
    struct timespec timesValues;
    struct timespec remTimesValues;
    double roundValue = floor(sec);
    timesValues.tv_sec = (time_t) roundValue;
    timesValues.tv_nsec = (long) ((sec - roundValue) * 1E9);
    hrtCounter = HighResolutionTimer::Counter();
    while (1) {
        while (nanosleep(&timesValues, &remTimesValues) == -1) {
            if (errno != EINTR) {
                return;
            }
            memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
        }

        nsecRemainder = (int32) (((HighResolutionTimer::Counter() - hrtCounter)
                * HighResolutionTimer::Period() - sec) * 1E9);
        if (nsecRemainder >= 0) {
            break;
        }
        else {
            timesValues.tv_sec = 0;
            timesValues.tv_nsec = nsecRemainder;
        }
    }
}

/** 
 * @see Sleep.h SleepNoMore
 * @brief Sleep maximum sec in the argument.
 * @param sec defines the maximum number of seconds to sleep.
 */
static inline void SleepOSNoMore(double sec) {
    int64 secCounts = (int64) (sec * HighResolutionTimer::Frequency());
    sec -= LINUX_SLEEP_NO_MORE_MIN_USEC_TIME * 1e-6;
    int64 start = HighResolutionTimer::Counter();
    if (sec > 0) {
        struct timespec timesValues;
        struct timespec remTimesValues;
        double roundValue = floor(sec);
        timesValues.tv_sec = (time_t) roundValue;
        timesValues.tv_nsec = (long) ((sec - roundValue) * 1E9);
        while (nanosleep(&timesValues, &remTimesValues) == -1) {
            if (errno != EINTR) {
                return;
            }
            memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
        }
    }
    int64 sleepUntil = secCounts + start;
    while (HighResolutionTimer::Counter() < sleepUntil)
        ;
}

/** 
 * @see Sleep.h SleepSec
 * @brief Sleep sec seconds.
 * @param sec is the number to seconds to sleep.
 */
static inline void SleepOSSecDouble(double sec) {
    struct timespec timesValues;
    struct timespec remTimesValues;
    double roundValue = floor(sec);
    timesValues.tv_sec = (time_t) roundValue;
    timesValues.tv_nsec = (long) ((sec - roundValue) * 1E9);
    while (nanosleep(&timesValues, &remTimesValues) == -1) {
        if (errno != EINTR) {
            return;
        }
        memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
    }
}

/** 
 * @see Sleep.h SleepSec
 * @brief Sleep sec seconds.
 * @param sec is the number of seconds to sleep.
 */
static inline void SleepOSSecFloat(float sec) {
    SleepOSSecDouble(sec);
}

/** 
 * @see Sleep.h SleepOSSec
 * @brief Sleep for msec milliseconds.
 * @param msec is the number of milliseconds to sleep.
 */
static inline void SleepOSMSec(int32 msec) {
    int32 sec = 0;
    int32 nanosec = 0;
    if (msec >= 1000) {
        sec = (int32) (msec / 1000);
        nanosec = (int32) ((msec - sec * 1000) * 1E6);
    }
    else {
        sec = 0;
        nanosec = (int32) (msec * 1E6);
    }
    struct timespec timesValues;
    struct timespec remTimesValues;
    timesValues.tv_sec = (time_t) sec;
    timesValues.tv_nsec = (long) nanosec;
    while (nanosleep(&timesValues, &remTimesValues) == -1) {
        if (errno != EINTR) {
            return;
        }
        memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
    }
}

/** 
 * @see Sleep.h SleepSemiBusy
 * @brief Sleep totalSleepSec seconds yielding cpu only for nonBusySleepSec seconds.
 * @param totalSleepSec is the number of seconds to sleep.
 * @param nonBusySleepSec is the number of seconds to sleep yielding cpu.
 */
static inline void SleepOSSemiBusy(double totalSleepSec,
                                   double nonBusySleepSec) {
    int64 startCounter = HighResolutionTimer::Counter();
    int64 sleepUntilCounter = startCounter
            + (int64) (totalSleepSec * HighResolutionTimer::Frequency());
    if ((nonBusySleepSec < totalSleepSec) && (nonBusySleepSec > 0.0)) {
        struct timespec timesValues;
        struct timespec remTimesValues;
        double roundValue = floor(nonBusySleepSec);
        timesValues.tv_sec = (time_t) roundValue;
        timesValues.tv_nsec = (long) ((nonBusySleepSec - roundValue) * 1E9);
        while (nanosleep(&timesValues, &remTimesValues) == -1) {
            if (errno != EINTR) {
                return;
            }
            memcpy(&timesValues, &remTimesValues, sizeof(struct timespec));
        }
    }
    while (HighResolutionTimer::Counter() < sleepUntilCounter)
        ;
}

/** @brief Get the number of seconds elapsed from 1 Jan 1970.
  * @return the number of seconds elapsed from 1 Jan 1970. */
static int32 SleepOSGetDateSeconds() {
    return (int32) time((time_t *) NULL);
}
#endif

