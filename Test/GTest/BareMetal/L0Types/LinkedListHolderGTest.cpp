/**
 * @file LinkedListHolderGTest.cpp
 * @brief Source file for class LinkedListHolderGTest
 * @date 06/08/2015
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
 * the class LinkedListHolderGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


#include "LinkedListHolderTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L0Types_LinkedListHolderTest,TestConstructor) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestConstructor());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestDestructor) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestDestructor());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestReset) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestReset(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestCleanup) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestCleanup(32));
}


TEST(BareMetal_L0Types_LinkedListHolderTest,TestList) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestList(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListSize) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListSize(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestFastListInsertSingle) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestFastListInsertSingle(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListInsert) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListInsert(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListInsertSortedSorter) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListInsertSortedSorter(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListInsertNullSorter) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListInsertNullSorter(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListInsertIndex) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListInsertIndex(10, 32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListInsertIndexOutOfBounds) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListInsertIndex(35, 32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListAdd) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListAdd(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListAddL) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListAddL(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListSearch) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListSearch());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListSearchFilter) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListSearchFilter());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListExtract) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListExtract());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListExtractSearchFilter) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListExtractSearchFilter());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListExtractIndex) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListExtractIndex());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListDelete) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListDelete());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListDeleteSearchFilter) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListDeleteSearchFilter());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListBSortSorter) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListBSortSorter(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListPeek) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListPeek(32));
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListIterateIterator) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListIterateIterator());
}

TEST(BareMetal_L0Types_LinkedListHolderTest,TestListSafeDelete) {
    LinkedListHolderTest myLinkedListHolderTest;
    ASSERT_TRUE(myLinkedListHolderTest.TestListSafeDelete());
}
