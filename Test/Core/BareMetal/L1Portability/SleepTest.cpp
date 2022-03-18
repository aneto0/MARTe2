/**
 * @file SleepTest.cpp
 * @brief Source file for class SleepTest
 * @date 25/06/2015
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
 * the class SleepTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

//#include <time.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SleepTest.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SleepTest::TestGetDateSeconds() {
    return (Sleep::GetDateSeconds() != 0);
}

bool SleepTest::TestNoMore(float64 sec) {
    bool testResult = false;
    float64 maxSleepTime = 2 * 1000 * sec; /* 100% margin */
    uint64 initialCounter = HighResolutionTimer::Counter();

    Sleep::NoMore(sec);

    uint64 sleptTime = (HighResolutionTimer::Counter() - initialCounter) * HighResolutionTimer::Period();
    testResult = (sleptTime <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSec(float64 sec) {
    bool testResult = false;
    float64 maxSleepTime = 2 * 1000 * sec; /* 100% margin */
    uint64 initialCounter = HighResolutionTimer::Counter();

    Sleep::Sec(sec);

    uint64 sleptTime = (HighResolutionTimer::Counter() - initialCounter) * HighResolutionTimer::Period();
    testResult = (sleptTime <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestMSec(int32 msec) {
    bool testResult = false;
    float64 maxSleepTime = 2 * msec; /* 100% margin */
    uint64 initialCounter = HighResolutionTimer::Counter();

    Sleep::MSec(msec);

    uint64 sleptTime = (HighResolutionTimer::Counter() - initialCounter) * HighResolutionTimer::Period();
    testResult = (sleptTime <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestBusy(float64 sec) {
    bool testResult = false;
    float64 maxSleepTime = 2 * 1000 * sec; /* 100% margin */
    uint64 initialCounter = HighResolutionTimer::Counter();

    Sleep::Busy(sec);

    uint64 sleptTime = (HighResolutionTimer::Counter() - initialCounter) * HighResolutionTimer::Period();
    testResult = (sleptTime <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSemiBusy(float64 totalSleepSec,
                             float64 nonBusySleepSec) {
    bool testResult = false;
    float64 maxSleepTime = 2 * 1000 * totalSleepSec; /* 100% margin */
    uint64 initialCounter = HighResolutionTimer::Counter();

    Sleep::SemiBusy(totalSleepSec, nonBusySleepSec);

    uint64 sleptTime = (HighResolutionTimer::Counter() - initialCounter) * HighResolutionTimer::Period();
    testResult = (sleptTime <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSetSchedulerGranularity() {
    uint32 currentValue = Sleep::GetSchedulerGranularity();
    uint32 valueToSet = 123456;
    Sleep::SetSchedulerGranularity(valueToSet);
    bool ok = (valueToSet == Sleep::GetSchedulerGranularity());
    Sleep::SetSchedulerGranularity(currentValue);
    return ok;
}

bool SleepTest::TestGetSchedulerGranularity() {
    return TestSetSchedulerGranularity();
}

