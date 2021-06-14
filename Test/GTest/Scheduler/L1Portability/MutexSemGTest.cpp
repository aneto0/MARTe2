/**
 * @file MutexSemGTest.cpp
 * @brief Source file for class MutexSemGTest
 * @date 10/07/2015
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
 * the class MutexSemGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MutexSemTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(Scheduler_L1Portability_MutexSemGTest,TestDefaultConstructor) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestDefaultConstructor());
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestCreateNoRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(false));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestCreateRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestCloseNoRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(false));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestCloseRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestClose(true));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestLock) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(16, TTInfiniteWait));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestLockWithFiniteTimeout) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLock(1, 10000000));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestUnLock) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock(50, TTInfiniteWait));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestUnLockWithFiniteTimeout) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestUnLock(50, 10000000));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestLockWithSmallFiniteTimeoutToFail) {
    MutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestLock(50, 1));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestUnLockWithSmallFiniteTimeoutToFail) {
    MutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestUnLock(50, 1));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestLockErrorCode) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLockErrorCode());
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestIsClosed) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestIsClosed());
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestIsRecursive) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestIsRecursive());
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestRecursiveOn) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive(true));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestRecursiveOff) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive(false));
}

TEST(Scheduler_L1Portability_MutexSemGTest,TestCopyConstructor) {
    MutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCopyConstructor());
}
