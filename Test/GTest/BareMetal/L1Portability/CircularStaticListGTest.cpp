/**
 * @file CircularStaticListGTest.cpp
 * @brief Source file for class CircularStaticListGTest
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

#include <CircularStaticListTest.h>
#include "CompilerTypes.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

class TestClassForCircularStaticList{
public:
    uint64 a;
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData_BasicType_32) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(32));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData_BasicType_1) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(1));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData_BasicType_0) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(0));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData_ComplexType_32) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(32));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData_ComplexType_1) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(1));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData_ComplexType_0) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData(0));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData2_BasicType_32) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(32));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData2_BasicType_1) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(1));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData2_BasicType_0) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(0));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData2_ComplexType_32) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(32));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData2_ComplexType_1) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(1));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListPushData2_ComplexType_0) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestPushData2(0));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListGetLast_BasicType_32) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(32));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListGetLast_BasicType_1) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(1));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListGetLast_BasicType_0) {
    CircularStaticListTest<uint64> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(0));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListGetLast_ComplexType_32) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(32));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListGetLast_ComplexType_1) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(1));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestCircularStaticListGetLast_ComplexType_0) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetLast(0));
}

TEST(BareMetal_L1Portability_CircularStaticListGTest,TestGetIndex) {
    CircularStaticListTest<TestClassForCircularStaticList> circularBufferTTest;
    ASSERT_TRUE(circularBufferTTest.TestGetIndex(16));
}
