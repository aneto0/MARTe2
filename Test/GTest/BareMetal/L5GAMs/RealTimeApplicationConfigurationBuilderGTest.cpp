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

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases3());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases4) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases4());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases5) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases5());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases6) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases6());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_CopyAllKnownSignalsProperties) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_CopyAllKnownSignalsProperties());
}


TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_MemberAliases) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_MemberAliases());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_Defaults) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_Defaults());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_Defaults2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_Defaults2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_InvalidDimensions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidNDimensionsInStruct());
}


TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_InvalidNElementsInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidNElementsInStruct());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_OnlyFrequencyInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlyFrequencyInStruct());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_OnlySyncSignalInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlySyncSignalInStruct());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_InvalidSyncSignalInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidSyncSignalInStruct());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources3());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_TypeMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_TypeMismatch());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_TypeMismatch1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_TypeMismatch1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NElementsMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NElementsMismatch());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NElementsMismatch1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NElementsMismatch1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NDimensionsMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NDimensionsMismatch());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NDimensionsMismatch1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NDimensionsMismatch1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch3());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_IncompatibleMap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_IncompatibleMap());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_IncompatibleMap1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_IncompatibleMap1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_PartialDataSourceWithType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_PartialDataSourceWithType());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructMap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMap());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructMapNamespaces) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMapNamespaces());
}


TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructNamespaceMap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructNamespaceMap());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_LeafMappedOnNode) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_LeafMappedOnNode());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_PartialDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_PartialDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_DefaultValuesMatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_DefaultValuesMatch());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_AssignNumberOfElements) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_AssignNumberOfElements());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_AssignNumberOfDimensions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_AssignNumberOfDimensions());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_IncompatibleDefaults) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_IncompatibleDefaults());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_IncompatibleDefaultsMultiVarAsArray) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_IncompatibleDefaultsMultiVarAsArray());
}


TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals1());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals3());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals_FalseNoType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals_FalseNoType());
}
/*
TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_NoType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_NoType());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_InvalidNElements) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_InvalidNElements());
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
}*/
