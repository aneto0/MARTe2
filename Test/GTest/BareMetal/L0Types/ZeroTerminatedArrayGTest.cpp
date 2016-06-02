/**
 * @file ZeroTerminatedArrayGTest.cpp
 * @brief Source file for class ZeroTerminatedArrayGTest
 * @date 21/01/2016
 * @author Giuseppe Ferrò
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
 * the class ZeroTerminatedArrayGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "ZeroTerminatedArrayTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


TEST(ZeroTerminatedArrayGTest,TestConstructor_char8) {
    ZeroTerminatedArrayTest<char8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}

TEST(ZeroTerminatedArrayGTest,TestConstructor_int8) {
    ZeroTerminatedArrayTest<int8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}

TEST(ZeroTerminatedArrayGTest,TestConstructor_uint8) {
    ZeroTerminatedArrayTest<uint8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}

TEST(ZeroTerminatedArrayGTest,TestConstructor_int16) {
    ZeroTerminatedArrayTest<int16> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}


TEST(ZeroTerminatedArrayGTest,TestConstructor_uint16) {
    ZeroTerminatedArrayTest<uint16> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}


TEST(ZeroTerminatedArrayGTest,TestConstructor_int32) {
    ZeroTerminatedArrayTest<int32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}


TEST(ZeroTerminatedArrayGTest,TestConstructor_uint32) {
    ZeroTerminatedArrayTest<uint32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}

TEST(ZeroTerminatedArrayGTest,TestConstructor_int64) {
    ZeroTerminatedArrayTest<int64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}


TEST(ZeroTerminatedArrayGTest,TestConstructor_uint64) {
    ZeroTerminatedArrayTest<uint64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}

TEST(ZeroTerminatedArrayGTest,TestConstructor_float32) {
    ZeroTerminatedArrayTest<float32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}


TEST(ZeroTerminatedArrayGTest,TestConstructor_float64) {
    ZeroTerminatedArrayTest<float64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}

TEST(ZeroTerminatedArrayGTest,TestPositionOperator_char8) {
    ZeroTerminatedArrayTest<char8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}

TEST(ZeroTerminatedArrayGTest,TestPositionOperator_int8) {
    ZeroTerminatedArrayTest<int8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}

TEST(ZeroTerminatedArrayGTest,TestPositionOperator_uint8) {
    ZeroTerminatedArrayTest<uint8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}

TEST(ZeroTerminatedArrayGTest,TestPositionOperator_int16) {
    ZeroTerminatedArrayTest<int16> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}


TEST(ZeroTerminatedArrayGTest,TestPositionOperator_uint16) {
    ZeroTerminatedArrayTest<uint16> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}


TEST(ZeroTerminatedArrayGTest,TestPositionOperator_int32) {
    ZeroTerminatedArrayTest<int32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}


TEST(ZeroTerminatedArrayGTest,TestPositionOperator_uint32) {
    ZeroTerminatedArrayTest<uint32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}

TEST(ZeroTerminatedArrayGTest,TestPositionOperator_int64) {
    ZeroTerminatedArrayTest<int64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}


TEST(ZeroTerminatedArrayGTest,TestPositionOperator_uint64) {
    ZeroTerminatedArrayTest<uint64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}

TEST(ZeroTerminatedArrayGTest,TestPositionOperator_float32) {
    ZeroTerminatedArrayTest<float32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestPositionOperator());
}


TEST(ZeroTerminatedArrayGTest,TestPositionOperator_float64) {
    ZeroTerminatedArrayTest<float64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestConstructor());
}


TEST(ZeroTerminatedArrayGTest,TestGetSize_char8) {
    ZeroTerminatedArrayTest<char8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}

TEST(ZeroTerminatedArrayGTest,TestGetSize_int8) {
    ZeroTerminatedArrayTest<int8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}

TEST(ZeroTerminatedArrayGTest,TestGetSize_uint8) {
    ZeroTerminatedArrayTest<uint8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}

TEST(ZeroTerminatedArrayGTest,TestGetSize_int16) {
    ZeroTerminatedArrayTest<int16> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}


TEST(ZeroTerminatedArrayGTest,TestGetSize_uint16) {
    ZeroTerminatedArrayTest<uint16> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}


TEST(ZeroTerminatedArrayGTest,TestGetSize_int32) {
    ZeroTerminatedArrayTest<int32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}


TEST(ZeroTerminatedArrayGTest,TestGetSize_uint32) {
    ZeroTerminatedArrayTest<uint32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}

TEST(ZeroTerminatedArrayGTest,TestGetSize_int64) {
    ZeroTerminatedArrayTest<int64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}


TEST(ZeroTerminatedArrayGTest,TestGetSize_uint64) {
    ZeroTerminatedArrayTest<uint64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}

TEST(ZeroTerminatedArrayGTest,TestGetSize_float32) {
    ZeroTerminatedArrayTest<float32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}


TEST(ZeroTerminatedArrayGTest,TestGetSize_float64) {
    ZeroTerminatedArrayTest<float64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetSize());
}



TEST(ZeroTerminatedArrayGTest,TestGetList_char8) {
    ZeroTerminatedArrayTest<char8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}

TEST(ZeroTerminatedArrayGTest,TestGetList_int8) {
    ZeroTerminatedArrayTest<int8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}

TEST(ZeroTerminatedArrayGTest,TestGetList_uint8) {
    ZeroTerminatedArrayTest<uint8> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}

TEST(ZeroTerminatedArrayGTest,TestGetList_int16) {
    ZeroTerminatedArrayTest<int16> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}


TEST(ZeroTerminatedArrayGTest,TestGetList_uint16) {
    ZeroTerminatedArrayTest<uint16> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}


TEST(ZeroTerminatedArrayGTest,TestGetList_int32) {
    ZeroTerminatedArrayTest<int32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}


TEST(ZeroTerminatedArrayGTest,TestGetList_uint32) {
    ZeroTerminatedArrayTest<uint32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}

TEST(ZeroTerminatedArrayGTest,TestGetList_int64) {
    ZeroTerminatedArrayTest<int64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}


TEST(ZeroTerminatedArrayGTest,TestGetList_uint64) {
    ZeroTerminatedArrayTest<uint64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}

TEST(ZeroTerminatedArrayGTest,TestGetList_float32) {
    ZeroTerminatedArrayTest<float32> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}


TEST(ZeroTerminatedArrayGTest,TestGetList_float64) {
    ZeroTerminatedArrayTest<float64> ztArrayTest;
    ASSERT_TRUE(ztArrayTest.TestGetList());
}

