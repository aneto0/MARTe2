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
 * $Id:$
 *
 **/
/**
 * @file ThreadsDatabaseTest.h
 * @brief Tests the ThreadsDatabase and associated functions.
 *
 * The test consists in observing if the database returns the correct informations foe each thread and if it remoce information when they exit.
 */

#ifndef THREADS_DATABASE_TEST_H
#define THREADS_DATABASE_TEST_H

/** the maximum number of tids which can be saved **/
#define MAX_NUM_THREADS 192

#include "ThreadsDatabase.h"
#include "Threads.h"
#include "Sleep.h"
#include "EventSem.h"

class ThreadsDatabaseTest {

private:

public:

    /** used as a return value of the thread function **/
    bool returnValue;

    /** used by the main process to control when all threads exit **/
    int32 exitCondition;

    /** used to control if the return value of the ThreadsDatabase is correct **/
    ThreadInformation* threadInfo;

    /**an array to save tids of various threads **/
    TID tids[MAX_NUM_THREADS];

    /** the number of valid tids in the array **/
    int32 tidsDim;

    /** timeout for the lock with timeout **/
    TimeoutType timeout;

    /** semaphore for threads syncronization. */
    EventSem eventsem;

    /** @brief Constructor. */
    ThreadsDatabaseTest() {
        returnValue = True;
        exitCondition = 0;
        threadInfo = NULL;
        tidsDim = 0;
        timeout = TTInfiniteWait;
        eventsem.Create();
    }

    /**
     * @brief Tests the efficiency of the Lock function of the database and compare the thread information returned by database with the thread's attributes.
     * @param nOfThreads number of threads to launch.
     * @return true if the exit condition value remains consistent and all the information received by database are correct. False otherwise. **/
    bool TestGetInfoAndLock(int32 nOfThreads);

    /**
     * @brief Tests the GetId() function of the database. A thread receives the tid of the other threads in database and compare it with the saved tids in the array.
     * Then, it kills the other threads and
     * check if they are removed from the database.
     * @param nOfThreads number of threads to launch.
     * @return true if the returned tids are correct and after kills all thread's information are removed from database, false otherwise. **/
    bool TestGetId(int32 nOfThreads);

    /**
     * @brief Launchs nOfThreads threads and for each thread the main process control before that its tid is in the database, then remove the entry from the database 
     * and check that the informations are not in the database anymore.
     * @param nOfThreads numbero of threads to launch.
     * @return true if the thread information pointer for each thread is in the database before the removing, and it's NULL after, false otherwise. **/
    bool TestRemoveEntry(int32 nOfThreads);

    /**
     * @brief Tests the lock of the database with a provided timeout. A thread lock the database and goes in an infinite loop, 
     * then another thread locks the database and it must wait until the timeout is expired, after that it exit incrementing the exitCondition.
     * @param the timeout for the lock function.
     * @return true if the second thread exit incrementing exitCondition after the expire of the timeout, false if too much time pass because this means that both thread are locked and the timeout 
     * does not work. **/
    bool TestTimeoutLock(TimeoutType time);

};

#endif
