/**
 * @file FastPollingMutexSemTest.h
 * @brief Header file for class FastPollingMutexSemTest
 * @date 02/07/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class FastPollingMutexSemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FAST_POLLING_MUTEXTEST_H_
#define FAST_POLLING_MUTEXTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "FastPollingMutexSem.h"
#include "EventSem.h"
#include "Threads.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the FastPollingMutexSem functions.
 */
class FastPollingMutexSemTest {

public:

    /**
     * @brief Default constructor.
     * @details Creates the mutex handle.
     */
    FastPollingMutexSemTest();

    /**
     * @brief Destructor.
     * @details Closes the mutex handle.
     */
    ~FastPollingMutexSemTest();

    /**
     * @brief Tests the FastPollingMutexSem constructor
     * @return true if the semaphore is initialized unlocked as expected.
     */
    bool TestConstructor();

    /**
     * @brief Tests the FastPollingMutexSem::Create function.
     * @param[in] locked is the desired initial state: true = locked, false = unlocked.
     * @return true if the semaphore is successfully created which also implies that a non NULL handle is created.
     */
    bool TestCreate(bool locked);

    /**
     * @brief Tests the FastPollingMutexSem::FastLock function.
     * @details This test spawns nOfThreads threads which compete for a shared resource variable.
     * Each of the threads will try to increment and decrement the shared variable as many times as possible during one second.
     * Each of the threads will call FastPollingMutexSem::Lock before accessing the shared resource.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function.
     * @return true if the calls to FastPollingMutexSem::Lock returned true and if the shared resource was protected.
     */
    bool TestFastLock(int32 nOfThreads,
                      TimeoutType timeout);

    /**
     * @brief Tests the FastPollingMutexSem::FastUnLock function.
     * @details This test spawns nOfThreads threads which compete for a shared resource variable.
     * Each of the threads will try to increment and decrement the shared as many times as possible during one second.
     * Each of the threads will call FastPollingMutexSem::UnLock before releasing the shared resource.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function (this allows to demonstrate that
     * the UnLock is also working for semaphores which were locked with a finite timeout).
     * @return true if the calls to FastPollingMutexSem::UnLock returned true and if the shared resource was protected.
     */
    bool TestFastUnLock(int32 nOfThreads,
                        TimeoutType timeout);

    /**
     * @brief Tests the FastPollingMutexSem::FastTryLock function.
     * @details This test spawns nOfThreads threads which compete for a shared resource variable.
     * Each of the threads will try to increment and decrement the shared as many times as possible during one second.
     * Each of the threads will call FastPollingMutexSem::FastTryLock before accessing the shared resource. If this call returns false
     * each thread will keep on calling FastPollingMutexSem::FastTryLock until it returns true.
     * @return true if the function behaves as expected when protecting a shared resource and if
     * it is demonstrated that a second consecutive call to FastPollingMutexSem::FastTryLock returns false.
     */
    bool TestFastTryLock(int32 nOfThreads);

    /**
     * @brief Tests the FastPollingMutexSem::Lock function and checks that the error code is correctly set.
     * @return true if the error code is set to Errors::Information when the call succeeds and to timeout when
     * there is a timeout in the Lock.
     */
    bool TestFastLockErrorCode();

    /**
     * @brief Tests the FastPollingMutexSem::Locked function.
     * @return true when the semaphore is locked and FastPollingMutexSem::Locked returns true
     * and when a semaphore in unlocked and FastPollingMutexSem::Locked returns false
     */
    bool TestLocked();

    /**
     * @brief Verifies if the semaphore dead-locks as expected.
     * @details the FastPollingMutexSem is a non-recursive semaphore and does not allow two consecutive locks
     * (i.e. without unlocking before) from the same thread and will go into dead-lock.
     * @return true when the testing thread goes into dead-lock after two consecutive calls to FastLock
     */
    bool TestRecursive();

private:

    /**
     * Semaphore which is tested by the unit testing functions
     */
    FastPollingMutexSem testMutex;

    /**
     * timeout to be used for the locking test
     */
    TimeoutType testMutexTimeout;

    /**
     * Shared variable that is protected by the unit testing functions
     */
    int32 sharedVariable;

    /**
     * Number of running threads for a given test
     */
    int32 nOfExecutingThreads;

    /**
     * This variable is set to true when the callback functions are expected to return
     */
    bool stop;

    /**
     * To be used by the threading callback functions to report an error
     */
    bool failed;

    /**
     * Helper semaphore to synchronize the launching of concurrent threads
     */
    EventSem synchSem;

    /**
     * @brief Helper function to manage the threads involved in most of the FastPollingMutexSem Tests.
     *
     * @details this method creates nOfThreads threads pointing at the functionToTest callback.
     * It waits one second and then informs the functionToTest callbacks that these should return.
     * This exchange of information is perform through the class variable stop=true. When all the threads
     * return, it sets the return value to !failed (which was set by the callback functions).
     *
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function.
     * @param[in] functionToTest the function callback to be called by the threads.
     * @return the value returned by the functionToTest.
     */
    bool GenericMutexTestCaller(int32 nOfThreads,
                                TimeoutType timeout,
                                ThreadFunctionType functionToTest);

    /**
     * Allow the callback functions to access the private methods of the class
     */
    /**
     * @see TestFastLock
     */
    friend void TestFastLockCallback(FastPollingMutexSemTest &mt);
    /**
     * @see TestFastUnLock
     */
    friend void TestFastUnLockCallback(FastPollingMutexSemTest &mt);
    /**
     * @see TestFastTryLock
     */
    friend void TestFastTryLockCallback(FastPollingMutexSemTest &mt);
    /**
     * @see TestFastLockErrorCode
     */
    friend void TestFastLockErrorCodeCallback(FastPollingMutexSemTest &mt);
    /**
     * @see TestRecursive
     */
    friend void TestRecursiveCallback(FastPollingMutexSemTest &mt);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MUTEXTEST_H_ */
