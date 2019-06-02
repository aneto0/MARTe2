/**
 * @file FastPollingMutexSemTest.cpp
 * @brief Source file for class FastPollingMutexSemTest
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
 * the class FastPollingMutexSemTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../../Scheduler/L1Portability/FastPollingMutexSemTest.h"

#include "Sleep.h"
#include "Atomic.h"
#include "FlagsType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

FastPollingMutexSemTest::FastPollingMutexSemTest() :
        extFlag(0),
        testMutexExt(extFlag) {
    testMutex.Create();
    synchSem.Create();
    sharedVariable = 0;
    nOfExecutingThreads = 0;
    testMutexTimeout = TTInfiniteWait;
    failed = true;
    stop = false;
    external = false;
}

FastPollingMutexSemTest::~FastPollingMutexSemTest() {
}

bool FastPollingMutexSemTest::TestConstructor() {
    FastPollingMutexSem mutexSem;
    return !mutexSem.Locked();
}

bool FastPollingMutexSemTest::TestConstructorFromExternal() {
    volatile int32 x = 0;
    FastPollingMutexSem mutexSem(x);
    if (mutexSem.Locked()) {
        return false;
    }

    x++;
    FastPollingMutexSem mutexSem1(x);
    return (mutexSem.Locked());
}

bool FastPollingMutexSemTest::TestCreate(bool locked) {
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

bool FastPollingMutexSemTest::TestCreateExternal(bool locked) {

    volatile int32 x;
    FastPollingMutexSem testSem(x);
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

bool FastPollingMutexSemTest::GenericMutexTestCaller(int32 nOfThreads,
                                                     TimeoutType timeout,
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

void TestFastLockCallback(FastPollingMutexSemTest &mt) {
    FlagsType error;
    mt.synchSem.Wait();
    while (!mt.stop) {
        if (mt.external) {
            mt.failed |= !(mt.testMutexExt.FastLock(mt.testMutexTimeout) == ErrorManagement::NoError);
        }
        else {
            mt.failed |= !(mt.testMutex.FastLock(mt.testMutexTimeout) == ErrorManagement::NoError);
        }
        int32 state = mt.sharedVariable;
        mt.sharedVariable++;
        Sleep::MSec(10);
        if (mt.sharedVariable != (state + 1)) {
            mt.failed = true;
        }
        if (mt.external) {
            mt.testMutexExt.FastUnLock();
        }
        else {
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

bool FastPollingMutexSemTest::TestFastLock(int32 nOfThreads,
                                           TimeoutType timeout) {
    return GenericMutexTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastLockCallback);
}

bool FastPollingMutexSemTest::TestFastLockExternal(int32 nOfThreads,
                                                   TimeoutType timeout) {
    external = true;
    return GenericMutexTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastLockCallback);
}

void TestFastUnLockCallback(FastPollingMutexSemTest &mt) {
    mt.synchSem.Wait();

    while (!mt.stop) {
        if (mt.external) {
            mt.testMutexExt.FastLock(mt.testMutexTimeout);
        }
        else {
            mt.testMutex.FastLock(mt.testMutexTimeout);
        }
        int32 state = mt.sharedVariable;
        mt.sharedVariable++;
        Sleep::MSec(10);
        if (mt.sharedVariable != (state + 1)) {
            mt.failed = true;
        }

        if (mt.external) {
            mt.testMutexExt.FastUnLock();
        }
        else {
            mt.testMutex.FastUnLock();
        }
    }
    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool FastPollingMutexSemTest::TestFastUnLock(int32 nOfThreads,
                                             TimeoutType timeout) {
    return GenericMutexTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastUnLockCallback);
}

bool FastPollingMutexSemTest::TestFastUnLockExternal(int32 nOfThreads,
                                                     TimeoutType timeout) {
    external = true;
    return GenericMutexTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastUnLockCallback);
}

void TestFastTryLockCallback(FastPollingMutexSemTest &mt) {
    FlagsType error;
    mt.synchSem.Wait();
    while (!mt.stop) {
        if (mt.external) {
            mt.testMutexExt.FastTryLock();
        }
        else {
            mt.testMutex.FastTryLock();
        }
        int32 state = mt.sharedVariable;
        mt.sharedVariable++;
        Sleep::MSec(10);
        if (mt.sharedVariable != (state + 1)) {
            mt.failed = true;
        }
        if (mt.external) {
            mt.testMutexExt.FastUnLock();
        }
        else {
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

bool FastPollingMutexSemTest::TestFastTryLock(int32 nOfThreads) {
    bool test = GenericMutexTestCaller(nOfThreads, TTInfiniteWait, (ThreadFunctionType) TestFastTryLockCallback);
    FastPollingMutexSem sem;
    test = sem.FastTryLock();
    test = !sem.FastTryLock();
    sem.FastUnLock();
    return test;
}

bool FastPollingMutexSemTest::TestFastTryLockExternal(int32 nOfThreads) {
    external = true;
    bool test = GenericMutexTestCaller(nOfThreads, TTInfiniteWait, (ThreadFunctionType) TestFastTryLockCallback);
    FastPollingMutexSem sem;
    test = sem.FastTryLock();
    test = !sem.FastTryLock();
    sem.FastUnLock();
    return test;
}

void TestFastLockErrorCodeCallback(FastPollingMutexSemTest &mt) {
    mt.failed = false;
    FlagsType returnError;
    //This should fail because it was already locked in the TestFastLockErrorCode
    ErrorManagement::ErrorType err = mt.testMutex.FastLock(mt.testMutexTimeout);
    if (err != ErrorManagement::Timeout) {
        mt.failed = true;
    }
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool FastPollingMutexSemTest::TestFastLockErrorCode() {
    ErrorManagement::ErrorType err = testMutex.FastLock(TTInfiniteWait);
    bool ok = false;
    if (err == ErrorManagement::NoError) {
        ok = GenericMutexTestCaller(1, 1, (ThreadFunctionType) TestFastLockErrorCodeCallback);
    }
    testMutex.FastUnLock();

    return ok;
}

bool FastPollingMutexSemTest::TestLocked() {
    bool test = true;

    testMutex.Create(false);
    test &= !testMutex.Locked();

    testMutex.Create(true);
    test &= testMutex.Locked();

    return test;
}

void TestRecursiveCallback(FastPollingMutexSemTest &mt) {
    mt.testMutex.FastLock();
    mt.testMutex.FastLock();
    mt.testMutex.FastUnLock();
    mt.testMutex.FastUnLock();
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool FastPollingMutexSemTest::TestRecursive() {
    bool test = false;
    testMutex.Create(false);
    nOfExecutingThreads = 1;
    int32 counter = 0;
    ThreadIdentifier threadId = Threads::BeginThread((ThreadFunctionType) TestRecursiveCallback, this);
    while (nOfExecutingThreads == 1) {
        Sleep::MSec(100);
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
