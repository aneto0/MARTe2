/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
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
 express or implied. 
 * See the Licence for the specific language governing 
 permissions and limitations under the Licence. 
 *
 * $Id: HRT.h 43 2012-02-08 17:07:58Z astephen $
 *
 **/

/**
 * @file
 * @brief Access to the high resolution counters. 
 *
 * These routines enable the calculation of high resolution timings.
 *
 * The time ellapsed can be calculated using:
 * int64 t1 = HRT::HRTCounter();
 * SOME CODE
 * double totalTime = (HRT::HRTCounter() - t1) * HRT::HRTPeriod();
 */
#ifndef _HIGH_RESOLUTION_TIMER_P_H_
#define _HIGH_RESOLUTION_TIMER_P_H_

/** @brief Reads the High Resolution Timer as 32 bit. Fast inline assembler.
  * @return number of cpu ticks in a 32 bit integer. */
static inline uint32 HighResolutionTimerRead32() {
__asm {
    _emit 0x0F
    _emit 0x31
}
}

/** @brief Reads the High Resolution Timer as 64 bit int. Fast inline assembler.
  * @return number of cpu ticks in a 64 bit integer. */
static inline int64 HighResolutionTimerRead64() {
__asm {
_emit 0x0F
_emit 0x31
}
}

#endif

