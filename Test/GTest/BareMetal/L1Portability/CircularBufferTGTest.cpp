/**
 * @file CircularBufferTGTest.cpp
 * @brief Source file for class CircularBufferTGTest
 * @date 18/04/2017
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

#include "CircularBufferTTest.h"
#include "CompilerTypes.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

class TestClassForCircularBufferT{
public:
    uint64 a;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(CircularBufferTGTest,TestCircularBufferTPushData_BasicType_32) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(32));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData_BasicType_1) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(1));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData_BasicType_0) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(0));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData_ComplexType_32) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(32));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData_ComplexType_1) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(1));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData_ComplexType_0) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(0));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData2_BasicType_32) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(32));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData2_BasicType_1) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(1));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData2_BasicType_0) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(0));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData2_ComplexType_32) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(32));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData2_ComplexType_1) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(1));
}

TEST(CircularBufferTGTest,TestCircularBufferTPushData2_ComplexType_0) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(0));
}

TEST(CircularBufferTGTest,TestCircularBufferTGetLast_BasicType_32) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(32));
}

TEST(CircularBufferTGTest,TestCircularBufferTGetLast_BasicType_1) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(1));
}

TEST(CircularBufferTGTest,TestCircularBufferTGetLast_BasicType_0) {
    CircularBufferTTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(0));
}

TEST(CircularBufferTGTest,TestCircularBufferTGetLast_ComplexType_32) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(32));
}

TEST(CircularBufferTGTest,TestCircularBufferTGetLast_ComplexType_1) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(1));
}

TEST(CircularBufferTGTest,TestCircularBufferTGetLast_ComplexType_0) {
    CircularBufferTTest<TestClassForCircularBufferT> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(0));
}
