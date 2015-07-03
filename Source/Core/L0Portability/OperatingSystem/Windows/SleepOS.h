/**
 * @file SleepOS.h
 * @brief Header file for class SleepOS
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

 * @details This header file contains the declaration of the class SleepOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SLEEPOS_H_
#define 		SLEEPOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <time.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "../../HighResolutionTimer.h"

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
    static inline void AtLeast(float64 sec) {
        int ticks = (int) (winSleepFreq * sec + 0.9999);
        if (ticks < 0)
            return;

        Sleep(ticks);
    }

    /**
     * @see Sleep.h SleepNoMore
     * @brief Sleep maximum sec in the argument.
     * @param[in] sec defines the maximum number of seconds to sleep.
     */
    static inline void NoMore(float64 sec) {
        int ticks = (int) (winSleepFreq * sec);
        if (ticks < 0)
            return;

        Sleep(ticks);
    }

    /**
     * @see Sleep.h SleepSec
     * @brief Sleep sec seconds.
     * @param[in] sec is the number to seconds to sleep.
     */
    static inline void SecDouble(float64 sec) {
        if (sec < 0)
            return;

        Sleep((unsigned long) (sec * 1000.0 + 0.5));
    }

    /**
     * @see Sleep.h SleepSec
     * @brief Sleep sec seconds.
     * @param[in] sec is the number of seconds to sleep.
     */
    static inline void SecFloat(float32 sec) {
        SecDouble(sec);
    }

    /**
     * @see Sleep.h SleepOSSec
     * @brief Sleep for msec milliseconds.
     * @param[in] msec is the number of milliseconds to sleep.
     */
    static inline void MSec(int32 msec) {
        if (msec < 0)
            return;

        Sleep(msec);
    }

    /**
     * @see Sleep.h SleepSemiBusy
     * @brief Sleep totalSleepSec seconds yielding cpu only for nonBusySleepSec seconds.
     * @param[in] totalSleepSec is the number of seconds to sleep.
     * @param[in] nonBusySleepSec is the number of seconds to sleep yielding cpu.
     */
    static inline void SemiBusy(float64 totalSleepSec,
                                float64 nonBusySleepSec) {
        NoMore(totalSleepSec);
    }

    /**
     * @brief Get the number of seconds elapsed from 1 Jan 1970.
     * @return the number of seconds elapsed from 1 Jan 1970.
     */
    static int32 GetDateSeconds() {
        return (int32) time((time_t *) NULL);
    }

private:
    static const uint32 winSleepFreq = 1000;

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SLEEPOS_H_ */

