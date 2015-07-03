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
     * @brief Constructor.
     */
    MemoryTest();

    /**
     * @brief Destructor.
     */
    ~MemoryTest();


    /**
     * @brief Tests the correct behavior of the malloc function.
     * @param[in] size is the number of integers to allocate.
     * @return true if the pointers to the allocated memory are valid.
     */
    bool TestMallocAndFree(int32 size);

    /**
     * @brief Tests the correct behavior of the realloc function.
     * @param[in] size1 is the number of integers to allocate with initial malloc.
     * @param[in] size2 is the additional memory which must be allocated.
     * @return true if the pointers are valid and the realloc does not corrupt the initial memory.
     */
    bool TestRealloc(int32 size1, int32 size2);

    /**
     * @brief Tests the correct behavior of the string duplicate.
     * @param[in] s is the string to duplicate.
     * @return true if the string result of the function is equal to s.
     */
    bool TestMemoryStringDup(const char8 *s);

    /**
     * @brief Tests the shared memory between two different threads and the main process.
     * @details creates a shared memory for a boolean value and an integer value  by threads and
     * checks if the values are effectively shared changing and accessing and checking them with different threads.
     * @return true if the shared integer and the shared boolean remain consistent with the operations done by threads.
     */
    bool TestSharedMemory();

    /**
     * @brief Tests the copy and the move functions.
     * @return true if the memory is copied correctly.
     */
    bool TestCopyAndMove();

    /**
     * @brief Tests the set and the search functions.
     * @return true if the functions work correctly.
     */
    bool TestSetAndSearch();


    /**
     * @brief Tests the header that contains the allocated memory information.
     * @return true if the returned informations on the allocated memory are correct.
     */
    bool TestHeader();

    /**
     * @brief Tests the memory database.
     * @return true if the informations of the allocated memory are stored correctly on the database.
     */
    bool TestDatabase();
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYTEST_H_ */

