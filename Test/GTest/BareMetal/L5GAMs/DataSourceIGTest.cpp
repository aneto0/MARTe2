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
#include <limits.h>
#include "gtest/gtest.h"

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
TEST(DataSourceIGTest,TestConstructor) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(DataSourceIGTest,TestInitialise) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(DataSourceIGTest,TestAddSignals) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestAddSignals());
}

TEST(DataSourceIGTest,TestSetConfiguredDatabase) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(DataSourceIGTest,TestGetNumberOfSignals) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetNumberOfSignals());
}

TEST(DataSourceIGTest,TestGetSignalIndex) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalIndex());
}

TEST(DataSourceIGTest,TestGetSignalName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalName());
}

TEST(DataSourceIGTest,TestGetSignalType) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalType());
}

TEST(DataSourceIGTest,TestGetSignalNumberOfDimensions) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfDimensions());
}

TEST(DataSourceIGTest,TestGetSignalNumberOfElements) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfElements());
}

TEST(DataSourceIGTest,TestGetSignalByteSize) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalByteSize());
}

TEST(DataSourceIGTest,TestGetSignalNumberOfStates) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfStates());
}

TEST(DataSourceIGTest,TestGetSignalStateName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalStateName());
}

TEST(DataSourceIGTest,TestGetSignalNumberOfConsumers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfConsumers());
}

TEST(DataSourceIGTest,TestGetSignalNumberOfProducers) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalNumberOfProducers());
}

TEST(DataSourceIGTest,TestGetSignalConsumerName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalConsumerName());
}

TEST(DataSourceIGTest,TestGetSignalProducerName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalProducerName());
}

TEST(DataSourceIGTest,TestGetSignalDefaultValue) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetSignalDefaultValue());
}

TEST(DataSourceIGTest,TestGetNumberOfFunctions) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetNumberOfFunctions());
}

TEST(DataSourceIGTest,TestGetFunctionIndex) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionIndex());
}

TEST(DataSourceIGTest,TestGetFunctionName) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionName());
}

TEST(DataSourceIGTest,TestGetFunctionNumberOfSignals) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionNumberOfSignals());
}

TEST(DataSourceIGTest,TestGetFunctionSignalsByteSize) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalsByteSize());
}

TEST(DataSourceIGTest,TestGetFunctionSignalIndex) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalIndex());
}

TEST(DataSourceIGTest,TestGetFunctionSignalAlias) {
    DataSourceITest test;
    ASSERT_TRUE(test.TestGetFunctionSignalAlias());
}
