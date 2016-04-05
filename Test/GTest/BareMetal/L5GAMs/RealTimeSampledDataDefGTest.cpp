/**
 * @file RealTimeSampledDataDefGTest.cpp
 * @brief Source file for class RealTimeSampledDataDefGTest
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
 * the class RealTimeSampledDataDefGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeSampledDataDefTest.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeSampledDataDefGTest,TestConstructor) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestConstructor());
}


TEST(RealTimeSampledDataDefGTest,TestInitialiseTrue) {
    RealTimeSampledDataDefTest rtsdTest;
    ConfigurationDatabase data;
    data.Write("Samples", 1);
    data.Write("SamplesPerCycle", 1);
    data.Write("IsFinal", "true");
    ASSERT_TRUE(rtsdTest.TestInitialise(data));
}

TEST(RealTimeSampledDataDefGTest,TestInitialise_NoSamples) {
    RealTimeSampledDataDefTest rtsdTest;
    ConfigurationDatabase data;
    data.Write("SamplesPerCycle", 1);
    data.Write("IsFinal", "true");
    ASSERT_TRUE(rtsdTest.TestInitialise(data));
}


TEST(RealTimeSampledDataDefGTest,TestInitialise_NoSamplesPerCycle) {
    RealTimeSampledDataDefTest rtsdTest;
    ConfigurationDatabase data;
    data.Write("Samples", 1);
    data.Write("IsFinal", "true");
    ASSERT_TRUE(rtsdTest.TestInitialise(data));
}


TEST(RealTimeSampledDataDefGTest,TestMergeWithLocal_AddSamples) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocal_AddSamples());
}

TEST(RealTimeSampledDataDefGTest,TestMergeWithLocal_AddSamplesPerCycle) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocal_AddSamplesPerCycle());
}

TEST(RealTimeSampledDataDefGTest,TestMergeWithLocal_AddPath) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocal_AddPath());
}

TEST(RealTimeSampledDataDefGTest,TestMergeWithLocal_AddType) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocal_AddType());
}

TEST(RealTimeSampledDataDefGTest,TestMergeWithLocalFalse_FinalGlobal) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocalFalse_FinalGlobal());
}

TEST(RealTimeSampledDataDefGTest,TestMergeWithLocal_DifferentSamples) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocal_DifferentSamples());
}

TEST(RealTimeSampledDataDefGTest,TestMergeWithLocal_DifferentSamplesPerCycle) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocal_DifferentSamplesPerCycle());
}

TEST(RealTimeSampledDataDefGTest,TestMergeWithLocal_DifferentType) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocal_DifferentType());
}

TEST(RealTimeSampledDataDefGTest,TestMergeWithLocal_DifferentPath) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestMergeWithLocal_DifferentPath());
}

TEST(RealTimeSampledDataDefGTest,TestVerify) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestVerify());
}

TEST(RealTimeSampledDataDefGTest,TestVerifyFalse_UnregisteredType) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestVerifyFalse_UnregisteredType());
}

TEST(RealTimeSampledDataDefGTest,TestVerifyFalse_UnintrospectableType) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestVerifyFalse_UnintrospectableType());
}

TEST(RealTimeSampledDataDefGTest,TestGetSamples) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestGetSamples());
}

TEST(RealTimeSampledDataDefGTest,TestGetSamplesPerCycle) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestGetSamplesPerCycle());
}

TEST(RealTimeSampledDataDefGTest,TestToStructuredData) {
    RealTimeSampledDataDefTest rtsdTest;
    ASSERT_TRUE(rtsdTest.TestToStructuredData());
}


