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
 * @brief Implementation of task sleeping in Windows
 */
#ifndef SLEEP_OS_H
#define SLEEP_OS_H

#include "../../HighResolutionTimer.h"
#include "time.h"

#ifndef WIN_SLEEP_FREQ
#define WIN_SLEEP_FREQ 1000
#endif

/**
 * @see Sleep.h SleepAtLeast
 */
static inline void SleepOSAtLeast(double sec) {
    int ticks = (int) (WIN_SLEEP_FREQ * sec + 0.9999);
    if (ticks < 0)
        return;

    Sleep(ticks);
}

/** 
 * @see Sleep.h SleepNoMore
 */
static inline void SleepOSNoMore(double sec) {
    int ticks = (int) (WIN_SLEEP_FREQ * sec);
    if (ticks < 0)
        return;

    Sleep(ticks);
}

/** 
 * @see Sleep.h SleepSec
 */
static inline void SleepOSSecDouble(double sec) {
    if (sec < 0)
        return;

    Sleep((unsigned long) (sec * 1000.0 + 0.5));
}

/** 
 * @see Sleep.h SleepSec
 */
static inline void SleepOSSecFloat(float sec) {
    SleepOSSecDouble(sec);
}

/** 
 * @see Sleep.h SleepOSSec
 */
static inline void SleepOSMSec(int32 msec) {
    if (msec < 0)
        return;

    Sleep(msec);
}

/** 
 * @see Sleep.h SleepSemiBusy
 */
static inline void SleepOSSemiBusy(double totalSleepSec,
                                   double nonBusySleepSec) {
    SleepOSNoMore(totalSleepSec);
}

static int32 SleepOSGetDateSeconds() {
    return (int32) time((time_t *) NULL);
}
#endif

