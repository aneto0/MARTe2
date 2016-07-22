/**
 * @file RealTimeApplicationConfigurationBuilderGTest.cpp
 * @brief Source file for class RealTimeApplicationConfigurationBuilderGTest
 * @date 13/lug/2016
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
 * the class RealTimeApplicationConfigurationBuilderGTest (public, protected, and private). Be aware that some 
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
#include "RealTimeApplicationConfigurationBuilderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_TypeMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_TypeMismatch());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NElementsMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NElementsMismatch());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NDimensionsMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NDimensionsMismatch());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_PartialInPredefinedDsNoType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_PartialInPredefinedDsNoType());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_NoTypeInPredefinedDs) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_NoTypeInPredefinedDs());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_PartialInPredefinedDsWithType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_PartialInPredefinedDsWithType());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsFalse_NoTypeInGAMSignal) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsFalse_NoTypeInGAMSignal());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignals());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_SameGAMIn2Threads) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_SameGAMIn2Threads());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreThanOneSyncInAThread_SameGAM) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreThanOneSyncInAThread_SameGAM());
}


TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_TwoProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_TwoProducers());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_MemoryOverlap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_MemoryOverlap());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySizeFalse_WrongRangeMaxMin) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySizeFalse_WrongRangeMaxMin());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySizeFalse_WrongRangeMaxNElements) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySizeFalse_WrongRangeMaxNElements());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestCalculateFunctionsMemory) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestCalculateFunctionsMemory());
}


TEST(RealTimeApplicationConfigurationBuilderGTest,TestAssignFunctionsMemoryToDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignFunctionsMemoryToDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctions());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestAllocateGAMMemory) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAllocateGAMMemory());
}
