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
#include "MARTe2UTest.h"

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
TEST(BareMetal_L5GAMs_GAMGTest,TestConstructor) {
    GAMTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestInitialise) {
    GAMTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestAddSignals) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddSignals());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestSetConfiguredDatabase) {
    GAMTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetNumberOfInputSignals) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetNumberOfInputSignals());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetNumberOfOutputSignals) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetNumberOfOutputSignals());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalName) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalName());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalIndex) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalIndex());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalDataSourceName) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalDataSourceName());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalType) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalType());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalNumberOfDimensions) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfDimensions());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalNumberOfElements) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfElements());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalByteSize) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalByteSize());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalDefaultValue) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalDefaultValue());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalNumberOfByteOffsets) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfByteOffsets());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalByteOffsetInfo) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalByteOffsetInfo());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalNumberOfRanges) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfRanges());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalRangesInfo) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalRangesInfo());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalNumberOfSamples) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfSamples());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalFrequency_Input) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalFrequency_Input());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetSignalFrequency_Output) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetSignalFrequency_Output());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestAllocateInputSignalsMemory) {
    GAMTest test;
    ASSERT_TRUE(test.TestAllocateInputSignalsMemory());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestAllocateOutputSignalsMemory) {
    GAMTest test;
    ASSERT_TRUE(test.TestAllocateOutputSignalsMemory());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetInputSignalsMemoryPointer) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetInputSignalsMemory());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetOutputSignalsMemoryPointer) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetOutputSignalsMemory());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetInputSignalMemoryPointer) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetInputSignalMemory());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetOutputSignalMemoryPointer) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetOutputSignalMemory());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetInputSignalMemoryPointer_Samples) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetInputSignalMemory_Samples());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetOutputSignalMemoryPointer_Samples) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetOutputSignalMemory_Samples());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestAddInputBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddInputBrokers());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestAddInputBrokers_InvalidBroker) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddInputBrokers_InvalidBroker());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestAddOutputBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddOutputBrokers());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestAddOutputBrokers_InvalidBroker) {
    GAMTest test;
    ASSERT_TRUE(test.TestAddOutputBrokers_InvalidBroker());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestSortBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestSortBrokers());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetInputBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetOutputBrokers) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestGetQualifiedName) {
    GAMTest test;
    ASSERT_TRUE(test.TestGetQualifiedName());
}

TEST(BareMetal_L5GAMs_GAMGTest,TestExportData) {
    GAMTest test;
    ASSERT_TRUE(test.TestExportData());
}
