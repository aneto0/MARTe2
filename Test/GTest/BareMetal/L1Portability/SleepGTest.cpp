/**
 * @file SleepGTest.cpp
 * @brief Source file for class SleepGTest
 * @date 25/06/2015
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
 * the class SleepGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "SleepTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*
TEST(BareMetal_L1Portability_SleepGTest,TestSleepAtLeast) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestAtLeast(1.2));
}
*/
TEST(BareMetal_L1Portability_SleepGTest,TestSleepNoMore) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestNoMore(0.8));
}

TEST(BareMetal_L1Portability_SleepGTest,TestSleepSecD) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestSec(0.2));
}

TEST(BareMetal_L1Portability_SleepGTest,TestSleepMSec) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestMSec(120));
}

TEST(BareMetal_L1Portability_SleepGTest,TestSleepBusy) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestBusy(0.4));
}

TEST(BareMetal_L1Portability_SleepGTest,TestSleepSemiBusy) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestSemiBusy(0.5, 0.2));
}

TEST(BareMetal_L1Portability_SleepGTest,TestGetDateSeconds) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestGetDateSeconds());
}

TEST(BareMetal_L1Portability_SleepGTest,TestGetSchedulerGranularity) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestGetSchedulerGranularity());
}

TEST(BareMetal_L1Portability_SleepGTest,TestSetSchedulerGranularity) {
    SleepTest sleepTest;
    ASSERT_TRUE(sleepTest.TestSetSchedulerGranularity());
}

