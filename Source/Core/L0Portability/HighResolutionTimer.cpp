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
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/

#include "HighResolutionTimer.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,HighResolutionTimerCalibratorOS.h)

static HighResolutionTimerCalibratorOS highResolutionTimerCalibratorOS;

/** the frequency of the HRT Clock. */
int64 HighResolutionTimerFrequency() {
    return highResolutionTimerCalibratorOS.HRTFrequency;
}

/** the HRT Clock period in seconds */
double HighResolutionTimerPeriod() {
    return highResolutionTimerCalibratorOS.HRTPeriod;
}

/** how many ticks in a msec for the HRT */
uint32 HighResolutionTimerMSecTics() {
    return highResolutionTimerCalibratorOS.HRTmSecTics;
}

