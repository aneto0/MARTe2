/**
 * @file HighResolutionTimerCalibratorOS.h
 * @brief Header file for class HighResolutionTimerCalibratorOS
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

 * @details This header file contains the declaration of the class HighResolutionTimerCalibratorOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMERCALIBRATOROS_H_
#define HIGHRESOLUTIONTIMERCALIBRATOROS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#endif

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to get the period and the frequency of the cpu clock depending on the operating system.
 */

class HighResolutionTimerCalibratorOS {

public:
    /** Number of cpu ticks in a millisecond */
    uint32 HRTmSecTics;

    /** Number of cpu ticks in a second */
    int64 HRTFrequency;

    /** Time between a tick and the other in seconds */
    float64 HRTPeriod;

    /** Contains the seconds and microseconds from epoch */
    struct timeval initialTime;

    /** Used to save the number of ticks at the calibration moment. */
    uint64 initialTicks;

    /** @brief Calculate the period and the frequency of the cpu clock. */
    HighResolutionTimerCalibratorOS() {

        gettimeofday(&initialTime, NULL);
        initialTicks = HighResolutionTimerA::Read64();

        HRTFrequency = 0;
        HRTPeriod = 0;

        char8 buffer[LINUX_CPUINFO_BUFFER_SIZE + 1];

        FILE *f;
        f = fopen("/proc/cpuinfo", "r");
        uint32 size = LINUX_CPUINFO_BUFFER_SIZE;
        size = fread(buffer, size, 1, f);
        fclose(f);

        const char8 *pattern = "MHz";
        char8 *p = strstr(buffer, pattern);
        if (p != NULL) {
            p = strstr(p, ":");
            p++;
            float64 f = atof(p);
            if (f != 0) {
                f *= 1.0e6;
                HRTFrequency = (int64) f;
                HRTPeriod = 1.0 / f;
            }
        }
        HRTmSecTics = HRTFrequency / 1000;
    }

    /**
     * @brief Get the current time stamp.
     * @param[in] timeStamp is a structure which contains the time stamp fields.
     * @return true if successful, false otherwise.
     */
    bool GetTimeStamp(TimeValues &timeStamp) {

        uint64 ticks = HighResolutionTimerA::Read64() - initialTicks;

        //Use HRT
        uint32 secHRT = (uint32) (ticks * HRTPeriod);
        uint32 nSecHRT = (uint32) ((ticks * HRTPeriod - secHRT) * 1e6);

        //Add HRT to the the initial time saved in the calibration.
        time_t sec = (time_t)(initialTime.tv_sec + secHRT);
        timeStamp.microseconds = initialTime.tv_usec + nSecHRT;

        //Check the overflow
        if (timeStamp.microseconds >= 1e6) {
            timeStamp.microseconds -= 1e6;
            sec++;
        }

        //fill the time structure
        struct tm *tValues;
        tValues = localtime((const time_t*) &sec);
        if (tValues == NULL) {
            return false;
        }
        timeStamp.seconds = tValues->tm_sec;
        timeStamp.minutes = tValues->tm_min;
        timeStamp.hours = tValues->tm_hour;
        timeStamp.days = tValues->tm_mday;
        timeStamp.month = tValues->tm_mon;
        timeStamp.year = tValues->tm_year;
        return true;
    }

private:

    /** The size of the buffer to retrieve the cpu informations by the system. */
    static const uint32 LINUX_CPUINFO_BUFFER_SIZE = 1023;
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HIGHRESOLUTIONTIMERCALIBRATOROS_H_ */

