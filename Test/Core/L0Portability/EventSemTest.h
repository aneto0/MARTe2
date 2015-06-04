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
 * @file EventSemTest.h
 * @brief Tests the EventSem and associated functions.
 *
 * The test consists in concurrent threads incrementing a shared variable.
 * These will only be allowed to change this variable value when the semaphore is posted.
 */

#ifndef EVENT_SEM_TEST_H
#define EVENT_SEM_TEST_H

#include "Threads.h"
#include "EventSem.h"

/**@brief Class for testing of EventSem functions. */
class EventSemTest {
private:
    /**
     * Implements the test logic.
     * The value of the sharedVariable is initialised to 0xABCD
     * The EventSem is reset and all the threads are created.
     * The threads will then wait on the event semaphore being allowed to proceed.
     * The Test function waits for some time before posting the semaphore. If the semaphore
     * is functioning as expected, the value of the sharedVariable should still be 0xABCD
     * before posting.
     * The value of the sharedVariable is then set to zero and the semaphore is posted.
     * It is then expected that the sharedVariable is individually incremented by each thread.
     * @param nOfThread the number of threads for the test
     * @return True if the final value of sharedVariable == nOfThreads
     */
    bool TestWait(uint32 nOfThreads);
public:

    /**
     * Shared variable that will be incremented by each thread
     */
    uint32 sharedVariable;

    /**
     * The EventSem to be tested
     */
    EventSem eventSem;

    /**
     * Timeout for the tests with timeout
     */
    TimeoutType timeout;

    /**
     * @brief Constructor
     */
    EventSemTest() {
        sharedVariable = 0;
        timeout = TTInfiniteWait;
        eventSem.Create();
    }

    /**
     * @brief Test for an infinite timeout
     * @return True if the final value of sharedVariable == nOfThreads
     */
    bool WaitNoTimeoutTest(uint32 nOfThreads);

    /**
     * @brief Test for an finite timeout of 2 seconds
     * @return True if the final value of sharedVariable == nOfThreads
     */
    bool WaitTimeoutTestSuccess(uint32 nOfThreads);

    /**
     * @brief Test for a finite timeout of 2 seconds
     *
     * In this test the timeout of the semaphore will be very low with respect to the time of the test. So that it
     * is very likely that when the semaphore is posted some of the semaphores will have already timed-out and
     * changed the initial value of the sharedVariable (set to 0xABCD).
     *
     * @param nOfThreads Number of threads that will change the sharedVariable value
     * @return True if when the semaphore is posted the sharedVariable is different from its initial value
     */
    bool WaitTimeoutTestFailure(uint32 nOfThreads);

    /**
     * @brief Checks that a semaphore, even after timing out still works.
     * @return Forces timeout by calling WaitTimeoutTestFailure and WaitNoTimeoutTest returns True afterwards.
     */
    bool WaitTimeoutTestFailureFollowedBySuccess(uint32 nOfThreads);
};

#endif

