/**
 * @file FastPollingEventSemTest.h
 * @brief Header file for class FastPollingEventSemTest
 * @date 24/03/2016
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

 * @details This header file contains the declaration of the class FastPollingEventSemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTPOLLINGEVENTSEMTEST_H_
#define FASTPOLLINGEVENTSEMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "FastPollingEventSem.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class FastPollingEventSemTest {

public:

    /**
     * @brief Default constructor.
     * @details Creates the event handle.
     */
    FastPollingEventSemTest();

    /**
     * @brief Destructor.
     * @details Closes the event handle.
     */
    ~FastPollingEventSemTest();

    /**
     * @brief Tests the FastPollingEventSem constructor
     * @return true if the semaphore is initialized as expected.
     */
    bool TestConstructor();

    /**
     * @brief Tests the FastPollingEventSem from external spin-lock
     * @return true if the semaphore is initialized as expected.
     */
    bool TestConstructorFromExternal();

    /**
     * @brief Tests the FastPollingEventSem::Create function.
     * @param[in] locked is the desired initial state: true = locked, false = unlocked.
     * @return true if the semaphore is successfully created which also implies that a non NULL handle is created.
     */
    bool TestCreate(bool wait);

    /**
     * @see TestCreate with FastPollingEventSem initalised by external spin-lock.
     */
    bool TestCreateExternal(bool wait);

    /**
     * @brief Tests the FastPollingEventSem::FastWait function.
     * @details This test spawns nOfThreads threads which waits on the event semaphore.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function.
     * @return true if the calls to FastPollingMutexSem::FastWait returns true and if the threads waits until the post function
     * is triggered by the main thread.
     */
    bool TestFastWait(int32 nOfThreads,
                      TimeoutType timeout);


    /**
     * @see TestFastLock with FastPollingMutexSem initalised by external spin-lock.
     */
    bool TestFastWaitExternal(int32 nOfThreads,
                      TimeoutType timeout);


    /**
     * @brief Tests the FastPollingEventSem::FastPost function.
     * @details This test spawns nOfThreads threads which waits on the event semaphore.
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the lock function.
     * @return true if the calls to FastPollingMutexSem::FastWait returns true and if the threads waits until the post function
     * is triggered by the main thread.
     */
    bool TestFastPost(int32 nOfThreads,
                        TimeoutType timeout);

    /**
     * @see TestFastUnLock with FastPollingMutexSem initalised by external spin-lock.
     */
    bool TestFastPostExternal(int32 nOfThreads,
                        TimeoutType timeout);


    /**
     * @brief Tests the FastPollingMutexSem::TestFastResetWait function.
     * @details This test spawns nOfThreads threads which waits on the event semaphore until the post, then other threads are launched
     * performing ResetAndWait to reuse the same semaphore again.
     * @return true if the function behaves as expected reusing two times the same event semaphore.
     */
    bool TestFastResetWait(int32 nOfThreads);

    /**
     * @see TestFastTryLock with FastPollingMutexSem initalised by external spin-lock.
     */
    bool TestFastResetWaitExternal(int32 nOfThreads);


    /**
     * @brief Tests the FastPollingMutexSem::FastWait function and checks that the error code is correctly set.
     * @return true if the error code is set to ErrorManagement::noError when the call succeeds and to ErrorManagement::timeout when
     * there is a timeout in the wait.
     */
    bool TestFastWaitErrorCode();


private:

    /**
     * Semaphore which is tested by the unit testing functions
     */
    FastPollingEventSem testEvent;

    /**
     * External flag
     */
    volatile int32 extFlag;

    /**
     * Semaphore initialised by external spin-lock variable.
     */
    FastPollingEventSem testEventExt;

    /**
     * timeout to be used for the locking test
     */
    TimeoutType testEventTimeout;

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
     * To use the external spin-lock
     */
    bool external;

    /**
     * @brief Helper function to manage the threads involved in most of the FastPollingEventSem Tests.
     *
     * @details this method creates nOfThreads threads pointing at the functionToTest callback.
     * It waits until all threads begin their execution and wait on the event semaphore, then posts the latter.
     *
     * @param[in] nOfThreads is the number of threads to launch.
     * @param[in] timeout the timeout value that will be used to call the wait function.
     * @param[in] functionToTest the function callback to be called by the threads.
     * @return the value returned by the functionToTest.
     */
    bool GenericEventTestCaller(int32 nOfThreads,
                                TimeoutType timeout,
                                ThreadFunctionType functionToTest);

    /**
     * Allow the callback functions to access the private methods of the class
     */
    /**
     * @see TestFastWait
     */
    friend void TestFastWaitCallback(FastPollingEventSemTest &mt);

    /**
     * @see TestFastResetWait
     */
    friend void TestFastResetWaitCallback(FastPollingEventSemTest &mt);
    /**
     * @see TestFastWaitErrorCode
     */
    friend void TestFastWaitErrorCodeCallback(FastPollingEventSemTest &mt);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* FASTPOLLINGEVENTSEMTEST_H_ */

