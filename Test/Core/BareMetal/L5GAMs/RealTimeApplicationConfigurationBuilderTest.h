/**
 * @file RealTimeApplicationConfigurationBuilderTest.h
 * @brief Header file for class RealTimeApplicationConfigurationBuilderTest
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

class RealTimeApplicationConfigurationBuilderTest {

public:
    RealTimeApplicationConfigurationBuilderTest();

    //basic
    bool TestInitialiseSignalsDatabase1(bool fromConfig);

    //nested datasources
    bool TestInitialiseSignalsDatabase2(bool fromConfig);

    //flag to specify that is not a datasource
    bool TestInitialiseSignalsDatabase_IgnoreDataSource();

    //invalid application
    bool TestInitialiseSignalsDatabaseFalse_InvalidApplication();

    //invalid nested gams
    bool TestInitialiseSignalsDatabaseFalse_InvalidNestedGAMs(bool fromConfig);

    //gam with no IO signals
    bool TestInitialiseSignalsDatabaseFalse_GAMsNoIO(bool fromConfig);

    //more than one time ds defined
    bool TestInitialiseSignalsDatabaseFalse_TooManyTimeDataSources(bool fromConfig);

    //no time ds defined
    bool TestInitialiseSignalsDatabaseFalse_NoTimeDataSource(bool fromConfig);

    //Basic
    bool TestFlattenSignalsDatabases1();

    //With nested names
    bool TestFlattenSignalsDatabases2();

    //Unspecified DataSource
    bool TestFlattenSignalsDatabases3();

    //Alias
    bool TestFlattenSignalsDatabases4();

    //With deep nested names
    bool TestFlattenSignalsDatabases5();

    //With deep nested names in data source
    bool TestFlattenSignalsDatabases6();

    //Put Locked flag in data source
    bool TestFlattenSignalsDatabases_LockedDataSource();

    //Known signal properties
    bool TestFlattenSignalsDatabases_CopyAllKnownSignalsProperties();

    //Member aliases in struct
    bool TestFlattenSignalsDatabases_MemberAliases();

    //Defaults in struct in GAM
    bool TestFlattenSignalsDatabases_Defaults();

    //Defaults in struct in ds
    bool TestFlattenSignalsDatabases_Defaults2();

    //dimensions>0 in struct
    bool TestFlattenSignalsDatabases_InvalidNDimensionsInStruct();

    //elements>1 in struct
    bool TestFlattenSignalsDatabases_InvalidNElementsInStruct();

    //Only frequency specified in struct
    bool TestFlattenSignalsDatabases_OnlyFrequencyInStruct();

    //Only sync signal specified in struct
    bool TestFlattenSignalsDatabases_OnlySyncSignalInStruct();

    //Invalid sync signal specified in struct
    bool TestFlattenSignalsDatabases_InvalidSyncSignalInStruct();

    //unregistered type
    bool TestFlattenSignalsDatabases_UnregisteredType();

    //Basic
    bool TestResolveDataSources1();

    //Different data sources
    bool TestResolveDataSources2();

    //Merge properties
    bool TestResolveDataSources3();

    //Structures Remapping
    bool TestResolveDataSources_StructMap();

    //Structures Remapping with ds namespaces
    bool TestResolveDataSources_StructMapNamespaces();

    //Structures Remapping using namespace
    bool TestResolveDataSources_StructNamespaceMap();

    //Type mismatch
    bool TestResolveDataSourceFalse_TypeMismatch();

    //Type mismatch directly from ds
    bool TestResolveDataSourceFalse_TypeMismatch1();

    //NElements mismatch
    bool TestResolveDataSourceFalse_NElementsMismatch();

    //NElements mismatch directly from ds
    bool TestResolveDataSourceFalse_NElementsMismatch1();

    //NDimensions mismatch
    bool TestResolveDataSourceFalse_NDimensionsMismatch();

    //NDimensions mismatch directly from ds
    bool TestResolveDataSourceFalse_NDimensionsMismatch1();

    //Default mismatch
    bool TestResolveDataSourceFalse_DefaultMismatch();

    //Default mismatch directly from ds
    bool TestResolveDataSourceFalse_DefaultMismatch1();

    //Default mismatch with structures
    bool TestResolveDataSourceFalse_DefaultMismatch3();

    //assignment of a leaf on a node
    bool TestResolveDataSourcesSignalsFalse_LeafMappedOnNode();

    //incompatible struct map
    bool TestResolveDataSourcesSignalsFalse_IncompatibleMap();

    //incompatible struct map
    bool TestResolveDataSourcesSignalsFalse_IncompatibleMap1();

    //pre-existent ds with type not allowed (map a structure on a leaf basically)
    bool TestResolveDataSourcesSignalsFalse_PartialDataSourceWithType();

    //Locked ds
    bool TestResolveDataSourcesSignalsFalse_AddSignalToLockedDataSource();

    //bool TestVerifyDataSourcesSignals();

    //Basic
    bool TestVerifyDataSourcesSignals1();

    //Partial struct node defined
    bool TestVerifyDataSourcesSignals_PartialDataSource();

    //Match of various defaults
    bool TestVerifyDataSourcesSignals_DefaultValuesMatch();

    //Automatic assignment of NumberOfElements
    bool TestVerifyDataSourcesSignals_AssignNumberOfElements();

    //Automatic assignment of NumberOfDimensions
    bool TestVerifyDataSourcesSignals_AssignNumberOfDimensions();

    //DS without type
    bool TestVerifyDataSourcesSignalsFalse_NoType();

    //no type
    bool TestVerifyDataSourcesSignalsFalse_NoType2();

    //NumberOfElements = 0
    bool TestVerifyDataSourcesSignalsFalse_InvalidNElements();

    //Incompatible defaults
    bool TestVerifyDataSourcesSignalsFalse_IncompatibleDefaults();

    //Incompatible defaults
    bool TestVerifyDataSourcesSignalsFalse_IncompatibleDefaultsMultiVarAsArray();

    //Basic
    bool TestResolveFunctionSignals1();

    //merge a struct
    bool TestResolveFunctionSignals2();

    //merge a struct
    bool TestResolveFunctionSignals3();

    //locked ds
    bool TestResolveFunctionSignalsFalse_CompleteSignalInLockedDataSource();

    //TODO The verify is useless??
    //basic
    bool TestVerifyFunctionSignals();

    //no type
    bool TestVerifyFunctionSignalsFalse_FalseNoType();

    //no #elements
    bool TestVerifyFunctionSignalsFalse_FalseNoNumberOfElements();

    //no #dimensions
    bool TestVerifyFunctionSignalsFalse_FalseNoNumberOfDimensions();

    //no qualified name
    bool TestVerifyFunctionSignalsFalse_FalseNoQualifiedName();

    //basic
    bool TestResolveStates(bool fromConfig);

    //basic
    bool TestResolveStatesFromConfig(bool fromConfig);

    //same gam in more states
    bool TestResolveStates2(bool fromConfig);

    //gam groups
    bool TestResolveStates_FunctionGroups(bool fromConfig);

    //more threads per state
    bool TestResolveStates_MoreThreads(bool fromConfig);

    //same gam in more than one thread in the same state
    bool TestResolveStatesFalse_SameGAMInMoreThreads(bool fromConfig);

    //more than one sync point in a thread
    bool TestResolveStatesFalse_MoreSyncs1(bool fromConfig);

    //more than one sync point in a thread
    bool TestResolveStatesFalse_MoreSyncs2(bool fromConfig);

    //more than one sync point in a thread
    bool TestResolveStatesFalse_MoreSyncs3(bool fromConfig);

    //invalid application
    bool TestResolveStatesFalse_InvalidApplication();

    //basic
    bool TestVerifyStates();

    //GAM never called
    bool TestVerifyStatesFalse_GAMNeverCalled();

    //basic
    bool TestResolveConsumersAndProducers();

    //more gam signals
    bool TestResolveConsumersAndProducers_MoreSignalsSameGAM();

    //more threads
    bool TestResolveConsumersAndProducers_MoreThreads();

    //more states
    bool TestResolveConsumersAndProducers_MoreStates();

    //more producers
    bool TestResolveConsumersAndProducers_MoreProducers();

    //basic
    bool TestVerifyConsumersAndProducers();

    //ranges
    bool TestVerifyConsumersAndProducers_Ranges();

    //ranges in structure
    bool TestVerifyConsumersAndProducers_Ranges2();

    //ranges overlap but in input
    bool TestVerifyConsumersAndProducers_Ranges3();

    //merging of time signals
    bool TestVerifyConsumersAndProducers_TimesSignals();

    //more than one producer
    bool TestVerifyConsumersAndProducersFalse_MoreProducers();

    //more than one producer
    bool TestVerifyConsumersAndProducersFalse_MoreProducers2();

    //range overlap fail
    bool TestVerifyConsumersAndProducersFalse_RangeOverlap();

    //range overlap fail same signal
    bool TestVerifyConsumersAndProducersFalse_RangeOverlap2();

    //producer in time signals
    bool TestVerifyConsumersAndProducersFalse_ProducerInTimeSignals();

    //the time signals must have a specific name
    bool TestVerifyConsumersAndProducersFalse_InvalidTimeSignals();

    //basic
    bool TestResolveFunctionSignalsMemorySize();

    //ranges
    bool TestResolveFunctionSignalsMemorySize_Ranges();

    //invalid ranges min>max
    bool TestResolveFunctionSignalsMemorySize_FalseInvalidRanges();

    //invalid ranges max>#elements
    bool TestResolveFunctionSignalsMemorySize_FalseInvalidRanges2();

    //invalid ranges field 3 columns
    bool TestResolveFunctionSignalsMemorySize_FalseInvalidRanges3();

    //basic
    bool TestResolveFunctionsMemory();

    //samples
    bool TestResolveFunctionsMemory_Samples();

    //interleaved ds
    bool TestResolveFunctionsMemory_InterleavedDs();

    //samples and blocks
    bool TestResolveFunctionsMemory_SamplesAndBlocks();


    //Samples =0
    bool TestResolveFunctionsMemoryFalse_InvalidSamples();

    //basic
    bool TestAssignFunctionsMemoryToDataSource();

    //same gam is producer and consumer
    bool TestAssignFunctionsMemoryToDataSource_GamIO();

    //basic
    bool TestAssignBrokersToFunctions();

    //unregistered ds if initialised from config
    bool TestAssignBrokersToFunctionsFalse_UnregisteredDataSource();

    //unsupported broker
    bool TestAssignBrokersToFunctionsFalse_UnsupportedBroker();

    //invalid application
    bool TestAssignBrokersToFunctionsFalse_InvalidApplication();

    bool TestSet();

    bool TestCopy();

    //basic
    bool TestPostConfigureDataSource();

    //invalid data source
    bool TestPostConfigureDataSourceFalse_InvalidDataSource();

    //basic
    bool TestPostConfigureFunctions();

    //invalid GAM
    bool TestPostConfigureFunctionsFalse_InvalidDataSource();


    bool TestConfigureAfterInitialisation();

    bool TestConfigureBeforeInitialisation();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONCONFIGURATIONBUILDERTEST_H_ */

