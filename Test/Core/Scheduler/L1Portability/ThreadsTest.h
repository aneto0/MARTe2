/**
 * @file ThreadsTest.h
 * @brief Header file for class ThreadsTest
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

 * @details This header file contains the declaration of the class ThreadsTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef THREADSTEST_H_
#define THREADSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
#include "CCString.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the Threads functions.
 */
class ThreadsTest {
public:

    /**
     * @brief Tests the Threads::BeginThread and Threads::EndThread function.
     * @details nOfThreads are lunched by the test function.
     * Each thread increments a variable and polls on a shared variable
     * which is managed by the test function. When the test function changes the shared variable
     * the thread exits incrementing a exit condition and allowing for a new thread to be started.
     * Notice that never two threads are never executed in parallel. The next thread is launched after the previous
     * thread ends.
     * The test function has a timeout, if the thread is not correctly lunched, after expiration of the time out
     * the test function will return false;
     * @param[in] name is the desired thread name.
     * @param[in] stackSize is the desired stack size.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if all the threads can be successfully launched.
     */
    bool TestBeginThread(CCString name,
                         uint32 stackSize,
                         int32 nOfThreads);


    /**
     * @brief Tests the Threads::BeginThread function.
     * @details Launches a thread with a NULL function.
     * @param[in] name is the desired thread name.
     * @return true if no function is launched.
     **/
    bool TestBeginThreadNullFunction(CCString name);

    /**
     * @brief Tests the Threads::IsAlive function.
     * @details Launches a thread and checks if IsAlive returns the expected result.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the thread is all the threads are alive, false otherwise.
     */
    bool TestIsAlive(int32 nOfThreads);

    /**
     * @brief Tests the Threads::IsAlive function.
     * @details Launches a thread and checks if it IsAlive after kill.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the thread is not alive after kill, false otherwise.
     */
    bool TestIsAliveAfterkill(int32 nOfThreads);

    /**
     * @brief Tests the Threads::Kill function.
     * @details Kills a thread with an invalid ID.
     * @return true if Threads::Kill return false.
     */
    bool TestKillInvalidID();

    /**
     * @brief Tests the Threads::Kill function.
     * @details Kills a thread and checks if it terminates.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the thread is killed, false if the thread is still alive or a second kill returns
     * true.
     */
    bool TestKill(int32 nOfThreads);

    /**
     * @brief Tests the Threads::SetPriority() for all priority levels and classes.
     * @details Launches a thread for each priority class and level and checks if the priority class and level returned are correct.
     * @return true if the priority level and  priority class are as expected, false otherwise.
     */
    bool TestPriority();

    /**
     * @brief Tests the Threads::GetState function
     * @details In Linux and Windows theGetState() function returns STATE_UNKNOWN.
     * @return true if state is STATE_UNKNOWN, false otherwise.
     */
    bool TestGetState();

    /**
     * @brief Tests the Threads::Id function which should return the own id.
     * @details Launches nOfThreads and checks their id.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the returned value of the Threads::Id() = tidTest, false otherwise.
     */
    bool TestId(int32 nOfThreads);

    /**
     * @brief Tests the Threads::GetCPUs function.
     * @details Launches consecutively threads with CPU mask which indicates in which CPU the thread can be
     * running.
     * @return true if GetCPUs returns the expected CPU mask, false otherwise.
     */
    bool TestGetCPUs();

    /**
     * @brief Tests the Threads::Name function.
     * @details The return value should be the threads name if the thread is launched with specified name.
     * @param[in] name is the desired thread name.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if Threads::Name() returns the expected name of the thread, false otherwise.
     */
    bool TestName(CCString name,
                  int32 nOfThreads);

    /**
     * @brief Tests the Threads::Name function.
     * @details The return value of Threads::Name() should be "Unknown" if the thread is launched without providing a name (namely NULL name).
     * @param[in] name is the desired thread name.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if Threads::Name() returns "Unknown", false otherwise.
     */
    bool TestNameNull();

    /**
     * @brief Tests the Threads::NumberOfThreads function
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the number of threads in the database is the expected number, false otherwise.
     */
    bool TestNumberOfThreads(int32 nOfThreads);

    /**
     * @brief Tests the Threads::FindByIndex function.
     * @details Stores a number of threads in the database sequentially.
     * These are then searched by index and checked if the returned tid is correct.
     * @return true if the searched index thread has the correct ID, false otherwise.
     */
    bool TestFindByIndex();

    /**
     * @brief Tests the Threads::GetThreadInfoCopy function.
     * @details Launches nOfThreads threads and get the thread information structure in the database
     * checking if it is consistent with the thread's specifications.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] name is the desired threads name.
     * @return true if the properties of the threads (name, priorityClass, priorityLevel and threadId), are
     *  equal to the expected values, false otherwise.
     */
    bool TestGetThreadInfoCopy(int32 nOfThreads,
                               CCString name);

    /**
     * @brief Tests the Threads::GetThreadInfoCopy function.
     * @details Call Threads::GetThreadInfoCopy() with an invalid thread ID
     * @return true if Threads::GetThreadInfoCopy() returns false, false otherwise
     */
    bool TestGetThreadInfoCopyInvalidID();

    /**
     * @brief Tests the Threads::FindByName function.
     * @details Launches a number of threads with the specified name and the others without name, then checks if the returned
     * tid of the FindByName function is correct.
     * @param[in] nOfThreads the number of threads to launch.
     * @param[in] name the name to be searched in the database.
     * @param[in] position the index of the thread with the specified name.
     * @return true if can find a thread with "name" and another with "known" as a name, false otherwise.
     */
    bool TestFindByName(int32 nOfThreads,
                        CCString name,
                        int32 position);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADSTEST_H_ */

