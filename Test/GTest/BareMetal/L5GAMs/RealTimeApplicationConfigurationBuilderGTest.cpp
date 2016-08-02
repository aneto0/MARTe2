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

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase1(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase1_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase1(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase2(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase2_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase2(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase_IgnoreDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase_IgnoreDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_GAMsNoIO) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_GAMsNoIO(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_GAMsNoIO_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_GAMsNoIO(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidApplication) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidApplication());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_NoTimeDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_NoTimeDataSource(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_NoTimeDataSource_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_NoTimeDataSource(true));
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

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_LockedDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_LockedDataSource());
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

TEST(RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_UnregisteredType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_UnregisteredType());
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

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_AddSignalToLockedDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_AddSignalToLockedDataSource());
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

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_NoType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_NoType());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_NoType2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_NoType2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_InvalidNElements) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_InvalidNElements());
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

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsFalse_CompleteSignalInLockedDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsFalse_CompleteSignalInLockedDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignals());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignalsFalse_FalseNoType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignalsFalse_FalseNoType());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignalsFalse_FalseNoNumberOfElements) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignalsFalse_FalseNoNumberOfElements());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignalsFalse_FalseNoNumberOfDimensions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignalsFalse_FalseNoNumberOfDimensions());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignalsFalse_FalseNoQualifiedName) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignalsFalse_FalseNoQualifiedName());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates2(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates2_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates2(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_FunctionGroups) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_FunctionGroups(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_FunctionGroups_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_FunctionGroups(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_MoreThreads) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_MoreThreads(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_MoreThreads_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_MoreThreads(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_SameGAMInMoreThreads) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_SameGAMInMoreThreads(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_SameGAMInMoreThreads_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_SameGAMInMoreThreads(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs1(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs1_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs1(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs2(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs2_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs2(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs3(false));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs3_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs3(true));
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_InvalidApplication) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_InvalidApplication());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyStates) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyStates());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyStatesFalse_GAMNeverCalled) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyStatesFalse_GAMNeverCalled());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers_MoreSignalsSameGAM) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers_MoreSignalsSameGAM());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers_MoreThreads) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers_MoreThreads());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers_MoreStates) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers_MoreStates());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers_MoreProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers_MoreProducers());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers_Ranges) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_Ranges());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers_Ranges2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_Ranges2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers_Ranges3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_Ranges3());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers_TimesSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_TimesSignals());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_MoreProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_MoreProducers());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_MoreProducers2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_MoreProducers2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_RangeOverlap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_RangeOverlap());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_RangeOverlap2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_RangeOverlap2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_ProducerInTimeSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_ProducerInTimeSignals());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_InvalidTimeSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_InvalidTimeSignals());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize_Ranges) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize_Ranges());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize_FalseInvalidRanges) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize_FalseInvalidRanges());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize_FalseInvalidRanges2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize_FalseInvalidRanges2());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize_FalseInvalidRanges3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize_FalseInvalidRanges3());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory_Samples) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory_Samples());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory_InterleavedDs) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory_InterleavedDs());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory_SamplesAndBlocks) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory_SamplesAndBlocks());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemoryFalse_InvalidSamples) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemoryFalse_InvalidSamples());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestAssignFunctionsMemoryToDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignFunctionsMemoryToDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestAssignFunctionsMemoryToDataSource_GamIO) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignFunctionsMemoryToDataSource_GamIO());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctions());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctionsFalse_UnregisteredDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctionsFalse_UnregisteredDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctionsFalse_UnsupportedBroker) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctionsFalse_UnsupportedBroker());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctionsFalse_InvalidApplication) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctionsFalse_InvalidApplication());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestCopy) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestCopy());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestSet) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestSet());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestPostConfiguredDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestPostConfigureDataSourceFalse_InvalidDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureDataSourceFalse_InvalidDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestPostConfigureFunctions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureFunctions());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestPostConfigureFunctionsFalse_InvalidDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureFunctionsFalse_InvalidDataSource());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestConfigureAfterInitialisation) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureAfterInitialisation());
}

TEST(RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation());
}

