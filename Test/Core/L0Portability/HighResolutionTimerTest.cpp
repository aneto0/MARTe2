/**
 * @file HighResolutionTimerTest.cpp
 * @brief Source file for class HighResolutionTimerTest
 * @date 26/06/2015
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
static bool Tolerance(float64 a,
                      float64 b,
                      float64 tolerance) {
    float64 diff = a - b;
    return (diff < tolerance) && (diff > -tolerance);
}

bool HighResolutionTimerTest::TestFrequency() {

    return HighResolutionTimer::Frequency() >= 0;

}

bool HighResolutionTimerTest::TestPeriod() {
    return HighResolutionTimer::Period() > 0;
}

bool HighResolutionTimerTest::TestPeriodFrequency() {
    int64 HRTfrequency = HighResolutionTimer::Frequency();
    float64 HRTperiod = HighResolutionTimer::Period();
    float64 relativePeriod = 1.0 / HRTfrequency;
    int64 relativeFrequency = (int64) (1.0 / HRTperiod);
    return (HRTperiod == relativePeriod) && (HRTfrequency == relativeFrequency);
}

//return true if the measured time is more or less equal to the sleep time.
bool HighResolutionTimerTest::TestCounter(float64 sleepTime) {

    int64 counter_1 = 0;
    int64 counter = 0;
    float64 time;

    counter_1 = HighResolutionTimer::Counter();
    Sleep::Sec(sleepTime);
    counter = HighResolutionTimer::Counter();
    time = HighResolutionTimer::TicksToTime(counter, counter_1);
    return Tolerance(time, sleepTime, sleepTime * .1);

}

//return true if the measured time is more or less equal to the sleep time.
bool HighResolutionTimerTest::TestCounter32(float64 sleepTime) {

    int32 counter_1 = 0;
    int32 counter = 0;
    float64 time;

    counter_1 = HighResolutionTimer::Counter32();
    Sleep::Sec(sleepTime);
    counter = HighResolutionTimer::Counter32();
    time = (counter-counter_1)*HighResolutionTimer::Period();
    return Tolerance(time, sleepTime, sleepTime * .1);

}

bool HighResolutionTimerTest::TestTicksToTime(float64 sleepTime) {

    int64 ticks = sleepTime*HighResolutionTimer::Frequency();

    return Tolerance(HighResolutionTimer::TicksToTime(ticks, 0), sleepTime, 0.05);
}

bool HighResolutionTimerTest::TestTimeStamp(uint32 millisecs) {

    TimeValues myTimeStamp1;
    TimeValues myTimeStamp2;
    uint32 conversions[] = { 1000, 60000, 3600000, 3600000 * 24 };

    //impose a tolerance of 50 milliseconds
    int32 tolerance = 50;

    HighResolutionTimer::GetTimeStamp(myTimeStamp1);
    Sleep::MSec(millisecs);
    HighResolutionTimer::GetTimeStamp(myTimeStamp2);

    //it could fail if you are unlucky and launch the test across one month and the other at least!
    uint32 elapsed = (myTimeStamp2.days - myTimeStamp1.days) * conversions[3] + (myTimeStamp2.hours - myTimeStamp1.hours) * conversions[2]
            + (myTimeStamp2.minutes - myTimeStamp1.minutes) * conversions[1] + (myTimeStamp2.seconds - myTimeStamp1.seconds) * conversions[0]
            + (myTimeStamp2.microseconds - myTimeStamp1.microseconds) / conversions[0];
    int32 diff = (int32) (elapsed - millisecs);

    if (diff > tolerance || diff < -tolerance) {
        return false;
    }

    //checks the boundaries
    if (myTimeStamp1.minutes > 59) {

        return false;
    }
    if (myTimeStamp1.hours > 23) {

        return false;
    }

    if (myTimeStamp1.days > 31 || myTimeStamp1.days == 0) {

        return false;
    }
    if (myTimeStamp1.month > 11) {

        return false;
    }

    return true;

}
