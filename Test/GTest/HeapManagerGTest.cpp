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
	
TEST(HeapManagerGTest,TestFree) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFree());
}

TEST(HeapManagerGTest,TestRealloc) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestRealloc());
}

TEST(HeapManagerGTest,TestAddHeap) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestAddHeap());
}

TEST(HeapManagerGTest,TestFindHeapByName) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFindHeapByName());
}

TEST(HeapManagerGTest,TestFindHeapByAddress) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestFindHeapByAddress());
}

TEST(HeapManagerGTest,TestRemoveHeap) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestRemoveHeap());
}

TEST(HeapManagerGTest,TestDuplicateDefault) {
    HeapManagerTest myHeapManagerTest;
    ASSERT_TRUE(myHeapManagerTest.TestDuplicateDefault());
}
