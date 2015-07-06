/**
 * @file MemoryTest.h
 * @brief Header file for class MemoryTest
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

 * @details This header file contains the declaration of the class MemoryTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYTEST_H_
#define 		MEMORYTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Memory.h"
#include "EventSem.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



/**
 * @brief A class to testing Memory functions.
 */
class MemoryTest {

private:

public:

    /**
     * Event semaphore used for threads synchronization
     */
    EventSem eventSem;

    /**
     * Used to communicate with threads
     */
    bool signals[MAX_NO_OF_MEMORY_MONITORS - 1];

    /**
     * Internal counter
     */
    uint32 counter;

    /**
     * The memory amount allocated by each thread.
     */
    int32 sizeStore[MAX_NO_OF_MEMORY_MONITORS - 1];

    /**
     * The number of memory chuncks allocated by each thread.
     */
    int32 chunksStore[MAX_NO_OF_MEMORY_MONITORS - 1];
    /**
     * @brief Constructor.
     */
    MemoryTest();

    /**
     * @brief Destructor.
     */
    ~MemoryTest();


    /**
     * @brief Tests the Memory::Malloc function.
     * @param[in] size is the number of integers to allocate.
     * @return true if the pointers to the allocated memory are valid.
     */
    bool TestMalloc(uint32 size);


    /**
     * @brief Tests the Memory::Free function.
     * @details Allocates the size passed by argument then frees the memory and see if the pointer is NULL.
     * @param[in] size is the size to be allocated.
     * @return true if successful, false otherwise.
     */
    bool TestFree(uint32 size);


    /**
     * @brief Tests the Memory::Realloc function.
     * @details Checks the function passing also size = 0 (Memory::Free behavior) or pointer = NULL (Memory::Malloc) behavior.
     * @param[in] size1 is the number of integers to allocate with initial malloc.
     * @param[in] size2 is the additional memory which must be allocated.
     * @return true if successful, false otherwise.
     */
    bool TestRealloc(uint32 size1, uint32 size2);

    /**
     * @brief Tests the Memory::StringDup function.
     * @details Checks if the string is duplicated correctly and if the function returns NULL in case of NULL input.
     * @param[in] s is the string to duplicate.
     * @return true if successful, false otherwise.
     */
    bool TestStringDup(const char8 *s);

    /**
     * @brief Tests the Memory::Check function.
     * @details Allocates a chunk of memory and check if the function return true on the memory allocated. Then checks if the function returns fale in case of null pointer input.
     * @param[in] size is the size which must be allocated.
     * @return true if successful, false otherwise.
     */
    bool TestCheck(uint32 size);



    /**
     * @brief Tests the Memory::SharedAlloc function.
     * @details Allocates a shared memory and checks that the returned pointer is not null.
     * @return true if successful, false otherwise.
     */
    bool TestSharedAlloc();


    /**
     * @brief Tests the Memory::SharedFree function.
     * @details Allocates a shared memory and checks that the returned pointer is not null, then frees the shared memory and checks that the pointer is null.
     * @return true if successful, false otherwise.
     */
    bool TestSharedFree();

    /**
     * @brief Tests the shared memory between two different threads and the main process.
     * @details creates a shared memory for a boolean value and an integer value  by threads and
     * checks if the values are effectively shared changing and accessing and checking them with different threads.
     * @return true if the shared integer and the shared boolean remain consistent with the operations done by threads.
     */
    bool TestSharedMemory();

    /**
     * @brief Tests the Memory::Copy function.
     * @details Copies a memory area with different size parameters and checks if the function returns false on case of null inputs.
     * @return true if successful, false otherwise.
     */
    bool TestCopy();


    /**
     * @brief Tests the Memory::Copy function.
     * @details Copies a memory area with different size parameters and checks if the function returns false on case of null inputs.
     * @return true if successful, false otherwise.
     */
    bool TestMove();


    /**
     * @brief Tests the Memory::Compare function.
     * @details Compares two memory areas checking if the result is correct.
     * @return true if successful, false otherwise.
     */
    bool TestCompare();

    /**
     * @brief Tests the Memory::Set function.
     * @details Sets a character in the beginning of a memory area and another character at its end. Then checks if the memory is set as expected.
     * @return true if successful, false otherwise.
     */
    bool TestSet();


    /**
     * @brief Tests the Memory::Search function.
     * @details Search characters on a memory area trying also if the function returns false in case of null input or not found characters
     * @return true if successful, false otherwise.
     */
    bool TestSearch();


    /**
     * @brief Tests the Memory::GetHeaderInfo function
     * @details Tests the header that contains the allocated memory information. To test correctly the function the macro MEMORY_STATICS must be defined.
     * @return true if the returned informations on the allocated memory are correct.
     */
    bool TestGetHeaderInfo();


    /**
     * @brief Tests the Memory::AllocationStatistics function.
     * @details Allocates memory and checks if the the database saves the informations correctly.
     * @return true if successful, false otherwise.
     */
    bool TestAllocationStatistics();

    /**
     * @brief Tests the Memory::GetUsedHeap function.
     * @details Launches a number of threads and checks that during their execution the heap memory is not empty. Then after the threads termination
     * checks that the heap memory is empty.
     * @return true if successful, false otherwise.
     */
    bool TestGetUsedHeap(uint32 nOfThreads);

    /**
     * @brief Tests the Memory::ClearStatisticsDatabase.
     * @details Allocates memory and checks that the number of elements in the database and the heap memory amount is not zero. Then calls the function and checks
     * that the memory database is empty.
     * @return true if successful, false otherwise.
     */
    bool TestClearStatisticsDatabase();

    /**
     * @brief Tests the Memory::GetStatisticsDatabaseNElements.
     * @details Allocates memory and checks that there is only one element in the database. Then frees the memories and checks that the number of elements is zero again.
     * @return true if successful, false otherwise.
     */
    bool TestGetStatisticsDatabaseNElements();

    /**
     * @brief Tests the memory database functions together.
     * @return true if the informations of the allocated memory are stored correctly on the database.
     */
    bool TestDatabase();
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYTEST_H_ */

