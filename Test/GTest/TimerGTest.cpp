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
#include "TimerTest.h"

class TimerGTest: public ::testing::Test {
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

TEST_F(TimerGTest,ConfigAndStartTimerTest) {
    TimerTest timerTest;
    ASSERT_TRUE(timerTest.ConfigAndStartTimerTest());
}

TEST_F(TimerGTest,ResetTimerTest) {
    TimerTest timerTest;
    ASSERT_TRUE(timerTest.ResetTimerTest());
}

TEST_F(TimerGTest,ConfigAndNotStartTimerTest) {
    TimerTest timerTest;
    ASSERT_TRUE(timerTest.ConfigAndNotStartTimerTest());
}
