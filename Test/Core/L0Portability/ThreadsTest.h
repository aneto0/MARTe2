/**
 * @file ThreadsTest.h
 * @brief Header file for class ThreadsTest
 * @date 25/06/2015
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
     * @brief Tests the begin function.
     * @details Launchs a thread and checks if it is alive and if the related name is correct.
     * @param[in] name is the desired thread name.
     * @param[in] stackSize is the desired stack size.
     * @return true if successful, false otherwise.
     */
    bool TestBegin(const char* name,
                   uint32 stackSize);

    /**
     * @brief Tests the end function.
     * @details Checks if the thread terminates after the end function call.
     * @return true if successful, false otherwise.
     */
    bool TestEnd();

    /**
     * @brief Tests the set priority and get priority (for all priority levels and classes) functions.
     * @return true if successful, false otherwise.
     */
    bool TestPriority();

    /**
     * @brief Tests the get state function which should return the state of the specified thread.
     * @return ture if successful, false otherwise.
     */
    bool TestState();

    /**
     * @brief Tests the Id function which should return the own id.
     * @return true if successful, false otherwise.
     */
    bool TestId();

    /**
     * @brief Test the get cpu function.
     * @return true if successful, false otherwise.
     */
    bool TestCPUs();

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

