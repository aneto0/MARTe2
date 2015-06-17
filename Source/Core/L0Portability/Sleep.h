/**
 * @file Sleep.h
 * @brief Header file for class Sleep
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

 * @details This header file contains the declaration of the class Sleep
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SLEEP_H_
#define 		SLEEP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,SleepOS.h)

extern "C" {
    /**
     * @brief Retrieve the time as seconds from the start of time.
     * @return the seconds elapsed from 00:00 of Jan 1, 1970.
     */
    int GetDateSeconds();

    /**
     * @brief Sleeps for the time requested or more.
     *
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the time in seconds to sleep (at least).
     */
    void SleepAtLeast(double sec);
    /**
     * @brief Sleeps no more than the requested time.
     *
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the time in seconds to sleep (no more).
     */
    void SleepNoMore(double sec);

    /**
     * @brief Sleeps for sec seconds (double value).
     * @param[in] sec is the time to sleep.
     */
    void SleepSecDouble(double sec);

    /**
     * @brief Sleeps for sec seconds (float value).
     * @param[in] sec is the time to sleep.
     */
    void SleepSec(float sec);

    /**
     * @brief Sleeps for msec milliseconds.
     * @param[in] msec is the number of milliseconds to sleep.
     */
    void SleepMSec(int32 msec);

    /** @brief Sleep without yield cpu.
     *
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the seconds to sleep.
     */
    void SleepBusy(double sec);

    /**
     * @brief Sleep yielding cpu for nonBusySleepSec.
     * @param[in] totalSleepSec is the total time in seconds to sleep.
     * @param[in] nonBusySleepSec is the time to sleep without use cpu.
     */
    void SleepSemiBusy(double totalSleepSec,
            double nonBusySleepSec);

};
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
    static int GetDateSeconds();

    /**
     * @brief Sleeps for the time requested or more.
     *
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the time in seconds to sleep (at least).
     */
    static void AtLeast(double sec);

    /**
     * @brief Sleeps no more than the requested time.
     *
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the time in seconds to sleep (no more).
     */
    static void NoMore(double sec);

    /**
     * @brief Sleeps for sec seconds (double value).
     * @param[in] sec is the time to sleep.
     */
    static void Sec(double sec);

    /**
     * @brief Sleeps for sec seconds (float value).
     * @param[in] sec is the time to sleep.
     */
    static void Sec(float sec);
    /**
     * @brief Sleeps for msec milliseconds.
     * @param[in] msec is the number of milliseconds to sleep.
     */
    static void MSec(int32 msec);

    /** @brief Sleep without yield cpu.
     *
     * @details This function uses HighResolutionTimer functions.
     * @param[in] sec is the seconds to sleep.
     */
    static void Busy(double sec);

    /**
     * @brief Sleep yielding cpu for nonBusySleepSec.
     * @param[in] totalSleepSec is the total time in seconds to sleep.
     * @param[in] nonBusySleepSec is the time to sleep without use cpu.
     */
    static void SemiBusy(double totalSleepSec,
                         double nonBusySleepSec);
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SLEEP_H_ */

