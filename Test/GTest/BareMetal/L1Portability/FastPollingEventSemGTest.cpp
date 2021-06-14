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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "FastPollingEventSemTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestConstructor) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestConstructor());
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestConstructorFromExternal) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestConstructorFromExternal());
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestCreateWait) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestCreate(true));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestCreateNoWait) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestCreate(false));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestCreateWait_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestCreateExternal(true));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestCreateNoWait_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestCreateExternal(false));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastWait) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWait(32, TTInfiniteWait));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastWaitWithFiniteTimeout) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWait(32, 10000000));
}


TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastWait_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWaitExternal(32, TTInfiniteWait));
}


TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastWaitWithFiniteTimeout_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWaitExternal(32, 10000000));
}



TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastPost) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastPost(32, TTInfiniteWait));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastPostWithFiniteTimeout) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastPost(32, 10000000));
}


TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastPost_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastPostExternal(32, TTInfiniteWait));
}


TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastPostWithFiniteTimeout_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastPostExternal(32, 10000000));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastResetWait) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastResetWait(32));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastResetWait_External) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastResetWaitExternal(32));
}

TEST(BareMetal_L1Portability_FastPollingEventSemGTest,TestFastWaitErrorCode) {
    FastPollingEventSemTest eventTest;
    ASSERT_TRUE(eventTest.TestFastWaitErrorCode());
}

