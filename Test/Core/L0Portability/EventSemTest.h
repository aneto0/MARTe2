/**
 * @file EventSemTest.h
 * @brief Header file for class EventSemTest
 * @date 26/giu/2015
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

 * @details This header file contains the declaration of the class EventSemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EVENTSEMTEST_H_
#define 		EVENTSEMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "EventSem.h"
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
     * @brief Tests the wait function.
     * @details Cheks that the wait function fails after the timeout expire.
     * @param[in] timeout is the desired timeout.
     * @return true if successful, false otherwise.
     */
    bool TestWait(TimeoutType timeout);


    /**
     * @brief Tests the post function.
     * @details The first post should return true, the second should return false because the handle is still signaled.
     * After the reset the post should have success again.
     * @return true if successful, false otherwise.
     */
    bool TestPost();


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEMTEST_H_ */

