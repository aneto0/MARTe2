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
 *
 **/

#include <limits.h>

#include "StaticListHolderTest.h"
#include "gtest/gtest.h"

class StaticListHolderGTest: public ::testing::Test {
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

TEST_F(StaticListHolderGTest, ListInt32) {
    StaticListHolderTest listInt32;
    ASSERT_TRUE(listInt32.ListInt32());
}

TEST_F(StaticListHolderGTest, ListInt64) {
    StaticListHolderTest listInt64;
    ASSERT_TRUE(listInt64.ListInt64());
}

TEST_F(StaticListHolderGTest, ListFloat) {
    StaticListHolderTest listFloat;
    ASSERT_TRUE(listFloat.ListFloat());
}

TEST_F(StaticListHolderGTest, ListDouble) {
    StaticListHolderTest listDouble;
    ASSERT_TRUE(listDouble.ListDouble());
}

TEST_F(StaticListHolderGTest, SlhSize2) {
    StaticListHolderTest slhSize2;
    ASSERT_TRUE(slhSize2.SlhSize2());
}

TEST_F(StaticListHolderGTest, SetAccessTimeoutTest) {
    StaticListHolderTest setAccessTimeoutTest;
    ASSERT_TRUE(setAccessTimeoutTest.SetAccessTimeoutTest());
}

TEST_F(StaticListHolderGTest, ListAddTest) {
    StaticListHolderTest listAddTest;
    ASSERT_TRUE(listAddTest.ListAddTest());
}

TEST_F(StaticListHolderGTest, AddFirstPosition) {
    StaticListHolderTest addFirstPosition;
    ASSERT_TRUE(addFirstPosition.AddFirstPosition());
}

TEST_F(StaticListHolderGTest, ListExtractTest) {
    StaticListHolderTest listExtractTest;
    ASSERT_TRUE(listExtractTest.ListExtractTest());
}

