/**
 * @file ThreadsTest.h
 * @brief Header file for class ThreadsTest
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

 * @details This header file contains the declaration of the class ThreadsTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef THREADSTEST_H_
#define 		THREADSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
#include "EventSem.h"
#include "MutexSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class for testing of Threads functions.
 */
class ThreadsTest {
public:

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
     * To be used for synchronization.
     */
    bool callbackTestSuccessful;

    ThreadsTest();

    ~ThreadsTest();

    /**
     * @brief Tests the thread creation
     * @param[in] nOfThreads number of threads to test
     * @return True if nOfThreads are created
     */
    bool BeginThread(uint32 nOfThreads);

    /**
     * @brief Test changes of priority levels and classes.
     * @details This a very basic test and does not check if (and how) the priority request is actually propagated to
     * the operating system
     * @return True if the priority level and class are changed consistently with
     * the request
     */
    bool Priorities();

    /**
     * @brief Test if the OS respect the priorities given to different threads. ATTENTION root is required to change priorities
     * @details Initialize threats with different priorities and stop them in a semaphore. When all the threats are inicialized set the semaphore "green" and check which threat goes first
     * @return true when the threads with more priority go first than the threads with less priority.
     */
    bool PrioritiesPropagationStartFirst();

    /**
     * @brief Checks if Threads::Id returns a Id different to 0
     * @return true when the Id of the threads is different to 0
     */
    bool ThreadIdTest();

    /**
     * @brief Test if a thread can kill itself and if one threat can kill another thread.
     * @details First part of the test is auto-kill. Second part of the test, target thread and a management thread are created. Thread MANAGEMENT tries to kill the thread target and then checks if the target is alive.
     * Moreover additional cases are tested. For instant the MANAGEMENT tries to kill the target twice, the second kill fails as expected but the program shouldn't crash.
     * @return true when all the threads are kill successfully and the return of the function ThreadsIsAlive() is what is expected.
     */
    bool ThreadKillTest();

    /**
     * @brief Test if TheadsGetsName gets the correct name
     * @details Also checks the case of a non valid Id. The expected name is NULL.
     * @return true when the name is the expected name
     */
    bool ThreadNameTest();

    /**
     * @brief Test if Threads::EndThread ends the thread
     * @return true when the target thread is alive at the beginning and dead at the end
     */
    bool ThreadEndTest();

    /**
     * @brief Checks if @see Threads::GetCPUs returns the expected CPU mask
     * @details Threads are created and assigned to different CPUs, then is checked if the ThreadsGetCPUs returns the expected value
     * @return true when ThreadsGetCPUs returns the expected value.
     */
    bool CpuRunTest();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADSTEST_H_ */

