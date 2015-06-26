/**
 * @file MutexIntegrationGTest.cpp
 * @brief Source file for class MutexIntegrationGTest
 * @date 25/06/2015
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
 * the class MutexIntegrationGTest (public, protected, and private). Be aware that some 
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
#include "MutexIntegrationTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/





class MutexIntegrationGTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor
        // (right before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test
        // (right before the destructor).
    }
};

TEST_F(MutexIntegrationGTest,TestSync) {
    MutexIntegrationTest mutextest;
    ASSERT_TRUE(mutextest.TestSync(10));
}

TEST_F(MutexIntegrationGTest,TestSyncTimeout) {
    MutexIntegrationTest mutextest;
    ASSERT_TRUE(mutextest.TestSyncTimeout(2000, 10));
}

TEST_F(MutexIntegrationGTest,TestSyncFast) {
    MutexIntegrationTest mutextest;
    ASSERT_TRUE(mutextest.TestSyncFast(10));
}

TEST_F(MutexIntegrationGTest,TestSyncTimeoutFast) {
    MutexIntegrationTest mutextest;
    ASSERT_TRUE(mutextest.TestSyncTimeoutFast(2000, 10));
}

TEST_F(MutexIntegrationGTest,FakeLock) {
    MutexIntegrationTest mutextest;
    ASSERT_TRUE(mutextest.FakeLock());
}

TEST_F(MutexIntegrationGTest,DeadLock) {
    MutexIntegrationTest mutextest;
    ASSERT_TRUE(mutextest.DeadLock());
}

TEST_F(MutexIntegrationGTest,KillWithLock) {
    MutexIntegrationTest mutextest;
    ASSERT_TRUE(mutextest.KillWithLock());
}



