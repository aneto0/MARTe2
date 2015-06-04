/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the 
 Licence.
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied.
 * See the Licence  
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/
#include <limits.h>
#include "gtest/gtest.h"
#include "ThreadsTest.h"

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

TEST_F(ThreadsGTest,BeginThreadTest) {
    ThreadsTest beginThread;
    ASSERT_TRUE(beginThread.BeginThread(100));
}


TEST_F(ThreadsGTest,PrioritiesThreadTest) {
    ThreadsTest prioritiesThread;
    ASSERT_TRUE(prioritiesThread.Priorities());
}

TEST_F(ThreadsGTest,PrioritiesPropagationStartFirst) {
    ThreadsTest prioritiesPropagationStartFirst;
    ASSERT_TRUE(prioritiesPropagationStartFirst.PrioritiesPropagationStartFirst());
}


TEST_F(ThreadsGTest,ThreadIdTest) {
    ThreadsTest threadIdTest;
    ASSERT_TRUE(threadIdTest.ThreadIdTest());
}


TEST_F(ThreadsGTest,ThreadKillTest) {
    ThreadsTest threadKillTest;
    ASSERT_TRUE(threadKillTest.ThreadKillTest());
}

TEST_F(ThreadsGTest,ThreadNameTest) {
    ThreadsTest threadNameTest;
    ASSERT_TRUE(threadNameTest.ThreadNameTest());
}

TEST_F(ThreadsGTest,ThreadEndTest) {
    ThreadsTest threadEndTest;
    ASSERT_TRUE(threadEndTest.ThreadEndTest());
}


TEST_F(ThreadsGTest,CpuRunTest) {
    ThreadsTest cpuRunTest;
    ASSERT_TRUE(cpuRunTest.CpuRunTest());
}

/*
TEST_F(ThreadsGTest,Prova) {
    ThreadsTest prova;
    ASSERT_TRUE(prova.Prova());
}
*/

