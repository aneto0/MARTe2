/**
 * @file EventSemTest.cpp
 * @brief Source file for class EventSemTest
 * @date 02/07/2015
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
 * the class EventSemTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "EventSemTest.h"
#include "Threads.h"
#include "Sleep.h"
#include "HighResolutionTimer.h"
#include "MutexSem.h"
#include "Seconds.h"
#include "ErrorManagement.h"
#include "CompositeErrorManagement.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

EventSemTest::EventSemTest() {
    eventSem.Create();
    mutexSem.Create();
    sharedVariable = 0;
    sharedVariable1 = 0;
    timeout = MilliSeconds::Infinite;
}

EventSemTest::~EventSemTest() {
    eventSem.Close();
    mutexSem.Close();
}

bool EventSemTest::TestConstructor() {
    EventSem testSem;
    return testSem.IsClosed();
}

bool EventSemTest::TestDestructor() {
    EventSem testSem;
    testSem.Create();
    bool test = !testSem.IsClosed();
    testSem.~EventSem();
    test = testSem.IsClosed();
    return test;
}

bool EventSemTest::TestCreate() {
    EventSem testSem;
    bool test = testSem.Create();
    if (test) {
        test &= (testSem.GetProperties() != NULL);
    }
    testSem.Close();
    return test;
}

bool EventSemTest::TestClose() {
    EventSem testSem;
    testSem.Create();
    return testSem.Close();
}

bool EventSemTest::TestCopyConstructor() {
    EventSem testSem;
    testSem.Create();
    EventSem copySem(testSem);
    testSem.Reset();

    MilliSeconds tt(1,Units::ms);
    testSem.Wait(tt);

    bool test = copySem.Post();
    test &= (testSem.GetProperties() == copySem.GetProperties());

    testSem.Close();
    copySem.Close();

    return test;
}

bool EventSemTest::TestIsClosed() {
    bool result = true;
    EventSem target;
    target.Create();
    result = result && !target.IsClosed();
    target.Close();
    result = result && target.IsClosed();
    return result;
}

void ThreadLocked(EventSemTest *tt) {
//    MilliSeconds time(500,Units::ms);
    tt->sharedVariable1++;
    tt->eventSem.Wait(MilliSeconds(500,Units::ms));
    tt->sharedVariable1--;
    Threads::EndThread();
}

bool EventSemTest::TestCopyConstructor2Sem() {
    ErrorManagement::ErrorType ret;
    sharedVariable1 = 0;

    eventSem.Create();
    EventSem copySem(eventSem);
    eventSem.Reset();
    Ticks counter1 = HighResolutionTimer::GetTicks();

    Threads::BeginThread((ThreadFunctionType) ThreadLocked, this);
    while (sharedVariable1 == 0) {
        Sleep::Short(10,Units::ms);
    }
    ret.fatalError = !copySem.Post();
    REPORT_ERROR(ret,"copySem Post failed");

    while (sharedVariable1 == 1) {
        Sleep::Short(10,Units::ms);
    }
    Ticks counter2 = HighResolutionTimer::GetTicks();
    Ticks deltaT = counter2 - counter1;

    MilliSeconds deltaMS(deltaT);
    if (deltaMS.GetTimeRaw() > 5000){
    	ret.timeout = true;
        COMPOSITE_REPORT_ERROR(ret,"deltaT > 5000 = ",deltaMS.GetTimeRaw());
    }

    return ret;
}

bool EventSemTest::TestWait(MilliSeconds timeoutTime) {

    EventSem newSem(eventSem);
    eventSem.Reset();
    ErrorManagement::ErrorType err = eventSem.Wait(timeoutTime);
    if (err == ErrorManagement::NoError) {
        err = newSem.ResetWait(timeoutTime);
    }
    return (err == ErrorManagement::Timeout);
}

bool EventSemTest::TestPost() {
    eventSem.Reset();
    eventSem.Post();
    eventSem.Post();
    eventSem.Reset();
    bool test = eventSem.Post();
    eventSem.Close();
    return test;
}

bool EventSemTest::TestReset() {
    eventSem.Reset();
    eventSem.Post();
    eventSem.Post();
    bool test = eventSem.Reset();
    test &= eventSem.Post();
    eventSem.Close();
    return test;
}

void PosterThreadCallback(EventSemTest *eventSemTest) {
    uint32 maxTimeMS = 2000;
    Ticks startT = HighResolutionTimer::GetTicks();
    while (eventSemTest->sharedVariable < 2) {
        Sleep::Short(100,Units::ms);
        eventSemTest->sharedVariable = 1;
        eventSemTest->eventSem.Post();
        MilliSeconds deltaMS = HighResolutionTimer::GetTicks() - startT;

        if (deltaMS.GetTime() > maxTimeMS) {
            break;
        }
    }

    Threads::EndThread();

}

bool EventSemTest::TestWait() {
    eventSem.Reset();
    sharedVariable = 0;
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) PosterThreadCallback, this);
    ErrorManagement::ErrorType err = eventSem.Wait();

    if (sharedVariable == 0) {
        //Too fast wait has failed for sure...
        err = ErrorManagement::FatalError;
    }
    sharedVariable = 2;
    eventSem.Close();
    //Wait for the thread to terminate
    int32 counter = 0;
    while (Threads::IsAlive(tid)) {
        Sleep::Short(100,Units::ms);;
        if (counter++ > 10) {
            Threads::Kill(tid);
            break;
        }
    }
    return (err == ErrorManagement::NoError);
}

bool EventSemTest::TestResetWait() {
    sharedVariable = 0;
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) PosterThreadCallback, this);
    ErrorManagement::ErrorType err = eventSem.ResetWait(MilliSeconds::Infinite);

    if (sharedVariable == 0) {
        //Too fast wait has failed for sure...
        err = ErrorManagement::FatalError;
    }
    sharedVariable = 2;
    eventSem.Close();

    //Wait for the thread to terminate
    int32 counter = 0;
    while (Threads::IsAlive(tid)) {
        Sleep::Short(100,Units::ms);
        if (counter++ > 10) {
            Threads::Kill(tid);
            break;
        }
    }
    return (err == ErrorManagement::NoError);
}

void MultiThreadedTestWaitCallback(EventSemTest *eventSemTest) {
    //Wait before proceeding...
    if (eventSemTest->timeout == MilliSeconds::Infinite) {
        //Also test if passing no parameter is indeed MilliSeconds::Infinite
        eventSemTest->eventSem.Wait();
    }
    else {
        eventSemTest->eventSem.Wait(eventSemTest->timeout);
    }

    eventSemTest->mutexSem.Lock();
    eventSemTest->sharedVariable++;
    eventSemTest->mutexSem.UnLock();
    Threads::EndThread();

}

bool EventSemTest::MultiThreadedTestWait(uint32 nOfThreads) {
    //Initialise the value of the sharedVariable to some unexpected value.
    sharedVariable = 0xABCD;
    //Reset the semaphore
    eventSem.Reset();

    uint32 i = 0;
    for (i = 0; i < nOfThreads; i++) {
        //Each thread will try to increment the value of sharedVariable
        Threads::BeginThread((ThreadFunctionType) MultiThreadedTestWaitCallback, this);
    }
    //Allow threads to start
    Sleep::Short(100,Units::ms);
    //The semaphore was not posted yet, so the value of the sharedVariable should still be 0xABCD
    if (sharedVariable != 0xABCD) {
        return false;
    }
    //Reset the value of the sharedVariable
    sharedVariable = 0;
    eventSem.Post();
    //Wait some time for the test to succeed...
    while (sharedVariable != nOfThreads) {
        Sleep::Short(100,Units::ms);
        i--;
        //Waited 20 ms x nOfThreads and the value of the sharedVariable is still
        //not nOfThreads which likely indicates a problem with the posting of the semaphore
        if (i == 0) {
            return false;
        }
    }

    Sleep::Short(100,Units::ms);
    //Check if all the threads have terminated
    for (i = 0; i < Threads::NumberOfThreads(); i++) {
        ThreadIdentifier tid = Threads::FindByIndex(i);
        if (Threads::IsAlive(tid)) {
            Threads::Kill(tid);
        }
    }

    return true;
}

bool EventSemTest::TestWaitTimeoutNoTimeout(uint32 nOfThreads) {
    timeout = MilliSeconds::Infinite;
    return MultiThreadedTestWait(nOfThreads);
}

bool EventSemTest::TestWaitTimeoutSuccess(uint32 nOfThreads) {
    //Set the timeout to two seconds
    timeout = MilliSeconds(2000,Units::ms);
    return MultiThreadedTestWait(nOfThreads);
}

bool EventSemTest::TestWaitTimeoutFailure(uint32 nOfThreads) {
    //Set the timeout to 1 millisecond
    timeout = MilliSeconds(1,Units::ms);
    //This should fail as when the semaphore is posted the value of the shared
    //variable will no longer be 0xABCD...
    return !MultiThreadedTestWait(nOfThreads);
}

bool EventSemTest::TestWaitTimeoutFailureFollowedBySuccess(uint32 nOfThreads) {
    return TestWaitTimeoutFailure(nOfThreads) && TestWaitTimeoutNoTimeout(nOfThreads);
}

