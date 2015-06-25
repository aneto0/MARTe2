/**
 * @file ThreadsGTest.cpp
 * @brief Source file for class ThreadsGTest
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

#include "gtest/gtest.h"
#include "ThreadsTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class ThreadsGTest: public ::testing::Test {
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

TEST_F(ThreadsGTest,TestBegin) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestBegin("hello", 64));
}

TEST_F(ThreadsGTest,TestEnd) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestEnd());
}

TEST_F(ThreadsGTest,TestPriority) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestPriority());
}

TEST_F(ThreadsGTest,TestState) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestState());
}

TEST_F(ThreadsGTest,TestId) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestId());
}

TEST_F(ThreadsGTest,TestCPUs) {
    ThreadsTest myUnitTest;
    ASSERT_TRUE(myUnitTest.TestCPUs());
}

