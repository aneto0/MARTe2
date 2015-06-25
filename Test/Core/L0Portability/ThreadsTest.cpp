/**
 * @file ThreadsTest.cpp
 * @brief Source file for class ThreadsTest
 * @date 25/06/2015
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

#include "ThreadsTest.h"
#include "Sleep.h"
#include "StringTestHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ThreadsTest::ThreadsTest() {
    exitCondition = 0;
    tidTest = 0;
    retValue = True;
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

bool ThreadsTest::TestBegin(const char* name,
                            uint32 stackSize) {
    TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE, name);

    int32 j = 0;

    //waits that the thread begins
    while (exitCondition < 1) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return False;
        }
        SleepSec(10e-3);

    }

    //checks if it is alive
    if (!Threads::IsAlive(tid)) {
        retValue = False;
    }

    //checks the name
    if (!StringTestHelper::Compare(Threads::Name(tid), name)) {
        retValue = False;
    }

    //allows the thread to exit
    exitCondition++;

    j = 0;
    //waits some time
    while (exitCondition < 3) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return False;
        }
        SleepSec(10e-3);

    }
    return retValue;
}

void EndTestFunction(ThreadsTest &tt) {

    //tells to the main process that the thread begins
    tt.exitCondition++;

    //end the thread
    Threads::EndThread();

    //if this code is executed there is an error.
    tt.exitCondition++;
}

bool ThreadsTest::TestEnd() {
    TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);
    int32 j = 0;

    //waits that the thread begins
    while (exitCondition < 1) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return False;
        }
        SleepSec(10e-3);

    }

    //kills the thread
    Threads::Kill(tid);
    SleepSec(1e-3);

    //error if the thread is still alive
    if (Threads::IsAlive(tid)) {
        exitCondition++;
        SleepSec(10e-3);
        return False;
    }

    exitCondition = 0;
    tid = Threads::BeginThread((ThreadFunctionType) EndTestFunction, this);
    //waits that the thread begins
    while (exitCondition < 1) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return False;
        }
        SleepSec(10e-3);

    }

    //the thread should exit before incrementing the exitCondition
    while (Threads::IsAlive(tid) && exitCondition < 2) {
        SleepSec(10e-3);
    }

    return exitCondition == 1;

}

bool ThreadsTest::TestPriority() {
    //seven available priority levels +1 for unknown priority level
    ThreadPriorityType allPrioLevelTypes[] = { PRIORITY_UNKNOWN, PRIORITY_IDLE, PRIORITY_LOWEST, PRIORITY_BELOW_NORMAL, PRIORITY_NORMAL, PRIORITY_ABOVE_NORMAL,
            PRIORITY_HIGHEST, PRIORITY_TIME_CRITICAL };

    //four available priority classes +1 for unknown priority class
    PriorityClassType allPrioClassTypes[] =
            { PRIORITY_CLASS_UNKNOWN, PRIORITY_CLASS_IDLE, PRIORITY_CLASS_NORMAL, PRIORITY_CLASS_HIGH, PRIORITY_CLASS_REAL_TIME };

    bool goOn = True;

    //Note: The PRIORITY_UNKNOWN cannot be setted. It is returned only in case of errors.

    //creates 40 threads with different prio classes and levels and checks them one by one
    for (uint32 i = 1; i < 5; i++) {
        for (uint32 j = 1; j < 8; j++) {
            exitCondition = 0;

            TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);

            int32 n = 0;

            //waits the thread beginning
            while (exitCondition < 1) {

                if (n++ > 100) {
                    Threads::Kill(tid);
                    return False;
                }
                SleepSec(10e-3);
            }

            //set prio level and class
            Threads::SetPriorityClass(tid, allPrioClassTypes[i]);
            Threads::SetPriorityLevel(tid, allPrioLevelTypes[j]);

            //gets prio level and class
            if (Threads::GetPriorityClass(tid) != allPrioClassTypes[i]) {
                goOn = False;
            }

            if (Threads::GetPriorityLevel(tid) != allPrioLevelTypes[j]) {
                goOn = False;
            }

            //end the thread
            exitCondition++;
            n = 0;
            //waits that the thread ends
            while (exitCondition < 3) {
                if (n++ > 100) {
                    Threads::Kill(tid);
                    return False;
                }
                SleepSec(10e-3);

            }

            if (!goOn) {
                return False;
            }
        }

    }

    TID fakeTid = (TID) 0;
    if (Threads::GetPriorityClass(fakeTid) != PRIORITY_CLASS_UNKNOWN || Threads::GetPriorityLevel(fakeTid) != PRIORITY_UNKNOWN) {
        return False;
    }
    return True;

}

bool ThreadsTest::TestState() {
    TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);
    int32 j = 0;

    //waits the thread beginning
    while (exitCondition < 1) {

        if (j++ > 100) {
            Threads::Kill(tid);
            return False;
        }
        SleepSec(10e-3);
    }

    bool ret = True;

    //in linux and windows the thread state function returns always unknown
    if (Threads::GetState(tid) != STATE_UNKNOWN) {
        ret = False;
    }

    //allows the thread to exit
    exitCondition++;
    //waits that the thread ends
    while (exitCondition < 3) {
        if (j++ > 100) {
            Threads::Kill(tid);
            return False;
        }
        SleepSec(10e-3);

    }
    return ret;

}

void TestIdFunction(ThreadsTest &tt) {
    if (Threads::Id() != tt.tidTest) {
        tt.retValue = False;
    }

    tt.exitCondition++;
}

bool ThreadsTest::TestId() {
    tidTest = Threads::BeginThread((ThreadFunctionType) TestIdFunction, this);

    int32 j = 0;
    //waits that the thread finishes
    while (exitCondition < 1) {

        if (j++ > 100) {
            Threads::Kill(tidTest);
            return False;
        }
        SleepSec(10e-3);
    }

    return retValue;

}

bool ThreadsTest::TestCPUs() {
    //start a thread without specifying a cpu.
    TID tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this);

    int32 j = 0;
    //waits that the thread begins
    while (exitCondition < 1) {

        if (j++ > 100) {
            Threads::Kill(tidTest);
            return False;
        }
        SleepSec(10e-3);
    }
    int32 nCPUs = Threads::GetCPUs(tid);

    exitCondition++;
    SleepSec(1e-3);

    //note: in windows the get cpus functions returns always -1
    for (int32 i = 1; i < nCPUs; i++) {
        exitCondition = 0;

        tid = Threads::BeginThread((ThreadFunctionType) DummyFunction, this, THREADS_DEFAULT_STACKSIZE, NULL, ExceptionHandler::NotHandled, i);
        int32 j = 0;
        while (exitCondition < 1) {

            if (j++ > 100) {
                Threads::Kill(tidTest);
                return False;
            }
            SleepSec(10e-3);
        }

        if (Threads::GetCPUs(tid) != i) {
            retValue = False;
        }

        j = 0;
        exitCondition++;
        //waits that the thread ends
        while (exitCondition < 3) {
            if (j++ > 100) {
                Threads::Kill(tid);
                return False;
            }
            SleepSec(10e-3);

        }

        if (retValue == False) {
            return False;
        }
    }

    return retValue;
}
