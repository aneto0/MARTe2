/**
 * @file GAMGTest.cpp
 * @brief Source file for class GAMGTest
 * @date 29/07/2016
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
 * the class GAMGTest (public, protected, and private). Be aware that some 
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
#include "GAMTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(GAMGTest,TestConstructor) {
    GAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(GAMGTest,TestInitialise) {
    GAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(GAMGTest,TestAddSignals) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddSignals());
}

TEST(GAMGTest,TestSetConfiguredDatabase) {
    GAMTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(GAMGTest,TestGetNumberOfInputSignals) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetNumberOfInputSignals());
}

TEST(GAMGTest,TestGetNumberOfOutputSignals) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetNumberOfOutputSignals());
}

TEST(GAMGTest,TestGetSignalName) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalName());
}

TEST(GAMGTest,TestGetSignalIndex) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalIndex());
}

TEST(GAMGTest,TestGetSignalDataSourceName) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalDataSourceName());
}

TEST(GAMGTest,TestGetSignalType) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalType());
}

TEST(GAMGTest,TestGetSignalNumberOfDimensions) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfDimensions());
}

TEST(GAMGTest,TestGetSignalNumberOfElements) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfElements());
}

TEST(GAMGTest,TestGetSignalByteSize) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalByteSize());
}

TEST(GAMGTest,TestGetSignalDefaultValue) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalDefaultValue());
}

TEST(GAMGTest,TestGetSignalNumberOfByteOffsets) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfByteOffsets());
}

TEST(GAMGTest,TestGetSignalByteOffsetInfo) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalByteOffsetInfo());
}

TEST(GAMGTest,TestGetSignalNumberOfRanges) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfRanges());
}

TEST(GAMGTest,TestGetSignalRangesInfo) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalRangesInfo());
}

TEST(GAMGTest,TestGetSignalNumberOfSamples) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfSamples());
}

TEST(GAMGTest,TestGetSignalFrequency_Input) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalFrequency_Input());
}

TEST(GAMGTest,TestGetSignalFrequency_Output) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalFrequency_Output());
}

TEST(GAMGTest,TestAllocateInputSignalsMemory) {
    GAMTest test;
    ASSERT_TRUE(test.TestAllocateInputSignalsMemory());
}

TEST(GAMGTest,TestAllocateOutputSignalsMemory) {
    GAMTest test;
    ASSERT_TRUE(test.TestAllocateOutputSignalsMemory());
}

TEST(GAMGTest,TestGetInputSignalsMemoryPointer) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetInputSignalsMemory());
}

TEST(GAMGTest,TestGetOutputSignalsMemoryPointer) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetOutputSignalsMemory());
}

TEST(GAMGTest,TestGetInputSignalMemoryPointer) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetInputSignalMemory());
}

TEST(GAMGTest,TestGetOutputSignalMemoryPointer) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetOutputSignalMemory());
}

TEST(GAMGTest,TestGetInputSignalMemoryPointer_Samples) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetInputSignalMemory_Samples());
}

TEST(GAMGTest,TestGetOutputSignalMemoryPointer_Samples) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetOutputSignalMemory_Samples());
}

TEST(GAMGTest,TestAddInputBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddInputBrokers());
}

TEST(GAMGTest,TestAddInputBrokers_InvalidBroker) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddInputBrokers_InvalidBroker());
}

TEST(GAMGTest,TestAddOutputBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddOutputBrokers());
}

TEST(GAMGTest,TestAddOutputBrokers_InvalidBroker) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddOutputBrokers_InvalidBroker());
}

TEST(GAMGTest,TestGetInputBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(GAMGTest,TestGetOutputBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(GAMGTest,TestGetQualifiedName) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetQualifiedName());
}
