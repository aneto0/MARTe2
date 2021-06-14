/**
 * @file DataSourceIGTest.cpp
 * @brief Source file for class DataSourceIGTest
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

 * @details This source file contains the definition of all the methods for
 * the class DataSourceIGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DataSourceITest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L5GAMs_DataSourceIGTest,TestConstructor) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestInitialise) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestAddSignals) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestAddSignals());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestSetConfiguredDatabase) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetNumberOfSignals) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetNumberOfSignals());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalIndex) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalIndex());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalName());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalType) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalType());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalNumberOfDimensions) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfDimensions());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalNumberOfElements) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfElements());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalByteSize) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalByteSize());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalNumberOfStates) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfStates());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalStateName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalStateName());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalNumberOfConsumers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfConsumers());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalNumberOfProducers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfProducers());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalConsumerName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalConsumerName());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalProducerName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalProducerName());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalDefaultValue) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalDefaultValue());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetSignalDefaultValueType) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalDefaultValueType());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetNumberOfFunctions) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetNumberOfFunctions());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionIndex) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionIndex());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionName());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionNumberOfSignals) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionNumberOfSignals());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalsByteSize) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalsByteSize());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalIndex) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalIndex());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalAlias) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalAlias());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalNumberOfByteOffsets) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalNumberOfByteOffsets());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalByteOffsetInfo) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalByteOffsetInfo());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalSamples) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalSamples());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalReadFrequencyInput) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalReadFrequencyInput());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalReadFrequencyOutput) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalReadFrequencyOutput());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalTrigger) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalTrigger());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetFunctionSignalGAMMemoryOffset) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalGAMMemoryOffset());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetAddBrokers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetAddBrokers());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestIsSupportedBroker) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestIsSupportedBroker());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetCurrentStateBuffer) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetCurrentStateBuffer());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetNumberOfStatefulMemoryBuffers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetNumberOfStatefulMemoryBuffers());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestPrepareInputOffsets) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestPrepareInputOffsets());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestPrepareOutputOffsets) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestPrepareOutputOffsets());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetInputOffset) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetInputOffset());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetOutputOffset) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetOutputOffset());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestTerminateInputCopy) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestTerminateInputCopy());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestTerminateOutputCopy) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestTerminateOutputCopy());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetNumberOfMemoryBuffers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetNumberOfMemoryBuffers());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetInputBrokers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestGetOutputBrokers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestSynchronise) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(BareMetal_L5GAMs_DataSourceIGTest,TestBrokerCopyTerminated) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestBrokerCopyTerminated());
}

