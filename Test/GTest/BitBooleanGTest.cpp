/**
 * @file BitBooleanGTest.cpp
 * @brief Source file for class BitBooleanGTest
 * @date 28/ago/2015
 * @author pc
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
 * the class BitBooleanGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "BitBooleanTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*
TEST(BitBooleanGTest,myTest) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.myTest());
}
*/

TEST(BitBooleanGTest,TestCopyOperatorUnion) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestCopyOperatorUnion());
}



TEST(BitBooleanGTest,TestBoolCastU8) {
    BitBooleanTest<uint8> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}


TEST(BitBooleanGTest,TestBoolCastU16) {
    BitBooleanTest<uint16> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BitBooleanGTest,TestBoolCastU32) {
    BitBooleanTest<uint32> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BitBooleanGTest,TestBoolCastU64) {
    BitBooleanTest<uint64> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BitBooleanGTest,TestBoolCastS8) {
    BitBooleanTest<int8> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BitBooleanGTest,TestBoolCastS16) {
    BitBooleanTest<int16> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BitBooleanGTest,TestBoolCastS32) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BitBooleanGTest,TestBoolCastS64) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}


TEST(BitBooleanGTest,TestAnyTypeCast) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestAnyTypeCast());
}

TEST(BitBooleanGTest,TestBitSizeU8) {
    BitBooleanTest<uint8> bitbool;
    ASSERT_TRUE(bitbool.TestBitSize());
}

TEST(BitBooleanGTest,TestBitSizeU16) {
    BitBooleanTest<uint16> bitbool;
    ASSERT_TRUE(bitbool.TestBitSize());
}

TEST(BitBooleanGTest,TestBitSizeU32) {
    BitBooleanTest<uint32> bitbool;
    ASSERT_TRUE(bitbool.TestBitSize());
}

TEST(BitBooleanGTest,TestBitSizeU64) {
    BitBooleanTest<uint64> bitbool;
    ASSERT_TRUE(bitbool.TestBitSize());
}

TEST(BitBooleanGTest,TestBitSizeS8) {
    BitBooleanTest<int8> bitbool;
    ASSERT_TRUE(bitbool.TestBitSize());
}

TEST(BitBooleanGTest,TestBitSizeS16) {
    BitBooleanTest<int16> bitbool;
    ASSERT_TRUE(bitbool.TestBitSize());
}

TEST(BitBooleanGTest,TestBitSizeS32) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestBitSize());
}

TEST(BitBooleanGTest,TestBitSizeS64) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestBitSize());
}


TEST(BitBooleanGTest,TestBitOffsetU8) {
    BitBooleanTest<uint8> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BitBooleanGTest,TestBitOffsetU16) {
    BitBooleanTest<uint16> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BitBooleanGTest,TestBitOffsetU32) {
    BitBooleanTest<uint32> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BitBooleanGTest,TestBitOffsetU64) {
    BitBooleanTest<uint64> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BitBooleanGTest,TestBitOffsetS8) {
    BitBooleanTest<int8> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BitBooleanGTest,TestBitOffsetS16) {
    BitBooleanTest<int16> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BitBooleanGTest,TestBitOffsetS32) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BitBooleanGTest,TestBitOffsetS64) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BitBooleanGTest,TestOffsetOutOfRangeS32) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestOffsetOutOfRange());
}

TEST(BitBooleanGTest,TestOffsetOutOfRangeS64) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestOffsetOutOfRange());
}

