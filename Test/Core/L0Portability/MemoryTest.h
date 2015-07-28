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

public:

    /**
     * @brief Tests the Memory::Malloc function.
     * @details Allocates blocks of memory with different sizes and checks that the return pointer is different than zero.
     * @param[in] size is the number of integers to allocate.
     * @return true if all the tests returns as expected, false otherwise.
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
     * @details Checks the function passing also size = 0 (Memory::Free behavior) or pointer = NULL (Memory::Malloc) behavior.
     * @param[in] size1 is the number of integers to allocate with initial malloc.
     * @param[in] size2 is the additional memory which must be allocated.
     * @return true if all tests return as expected, false otherwise.
     */
    bool TestRealloc(uint32 size1,
                     uint32 size2);

    /**
     * @brief Tests the Memory::StringDup function.
     * @details Checks if the string is duplicated correctly and if the function returns NULL in case of NULL input.
     * @param[in] s is the string to duplicate.
     * @return true if the tests return as expected, false otherwise.
     */
    bool TestStringDup(const char8 *s);

    /**
     * @brief Tests the Memory::Check function.
     * @details Allocates a chunk of memory and check if the function return true on the memory allocated.
     * Then checks if the function returns false in case of null pointer input.
     * @param[in] size is the size which must be allocated.
     * @return true if all tests return as expected, false otherwise.
     */
    bool TestCheck(uint32 size);

    /**
     * @brief Tests the Memory::Copy function.
     * @details Copies a memory area with different size parameters and checks if the function returns false on case of null inputs.
     * @return true if the tests return as expected, false otherwise.
     */
    bool TestCopy();

    /**
     * @brief Tests the Memory::Copy function.
     * @details Copies a memory area with different size parameters checking if the two memory blocks after the operation are equal.
     * Then checks if the function returns false on case of null inputs.
     * @return true if the tests return as expected, false otherwise.
     */
    bool TestMove();

    /**
     * @brief Tests the Memory::Compare function.
     * @details Compares two memory areas checking if the result is correct.
     * @return true if the tests return as expected, false otherwise.
     */
    bool TestCompare();

    /**
     * @brief Tests the Memory::Set function.
     * @details Sets a character in the beginning of a memory area and another character at its end. Then checks if the memory is set as expected.
     * @return true if the tests return as expected, false otherwise.
     */
    bool TestSet();

    /**
     * @brief Tests the Memory::Search function.
     * @details Searches characters on a memory area checking the result in case of the character is in the buffer and in case of the
     * character not in the buffer. Then checks also if the function returns false in case of null input or not found characters
     * @return true if the tests return as expected, false otherwise.
     */
    bool TestSearch();

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYTEST_H_ */

