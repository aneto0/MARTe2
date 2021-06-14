/**
 * @file FastPollingMutexSemGTest.cpp
 * @brief Source file for class FastPollingMutexSemGTest1
 * @date 23/07/2015
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
 * the class FastPollingMutexSemGTest1 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "FastPollingMutexSemTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestConstructor) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestConstructor());
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestConstructorFromExternal) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestConstructorFromExternal());
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestCreateNoLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(false));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestCreateLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestCreateNoLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreateExternal(false));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestCreateLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreateExternal(true));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(32, TTInfiniteWait));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastLockWithFiniteTimeout) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(32, 10000000));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockExternal(32, TTInfiniteWait));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastLockWithFiniteTimeout_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockExternal(32, 10000000));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastUnLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(32, TTInfiniteWait));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastUnLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLockExternal(32, TTInfiniteWait));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastUnLockWithFiniteTimeout) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(32, 10000000));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastUnLockWithFiniteTimeout_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLockExternal(32, 10000000));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastLockWithSmallFiniteTimeoutToFail) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastLock(32, 1));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastLockWithSmallFiniteTimeoutToFail_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastLockExternal(32, 1));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastUnLockWithSmallFiniteTimeoutToFail) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastUnLock(32, 1));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastUnLockWithSmallFiniteTimeoutToFail_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastUnLockExternal(32, 1));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastLockErrorCode) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockErrorCode());
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastTryLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastTryLock(32));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestFastTryLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastTryLockExternal(32));
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestLocked) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLocked());
}

TEST(BareMetal_L1Portability_FastPollingMutexSemGTest,TestRecursive) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive());
}
