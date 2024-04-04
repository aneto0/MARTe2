/**
 * @file EventSemTest.h
 * @brief Header file for class EventSemTest
 * @date 26/07/2015
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class EventSemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTSEMTEST_H_
#define EVENTSEMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EventSem.h"
#include "MutexSem.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the EventSem functions.
 */
class EventSemTest {
public:
    /**
     * The event semaphore object
     */
    EventSem eventSem;
    int32    sharedVariable1;

    /**
     * @brief Constructor.
     * @details Creates the semaphore handle.
     */
    EventSemTest();

    /**
     * @brief Destructor.
     * @details Close the semaphore handle.
     */
    ~EventSemTest();

    /**
     * @brief Tests the EventSem::EventSem constructor.
     * @return true if the semaphore is successfully instantiated. To check this is used the function IsClosed() which returns true if it is closed
     */
    bool TestConstructor();

    /**
     * @brief Tests the EventSem::~EventSem destructor.
     * @return true if the destructor calls the semaphore close function.
     */
    bool TestDestructor();

    /**
     * @brief Tests the EventSem::Create function.
     * @return true if the semaphore is successfully created which also implies that a non NULL handle is created.
     */
    bool TestCreate();

    /**
     * @brief Tests the EventSem::Close function.
     * @return true if the semaphore is successfully closed.
     */
    bool TestClose();

    /**
     * @brief Tests the EventSem copy constructor
     *
     * @details the EventSem has a copy constructor which allow to copy and share the handle of the same semaphore.
     * This means that the semaphore implementation is still the same but is encapsulated by two different instances
     * of the EventSem class.
     * Given this it should be possible to Lock a thread with EventSem instance and unlock with another MutexSem instance
     * which was constructed from the first and which consequently shares the same handle.
     *
     * @return true if the a copy constructed semaphore does share the same handle (i.e. the pointer address do the
     * handle is the same) and is capable of correctly acting on this handle.
     */
    bool TestCopyConstructor();

    /**
     * @brief Test the EventSem copy constructor using different semaphores.
     *
     * @details Create two semaphores using copying constructor. Then Lock a thread with one semaphore and unLuck with the other.
     * @return true if the threat is unLocked before the timeout.
     */
    bool TestCopyConstructor2Sem();

    /**
     * @brief Tests the EventSem::IsClosed function
     * @return true if the semaphore can report effectively if it is closed or not
     */
    bool TestIsClosed();

    /**
     * @brief Tests the EventSem::Wait function.
     * @details Checks that the wait function fails gracefully after the timeout expire.
     * @param[in] timeout is the desired timeout.
     * @return true if the wait function correctly expires with the timeout and signals the correct error.
     */
    bool TestWait(TimeoutType timeout);

    /**
     * @brief Tests the EventSem::Wait function.
     * @details This functions waits on an event semaphore. A thread is spawned and should
     * post this semaphore.
     * @return true if the function waits until it is posted and if EventSem::Wait returns true.
     */
    bool TestWait();

    /**
     * @brief Tests the EventSem::ResetWait function.
     * @details Same strategy as TestWait but using the ResetWait function.
     * @return true if the function waits until it is posted and if EventSem::WaitReset returns true.
     */
    bool TestResetWait();

    /**
     * @brief Tests the EventSem::Post .
     * @details The first post should return true, the second should return false because the handle is still signaled.
     * After the reset the post should have success again.
     * @return true if the semaphore can be successfully posted after being reset, false otherwise.
     */
    bool TestPost();

    /**
     * @brief Tests the EventSem::Reset function.
     * @details Same strategy as TestPost but now focusing on the return value of the Reset function.
     * The first post should return true, the second should return false because the handle is still signaled.
     * After the reset the post should have success again.
     * @return true if the semaphore can be successfully posted after being reset, false otherwise.
     */
    bool TestReset();

    /**
     * @brief Test for an infinite timeout
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the final value of sharedVariable == nOfThreads
     */
    bool TestWaitTimeoutNoTimeout(uint32 nOfThreads);

    /**
     * @brief Test for an finite timeout of 2 seconds
     * @param[in] nOfThreads is the number of threads to launch.
     * @return true if the final value of sharedVariable == nOfThreads
     */
    bool TestWaitTimeoutSuccess(uint32 nOfThreads);

    /**
     * @brief Test for a finite timeout of 2 seconds
     *
     * @details In this test the timeout of the semaphore will be very low with respect to the time of the test. So that it
     * is very likely that when the semaphore is posted some of the semaphores will have already timed-out and
     * changed the initial value of the sharedVariable (set to 0xABCD).
     *
     * @param[in] nOfThreads Number of threads that will change the sharedVariable value
     * @return true when the semaphore is posted and the sharedVariable is different from its initial value
     */
    bool TestWaitTimeoutFailure(uint32 nOfThreads);

    /**
     * @brief Checks that a semaphore, even after timing out still works.
     * @param[in] nOfThreads is the number of threads to launch.
     * @return Forces timeout by calling WaitTimeoutTestFailure and WaitErrorManagement::NoTimeoutTest returns true afterwards.
     */
    bool TestWaitTimeoutFailureFollowedBySuccess(uint32 nOfThreads);

private:
    /**
     * Shared variable that will be incremented by each thread
     */
    uint32 sharedVariable;

    /**
     * timeout for the tests with timeout
     */
    TimeoutType timeout;

    /**
     * Helper semaphore
     */
    MutexSem mutexSem;

    /**
     * @brief Implements the test logic.
     * @details The value of the sharedVariable is initialised to 0xABCD\n
     * The EventSem is reset and all the threads are created.\n
     * The threads will then wait on the event semaphore being allowed to proceed.\n
     * The Test function waits for some time before posting the semaphore. If the semaphore
     * is functioning as expected, the value of the sharedVariable should still be 0xABCD
     * before posting.
     *
     * The value of the sharedVariable is then set to zero and the semaphore is posted.\n
     * It is then expected that the sharedVariable is individually incremented by each thread.
     *
     * @param[in] nOfThreads the number of threads for the test
     * @return true if the final value of sharedVariable is equal to nOfThreads
     */
    bool MultiThreadedTestWait(uint32 nOfThreads);

    /**
     * @brief Helper callback function that is used by the thread spawned by the MultiThreadedTestWait.
     * @param[in] eventSemTest the class instance under test
     */
    friend void MultiThreadedTestWaitCallback(EventSemTest &eventSemTest);
    /**
     * @brief Helper callback to post the EventSem so that the Wait and Reset functions can be tested
     * @param[in] eventSemTest the class instance under test
     */
    friend void PosterThreadCallback(EventSemTest &eventSemTest);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEMTEST_H_ */

