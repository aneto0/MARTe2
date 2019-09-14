/**
 * @file ThreadsGTest.cpp
 * @brief Source file for class ThreadsGTest
 * @date 25/06/2015
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
 * the class ThreadsGTest (public, protected, and private). Be aware that some 
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
#include "ThreadsTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(ThreadsGTest,TestBeginThread) {
    ASSERT_TRUE(ThreadsTest::TestBeginThread("hello", THREADS_DEFAULT_STACKSIZE, 32));
}

TEST(ThreadsGTest,TestBeginThreadStacksize0) {
    ASSERT_FALSE(ThreadsTest::TestBeginThread("hello", 0, 1));
}

TEST(ThreadsGTest,TestBeginThreadNullFunction) {
    ASSERT_TRUE(ThreadsTest::TestBeginThreadNullFunction("hello"));
}

TEST(ThreadsGTest,TestIsAlive) {
    ASSERT_TRUE(ThreadsTest::TestIsAlive(32));
}

TEST(ThreadsGTest,TestIsAliveAfterkill) {
    ASSERT_TRUE(ThreadsTest::TestIsAlive(32));
}

TEST(ThreadsGTest,TestKillInvalidID) {
    ASSERT_TRUE(ThreadsTest::TestKillInvalidID());
}

TEST(ThreadsGTest,TestKill) {
    ASSERT_TRUE(ThreadsTest::TestKill(32));
}

// Failure where the thread state is supported!
TEST(ThreadsGTest,TestGetState) {
    ASSERT_TRUE(ThreadsTest::TestGetState());
}

TEST(ThreadsGTest,TestId) {
    ASSERT_TRUE(ThreadsTest::TestId(32));
}

TEST(ThreadsGTest,TestGetCPUs) {
    ASSERT_TRUE(ThreadsTest::TestGetCPUs());
}

TEST(ThreadsGTest,TestName) {
    ASSERT_TRUE(ThreadsTest::TestName("Hello", 10));
}

TEST(ThreadsGTest,TestNameNull) {
    ASSERT_TRUE(ThreadsTest::TestNameNull());
}

TEST(ThreadsGTest,TestNumberOfThreads) {
    ASSERT_TRUE(ThreadsTest::TestNumberOfThreads(32));
}

TEST(ThreadsGTest,TestFindByIndex) {
    ASSERT_TRUE(ThreadsTest::TestFindByIndex());
}

TEST(ThreadsGTest,TestGetThreadInfoCopy) {
    ASSERT_TRUE(ThreadsTest::TestGetThreadInfoCopy(32, "Hello"));
}

TEST(ThreadsGTest,TestGetThreadInfoCopyInvalidID) {
    ASSERT_TRUE(ThreadsTest::TestGetThreadInfoCopyInvalidID());
}

TEST(ThreadsGTest,TestFindByName) {
    ASSERT_TRUE(ThreadsTest::TestFindByName(10, "Hello", 2));
}

TEST(ThreadsGTest,TestPriority) {
    ASSERT_TRUE(ThreadsTest::TestPriority());
}

