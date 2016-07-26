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

    bool TestInitialiseSignalsDatabase();

    bool TestInitialiseSignalsDatabase1();

    bool TestFlattenSignalsDatabases();

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

    //no type
    bool TestResolveFunctionSignals_FalseNoType();





    bool TestResolveFunctionSignalsFalse_NoTypeInGAMSignal();

    bool TestVerifyFunctionSignals();

    bool TestResolveStates();

    bool TestResolveStatesFalse_SameGAMIn2Threads();

    bool TestResolveStatesFalse_MoreThanOneSyncInAThread_SameGAM();

    bool TestResolveConsumersAndProducers();

    bool TestVerifyConsumersAndProducers();

    bool TestVerifyConsumersAndProducersFalse_TwoProducers();

    bool TestVerifyConsumersAndProducersFalse_MemoryOverlap();

    bool TestResolveFunctionSignalsMemorySize();

    bool TestResolveFunctionSignalsMemorySizeFalse_WrongRangeMaxMin();

    bool TestResolveFunctionSignalsMemorySizeFalse_WrongRangeMaxNElements();

    bool TestResolveFunctionsMemory();

    bool TestCalculateFunctionsMemory();

    bool TestAssignFunctionsMemoryToDataSource();

    bool TestAssignBrokersToFunctions();

    bool TestAllocateGAMMemory();


};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONCONFIGURATIONBUILDERTEST_H_ */

