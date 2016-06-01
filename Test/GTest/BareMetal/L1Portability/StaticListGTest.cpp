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

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StaticListTest.h"
#include "GeneralDefinitions.h"
#include "gtest/gtest.h"

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

TEST(StaticListGTest,TestDefaultConstructor) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestDefaultConstructor());
}

TEST(StaticListGTest,TestAddOnEmptyList) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestAddOnEmptyList<25u>());
}

TEST(StaticListGTest,TestAddOnNonEmptyList) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestAddOnNonEmptyList<25u>());
}

TEST(StaticListGTest,TestAddOnFullList) {
    //WARN This test stresses the capacity of the list, so it takes a long time to finish.
    //StaticListTest<uint32,10,demoValues,sizeof(demoValues)> tester;
    //ASSERT_TRUE(tester.TestAddOnFullList<25u>());
}

TEST(StaticListGTest,TestInsertOnEmptyList) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestInsertOnEmptyList<25u>());
}

TEST(StaticListGTest,TestInsertOnNonEmptyListAtBeginning) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestInsertOnNonEmptyListAtBeginning<25u>());
}

TEST(StaticListGTest,TestInsertOnNonEmptyListAtMiddle) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestInsertOnNonEmptyListAtMiddle<25u>());
}

TEST(StaticListGTest,TestInsertOnNonEmptyListAtEnd) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestInsertOnNonEmptyListAtEnd<25u>());
}

TEST(StaticListGTest,TestInsertOnNonEmptyListAtInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestInsertOnNonEmptyListAtInvalidPosition<5u>());
}

TEST(StaticListGTest,TestExtractAtBeginning) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestExtractAtBeginning());
}

TEST(StaticListGTest,TestExtractAtMiddle) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestExtractAtMiddle());
}

TEST(StaticListGTest,TestExtractAtEnd) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestExtractAtEnd());
}

TEST(StaticListGTest,TestExtractInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestExtractInvalidPosition());
}

TEST(StaticListGTest,TestRemoveAtBeginning) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestRemoveAtBeginning());
}

TEST(StaticListGTest,TestRemoveAtMiddle) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestRemoveAtMiddle());
}

TEST(StaticListGTest,TestRemoveAtEnd) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestRemoveAtEnd());
}

TEST(StaticListGTest,TestRemoveInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestRemoveInvalidPosition());
}

TEST(StaticListGTest,TestConstantness) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestConstantness());
}

TEST(StaticListGTest,TestGetSize) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestGetSize());
}

TEST(StaticListGTest,TestGetCapacity) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestGetCapacity());
}

TEST(StaticListGTest,TestPeek) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestPeek());
}

TEST(StaticListGTest,TestPeekInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestPeekInvalidPosition());
}

TEST(StaticListGTest,TestSquareOperator) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestSquareOperator());
}

TEST(StaticListGTest,TestSquareOperatorInvalidPosition) {
    StaticListTest<uint32, 10, demoValues, sizeof(demoValues)> tester;
    ASSERT_TRUE(tester.TestSquareOperatorInvalidPosition());
}
