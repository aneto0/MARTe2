/**
 * @file HashFunctionGTest.cpp
 * @brief Source file for class HashFunctionGTest
 * @date 19/08/2019
 * @author Giuseppe Ferro
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
 * the class HashFunctionGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "HashFunctionTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_uint8) {
    HashFunctionTest < uint8 > test;
    uint8 input = 123;
    uint32 result = 177630;
    ASSERT_TRUE(test.TestDJB(input, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_int8) {
    HashFunctionTest < int8 > test;
    int8 input = -123;
    uint32 result = 177440;
    ASSERT_TRUE(test.TestDJB(input, result));
}


TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_uint16) {
    HashFunctionTest < uint16 > test;
    uint16 input = 1234;
    uint32 result = 5858387;
    ASSERT_TRUE(test.TestDJB(input, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_int16) {
    HashFunctionTest < int16 > test;
    int16 input = -1234;
    uint32 result = 5858832;
    ASSERT_TRUE(test.TestDJB(input, result));
}


TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_uint32) {
    HashFunctionTest < uint32 > test;
    uint32 input = 123;
    uint32 result = 2088522014;
    ASSERT_TRUE(test.TestDJB(input, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_int32) {
    HashFunctionTest < int32 > test;
    int32 input = -1234;
    uint32 result = 2085307952;
    ASSERT_TRUE(test.TestDJB(input, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_uint64) {
    HashFunctionTest < uint64 > test;
    uint64 input = 123;
    uint32 result = 375107614;
    ASSERT_TRUE(test.TestDJB(input, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_int64) {
    HashFunctionTest < int64 > test;
    int64 input = -1234;
    uint32 result = 2688058992;
    ASSERT_TRUE(test.TestDJB(input, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_float32) {
    HashFunctionTest < float32 > test;
    float32 input = 123.4;
    uint32 result = 2084352592;
    ASSERT_TRUE(test.TestDJB(input, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestDJB_float64) {
    HashFunctionTest < float64 > test;
    float64 input = -1234.25;
    uint32 result = 2075290303;
    ASSERT_TRUE(test.TestDJB(input, result));
}




TEST(BareMetal_L0Types_HashFunctionGTest,TestDJBPointer) {
    HashFunctionTest < uint32 > test;
    uint32 input[] = {123, 1234};
    uint32 size=2;
    uint32 result = 0;
    ASSERT_TRUE(test.TestDJB(input, size, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestFNV1A) {
    HashFunctionTest < uint32 > test;
    uint32 input = 123;
    uint32 result = 0;
    ASSERT_TRUE(test.TestFNV1A(input, result));
}

TEST(BareMetal_L0Types_HashFunctionGTest,TestFNV1APointer) {
    HashFunctionTest < uint32 > test;
    uint32 input[] = {123, 1234};
    uint32 size=2u;
    uint32 result = 0;
    ASSERT_TRUE(test.TestFNV1A(input, size, result));
}

