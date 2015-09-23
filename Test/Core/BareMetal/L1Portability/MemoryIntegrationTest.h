/**
 * @file MemoryIntegrationTest.h
 * @brief Header file for class MemoryIntegrationTest
 * @date 29/06/2015
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

 * @details This header file contains the declaration of the class MemoryIntegrationTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYINTEGRATIONTEST_H_
#define MEMORYINTEGRATIONTEST_H_
/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryCheck.h"
#include "Sleep.h"
#include "Threads.h"
#include "EventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/** @brief Class for testing of Memory functions. */
class MemoryIntegrationTest {

private:

public:
    /**
     * A semaphore used for the shared memory test.
     */
    EventSem eventSem;

    bool signals[MAX_NO_OF_MEMORY_MONITORS - 1];

    uint32 counter;

    int32 sizeStore[MAX_NO_OF_MEMORY_MONITORS - 1];

    int32 chunksStore[MAX_NO_OF_MEMORY_MONITORS - 1];

    /**
     * @brief Constructor.
     */
    MemoryIntegrationTest();

    /**
     * @brief Destructor.
     */
    ~MemoryIntegrationTest();

    /**
     * @brief Tests the shared memory between two different threads and the main process.
     * @details creates a shared memory for a boolean value and an integer value  by threads and
     * checks if the values are effectively shared changing and accessing and checking them with different threads.
     * @return true if the shared integer and the shared boolean remain consistent with the operations done by threads.
     */
    bool TestSharedMemory();

    bool TestHeader();

    bool TestDatabase();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYINTEGRATIONTEST_H_ */

