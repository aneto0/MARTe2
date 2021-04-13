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
     * @details Checks that the wait function post gracefully after that all the actors check in.
     * @param[in] numberOfActors is the number of actors to synchronize.
     * @param[in] timeout is the desired timeout.
     * @return true if the wait function correctly expires after all actors checkd in.
     */
    bool TestWaitForAll(uint32 numberOfActors, TimeoutType timeout);

    /**
     * @brief Tests the CountingSem::ForceReset function.
     * @details Checks that the force reset function works properly.
     * @param[in] numberOfActors is the number of actors to synchronize.
     * @param[in] timeout is the desired timeout.
     * @return true if the reset works properly.
     */
    bool TestForcePass(uint32 numberOfActors, TimeoutType timeout);

    /**
     * @brief Tests the CountingSem::ForceReset function.
     * @details Checks that the force reset function works properly.
     * @param[in] numberOfActors is the number of actors to synchronize.
     * @param[in] timeout is the desired timeout.
     * @return true if the reset works properly.
     */
    bool TestForceReset(uint32 numberOfActors, TimeoutType timeout);

private:
    // NO PRIVATE MEMBERS
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EVENTSEMTEST_H_ */

