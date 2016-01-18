/**
 * @file MemoryGTest.cpp
 * @brief Source file for class MemoryGTest
 * @date 29/06/2015
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
 * the class MemoryGTest (public, protected, and private). Be aware that some
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
#include "EventSemTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(EventSemGTest,TestConstructor) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestConstructor());
}

TEST(EventSemGTest,TestDestructor) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestDestructor());
}

TEST(EventSemGTest,TestCreate) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestCreate());
}

TEST(EventSemGTest,TestClose) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestClose());
}

TEST(EventSemGTest,TestCopyConstructor) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestCopyConstructor());
}

TEST(EventSemGTest,TestIsClosed) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestIsClosed());
}

TEST(EventSemGTest,TestCopyConstructor2Sem) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestCopyConstructor2Sem());
}

TEST(EventSemGTest,TestWait) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWait());
}

TEST(EventSemGTest,TestResetWait) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestResetWait());
}

TEST(EventSemGTest,TestPost) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestPost());
}

TEST(EventSemGTest,TestReset) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestReset());
}

TEST(EventSemGTest,TestWaitTimeout) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWait(500));
}
/*
TEST(EventSemGTest,TestWaitTimeoutNoTimeout) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutNoTimeout(32));
}

TEST(EventSemGTest,TestWaitTimeoutSuccess) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutSuccess(32));
}

TEST(EventSemGTest,TestWaitTimeoutFailure) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutFailure(32));
}

TEST(EventSemGTest,TestWaitTimeoutFailureFollowedBySuccess) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutFailureFollowedBySuccess(32));
}
*/
