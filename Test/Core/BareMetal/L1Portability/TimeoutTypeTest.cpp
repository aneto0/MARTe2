/**
 * @file TimeoutTypeTest.cpp
 * @brief Source file for class TimeoutTypeTest
 * @date 23/07/2015
 * @author Llorenç Capellà
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

using namespace MARTe;

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
}

bool TimeoutTypeTest::TestConstructorDefault() {
    TimeoutType time;
    bool ok = TTInfiniteWait == time.GetTimeoutMSec();
    if (ok) {
        ok = TTInfiniteWait == time.GetTimeoutUSec();
    }
    return ok;
}

bool TimeoutTypeTest::TestConstructorNotDefault(uint32 msec) {
    TimeoutType time(msec);
    return (msec == time.GetTimeoutMSec());
}

bool TimeoutTypeTest::TestConstructorNotDefault_2(int32 msec) {
    TimeoutType time(msec);
    bool ok;
    if (msec < 0) {
        ok = time.GetTimeoutMSec() == 0u;
        if (ok) {
            ok = time.GetTimeoutUSec() == 0u;
        }
    }
    else {
        ok = time.GetTimeoutMSec() == static_cast<uint32>(msec);
        if (ok) {
            ok = time.GetTimeoutUSec() == static_cast<uint64>(msec) * 1000;
        }
    }
    return ok;
}

bool TimeoutTypeTest::TestConstructorNotDefault_3(uint64 usec) {
    TimeoutType time(usec);
    bool ok = time.GetTimeoutMSec() == static_cast<uint32>(usec / 1000u);
    if (ok) {
        ok = time.GetTimeoutUSec() == usec;
    }
    usec = static_cast<uint64>(0xFFFFFFFFFFFFF);
    TimeoutType time2(usec);
    if (ok) {
        ok = (time2.GetTimeoutMSec() == 0xFFFFFFFFu);
    }
    if (ok) {
        ok = time2.GetTimeoutUSec() == usec;
    }
    return ok;
}

bool TimeoutTypeTest::TestSetTimeoutSec(double sec) {
    TimeoutType time;
    time.SetTimeoutSec(sec);
    bool ok = (sec * 1000 == time.GetTimeoutMSec());
    if (ok) {
        ok = (sec * 1000000.0 == time.GetTimeoutUSec());
    }
    return ok;
}

bool TimeoutTypeTest::TestSetTimeoutHighResolutionTimerTicks() {
    TimeoutType timeout;
    int64 ticks = 100000000;
    uint32 expectedTimeout;
    uint64 expecteduTimeout;
    timeout.SetTimeoutHighResolutionTimerTicks(ticks);
    expectedTimeout = ticks * HighResolutionTimer::Period() * 1000;
    expecteduTimeout = ticks * HighResolutionTimer::Period() * 1000000;
    retValue = (expectedTimeout == timeout.GetTimeoutMSec());
    if (retValue) {
        retValue = (expecteduTimeout == timeout.GetTimeoutUSec());
    }
    return retValue;
}

bool TimeoutTypeTest::TestHighResolutionTimerTicks() {
    TimeoutType timeout;
    int32 msecTimeout = 100;
    uint64 expectedTicks;
    timeout.SetTimeoutSec(double(msecTimeout) * 1e-3);
    //Floating point operations is order dependent. Here, the same operations as the implementation are followed otherwise the result may don't meet the programmer expectations
    expectedTicks = uint64((1e-6 * (msecTimeout * 1e3)) * HighResolutionTimer::Frequency());
    return (expectedTicks == timeout.HighResolutionTimerTicks());
}

bool TimeoutTypeTest::TestSubstractAssignOperator() {
    int32 intialTimeout = 100;
    TimeoutType timeout(static_cast<uint32>(intialTimeout));
    uint32 substractValue = 60u;
    int32 expectedTime = intialTimeout - substractValue;
    timeout -= substractValue;
    retValue = uint32(expectedTime) == timeout.GetTimeoutMSec();
    retValue &= (uint64(expectedTime) * 1000) == timeout.GetTimeoutUSec();
    timeout -= substractValue;
    retValue &= (0u == timeout.GetTimeoutMSec());
    if (retValue) {
        retValue = 0u == timeout.GetTimeoutUSec();
    }
    //Using uint64
    int64 intialTimeoutu = 100 * 1000;
    TimeoutType timeoutu(static_cast<uint64>(intialTimeoutu));
    uint64 substractValueu = 60000u;
    uint64 expectedTimeu = intialTimeoutu - substractValueu;
    timeoutu -= substractValueu;
    retValue &= expectedTimeu == timeoutu.GetTimeoutUSec();
    retValue &= static_cast<uint32>(expectedTimeu / 1000u) == timeoutu.GetTimeoutMSec();
    timeoutu -= substractValueu;
    retValue &= (0 == timeoutu.GetTimeoutUSec());
    if (retValue) {
        retValue = 0 == timeoutu.GetTimeoutMSec();
    }

    //Using uint64 and values smaller than 1 ms
    int64 intialTimeoutu2 = 100;
    TimeoutType timeoutu2(static_cast<uint64>(intialTimeoutu2));
    uint64 substractValueu2 = 60u;
    uint64 expectedTimeu2 = intialTimeoutu2 - substractValueu2;
    timeoutu2 -= substractValueu2;
    retValue &= expectedTimeu2 == timeoutu2.GetTimeoutUSec();
    retValue &= 0u == timeoutu2.GetTimeoutMSec();
    timeoutu2 -= substractValueu2;
    retValue &= (0 == timeoutu2.GetTimeoutUSec());
    if (retValue) {
        retValue = 0 == timeoutu2.GetTimeoutMSec();
    }

    //Using uint64 and values greater than 0xFFFFFFFF*1000 microseconds
    int64 intialTimeoutu3 = static_cast<uint64>(0xFFFFFFFF) * 1e4;
    TimeoutType timeoutu3(static_cast<uint64>(intialTimeoutu3));
    uint64 substractValueu3 = 6000u;
    uint64 expectedTimeu3 = intialTimeoutu3 - substractValueu3;
    timeoutu3 -= substractValueu3;
    retValue &= expectedTimeu3 == timeoutu3.GetTimeoutUSec();
    retValue &= TTInfiniteWait == timeoutu3.GetTimeoutMSec();
    substractValueu3 = static_cast<uint64>(0xFFFFFFFF) * 9999;
    timeoutu3 -= substractValueu3;
    expectedTimeu3 -= substractValueu3;
    retValue &= (expectedTimeu3 == timeoutu3.GetTimeoutUSec());
    if (retValue) {
        retValue = static_cast<uint32>(expectedTimeu3 / 1000) == timeoutu3.GetTimeoutMSec();
    }
    return retValue;
}

bool TimeoutTypeTest::TestEqualComparison() {
    TimeoutType timeout1(100u);
    TimeoutType timeout2(100u);
    TimeoutType timeout3(10u);
    retValue = (timeout1 == timeout2);
    retValue &= !(timeout1 == timeout3);
    retValue &= (timeout1 == 100u);
    retValue &= (timeout1 != 10u);
    return retValue;
}

bool TimeoutTypeTest::TestDiffComparison() {
    TimeoutType timeout1(100u);
    TimeoutType timeout2(100u);
    TimeoutType timeout3(10u);
    retValue = !(timeout1 != timeout2);
    retValue &= timeout1 != timeout3;
    return retValue;
}

bool TimeoutTypeTest::TestAssignOperator() {
    TimeoutType timeout1(100u);
    TimeoutType timeout2;
    timeout2 = timeout1;
    return (timeout1.GetTimeoutMSec() == timeout2.GetTimeoutMSec());
}

bool TimeoutTypeTest::TestIsFinite() {
    TimeoutType timeout1;
    TimeoutType timeout2(100u);
    retValue = !timeout1.IsFinite();
    retValue &= timeout2.IsFinite();
    return retValue;
}

bool TimeoutTypeTest::TestGetTimeoutMSec() {
    TimeoutType timeout;
    return (TTInfiniteWait == timeout.GetTimeoutMSec());
}
