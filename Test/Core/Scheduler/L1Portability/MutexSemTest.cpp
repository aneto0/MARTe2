/**
 * @file MutexSemTest.cpp
 * @brief Source file for class MutexSemTest
 * @date 25/06/2015
 * @author Giuseppe Ferro
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
 * the class MutexSemTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MutexSemTest.h"
#include "Sleep.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MutexSemTest::MutexSemTest() {
    testMutex.Create();
    synchSem.Create();

    sharedVariable = 0;
    nOfExecutingThreads = 0;
    testMutexTimeout = TTInfiniteWait;
    failed = true;
    stop = false;
}

MutexSemTest::~MutexSemTest() {
    testMutex.Close();
}

bool MutexSemTest::TestDefaultConstructor() {
    MutexSem target;
    return target.IsClosed() && !target.IsRecursive();
}

bool MutexSemTest::TestCreate(bool recursive) {
    MutexSem testSem;
    bool test = testSem.Create(recursive);
    if (test) {
        test &= (testSem.GetProperties() != NULL);
    }
    testSem.Close();
    return test;
}

bool MutexSemTest::TestClose(bool recursive) {
    MutexSem testSem;
    testSem.Create(recursive);
    return testSem.Close();
}

bool MutexSemTest::GenericMutexSemTestCaller(int32 nOfThreads,
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

void TestLockCallback(MutexSemTest &mt) {
    mt.synchSem.Wait();
    while (!mt.stop) {
        ErrorManagement::ErrorType err = mt.testMutex.Lock(mt.testMutexTimeout);
        mt.failed |= (err != ErrorManagement::NoError);
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
    Threads::EndThread();

}

bool MutexSemTest::TestLock(int32 nOfThreads,
                            TimeoutType timeout) {
    return GenericMutexSemTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestLockCallback);
}

void TestUnLockCallback(MutexSemTest &mt) {
    mt.synchSem.Wait();

    while (!mt.stop) {
        ErrorManagement::ErrorType err = mt.testMutex.Lock(mt.testMutexTimeout);
        mt.failed |= (err != ErrorManagement::NoError);
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
    Threads::EndThread();

}

bool MutexSemTest::TestUnLock(int32 nOfThreads,
                              TimeoutType timeout) {
    return GenericMutexSemTestCaller(nOfThreads, timeout, (ThreadFunctionType) TestUnLockCallback);
}

void TestLockErrorCodeCallback(MutexSemTest &mt) {
    mt.failed = false;
    //This should fail because it was already locked in the TestLockErrorCode
    ErrorManagement::ErrorType err = mt.testMutex.Lock(mt.testMutexTimeout);
    if (err != ErrorManagement::Timeout) {
        mt.failed = true;
    }
    Atomic::Decrement(&mt.nOfExecutingThreads);
    Threads::EndThread();

}

bool MutexSemTest::TestLockErrorCode() {
    bool ok = false;
    ErrorManagement::ErrorType err = testMutex.Lock();
    if (err == ErrorManagement::NoError) {
        ok = GenericMutexSemTestCaller(1, 1, (ThreadFunctionType) TestLockErrorCodeCallback);
    }
    testMutex.UnLock();

    return ok;
}

bool MutexSemTest::TestIsClosed() {
    bool result = true;
    MutexSem target;
    target.Create();
    result = result && !target.IsClosed();
    target.Close();
    result = result && target.IsClosed();
    return result;
}

bool MutexSemTest::TestIsRecursive() {
    bool test = true;

    testMutex.Close();
    test = testMutex.Create(false);
    test &= !testMutex.IsRecursive();
    testMutex.Close();

    test &= testMutex.Create(true);
    test &= testMutex.IsRecursive();
    testMutex.Close();

    return test;
}

void TestRecursiveCallback(MutexSemTest &mt) {
    mt.testMutex.Lock();
    mt.testMutex.Lock();
    mt.testMutex.UnLock();
    mt.testMutex.UnLock();
    Atomic::Decrement(&mt.nOfExecutingThreads);
    Threads::EndThread();

}

bool MutexSemTest::TestRecursive(bool recursive) {
    bool test = true;
    testMutex.Close();
    test = testMutex.Create(recursive);
    nOfExecutingThreads = 1;
    int32 counter = 0;
    ThreadIdentifier threadId = Threads::BeginThread((ThreadFunctionType) TestRecursiveCallback, this);
    while (nOfExecutingThreads == 1) {
        Sleep::MSec(100);
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

bool MutexSemTest::TestCopyConstructor() {
    bool test = true;
    test = testMutex.Create(false);
    MutexSem copyTestMutex(testMutex);

    if (testMutex.GetProperties() != copyTestMutex.GetProperties()) {
        test = false;
    }

    test &= (testMutex.Lock() == ErrorManagement::NoError);
    test &= copyTestMutex.UnLock();

    return test;
}
