/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:$
 *
 **/

#include "GeneralDefinitions.h"
#include "HighResolutionTimerTest.h"

//return true if a and b are equal less than a tolerance
bool Tolerance(double a, double b, double tolerance) {
    return (a - b < tolerance) && (b - a < tolerance);
}

//return true if frequecy and period are mutual
bool HighResolutionTimerTest::TestFrequency() {
    int64 HRTfrequency = HighResolutionTimer::Frequency();
    double HRTperiod = HighResolutionTimer::Period();
    double relativePeriod = 1.0 / HRTfrequency;
    int64 relativeFrequency = (int64) (1.0 / HRTperiod);
    return (HRTperiod == relativePeriod) && (HRTfrequency == relativeFrequency)
            && (HighResolutionTimerMSecTics() == (HRTfrequency / 1000.0));
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
