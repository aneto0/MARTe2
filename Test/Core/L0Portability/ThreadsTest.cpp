/**
 * @file ThreadsTest.cpp
 * @brief Source file for class ThreadsTest
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
 * the class ThreadsTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ThreadsTest.h"
#include "Atomic.h"
#include "Sleep.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ThreadsTest::ThreadsTest() {
    exitCondition = 0;
    tidTest = 0;
    retValue = true;
}

void DummyFunction(ThreadsTest &tt) {

    //tells to the main process that the thread begins
    tt.exitCondition++;

    //waits the permission of the main process to exit
    while (tt.exitCondition < 2) {

        SleepSec(1e-3);
    }

    tt.exitCondition++;
}

bool ThreadsTest::TestBeginThread(const char8* name,
                                  uint32 stackSize,
                                  uint32 nOfThreads) {

    //test on thread number
    for (uint32 i = 0; i < nOfThreads; i++) {

        exitCondition = 0;
        //Calls the thread callback.
        TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, stackSize, name);

        int32 j = 0;

        //waits that the thread begins
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

        //allows the thread to exit
        exitCondition++;

        j = 0;
        //waits some time
        while (exitCondition < 3) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

    }

    exitCondition = 0;
    //launches a thread with 0 stacksize
    //the size associated to the thread should be the minimum possible (in linux is 16K)

    TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, 0, name);

    int32 j = 0;

    //waits that the thread begins
    while (exitCondition < 1) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return false;
        }
        SleepSec(10e-3);

    }

    //allows the thread to exit
    exitCondition++;

    j = 0;
    //waits some time
    while (exitCondition < 3) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return false;
        }
        SleepSec(10e-3);

    }

    //try to launch a thread with null function. The thread is created
    //but the function is not launched because is null (see ThreadInformation::UserThreadFunction)
    tid = Threads::BeginThread((ThreadFunctionType) NULL, this, THREADS_DEFAULT_STACKSIZE, name);

    return retValue && tid != (TID) 0;
}

void EndTestFunction(ThreadsTest &tt) {

    //tells to the main process that the thread begins
    tt.exitCondition++;

    //end the thread if exitCondition!=0
    if (tt.exitCondition) {
        Threads::EndThread();
    }

    //if this code is executed there is an error.
    tt.exitCondition++;
}

bool ThreadsTest::TestEndThread(uint32 nOfThreads) {

    for (uint32 i = 0; i < nOfThreads; i++) {

        exitCondition = 0;

        TID tid = Threads::BeginThread((ThreadFunctionType) EndTestFunction, this);

        int32 j = 0;
        //waits that the thread begins
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

        //the thread should exit before incrementing the exitCondition
        while (Threads::IsAlive(tid) && exitCondition < 2) {
            SleepSec(10e-3);
        }

        if (exitCondition != 1) {
            return false;
        }

    }

    //Obviously if EndThread is not called, the thread executes all the callback 
    for (uint32 i = 0; i < nOfThreads; i++) {

        exitCondition = -1;

        TID tid = Threads::BeginThread((ThreadFunctionType) EndTestFunction, this);

        int32 j = 0;
        //waits that the thread begins
        while (exitCondition < 0) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

        //the thread should exit before incrementing the exitCondition
        while (Threads::IsAlive(tid) && exitCondition < 1) {
            SleepSec(10e-3);
        }

        if (exitCondition != 1) {
            return false;
        }

    }

    return true;

}

bool ThreadsTest::TestIsAlive(uint32 nOfThreads) {

    for (uint32 i = 0; i < nOfThreads; i++) {

        exitCondition = 0;

        //Calls the thread callback.
        TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);

        int32 j = 0;

        //waits that the thread begins
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

        //checks if it is alive
        if (!Threads::IsAlive(tid)) {
            retValue = false;
        }

        //allows the thread to exit
        exitCondition++;

        j = 0;
        //waits some time
        while (exitCondition < 3) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }
        if (!retValue) {
            return false;
        }

    }

    //try IsAlive after a kill

    for (uint32 i = 0; i < nOfThreads; i++) {

        exitCondition = 0;

        //Calls the thread callback.
        TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);

        int32 j = 0;

        //waits that the thread begins
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

        Threads::Kill(tid);

        //checks if it is alive
        if (Threads::IsAlive(tid)) {
            retValue = false;
        }

        //allows the thread to exit if the thread is still alive
        exitCondition++;

        j = 0;
        //waits some time
        while (exitCondition < 3 && !retValue) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

        if (!retValue) {
            return false;
        }

    }

    return !Threads::IsAlive((TID) 0);
}

bool ThreadsTest::TestKill(uint32 nOfThreads) {

    //try to kill an invalid thread
    if (Threads::Kill((TID) 0)) {
        return false;
    }

    for (uint32 i = 0; i < nOfThreads; i++) {

        exitCondition = 0;

        TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);

        int32 j = 0;
        //waits that the thread begins
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

        //kills the thread

        if (!Threads::Kill(tid)) {
            return false;
        }
        SleepSec(1e-3);

        //try to kill again the same thread
        if (Threads::Kill(tid)) {
            return false;
        }

        //error if the thread is still alive
        if (Threads::IsAlive(tid)) {
            exitCondition++;
            SleepSec(10e-3);
            return false;
        }

    }

    return true;
}

bool PriorityTestFunction(ThreadsTest &t) {
    //seven available priority levels +1 for unknown priority level
    ThreadPriorityType allPrioLevelTypes[] = { PRIORITY_UNKNOWN, PRIORITY_IDLE, PRIORITY_LOWEST, PRIORITY_BELOW_NORMAL, PRIORITY_NORMAL, PRIORITY_ABOVE_NORMAL,
            PRIORITY_HIGHEST, PRIORITY_TIME_CRITICAL };

    //four available priority classes +1 for unknown priority class
    PriorityClassType allPrioClassTypes[] =
            { PRIORITY_CLASS_UNKNOWN, PRIORITY_CLASS_IDLE, PRIORITY_CLASS_NORMAL, PRIORITY_CLASS_HIGH, PRIORITY_CLASS_REAL_TIME };

    bool goOn = true;

    //Note: The PRIORITY_UNKNOWN cannot be set. It is returned only in case of errors.

    //creates 40 threads with different priority classes and levels and checks them one by one
    for (uint32 i = 1; i < 5; i++) {
        for (uint32 j = 1; j < 8; j++) {
            t.exitCondition = 0;

            TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, &t);

            int32 n = 0;

            //waits the thread beginning
            while (t.exitCondition < 1) {

                if (n++ > 100) {
                    Threads::Kill(tid);
                    return false;
                }
                SleepSec(10e-3);
            }

            //set priority level and class
            Threads::SetPriorityClass(tid, allPrioClassTypes[i]);
            Threads::SetPriorityLevel(tid, allPrioLevelTypes[j]);

            //gets priority level and class
            if (Threads::GetPriorityClass(tid) != allPrioClassTypes[i]) {
                goOn = false;
            }

            if (Threads::GetPriorityLevel(tid) != allPrioLevelTypes[j]) {
                goOn = false;
            }

            //end the thread
            t.exitCondition++;
            n = 0;
            //waits that the thread ends
            while (t.exitCondition < 3) {
                if (n++ > 100) {
                    Threads::Kill(tid);
                    return false;
                }
                SleepSec(10e-3);

            }

            if (!goOn) {
                return false;
            }
        }

    }

    TID fakeTid = (TID) 0;
    if (Threads::GetPriorityClass(fakeTid) != PRIORITY_CLASS_UNKNOWN || Threads::GetPriorityLevel(fakeTid) != PRIORITY_UNKNOWN) {
        return false;
    }

    return true;
}

bool ThreadsTest::TestPriorityLevel() {

    Threads::SetPriorityLevel((TID) -1, PRIORITY_HIGHEST);
    if (Threads::GetPriorityLevel((TID) -1) != PRIORITY_UNKNOWN) {
        return false;
    }

    return PriorityTestFunction(*this);

}

bool ThreadsTest::TestPriorityClass() {

    Threads::SetPriorityClass((TID) -1, PRIORITY_CLASS_HIGH);
    if (Threads::GetPriorityClass((TID) -1) != PRIORITY_CLASS_UNKNOWN) {
        return false;
    }

    return PriorityTestFunction(*this);
}

bool ThreadsTest::TestGetState() {
    TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);
    int32 j = 0;

    //waits the thread beginning
    while (exitCondition < 1) {

        if (j++ > 100) {
            Threads::Kill(tid);
            return false;
        }
        SleepSec(10e-3);
    }

    bool ret = true;

    //in linux and windows the thread state function returns always unknown
    if (Threads::GetState(tid) != STATE_UNKNOWN) {
        ret = false;
    }

    //allows the thread to exit
    exitCondition++;
    //waits that the thread ends
    while (exitCondition < 3) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return false;
        }
        SleepSec(10e-3);

    }
    return ret;

}

void TestIdFunction(ThreadsTest &tt) {
    if (Threads::Id() != tt.tidTest) {
        tt.retValue = false;
    }

    tt.exitCondition++;
}

bool ThreadsTest::TestId(uint32 nOfThreads) {

    for (uint32 i = 0; i < nOfThreads; i++) {

        exitCondition = 0;
        tidTest = Threads::BeginThread((ThreadFunctionType) TestIdFunction, this);

        int32 j = 0;
        //waits that the thread finishes
        while (exitCondition < 1) {

            if (j++ > 100) {
                Threads::Kill(tidTest);
                return false;
            }
            SleepSec(10e-3);
        }

        if (!retValue) {
            return false;
        }
    }

    return true;

}

bool ThreadsTest::TestGetCPUs() {
    //start a thread without specifying a cpu.
    TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);

    int32 j = 0;
    //waits that the thread begins
    while (exitCondition < 1) {

        if (j++ > 100) {
            Threads::Kill(tidTest);
            return false;
        }
        SleepSec(10e-3);
    }
    int32 nCPUs = Threads::GetCPUs(tid);

    exitCondition++;
    SleepSec(1e-3);

    //note: in windows the get cpus function returns always -1
    for (int32 i = 1; i < nCPUs; i++) {
        exitCondition = 0;

        tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE, NULL, ExceptionHandler::NotHandled, i);
        int32 j = 0;
        while (exitCondition < 1) {

            if (j++ > 100) {
                Threads::Kill(tidTest);
                return false;
            }
            SleepSec(10e-3);
        }

        if (Threads::GetCPUs(tid) != i) {
            retValue = false;
        }

        j = 0;
        exitCondition++;
        //waits that the thread ends
        while (exitCondition < 3) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }

        if (retValue == false) {
            return false;
        }
    }

    return retValue;
}

bool ThreadsTest::TestName(const char8* name,
                           uint32 nOfThreads) {

    for (uint32 i = 0; i < nOfThreads; i++) {

        exitCondition = 0;
        TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE, name);
        int32 j = 0;
        while (exitCondition < 1) {

            if (j++ > 100) {
                Threads::Kill(tidTest);
                return false;
            }
            SleepSec(10e-3);
        }

        if (Threads::Name(tid) == NULL) {
            retValue = false;
        }
        else {

            if (StringHelper::Compare(name, Threads::Name(tid)) != 0) {
                retValue = false;
            }

        }
        exitCondition++;

        //waits that the thread ends
        while (exitCondition < 3) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            SleepSec(10e-3);

        }
        if (!retValue) {
            retValue = true;
        }

    }

    //try to put a null name
    exitCondition = 0;
    uint32 j = 0;

    TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE, NULL);
    while (exitCondition < 1) {

        if (j++ > 100) {
            Threads::Kill(tidTest);
            return false;
        }
        SleepSec(10e-3);
    }
    if (StringHelper::Compare(Threads::Name(tid), "Unknown") != 0) {

        retValue = false;
    }

    exitCondition++;
    j = 0;
    //waits that the thread ends
    while (exitCondition < 3) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return false;
        }
        SleepSec(10e-3);

    }

    return retValue;
}

void WaitFunction(ThreadsTest &tt) {

    Atomic::Increment(&(tt.exitCondition));

    while (tt.exitCondition > -1) {
        SleepSec(1e-3);
    }

    Atomic::Decrement(&(tt.exitCondition));

}

bool ThreadsTest::TestNumberOfThreads(uint32 nOfThreads) {

    if (Threads::NumberOfThreads() != 0) {
        return false;
    }

    for (uint32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread((ThreadFunctionType) WaitFunction, this);

    }

    //waits that threads begin
    uint32 j = 0;
    while ((uint32) exitCondition < (nOfThreads)) {
        if (j++ > 10 * nOfThreads) {
            exitCondition = -1;
            SleepSec(1.0);
            return false;
        }
        SleepSec(10e-3);
    }

    //check
    if (Threads::NumberOfThreads() != nOfThreads) {
        retValue = false;
    }
    exitCondition = -1;

    //wait the threads termination
    j = 0;
    while ((uint32) exitCondition > (-nOfThreads - 1)) {
        if (j++ > 10 * nOfThreads) {
            return false;
        }
        SleepSec(10e-3);
    }

    return retValue && Threads::NumberOfThreads() == 0;
}

bool ThreadsTest::TestFindByIndex(uint32 nOfThreads) {

    uint32 i = 0;

    for (i = 0; i < nOfThreads; i++) {

        TID tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);

        //waits that threads begin
        uint32 j = 0;
        while ((uint32) exitCondition < (i + 1)) {
            if (j++ > 10 * nOfThreads) {
                exitCondition = -1;
                SleepSec(1.0);
                return false;
            }
            SleepSec(10e-3);
        }

        //check
        if (Threads::FindByIndex(i) != tid) {
            retValue = false;
            i++;
            break;
        }

    }

    exitCondition = -1;

    //let exit the threads
    uint32 j = 0;
    while ((uint32) exitCondition > (-i - 1)) {
        if (j++ > 10 * nOfThreads) {
            return false;
        }
        SleepSec(10e-3);
    }

    if (!retValue) {
        return false;
    }



    if (THREADS_DATABASE_GRANULARITY >= 2) {
        //second test
        exitCondition = 0;

        TID firstTid;
        TID lastTid;

        for (i = 0; i < THREADS_DATABASE_GRANULARITY; i++) {

            switch (i) {

            case 0:

                firstTid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);
                break;

            case THREADS_DATABASE_GRANULARITY - 1:
                lastTid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);
                break;

            default:
                Threads::BeginThread((ThreadFunctionType) WaitFunction, this);
            }

            //waits that threads begin
            uint32 j = 0;
            while ((uint32) exitCondition < (i + 1)) {
                if (j++ > 10 * nOfThreads) {
                    exitCondition = -1;
                    SleepSec(1.0);
                    return false;
                }
                SleepSec(10e-3);
            }

        }

        //removes the first thread
        Threads::Kill(firstTid);
        Atomic::Decrement(&exitCondition);

        //removes the last thread
        Threads::Kill(lastTid);
        Atomic::Decrement(&exitCondition);

        //adds another thread
        TID tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);


        //waits that the thread begins
        j = 0;
        while ((uint32) exitCondition < (i - 1)) {
            if (j++ > 10 * nOfThreads) {
                exitCondition = -1;
                SleepSec(1.0);
                return false;
            }
            SleepSec(10e-3);
        }

        //the next position is at the end but there are THREADS_DATABASE_GRANULARITY - 1 threads!
        if (Threads::FindByIndex(THREADS_DATABASE_GRANULARITY - 2) != tid) {
            retValue = false;
        }

        tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);


        //waits that the thread begins
        j = 0;
        while ((uint32) exitCondition < i) {
            if (j++ > 10 * nOfThreads) {
                exitCondition = -1;
                SleepSec(1.0);
                return false;
            }
            SleepSec(10e-3);
        }

        //the next position is at the beginning!
        if (Threads::FindByIndex(0) != tid) {
            retValue = false;
        }

        exitCondition = -1;

        //let exit the threads
        j = 0;
        while ((uint32) exitCondition > (-i - 1)) {
            if (j++ > 10 * nOfThreads) {
                return false;
            }
            SleepSec(10e-3);
        }

    }
    return retValue;

}

bool ThreadsTest::TestGetThreadInfoCopy(uint32 nOfThreads,
                                        const char8* name) {

    uint32 i = 0;

    ThreadInformation ti;

    for (i = 0; i < nOfThreads; i++) {

        TID tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this, THREADS_DEFAULT_STACKSIZE, name);

        //waits that the thread begins
        uint32 j = 0;
        while ((uint32) exitCondition < (i + 1)) {

            if (j++ > 10 * nOfThreads) {
                exitCondition = -1;
                SleepSec(1.0);
                return false;
            }
            SleepSec(10e-3);
        }

        if (!Threads::GetThreadInfoCopy(ti, -1, tid)) {
            retValue = false;
        }

        if (StringHelper::Compare(name, ti.ThreadName()) != 0) {
            retValue = false;
        }

        if (ti.threadId != tid) {
            retValue = false;
        }

        //by default the priority is normal
        if (ti.priorityClass != PRIORITY_CLASS_NORMAL) {
            retValue = false;
        }

        if (ti.priorityLevel != PRIORITY_NORMAL) {
            retValue = false;
        }

        Threads::SetPriorityLevel(tid, PRIORITY_HIGHEST);

        Threads::SetPriorityClass(tid, PRIORITY_CLASS_HIGH);

        Threads::GetThreadInfoCopy(ti, -1, tid);

        if (ti.priorityClass != PRIORITY_CLASS_HIGH) {
            retValue = false;
        }

        if (ti.priorityLevel != PRIORITY_HIGHEST) {
            retValue = false;
        }

        //if the index is >=0 the function search by index in the database
        ThreadInformation tiCopy;
        ThreadInformation tiCopy2;
        if (!Threads::GetThreadInfoCopy(tiCopy, i, tid)) {
            retValue = false;
        }

        if (!Threads::GetThreadInfoCopy(tiCopy2, i, (TID) 0)) {
            retValue = false;
        }

        if (StringHelper::Compare(name, tiCopy.ThreadName()) != 0 || StringHelper::Compare(name, tiCopy2.ThreadName()) != 0) {
            retValue = false;
        }

        if (tiCopy.threadId != tid || tiCopy2.threadId != tid) {
            retValue = false;
        }

        if (tiCopy.priorityClass != PRIORITY_CLASS_HIGH || tiCopy2.priorityClass != PRIORITY_CLASS_HIGH) {
            retValue = false;
        }

        if (tiCopy.priorityLevel != PRIORITY_HIGHEST || tiCopy2.priorityLevel != PRIORITY_HIGHEST) {
            retValue = false;
        }

        if (!retValue) {
            i++;
            break;
        }

    }

    exitCondition = -1;

    //let exit the threads
    uint32 j = 0;
    while ((uint32) exitCondition > (-i - 1)) {
        if (j++ > 10 * nOfThreads) {
            return false;
        }
        SleepSec(10e-3);
    }

    if (!retValue) {
        return false;
    }

    //creates a copy
    ThreadInformation tiCopy(ti);

    if (StringHelper::Compare(tiCopy.ThreadName(), ti.ThreadName()) != 0) {
        return false;
    }

    if (tiCopy.threadId != ti.threadId) {
        return false;
    }

    if (tiCopy.priorityClass != ti.priorityClass) {
        return false;
    }

    if (tiCopy.priorityLevel != ti.priorityLevel) {
        return false;
    }

    //creates another copy
    ThreadInformation tiCopy2;

    tiCopy2 = tiCopy;

    if (StringHelper::Compare(tiCopy2.ThreadName(), ti.ThreadName()) != 0) {
        return false;
    }

    if (tiCopy2.threadId != ti.threadId) {
        return false;
    }

    if (tiCopy2.priorityClass != ti.priorityClass) {
        return false;
    }

    if (tiCopy2.priorityLevel != ti.priorityLevel) {
        return false;
    }

    return !Threads::GetThreadInfoCopy(ti, -1, (TID) 0);

}

bool ThreadsTest::TestFindByName(uint32 nOfThreads,
                                 const char8 *name,
                                 uint32 position) {
    uint32 i = 0;
    if (position > nOfThreads) {
        position = nOfThreads;
    }

    TID tidTest = 0;
    TID tid = 0;

    for (i = 0; i < nOfThreads; i++) {

        if (i == position) {
            tidTest = Threads::BeginThread((ThreadFunctionType) WaitFunction, this, THREADS_DEFAULT_STACKSIZE, name);
        }
        else {
            if (tid == 0) {
                tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);
            }
            else {
                Threads::BeginThread((ThreadFunctionType) WaitFunction, this);

            }
        }
        //waits that the thread begins
        uint32 j = 0;
        while ((uint32) exitCondition < (i + 1)) {
            if (j++ > 10 * nOfThreads) {
                exitCondition = -1;
                SleepSec(1.0);
                return false;
            }
            SleepSec(10e-3);
        }

    }

    if (tidTest != Threads::FindByName(name)) {
        retValue = false;
    }

    if (tid != Threads::FindByName("Unknown")) {
        retValue = false;
    }

    exitCondition = -1;

    //let exit the threads
    uint32 j = 0;
    while ((uint32) exitCondition > (-i - 1)) {
        if (j++ > 10 * nOfThreads) {
            return false;
        }
        SleepSec(10e-3);
    }

    return retValue && Threads::FindByName(NULL) == (TID) 0;

}

void DoNothing(void *t) {
    ((ThreadsTest*) t)->exitCondition++;
}

void DoNothingProtected(ThreadsTest &t) {
    t.exitCondition++;
    ExceptionHandler ehi;
    Threads::ProtectedExecute(DoNothing, (void*) &t, &ehi);
}

bool ThreadsTest::TestProtectedExecute() {

    TID tid = Threads::BeginThread((ThreadFunctionType) DoNothingProtected, this);

    uint32 j = 0;
    while (exitCondition < 2) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return false;
        }
        SleepSec(10e-3);
    }

    return exitCondition == 2;
}

