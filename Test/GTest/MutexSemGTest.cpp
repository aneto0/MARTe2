/**
 * @file MutexSemGTest1.cpp
 * @brief Source file for class MutexSemGTest1
 * @date Jul 10, 2015
 * @author llcapella
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
 * the class MutexSemGTest1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <MutexSemTest.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(MutexSemGTest,TestDefaultConstructor) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestDefaultConstructor());
}

TEST(MutexSemGTest,TestCreateNoRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(false));
}

TEST(MutexSemGTest,TestCreateRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true));
}

TEST(MutexSemGTest,TestCloseNoRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(false));
}

TEST(MutexSemGTest,TestCloseRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(true));
}

TEST(MutexSemGTest,TestLock) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(50, TTInfiniteWait));
}

TEST(MutexSemGTest,TestLockWithFiniteTimeout) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(1, 10000000));
}

TEST(MutexSemGTest,TestUnLock) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock(50, TTInfiniteWait));
}

TEST(MutexSemGTest,TestUnLockWithFiniteTimeout) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock(50, 10000000));
}

TEST(MutexSemGTest,TestLockWithSmallFiniteTimeoutToFail) {
    MutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestLock(500, 1));
}

TEST(MutexSemGTest,TestUnLockWithSmallFiniteTimeoutToFail) {
    MutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestUnLock(500, 1));
}

TEST(MutexSemGTest,TestLockErrorCode) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLockErrorCode());
}

TEST(MutexSemGTest,TestIsClosed) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestIsClosed());
}

TEST(MutexSemGTest,TestIsRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestIsRecursive());
}

TEST(MutexSemGTest,TestRecursiveOn) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive(true));
}

TEST(MutexSemGTest,TestRecursiveOff) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive(false));
}

TEST(MutexSemGTest,TestCopyConstructor) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCopyConstructor());
}

