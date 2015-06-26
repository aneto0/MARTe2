/**
 * @file HighResolutionTimerTest.cpp
 * @brief Source file for class HighResolutionTimerTest
 * @date 26/06/2015
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

 * @details This source file contains the definition of all the methods for
 * the class HighResolutionTimerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HighResolutionTimerTest.h"

//Sleep.h includes HighResolutionTimer.h
#include "Sleep.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//return true if a and b are equal less than a tolerance
bool Tolerance(double a,
               double b,
               double tolerance) {
    return (a - b < tolerance) && (b - a < tolerance);
}

//return true if frequecy and period are mutual
bool HighResolutionTimerTest::TestFrequency() {
    int64 HRTfrequency = HighResolutionTimer::Frequency();
    double HRTperiod = HighResolutionTimer::Period();
    double relativePeriod = 1.0 / HRTfrequency;
    int64 relativeFrequency = (int64) (1.0 / HRTperiod);
    return (HRTperiod == relativePeriod) && (HRTfrequency == relativeFrequency) && (HighResolutionTimerMSecTics() == (HRTfrequency / 1000.0));
}

//return true if the measured time is more or less equal to the sleep time.
bool HighResolutionTimerTest::TestCounter(double sleepTime) {

    int64 counter_1 = 0;
    int64 counter = 0;
    double time;

    counter_1 = HighResolutionTimer::Counter();
    SleepSec(sleepTime);
    counter = HighResolutionTimer::Counter();
    time = HighResolutionTimer::TicksToTime(counter, counter_1);
    return Tolerance(time, sleepTime, 0.05);

}

bool HighResolutionTimerTest::TestTimeStamp(uint32 millisecs) {

    TimeValues myTimeStamp1;
    TimeValues myTimeStamp2;
    uint32 conversions[] = { 1000, 6e4, 36e5, 36e5 * 24 };

    //impose a tolerance of ten milliseconds
    int32 tolerance = 10;

    HighResolutionTimer::GetTimeStamp(myTimeStamp1);
    SleepMSec(millisecs);
    HighResolutionTimer::GetTimeStamp(myTimeStamp2);

    //it could fail if you are unlucky and launch the test across one month and the other at least!
    uint32 elapsed = (myTimeStamp2.days - myTimeStamp1.days) * conversions[3] + (myTimeStamp2.hours - myTimeStamp1.hours) * conversions[2]
            + (myTimeStamp2.minutes - myTimeStamp1.minutes) * conversions[1] + (myTimeStamp2.seconds - myTimeStamp1.seconds) * conversions[0]
            + (myTimeStamp2.microseconds - myTimeStamp1.microseconds) / conversions[0];
    int32 diff = (int32) (elapsed - millisecs);

    if (diff > tolerance || diff < -tolerance) {
        return False;
    }

    //checks the boundaries
    if (myTimeStamp1.minutes > 59) {

        return False;
    }
    if (myTimeStamp1.hours > 23) {

        return False;
    }

    if (myTimeStamp1.days > 31 || myTimeStamp1.days == 0) {

        return False;
    }
    if (myTimeStamp1.month > 11) {

        return False;
    }

    return True;

}
