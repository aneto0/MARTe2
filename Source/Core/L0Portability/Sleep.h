/**
 * @file Sleep.h
 * @brief Header file for class Sleep
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

 * @details This header file contains the declaration of the class Sleep
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SLEEP_H_
#define SLEEP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Sleep functions.
 *
 * @details This functions allows tasks to sleep for a desired time. There are functions which sleeps wasting cpu time using
 * HighResolutionTimer and functions which sleeps yielding cpu using system call functions.
 * These functions are generally used in multithreading applications for example to allows threads to wait on a
 * condition without use cpu.
 *
 * @details Most of the implementation is delegated to SleepOS.h which provides system calls to allows sleep functions.
 */
class Sleep {

public:

    /**
     * @brief Retrieve the time as seconds from the start of time.
     * @return the seconds elapsed from 00:00 of Jan 1, 1970.
     */
    static int32 GetDateSeconds(void);

    /**
     * @brief Sleeps for the time requested or more.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the time in seconds to sleep (at least).
     */
    static void AtLeast(float64 sec);

    /**
     * @brief Sleeps no more than the requested time.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the time in seconds to sleep (no more).
     */
    static void NoMore(float64 sec);

    /**
     * @brief Sleeps for sec seconds (float64 value).
     * @param[in] sec is the time to sleep.
     */
    static void Sec(float64 sec);

    /**
     * @brief Sleeps for msec milliseconds.
     * @param[in] msec is the number of milliseconds to sleep.
     */
    static void MSec(int32 msec);

    /**
     * @brief Sleep without yield cpu.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the seconds to sleep.
     */
    inline static void Busy(float64 sec);

    /**
     * @brief Sleep yielding cpu for nonBusySleepSec.
     * @details This function uses HighResolutionTimer functions.
     * @param[in] totalSleepSec is the total time in seconds to sleep.
     * @param[in] nonBusySleepSec is the time to sleep without use cpu.
     */
    static void SemiBusy(float64 totalSleepSec,
                         float64 nonBusySleepSec);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void Sleep::Busy(float64 sec) {
    int64 startCounter = HighResolutionTimer::Counter();
    int64 endCounter = static_cast<int64>(sec) * HighResolutionTimer::Frequency();
    int64 sleepUntilCounter = startCounter + endCounter;
    while (HighResolutionTimer::Counter() < sleepUntilCounter) {
    }
}

#endif /* SLEEP_H_ */
