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

    bool TestFlattenSignalsDatabases();

    bool TestResolveDataSources();

    bool TestResolveDataSourceFalse_TypeMismatch();

    bool TestResolveDataSourceFalse_NElementsMismatch();

    bool TestResolveDataSourceFalse_NDimensionsMismatch();

    bool TestVerifyDataSourcesSignals();

    bool TestVerifyDataSourcesSignals_PartialInPredefinedDsNoType();

    bool TestVerifyDataSourcesSignalsFalse_NoTypeInPredefinedDs();

    bool TestVerifyDataSourcesSignalsFalse_PartialInPredefinedDsWithType();

    bool TestResolveFunctionSignals();

    bool TestResolveFunctionSignals2();

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

    bool TestAllocateFunctionsMemory();

    bool TestCalculateFunctionsMemory();

    bool TestAssignFunctionsMemoryToDataSource();

    bool TestAssignBrokersToFunctions();

    bool TestAllocateGAMMemory();


};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEAPPLICATIONCONFIGURATIONBUILDERTEST_H_ */

