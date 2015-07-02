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
#include "EventSemTest.h"

class EventSemGTest: public ::testing::Test {
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

TEST_F(EventSemGTest,TestCreate) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestCreate());
}

TEST_F(EventSemGTest,TestClose) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestClose());
}

TEST_F(EventSemGTest,TestCopyConstructor) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestCopyConstructor());
}

TEST_F(EventSemGTest,TestWaitSimple) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitSimple());
}

TEST_F(EventSemGTest,TestPostSimple) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestPostSimple());
}

TEST_F(EventSemGTest,TestResetSimple) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestResetSimple());
}

TEST_F(EventSemGTest,TestWait) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWait(500));
}

TEST_F(EventSemGTest,TestWaitNoTimeout) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitNoTimeout(32));
}

TEST_F(EventSemGTest,TestWaitTimeoutSuccess) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutSuccess(32));
}

TEST_F(EventSemGTest,TestWaitTimeoutFailure) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutFailure(32));
}

TEST_F(EventSemGTest,TestWaitTimeoutFailureFollowedBySuccess) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutFailureFollowedBySuccess(32));
}
