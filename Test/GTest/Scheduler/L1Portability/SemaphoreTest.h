/**
 * @file SemaphoreTest.h
 * @brief Header file for class SemaphoreTest
 * @date 26/07/2015
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

 * @details This header file contains the declaration of the class SemaphoreTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SEMAPHORETEST_H_
#define SEMAPHORETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Semaphore.h"
#include "MutexSem.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the Semaphore functions.
 */
namespace SemaphoreTest {
    /**
     * @brief Tests the Semaphore::Semaphore constructor.
     * @return true if the semaphore is successfully instantiated. To check this is used the function IsClosed() which returns true if it is closed
     */
    bool TestConstructor();

    /**
     * @brief Tests the Semaphore::~Semaphore destructor.
     * @return true if the destructor calls the semaphore close function.
     */
    bool TestDestructor();

    /**
     * @brief Tests the Semaphore::Create function.
     * @return true if the semaphore is successfully created which also implies that a non NULL handle is created.
     */
    bool TestCreate();

    /**
     * @brief Tests the Semaphore::Close function.
     * @return true if the semaphore is successfully closed.
     */
    bool TestClose();

    /**
     * @brief Tests the Semaphore::IsClosed function
     * @return true if the semaphore can report effectively if it is closed or not
     */
    bool TestStatus();

    /**
     * @brief Tests the Semaphore::Wait function.
     * @details Checks that the wait function fails gracefully after the timeout expire.
     * @param[in] timeout is the desired timeout.
     * @return true if the wait function correctly expires with the timeout and signals the correct error.
     */
    bool TestTake_Latching(MilliSeconds timeout);

    /**
     * @brief Tests the Semaphore::Post .
     * @details The first post should return true, the second should return false because the handle is still signaled.
     * After the reset the post should have success again.
     * @return true if the semaphore can be successfully posted after being reset, false otherwise.
     */
    bool TestSet_Latching();

    /**
     * @brief Tests the Semaphore::Reset function.
     * @details Same strategy as TestPost but now focusing on the return value of the Reset function.
     * The first post should return true, the second should return false because the handle is still signaled.
     * After the reset the post should have success again.
     * @return true if the semaphore can be successfully posted after being reset, false otherwise.
     */
    bool TestReset_Latching();

    /**
     * @brief Tests the Semaphore::Take function.
     * @details The functions waits on a latching semaphore. A thread is spawned and should
     * post this semaphore.
     * @return true if the function waits until it is posted and if Semaphore::Wait returns true.
     */
    bool TestTake_Latching_Thread();

    /**
     * @brief Tests the Semaphore(Latching) functionality.
     * @details A number of threads wait on a latching semaphore. the functions checks the amount of threads waiting before and after Set().
     * @return true if the count is ok
     */
    bool TestTake_Latching_Threads(uint32 nOfThreads,MilliSeconds timeout);

    /**
     * @brief Tests the Semaphore(AutoResetting) functionality.
     * @details A number of threads wait on a AutoResetting semaphore. the functions checks the amount of threads waiting before and after Set().
     * @return true if the count is ok
     */
    bool TestTake_AutoResetting_Threads(uint32 nOfThreads,MilliSeconds timeout);

    /**
     * @brief Tests the Semaphore(MultiLock) functionality.
     * @details A number of threads wait on a Counting semaphore. the functions checks the amount of threads waiting before and after Set(). It will unblock 3 threads at a time
     * @return true if the count is ok
     */
    bool TestTake_Counting_Threads(uint32 nOfThreads,MilliSeconds timeout);

    /**
     * @brief Tests the Semaphore(Mutex) functionality.
     * @details A number of threads wait on a latching semaphore. the functions checks the amount of threads waiting before and after Set(). It will unblock stepRelease threads at a time
     * @return true if the count is ok
     */
    bool TestTake_Combined_Threads(uint32 nOfThreads,int32 stepRelease,MilliSeconds timeout,FILE *errorDetails);

    /**
     * @brief Tests the Semaphore(Mutex) functionality.
     * @details A number of threads wait on a latching semaphore. the functions checks the amount of threads waiting before and after Set(). It will unblock all threads at a time
     * @return true if the count is ok
     */
    bool TestTake_Mutex_Threads(uint32 nOfThreads,MilliSeconds timeout,FILE *errorDetails);

    bool TestMultiWait_Threads(uint32 nOfThreads,MilliSeconds timeout,FILE *errorDetails);

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SEMAPHORETEST_H_ */

