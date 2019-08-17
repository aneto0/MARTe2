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
#include "TimeType.h"
#include "MilliSeconds.h"
#include "Ticks.h"
#include "HighResolutionTimer.h"
#include "CompositeErrorManagement.h"


using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TimeoutTypeTest::TimeoutTypeTest() {
}

TimeoutTypeTest::~TimeoutTypeTest() {
}

bool TimeoutTypeTest::TestConstructorDefault() {
	MilliSeconds time;
    return (time.GetTimeRaw()  == 0);
}

bool TimeoutTypeTest::TestConstructorNotDefault(uint32 msec) {
	MilliSeconds time(msec,Units::ms);
    return (msec == time.GetTimeRaw());
}

bool TimeoutTypeTest::TestSetTimeoutSec(double sec) {
	MilliSeconds time(sec,Units::s);
    return (sec * 1000 == time.GetTimeRaw());
}

bool TimeoutTypeTest::TestSetTimeoutHighResolutionTimerTicks() {
    int64 ticks = 100000000;
	MilliSeconds time(ticks,Units::ticks);
    int64 expectedTimeout;
    expectedTimeout = static_cast<int64>((double)ticks * HighResolutionTimer::Period() * 1000.0 +0.5);
    bool ret = (expectedTimeout == time.GetTimeRaw());
    if (!ret){
    	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"expecting ",expectedTimeout, " instead ",time.GetTimeRaw(), " Period = ",HighResolutionTimer::Period());
    }
    return ret;
}

bool TimeoutTypeTest::TestHighResolutionTimerTicks() {
    int32 msecTimeout = 100;
	Ticks time(msecTimeout,Units::ms);
    uint64 expectedTicks;
    expectedTicks = uint64(1e-3*msecTimeout*HighResolutionTimer::Frequency()+0.5);

    bool ret = (expectedTicks == time.GetTimeRaw());
    if (!ret){
    	COMPOSITE_REPORT_ERROR(ErrorManagement::FatalError,"expecting ",expectedTicks, " instead ",time.GetTimeRaw(), " Frequency = ",HighResolutionTimer::Frequency());
    }
    return ret;
}

bool TimeoutTypeTest::TestSubstractAssignOperator() {
    uint32 intialTimeout = 100;
    uint32 substractValue = 60;
    uint32 expectedTime = intialTimeout-substractValue;

    MilliSeconds time(intialTimeout,Units::ms);
    time -= MilliSeconds(substractValue,Units::ms);

    return  (expectedTime == time.GetTimeRaw());
}

bool TimeoutTypeTest::TestEqualComparison() {
    MilliSeconds time1(100,Units::ms);
    MilliSeconds time2(100,Units::ms);
    MilliSeconds time3(10,Units::ms);
    return  ((time1 == time2) && (time2 != time3));
}


bool TimeoutTypeTest::TestAssignOperator() {
    MilliSeconds time1(100,Units::ms);
    MilliSeconds time2;
    time2 = time1;
    return (time1.GetTimeRaw() == time2.GetTimeRaw());
}

bool TimeoutTypeTest::TestIsFinite() {
    MilliSeconds time1(MilliSeconds::Infinite);
    MilliSeconds time2(100,Units::ms);
    return time1.IsInfinite() && !time2.IsInfinite();
}

