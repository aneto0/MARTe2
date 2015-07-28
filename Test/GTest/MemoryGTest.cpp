/**
 * @file MemoryGTest.cpp
 * @brief Source file for class MemoryGTest
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

 * @details This source file contains the definition of all the methods for
 * the class MemoryGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "MemoryTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MemoryGTest,TestMalloc) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestMalloc(100));
}

TEST(MemoryGTest,TestFree) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestFree(100));
}

TEST(MemoryGTest,TestRealloc) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestRealloc(100, 10));
}

TEST(MemoryGTest,TestStringDup) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestStringDup("Hello"));
}

TEST(MemoryGTest,TestCheck) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestCheck(100));
}

TEST(MemoryGTest,TestCopy) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestCopy());
}

TEST(MemoryGTest,TestMove) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestMove());
}

TEST(MemoryGTest,TestCompare) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestCompare());
}

TEST(MemoryGTest,TestSet) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestSet());
}

TEST(MemoryGTest,TestSearch) {
    MemoryTest memorytest;
    ASSERT_TRUE(memorytest.TestSearch());
}

