/**
 * @file StaticListGTest.cpp
 * @brief Source file for class StaticListGTest
 * @date 07/08/2015
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

 * @details This source file contains the definition of all the methods for
 * the class StaticListGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "StaticListTest.h"
#include "StreamString.h"


using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

uint32 demoValues[] = { 3, 7, 2, 5, 45, 3, 6, 7, 47, 3, 74, 5, 32, 91, 8, 66, 9 };

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_StaticListGTest,TestDefaultConstructor) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestDefaultConstructor());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestAddOnEmptyList) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestAddOnEmptyList<25u>());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestAddOnNonEmptyList) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestAddOnNonEmptyList<25u>());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestAddOnFullList) {
    //WARN This test stresses the capacity of the list, so it takes a long time to finish.
    //StaticListTest<uint32,10,demoValues,sizeof(demoValues)/sizeof(uint32)> tester;
    //ASSERT_TRUE(tester.TestAddOnFullList<25u>());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestInsertOnEmptyList) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestInsertOnEmptyList<25u>());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestInsertOnNonEmptyListAtBeginning) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestInsertOnNonEmptyListAtBeginning<25u>());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestInsertOnNonEmptyListAtMiddle) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestInsertOnNonEmptyListAtMiddle<25u>());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestInsertOnNonEmptyListAtEnd) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestInsertOnNonEmptyListAtEnd<25u>());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestInsertOnNonEmptyListAtInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestInsertOnNonEmptyListAtInvalidPosition<5u>());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestExtractAtBeginning) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestExtractAtBeginning());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestExtractAtMiddle) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestExtractAtMiddle());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestExtractAtEnd) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestExtractAtEnd());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestExtractInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestExtractInvalidPosition());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestRemoveAtBeginning) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestRemoveAtBeginning());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestRemoveAtMiddle) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestRemoveAtMiddle());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestRemoveAtEnd) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestRemoveAtEnd());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestRemoveInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestRemoveInvalidPosition());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestConstantness) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestConstantness());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestGetSize) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestGetSize());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestGetCapacity) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestGetCapacity());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestPeek) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestPeek());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestPeekInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestPeekInvalidPosition());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestSquareOperator) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestSquareOperator());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestSquareOperatorInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestSquareOperatorInvalidPosition());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestSet) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestSet<5u> (2u));
}

TEST(BareMetal_L1Portability_StaticListGTest,TestCleanOnEmptyList) {
    StaticListTest<uint32, 10, demoValues, 0> tester;
    ASSERT_TRUE(tester.TestClean());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestCleanOnNonEmptyList) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestClean());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestGetAllocatedMemoryConstOnEmptyList) {
    StaticListTest<uint32, 10, demoValues, 0> tester;
    ASSERT_TRUE(tester.TestGetAllocatedMemoryConst());
}

TEST(BareMetal_L1Portability_StaticListGTest,TestGetAllocatedMemoryConstOnNonEmptyList) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)/sizeof(uint32)> tester;
    ASSERT_TRUE(tester.TestGetAllocatedMemoryConst());
}
