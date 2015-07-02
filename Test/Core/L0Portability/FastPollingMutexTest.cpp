/**
 * @file MutexTest.cpp
 * @brief Source file for class MutexTest
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
 * the class MutexTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "FastPollingMutexTest.h"
#include "Sleep.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FastPollingMutexTest::FastPollingMutexTest() {
    testMutex.Create();
    synchSem.Create();

    sharedVariable = 0;
    nOfExecutingThreads = 0;
    testMutexTimeout = TTInfiniteWait;
    failed = true;
    stop = false;
}

FastPollingMutexTest::~FastPollingMutexTest() {
}

bool FastPollingMutexTest::TestConstructor() {
    FastPollingMutexSem mutexSem;
    return !mutexSem.Locked();
}

bool FastPollingMutexTest::TestCreate(bool locked) {
    FastPollingMutexSem testSem;
    testSem.Create(locked);

    bool test = true;
    if (locked) {
        test = testSem.Locked();
    }
    else {
        test = !testSem.Locked();
    }
    return test;
}

bool FastPollingMutexTest::GenericMutexTestCaller(
        int32 nOfThreads, TimeoutType timeout,
        ThreadFunctionType functionToTest) {
    failed = false;
    stop = false;
    nOfExecutingThreads = 0;
    testMutexTimeout = timeout;

    synchSem.Reset();
    for (int32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread(functionToTest, this);
        nOfExecutingThreads++;
    }
    synchSem.Post();

    Sleep::Sec(1.0f);
    stop = true;
    while (nOfExecutingThreads > 0) {
        Sleep::MSec(10);
    }
    return !failed;
}

void TestFastLockCallback(FastPollingMutexTest &mt) {
    mt.synchSem.Wait();
    while (!mt.stop) {
        mt.failed |= !mt.testMutex.FastLock(mt.testMutexTimeout);
        int32 state = mt.sharedVariable;
        mt.sharedVariable++;
        Sleep::MSec(10);
        if (mt.sharedVariable != (state + 1)) {
            mt.failed = true;
        }
        mt.testMutex.FastUnLock();
        if (mt.failed) {
            break;
        }
    }
    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool FastPollingMutexTest::TestFastLock(int32 nOfThreads, TimeoutType timeout) {
    return GenericMutexTestCaller(nOfThreads, timeout,
                                  (ThreadFunctionType) TestFastLockCallback);
}

void TestFastUnLockCallback(FastPollingMutexTest &mt) {
    mt.synchSem.Wait();

    while (!mt.stop) {
        mt.testMutex.FastLock(mt.testMutexTimeout);
        int32 state = mt.sharedVariable;
        mt.sharedVariable++;
        Sleep::MSec(10);
        if (mt.sharedVariable != (state + 1)) {
            mt.failed = true;
        }

        mt.testMutex.FastUnLock();
    }
    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool FastPollingMutexTest::TestFastUnLock(int32 nOfThreads,
                                                  TimeoutType timeout) {
    return GenericMutexTestCaller(nOfThreads, timeout,
                                  (ThreadFunctionType) TestFastUnLockCallback);
}

void TestFastTryLockCallback(FastPollingMutexTest &mt) {
    mt.synchSem.Wait();
    while (!mt.stop) {
        if (mt.testMutex.FastTryLock()) {
            int32 state = mt.sharedVariable;
            mt.sharedVariable++;
            Sleep::MSec(10);
            if (mt.sharedVariable != (state + 1)) {
                mt.failed = true;
            }
            mt.testMutex.FastUnLock();
        }
        if (mt.failed) {
            break;
        }
    }
    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool FastPollingMutexTest::TestFastTryLock(int32 nOfThreads) {
    bool test = GenericMutexTestCaller(
            nOfThreads, TTInfiniteWait,
            (ThreadFunctionType) TestFastTryLockCallback);
    FastPollingMutexSem sem;
    test = sem.FastTryLock();
    test = !sem.FastTryLock();
    sem.FastUnLock();
    return test;
}

void TestFastLockErrorCodeCallback(FastPollingMutexTest &mt) {
    mt.failed = false;
    Error returnError;
    //This should fail because it was already locked in the TestFastLockErrorCode
    bool ret = mt.testMutex.FastLock(mt.testMutexTimeout, returnError);
    if (!ret) {
        mt.failed = true;
    }
    if (returnError != Timeout) {
        mt.failed = true;
    }
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool FastPollingMutexTest::TestFastLockErrorCode() {

    Error returnError;
    bool test = true;

    test = testMutex.FastLock(TTInfiniteWait, returnError);
    if (returnError != Debug) {
        test = False;
    }

    if (test) {
        GenericMutexTestCaller(1, 1, (ThreadFunctionType) TestFastLockErrorCodeCallback);
    }
    testMutex.FastUnLock();

    return test;
}

bool FastPollingMutexTest::TestLocked() {
    bool test = true;

    testMutex.Create(false);
    test &= !testMutex.Locked();

    testMutex.Create(true);
    test &= testMutex.Locked();

    return test;
}

void TestRecursiveCallback(FastPollingMutexTest &mt) {
    mt.failed = mt.testMutex.FastLock();
    mt.failed = mt.testMutex.FastLock();
    mt.testMutex.FastUnLock();
    mt.testMutex.FastUnLock();
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool FastPollingMutexTest::TestRecursive() {
    bool test = false;
    testMutex.Create(false);
    nOfExecutingThreads = 1;
    int32 counter = 0;
    TID threadId = Threads::BeginThread((ThreadFunctionType) TestRecursiveCallback, this);
    while (nOfExecutingThreads == 1) {
        SleepMSec(100);
        if (counter++ > 10) {
            test = true;
            Threads::Kill(threadId);
            break;
        }
    }
    if (nOfExecutingThreads == 0) {
        //A fast polling mutex semaphore should have dead-locked...
        test = false;
    }

    return test;
}
