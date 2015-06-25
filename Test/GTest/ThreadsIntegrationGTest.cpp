/**
 * @file ThreadsIntegrationGTest.cpp
 * @brief Source file for class ThreadsIntegrationGTest
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
 * the class ThreadsIntegrationGTest (public, protected, and private). Be aware that some 
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
#include "ThreadsIntegrationTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class ThreadsIntegrationGTest: public ::testing::Test {
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

TEST_F(ThreadsIntegrationGTest,BeginThreadTest) {
    ThreadsIntegrationTest beginThread;
    ASSERT_TRUE(beginThread.BeginThread(100));
}

TEST_F(ThreadsIntegrationGTest,PrioritiesThreadTest) {
    ThreadsIntegrationTest prioritiesThread;
    ASSERT_TRUE(prioritiesThread.Priorities());
}

TEST_F(ThreadsIntegrationGTest,PrioritiesPropagationStartFirst) {
    ThreadsIntegrationTest prioritiesPropagationStartFirst;
    ASSERT_TRUE(prioritiesPropagationStartFirst.PrioritiesPropagationStartFirst());
}

TEST_F(ThreadsIntegrationGTest,ThreadIdTest) {
    ThreadsIntegrationTest threadIdTest;
    ASSERT_TRUE(threadIdTest.ThreadIdTest());
}

TEST_F(ThreadsIntegrationGTest,ThreadKillTest) {
    ThreadsIntegrationTest threadKillTest;
    ASSERT_TRUE(threadKillTest.ThreadKillTest());
}

TEST_F(ThreadsIntegrationGTest,ThreadNameTest) {
    ThreadsIntegrationTest threadNameTest;
    ASSERT_TRUE(threadNameTest.ThreadNameTest());
}

TEST_F(ThreadsIntegrationGTest,ThreadEndTest) {
    ThreadsIntegrationTest threadEndTest;
    ASSERT_TRUE(threadEndTest.ThreadEndTest());
}

TEST_F(ThreadsIntegrationGTest,CpuRunTest) {
    ThreadsIntegrationTest cpuRunTest;
    ASSERT_TRUE(cpuRunTest.CpuRunTest());
}

