/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the 
 Licence.
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied.
 * See the Licence  
 permissions and limitations under the Licence.
 *
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/

#include "GeneralDefinitions.h"
#include "Sleep.h"
#include "ThreadsTest.h"
#include "Threads.h"
#include "stdio.h"

void ThreadsTestIncrementCounter(ThreadsTest &tt) {
    tt.incrementCounter++;
}

void ThreadsTestWaitIncrementCounter(ThreadsTest &tt) {
    SleepSec(0.02 * tt.nOfThreads1);
    tt.incrementCounter++;
}

uint32 ConvertPriorityLevelNumberToPriorityLevelName(uint32 i) {
    if (i == 0) {
        return Threads::PRIORITY_UNKNOWN;
    }
    else if (i == 1) {
        return Threads::PRIORITY_IDLE;
    }
    else if (i == 2) {
        return Threads::PRIORITY_LOWEST;
    }
    else if (i == 3) {
        return Threads::PRIORITY_BELOW_NORMAL;
    }
    else if (i == 4) {
        return Threads::PRIORITY_NORMAL;
    }
    else if (i == 5) {
        return Threads::PRIORITY_ABOVE_NORMAL;
    }
    else if (i == 6) {
        return Threads::PRIORITY_HIGHEST;
    }
    else if (i == 7) {
        return Threads::PRIORITY_TIME_CRITICAL;
    }
    else {
        return 1000;
    }
}

uint32 ConvertPriorityClassNumberToPriorityClassName(uint32 i) {
    if (i == 0) {
        return Threads::PRIORITY_CLASS_UNKNOWN;
    }
    else if (i == 1) {
        return Threads::PRIORITY_CLASS_IDLE;
    }
    else if (i == 2) {
        return Threads::PRIORITY_CLASS_NORMAL;
    }
    else if (i == 3) {
        return Threads::PRIORITY_CLASS_HIGH;
    }
    else if (i == 4) {
        return Threads::PRIORITY_CLASS_REAL_TIME;
    }
    else {
        return 1000;
    }
}

void PrioritiesCallback(ThreadsTest &tt) {
    bool ok = True;
    TID threadId = Threads::Id();
    uint32 priorityClass = Threads::GetPriorityClass(threadId);
    uint32 priorityLevel = Threads::PRIORITY_UNKNOWN;
    uint32 i = 0; //i=[0 7].priority level 0--> PRIORITY_UNKNOWN, 1--> PRIORITY_IDLE , 2--> PRIORITY_LOWEST...
    uint32 j = 0; //j=[0 4]. Priority Class. 0 --1 PRIORITY_CLASS_UNKNOWN...

    Threads::SetPriorityLevel(threadId, Threads::PRIORITY_LOWEST);
    //Verify that the class was not changed
    ok = ok && ((int32) priorityClass == Threads::GetPriorityClass(threadId));
    //verify that the priority is as expected
    ok = ok
            && (Threads::PRIORITY_LOWEST
                    == ((uint32) Threads::GetPriorityLevel(threadId))); //Threads::GetPriorityLevel(threadId) is an int32
    while (i < 8) {
        //Convert a number to priority level name in order to use the defined convention in Threads.h.
        priorityLevel = ConvertPriorityLevelNumberToPriorityLevelName(i);
        //set the priority level to the level specified in priorityLevel
        Threads::SetPriorityLevel(threadId, priorityLevel);
        //check that the priorityLevel is still what it is expected
        ok = ok
                && (priorityLevel
                        == ((uint32) Threads::GetPriorityLevel(threadId)));
        i++;
    }
    while (j < 4) {
        //Convert a number to priority class name in order to use the defined convention in Threads.h.
        priorityClass = ConvertPriorityClassNumberToPriorityClassName(j);
        //set the priority level to the level specified in priorityLevel
        Threads::SetPriorityClass(threadId, priorityLevel);
        // check that the priorityLevel is still what it is expected
        ok = ok
                && (priorityLevel
                        == ((uint32) Threads::GetPriorityClass(threadId)));
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
        Threads::BeginThread((ThreadFunctionType) ThreadsTestIncrementCounter,
                             this);
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
        printf("\n\nCan not lock PriorityClassRunFirst\n");
    }
    bool ok = true;
    static int32 i = 0; //indicates priority classes;
    static int32 j = 0; //indicates subpriorities

    if (j >= 8) {
        i++;
        j = 0;
    }
    TID threadId = Threads::Id();
    Threads::SetPriorityClass(threadId, i);
    ThreadsSetPriorityLevel(threadId, j);
    j++;
    tt.incrementCounter++;
    //unlock
    if (!tt.mutexSem.UnLock()) {
        printf("Can not unlock PriorityClassRunFirst");
    }

    if (!tt.eventSem.Wait()) {
        printf("\n\nCan not wait in a Sem\n");
    }
    if (!tt.mutexSem.Lock()) {
        printf("\n\nCan not lock\n");
    }
    //Decrement counter in order to know when all the threads finish to run this part of code
    tt.incrementCounter--;
    j--;
    if (j < 0) {
        i--;
        j = 7;
    }
    ok = ok && (i == Threads::GetPriorityClass(threadId));
    ok = ok && (j == Threads::GetPriorityLevel(threadId));
    if (!tt.mutexSem.UnLock()) {
        printf("Can not unlock");
    }
    tt.callbackTestSuccessful = ok;
    return;
}

bool ThreadsTest::PrioritiesPropagationStartFirst() {
    int32 k = 0; //count the number of threads created.
    incrementCounter = 0;
    ThreadsTest::nOfThreads1 = 40;
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
    callbackTestSuccessful = true;
    nOfThreads1 = 10; //number of threads you want to create
    while (incrementCounter < nOfThreads1) {
        Threads::BeginThread((ThreadFunctionType) CheckIdDifferentTo0, this);
        SleepMSec(100);
        if (!eventSem.Wait()) {
            printf("Impossible to wait initialization CheckIdDifferentTo0");
        }
        eventSem.Reset();
        incrementCounter++;
    }
    //Wait for all the threats to be initialized and waiting
    callbackTestSuccessful = callbackTestSuccessful
            && (ThreadsTest::nOfThreads1 == incrementCounter);
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
    SleepMSec(1e-6);
    printf("\n\nThis message never should appear\n\n");
    return;
}

void InfiniteLoop(ThreadsTest &tt) {
    //The mutex protects this region of code
    if (!tt.mutexSem.Lock()) {
        printf("Fatal error lock");
    }
    tt.threadIdTarget = ThreadsId();
    //Initialization is done
    tt.eventSem.Post();
    if (!tt.mutexSem.UnLock()) {
        printf("Fatal error unlock");
    }
    //infinite loop
    while (1) {
        SleepMSec(100);
    }
    return;
}

void ThreadManagement(ThreadsTest &tt) {
    tt.falseId = (TID) - 1;
    bool ok = false;
    char const *PointerToString = "NULL";
    int32 i = 0;
    //Check if the thredTarget(INFINITE) is alive. Should be true
    ok = ThreadsIsAlive(tt.threadIdTarget);
    tt.callbackTestSuccessful = ok;
    //kill the Thread target
    ok = ThreadsKill(tt.threadIdTarget);
    //SleepMSec(1);
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
    Threads::BeginThread((ThreadFunctionType) InfiniteLoop, &tt,
    THREADS_DEFAULT_STACKSIZE,
                         tt.nameThreadTest, ExceptionHandler::NotHandled);
    if (!tt.eventSem.Wait()) {
        printf("Impossible to wait the thread in THREAD_MANAGENT");
    }
    //Reset eventSem in order to use in the future
    tt.eventSem.Reset();
    PointerToString = ThreadsName(tt.threadIdTarget);
    ok = (0 == strcmp(PointerToString, tt.nameThreadTest));
    tt.callbackTestSuccessful = tt.callbackTestSuccessful && ok;
    ThreadsKill(tt.threadIdTarget);
    SleepMSec(1);
    //ThreadsKill(tt.falseId);
    //ThreadsIsAlive(tt.falseId);
    //reset eventSem in order to use in the future
    tt.eventSem.Reset();
    //EndThread
    tt.eventSemEndManagement.Post();
    return;
}

bool ThreadsTest::ThreadKillTest() {
    eventSem.Reset();
    eventSemEndManagement.Reset();
    Threads::BeginThread((ThreadFunctionType) AutoKillThread, this,
    THREADS_DEFAULT_STACKSIZE,
                         "AUTOKILL");
    //Waiting to initialize the thread
    if (!eventSem.Wait()) {
        printf("Impossible to wait initialization AUTOKILL");
    }
    //Reset eventSem in order to use again
    eventSem.Reset();
    //Waiting to kill the thread
    while (ThreadsIsAlive(threadIdTarget)) {
        SleepMSec(1);
    }
    Threads::BeginThread((ThreadFunctionType) InfiniteLoop, this,
    THREADS_DEFAULT_STACKSIZE,
                         "INFINITE");
    //Waiting the initialization of the thread INFINITE
    if (!eventSem.Wait()) {
        printf("Impossible to wait initialization INFINITE");
    }
    //Reset eventSem in order to use again
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) ThreadManagement, this,
    THREADS_DEFAULT_STACKSIZE,
                         "THREAD_MANAGEMETN");
    //Waiting thread management to run its on code
    if (!eventSemEndManagement.Wait()) {
        printf("Impossible to wait end THREAD_MANAGEMETN");
    }
    SleepSec(1.0);
    return callbackTestSuccessful;
}

bool ThreadsTest::ThreadNameTest() {
    falseId = (TID) - 1;
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) InfiniteLoop, this,
    THREADS_DEFAULT_STACKSIZE,
                         nameThreadTest, ExceptionHandler::NotHandled);
    if (!eventSem.Wait()) {
        printf("Impossible to wait the thread in thread F4Ethread");
    }
    //reset eventSem in order to use in the future
    eventSem.Reset();
    //compare the expected name with the real name
    callbackTestSuccessful = (0
            == strcmp(ThreadsName(threadIdTarget), nameThreadTest));
    //Check if with a false Id the ThreadsName returns null.
    // if ThreadsName(falseId) is NULL the strcmp command doesn't work for this reason first is checked if ThreadsName(falseId) is false
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
    SleepSec(1.0);
    printf("\n\nThis message never will appear\n\n");
    SleepSec(10.0);
    printf("\n\nThis message never will appear\n\n");
    return;
}

void ThreadManagementEnd(ThreadsTest &tt) {
    bool ok;
    tt.callbackTestSuccessful = true;
    ok = ThreadsIsAlive(tt.threadIdTarget);
    tt.callbackTestSuccessful = tt.callbackTestSuccessful && ok;
    // Says to the EndTarget that can continue
    tt.incrementCounter++;
    if (!tt.eventSem.Wait()) {
        printf("\nThreadManagmentEnd cannot not wait\n");
    }
    //Wait EndTarget ends
    SleepSec(1.1);
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
    Threads::BeginThread((ThreadFunctionType) EndTarget, this,
    THREADS_DEFAULT_STACKSIZE,
                         "\nEndTarget\n");
    //Waiting the initialization of the thread INFINITE
    if (!eventSem.Wait()) {
        printf("\nImpossible to wait initialization EndTarget\n");
    }
    //Reset eventSem in order to use again
    eventSem.Reset();
    Threads::BeginThread((ThreadFunctionType) ThreadManagementEnd, this,
    THREADS_DEFAULT_STACKSIZE,
                         "THREAD_MANAGEMETN_END");
    //Waiting to the thread management to run its on code
    if (!eventSemEndManagement.Wait()) {
        printf("Impossible to wait end THREAD_MANAGEMETN_END");
    }
    callbackTestSuccessful = callbackTestSuccessful
            && !ThreadsIsAlive(threadIdTarget);
    return callbackTestSuccessful;
}

void CheckCpuRun(ThreadsTest &tt) {
    tt.callbackTestSuccessful = tt.callbackTestSuccessful
            && (tt.incrementCounter == ThreadsGetCPUs(Threads::Id()));
    tt.incrementCounter++;
    tt.eventSem.Post();
    return;
}
void GetNumberCPU(ThreadsTest &tt){
    tt.nOfThreads1= ThreadsGetCPUs(Threads::Id());
    tt.eventSem.Post();
}

bool ThreadsTest::CpuRunTest() {
    callbackTestSuccessful = true;
    //nOfThreads1 = 255;
    incrementCounter = 1;
    eventSem.Reset();

    ThreadsBeginThread((ThreadFunctionType) GetNumberCPU, this,
            THREADS_DEFAULT_STACKSIZE,
                               "NO_NAME", ExceptionHandler::NotHandled);
    eventSem.Wait();
    eventSem.Reset();
    while (incrementCounter < nOfThreads1) {
        ThreadsBeginThread((ThreadFunctionType) CheckCpuRun, this,
        THREADS_DEFAULT_STACKSIZE,
                           "NO_NAME", ExceptionHandler::NotHandled,
                           incrementCounter);
        if (!eventSem.Wait()) {
            printf("can not wait");
        }
        eventSem.Reset();
    }
    return callbackTestSuccessful;
}

