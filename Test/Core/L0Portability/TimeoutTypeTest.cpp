/**
 * @file TimeoutTypeTest.cpp
 * @brief Source file for class TimeoutTypeTest
 * @date Jul 23, 2015
 * @author llcapella
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
 * the class TimeoutTypeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TimeoutTypeTest.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TimeoutTypeTest::TimeoutTypeTest() {
    retValue = true;
}

TimeoutTypeTest::~TimeoutTypeTest() {
    // Auto-generated destructor stub for TimeoutTypeTest
    // TODO Verify if manual additions are needed
}

bool TimeoutTypeTest::TestConstructorDefault() {
    TimeoutType time;
    return (TTInfiniteWait == time.GetTimeoutMSec());
}

bool TimeoutTypeTest::TestConstructorNotDefault(uint32 msec) {
    TimeoutType time(msec);
    return (msec == time.GetTimeoutMSec());
}

bool TimeoutTypeTest::TestSetTimeoutSec(double sec) {
    TimeoutType time;
    time.SetTimeoutSec(sec);
    return (sec * 1000 == time.GetTimeoutMSec());
}

bool TimeoutTypeTest::TestSetTimeoutHighResolutionTimerTicks() {
    TimeoutType timeout;
    int64 ticks = 100000000;
    uint32 expectedTimeout;
    timeout.SetTimeoutHighResolutionTimerTicks(ticks);
    expectedTimeout = ticks * HighResolutionTimer::Period()*1000;
    retValue = (expectedTimeout == timeout.GetTimeoutMSec());
    ticks = -100;
    timeout.SetTimeoutHighResolutionTimerTicks(ticks);
    retValue &= (timeout.GetTimeoutMSec() == 0);
    return retValue;
}

bool TimeoutTypeTest::TestHighResolutionTimerTicks() {
    TimeoutType timeout;
    int32 msecTimeout = 100;
    int64 expectedTicks;
    timeout.SetTimeoutSec(double(msecTimeout)*1e-3);
    expectedTicks = int64(1e-3*msecTimeout*HighResolutionTimer::Frequency());
    return (expectedTicks == timeout.HighResolutionTimerTicks());
}

bool TimeoutTypeTest::TestAutoSubstractOPerator() {
    int32 intialTimeout = 100;
    TimeoutType timeout(intialTimeout);
    int32 substractValue = 60;
    int32 expectedTime = intialTimeout-substractValue;
    timeout -= substractValue;
    retValue = uint32(expectedTime) == timeout.GetTimeoutMSec();
    timeout -= substractValue;
    retValue &= (0 == timeout.GetTimeoutMSec());
    return retValue;
}

bool TimeoutTypeTest::TestEqualComparison() {
    TimeoutType timeout1(100);
    TimeoutType timeout2(100);
    TimeoutType timeout3(10);
    retValue = (timeout1 == timeout2);
    retValue &= !(timeout1 == timeout3);
    return retValue;
}

bool TimeoutTypeTest::TestDiffComparison() {
    TimeoutType timeout1(100);
    TimeoutType timeout2(100);
    TimeoutType timeout3(10);
    retValue = !(timeout1 != timeout2);
    retValue &= timeout1 != timeout3;
    return retValue;
}

bool TimeoutTypeTest::TestAssingOperator() {
    TimeoutType timeout1(100);
    TimeoutType timeout2;
    timeout2 = timeout1;
    return (timeout1.GetTimeoutMSec() == timeout2.GetTimeoutMSec());
}

bool TimeoutTypeTest::TestIsFinite() {
    TimeoutType timeout1;
    TimeoutType timeout2(100);
    retValue = !timeout1.IsFinite();
    retValue &= timeout2.IsFinite();
    return retValue;
}

bool TimeoutTypeTest::TestGetTimeoutMSec() {
    TimeoutType timeout;
    return (TTInfiniteWait == timeout.GetTimeoutMSec());
}
