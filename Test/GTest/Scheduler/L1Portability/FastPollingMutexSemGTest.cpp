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

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestSupport.h"
#include "FastPollingMutexSemTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FastPollingMutexSemGTest,TestConstructor) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestConstructor());
}

TEST(FastPollingMutexSemGTest,TestConstructorFromExternal) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestConstructorFromExternal());
}

TEST(FastPollingMutexSemGTest,TestCreateNoLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(false));
}

TEST(FastPollingMutexSemGTest,TestCreateLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreate(true));
}

TEST(FastPollingMutexSemGTest,TestCreateNoLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreateExternal(false));
}

TEST(FastPollingMutexSemGTest,TestCreateLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestCreateExternal(true));
}

TEST(FastPollingMutexSemGTest,TestFastLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(32, MilliSeconds::Infinite));
}

TEST(FastPollingMutexSemGTest,TestFastLockWithFiniteTimeout) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLock(32, MilliSeconds(100000,Units::ms)));
}

TEST(FastPollingMutexSemGTest,TestFastLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockExternal(32, MilliSeconds::Infinite));
}

TEST(FastPollingMutexSemGTest,TestFastLockWithFiniteTimeout_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockExternal(32, MilliSeconds(100000,Units::ms)));
}

TEST(FastPollingMutexSemGTest,TestFastUnLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(32, MilliSeconds::Infinite));
}

TEST(FastPollingMutexSemGTest,TestFastUnLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLockExternal(32, MilliSeconds::Infinite));
}

TEST(FastPollingMutexSemGTest,TestFastUnLockWithFiniteTimeout) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLock(32, MilliSeconds(100000,Units::ms)));
}

TEST(FastPollingMutexSemGTest,TestFastUnLockWithFiniteTimeout_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastUnLockExternal(32, MilliSeconds(100000,Units::ms)));
}

TEST(FastPollingMutexSemGTest,TestFastLockWithSmallFiniteTimeoutToFail) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastLock(32, MilliSeconds(1,Units::ms)));
}

TEST(FastPollingMutexSemGTest,TestFastLockWithSmallFiniteTimeoutToFail_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastLockExternal(32, MilliSeconds(1,Units::ms)));
}

TEST(FastPollingMutexSemGTest,TestFastUnLockWithSmallFiniteTimeoutToFail) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastUnLock(32, MilliSeconds(1,Units::ms)));
}

TEST(FastPollingMutexSemGTest,TestFastUnLockWithSmallFiniteTimeoutToFail_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_FALSE(mutextest.TestFastUnLockExternal(32, MilliSeconds(1,Units::ms)));
}

TEST(FastPollingMutexSemGTest,TestFastLockErrorCode) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastLockErrorCode());
}

TEST(FastPollingMutexSemGTest,TestFastTryLock) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastTryLock(32));
}

TEST(FastPollingMutexSemGTest,TestFastTryLock_External) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestFastTryLockExternal(32));
}

TEST(FastPollingMutexSemGTest,TestLocked) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestLocked());
}

TEST(FastPollingMutexSemGTest,TestRecursive) {
    FastPollingMutexSemTest mutextest;
    ASSERT_TRUE(mutextest.TestRecursive());
}
