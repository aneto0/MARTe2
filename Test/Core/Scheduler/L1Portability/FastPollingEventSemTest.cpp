/**
 * @file FastPollingEventSemTest.cpp
 * @brief Source file for class FastPollingEventSemTest
 * @date 24/03/2016
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
 * the class FastPollingEventSemTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FastPollingEventSemTest.h"
#include "Sleep.h"
#include "Atomic.h"
#include "FlagsType.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FastPollingEventSemTest::FastPollingEventSemTest() :
        extFlag(0),
        testEventExt(extFlag) {
    testEvent.Create();
    sharedVariable = 0;
    nOfExecutingThreads = 0;
    testEventTimeout=Ticks::Infinite;
    failed = true;
    stop = false;
    external = false;
}

FastPollingEventSemTest::~FastPollingEventSemTest() {
}

bool FastPollingEventSemTest::TestConstructor() {
    FastPollingEventSem eventSem;
    return true;
}

bool FastPollingEventSemTest::TestConstructorFromExternal() {
    volatile int32 x = 0;
    FastPollingEventSem evSem(x);
    if (x != 0) {
        return false;
    }

    x++;
    FastPollingEventSem evSem1(x);
    return x == 1;
}

bool FastPollingEventSemTest::TestCreate(bool wait) {
    FastPollingEventSem testSem;
    testSem.Create(wait);

    return true;
}

bool FastPollingEventSemTest::TestCreateExternal(bool wait) {

    volatile int32 x;
    FastPollingEventSem testSem(x);
    testSem.Create(wait);

    bool test = true;
    if (wait) {
        test = (x == 0);
    }
    else {
        test = (x == 1);
    }
    return test;
}

bool FastPollingEventSemTest::GenericEventTestCaller(int32 nOfThreads,const Ticks &timeout,ThreadFunctionType functionToTest) {
    failed = false;
    stop = false;
    nOfExecutingThreads = 0;
    testEventTimeout = timeout;

    testEvent.Reset();
    for (int32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread(functionToTest, this);
        nOfExecutingThreads++;
    }
    while (sharedVariable < nOfExecutingThreads) {
        Sleep::Short(10,Units::ms);
    }

    Sleep::Short(10,Units::ms);
    failed = (sharedVariable != nOfExecutingThreads);

    if (external) {
        testEventExt.FastPost();
    }
    else {
        testEvent.FastPost();
    }

    Sleep::Short(1.0f,Units::s);
    stop = true;
    while (nOfExecutingThreads > 0) {
        Sleep::Short(10,Units::ms);
    }
    return !failed;
}

void TestFastWaitCallback(FastPollingEventSemTest *mt) {

    FlagsType error;
    Atomic::Increment(&mt->sharedVariable);
    if (mt->external) {
        mt->testEventExt.FastWait();
    }
    else {
        mt->testEvent.FastWait();
    }

    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt->nOfExecutingThreads);
}

bool FastPollingEventSemTest::TestFastWait(int32 nOfThreads,const Ticks & timeout) {
    return GenericEventTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastWaitCallback);
}

bool FastPollingEventSemTest::TestFastWaitExternal(int32 nOfThreads,const Ticks & timeout) {
    external = true;
    return GenericEventTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastWaitCallback);
}

bool FastPollingEventSemTest::TestFastPost(int32 nOfThreads,const Ticks &timeout) {
    return GenericEventTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastWaitCallback);
}

bool FastPollingEventSemTest::TestFastPostExternal(int32 nOfThreads,const Ticks &timeout) {
    external = true;
    return GenericEventTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastWaitCallback);
}

void TestFastResetWaitCallback(FastPollingEventSemTest *mt) {
    FlagsType error;
    Atomic::Increment(&mt->sharedVariable);
    if (mt->external) {
        mt->testEventExt.FastResetWait();
    }
    else {
        mt->testEvent.FastResetWait();
    }
    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt->nOfExecutingThreads);
}

bool FastPollingEventSemTest::TestFastResetWait(int32 nOfThreads) {
    bool test = GenericEventTestCaller(nOfThreads, Ticks::Infinite, (ThreadFunctionType) TestFastResetWaitCallback);
    sharedVariable=0;
    // launch other threads without reset the semaphore
    test &= GenericEventTestCaller(nOfThreads, Ticks::Infinite, (ThreadFunctionType) TestFastResetWaitCallback);

    return test;
}

bool FastPollingEventSemTest::TestFastResetWaitExternal(int32 nOfThreads) {
    external = true;
    bool test = GenericEventTestCaller(nOfThreads, Ticks::Infinite, (ThreadFunctionType) TestFastResetWaitCallback);
    sharedVariable=0;
    // launch other threads without reset the semaphore
    test &= GenericEventTestCaller(nOfThreads, Ticks::Infinite, (ThreadFunctionType) TestFastResetWaitCallback);

    return test;
}

void TestFastWaitErrorCodeCallback(FastPollingEventSemTest *mt) {
    mt->failed = false;
    //This should fail because it was already locked in the TestFastLockErrorCode
    ErrorManagement::ErrorType err = mt->testEvent.FastWait(Ticks::Infinite);
    if (err != ErrorManagement::ErrorType(ErrorManagement::NoError)) {
        mt->failed = true;
    }
    Atomic::Decrement(&mt->nOfExecutingThreads);
}

bool FastPollingEventSemTest::TestFastWaitErrorCode() {

    bool ok = true;
    Threads::BeginThread((ThreadFunctionType) TestFastWaitErrorCodeCallback, this);
    nOfExecutingThreads++;

    ErrorManagement::ErrorType err = testEvent.FastWait(Ticks(1,Units::ticks));
    ok &= (err == ErrorManagement::ErrorType(ErrorManagement::Timeout));

    testEvent.FastPost();
    while (nOfExecutingThreads > 0) {
        Sleep::Short(10,Units::ms);
    }
    ok &= !failed;
    return ok;
}

