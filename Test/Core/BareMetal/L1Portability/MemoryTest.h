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
#define MEMORYTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryCheck.h"
#include "MemoryOperationsHelper.h"
#include "HeapManager.h"
#include "StringHelper.h"
// TODO split files
// TODO test of HeapManager

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the Memory functions.
 */
class MemoryTest {

public:

    /**
     * @brief Tests the Memory::Malloc function.
     * @details Allocates blocks of memory with different sizes and checks that the return pointer is different than zero.
     * @param[in] size is the number of integers to allocate.
     * @return true if the memory can be successfully allocated and freed.
     */
    bool TestMalloc(uint32 size);

    /**
     * @brief Tests the Memory::Free function.
     * @details Allocates the size passed by argument then frees the memory and checks that the pointer is NULL.
     * @param[in] size is the size to be allocated.
     * @return true if the pointer to the memory block is NULL after the free operation.
     */
    bool TestFree(uint32 size);

    /**
     * @brief Tests the Memory::Realloc function.
     * @details Checks if the previous allocated memory is preserved after a realloc.
     * @param[in] size1 is the number of integers to allocate with initial malloc.
     * @param[in] size2 is the additional memory which must be allocated.
     * @return true if the first block of memory can be successfully allocated and a subsequent
     * call to realloc, successfully allocates new memory and preserves the original memory block.
     */
    bool TestRealloc(uint32 size1, uint32 size2);


    /**
     * @brief Tests the Memory::Realloc function.
     * @details Checks the function with a size = 0 argument (Memory::Free behavior).
     * @return true if the function behaves like the Memory::Free.
     */
    bool TestReallocZeroSize();


    /**
     * @brief Tests the Memory::Realloc function.
     * @details Checks the function passing NULL pointer input (Memory::Malloc) behavior.
     * @param[in] size is the number of integers to allocate.
     * @return true if the function behaves like the Memory::Malloc.
     */
    bool TestReallocNullPointer(uint32 size);


    /**
     * @brief Tests the Memory::StringDup function.
     * @param[in] s is the string to duplicate.
     * @return true if the string is duplicated correctly and if the function returns NULL in case of NULL input.
     */
    bool TestStringDup(const char8 *s);

    /**
     * @brief Tests the Memory::Check function.
     * @param[in] size is the size which must be allocated.
     * @return true if the Memory::Check function returns true on the memory previously allocated and false in
     * case of a NULL pointer input
     */
    bool TestCheck(uint32 size);

    /**
     * @brief Tests the Memory::Copy function.
     * @details Copies an area of memory checking if the number of size specified are copied correctly.
     * @return true if the memory is copied correctly.
     */
    bool TestCopy();

    /**
     * @brief Tests the Memory::Copy function with size = 0.
     * @return true if the Memory::Copy function returns true and no data is copied.
     */
    bool TestCopyZeroSize();

    /**
     * @brief Tests the Memory::Copy function with a NULL pointer.
     * @return true if the Memory::Copy function returns false.
     */
    bool TestCopyNullPointer();

    /**
     * @brief Tests the Memory::Move function.
     * @return true if an area of memory can be successfully moved.
     */
    bool TestMove();

    /**
     * @brief Tests the Memory::Move function with size = 0.
     * @return true if the Memory::Move function returns true and no data is moved.
     */
    bool TestMoveZeroSize();

    /**
     * @brief Tests the Memory::Move function with a NULL pointer.
     * @return true if the Memory::Move function returns false.
     */
    bool TestMoveNullPointer();

    /**
     * @brief Tests the Memory::Compare function.
     * @details Exercises the Compares Memory::Compare function with several patterns.
     * @return true if all the patterns return as expected.
     */
    bool TestCompare();

    /**
     * @brief Tests the Memory::Set function.
     * @details Sets a list of characters in the beginning of a memory area and another list of characters at its end.
     * @return true if the memory is set as specified above.
     */
    bool TestSet();

    /**
     * @brief Tests the Memory::Set function with size = 0.
     * @return true if the Memory::Set function returns true and no data is moved.
     */
    bool TestSetZeroSize();

    /**
     * @brief Tests the Memory::Search function, searching for a character saved on a memory area .
     * @return true if the the character can be found.
     */
    bool TestSearch();

    /**
     * @brief Tests the Memory::Search function, searching for a non-existing character on a memory area .
     * @return true if the the character cannot be found.
     */
    bool TestSearchNotInBuffer();

    /**
     * @brief Tests the Memory::Search function looking for a character that exists but is out of the searching bounds.
     * @return true if the Memory::Search returns false.
     */
    bool TestSearchOutOfRanges();

    /**
     * @brief Tests the Memory::Search function with size = 0.
     * @return true if the Memory::Search function returns false for size = 0.
     */
    bool TestSearchZeroSize();

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYTEST_H_ */

