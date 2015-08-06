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

#include <limits.h>

#include "../Core/L1Objects/LinkedListableTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(LinkedListableGTest,TestConstructor) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestConstructor());
}

TEST(LinkedListableGTest,TestDestructor) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestDestructor());
}

TEST(LinkedListableGTest,TestSetGetNext) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSetGetNext());
}

TEST(LinkedListableGTest,TestSize) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSize());
}

TEST(LinkedListableGTest,TestBSortSorter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestBSortSorter());
}

TEST(LinkedListableGTest,TestBSortFn) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestBSortFn());
}

TEST(LinkedListableGTest,TestInsert) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsert(10, 10));
}

TEST(LinkedListableGTest,TestInsertEntireList) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertEntireList());
}

TEST(LinkedListableGTest,TestInsertSortedSorter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertSortedSorter(10));
}

TEST(LinkedListableGTest,TestInsertSortedFn) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertSortedFn(10));
}

TEST(LinkedListableGTest,TestInsertNullListInput) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertNullListInput());
}

TEST(LinkedListableGTest,TestInsertNullSorterInput) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestInsertNullSorterInput());
}

TEST(LinkedListableGTest,TestAdd) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestAdd(10));
}

TEST(LinkedListableGTest,TestAddL) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestAddL(10));
}

TEST(LinkedListableGTest,TestSearch) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSearch());
}

TEST(LinkedListableGTest,TestSearchFilter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSearchFilter());
}


TEST(LinkedListableGTest,TestSearchFn) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSearchFn());
}

TEST(LinkedListableGTest,TestSearchFilterPreviousElement) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestSearchFilterPreviousElement());
}

TEST(LinkedListableGTest,TestExtract) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestExtract());
}

TEST(LinkedListableGTest,TestExtractSearchFilter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestExtractSearchFilter());
}

TEST(LinkedListableGTest,TestDelete) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestDelete());
}

TEST(LinkedListableGTest,TestDeleteSearchFilter) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestDeleteSearchFilter());
}


TEST(LinkedListableGTest,TestDeleteSearchFn) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestDeleteSearchFn());
}
TEST(LinkedListableGTest,TestPeek) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestPeek());
}

TEST(LinkedListableGTest,TestIterateIterator) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestIterateIterator());
}


TEST(LinkedListableGTest,TestIterateFn) {
    LinkedListableTest myLinkedListableTest;
    ASSERT_TRUE(myLinkedListableTest.TestIterateFn());
}
