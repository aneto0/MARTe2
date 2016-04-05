/**
 * @file RealTimeDataDefContainerGTest.cpp
 * @brief Source file for class RealTimeDataDefContainerGTest
 * @date 04/03/2016
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
 * the class RealTimeDataDefContainerGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeDataDefContainerTest.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeDataDefContainerGTest,TestConstructor) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestConstructor());
}

TEST(RealTimeDataDefContainerGTest,TestInitialise) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestConstructor());
}

TEST(RealTimeDataDefContainerGTest,TestMergeWithLocal) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestMergeWithLocal());
}

TEST(RealTimeDataDefContainerGTest,TestMergeWithLocal_AddDefinition) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestMergeWithLocal_AddDefinition());
}

TEST(RealTimeDataDefContainerGTest,TestMergeWithLocalFalse_GlobalFinal) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestMergeWithLocalFalse_GlobalFinal());
}

TEST(RealTimeDataDefContainerGTest,TestMergeWithLocal_AddInput) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestMergeWithLocal_AddInput());
}

TEST(RealTimeDataDefContainerGTest,TestMergeWithLocal_AddOutput) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestMergeWithLocal_AddOutput());
}

TEST(RealTimeDataDefContainerGTest,TestVerify) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestVerify());
}

TEST(RealTimeDataDefContainerGTest,TestVerify_DefInvalid) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestVerify_DefInvalid());
}

TEST(RealTimeDataDefContainerGTest,TestIsInput) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestIsInput());
}

TEST(RealTimeDataDefContainerGTest,TestIsOutput) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestIsOutput());
}

TEST(RealTimeDataDefContainerGTest,TestToStructuredData) {
    RealTimeDataDefContainerTest rtdcTest;
    ASSERT_TRUE(rtdcTest.TestToStructuredData());
}
