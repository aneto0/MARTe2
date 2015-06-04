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
/**
 * @file
 * @brief Access to processor's information
 */
#ifndef HIGH_RESOLUTION_TIMER_CALIBRATOR_OS_H
#define HIGH_RESOLUTION_TIMER_CALIBRATOR_OS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINUX_CPUINFO_BUFFER_SIZE 1023

/** @brief A class to get the period and the frequency of the cpu clock. */

class HighResolutionTimerCalibratorOS {

public:
    /** Number of cpu ticks in a millisecond */
    uint32 HRTmSecTics;

    /** Number of cpu ticks in a second */
    uint64 HRTFrequency;
 
    /** Time between a tick and the other in seconds */
    double HRTPeriod;

    /** @brief Calculate the period and the frequency of the cpu clock. */
    HighResolutionTimerCalibratorOS() {
        HRTFrequency = 0;
        HRTPeriod = 0;

        char buffer[LINUX_CPUINFO_BUFFER_SIZE + 1];

        FILE *f;
        f = fopen("/proc/cpuinfo", "r");
        uint32 size = LINUX_CPUINFO_BUFFER_SIZE;
        size = fread(buffer, size, 1, f);
        fclose(f);

        const char *pattern = "MHz";
        char *p = strstr(buffer, pattern);
        if (p != NULL) {
            p = strstr(p, ":");
            p++;
            double f = atof(p);
            if (f != 0) {
                f *= 1.0e6;
                HRTFrequency = (int64) f;
                HRTPeriod = 1.0 / f;
            }
        }
        HRTmSecTics = HRTFrequency / 1000;
    }
};

#endif

