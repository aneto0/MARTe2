/**
 * @file BufferTGTest.cpp
 * @brief Source file for class BufferTGTest
 * @date 11/04/2017
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

#include "BufferTTest.h"
#include "CompilerTypes.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

class TestClassForBufferT{
public:
    uint64 a;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BufferTGTest,TestBufferTConstructor_BasicType_32) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestConstructor(32));
}

TEST(BufferGTest,TestBufferTConstructor_BasicType_1) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestConstructor(1));
}

TEST(BufferGTest,TestBufferTConstructor_BasicType_0) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestConstructor(0));
}
TEST(BufferGTest,TestBufferTConstructor_ComplexType_32) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestConstructor(32));
    }

TEST(BufferGTest,TestBufferTConstructor_ComplexType_1) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestConstructor(1));
}

TEST(BufferGTest,TestBufferTConstructor_ComplexType_0) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestConstructor(0));
}

TEST(BufferGTest,TestBufferTInitialise_BasicType_32) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestInitialise(32));
}

TEST(BufferGTest,TestBufferTInitialise_BasicType_1) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestInitialise(1));
}

TEST(BufferGTest,TestBufferTInitialise_BasicType_0) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_FALSE(bufferTTest.TestInitialise(0));
}

TEST(BufferGTest,TestBufferTInitialise_ComplexType_32) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestInitialise(32));
}

TEST(BufferGTest,TestBufferTInitialise_ComplexType_1) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestInitialise(1));
}

TEST(BufferGTest,TestBufferTInitialise_ComplexType_0) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_FALSE(bufferTTest.TestInitialise(0));
}


TEST(BufferGTest,TestBufferTGetSize_BasicType_32) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestGetSize(32));
}

TEST(BufferGTest,TestBufferTGetSize_BasicType_1) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestGetSize(1));
}

TEST(BufferGTest,TestBufferTGetSize_BasicType_0) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_FALSE(bufferTTest.TestGetSize(0));
}

TEST(BufferGTest,TestBufferTGetSize_ComplexType_32) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestGetSize(32));
}

TEST(BufferGTest,TestBufferTGetSize_ComplexType_1) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestGetSize(1));
}

TEST(BufferGTest,TestBufferTGetSize_ComplexType_0) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_FALSE(bufferTTest.TestGetSize(0));
}

TEST(BufferGTest,TestBufferTGetData_BasicType_32) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestGetData(32));
}

TEST(BufferGTest,TestBufferTGetData_BasicType_1) {
   BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestGetData(1));
}

TEST(BufferGTest,TestBufferTGetData_BasicType_0) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_FALSE(bufferTTest.TestGetData(0));
}

TEST(BufferGTest,TestBufferTGetData_ComplexType_32) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestGetData(32));
}

TEST(BufferGTest,TestBufferTGetData_ComplexType_1) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestGetData(1));
}

TEST(BufferGTest,TestBufferTGetData_ComplexType_0) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_FALSE(bufferTTest.TestGetData(0));
}

TEST(BufferGTest,TestBufferTPutData_BasicType_32) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestPutData(32));
}

TEST(BufferGTest,TestBufferTPutData_BasicType_1) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestPutData(1));
}

TEST(BufferGTest,TestBufferTPutData_BasicType_0) {
    BufferTTest<uint64> bufferTTest;
    ASSERT_FALSE(bufferTTest.TestPutData(0));
}

TEST(BufferGTest,TestBufferTPutData_ComplexType_32) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestPutData(32));
}

TEST(BufferGTest,TestBufferTPutData_ComplexType_1) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_TRUE(bufferTTest.TestPutData(1));
}

TEST(BufferGTest,TestBufferTPutData_ComplexType_0) {
    BufferTTest<TestClassForBufferT> bufferTTest;
    ASSERT_FALSE(bufferTTest.TestPutData(0));
}
