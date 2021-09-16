/**
 * @file TimeStampGTest.cpp
 * @brief Source file for class TimeStampGTest
 * @date 11/11/2015
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
 * the class TimeStampGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "TimeStampTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_TimeStampGTest, TestConstructor) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestConstructor());
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestSetMicroSeconds) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestSetMicroSeconds(123456));
    ASSERT_TRUE(myTimeStampTest.TestSetMicroSeconds(5000));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestGetMicroSeconds) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestGetMicroSeconds(123456));
    ASSERT_TRUE(myTimeStampTest.TestGetMicroSeconds(5000));
}


TEST(BareMetal_L1Portability_TimeStampGTest, TestSetSeconds) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestSetSeconds(32));
    ASSERT_TRUE(myTimeStampTest.TestSetMicroSeconds(61));
}


TEST(BareMetal_L1Portability_TimeStampGTest, TestGetSeconds) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestGetSeconds(32));
    ASSERT_TRUE(myTimeStampTest.TestGetMicroSeconds(61));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestSetMinutes) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestSetMinutes(32));
    ASSERT_TRUE(myTimeStampTest.TestSetMinutes(61));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestGetMinutes) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestGetMinutes(32));
    ASSERT_TRUE(myTimeStampTest.TestGetMinutes(61));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestSetHour) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestSetHour(11));
    ASSERT_TRUE(myTimeStampTest.TestSetHour(25));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestGetHour) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestGetHour(11));
    ASSERT_TRUE(myTimeStampTest.TestGetHour(25));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestSetDay) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestSetDay(11));
    ASSERT_TRUE(myTimeStampTest.TestSetDay(34));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestGetDay) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestGetDay(11));
    ASSERT_TRUE(myTimeStampTest.TestGetDay(34));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestSetMonth) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestSetMonth(10));
    ASSERT_TRUE(myTimeStampTest.TestSetMonth(13));
}


TEST(BareMetal_L1Portability_TimeStampGTest, TestGetMonth) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestGetMonth(10));
    ASSERT_TRUE(myTimeStampTest.TestGetMonth(13));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestSetYear) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestSetYear(1901));
    ASSERT_TRUE(myTimeStampTest.TestSetYear(1899));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestGetYear) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestGetYear(1901));
    ASSERT_TRUE(myTimeStampTest.TestGetYear(1899));
}

TEST(BareMetal_L1Portability_TimeStampGTest, TestConvertFromEpoch) {
    TimeStampTest myTimeStampTest;
    ASSERT_TRUE(myTimeStampTest.TestConvertFromEpoch());
}

