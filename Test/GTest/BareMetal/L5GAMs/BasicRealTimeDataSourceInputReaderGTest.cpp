/**
 * @file BasicRealTimeDataSourceInputReaderGTest.cpp
 * @brief Source file for class BasicRealTimeDataSourceInputReaderGTest
 * @date 22/mar/2016
 * @author pc
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
 * the class BasicRealTimeDataSourceInputReaderGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicRealTimeDataSourceInputReaderTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicRealTimeDataSourceInputReaderGTest,TestConstructor) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestConstructor());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_DefaultValues_Static) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_DefaultValues_Static());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_DefaultValues_Allocation) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_DefaultValues_Allocation());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_Static) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_Static());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_Allocation) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_Allocation());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_MoreThanOneVariable) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_MoreThanOneVariable());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_MultiDim_Vector) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_MultiDim_Vector());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestRead_MultiDim_Matrix) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestRead_MultiDim_Matrix());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestSynchroniseOnSpinLockSemNotSync) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestSynchroniseOnSpinLockSemNotSync());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestSynchroniseOnSpinLockSem) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestSynchroniseOnSpinLockSem());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestSynchroniseOnSpinLockSemWaitTime) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestSynchroniseOnSpinLockSemWaitTime());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestSynchroniseOnSpinLockSemMoreReads) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestSynchroniseOnSpinLockSemMoreReads());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestSynchroniseOnSpinLockSemTimeout) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestSynchroniseOnSpinLockSemTimeout());
}

TEST(BasicRealTimeDataSourceInputReaderGTest,TestSynchroniseOnSpinLockNoSleep) {
    BasicRealTimeDataSourceInputReaderTest rtdsirTest;
    ASSERT_TRUE(rtdsirTest.TestSynchroniseOnSpinLockNoSleep());
}
