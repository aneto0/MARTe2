/**
 * @file FastPollingEventSemGTest.cpp
 * @brief Source file for class FastPollingEventSemGTest
 * @date 24/03/2016
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
 * the class FastPollingEventSemGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include <limits.h>

#include "../../../Core/Scheduler/L1Portability/FastPollingEventSemTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FastPollingEventSemGTest,TestConstructor) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestConstructor());
}

TEST(FastPollingEventSemGTest,TestConstructorFromExternal) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestConstructorFromExternal());
}

TEST(FastPollingEventSemGTest,TestCreateWait) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestCreate(true));
}

TEST(FastPollingEventSemGTest,TestCreateNoWait) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestCreate(false));
}

TEST(FastPollingEventSemGTest,TestCreateWait_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestCreateExternal(true));
}

TEST(FastPollingEventSemGTest,TestCreateNoWait_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestCreateExternal(false));
}

TEST(FastPollingEventSemGTest,TestFastWait) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWait(32, TTInfiniteWait));
}

TEST(FastPollingEventSemGTest,TestFastWaitWithFiniteTimeout) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWait(32, 10000000));
}


TEST(FastPollingEventSemGTest,TestFastWait_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWaitExternal(32, TTInfiniteWait));
}


TEST(FastPollingEventSemGTest,TestFastWaitWithFiniteTimeout_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWaitExternal(32, 10000000));
}



TEST(FastPollingEventSemGTest,TestFastPost) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastPost(32, TTInfiniteWait));
}

TEST(FastPollingEventSemGTest,TestFastPostWithFiniteTimeout) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastPost(32, 10000000));
}


TEST(FastPollingEventSemGTest,TestFastPost_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastPostExternal(32, TTInfiniteWait));
}


TEST(FastPollingEventSemGTest,TestFastPostWithFiniteTimeout_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastPostExternal(32, 10000000));
}

TEST(FastPollingEventSemGTest,TestFastResetWait) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastResetWait(32));
}

TEST(FastPollingEventSemGTest,TestFastResetWait_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastResetWaitExternal(32));
}

TEST(FastPollingEventSemGTest,TestFastWaitErrorCode) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWaitErrorCode());
}

