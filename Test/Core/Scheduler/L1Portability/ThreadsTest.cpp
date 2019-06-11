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
#include "ThreadInformation.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
//extern uint32 nOfEntries=0;
ThreadsTest::ThreadsTest() {
    exitCondition = 0;
    tidTest = 0;
    retValue = true;
}

static void DummyFunction(ThreadsTest *tt) {
    //tells to the main process that the thread begins
    Atomic::Increment(&tt->exitCondition);
    //waits the permission of the main process to exit
    while (tt->exitCondition < 2) {
        Sleep::Short(1,Units::ms);
        Sleep::Short(1,Units::ms);
    }
    Atomic::Increment(&tt->exitCondition);
    Threads::EndThread();
}

bool ThreadsTest::TestBeginThread(CCString name,
                                  uint32 stackSize,
                                  int32 nOfThreads) {

    //test on thread number
    for (int32 i = 0; i < nOfThreads; i++) {

        exitCondition = 0;
        //Calls the thread callback.
        //Note: in windows if the stack size is zero, the thread is initialized with the default stack size.
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, stackSize, name);
        int32 j = 0;

        //Waits for the thread to begin
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);

                return false;
            }
            Sleep::Short(10,Units::ms);
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
            Sleep::Short(10,Units::ms);
        }
    }
    return true;
}

bool ThreadsTest::TestEndThread(CCString name,
                                uint32 stackSize,
                                int32 nOfThreads) {
    return TestBeginThread(name, stackSize, nOfThreads);

}

bool ThreadsTest::TestBeginThreadNullFunction(CCString name) {
//try to launch a thread with null function. The thread is created
//but the function is not launched because is null (see ThreadInformation::UserThreadFunction)
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) NULL, this, THREADS_DEFAULT_STACKSIZE, name);

    return tid != (ThreadIdentifier) 0;
}

bool ThreadsTest::TestIsAlive(int32 nOfThreads) {

    for (int32 i = 0; i < nOfThreads; i++) {
        exitCondition = 0;
        //Calls the thread callback.
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);
        int32 j = 0;
        //waits for the thread to begin
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            Sleep::Short(10,Units::ms);
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
            Sleep::Short(10,Units::ms);
        }
        if (!retValue) {
            return false;
        }
    }
    return retValue;
}

bool ThreadsTest::TestIsAliveAfterkill(int32 nOfThreads) {
    //try IsAlive after a kill
    for (int32 i = 0; i < nOfThreads; i++) {
        exitCondition = 0;
        //Calls the thread callback.
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);
        int32 j = 0;
        //Waits for the thread to begin
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            Sleep::Short(10,Units::ms);
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
            Sleep::Short(10,Units::ms);
        }
        if (!retValue) {
            return false;
        }
    }
    return retValue;
}

bool ThreadsTest::TestKillInvalidID() {
    //try to kill an invalid thread
    return !(Threads::Kill(InvalidThreadIdentifier));
}

bool ThreadsTest::TestKill(int32 nOfThreads) {
    for (int32 i = 0; i < nOfThreads; i++) {
        exitCondition = 0;
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);
        int32 j = 0;
        //Waits for the thread to begin
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return false;
            }
            Sleep::Short(10,Units::ms);
        }
        //kills the thread
        if (!Threads::Kill(tid)) {
            return false;
        }
        //Sleep::Short(1,Units::ms);
        //try to kill again the same thread
        if (Threads::Kill(tid)) {
            return false;
        }
        //error if the thread is still alive
        if (Threads::IsAlive(tid)) {
            exitCondition++;
            Sleep::Short(10,Units::ms);
            return false;
        }
    }
    return true;
}

bool PriorityTestFunction(ThreadsTest &t) {

    ThreadIdentifier fakeTid = (ThreadIdentifier) 0;
    if (Threads::GetPriorityClass(fakeTid) != Threads::UnknownPriorityClass || Threads::GetPriorityLevel(fakeTid) != 0) {
        return false;
    }

    //Available priority classes
    Threads::PriorityClassType allPrioClassTypes[] = { Threads::UnknownPriorityClass, Threads::IdlePriorityClass, Threads::NormalPriorityClass,
            Threads::RealTimePriorityClass };
    bool goOn = true;
    //Creates all the necessary thread combinations with different priority classes and levels and checks
    //that each of them has the correct priority class and level
    for (uint32 i = 1; i < 4; i++) {
        for (uint8 prio = 0; prio < 16; prio++) {
            t.exitCondition = 0;
            ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, &t);
            int32 n = 0;
            //waits the thread beginning
            while (t.exitCondition < 1) {
                if (n++ > 100) {
                    Threads::Kill(tid);
                    return false;
                }
                Sleep::Short(10,Units::ms);
            }

            //set priority level and class
            Threads::SetPriority(tid, allPrioClassTypes[i], prio);

            //gets priority level and class
            if (Threads::GetPriorityClass(tid) != allPrioClassTypes[i]) {
                goOn = false;
            }

            if (Threads::GetPriorityLevel(tid) != prio) {
                goOn = false;
            }
            //end the thread
            Atomic::Increment(&t.exitCondition);
            n = 0;
            //waits that the thread ends
            while (t.exitCondition < 3) {
                if (n++ > 100) {
                    Threads::Kill(tid);
                    return false;
                }
                Sleep::Short(10,Units::ms);
            }
            if (!goOn) {
                return false;
            }
        }
    }

    return true;
}

bool ThreadsTest::TestPriority() {
    return PriorityTestFunction(*this);
}

bool ThreadsTest::TestGetState() {
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);
    int32 j = 0;
    //waits the thread to begin
    while (exitCondition < 1) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return false;
        }
        Sleep::Short(10,Units::ms);
    }
    bool ret = true;
//in linux and windows the thread state function returns always unknown
    if (Threads::GetState(tid) != Threads::UnknownThreadStateType) {
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
        Sleep::Short(10,Units::ms);
    }
    return ret;
}

static void TestIdFunction(ThreadsTest *tt) {
    while (tt->tidTest == 0) {
        Sleep::Short(1,Units::ms);
    }
    if (Threads::Id() != tt->tidTest) {
        tt->retValue = false;
    }
    tt->exitCondition++;
    Threads::EndThread();

}

bool ThreadsTest::TestId(int32 nOfThreads) {
    for (int32 i = 0; i < nOfThreads; i++) {
        exitCondition = 0;
        tidTest = 0;
        tidTest = Threads::BeginThread((ThreadFunctionType) TestIdFunction, this);
        int32 j = 0;
        //waits that the thread finishes
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tidTest);
                return false;
            }
            Sleep::Short(10,Units::ms);
        }
        if (!retValue) {
            return false;
        }
    }
    return true;
}

bool ThreadsTest::TestGetCPUs() {
    //start a thread without specifying a cpu.
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);
    int32 j = 0;
    //waits that the thread begins
    while (exitCondition < 1) {

        if (j++ > 100) {
            Threads::Kill(tidTest);
            return false;
        }
        Sleep::Short(10,Units::ms);
    }
    uint32 nCPUs = Threads::GetCPUs(tid);
    exitCondition++;
    Sleep::Short(1,Units::ms);
    //note: in windows the get cpus function returns always -1

    for (uint32 i = 1; i < nCPUs; i++) {
        exitCondition = 0;
        tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE, emptyString, ExceptionHandler::NotHandled, i);
        int32 j = 0;
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tidTest);
                return false;
            }
            Sleep::Short(10,Units::ms);
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
            Sleep::Short(10,Units::ms);
        }

        if (retValue == false) {
            return false;
        }
    }
    return retValue;
}

bool ThreadsTest::TestName(CCString name,int32 nOfThreads) {

    for (int32 i = 0; i < nOfThreads; i++) {
        exitCondition = 0;
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE, name);
        int32 j = 0;
        while (exitCondition < 1) {
            if (j++ > 100) {
                Threads::Kill(tidTest);
                return false;
            }
            Sleep::Short(10,Units::ms);
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
            Sleep::Short(10,Units::ms);
        }

    }
    return retValue;
}

bool ThreadsTest::TestNameNull() {
    //try to put a null name
    exitCondition = 0;
    uint32 j = 0;
    ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE );
    while (exitCondition < 1) {
        if (j++ > 100) {
            Threads::Kill(tidTest);
            return false;
        }
        Sleep::Short(10,Units::ms);
    }
    if (StringHelper::Compare(Threads::Name(tid), "Unknown") != 0) {
        retValue = false;
    }
    exitCondition++;
    j = 0;
    //waits for the thread to end
    while (exitCondition < 3) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return false;
        }
        Sleep::Short(10,Units::ms);
    }
    return retValue;
}

static void WaitFunction(ThreadsTest *tt) {
    Atomic::Increment(&(tt->exitCondition));
    while (tt->exitCondition != 0) {
        Sleep::Short(1,Units::ms);
    }
    Atomic::Increment(&(tt->exitCondition));
    Threads::EndThread();
}

bool ThreadsTest::CheckThreadStart(int32 N){
    //Wait for the threads to begin
    int32 j = 0;
    while (exitCondition < N) {
        if (j > (10 * N)) {
            exitCondition = 0;
            Sleep::Short(1,Units::s);
            return false;
        }
        j++;
        Sleep::Short(10,Units::ms);
    }
	return true;
}

bool ThreadsTest::CheckThreadTermination(int32 N){
    int32 j = 0;
    while (exitCondition < N) {
        if (j++ > (10 * N)) {
            return false;
        }
        Sleep::Short(10,Units::ms);
    }
    return true;
}

bool ThreadsTest::TestNumberOfThreads(int32 nOfThreads) {
	exitCondition = 0;
    if (Threads::NumberOfThreads() != 0) {
        return false;
    }
    for (int32 i = 0; i < nOfThreads; i++) {
        Threads::BeginThread((ThreadFunctionType) WaitFunction, this);
    }

    if (!CheckThreadStart(nOfThreads)){
    	return false;
    }

    //check
    if (static_cast<int32>(Threads::NumberOfThreads()) != nOfThreads) {
        retValue = false;
    }
    exitCondition = 0;
    //wait the threads termination
    CheckThreadTermination(nOfThreads);

    Sleep::Short(10,Units::ms);
    return retValue && Threads::NumberOfThreads() == 0;
}

bool ThreadsTest::TestFindByIndex(int32 nOfThreads) {
	exitCondition = 0;
    int32 i = 0;
    for (i = 0; i < nOfThreads; i++) {
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);

        if (!CheckThreadStart(i+1)){
        	return false;
        }

        //check
        if (Threads::FindByIndex(static_cast<uint32>(i)) != tid) {
            retValue = false;
            i++;
            break;
        }
    }

    exitCondition = 0;

    //wait the threads termination
    CheckThreadTermination(nOfThreads);

    if (!retValue) {
        return false;
    }

    exitCondition = 0;
    if (THREADS_DATABASE_GRANULARITY >= 2) {
        //second test
        ThreadIdentifier firstTid;
        ThreadIdentifier lastTid;
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

            if (!CheckThreadStart(i+1)){
            	return false;
            }

        }

        //removes the first thread
        Threads::Kill(firstTid);
        Sleep::Short(10,Units::ms);

        Atomic::Decrement(&exitCondition);
        //removes the last thread
        Threads::Kill(lastTid);
        Sleep::Short(10,Units::ms);

        Atomic::Decrement(&exitCondition);
        //adds another thread
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);

        if (!CheckThreadStart(THREADS_DATABASE_GRANULARITY-1)){
        	return false;
        }

        //the next position is at the end but there are THREADS_DATABASE_GRANULARITY - 1 threads!
        if (Threads::FindByIndex(THREADS_DATABASE_GRANULARITY - 1) != tid) {
            retValue = false;
        }

        tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);

        if (!CheckThreadStart(THREADS_DATABASE_GRANULARITY)){
        	return false;
        }

        //the next position is at the beginning!
        if (Threads::FindByIndex(0) != tid) {
            retValue = false;
        }

        exitCondition = 0;
        //wait the threads termination
        CheckThreadTermination(nOfThreads);

    }
    return retValue;
}

bool ThreadsTest::TestGetThreadInfoCopy(int32 nOfThreads,
                                        CCString name) {
    int32 i = 0;
    ThreadInformation ti;
    for (i = 0; i < nOfThreads; i++) {
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this, THREADS_DEFAULT_STACKSIZE, name);
        //waits that the thread begins
        if (!CheckThreadStart(i+1)){
        	return false;
        }

        if (!Threads::GetThreadInfoCopy(ti, tid)) {
            retValue = false;
        }

        if (StringHelper::Compare(name, ti.ThreadName()) != 0) {
            retValue = false;
        }
        if (ti.GetThreadIdentifier() != tid) {
            retValue = false;
        }

        //by default the priority is normal
        if (ti.GetPriorityClass() != Threads::NormalPriorityClass) {
            retValue = false;
        }
        if (ti.GetPriorityLevel() != 0) {
            retValue = false;
        }

        Threads::SetPriority(tid, Threads::IdlePriorityClass, 15);
        Threads::GetThreadInfoCopy(ti, tid);
        if (ti.GetPriorityClass() != Threads::IdlePriorityClass) {
            retValue = false;
        }
        if (ti.GetPriorityLevel() != 15) {
            retValue = false;
        }

        //A priority greater than 16 should be set to 15
        Threads::SetPriority(tid, Threads::NormalPriorityClass, 16);
        Threads::GetThreadInfoCopy(ti, tid);
        if (ti.GetPriorityLevel() != 15) {
            retValue = false;
        }

        //if the index is >=0 the function search by index in the database
        ThreadInformation tiCopy;
        ThreadInformation tiCopy2;
        if (!Threads::GetThreadInfoCopy(tiCopy, tid)) {
            retValue = false;
        }

        if (!Threads::GetThreadInfoCopy(tiCopy2, static_cast<uint32>(i))) {
            retValue = false;
        }

        if ((StringHelper::Compare(name, tiCopy.ThreadName()) != 0) || (StringHelper::Compare(name, tiCopy2.ThreadName()) != 0)) {
            retValue = false;
        }

        if ((tiCopy.GetThreadIdentifier() != tid) || (tiCopy2.GetThreadIdentifier() != tid)) {
            retValue = false;
        }

        if ((tiCopy.GetPriorityClass() != Threads::NormalPriorityClass) || (tiCopy2.GetPriorityClass() != Threads::NormalPriorityClass)) {
            retValue = false;
        }

        if ((tiCopy.GetPriorityLevel() != 15) || (tiCopy2.GetPriorityLevel() != 15)) {
            retValue = false;
        }

        if (!retValue) {
            i++;
            break;
        }

    }

    exitCondition = 0;
    //wait the threads termination
    CheckThreadTermination(nOfThreads);

    return retValue;
}

bool ThreadsTest::TestGetThreadInfoCopyInvalidID() {
    ThreadInformation ti;
    return !Threads::GetThreadInfoCopy(ti, (ThreadIdentifier) 0);
}

bool ThreadsTest::TestFindByName(int32 nOfThreads,
                                 CCString name,
                                 int32 position) {
    int32 i = 0;
    if (position > nOfThreads) {
        position = nOfThreads;
    }
    ThreadIdentifier tidTest = 0;
    ThreadIdentifier tid = 0;
    for (i = 0; i < nOfThreads; i++) {
        if (i == position) {
            tidTest = Threads::BeginThread((ThreadFunctionType) WaitFunction, this, THREADS_DEFAULT_STACKSIZE, name);
        }
        else {
        	ThreadIdentifier ttid = Threads::BeginThread((ThreadFunctionType) WaitFunction, this);
            if (tid == 0) {
                tid = ttid;
            }
        }

        if (!CheckThreadStart(i+1)){
        	return false;
        }
    }

    if (tidTest != Threads::FindByName(name)) {
        retValue = false;
    }
    if (tid != Threads::FindByName("Unknown")) {
        retValue = false;
    }
    exitCondition = 0;

    //wait the threads termination
    CheckThreadTermination(nOfThreads);

    return retValue && Threads::FindByName(NULL) == InvalidThreadIdentifier;

}

