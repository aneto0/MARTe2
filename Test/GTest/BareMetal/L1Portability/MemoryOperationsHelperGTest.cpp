/**
 * @file MemoryOperationsHelperGTest.cpp
 * @brief Source file for class MemoryOperationsHelperGTest
 * @date 28/08/2015
 * @author LlorençCapellà
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

 * @details This source file contains the definition of all the methods for
 * the class MemoryOperationsHelperGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "MemoryOperationsHelperTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestCopy) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestCopy());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestCopyZeroSize) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestCopyZeroSize());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestCopyNullPointer) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestCopyNullPointer());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestMove) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestMove());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestMoveZeroSize) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestMoveZeroSize());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestMoveNullPointer) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestMoveNullPointer());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestCompare) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestCompare());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestSet) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestSet());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestSetZeroSize) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestSetZeroSize());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestSearch) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestSearch());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestSearchNotInBuffer) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestSearchNotInBuffer());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestSearchOutOfRanges) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestSearchOutOfRanges());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestSearchZeroSize) {
    MemoryOperationsHelperTest MyMemoryOperationsHelperTest;
    ASSERT_TRUE(MyMemoryOperationsHelperTest.TestSearchZeroSize());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestInterleavedToFlat) {
    MemoryOperationsHelperTest test;
    ASSERT_TRUE(test.TestInterleavedToFlat());
}

TEST(BareMetal_L1Portability_MemoryOperationsHelperGTest,TestFlatToInterleaved) {
    MemoryOperationsHelperTest test;
    ASSERT_TRUE(test.TestFlatToInterleaved());
}

