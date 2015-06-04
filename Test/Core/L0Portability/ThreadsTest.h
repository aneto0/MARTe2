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
/**
 * @file ThreadsTest.h
 * @brief Tests the Threads class and associated functions
 */

#ifndef THREADS_TEST_H
#define THREADS_TEST_H

#include "Threads.h"
#include "EventSem.h"
#include "MutexSem.h"

/** @brief A class for testing of Threads functions. */
class ThreadsTest {
public:
    ThreadsTest() {
        callbackTestSuccessful = False;
        incrementCounter = 0;

        eventSem.Create();
        eventSemEndManagement.Create();
        nOfThreads1 = 0;
        threadIdTarget = 0;
        falseId = (TID)1;
        mutexSem.Create(False);
        nameThreadTest = "F4EThread";
    }
    /**
     * Variable incremented by the call back function ThreadsTestIncrementCounter
     */
    int32 incrementCounter;

    /**
     * Indicates how many threads will be created
     */
    int32 nOfThreads1;

    /**
     * Variable mutexSem to protect parts of the code
     */
    MutexSem mutexSem;

    /**
     * Store the Id of one thread
     */
    TID threadIdTarget;

    /**
     * Random Id to check Thread function with a non existing Thread Id
     */
    TID falseId;

    /**
     * Name of a thread
     */

    const char *nameThreadTest;

    /**
     * Generic event sem used for synchronisations in the test
     */
    EventSem eventSem;

    /**
     * event semaphore used to indicate the end of the thread MANAGEMENT
     */
    EventSem eventSemEndManagement;

    /**
     * To be used to communicate between callbacks and the 
     * function spawing the thread
     */
    bool callbackTestSuccessful;

    /**
     * @brief Tests the thread creation
     * @param nOfThreads number of threads to test
     * @return True if nOfThreads are created
     */
    bool BeginThread(uint32 nOfThreads);

    /**
     * @brief Test changes of priority levels and classes.
     * This a very basic test and does not check if (and how) the priority request is actually propagated to
     * the operating system
     * @return True if the priority level and class are changed consistently with
     * the request
     */
    bool Priorities();

    /**
     *@brief Test if the OS respect the priorities given to different threads. ATTENTION root is required to change priorities
     *Initialize threats with different priorities and stop them in a semaphore. When all the threats are inicialized set the semaphore "green" and check which threat goes first
     *@return true when the threads with more priority go first than the threads with less priority.
     */
    bool PrioritiesPropagationStartFirst();

    /**
     *@brief Checks @see Threads::Id() returns a Id different to 0
     *@return true when the Id of the threads is different to 0
     */
    bool ThreadIdTest();

    /**
     *@brief Test if a thread can kill itself and if one threat can kill another thread.
     *First part of the test is autokill. Second part of the test, target thread and a management thread are created. Thread MANAGEMENT tries to kill the thread target and then checks if the target is alived. Moreover additional cases are tested. For instant the MANAGEMENT tries to kill the target twice, the second kill fails as expectect but the program shouldn't crash. Nevertheless with a invalid Id the ThreadsKill crash...
     *@return true when all the threads are kill succesfully and the return of the function ThreadsIsAlive() is what is expected.
     */
    bool ThreadKillTest();

    /**
     *@brief Test if TheadsGetsName gets the correct name
     * Also is check the case of a non valid Id. The expected name is NULL.
     *@return true when the name is the expected name
     */
    bool ThreadNameTest();

    /**
     *@brief Test if @see Threads::EndThread ends the thread
     *@return true when the target thread is alive at the beginning and dead at the end
     */
    bool ThreadEndTest();

    /**
     *@brief Checks if @see Threads::GetCPUs returns the expected CPU mask
     * 254 threads are created assigning a different runCPU, then is checked if the ThreadsGetCPUs returns the expected value
     *@return true when ThreadsGetCPUs returns the expected value.
     */
    bool CpuRunTest();
};

#endif

