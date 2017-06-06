/**
 * @file BufferGTest.cpp
 * @brief Source file for class BufferGTest
 * @date 07/04/2017
 * @author Andre' Torres
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
 * the class BufferGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BufferTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BufferGTest,TestConstructor_BasicType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestConstructor_BasicType(32));
}

TEST(BufferGTest,TestConstructor_BasicType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestConstructor_BasicType(1));
}

TEST(BufferGTest,TestConstructor_BasicType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestConstructor_BasicType(0));
}

TEST(BufferGTest,TestConstructor_ComplexType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestConstructor_ComplexType(32));
}

TEST(BufferGTest,TestConstructor_ComplexType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestConstructor_ComplexType(1));
}

TEST(BufferGTest,TestConstructor_ComplexType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestConstructor_ComplexType(0));
}

TEST(BufferGTest,TestInitialise_BasicType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestInitialise_BasicType(32));
}

TEST(BufferGTest,TestInitialise_BasicType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestInitialise_BasicType(1));
}

TEST(BufferGTest,TestInitialise_BasicType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_FALSE(bufferTest.TestInitialise_BasicType(0));
}

TEST(BufferGTest,TestInitialise_ComplexType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestInitialise_ComplexType(32));
}

TEST(BufferGTest,TestInitialise_ComplexType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestInitialise_ComplexType(1));
}

TEST(BufferGTest,TestInitialise_ComplexType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_FALSE(bufferTest.TestInitialise_ComplexType(0));
}

TEST(BufferGTest,TestGetSize_BasicType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestGetSize_BasicType(32));
}

TEST(BufferGTest,TestGetSize_BasicType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestGetSize_BasicType(1));
}

TEST(BufferGTest,TestGetSize_BasicType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_FALSE(bufferTest.TestGetSize_BasicType(0));
}

TEST(BufferGTest,TestGetSize_ComplexType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestGetSize_ComplexType(32));
}

TEST(BufferGTest,TestGetSize_ComplexType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestGetSize_ComplexType(1));
}

TEST(BufferGTest,TestGetSize_ComplexType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_FALSE(bufferTest.TestGetSize_ComplexType(0));
}

TEST(BufferGTest,TestGetData_BasicType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestGetData_BasicType(32));
}

TEST(BufferGTest,TestGetData_BasicType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestGetData_BasicType(1));
}

TEST(BufferGTest,TestGetData_BasicType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_FALSE(bufferTest.TestGetData_BasicType(0));
}

TEST(BufferGTest,TestGetData_ComplexType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestGetData_ComplexType(32));
}

TEST(BufferGTest,TestGetData_ComplexType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestGetData_ComplexType(1));
}

TEST(BufferGTest,TestGetData_ComplexType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_FALSE(bufferTest.TestGetData_ComplexType(0));
}

TEST(BufferGTest,TestPutData_BasicType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestPutData_BasicType(32));
}

TEST(BufferGTest,TestPutData_BasicType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestPutData_BasicType(1));
}

TEST(BufferGTest,TestPutData_BasicType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_FALSE(bufferTest.TestPutData_BasicType(0));
}

TEST(BufferGTest,TestPutData_ComplexType_32) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestPutData_ComplexType(32));
}

TEST(BufferGTest,TestPutData_ComplexType_1) {
    MARTe::BufferTest bufferTest;
    ASSERT_TRUE(bufferTest.TestPutData_ComplexType(1));
}

TEST(BufferGTest,TestPutData_ComplexType_0) {
    MARTe::BufferTest bufferTest;
    ASSERT_FALSE(bufferTest.TestPutData_ComplexType(0));
}
	
