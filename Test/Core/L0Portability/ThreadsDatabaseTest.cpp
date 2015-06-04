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
#include "ThreadsDatabaseTest.h"

//return the number of thread's informations saved in the database
int32 nDatabasedThreads() {
    ThreadsDatabase::Lock();
    int32 rvalue = ThreadsDatabase::NumberOfThreads();
    ThreadsDatabase::UnLock();
    return rvalue;
}

//Infinite loop
void DoNothing(ThreadsDatabaseTest &threadDatabaseTest) {
    threadDatabaseTest.exitCondition++;
    SleepSec(1e-3);
    while (1) {
        SleepSec(1e-3);
    }
}

//return false if at least one thread does not match its tid in the database or if the exitCondition variable change in the locked block (this means that lock function does not work).
void GetInfo(ThreadsDatabaseTest &threadDatabaseTest) {

    if (!threadDatabaseTest.eventsem.Wait()) {
        threadDatabaseTest.exitCondition++;
        threadDatabaseTest.returnValue = False;
        return;
    }

    ThreadsDatabase::Lock();
    int32 state = threadDatabaseTest.exitCondition;
    threadDatabaseTest.returnValue = threadDatabaseTest.returnValue
            && ((threadDatabaseTest.threadInfo =
                    ThreadsDatabase::GetThreadInformation(Threads::Id()))
                    != NULL);

    if (!threadDatabaseTest.returnValue) {
        threadDatabaseTest.exitCondition++;
        ThreadsDatabase::UnLock();
        return;
    }

    if (threadDatabaseTest.threadInfo->threadId != Threads::Id()) {
        threadDatabaseTest.returnValue = False;
        threadDatabaseTest.exitCondition++;
        ThreadsDatabase::UnLock();
        return;
    }
    ThreadInformation controller;

    if (!ThreadsDatabase::GetInfo(controller, -1, Threads::Id())) {
        threadDatabaseTest.returnValue = False;
        threadDatabaseTest.exitCondition++;
        ThreadsDatabase::UnLock();
        return;
    }

    if (controller.threadId != threadDatabaseTest.threadInfo->threadId) {
        threadDatabaseTest.returnValue = False;
        threadDatabaseTest.exitCondition++;
        ThreadsDatabase::UnLock();
        return;
    }

    int32 n = ThreadsDatabase::NumberOfThreads();

    if (n > 0) {

        if (!ThreadsDatabase::GetInfo(controller, n - 1, -1)) {
            threadDatabaseTest.returnValue = False;
            threadDatabaseTest.exitCondition++;
            ThreadsDatabase::UnLock();
            return;
        }

        if (controller.threadId != ThreadsDatabase::GetThreadID(n - 1)) {
            threadDatabaseTest.returnValue = False;
            threadDatabaseTest.exitCondition++;
            ThreadsDatabase::UnLock();
            return;
        }
    }

    SleepSec(1e-3);

    threadDatabaseTest.exitCondition++;
    if (threadDatabaseTest.exitCondition != (state + 1)) {
        threadDatabaseTest.returnValue = False;
    }
    ThreadsDatabase::UnLock();

}

//The thread that calls this function, gets tids from database and kill the other threads. Then, checks if it remains alone in the database and exit.
void GetId(ThreadsDatabaseTest &threadDatabaseTest) {
    TID tid;
    for (int32 i = 0; i < threadDatabaseTest.tidsDim; i++) {
        threadDatabaseTest.returnValue = threadDatabaseTest.returnValue
                && ((tid = ThreadsDatabase::GetThreadID(0)) != 0);
        if (!threadDatabaseTest.returnValue) {
            break;
        }
        if (tid != Threads::Id()) {
            threadDatabaseTest.returnValue = threadDatabaseTest.returnValue
                    && Threads::Kill(tid);
        }
        if (!threadDatabaseTest.returnValue) {
            break;
        }
    }
    threadDatabaseTest.exitCondition++;
    threadDatabaseTest.returnValue = threadDatabaseTest.returnValue
            && nDatabasedThreads() == 1
            && ThreadsDatabase::GetThreadID(0) == Threads::Id();
}

//Only Increment exit condition after the lock. It's used to test the lock with timeout.
void DoNothings(void *myThreadDatabaseTest) {
    ThreadsDatabaseTest *threadDatabaseTest =
            (ThreadsDatabaseTest*) myThreadDatabaseTest;

    while (threadDatabaseTest->exitCondition < 1) {
        SleepSec(1e-3);
    }
    threadDatabaseTest->exitCondition++;
    while (threadDatabaseTest->exitCondition < 3) {
        SleepSec(1e-3);
    }
    ThreadsDatabase::Lock(threadDatabaseTest->timeout);
    threadDatabaseTest->exitCondition++;
    ThreadsDatabase::UnLock();
}

//It locks the mutex and goes in an infinite loop.
void LockTimeout(ThreadsDatabaseTest &threadDatabaseTest) {

    threadDatabaseTest.exitCondition++;
    while (threadDatabaseTest.exitCondition < 2) {
        SleepSec(1e-3);
    }
    ThreadsDatabase::Lock(threadDatabaseTest.timeout);
    threadDatabaseTest.exitCondition++;
    while (1) {
        SleepSec(1e-3);
    }
    ThreadsDatabase::UnLock();
}

void DoNothingsProtected(ThreadsDatabaseTest *threadDatabaseTest) {
    ThreadProtectedExecute(DoNothings, (void*) threadDatabaseTest, NULL);
}

//Test the goodness of the informations in the database and the efficiency of lock and unlock functions
bool ThreadsDatabaseTest::TestGetInfoAndLock(int32 nOfThreads) {
    exitCondition = 0;
    returnValue = True;
    if (!eventsem.Reset()) {
        return False;
    }
    for (int32 i = 0; i < nOfThreads; i++) {
        tids[i] = Threads::BeginThread((ThreadFunctionType) GetInfo, this);
    }
    SleepSec(10e-3);
    if (!eventsem.Post()) {
        for (int32 i = 0; i < nOfThreads; i++) {
            Threads::Kill(tids[i]);
        }
        return False;
    }

    int32 j = 0;
    while (exitCondition < nOfThreads) {
        if (j++ > 10 * nOfThreads) {
            for (int32 i = 0; i < nOfThreads; i++) {
                Threads::Kill(tids[i]);
            }
            returnValue = False;
            break;
        }
        SleepSec(100e-3);
    }

    return returnValue && (nDatabasedThreads() == 0);

}

//Test the behavior of the function RemoveEntry()
bool ThreadsDatabaseTest::TestRemoveEntry(int32 nOfThreads) {
    exitCondition = 0;
    returnValue = True;

    //Launch nOfThreads threads in an infinite loop
    for (int32 i = 0; i < nOfThreads; i++) {
        tids[i] = Threads::BeginThread((ThreadFunctionType) DoNothing, this);
        tidsDim++;
    }

    int32 j = 0;

    //wait that all threads begin
    while (exitCondition < nOfThreads) {
        if (j++ > 10 * nOfThreads) {
            for (int32 i = 0; i < tidsDim; i++) {
                Threads::Kill(tids[i]);
            }
            return False;
        }
        SleepSec(10e-3);
    }

    //for each thread...
    for (int32 i = 0; i < nOfThreads; i++) {
        //compare the tid in database with tids saved in the array,
        if ((threadInfo = ThreadsDatabase::GetThreadInformation(tids[i]))
                == NULL) {
            returnValue = False;
            break;
        }

        if (threadInfo->threadId != tids[i]) {
            returnValue = False;
            break;
        }

        //remove the informations of the thread from database
        if (ThreadsDatabase::RemoveEntry(tids[i]) == NULL) {
            returnValue = False;
            break;
        }

        //check if the informations were removed from database
        returnValue = returnValue
                && ThreadsDatabase::GetThreadInformation(tids[i]) == NULL
                && nDatabasedThreads() == (nOfThreads - i - 1);

        //re-add the informations in the database
        ThreadsDatabase::NewEntry(threadInfo);

        //kill the thread
        if (Threads::IsAlive(tids[i])) {
            returnValue = returnValue && Threads::Kill(tids[i]);
        }
        else {
            returnValue = False;
        }
        if (!returnValue)
            break;
    }

    //check if the database is empty
    if (returnValue)
        return returnValue && (nDatabasedThreads() == 0);
    else {
        for (int32 i = 0; i < tidsDim; i++) {
            if (Threads::IsAlive(tids[i])) {
                Threads::Kill(tids[i]);
            }
        }
        return False;
    }
}

//Test the behavior of the GetId function
bool ThreadsDatabaseTest::TestGetId(int32 nOfThreads) {
    exitCondition = 0;
    returnValue = True;
    tidsDim = 0;

    //Launching nOfThreads-1 threads in an infinite loop
    for (int32 i = 0; i < nOfThreads - 1; i++) {
        Threads::BeginThread((ThreadFunctionType) DoNothing, this);
        tidsDim++;
    }

    int32 j = 0;

    //wait that all threads begin
    while (exitCondition < nOfThreads - 1) {
        if (j++ > 10 * nOfThreads) {
            for (int32 i = 0; i < tidsDim; i++) {
                Threads::Kill(tids[i]);
            }
            return False;
        }
        SleepSec(10e-3);
    }

    tidsDim++;

    //launch a thread with GetId function. It obtains tids of the other threads from database and kill them all.
    Threads::BeginThread((ThreadFunctionType) GetId, this);

    j = 0;

    while (exitCondition < nOfThreads) {
        if (j++ > 10 * nOfThreads) {
            returnValue = False;
        }
        SleepSec(10e-3);
    }

    //check if the database is empty
    if (returnValue)
        return returnValue && (nDatabasedThreads() == 0);
    else {
        for (int32 i = 0; i < tidsDim; i++) {
            if (Threads::IsAlive(tids[i])) {
                Threads::Kill(tids[i]);
            }
        }
        return False;
    }
}

//Test the lock with timeout
bool ThreadsDatabaseTest::TestTimeoutLock(TimeoutType time) {
    timeout = time;
    exitCondition = 0;
    returnValue = False;

    //launching a thread which locks with timeout and goes in an infinite loop.
    TID tid1 = Threads::BeginThread((ThreadFunctionType) LockTimeout, this);

    //launching a thread which locks with timeout and then increment the exitCondition variable
    TID tid2 = Threads::BeginThread((ThreadFunctionType) DoNothingsProtected,
                                    this);
    uint32 j = 0;

    //wait until the second thread increments exitCondition or too much time is elapsed
    while (exitCondition < 4) {
        if (j++ > (4 * time.msecTimeout))
            break;
        SleepSec(1e-3);
    }

    //kill both threads and return false if too much time is elapsed
    if (exitCondition < 4) {
        if (Threads::IsAlive(tid2)) {
            Threads::Kill(tid2);
        }
        if (Threads::IsAlive(tid1)) {
            Threads::Kill(tid1);
        }
        return False;
    }

    //kill the first thread and return true if after the expire the second thread did his job.
    return Threads::Kill(tid1);
}

