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
#include "../../GeneralDefinitions.h"
#include "cmsis_os.h"


/**
 * @see Sleep.h SleepAtLeast
 * @brief Sleep more than sec in argument.
 * @param sec defines the minimum number of seconds to sleep.
 */
static inline void SleepOSAtLeast(double sec) {
	uint32_t time2sleep=((uint32_t) sec)*1000;
	osDelay(time2sleep); 
}

/** 
 * @see Sleep.h SleepNoMore
 * @brief Sleep maximum sec in the argument.
 * @param sec defines the maximum number of seconds to sleep.
 */
static inline void SleepOSNoMore(double sec) {
	uint32_t time2sleep=((uint32_t)sec)*1000;
    osDelay(time2sleep);
}

/** 
 * @see Sleep.h SleepSec
 * @brief Sleep sec seconds.
 * @param sec is the number to seconds to sleep.
 */
static inline void SleepOSSecDouble(double sec) {
	uint32_t time2sleep = ((uint32_t) sec)*1000;
	osDelay(time2sleep);
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
    osDelay((uint32_t)msec);
}

/** 
 * @see Sleep.h SleepSemiBusy
 * @brief Sleep totalSleepSec seconds yielding cpu only for nonBusySleepSec seconds.
 * @param totalSleepSec is the number of seconds to sleep.
 * @param nonBusySleepSec is the number of seconds to sleep yielding cpu.
 */
static inline void SleepOSSemiBusy(double totalSleepSec,
                                   double nonBusySleepSec) {
    
	uint32_t time2sleep=((uint32_t)(totalSleepSec+nonBusySleepSec))*1000;
	osDelay(time2sleep);
}

/** @brief Get the number of seconds elapsed from 1 Jan 1970.
  * @return the number of seconds elapsed from 1 Jan 1970. */
static int32 SleepOSGetDateSeconds() {
    return 0;
}
#endif

