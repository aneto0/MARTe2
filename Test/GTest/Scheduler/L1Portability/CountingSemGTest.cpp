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
#include "CountingSemTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(Scheduler_L1Portability_CountingSemGTest,TestConstructor) {
    CountingSemTest semTest;
    ASSERT_TRUE(semTest.TestConstructor());
}

TEST(Scheduler_L1Portability_CountingSemGTest,TestDestructor) {
    CountingSemTest semTest;
    ASSERT_TRUE(semTest.TestDestructor());
}

TEST(Scheduler_L1Portability_CountingSemGTest,TestCreate) {
    CountingSemTest semTest;
    ASSERT_TRUE(semTest.TestCreate(10));
}

TEST(Scheduler_L1Portability_CountingSemGTest,TestWaitForAll) {
    CountingSemTest semTest;
    ASSERT_TRUE(semTest.TestWaitForAll(10, 1000));
}



TEST(Scheduler_L1Portability_CountingSemGTest,TestForceReset) {
    CountingSemTest semTest;
    ASSERT_TRUE(semTest.TestForceReset());
}

TEST(Scheduler_L1Portability_CountingSemGTest,TestForcePass) {
    CountingSemTest semTest;
    ASSERT_TRUE(semTest.TestForcePass());
}
/*
TEST(Scheduler_L1Portability_EventSemGTest,TestWaitTimeoutNoTimeout) {
    EventSemTest eventSemTesti;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutNoTimeout(32));
}

TEST(Scheduler_L1Portability_EventSemGTest,TestWaitTimeoutSuccess) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutSuccess(32));
}

TEST(Scheduler_L1Portability_EventSemGTest,TestWaitTimeoutFailure) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutFailure(32));
}

TEST(Scheduler_L1Portability_EventSemGTest,TestWaitTimeoutFailureFollowedBySuccess) {
    EventSemTest eventSemTest;
    ASSERT_TRUE(eventSemTest.TestWaitTimeoutFailureFollowedBySuccess(32));
}
*/
