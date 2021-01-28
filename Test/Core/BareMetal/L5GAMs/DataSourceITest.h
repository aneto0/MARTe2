/**
 * @file DataSourceITest.h
 * @brief Header file for class DataSourceITest
 * @date 24/07/2016
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class DataSourceITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCEITEST_H_
#define DATASOURCEITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Tests the DataSourceI public methods.
 */
class DataSourceITest {
public:
    /**
     * @brief Tests the Constructor method.
     */
    bool TestConstructor();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the AddSignals method.
     */
    bool TestAddSignals();

    /**
     * @brief Tests the SetConfiguredDatabase method (This is tested implicitly).
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the GetNumberOfSignals method.
     */
    bool TestGetNumberOfSignals();

    /**
     * @brief Tests the GetSignalIndex method.
     */
    bool TestGetSignalIndex();

    /**
     * @brief Tests the GetSignalName method.
     */
    bool TestGetSignalName();

    /**
     * @brief Tests the GetSignalType method.
     */
    bool TestGetSignalType();

    /**
     * @brief Tests the GetSignalNumberOfDimensions method.
     */
    bool TestGetSignalNumberOfDimensions();

    /**
     * @brief Tests the GetSignalNumberOfElements method.
     */
    bool TestGetSignalNumberOfElements();

    /**
     * @brief Tests the GetSignalByteSize method.
     */
    bool TestGetSignalByteSize();

    /**
     * @brief Tests the GetSignalNumberOfStates method.
     */
    bool TestGetSignalNumberOfStates();

    /**
     * @brief Tests the GetSignalStateName method.
     */
    bool TestGetSignalStateName();

    /**
     * @brief Tests the GetSignalNumberOfConsumers method.
     */
    bool TestGetSignalNumberOfConsumers();

    /**
     * @brief Tests the GetSignalNumberOfProducers method.
     */
    bool TestGetSignalNumberOfProducers();

    /**
     * @brief Tests the GetSignalConsumerName method.
     */
    bool TestGetSignalConsumerName();

    /**
     * @brief Tests the GetSignalProducerName method.
     */
    bool TestGetSignalProducerName();

    /**
     * @brief Tests the GetSignalDefaultValue method.
     */
    bool TestGetSignalDefaultValue();

    /**
     * @brief Tests the GetSignalDefaultValueType method.
     */
    bool TestGetSignalDefaultValueType();

    /**
     * @brief Tests the GetNumberOfFunctions method.
     */
    bool TestGetNumberOfFunctions();

    /**
     * @brief Tests the GetFunctionName method.
     */
    bool TestGetFunctionName();

    /**
     * @brief Tests the GetFunctionIndex method.
     */
    bool TestGetFunctionIndex();

    /**
     * @brief Tests the GetFunctionNumberOfSignals method.
     */
    bool TestGetFunctionNumberOfSignals();

    /**
     * @brief Tests the GetFunctionSignalsByteSize method.
     */
    bool TestGetFunctionSignalsByteSize();

    /**
     * @brief Tests the GetFunctionSignalName method.
     */
    bool TestGetFunctionSignalName();

    /**
     * @brief Tests the GetFunctionSignalIndex method.
     */
    bool TestGetFunctionSignalIndex();

    /**
     * @brief Tests the GetFunctionSignalAlias method.
     */
    bool TestGetFunctionSignalAlias();

    /**
     * @brief Tests the GetFunctionSignalNumberOfByteOffsets method.
     */
    bool TestGetFunctionSignalNumberOfByteOffsets();

    /**
     * @brief Tests the GetFunctionSignalByteOffsetInfo method.
     */
    bool TestGetFunctionSignalByteOffsetInfo();

    /**
     * @brief Tests the GetFunctionSignalSamples method.
     */
    bool TestGetFunctionSignalSamples();

    /**
     * @brief Tests the GetFunctionSignalReadFrequencyInput method.
     */
    bool TestGetFunctionSignalReadFrequencyInput();

    /**
     * @brief Tests the GetFunctionSignalReadFrequencyOutput method.
     */
    bool TestGetFunctionSignalReadFrequencyOutput();

    /**
     * @brief Tests the GetFunctionSignalTrigger method.
     */
    bool TestGetFunctionSignalTrigger();

    /**
     * @brief Tests the GetFunctionSignalGAMMemoryOffset method.
     */
    bool TestGetFunctionSignalGAMMemoryOffset();

    /**
     * @brief Tests the GetAddBrokers method.
     */
    bool TestGetAddBrokers();

    /**
     * @brief Tests the IsSupportedBroker method.
     */
    bool TestIsSupportedBroker();

    /**
     * @brief Tests the GetCurrentStateBuffer() method.
     */
    bool TestGetCurrentStateBuffer();

    /**
     * @brief Tests the GetNumberOfStatefulMemoryBuffers() method.
     */
    bool TestGetNumberOfStatefulMemoryBuffers();

    /**
     * @brief Tests the PrepareInputOffsets() method.
     */
    bool TestPrepareInputOffsets();

    /**
     * @brief Tests the PrepareOutputOffsets() method.
     */
    bool TestPrepareOutputOffsets();

    /**
     * @brief Tests the GetInputOffset() method.
     */
    bool TestGetInputOffset();

    /**
     * @brief Tests the GetOutputOffset() method.
     */
    bool TestGetOutputOffset();

    /**
     * @brief Tests the TerminateInputCopy() method.
     */
    bool TestTerminateInputCopy();

    /**
     * @brief Tests the TerminateOutputCopy() method.
     */
    bool TestTerminateOutputCopy();

    /**
     * @brief Tests the GetNumberOfMemoryBuffers() method.
     */
    bool TestGetNumberOfMemoryBuffers();

    /**
     * @brief Tests the TestGetInputBrokers() method.
     */
    bool TestGetInputBrokers();

    /**
     * @brief Tests the TestGetOutputBrokers() method.
     */
    bool TestGetOutputBrokers();

    /**
     * @brief Tests the Synchronise() method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the BrokerCopyTerminated() method.
     */
    bool TestBrokerCopyTerminated();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCEITEST_H_ */

