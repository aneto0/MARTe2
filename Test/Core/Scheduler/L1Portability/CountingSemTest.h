/**
 * @file CountingSemTest.h
 * @brief Header file for class CountingSemTest
 * @date 12/04/2021
 * @author Martino Ferrari
 *
 * @copyright Copyright 2021 F4E | European Joint Undertaking for ITER and
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

 * @details This header file contains the declaration of the class CountingSemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef COUNTINGSEMTEST_H_
#define COUNTINGSEMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CountingSem.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the CountingSem functions.
 */
class CountingSemTest {
public:
    int32       sharedVariable1;
    CountingSem countingSem;
    /**
     * @brief Constructor.
     * @details Creates the semaphore handle.
     */
    CountingSemTest();

    /**
     * @brief Destructor.
     * @details Close the semaphore handle.
     */
    ~CountingSemTest();

    /**
     * @brief Tests the CountingSem::EventSem constructor.
     * @return true if the semaphore is successfully instantiated. To check this is used the function IsClosed() which returns true if it is closed
     */
    bool TestConstructor();

    /**
     * @brief Tests the CountingSem::~EventSem destructor.
     * @return true if the destructor calls the semaphore close function.
     */
    bool TestDestructor();

    /**
     * @brief Tests the CountingSem::Create function.
     * @param[in] numberOfActorsIn is the number of actors to syncrhonize.
     * @return true if the semaphore is successfully created which also implies that a non NULL handle is created.
     */
    bool TestCreate(uint32 numberOfActorsIn);


    /**
     * @brief Tests the CountingSem::WaitForAll function.
     * @details Checks that the wait function fails gracefully after the timeout expire.
     * @param[in] numberOfActors is the number of actors to synchronize.
     * @param[in] timeout is the desired timeout.
     * @return true if the wait function correctly expires with the timeout and signals the correct error.
     */
    bool TestWaitForAll(uint32 numberOfActors,TimeoutType timeout);

    /**
     * @brief Tests the CountingSem::Wait function.
     * @details This functions waits on an event semaphore. A thread is spawned and should
     * post this semaphore.
     * @return true if the function waits until it is posted and if CountingSem::Wait returns true.
     */
    bool TestWait();

    /**
     * @brief Tests the CountingSem::ResetWait function.
     * @details Same strategy as TestWait but using the ResetWait function.
     * @return true if the function waits until it is posted and if CountingSem::WaitReset returns true.
     */
    bool TestResetWait();

    /**
     * @brief Tests the CountingSem::Reset function.
     * @details Same strategy as TestPost but now focusing on the return value of the Reset function.
     * The first post should return true, the second should return false because the handle is still signaled.
     * After the reset the post should have success again.
     * @return true if the semaphore can be successfully posted after being reset, false otherwise.
     */
    bool TestReset();

    bool TestForceReset();

    bool TestForcePass();

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
     * @brief Implements the test logic.
     * @details The value of the sharedVariable is initialised to 0xABCD\n
     * The CountingSem is reset and all the threads are created.\n
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
    friend void MultiThreadedTestWaitCallback(CountingSemTest &eventSemTest);
    /**
     * @brief Helper callback to post the CountingSem so that the Wait and Reset functions can be tested
     * @param[in] eventSemTest the class instance under test
     */
    friend void PosterThreadCallback(CountingSemTest &eventSemTest);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEMTEST_H_ */

