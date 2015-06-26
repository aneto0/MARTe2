/**
 * @file EventSemIntegrationGTest.cpp
 * @brief Source file for class EventSemIntegrationGTest
 * @date 26/06/2015
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
 * the class EventSemIntegrationGTest (public, protected, and private). Be aware that some 
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
#include "EventSemIntegrationTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class EventSemIntegrationGTest: public ::testing::Test {
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

TEST_F(EventSemIntegrationGTest,WaitNoTimeoutTest) {
    EventSemIntegrationTest eventSemTest;
    ASSERT_TRUE(eventSemTest.WaitNoTimeoutTest(32));
}

TEST_F(EventSemIntegrationGTest,WaitTimeoutTestSuccess) {
    EventSemIntegrationTest eventSemTest;
    ASSERT_TRUE(eventSemTest.WaitTimeoutTestSuccess(32));
}

TEST_F(EventSemIntegrationGTest,WaitTimeoutTestFailure) {
    EventSemIntegrationTest eventSemTest;
    ASSERT_TRUE(eventSemTest.WaitTimeoutTestFailure(32));
}

TEST_F(EventSemIntegrationGTest,WaitTimeoutTestFailureFollowedBySuccess) {
    EventSemIntegrationTest eventSemTest;
    ASSERT_TRUE(eventSemTest.WaitTimeoutTestFailureFollowedBySuccess(32));
}

