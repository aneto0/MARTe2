/**
 * @file ThreadsTest.cpp
 * @brief Source file for class ThreadsTest
 * @date 23/06/2015
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
 * the class ThreadsTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "Sleep.h"
#include "ThreadsTest.h"
#include "StringTestHelper.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



ThreadsTest::ThreadsTest() {
    callbackTestSuccessful = False;
    incrementCounter = 0;

    eventSem.Create();
    eventSemEndManagement.Create();
    nOfThreads1 = 0;
    threadIdTarget = 0;
    falseId = (TID) 1;
    mutexSem.Create(False);
    nameThreadTest = "F4EThread";
}

ThreadsTest::~ThreadsTest() {
    eventSem.Close();
    eventSemEndManagement.Close();
    mutexSem.Close();

}

void ThreadsTestIncrementCounter(ThreadsTest &tt) {
    tt.incrementCounter++;
}

void ThreadsTestWaitIncrementCounter(ThreadsTest &tt) {
    SleepSec(0.02 * tt.nOfThreads1);
    tt.incrementCounter++;
}

ThreadPriorityType ConvertPriorityLevelNumberToPriorityLevelName(uint32 i) {
    if (i == 0) {
        return PRIORITY_UNKNOWN;
    }
    else if (i == 1) {
        return PRIORITY_IDLE;
    }
    else if (i == 2) {
        return PRIORITY_LOWEST;
    }
    else if (i == 3) {
        return PRIORITY_BELOW_NORMAL;
    }
    else if (i == 4) {
        return PRIORITY_NORMAL;
    }
    else if (i == 5) {
        return PRIORITY_ABOVE_NORMAL;
    }
    else if (i == 6) {
        return PRIORITY_HIGHEST;
    }
    else if (i == 7) {
        return PRIORITY_TIME_CRITICAL;
    }
    else {
        return PRIORITY_UNKNOWN;
    }
}

PriorityClassType ConvertPriorityClassNumberToPriorityClassName(uint32 i) {
    if (i == 0) {
        return PRIORITY_CLASS_UNKNOWN;
    }
    else if (i == 1) {
        return PRIORITY_CLASS_IDLE;
    }
    else if (i == 2) {
        return PRIORITY_CLASS_NORMAL;
    }
    else if (i == 3) {
        return PRIORITY_CLASS_HIGH;
    }
    else if (i == 4) {
        return PRIORITY_CLASS_REAL_TIME;
    }
    else {
        return PRIORITY_CLASS_UNKNOWN;
    }
}

void PrioritiesCallback(ThreadsTest &tt) {
    bool ok = True;
    TID threadId = Threads::Id();
    uint32 priorityClass = (uint32) Threads::GetPriorityClass(threadId);
    uint32 i = 1; //i=[1 7].priority level 0--> PRIORITY_UNKNOWN, 1--> PRIORITY_IDLE , 2--> PRIORITY_LOWEST...
    uint32 j = 1; //j=[1 4]. Priority Class. 0 --1 PRIORITY_CLASS_UNKNOWN...

    Threads::SetPriorityLevel(threadId, PRIORITY_LOWEST);
    //Verify that the class was not changed
    ok = ok && (priorityClass == (uint32) Threads::GetPriorityClass(threadId));
    //verify that the priority is as expected
    ok = ok && (PRIORITY_LOWEST == (Threads::GetPriorityLevel(threadId))); //Threads::GetPriorityLevel(threadId) is an int32
    while (i < 8) {
        //set the priority level to the level specified in priorityLevel
        Threads::SetPriorityLevel(threadId, ConvertPriorityLevelNumberToPriorityLevelName(i));
        //check that the priorityLevel is still what it is expected
        ok = ok && (i == ((uint32) Threads::GetPriorityLevel(threadId)));
        i++;
    }
    while (j < 4) {
        //set the priority level to the level specified in priorityLevel
        Threads::SetPriorityClass(threadId, ConvertPriorityClassNumberToPriorityClassName(j));
        // check that the priorityLevel is still what it is expected
        ok = ok && (j == ((uint32) Threads::GetPriorityClass(threadId)));
        j++;
    }
    tt.callbackTestSuccessful = ok;
    tt.eventSem.Post();
}

bool ThreadsTest::BeginThread(uint32 nOfThreads) {
    uint32 i = 0;
    for (i = 0; i < nOfThreads; i++) {
        //Each thread will increment incrementCounter and its value should arrive
        //to nOfThreads
        Threads::BeginThread((ThreadFunctionType) ThreadsTestIncrementCounter, this);
    }
    //Give some time for all the threads to have started...
    while (incrementCounter != (int32) nOfThreads) {
        SleepMSec(20);
        i--;
        //Waited 20 ms x nOfThreads and the value of the incrementCounter is still
        //Not nOfThreads which likely indicates a problem with the creation of the thread
        if (i == 0) {
            return False;
        }
    }
    return True;
}

bool ThreadsTest::Priorities() {
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) PrioritiesCallback, this);

    if (!eventSem.Wait()) {
        return False;
    }

    return callbackTestSuccessful;
}

void PriorityClassRunFirst(ThreadsTest &tt) {
    //This part of code is protected
    if (!tt.mutexSem.Lock()) {
        return;
    }
    bool ok = True;
    static int32 i = 1; //indicates priority classes;
    static int32 j = 1; //indicates subpriorities

    //increment the priority class
    if (j > 7) {
        i++;
        j = 1;
    }
    TID threadId = Threads::Id();
    Threads::SetPriorityClass(threadId, ConvertPriorityClassNumberToPriorityClassName(i));
    ThreadsSetPriorityLevel(threadId, ConvertPriorityLevelNumberToPriorityLevelName(j));
    j++;
    tt.incrementCounter++;
    //unlock
    if (!tt.mutexSem.UnLock()) {
        tt.callbackTestSuccessful = False;
        return;
    }

    if (!tt.eventSem.Wait()) {
        tt.callbackTestSuccessful = False;
        return;
    }
    if (!tt.mutexSem.Lock()) {
        tt.callbackTestSuccessful = False;
        return;
    }
    //Decrement counter in order to know when all the threads finish to run this part of code
    tt.incrementCounter--;
    j--;

    //decrement the priority class
    if (j < 1) {
        i--;
        j = 7;
    }
    ok = ok && (i == (int32) Threads::GetPriorityClass(threadId));
    ok = ok && (j == (int32) Threads::GetPriorityLevel(threadId));
    if (!tt.mutexSem.UnLock()) {
        tt.callbackTestSuccessful = False;
        return;
    }
    tt.callbackTestSuccessful = ok;
    return;
}

bool ThreadsTest::PrioritiesPropagationStartFirst() {
    int32 k = 0; //count the number of threads created.
    incrementCounter = 0;
    ThreadsTest::nOfThreads1 = 28; //7 levels x 4 classes
    eventSem.Reset();
    while (k < (int32) ThreadsTest::nOfThreads1) {
        Threads::BeginThread((ThreadFunctionType) PriorityClassRunFirst, this);
        k++;
    }
    //Wait for all the threads to be initialized and waiting
    while (ThreadsTest::incrementCounter < ThreadsTest::nOfThreads1) {
        SleepMSec(1);
    }
    //Ensure that the last thread are also waiting
    SleepMSec(100);
    //Allow all threads to start
    eventSem.Post();
    //Wait all the threads to finish
    while (ThreadsTest::incrementCounter > 0) {
        SleepMSec(1);
    }
    //Ensure that every thread has finished to work.
    SleepMSec(50);
    return callbackTestSuccessful;
}

void CheckIdDifferentTo0(ThreadsTest &tt) {
    TID threadId = 0;
    //Lock this part of code. This is not required because the threads are created when the previous thread is finished
    tt.mutexSem.Lock();
    threadId = Threads::Id();
    tt.callbackTestSuccessful = tt.callbackTestSuccessful && (0 != threadId);
    //it indicates the next threat can be created
    tt.eventSem.Post();
    //unlock
    tt.mutexSem.UnLock();
    return;
}

bool ThreadsTest::ThreadIdTest() {
    incrementCounter = 0;
    callbackTestSuccessful = True;
    nOfThreads1 = 10; //number of threads you want to create
    while (incrementCounter < nOfThreads1) {
        Threads::BeginThread((ThreadFunctionType) CheckIdDifferentTo0, this);
        SleepMSec(100);
        if (!eventSem.Wait()) {
            SleepSec(2.0);
            return False;
        }
        eventSem.Reset();
        incrementCounter++;
    }
    //Wait for all the threats to be initialized and waiting
    callbackTestSuccessful = callbackTestSuccessful && (ThreadsTest::nOfThreads1 == incrementCounter);
    return callbackTestSuccessful;
}

void AutoKillThread(ThreadsTest &tt) {
    //Obtain the Id of the Thread AutoKill
    tt.threadIdTarget = Threads::Id();
    // The thread is initialized and the main can continue.
    tt.eventSem.Post();
    //kill yourself
    tt.callbackTestSuccessful = ThreadsKill(tt.threadIdTarget);
    //Even the return of the ThreadsKill is true (thread killed), it is required some time to really kill it.
    SleepMSec(1);
    return;
}

void InfiniteLoop(ThreadsTest &tt) {
    //The mutex protects this region of code
    if (!tt.mutexSem.Lock()) {
        return;
    }
    tt.threadIdTarget = ThreadsId();

    if (!tt.mutexSem.UnLock()) {
        return;
    }
    //Initialization is done
    tt.eventSem.Post();
    //infinite loop
    while (1) {
        SleepMSec(100);
    }
    return;
}

void ThreadManagement(ThreadsTest &tt) {
    tt.falseId = (TID) - 1;
    bool ok = false;
    const char *PointerToString = "NULL";
    int32 i = 0;
    //Check if the thredTarget(INFINITE) is alive. Should be true
    ok = ThreadsIsAlive(tt.threadIdTarget);

    tt.callbackTestSuccessful = ok;
    //kill the Thread target
    ok = Threads::Kill(tt.threadIdTarget);
    tt.callbackTestSuccessful = tt.callbackTestSuccessful && ok;

    //To kill a thread it is required some time. A time out is implemented--> if in 5 cycles the ThreadsIsAlive is true then the thread never will die...
    while (ThreadsIsAlive(tt.threadIdTarget) && i < 5) {
        SleepMSec(1);
        i++;
    }
    //Check if the Thread target is alived. Expected value false.
    ok = ThreadsIsAlive(tt.threadIdTarget);

    tt.callbackTestSuccessful = tt.callbackTestSuccessful && !ok;

    //Try to kill the Thread again. Expected value false
    ok = ThreadsKill(tt.threadIdTarget);

    tt.callbackTestSuccessful = tt.callbackTestSuccessful && !ok;

    //Start again the  Thread target
    Threads::BeginThread((ThreadFunctionType) InfiniteLoop, &tt, THREADS_DEFAULT_STACKSIZE, tt.nameThreadTest, ExceptionHandler::NotHandled);
    if (!tt.eventSem.Wait()) {
        return;
    }
    //Reset eventSem in order to use in the future
    tt.eventSem.Reset();
    PointerToString = ThreadsName(tt.threadIdTarget);
    ok = StringTestHelper::Compare(PointerToString, tt.nameThreadTest);
    tt.callbackTestSuccessful = tt.callbackTestSuccessful && ok;

    ThreadsKill(tt.threadIdTarget);
    SleepMSec(1);

    //reset eventSem in order to use in the future
    tt.eventSem.Reset();
    //EndThread
    tt.eventSemEndManagement.Post();
    return;
}

bool ThreadsTest::ThreadKillTest() {
    eventSem.Reset();
    eventSemEndManagement.Reset();
    Threads::BeginThread((ThreadFunctionType) AutoKillThread, this, THREADS_DEFAULT_STACKSIZE, "AUTOKILL");
    //Waiting to initialize the thread
    if (!eventSem.Wait()) {
        SleepSec(2.0);
        return False;
    }
    //Reset eventSem in order to use again
    eventSem.Reset();
    //Waiting to kill the thread
    while (ThreadsIsAlive(threadIdTarget)) {
        SleepMSec(1);
    }
    Threads::BeginThread((ThreadFunctionType) InfiniteLoop, this, THREADS_DEFAULT_STACKSIZE, "INFINITE");
    //Waiting the initialization of the thread INFINITE
    if (!eventSem.Wait()) {
        SleepSec(2.0);
        return False;
    }
    //Reset eventSem in order to use again
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) ThreadManagement, this, THREADS_DEFAULT_STACKSIZE, "THREAD_MANAGEMETN");
    //Waiting thread management to run its on code
    if (!eventSemEndManagement.Wait()) {
        SleepSec(2.0);
        return False;
    }
    SleepSec(1.0);
    return callbackTestSuccessful;
}

bool ThreadsTest::ThreadNameTest() {
    falseId = (TID) - 1;
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) InfiniteLoop, this, THREADS_DEFAULT_STACKSIZE, nameThreadTest, ExceptionHandler::NotHandled);
    if (!eventSem.Wait()) {
        SleepSec(2.0);
        return False;
    }
    //reset eventSem in order to use in the future
    eventSem.Reset();
    //compare the expected name with the real name
    callbackTestSuccessful = (0 == strcmp(ThreadsName(threadIdTarget), nameThreadTest));
    //Check if with a false Id the ThreadsName returns null.
    if (ThreadsName(falseId) == 0) {    //expected case
        callbackTestSuccessful = callbackTestSuccessful && true;
    }
    else {    //unexpected case (returns false
        callbackTestSuccessful = callbackTestSuccessful && false;
    }
    SleepMSec(1);
    ThreadsKill(threadIdTarget);
    SleepMSec(1);
    return callbackTestSuccessful;
}

void EndTarget(ThreadsTest &tt) {
    tt.threadIdTarget = Threads::Id();
    //Main can continue
    tt.eventSem.Post();
    //Waiting THREAD_MANAGEMETN_END
    while (tt.incrementCounter < 1) {
        SleepMSec(10);
    }
    //THREAD_MANAGEMETN_END can continue
    tt.eventSem.Post();
    ThreadsEndThread();
    return;
}

void ThreadManagementEnd(ThreadsTest &tt) {
    bool ok;
    tt.callbackTestSuccessful = True;
    ok = ThreadsIsAlive(tt.threadIdTarget);
    tt.callbackTestSuccessful = tt.callbackTestSuccessful && ok;

    // Says to the EndTarget that can continue
    tt.incrementCounter++;
    if (!tt.eventSem.Wait()) {
        SleepSec(2.0);
        tt.callbackTestSuccessful = False;
        return;
    }
    //Wait EndTarget ends
    SleepSec(1.0);
    //Reset sem to use in the future
    tt.eventSem.Reset();
    ok = ThreadsIsAlive(tt.threadIdTarget);

    tt.callbackTestSuccessful = tt.callbackTestSuccessful && !ok;
    //IsAlive with a wrong Id
    //ThreadsIsAlive(tt.falseId);
    tt.eventSemEndManagement.Post();
    return;
}

bool ThreadsTest::ThreadEndTest() {
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) EndTarget, this, THREADS_DEFAULT_STACKSIZE, "\nEndTarget\n");
    //Waiting the initialization of the thread INFINITE
    if (!eventSem.Wait()) {
        SleepSec(1.0);
        return False;
    }
    //Reset eventSem in order to use again
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) ThreadManagementEnd, this, THREADS_DEFAULT_STACKSIZE, "THREAD_MANAGEMETN_END");
    //Waiting to the thread management to run its on code
    if (!eventSemEndManagement.Wait()) {
        SleepSec(1.0);
        return False;
    }
    callbackTestSuccessful = callbackTestSuccessful && !ThreadsIsAlive(threadIdTarget);
    return callbackTestSuccessful;
}

void CheckCpuRun(ThreadsTest &tt) {
    tt.callbackTestSuccessful = tt.callbackTestSuccessful && (tt.incrementCounter == ThreadsGetCPUs(Threads::Id()));
    tt.incrementCounter++;
    tt.eventSem.Post();
    return;
}
void GetNumberCPU(ThreadsTest &tt) {
    tt.nOfThreads1 = ThreadsGetCPUs(Threads::Id());
    tt.eventSem.Post();
}

bool ThreadsTest::CpuRunTest() {
    callbackTestSuccessful = true;
    //nOfThreads1 = 255;
    incrementCounter = 1;
    eventSem.Reset();

    //Get the number of available cpus for threads
    ThreadsBeginThread((ThreadFunctionType) GetNumberCPU, this, THREADS_DEFAULT_STACKSIZE, "NO_NAME", ExceptionHandler::NotHandled);
    eventSem.Wait();
    eventSem.Reset();
    while (incrementCounter < nOfThreads1) {
        ThreadsBeginThread((ThreadFunctionType) CheckCpuRun, this, THREADS_DEFAULT_STACKSIZE, "NO_NAME", ExceptionHandler::NotHandled, incrementCounter);
        if (!eventSem.Wait()) {
            SleepSec(2.0);
            return False;
        }
        eventSem.Reset();
    }
    return callbackTestSuccessful;
}
