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
 * @brief Implentation of task sleeping. 
 */
#ifndef SLEEP_H
#define SLEEP_H

/**
 * @brief Sleep functions.
 * 
 * This functions allows tasks to sleep for a desired time. There are functions which sleeps wasting cpu time using 
 * HighResolutionTimer and functions which sleeps yielding cpu using system call functions.
 * 
 * Most of the implementation is delegated to SleepOS.h which provides system calls to allows sleep functions.
 *
 * These functions are generally used in multithreading applications for example to allows threads to wait on a
 * condition without use cpu. 
 */

#include "GeneralDefinitions.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,SleepOS.h)
extern "C" {
    /** @brief Retrieve the time as seconds from the start of time.
      * @return the seconds elapsed from 00:00 of Jan 1, 1970. */
    int GetDateSeconds();
};

/** @brief Sleeps for the time requested or more.
  *
  * This function uses HighResolutionTimer functions.
  * @param sec is the time in seconds to sleep (at least). */
static inline void SleepAtLeast(double sec) {
    SleepOSAtLeast(sec);
}

/** @brief Sleeps no more than the time requested.
  *
  * This function uses HighResolutionTimer functions.
  * @param sec is the time in seconds to sleep (no more). */
static inline void SleepNoMore(double sec) {
    SleepOSNoMore(sec);
}

/** @brief Sleeps for sec seconds (double value).
  * @param sec is the time to sleep. */
static inline void SleepSec(double sec) {
    SleepOSSecDouble(sec);
}

/** @brief Sleeps for sec seconds (float value).
  * @param sec is the time to sleep. */
static inline void SleepSec(float sec) {
    SleepOSSecFloat(sec);
}

/** @brief Sleeps for msec milliseconds.
  * @param msec is the number of milliseconds to sleep. */
static inline void SleepMSec(int32 msec) {
    SleepOSMSec(msec);
}

/** @brief Sleep without yield cpu.
  *
  * This function uses HighResolutionTimer functions.
  * @param sec is the seconds to sleep. */
static inline void SleepBusy(double sec) {
    int64 startCounter = HighResolutionTimer::Counter();
    int64 sleepUntil = startCounter
            + (int64) (sec * HighResolutionTimer::Frequency());
    while (HighResolutionTimer::Counter() < sleepUntil)
        ;
}

/** @brief Sleep yielding cpu for nonBusySleepSec.
  * @param totalSleepSec is the total time in seconds to sleep.
  * @param nonBusySleepSec is the time to sleep without use cpu. */
static inline void SleepSemiBusy(double totalSleepSec, double nonBusySleepSec) {
    SleepOSSemiBusy(totalSleepSec, nonBusySleepSec);
}
#endif

