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

#include "MutexTest.h"
#include "Sleep.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MutexTest::MutexTest() {
    testMutex.Create();
    synchSem.Create();

    sharedVariable = 0;
    nOfExecutingThreads = 0;
    testMutexTimeout = TTInfiniteWait;
    failed = true;
    stop = false;
}

MutexTest::~MutexTest() {
    testMutex.Close();
}

bool MutexTest::GenericMutexTestCaller(int32 nOfThreads,
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

void TestLockCallback(MutexTest &mt) {
    mt.synchSem.Wait();
    while (!mt.stop) {
        mt.failed |= !mt.testMutex.Lock(mt.testMutexTimeout);
        int32 state = mt.sharedVariable;
        mt.sharedVariable++;
        Sleep::MSec(10);
        if (mt.sharedVariable != (state + 1)) {
            mt.failed = true;
        }
        mt.testMutex.UnLock();
        if (mt.failed) {
            break;
        }
    }
    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool MutexTest::TestLock(int32 nOfThreads,
                         TimeoutType timeout) {
    return GenericMutexTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestLockCallback);
}

void TestUnLockCallback(MutexTest &mt) {
    mt.synchSem.Wait();

    while (!mt.stop) {
        mt.testMutex.Lock(mt.testMutexTimeout);
        int32 state = mt.sharedVariable;
        mt.sharedVariable++;
        Sleep::MSec(10);
        if (mt.sharedVariable != (state + 1)) {
            mt.failed = true;
        }
        mt.failed |= !mt.testMutex.UnLock();
        if (mt.failed) {
            break;
        }
    }
    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool MutexTest::TestUnLock(int32 nOfThreads,
                           TimeoutType timeout) {
    return GenericMutexTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestUnLockCallback);
}

void TestFastLockCallback(MutexTest &mt) {
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

bool MutexTest::TestFastLock(int32 nOfThreads,
                             TimeoutType timeout) {
    return GenericMutexTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastLockCallback);
}

void TestFastUnLockCallback(MutexTest &mt) {
    mt.synchSem.Wait();

    while (!mt.stop) {
        mt.testMutex.FastLock(mt.testMutexTimeout);
        int32 state = mt.sharedVariable;
        mt.sharedVariable++;
        Sleep::MSec(10);
        if (mt.sharedVariable != (state + 1)) {
            mt.failed = true;
        }
        mt.failed |= !mt.testMutex.FastUnLock();
        if (mt.failed) {
            break;
        }
    }
    //Careful that without this, the threads when exiting can overwrite the
    //assignment operation and the subtraction of the value thus generating
    //a racing condition at the end of the test (see below while(nOfExecutingThreads > 0)
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool MutexTest::TestFastUnLock(int32 nOfThreads,
                               TimeoutType timeout) {
    return GenericMutexTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestFastUnLockCallback);
}

void TestFastTryLockCallback(MutexTest &mt) {
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

bool MutexTest::TestFastTryLock(int32 nOfThreads) {
    bool test = GenericMutexTestCaller(nOfThreads, TTInfiniteWait, (ThreadFunctionType) TestFastTryLockCallback);
    MutexSem sem;
    test = sem.FastTryLock();
    test = !sem.FastTryLock();
    sem.FastUnLock();
    sem.Close();
    return test;
}

void TestLockErrorCodeCallback(MutexTest &mt) {
    mt.failed = false;
    Error returnError;
    //This should fail because it was already locked in the TestLockErrorCode
    bool ret = mt.testMutex.Lock(mt.testMutexTimeout, returnError);
    if (!ret) {
        mt.failed = true;
    }
    if (returnError != Timeout) {
        mt.failed = true;
    }
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

void TestFastLockErrorCodeCallback(MutexTest &mt) {
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

bool MutexTest::TestLockErrorCode() {

    Error returnError;
    bool test = true;

    test = testMutex.Lock(TTInfiniteWait, returnError);
    if (returnError != Debug) {
        test = False;
    }

    if (test) {
        GenericMutexTestCaller(1, 1, (ThreadFunctionType) TestLockErrorCodeCallback);
    }
    testMutex.UnLock();

    return test;
}

bool MutexTest::TestFastLockErrorCode() {

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

bool MutexTest::TestIsRecursive() {
    bool test = true;

    testMutex.Close();
    test = testMutex.Create(false, false);
    test &= !testMutex.IsRecursive();
    testMutex.Close();

    test &= testMutex.Create(true, false);
    test &= !testMutex.IsRecursive();
    testMutex.Close();

    test = testMutex.Create(false, true);
    test &= testMutex.IsRecursive();
    testMutex.Close();

    test &= testMutex.Create(true, true);
    test &= testMutex.IsRecursive();
    testMutex.Close();

    return test;
}

void TestRecursiveCallback(MutexTest &mt) {
    mt.failed = mt.testMutex.Lock();
    mt.failed = mt.testMutex.Lock();
    mt.testMutex.UnLock();
    mt.testMutex.UnLock();
    Atomic::Decrement(&mt.nOfExecutingThreads);
}

bool MutexTest::TestRecursive(bool recursive) {
    bool test = true;
    testMutex.Close();
    test = testMutex.Create(false, recursive);
    nOfExecutingThreads = 1;
    int32 counter = 0;
    TID threadId = Threads::BeginThread((ThreadFunctionType) TestRecursiveCallback, this);
    while (nOfExecutingThreads == 1) {
        SleepMSec(100);
        if (counter++ > 10) {
            if (!recursive) {
                test = true;
            }
            else {
                test = false;
            }
            Threads::Kill(threadId);
            break;
        }
    }
    if (nOfExecutingThreads == 0) {
        //A non-recursive semaphore should have dead-locked...
        if (!recursive) {
            test = false;
        }
        else {
            test = true;
        }
    }

    return test;
}

bool MutexTest::TestCopyConstructor() {
    bool test = true;
    test = testMutex.Create(false, false);
    MutexSem copyTestMutex(testMutex);

    if (testMutex.Handle() != copyTestMutex.Handle()) {
        test = false;
    }

    test &= testMutex.Lock();
    test &= copyTestMutex.UnLock();

    return test;
}
