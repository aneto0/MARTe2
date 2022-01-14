/**
 * @file TimeoutTypeGTest.cpp
 * @brief Source file for class TimeoutGTest
 * @date 23/07/2015
 * @author Llorenç Capellà
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
 * the class TimeoutGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "TimeoutTypeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestConstructorDefault) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestConstructorDefault());
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestConstructorNotDefault) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestConstructorNotDefault(10));
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestConstructorInt32Constructor_P) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestConstructorNotDefault_2(7));
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestConstructorInt32Constructor_N) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestConstructorNotDefault_2(-7));
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestConstructoruInt64Constructor) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestConstructorNotDefault_3(1024));
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestSetTimeoutSec) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestSetTimeoutSec(1));
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestSetTimeoutHighResolutionTimerTicks) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestSetTimeoutHighResolutionTimerTicks());
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestHighResolutionTimerTicks) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestHighResolutionTimerTicks());
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestAutoSubstractOPerator) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestSubstractAssignOperator());
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestEqualComparison) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestEqualComparison());
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestDiffComparison) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestDiffComparison());
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestAssingOperator) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestAssignOperator());
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestIsFinite) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestIsFinite());
}

TEST(BareMetal_L1Portability_TimeoutTypeGTest,TestGetTimeoutMSec) {
    TimeoutTypeTest timeoutTypeTest;
    ASSERT_TRUE(timeoutTypeTest.TestGetTimeoutMSec());
}
