/**
 * @file RealTimeApplicationConfigurationBuilderTest.h
 * @brief Header file for class RealTimeApplicationConfigurationBuilderTest
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

 * @details This header file contains the declaration of the class RealTimeApplicationConfigurationBuilderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEAPPLICATIONCONFIGURATIONBUILDERTEST_H_
#define REALTIMEAPPLICATIONCONFIGURATIONBUILDERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeApplicationConfigurationBuilder.h"
#include "GAMTestHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
/**
 * @brief Tests all the RealTimeApplicationConfigurationBuilder functions.
 */
class RealTimeApplicationConfigurationBuilderTest {

public:
    /**
     * Constructor
     */
    RealTimeApplicationConfigurationBuilderTest();

    /**
     * @brief Tests the InitialiseSignalsDatabase method.
     */
    bool TestInitialiseSignalsDatabase1(bool fromConfig);

    /**
     * @brief Tests the InitialiseSignalsDatabase method with nested data sources.
     */
    bool TestInitialiseSignalsDatabase2(bool fromConfig);

    /**
     * @brief Tests the InitialiseSignalsDatabase method to specify that is not a data source.
     */
    bool TestInitialiseSignalsDatabase_IgnoreDataSource();

    /**
     * @brief Tests the InitialiseSignalsDatabase method with an invalid application.
     */
    bool TestInitialiseSignalsDatabaseFalse_InvalidApplication();

    /**
     * @brief Tests the InitialiseSignalsDatabase method with invalid nested gams.
     */
    bool TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs(bool fromConfig);

    /**
     * @brief Tests the InitialiseSignalsDatabase method with a GAM with no IO signals.
     */
    bool TestInitialiseSignalsDatabaseFalse_GAMsNoIO(bool fromConfig);

    /**
     * @brief Tests the InitialiseSignalsDatabase method with more than one TimingDataSource defined.
     */
    bool TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources(bool fromConfig);

    /**
     * @brief Tests the InitialiseSignalsDatabaseFalse method with no TimingDataSource defined.
     */
    bool TestInitialiseSignalsDatabaseFalse_NoTimeDataSource(bool fromConfig);

    /**
     * @brief Tests the FlattenSignalsDatabases.
     */
    bool TestFlattenSignalsDatabases1();

    /**
     * @brief Tests the FlattenSignalsDatabases method with nested names.
     */
    bool TestFlattenSignalsDatabases2();

    /**
     * @brief Tests the FlattenSignalsDatabases method with unspecified DataSource.
     */
    bool TestFlattenSignalsDatabases3();

    /**
     * @brief Tests the FlattenSignalsDatabases method with Alias.
     */
    bool TestFlattenSignalsDatabases4();

    /**
     * @brief Tests the FlattenSignalsDatabases method With deep nested names.
     */
    bool TestFlattenSignalsDatabases5();

    /**
     * @brief Tests the FlattenSignalsDatabases method with deep nested names in data source.
     */
    bool TestFlattenSignalsDatabases6();

    /**
     * @brief Tests the FlattenSignalsDatabases method with the Locked flag in data source.
     */
    bool TestFlattenSignalsDatabases_LockedDataSource();

    /**
     * @brief Tests the FlattenSignalsDatabases and check all known properties.
     */
    bool TestFlattenSignalsDatabases_CopyAllKnownSignalsProperties();

    /**
     * @brief Tests the FlattenSignalsDatabases method and check Member aliases in struct.
     */
    bool TestFlattenSignalsDatabases_MemberAliases();

    /**
     * @brief Tests the FlattenSignalsDatabases method and check Defaults in struct in GAM.
     */
    bool TestFlattenSignalsDatabases_Defaults();

    /**
     * @brief Tests the FlattenSignalsDatabases method and check Defaults in struct in DataSourceI.
     */
    bool TestFlattenSignalsDatabases_Defaults2();

    /**
     * @brief Tests the FlattenSignalsDatabases method with dimensions>0 in struct.
     */
    bool TestFlattenSignalsDatabases_InvalidNDimensionsInStruct();

    /**
     * @brief Tests the FlattenSignalsDatabases method with elements>1 in struct.
     */
    bool TestFlattenSignalsDatabases_InvalidNElementsInStruct();

    /**
     * @brief Tests the FlattenSignalsDatabases method with only frequency specified in struct.
     */
    bool TestFlattenSignalsDatabases_OnlyFrequencyInStruct();

    /**
     * @brief Tests the FlattenSignalsDatabases method with only trigger specified in struct.
     */
    bool TestFlattenSignalsDatabases_OnlyTriggerInStruct();

    /**
     * @brief Tests the FlattenSignalsDatabases method with only sync signal specified in struct.
     */
    bool TestFlattenSignalsDatabases_OnlySyncSignalInStruct();

    /**
     * @brief Tests the FlattenSignalsDatabases method with only trigger signal specified in struct.
     */
    bool TestFlattenSignalsDatabases_OnlyTriggerSignalInStruct();

    /**
     * @brief Tests the FlattenSignalsDatabases method with an invalid sync signal specified in struct.
     */
    bool TestFlattenSignalsDatabases_InvalidSyncSignalInStruct();

    /**
     * @brief Tests the FlattenSignalsDatabases method with an invalid trigger signal specified in struct.
     */
    bool TestFlattenSignalsDatabases_InvalidTriggerSignalInStruct();

    /**
     * @brief Tests the FlattenSignalsDatabases method with an unregistered type.
     */
    bool TestFlattenSignalsDatabases_UnregisteredType();

    /**
     * @brief Tests the ResolveDataSources method.
     */
    bool TestResolveDataSources1();

    /**
     * @brief Tests the ResolveDataSources method with different data sources.
     */
    bool TestResolveDataSources2();

    /**
     * @brief Tests the ResolveDataSources method with Merge properties.
     */
    bool TestResolveDataSources3();

    /**
     * @brief Tests the ResolveDataSources method with structures remapping.
     */
    bool TestResolveDataSources_StructMap();

    /**
     * @brief Tests the ResolveDataSources method with structures remapping with DataSourceI namespaces.
     */
    bool TestResolveDataSources_StructMapNamespaces();

    /**
     * @brief Tests the ResolveDataSources method with structures remapping using namespaces.
     */
    bool TestResolveDataSources_StructNamespaceMap();

    /**
     * @brief Tests the ResolveDataSources method with structures remapping using namespaces.
     */
    bool TestResolveDataSources_StructNamespaceMapInDs();

    /**
     * @brief Tests the ResolveDataSource method with Type mismatch.
     */
    bool TestResolveDataSourceFalse_TypeMismatch();

    /**
     * @brief Tests the ResolveDataSource method Type with mismatch directly from DataSource.
     */
    bool TestResolveDataSourceFalse_TypeMismatch1();

    /**
     * @brief Tests the ResolveDataSource method with NElements mismatch.
     */
    bool TestResolveDataSourceFalse_NElementsMismatch();

    /**
     * @brief Tests the ResolveDataSource method with NElements mismatch directly from DataSource.
     */
    bool TestResolveDataSourceFalse_NElementsMismatch1();

    /**
     * @brief Tests the ResolveDataSource method with NDimensions mismatch.
     */
    bool TestResolveDataSourceFalse_NDimensionsMismatch();

    /**
     * @brief Tests the ResolveDataSource method with NDimensions mismatch directly from DataSource.
     */
    bool TestResolveDataSourceFalse_NDimensionsMismatch1();

    /**
     * @brief Tests the ResolveDataSource method with Default mismatch.
     */
    bool TestResolveDataSourceFalse_DefaultMismatch();

    /**
     * @brief Tests the ResolveDataSource method with Default mismatch directly from DataSource.
     */
    bool TestResolveDataSourceFalse_DefaultMismatch1();

    /**
     * @brief Tests the ResolveDataSource method with Default mismatch with structures.
     */
    bool TestResolveDataSourceFalse_DefaultMismatch3();

    /**
     * @brief Tests the ResolveDataSources method assignment of a leaf on a node.
     */
    bool TestResolveDataSourcesSignalsFalse_LeafMappedOnNode();

    /**
     * @brief Tests the ResolveDataSourcesSignals method with an incompatible struct map.
     */
    bool TestResolveDataSourcesSignalsFalse_IncompatibleMap();

    /**
     * @brief Tests the ResolveDataSourcesSignals method with an incompatible struct map.
     */
    bool TestResolveDataSourcesSignalsFalse_IncompatibleMap1();

    /**
     * @brief Tests the ResolveDataSourcesSignals method with a pre-existent DataSource with type not allowed (map a structure on a leaf basically).
     */
    bool TestResolveDataSourcesSignalsFalse_PartialDataSourceWithType();

    /**
     * @brief Tests the ResolveDataSourcesSignals method with a Locked DataSource.
     */
    bool TestResolveDataSourcesSignalsFalse_AddSignalToLockedDataSource();

    /**
     * @brief Tests the ResolveFunctionSignals method with a locked DataSource.
     */
    bool TestResolveDataSourcesSignalsFalse_CompleteSignalInLockedDataSource();

    /**
     * @brief Tests the VerifyDataSourcesSignals.
     */
    bool TestVerifyDataSourcesSignals1();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with a partial struct node defined.
     */
    bool TestVerifyDataSourcesSignals_PartialDataSource();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with matching of various defaults.
     */
    bool TestVerifyDataSourcesSignals_DefaultValuesMatch();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with automatic assignment of NumberOfElements.
     */
    bool TestVerifyDataSourcesSignals_AssignNumberOfElements();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with automatic assignment of NumberOfDimensions.
     */
    bool TestVerifyDataSourcesSignals_AssignNumberOfDimensions();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with a DataSource with type.
     */
    bool TestVerifyDataSourcesSignalsFalse_NoType();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with no type.
     */
    bool TestVerifyDataSourcesSignalsFalse_NoType2();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with NumberOfElements = 0.
     */
    bool TestVerifyDataSourcesSignalsFalse_InvalidNElements();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with incompatible defaults/
     */
    bool TestVerifyDataSourcesSignalsFalse_IncompatibleDefaults();

    /**
     * @brief Tests the VerifyDataSourcesSignals method with incompatible defaults.
     */
    bool TestVerifyDataSourcesSignalsFalse_IncompatibleDefaultsMultiVarAsArray();

    /**
     * @brief Tests the ResolveFunctionSignals.
     */
    bool TestResolveFunctionSignals1();

    /**
     * @brief Tests the ResolveFunctionSignals.
     */
    bool TestResolveFunctionSignals2();

    /**
     * @brief Tests the ResolveFunctionSignals.
     */
    bool TestResolveFunctionSignals3();

    /**
     * @brief Tests VerifyFunctionSignals method.
     */
    bool TestVerifyFunctionSignals();

    /**
     * @brief Tests the VerifyFunctionSignals method with with no type.
     */
    bool TestVerifyFunctionSignalsFalse_FalseNoType();

    /**
     * @brief Tests the VerifyFunctionSignals method with no #elements.
     */
    bool TestVerifyFunctionSignalsFalse_FalseNoNumberOfElements();

    /**
     * @brief Tests the VerifyFunctionSignals method with no #dimensions.
     */
    bool TestVerifyFunctionSignalsFalse_FalseNoNumberOfDimensions();

    /**
     * @brief Tests the VerifyFunctionSignals method with no qualified name.
     */
    bool TestVerifyFunctionSignalsFalse_FalseNoQualifiedName();

    /**
     * @brief Tests the ResolveStates method.
     */
    bool TestResolveStates(bool fromConfig);

    /**
     * @brief Tests the ResolveStates method with same gam in more states.
     */
    bool TestResolveStates2(bool fromConfig);

    /**
     * @brief Tests the ResolveStates method with gam groups.
     */
    bool TestResolveStates_FunctionGroups(bool fromConfig);

    /**
     * @brief Tests the ResolveStates method with more threads per state.
     */
    bool TestResolveStates_MoreThreads(bool fromConfig);

    /**
     * @brief Tests the ResolveStates method with the same GAM in more than one thread in the same state.
     */
    bool TestResolveStatesFalse_SameGAMInMoreThreads(bool fromConfig);

    /**
     * @brief Tests the ResolveStates method with more than one sync point in a thread.
     */
    bool TestResolveStatesFalse_MoreSyncs1(bool fromConfig);

    /**
     * @brief Tests the ResolveStates method with more than one sync point in a thread.
     */
    bool TestResolveStatesFalse_MoreSyncs2(bool fromConfig);

    /**
     * @brief Tests the ResolveStates method with more than one sync point in a thread.
     */
    bool TestResolveStatesFalse_MoreSyncs3(bool fromConfig);

    /**
     * @brief Tests the ResolveStates method with an invalid application.
     */
    bool TestResolveStatesFalse_InvalidApplication();

    /**
     * @brief Tests the VerifyStates method.
     */
    bool TestVerifyStates();

    /**
     * @brief Tests the VerifyStates method with a GAM that is never called.
     */
    bool TestVerifyStatesFalse_GAMNeverCalled();

    /**
     * @brief Tests the ResolveConsumersAndProducers method.
     */
    bool TestResolveConsumersAndProducers();

    /**
     * @brief Tests the ResolveConsumersAndProducers method with more gam signals.
     */
    bool TestResolveConsumersAndProducers_MoreSignalsSameGAM();

    /**
     * @brief Tests the ResolveConsumersAndProducers method with more threads.
     */
    bool TestResolveConsumersAndProducers_MoreThreads();

    /**
     * @brief Tests the ResolveConsumersAndProducers method with more states.
     */
    bool TestResolveConsumersAndProducers_MoreStates();

    /**
     * @brief Tests the ResolveConsumersAndProducers method with more producers.
     */
    bool TestResolveConsumersAndProducers_MoreProducers();

    /**
     * @brief Tests the VerifyConsumersAndProducers method.
     */
    bool TestVerifyConsumersAndProducers();

    /**
     * @brief Tests the VerifyConsumersAndProducers method with ranges.
     */
    bool TestVerifyConsumersAndProducers_Ranges();

    /**
     * @brief Tests the VerifyConsumersAndProducers method with ranges in structure.
     */
    bool TestVerifyConsumersAndProducers_Ranges2();

    /**
     * @brief Tests the VerifyConsumersAndProducers method with ranges overlap (but in input).
     */
    bool TestVerifyConsumersAndProducers_Ranges3();

    /**
     * @brief Tests the VerifyConsumersAndProducers method with merging of timing signals.
     */
    bool TestVerifyConsumersAndProducers_TimingSignals();

    /**
     * @brief Tests the VerifyConsumersAndProducers method with more than one producer.
     */
    bool TestVerifyConsumersAndProducersFalse_MoreProducers();

    /**
     * @brief Tests the VerifyConsumersAndProducers method more than one producer.
     */
    bool TestVerifyConsumersAndProducersFalse_MoreProducers2();

    /**
     * @brief Tests the VerifyConsumersAndProducers method with range overlap fail.
     */
    bool TestVerifyConsumersAndProducersFalse_RangeOverlap();

    /**
     * @brief Tests the VerifyConsumersAndProducers method with range overlap fail same signal.
     */
    bool TestVerifyConsumersAndProducersFalse_RangeOverlap2();

    /**
     * @brief Tests the VerifyConsumersAndProducers method with producer in timing signals.
     */
    bool TestVerifyConsumersAndProducersFalse_ProducerInTimeSignals();

    /**
     * @brief Tests the ResolveFunctionSignalsMemorySize.
     */
    bool TestResolveFunctionSignalsMemorySize();

    /**
     * @brief Tests the ResolveFunctionSignalsMemorySize method with ranges.
     */
    bool TestResolveFunctionSignalsMemorySize_Ranges();

    /**
     * @brief Tests the ResolveFunctionSignalsMemorySize method with invalid ranges min>max.
     */
    bool TestResolveFunctionSignalsMemorySize_FalseInvalidRanges();

    /**
     * @brief Tests the ResolveFunctionSignalsMemorySize method with invalid ranges max>#elements.
     */
    bool TestResolveFunctionSignalsMemorySize_FalseInvalidRanges2();

    /**
     * @brief Tests the ResolveFunctionSignalsMemorySize method with invalid ranges field 3 columns.
     */
    bool TestResolveFunctionSignalsMemorySize_FalseInvalidRanges3();

    /**
     * @brief Tests the ResolveFunctionsMemory method.
     */
    bool TestResolveFunctionsMemory();

    /**
     * @brief Tests the ResolveFunctionsMemory method with samples.
     */
    bool TestResolveFunctionsMemory_Samples();

    /**
     * @brief Tests the ResolveFunctionsMemory method with interleaved DataSource.
     */
    bool TestResolveFunctionsMemory_InterleavedDs();

    /**
     * @brief Tests the ResolveFunctionsMemory method with samples and blocks.
     */
    bool TestResolveFunctionsMemory_SamplesAndBlocks();

    /**
     * @brief Tests the ResolveFunctionsMemory method with Samples =0.
     */
    bool TestResolveFunctionsMemoryFalse_InvalidSamples();

    /**
     * @brief Tests the AssignFunctionsMemoryToDataSource.
     */
    bool TestAssignFunctionsMemoryToDataSource();

    /**
     * @brief Tests the AssignFunctionsMemoryToDataSource method with the same gam as producer and consumer
     */
    bool TestAssignFunctionsMemoryToDataSource_GamIO();

    /**
     * @brief Tests the ResolveFunctionSignalsMemorySize with a structure aligned with unused memory.
     */
    bool TestAssignFunctionsMemoryToDataSource_AlignedStruct();

    /**
     * @brief Tests the AssignBrokersToFunctions.
     */
    bool TestAssignBrokersToFunctions();

    /**
     * @brief Tests the AssignBrokersToFunctions method with an unregistered DataSource and initialised from configuration.
     */
    bool TestAssignBrokersToFunctionsFalse_UnregisteredDataSource();

    /**
     * @brief Tests the AssignBrokersToFunctions method with an  unsupported broker.
     */
    bool TestAssignBrokersToFunctionsFalse_UnsupportedBroker();

    /**
     * @brief Tests the AssignBrokersToFunctions method with an invalid application.
     */
    bool TestAssignBrokersToFunctionsFalse_InvalidApplication();

    /**
     * @brief Tests the Set method.
     */
    bool TestSet();

    /**
     * @brief Tests the Copy method.
     */
    bool TestCopy();

    /**
     * @brief Tests the PostConfigureDataSource.
     */
    bool TestPostConfigureDataSource();

    /**
     * @brief Tests the PostConfigureDataSource method with an invalid data source
     */
    bool TestPostConfigureDataSourceFalse_InvalidDataSource();

    /**
     * @brief Tests the PostConfigureFunctions.
     */
    bool TestPostConfigureFunctions();

    /**
     * @brief Tests the PostConfigureFunctions method invalid GAM.
     */
    bool TestPostConfigureFunctionsFalse_InvalidDataSource();

    /**
     * @brief Tests the ConfigureAfterInitialisation method.
     */
    bool TestConfigureAfterInitialisation();

    /**
     * @brief Tests the ConfigureBeforeInitialisation method.
     */
    bool TestConfigureBeforeInitialisation();

    /**
     * @brief Tests the ConfigureThreads method (tested implicitly).
     */
    bool TestConfigureThreads();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONCONFIGURATIONBUILDERTEST_H_ */

