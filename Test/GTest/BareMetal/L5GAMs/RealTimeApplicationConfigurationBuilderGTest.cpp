/**
 * @file RealTimeApplicationConfigurationBuilderGTest.cpp
 * @brief Source file for class RealTimeApplicationConfigurationBuilderGTest
 * @date 13/07/2016
 * @author Giuseppe Ferro
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

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase1(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase1_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase1(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase2(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase2_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase2(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabase_IgnoreDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabase_IgnoreDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_GAMsNoIO) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_GAMsNoIO(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_GAMsNoIO_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_GAMsNoIO(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_InvalidApplication) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_InvalidApplication());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_NoTimeDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_NoTimeDataSource(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestInitialiseSignalsDatabaseFalse_NoTimeDataSource_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestInitialiseSignalsDatabaseFalse_NoTimeDataSource(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases4) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases4());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases5) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases5());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases6) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases6());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_LockedDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_LockedDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_CopyAllKnownSignalsProperties) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_CopyAllKnownSignalsProperties());
}


TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_MemberAliases) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_MemberAliases());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_Defaults) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_Defaults());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_Defaults2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_Defaults2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_InvalidDimensions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidNDimensionsInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_InvalidNElementsInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidNElementsInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_OnlyFrequencyInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlyFrequencyInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_OnlySyncSignalInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlySyncSignalInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_OnlyTriggerInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlyTriggerInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_OnlyTriggerSignalInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_OnlyTriggerSignalInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_InvalidSyncSignalInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidSyncSignalInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_InvalidTriggerSignalInStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_InvalidTriggerSignalInStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestFlattenSignalsDatabases_UnregisteredType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestFlattenSignalsDatabases_UnregisteredType());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSource_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSource_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_TypeMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_TypeMismatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_TypeMismatch1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_TypeMismatch1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NElementsMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NElementsMismatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NElementsMismatch1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NElementsMismatch1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NDimensionsMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NDimensionsMismatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_NDimensionsMismatch1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_NDimensionsMismatch1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourceFalse_DefaultMismatch3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourceFalse_DefaultMismatch3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_IncompatibleMap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_IncompatibleMap());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_IncompatibleMap1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_IncompatibleMap1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_PartialDataSourceWithType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_PartialDataSourceWithType());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructMap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMap());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructMap2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMap2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructMap3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMap3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructMapNamespaces) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructMapNamespaces());
}


TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructNamespaceMap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructNamespaceMap());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSources_StructNamespaceMapInDs) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSources_StructNamespaceMapInDs());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_LeafMappedOnNode) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_LeafMappedOnNode());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_AddSignalToLockedDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_AddSignalToLockedDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveDataSourcesSignalsFalse_CompleteSignalInLockedDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveDataSourcesSignalsFalse_CompleteSignalInLockedDataSource());
}
TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_PartialDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_PartialDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_DefaultValuesMatch) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_DefaultValuesMatch());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_AssignNumberOfElements) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_AssignNumberOfElements());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignals_AssignNumberOfDimensions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignals_AssignNumberOfDimensions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_NoType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_NoType());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_NoType2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_NoType2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_InvalidNElements) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_InvalidNElements());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_IncompatibleDefaults) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_IncompatibleDefaults());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyDataSourcesSignalsFalse_IncompatibleDefaultsMultiVarAsArray) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyDataSourcesSignalsFalse_IncompatibleDefaultsMultiVarAsArray());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignals3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignals3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignals());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignalsFalse_FalseNoType) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignalsFalse_FalseNoType());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignalsFalse_FalseNoNumberOfElements) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignalsFalse_FalseNoNumberOfElements());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignalsFalse_FalseNoNumberOfDimensions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignalsFalse_FalseNoNumberOfDimensions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyFunctionSignalsFalse_FalseNoQualifiedName) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyFunctionSignalsFalse_FalseNoQualifiedName());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStates) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStates2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates2(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStates2_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates2(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_FunctionGroups) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_FunctionGroups(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_FunctionGroups_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_FunctionGroups(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_MoreThreads) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_MoreThreads(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStates_MoreThreads_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStates_MoreThreads(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_SameGAMInMoreThreads) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_SameGAMInMoreThreads(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_SameGAMInMoreThreads_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_SameGAMInMoreThreads(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs1(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs1_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs1(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs2(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs2_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs2(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs3(false));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_MoreSyncs3_FromConfig) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_MoreSyncs3(true));
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveStatesFalse_InvalidApplication) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveStatesFalse_InvalidApplication());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_ResolveStates_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_ResolveStates_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_VerifyConsumersProducers_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_VerifyConsumersProducers_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_ResolveFunctionSignalsMemorySize_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_ResolveFunctionSignalsMemorySize_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_ResolveFunctionsMemory_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_ResolveFunctionsMemory_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyStates) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyStates());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyStatesFalse_GAMNeverCalled) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyStatesFalse_GAMNeverCalled());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers_MoreSignalsSameGAM) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers_MoreSignalsSameGAM());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers_MoreThreads) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers_MoreThreads());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers_MoreStates) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers_MoreStates());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveConsumersAndProducers_MoreProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveConsumersAndProducers_MoreProducers());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers_Ranges) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_Ranges());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers_Ranges2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_Ranges2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers_Ranges3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_Ranges3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducers_TimingSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducers_TimingSignals());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_MoreProducers) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_MoreProducers());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_MoreProducers2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_MoreProducers2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_RangeOverlap) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_RangeOverlap());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_RangeOverlap2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_RangeOverlap2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestVerifyConsumersAndProducersFalse_ProducerInTimeSignals) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestVerifyConsumersAndProducersFalse_ProducerInTimeSignals());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize_Ranges) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize_Ranges());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize_FalseInvalidRanges) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize_FalseInvalidRanges());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize_FalseInvalidRanges2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize_FalseInvalidRanges2());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionSignalsMemorySize_FalseInvalidRanges3) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionSignalsMemorySize_FalseInvalidRanges3());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory_Samples) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory_Samples());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory_InterleavedDs) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory_InterleavedDs());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemory_SamplesAndBlocks) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemory_SamplesAndBlocks());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestResolveFunctionsMemoryFalse_InvalidSamples) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestResolveFunctionsMemoryFalse_InvalidSamples());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestAssignFunctionsMemoryToDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignFunctionsMemoryToDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestAssignFunctionsMemoryToDataSource_GamIO) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignFunctionsMemoryToDataSource_GamIO());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestAssignFunctionsMemoryToDataSource_AlignedStruct) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignFunctionsMemoryToDataSource_AlignedStruct());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctionsFalse_UnregisteredDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctionsFalse_UnregisteredDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctionsFalse_UnsupportedBroker) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctionsFalse_UnsupportedBroker());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctionsFalse_UnsupportedBroker_Output) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctionsFalse_UnsupportedBroker_Output());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestAssignBrokersToFunctionsFalse_InvalidApplication) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestAssignBrokersToFunctionsFalse_InvalidApplication());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestCopy) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestCopy());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestSet) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestSet());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestPostConfiguredDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestPostConfigureDataSourceFalse_InvalidDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureDataSourceFalse_InvalidDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestPostConfigureFunctions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureFunctions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestPostConfigureFunctionsFalse_InvalidDataSource) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestPostConfigureFunctionsFalse_InvalidDataSource());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureAfterInitialisation) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureAfterInitialisation());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_VerifyDataSourcesSignals_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_VerifyDataSourcesSignals_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_ResolveFunctionSignals_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_ResolveFunctionSignals_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_VerifyFunctionSignals_False) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_VerifyFunctionSignals_False());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureThreads) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureThreads());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_False_NoFunctions) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_False_NoFunctions());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestConfigureBeforeInitialisation_False_NoData) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestConfigureBeforeInitialisation_False_NoData());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestArraysOfStructures1) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestArraysOfStructures1());
}

TEST(BareMetal_L5GAMs_RealTimeApplicationConfigurationBuilderGTest,TestArraysOfStructures2) {
    RealTimeApplicationConfigurationBuilderTest test;
    ASSERT_TRUE(test.TestArraysOfStructures2());
}
