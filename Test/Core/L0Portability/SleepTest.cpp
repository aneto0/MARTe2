/**
 * @file SleepTest.cpp
 * @brief Source file for class SleepTest
 * @date 25/06/2015
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
 * the class SleepTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SleepTest.h"
#include <time.h>

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool SleepTest::TestSleepAtleast(double sec) {
    bool testResult = false;
    double maxSleepTime = 2 * 1000 * sec; /* 100% margin */
    int initialTime = clock();

    Sleep::AtLeast(sec);

    testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000)) <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSleepNoMore(double sec) {
    bool testResult = false;
    double maxSleepTime = 2 * 1000 * sec; /* 100% margin */
    int initialTime = clock();

    Sleep::NoMore(sec);

    testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000)) <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSleepSec(double sec) {
    bool testResult = false;
    double maxSleepTime = 2 * 1000 * sec; /* 100% margin */
    int initialTime = clock();

    Sleep::Sec(sec);

    testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000)) <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSleepSec(float sec) {
    bool testResult = false;
    float maxSleepTime = 2 * 1000 * sec; /* 100% margin */
    int initialTime = clock();

    Sleep::Sec(sec);

    testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000)) <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSleepMSec(int32 msec) {
    bool testResult = false;
    double maxSleepTime = 2 * msec; /* 100% margin */
    int initialTime = clock();

    Sleep::MSec(msec);

    testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000)) <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSleepBusy(double sec) {
    bool testResult = false;
    double maxSleepTime = 2 * 1000 * sec; /* 100% margin */
    int initialTime = clock();

    Sleep::Busy(sec);

    testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000)) <= maxSleepTime);

    return testResult;
}

bool SleepTest::TestSemiBusy(double totalSleepSec,
                             double nonBusySleepSec) {
    bool testResult = false;
    double maxSleepTime = 2 * 1000 * totalSleepSec; /* 100% margin */
    int initialTime = clock();

    Sleep::SemiBusy(totalSleepSec, nonBusySleepSec);

    testResult = (((clock() - initialTime) / (CLOCKS_PER_SEC / 1000)) <= maxSleepTime);

    return testResult;
}

bool SleepTest::All() {
    bool ok = TestSleepAtleast(1.2);
    ok = ok && TestSleepNoMore(0.8);
    ok = ok && TestSleepSec(0.3);
    ok = ok && TestSleepSec(0.2);
    ok = ok && TestSleepMSec(120);
    ok = ok && TestSleepBusy(0.4);
    ok = ok && TestSemiBusy(0.5, 0.2);
    GetDateSeconds();
    return ok;
}
