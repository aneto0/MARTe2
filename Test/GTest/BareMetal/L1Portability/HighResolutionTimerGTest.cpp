/**
 * @file HighResolutionTimerGTest.cpp
 * @brief Source file for class HighResolutionTimerGTest
 * @date 26/06/2015
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
 * the class HighResolutionTimerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "HighResolutionTimerTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(BareMetal_L1Portability_HighResolutionTimerGTest,TestFrequency) {
    HighResolutionTimerTest hrtTest;
    ASSERT_TRUE(hrtTest.TestFrequency());
}

TEST(BareMetal_L1Portability_HighResolutionTimerGTest,TestPeriod) {
    HighResolutionTimerTest hrtTest;
    ASSERT_TRUE(hrtTest.TestPeriod());
}

TEST(BareMetal_L1Portability_HighResolutionTimerGTest,TestPeriodFrequency) {
    HighResolutionTimerTest hrtTest;
    ASSERT_TRUE(hrtTest.TestPeriodFrequency());
}

TEST(BareMetal_L1Portability_HighResolutionTimerGTest,TestCounter) {
    HighResolutionTimerTest hrtTest;
    ASSERT_TRUE(hrtTest.TestCounter(0.5));
}

TEST(BareMetal_L1Portability_HighResolutionTimerGTest,TestCounter32) {
    HighResolutionTimerTest hrtTest;
    //This has to be a short time otherwise it will overflow in 32 bits...
    ASSERT_TRUE(hrtTest.TestCounter32(0.05));
}

TEST(BareMetal_L1Portability_HighResolutionTimerGTest,TestTicksToTime) {
    HighResolutionTimerTest hrtTest;
    ASSERT_TRUE(hrtTest.TestTicksToTime());
}

TEST(BareMetal_L1Portability_HighResolutionTimerGTest,TestGetTimeStamp) {
    HighResolutionTimerTest hrtTest;
    ASSERT_TRUE(hrtTest.TestGetTimeStamp(100));
}


