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
#include "FastPollingMutexTest.h"


//It's used to implement FastLock() function
bool MutexLock(FastPollingMutexTest &fastMutexTest, int32 index) {

    if (!fastMutexTest.fastMutexSem[index - 1].FastLock(
            fastMutexTest.timeout)) {
        return False;
    }

    return True;
}

//It's used to implement FastUnLock() function
bool MutexUnLock(FastPollingMutexTest &fastMutexTest, int32 index) {

    if (!fastMutexTest.fastMutexSem[index - 1].FastUnLock()) {
        return False;
    }

    return True;
}

/**Increase the shared variable only if between an increment and the other the variable remains consistent, else return. This function acts differently depending on the boolean variables**/
void Increment(FastPollingMutexTest &fastMutexTest) {
    //if free is True, the thread try to unlock the locked mutex. This is possible using FastPollingMutexSem
    if (fastMutexTest.free) {
        if (!fastMutexTest.fastMutexSem[1].FastTryLock()) {
            if (!MutexUnLock(fastMutexTest, 2)) {
                fastMutexTest.exitCondition++;
                return;
            }
        }
    }
    //Lock Mutex 1
    if (!MutexLock(fastMutexTest, 1)) {
        fastMutexTest.exitCondition++;
        return;
    }
    //Save the state of the shared variable before increments
    int32 state = fastMutexTest.sharedVariable;
    int32 i = 0;

    //if deadlock is True the mutex 2 is blocked (after checking by the value of exitCondition that the other thread blocked the mutex 2)
    if (fastMutexTest.deadlock) {
        fastMutexTest.exitCondition++;
        while (fastMutexTest.exitCondition < 2) {
            SleepSec(1e-3);
        }
        //If The Deadlock condition it's not managed, it remains in an infinite loop, otherwise the lock fail and it returns
        if (!MutexLock(fastMutexTest, 2)) {
            fastMutexTest.exitCondition++;
            return;
        }
    }
    //Increment shared variable of ten, sleep at each loop and returns if the variable is not consistent (it means that the lock does not work)
    while (i < 10) {
        if (fastMutexTest.sharedVariable != (state + i)) {
            fastMutexTest.exitCondition++;
            return;
        }
        fastMutexTest.sharedVariable++;
        i++;
        SleepSec(1e-3);
    }
    //increment the exit condition and unlock mutex 1
    fastMutexTest.exitCondition++;
    MutexUnLock(fastMutexTest, 1);
}

//It's used only to trigger the deadlock condition
void Decrement(FastPollingMutexTest &fastMutexTest) {
    //Lock the Mutex 2
    if (!MutexLock(fastMutexTest, 2)) {
        fastMutexTest.exitCondition++;
        return;
    }

    fastMutexTest.exitCondition++;

    //wait that the other thread blocks the mutex 1 
    while (fastMutexTest.exitCondition < 2) {
        SleepSec(1e-3);
    }
    //try to lock mutex 1 triggering deadlock condition. 
    if (!MutexLock(fastMutexTest, 1)) {
        fastMutexTest.exitCondition++;
        return;
    }

    //decrement the shared variable
    fastMutexTest.sharedVariable--;

    fastMutexTest.exitCondition++;
    MutexUnLock(fastMutexTest, 1);
}

//It's used to test the behavior of system when a thread is killed with a mutex active and to test the lock with timeout
void InfiniteLoop(FastPollingMutexTest &fastMutexTest) {

    //activate mutex. If the lock fail because the timeout, the shared variable is incremented
    if (!MutexLock(fastMutexTest, 1)) {
        MutexLock(fastMutexTest, 2);
        fastMutexTest.sharedVariable++;
        fastMutexTest.exitCondition++;
        MutexUnLock(fastMutexTest, 2);
        return;
    }

    fastMutexTest.exitCondition++;

    while (1) {
        SleepSec(1e-3);
    }

    MutexUnLock(fastMutexTest, 1);
}

//It launchs nOfThreads threads which call Increment function. 
bool FastPollingMutexTest::TestSync(int32 nOfThreads) {
    sharedVariable = 0;
    timeout = TTInfiniteWait;
    exitCondition = 0;
    expired = False;
    free = False;
    deadlock = False;

    FastPollingMutexSem myFastMutex;
    myFastMutex.Create(True);
    if (!myFastMutex.Locked()) {
        return False;
    }
    myFastMutex.Close();

    for (int32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread((ThreadFunctionType) Increment, this);
    }
    int32 j = 0;
    while (exitCondition < nOfThreads) {
        if (j++ > 10 * nOfThreads) {
            return False;
        }
        SleepSec(10e-3);
    }
    return (sharedVariable == 10 * nOfThreads);
}

//It launchs nOfThreads threads, the first lock mutex and goes in an infinite loops, the others must wait the timeout and increment the shared variable after the lock fail.
bool FastPollingMutexTest::TestSyncTimeout(TimeoutType timeout,
                                           int32 nOfThreads) {
    sharedVariable = 0;
    exitCondition = 0;
    this->timeout = timeout;
    expired = False;
    free = False;
    deadlock = False;
    //launch the first thread and wait the lock.
    TID tidFirst = Threads::BeginThread((ThreadFunctionType) InfiniteLoop,
                                        this);
    uint32 j = 0;
    while (exitCondition < 1) {
        if (j++ > (10 * timeout.msecTimeout * nOfThreads)) {
            Threads::Kill(tidFirst);
            return False;
        }
        SleepSec(1e-3);
    }
    //return false if the lock fail
    if (sharedVariable > 0) {
        Threads::Kill(tidFirst);
        return False;
    }
    //launch other threads on the same function
    for (int32 i = 0; i < (nOfThreads - 1); i++) {
        Threads::BeginThread((ThreadFunctionType) InfiniteLoop, this);
    }
    j = 0;
    while (exitCondition < (nOfThreads)) {
        if (j++ > (10 * timeout.msecTimeout * nOfThreads)) {
            return False;
        }
        SleepSec(1e-3);
    }
    //kill the first thread and return true if successive threads incremented the variable.
    Threads::Kill(tidFirst);
    return (sharedVariable == (nOfThreads - 1));
}

//The main process locks the mutex and then it launchs a thread which try to unlock it. This is possible with FastPollingMutexSems
bool FastPollingMutexTest::FakeLock() {
    sharedVariable = 0;
    exitCondition = 0;
    timeout = TTInfiniteWait;
    expired = False;
    free = True;
    deadlock = False;
    fastMutexSem[1].FastLock();
    Threads::BeginThread((ThreadFunctionType) Increment, this);
    int32 j = 0;
    while (exitCondition < 1) {
        if (j++ > 10) {
            return False;
        }
        SleepSec(10e-3);
    }
    //return true if the unlock works
    return (sharedVariable == 10);
}

//It launchs two threads, one call Increment and the other Decrement to trigger the deadlock condition.
bool FastPollingMutexTest::DeadLock() {
    sharedVariable = 0;
    exitCondition = 0;
    timeout = TTInfiniteWait;
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

//The main process launchs a thread which locks a mutex and, after it launchs another thread which waits on mutex. Then the main process kills the first thread and unlock the mutex allowing
//that the operations of the second thread. If the shared variable is 10, it means that after kill the mutex remains locked, otherwise the function return false. 
bool FastPollingMutexTest::KillWithLock() {

    sharedVariable = 0;
    exitCondition = 0;
    timeout = TTInfiniteWait;
    expired = False;
    free = False;
    deadlock = False;
    TID tid1 = Threads::BeginThread((ThreadFunctionType) InfiniteLoop, this);
    int32 j = 0;
    while (exitCondition < 1) {
        if (j++ > 2000)
            return False;
        SleepSec(1e-3);
    }
    //Lock Fail
    if (sharedVariable > 0) {
        Threads::Kill(tid1);
        return False;
    }
    //Launch the second thread which remains blocked on mutex
    TID tid2 = Threads::BeginThread((ThreadFunctionType) Increment, this);

    SleepSec(10e-3);

    //kill the first thread
    Threads::Kill(tid1);

    //remember to unlock the mutex. If the mutex is already un-blocked return false because this is unexpected.
    if (!fastMutexSem[0].FastTryLock()) {
        fastMutexSem[0].FastUnLock();
    }
    else {
        Threads::Kill(tid2);
        return False;
    }
    j = 0;

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

    //return true if the second thread did its job correctly
    return (sharedVariable == 10);
}

