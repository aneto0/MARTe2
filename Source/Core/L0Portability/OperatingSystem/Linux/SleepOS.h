/**
 * @file SleepOS.h
 * @brief Header file for class SleepOS
 * @date 10/06/2015
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
 *
 * @details This header file contains the declaration of the class SleepOS
 * (all of its public, protected and private members). It may also include
 * definitions for inline and friend methods which need to be visible to
 * the compiler.
 */

#ifndef SLEEPOS_H_
#define 		SLEEPOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <string.h>
#include <math.h>
#include <errno.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "../../HighResolutionTimer.h"

#ifndef LINUX_SLEEP_NO_MORE_MIN_USEC_TIME
#define LINUX_SLEEP_NO_MORE_MIN_USEC_TIME 5000
#endif
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Platform dependent implementation of sleep functions.
 *
 * @details These functions allows tasks to sleep for a desired time. There are functions which sleeps wasting cpu time using
 * HighResolutionTimer and functions which sleeps yielding cpu using system call functions.
 * These functions are generally used in multi-threading applications for example to allow threads to wait on a
 * condition without use cpu.
 */
class SleepOS {

public:

    /**
     * @see Sleep.h SleepAtLeast
     * @brief Sleep more than sec in argument.
     * @param[in] sec defines the minimum number of seconds to sleep.
     */
    static inline void AtLeast(double sec) {
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

            nsecRemainder = (int32) (((HighResolutionTimer::Counter() - hrtCounter) * HighResolutionTimer::Period() - sec) * 1E9);
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
     * @param[in] sec defines the maximum number of seconds to sleep.
     */
    static inline void NoMore(double sec) {
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
        while (HighResolutionTimer::Counter() < sleepUntil) {
        }

    }

    /**
     * @see Sleep.h SleepSec
     * @brief Sleep sec seconds.
     * @param[in] sec is the number to seconds to sleep.
     */
    static inline void SecDouble(double sec) {
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
     * @param[in] sec is the number of seconds to sleep.
     */
    static inline void SecFloat(float sec) {
        SecDouble(sec);
    }

    /**
     * @see Sleep.h SleepOSSec
     * @brief Sleep for msec milliseconds.
     * @param[in] msec is the number of milliseconds to sleep.
     */
    static inline void MSec(int32 msec) {
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
     * @param[in] totalSleepSec is the number of seconds to sleep.
     * @param[in] nonBusySleepSec is the number of seconds to sleep yielding cpu.
     */
    static inline void SemiBusy(double totalSleepSec,
                                double nonBusySleepSec) {
        int64 startCounter = HighResolutionTimer::Counter();
        int64 sleepUntilCounter = startCounter + (int64) (totalSleepSec * HighResolutionTimer::Frequency());
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
        while (HighResolutionTimer::Counter() < sleepUntilCounter) {
        }
    }

    /**
     * @brief Get the number of seconds elapsed from 1 Jan 1970.
     * @return the number of seconds elapsed from 1 Jan 1970.
     */
    static int32 GetDateSeconds() {
        return (int32) time((time_t *) NULL);
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Friend method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SLEEPOS_H_ */

