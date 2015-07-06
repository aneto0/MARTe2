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
#define 		THREADSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Threads.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the Threads functions.
 */
class ThreadsTest {
public:

    ThreadsTest();

    /**
     * @brief Tests the Threads::BeginThread function.
     * @details Launches a thread and checks if it is alive and if the related name is correct.
     * @param[in] name is the desired thread name.
     * @param[in] stackSize is the desired stack size.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if successful, false otherwise.
     */
    bool TestBeginThread(const char8 *name,
                   uint32 stackSize,
                   uint32 nOfThreads);

    /**
     * @brief Tests the Threads::EndThread function.
     * @details Checks if the threads terminate after the end function call.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if successful, false otherwise.
     */
    bool TestEndThread(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::IsAlive function.
     * @details Launches a thread and checks if IsAlive returns the expected result.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if successful, false otherwise.
     */
    bool TestIsAlive(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::Kill function.
     * @details Kills a thread and checks if it terminates.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if successful, false otherwise.
     */
    bool TestKill(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::SetPriorityLevel and Threads::GetPriorityLevel (for all priority levels) functions.
     * @details Launches a thread for each priority class and level and checks if the priority class and level returned is correct.
     * @return true if successful, false otherwise.
     */
    bool TestPriorityLevel();

    /**
     * @brief Tests the Threads::SetPriorityClass and Threads::GetPriorityClass functions.
     * @details Launches a thread for each priority class and level and checks if the priority class and level returned is correct.
     * @return true if successful, false otherwise.
     */
    bool TestPriorityClass();

    /**
     * @brief Tests the Threads::GetState function which should return the state of the specified thread.
     * @return true if successful, false otherwise.
     */
    bool TestGetState();

    /**
     * @brief Tests the Threads::Id function which should return the own id.
     * @details Launches some threads and checks their id.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if successful, false otherwise.
     */
    bool TestId(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::GetCPUs function.
     * @return true if successful, false otherwise.
     */
    bool TestGetCPUs();

    /**
     * @brief Tests the Threads::Name function.
     * @details The return value should be the threads name if the thread is launched with specified name,
     * "Unknown" if the thread is launched without providing a name (namely NULL name).
     * @param[in] name is the desired thread name.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if successful, false otherwise.
     */
    bool TestName(const char8 *name,
                  uint32 nOfThreads);


    /**
     * @brief Tests the Threads::NumberOfThreads function
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the number of threads in the database is the expected number, false otherwise.
     */
    bool TestNumberOfThreads(uint32 nOfThreads);


    /**
     * @brief Tests the Threads::FindByIndex function.
     * @details Stores some threads in the database sequentially, then search them by index and checks if the returned tid is correct.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if successful, false otherwise.
     */
    bool TestFindByIndex(uint32 nOfThreads);

    /**
     * @brief Tests the Threads::GetThreadInfoCopy function.
     * @details Launches nOfThreads threads and get the thread information structure in the database checking that it is consistent
     * with the thread's specifications.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] name is the desired threads name.
     * @return true if successful, false otherwise.
     */
    bool TestGetThreadInfoCopy(uint32 nOfThreads,
                                            const char8 *name);

    /**
     * @brief Tests the Threads::FindByName function.
     * @details Launches thread with the specified name and the others without name, then checks if the returned
     * tid of the FindByName function is correct.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] name is the name to be searched in the database.
     * @param[in] position is the index of the thread with the specified name.
     * @return true if successful, false otherwise.
     */
    bool TestFindByName(uint32 nOfThreads, const char8 *name, uint32 position);

    /**
     * @brief Tests the Threads::ProtectedExecute function.
     * @return true if successful, false otherwise.
     */
    bool TestProtectedExecute();

    /**
     * A shared variable used for synchronization.
     */
    int32 exitCondition;

    /**
     * A variable used to save a thread identifier.
     */
    TID tidTest;

    /**
     * A boolean to store the return value.
     */
    bool retValue;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* THREADSTEST_H_ */

