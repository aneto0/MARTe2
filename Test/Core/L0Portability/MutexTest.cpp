/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:$
 *
 **/

#include "GeneralDefinitions.h"
#include "MutexTest.h"


//It's used to implement Lock() or FastLock() functions depending by the boolean usingFast
bool MutexLock(MutexTest &mutexTest, int32 index) {
    if (!mutexTest.usingFast) {
        if (!mutexTest.mutexSem[index - 1].Lock(mutexTest.timeout)) {
            return False;
        }
    }
    else {
        if (!mutexTest.mutexSem[index - 1].FastLock(mutexTest.timeout)) {
            return False;
        }
    }
    return True;
}

//It's used to implement UnLock() or FastUnLock() functions depending by the boolean usingFast
bool MutexUnLock(MutexTest &mutexTest, int32 index) {

    if (!mutexTest.usingFast) {
        if (!mutexTest.mutexSem[index - 1].UnLock()) {
            return False;
        }
    }
    else {
        if (!mutexTest.mutexSem[index - 1].FastUnLock()) {
            return False;
        }
    }
    return True;
}

/**Increase the shared variable only if between an increment and the other the variable remains consistent, else return. This function acts differently depending on the boolean variables**/
void Increment(MutexTest &mutexTest) {
    //if free is True, the thread try to unlock the locked mutex
    if (mutexTest.free) {
        if (!mutexTest.mutexSem[1].FastTryLock()) {
            if (!MutexUnLock(mutexTest, 2)) {
                mutexTest.exitCondition++;
                return;
            }
        }
    }
    //Lock Mutex 1
    if (!MutexLock(mutexTest, 1)) {
        mutexTest.exitCondition++;
        return;
    }
    //Save the state of the shared variable before increments
    int32 state = mutexTest.sharedVariable;
    int32 i = 0;

    //if deadlock is True the mutex 2 is blocked (after checking by the value of exitCondition that the other thread blocked the mutex 2)
    if (mutexTest.deadlock) {
        mutexTest.exitCondition++;
        while (mutexTest.exitCondition < 2) {
            SleepSec(1e-3);
        }
        //If The Deadlock condition it's not managed, it remains in an infinite loop, otherwise the lock fail and it returns
        if (!MutexLock(mutexTest, 2)) {
            mutexTest.exitCondition++;
            return;
        }
    }
    //Increment shared variable of ten, sleep at each loop and returns if the variable is not consistent (it means that the lock does not work)
    while (i < 10) {
        if (mutexTest.sharedVariable != (state + i)) {
            mutexTest.exitCondition++;
            return;
        }
        mutexTest.sharedVariable++;
        i++;
        SleepSec(1e-3);
    }
    //increment the exit condition and unlock mutex 1
    mutexTest.exitCondition++;
    MutexUnLock(mutexTest, 1);
}

//It's used only to trigger the deadlock condition
void Decrement(MutexTest &mutexTest) {
    //Lock the Mutex 2
    if (!MutexLock(mutexTest, 2)) {
        mutexTest.exitCondition++;
        return;
    }

    mutexTest.exitCondition++;

    //wait that the other thread blocks the mutex 1 
    while (mutexTest.exitCondition < 2) {
        SleepSec(1e-3);
    }
    //try to lock mutex 1 triggering deadlock condition. 
    if (!MutexLock(mutexTest, 1)) {
        mutexTest.exitCondition++;
        return;
    }

    //decrement the shared variable
    mutexTest.sharedVariable--;

    mutexTest.exitCondition++;
    MutexUnLock(mutexTest, 1);
}

//It's used to test the mutex with timeouts
void TimeoutFunction(MutexTest &mutexTest) {

    int32 longExpire = (int32) (mutexTest.timeout.msecTimeout) * 4;
    int32 timeCounter = 0;
    //Lock mutex with timeout passed by argument
    if (!MutexLock(mutexTest, 1)) {
        //if the lock fail and the boolean expired begins True to allow to the first thread to break the following loop.
        mutexTest.expired = True;
    }
    //Only for the first thread, before that the mutex fail because the timeout.
    if (!mutexTest.expired) {
        //wait the expire of the timeout
        while (!mutexTest.expired) {
            SleepSec(1e-3);
            //it's incremented each (about) ms. 
            timeCounter++;
            //if a lot of time is elapsed, change the shared variable make it wrong. The result will be False because the timeout fail.
            if (timeCounter > longExpire) {
                mutexTest.sharedVariable = -1;
                break;
            }
        }
    }
    //This unlock works only if the timeout fails, otherwise there is nothing to unlock if the mutex fail.
    MutexUnLock(mutexTest, 1);
    //call increment function which returns the right value only if the shared variable is consistent.
    Increment(mutexTest);
}

//It's used to test the behavior of system when a thread is killed with a mutex active.
void WaitLoop(MutexTest &mutexTest) {

    //activate mutex.
    if (!MutexLock(mutexTest, 1)) {
        mutexTest.exitCondition++;
        return;
    }

    //increment the shared variable only to show that mutex does not fail.
    mutexTest.sharedVariable++;
    mutexTest.exitCondition++;
    int32 j = 0;
    //wait about 500 ms.
    while (j++ < 500) {
        SleepSec(1e-3);
    }
    //unlock mutex...by now this thread can be killed.
    MutexUnLock(mutexTest, 1);
    j = 0;
    //wait about 500 ms that the thrad is killed.
    while (j++ < 500) {
        SleepSec(1e-3);
    }
    mutexTest.exitCondition++;
}

//It launchs nOfThreads threads which call Increment function. 
bool MutexTest::TestSync(int32 nOfThreads) {
    sharedVariable = 0;
    timeout = TTInfiniteWait;
    exitCondition = 0;
    usingFast = False;
    expired = False;
    free = False;
    deadlock = False;
    for (int32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread((ThreadFunctionType) Increment, this);
    }
    int32 j=0;
    while (exitCondition < nOfThreads) {
        if(j++>10*nOfThreads){
            return False;
        }
        SleepSec(10e-3);
    }
    return (sharedVariable == 10 * nOfThreads);
}

//It launch nOfThreads threads on Timeout function.
bool MutexTest::TestSyncTimeout(TimeoutType timeout, int32 nOfThreads) {
    sharedVariable = 0;
    exitCondition = 0;
    this->timeout = timeout;
    usingFast = False;
    expired = False;
    free = False;
    deadlock = False;
    for (int32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread((ThreadFunctionType) TimeoutFunction, this);
    }
    uint32 j=0;
    while (exitCondition < nOfThreads) {
        if(j++>10*timeout.msecTimeout){
            return False;
        }
        SleepSec(10e-3);
    }
    return (sharedVariable == 10 * nOfThreads);
}

//It launchs nOfThreads threads on Increment function using fast mutex functions
bool MutexTest::TestSyncFast(int32 nOfThreads) {
    sharedVariable = 0;
    exitCondition = 0;
    usingFast = True;
    timeout = TTInfiniteWait;
    expired = False;
    free = False;
    deadlock = False;
    for (int32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread((ThreadFunctionType) Increment, this);
    }
    int32 j=0;
    while (exitCondition < nOfThreads) {
        if(j++>10*nOfThreads){
            return False;
        }
        SleepSec(10e-3);
    }
    return (sharedVariable == 10 * nOfThreads);
}

//It launchs nOfThreads threads on Timeout_function using fast mutex functions
bool MutexTest::TestSyncTimeoutFast(TimeoutType timeout, int32 nOfThreads) {
    sharedVariable = 0;
    exitCondition = 0;
    this->timeout = timeout;
    usingFast = True;
    expired = False;
    free = False;
    deadlock = False;
    for (int32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread((ThreadFunctionType) TimeoutFunction, this);
    }
    uint32 j=0;
    while (exitCondition < nOfThreads) {
        if(j++>10*timeout.msecTimeout){
            return False;
        }
        SleepSec(10e-3);
    }
    return (sharedVariable == 10 * nOfThreads);
}

//The main process lock the mutex and then it launchs a thread which try to unlock it.
bool MutexTest::FakeLock() {
    sharedVariable = 0;
    exitCondition = 0;
    timeout = TTInfiniteWait;
    usingFast = False;
    expired = False;
    free = True;
    deadlock = False;
    mutexSem[1].Lock();
    Threads::BeginThread((ThreadFunctionType) Increment, this);
    int32 j=0;
    while (exitCondition < 1) {
        if(j++>100){
            return False;
        }
        SleepSec(10e-3);
    }
    return (sharedVariable == 0);
}

//It launchs two threads, one call Increment and the other Decrement to trigger the deadlock condition.
bool MutexTest::DeadLock() {
    sharedVariable = 0;
    exitCondition = 0;
    timeout = TTInfiniteWait;
    usingFast = False;
    expired = False;
    free = False;
    deadlock = True;
    TID tid1 = Threads::BeginThread((ThreadFunctionType) Increment, this);
    TID tid2 = Threads::BeginThread((ThreadFunctionType) Decrement, this);
    int32 j = 0;
    while (exitCondition < 4) {
        if (j++ > 100)
            break;
        SleepSec(10e-3);
    }
    Threads::Kill(tid1);
    Threads::Kill(tid2);
    return (sharedVariable == 0);
}

//The main process launchs a thread which locks a mutex and, after it launchs another thread which wait on mutex. Then the main process kill the first thread. If the shared variable is 11 it means that the
//thread murder triggers a fail or an unlock of the mutex allowing operations of the second thread. 
bool MutexTest::KillWithLock() {

    sharedVariable = 0;
    exitCondition = 0;
    timeout = TTInfiniteWait;
    usingFast = False;
    expired = False;
    free = False;
    deadlock = False;
    TID tid1 = Threads::BeginThread((ThreadFunctionType) WaitLoop, this);
    int32 j = 0;
    while (exitCondition < 1) {
        if (j++ > 2000)
            return False;
        SleepSec(1e-3);
    }
    //Lock Fail
    if (sharedVariable == 0) {
        return False;
    }
    //Launch the second thread which remains blocked on mutex
    TID tid2 = Threads::BeginThread((ThreadFunctionType) Increment, this);

    //kill the first thread
    Threads::Kill(tid1);
    j=0;
    //wait that the second thread finish its job (or about two second max)
    while (exitCondition < 2) {
        if (j++ > 2000)
            break;
        SleepSec(1e-3);
    }

    //something wrong...kill second thread too
    if (exitCondition < 2) {
        Threads::Kill(tid2);
    }

    //return true if first thread was killed and second thread did his job correctly
    return (sharedVariable == 11) && j < 2000 && exitCondition < 3;
}

