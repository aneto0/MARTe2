/**
 * @file LinkedListableGTest.cpp
 * @brief Source file for class LinkedListableGTest
 * @date 05/08/2015
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
 * the class LinkedListableGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LinkedListableTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L0Types_LinkedListableGTest,TestConstructor) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestConstructor());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestDestructor) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestDestructor());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestSetGetNext) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSetGetNext());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestSize) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSize());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestBSortSorter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestBSortSorter());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestInsert) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsert(10, 10));
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestInsertEntireList) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertEntireList());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestInsertSortedSorter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertSortedSorter(10));
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestInsertNullListInput) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertNullListInput());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestInsertNullSorterInput) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertNullSorterInput());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestAdd) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestAdd(10));
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestAddL) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestAddL(10));
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestSearch) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSearch());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestSearchFilter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSearchFilter());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestSearchFilterPreviousElement) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSearchFilterPreviousElement());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestExtract) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestExtract());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestExtractSearchFilter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestExtractSearchFilter());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestDelete) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestDelete());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestDeleteSearchFilter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestDeleteSearchFilter());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestPeek) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestPeek());
}

TEST(BareMetal_L0Types_LinkedListableGTest,TestIterateIterator) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestIterateIterator());
}

