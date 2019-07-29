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
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestBeginThread("hello", THREADS_DEFAULT_STACKSIZE, 32));
}

TEST(ThreadsGTest,TestBeginThreadStacksize0) {
    ThreadsTest myUnitTest;
    ASSERT_FALSE(myUnitTest.TestBeginThread("hello", 0, 32));
}

TEST(ThreadsGTest,TestBeginThreadNullFunction) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestBeginThreadNullFunction("hello"));
}

TEST(ThreadsGTest,TestIsAlive) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestIsAlive(32));
}

TEST(ThreadsGTest,TestIsAliveAfterkill) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestIsAlive(32));
}

TEST(ThreadsGTest,TestKillInvalidID) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestKillInvalidID());
}

TEST(ThreadsGTest,TestKill) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestKill(32));
}

// Failure where the thread state is supported!
TEST(ThreadsGTest,TestGetState) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestGetState());
}

TEST(ThreadsGTest,TestId) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestId(32));
}

TEST(ThreadsGTest,TestGetCPUs) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestGetCPUs());
}

TEST(ThreadsGTest,TestName) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestName("Hello", 10));
}

TEST(ThreadsGTest,TestNameNull) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestNameNull());
}

TEST(ThreadsGTest,TestNumberOfThreads) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestNumberOfThreads(32));
}

TEST(ThreadsGTest,TestFindByIndex) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestFindByIndex());
}

TEST(ThreadsGTest,TestGetThreadInfoCopy) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestGetThreadInfoCopy(32, "Hello"));
}

TEST(ThreadsGTest,TestGetThreadInfoCopyInvalidID) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestGetThreadInfoCopyInvalidID());
}

TEST(ThreadsGTest,TestFindByName) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestFindByName(10, "Hello", 2));
}

TEST(ThreadsGTest,TestPriority) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestPriority());
}

