/**
 * @file HeapManagerGTest.cpp
 * @brief Source file for class HeapManagerGTest
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

 * @details This source file contains the definition of all the methods for
 * the class HeapManagerGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HeapManagerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(HeapManagerGTest,TestMallocDefaultName) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestMallocDefaultName());
}

TEST(HeapManagerGTest,TestMallocSpecificName) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestMallocSpecificName());
}

TEST(HeapManagerGTest,TestMallocInvalidName) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestMallocInvalidName());
}
	
TEST(HeapManagerGTest,TestFree) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFree());
}

TEST(HeapManagerGTest,TestFreeInvalidPointer) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFreeInvalidPointer());
}

TEST(HeapManagerGTest,TestRealloc) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestRealloc());
}

TEST(HeapManagerGTest,TestReallocInvalidPointer) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestReallocInvalidPointer());
}

TEST(HeapManagerGTest,TestAddHeap) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestAddHeap());
}

TEST(HeapManagerGTest,TestAddHeapNULL) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestAddHeapNULL());
}

TEST(HeapManagerGTest,TestAddHeapRepetedHeap) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestAddHeapRepetedHeap());
}

TEST(HeapManagerGTest,TestAddHeapTooMuch) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestAddHeapTooMuch());
}

TEST(HeapManagerGTest,TestFindHeapByName) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFindHeapByName());
}

TEST(HeapManagerGTest,TestFindHeapByNameInvalidName) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFindHeapByNameInvalidName());
}

TEST(HeapManagerGTest,TestFindHeapByAddress) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFindHeapByAddress());
}

TEST(HeapManagerGTest,TestFindHeapByAddressInvalidAddress) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFindHeapByAddressInvalidAddress());
}


TEST(HeapManagerGTest,TestFindHeapByAddress2Heaps) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFindHeapByAddress2Heaps());
}

TEST(HeapManagerGTest,TestGetStandardHeap) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestGetStandardHeap());
}

TEST(HeapManagerGTest,TestRemoveHeap) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestRemoveHeap());
}

TEST(HeapManagerGTest,TestDuplicateDefault) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestDuplicateDefault());
}

TEST(HeapManagerGTest,TestDuplicateSpecificName) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestDuplicateSpecificName());
}

TEST(HeapManagerGTest,TestDuplicateString) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestDuplicateString());
}

TEST(HeapManagerGTest,TestDuplicateNoAllocationMemory) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestDuplicateNoAllocationMemory());
}
