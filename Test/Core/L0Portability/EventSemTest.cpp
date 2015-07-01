/**
 * @file EventSemTest.cpp
 * @brief Source file for class EventSemTest
 * @date 26/giu/2015
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
    timeout = TTInfiniteWait;
}

EventSemTest::~EventSemTest() {
    eventSem.Close();
    mutexSem.Close();
}

bool EventSemTest::TestCreate(){
    EventSem testSem;
    bool test = testSem.Create();
    if (test){
        test &= (testSem.Handle() != NULL);
    }
    testSem.Close();
    return test;
}

bool EventSemTest::TestClose(){
    EventSem testSem;
    testSem.Create();
    return testSem.Close();
}

bool EventSemTest::TestCopyConstructor(){
    EventSem testSem;
    testSem.Create();

    EventSem copySem(testSem);
    testSem.Reset();
    testSem.Wait(1);

    bool test = copySem.Post();
    test &= (testSem.Handle() == copySem.Handle());

    testSem.Close();
    copySem.Close();
    return test;
}

bool EventSemTest::TestWait(TimeoutType timeout) {

    Error myErrorType = Debug;


    EventSem newSem(eventSem);

    if (eventSem.Wait(timeout, myErrorType)) {
        return False;
    }

    if (myErrorType != Timeout) {
        return False;
    }

    myErrorType = Debug;

    if (newSem.ResetWait(timeout, myErrorType)) {
        return False;
    }
    return myErrorType == Timeout;

}

bool EventSemTest::TestPost() {
    if (!eventSem.Post()) {
        return False;
    }

    if (eventSem.Post()) {
        return False;
    }

    if (!eventSem.Reset()) {
        return False;
    }

    if (!eventSem.Post()) {
        return False;
    }

    return True;
}

void EventSemTestWait(EventSemTest &eventSemTest) {
    //Wait before proceeding...
    if (eventSemTest.timeout == TTInfiniteWait) {
        //Also test if passing no parameter is indeed TTInfiniteWait
        eventSemTest.eventSem.Wait();
    }
    else {
        eventSemTest.eventSem.Wait(eventSemTest.timeout);
    }

    eventSemTest.mutexSem.Lock();
    eventSemTest.sharedVariable++;
    eventSemTest.mutexSem.UnLock();

}

bool EventSemTest::MultiThreadedTestWait(uint32 nOfThreads) {
    //Initialised the value of the sharedVariable to some unexpected value.
    sharedVariable = 0xABCD;
    //Reset the semaphore
    eventSem.Reset();
    uint32 i = 0;
    for (i = 0; i < nOfThreads; i++) {
        //Each thread will try to increment the value of sharedVariable
        Threads::BeginThread((ThreadFunctionType) EventSemTestWait, this);
    }
    //Allow threads to start
    SleepMSec(100);
    //The semaphore was not posted yet, so the value of the sharedVariable should still be 0xABCD
    if (sharedVariable != 0xABCD) {
        return False;
    }
    //Reset the value of the sharedVariable
    sharedVariable = 0;
    eventSem.Post();
    //Wait some time for the test to succeed...
    while (sharedVariable != nOfThreads) {
        SleepMSec(100);
        i--;
        //Waited 20 ms x nOfThreads and the value of the sharedVariable is still
        //not nOfThreads which likely indicates a problem with the posting of the semaphore
        if (i == 0) {
            return False;
        }
    }

    SleepMSec(10);
    return True;
}

bool EventSemTest::WaitNoTimeoutTest(uint32 nOfThreads) {
    timeout = TTInfiniteWait;
    return MultiThreadedTestWait(nOfThreads);
}

bool EventSemTest::WaitTimeoutTestSuccess(uint32 nOfThreads) {
    //Set the timeout to two seconds
    timeout = 2000;
    return MultiThreadedTestWait(nOfThreads);
}

bool EventSemTest::WaitTimeoutTestFailure(uint32 nOfThreads) {
    //Set the timeout to 1 millisecond
    timeout = 1;
    //This should fail as when the semaphore is posted the value of the shared
    //variable will no longer be 0xABCD...
    return !MultiThreadedTestWait(nOfThreads);
}

bool EventSemTest::WaitTimeoutTestFailureFollowedBySuccess(uint32 nOfThreads) {
    return WaitTimeoutTestFailure(nOfThreads) && WaitNoTimeoutTest(nOfThreads);
}

