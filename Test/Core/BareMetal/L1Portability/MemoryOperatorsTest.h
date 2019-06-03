/**
 * @file MemoryOperatorsTest.h
 * @brief Header file for class MemoryOperatorsTest
 * @date 25/08/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class MemoryOperatorsTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L0PORTABILITY_MEMORYOPERATIONSHELPERTEST_H_
#define TEST_CORE_L0PORTABILITY_MEMORYOPERATIONSHELPERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class MemoryOperatorsTest {

public:

    /*
     * @brief Default constructor
     */
    MemoryOperatorsTest();

    /*
     * @brief Destructor
     */
    virtual ~MemoryOperatorsTest();

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
#if 0
    /**
     * @brief Tests the InterleavedToFlat function
     */
    bool TestInterleavedToFlat();

    /**
     * @brief Tests the FlatToInterleaved function
     */
    bool TestFlatToInterleaved();
#endif
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L0PORTABILITY_MEMORYOPERATIONSHELPERTEST_H_ */

