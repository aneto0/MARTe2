/**
 * @file HighResolutionTimerCalibratorOS.h
 * @brief Header file for class HighResolutionTimerCalibratorOS
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

 * @details This header file contains the declaration of the class HighResolutionTimerCalibratorOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef HIGHRESOLUTIONTIMERCALIBRATOROS_H_
#define 		HIGHRESOLUTIONTIMERCALIBRATOROS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <Windows.h>
#include <time.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to get the period and the frequency of the clock.
 */
class HighResolutionTimerCalibratorOS {

public:
    /** Number of cpu ticks in a milliseconds. */
    uint32 HRTmSecTics;

    /** Number of cpu ticks in a second. */
    uint64 HRTFrequency;

    /** Time between a tick and the other in seconds. */
    double HRTPeriod;

    /** Contains the seconds and microseconds from epoch */
    struct timeval initialTime;

    /** Used to save the number of ticks at the calibration moment. */
    uint64 initialTicks;

    /**
     * @brief Get the frequency and the period of the cpu clock.
     */
    HighResolutionTimerCalibratorOS() {

        time((time_t *) &initialTime.tv_sec);

        //The precision is at the millisecond!
        SYSTEMTIME forMs;
        GetSystemTime(&forMs);
        initialTime.tv_usec = forMs.wMilliseconds * 1000;

        initialTicks = HighResolutionTimerA::Read64();

        uint64 tt0, tt1, tt2, tt3, tt4, tt5, dTa, dTb;
        dTa = 0;
        dTb = 0;
        for (int i = 0; i < 50; i++) {
            tt2 = HighResolutionTimerA::Read64();
            QueryPerformanceCounter((LARGE_INTEGER *) &tt0);
            tt3 = HighResolutionTimerA::Read64();
            tt4 = tt3;
            while ((tt4 - tt3) < 100000)
                tt4 = HighResolutionTimerA::Read64(); // .5 ms at 200 Mhz
            QueryPerformanceCounter((LARGE_INTEGER *) &tt1);
            tt5 = HighResolutionTimerA::Read64();
            dTa += (tt1 - tt0);
            dTb += ((tt5 + tt4) - (tt3 + tt2)) / 2;
        }
        QueryPerformanceFrequency((LARGE_INTEGER *) &HRTFrequency);
        HRTFrequency *= dTb;
        HRTFrequency /= dTa;

        HRTFrequency += 999999;
        HRTFrequency /= 2000000;
        HRTFrequency *= 2000000;

        HRTPeriod = 1.0 / (int64) HRTFrequency;
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
        uint32 uSecHRT = (uint32) ((ticks * HRTPeriod - secHRT) * 1e6);

        //Add HRT to the the initial time saved in the calibration.
        time_t sec = (time_t)(initialTime.tv_sec + secHRT);
        timeStamp.microseconds = initialTime.tv_usec + uSecHRT;

        //Check the overflow
        if (timeStamp.microseconds >= 1e6) {
            timeStamp.microseconds -= 1e6;
            sec++;
        }

        //fill the time structure
        struct tm tValues;
        if (localtime_s(&tValues, (const time_t*) &sec) != 0) {
            return False;
        }
        timeStamp.seconds = tValues.tm_sec;
        timeStamp.minutes = tValues.tm_min;
        timeStamp.hours = tValues.tm_hour;
        timeStamp.days = tValues.tm_mday;
        timeStamp.month = tValues.tm_mon;
        timeStamp.year = tValues.tm_year;
        return True;
    }

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HIGHRESOLUTIONTIMERCALIBRATOROS_H_ */

