/**
 * @file BitBooleanGTest.cpp
 * @brief Source file for class BitBooleanGTest
 * @date 28/08/2015
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
 * the class BitBooleanGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "BitBooleanTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
/*
TEST(BareMetal_L0Types_BitBooleanGTest,myTest) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.myTEST(BareMetal_L0Types_));
}
*/

TEST(BareMetal_L0Types_BitBooleanGTest,TestCopyOperatorUnion) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestCopyOperatorUnion());
}



TEST(BareMetal_L0Types_BitBooleanGTest,TestBoolCastU8) {
    BitBooleanTest<uint8> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}


TEST(BareMetal_L0Types_BitBooleanGTest,TestBoolCastU16) {
    BitBooleanTest<uint16> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBoolCastU32) {
    BitBooleanTest<uint32> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBoolCastU64) {
    BitBooleanTest<uint64> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBoolCastS8) {
    BitBooleanTest<int8> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBoolCastS16) {
    BitBooleanTest<int16> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBoolCastS32) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBoolCastS64) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestBoolCast());
}


TEST(BareMetal_L0Types_BitBooleanGTest,TestAnyTypeCast) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestAnyTypeCast());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestNumberOfBitsU8) {
    BitBooleanTest<uint8> bitbool;
    ASSERT_TRUE(bitbool.TestNumberOfBits());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestNumberOfBitsU16) {
    BitBooleanTest<uint16> bitbool;
    ASSERT_TRUE(bitbool.TestNumberOfBits());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestNumberOfBitsU32) {
    BitBooleanTest<uint32> bitbool;
    ASSERT_TRUE(bitbool.TestNumberOfBits());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestNumberOfBitsU64) {
    BitBooleanTest<uint64> bitbool;
    ASSERT_TRUE(bitbool.TestNumberOfBits());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestNumberOfBitsS8) {
    BitBooleanTest<int8> bitbool;
    ASSERT_TRUE(bitbool.TestNumberOfBits());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestNumberOfBitsS16) {
    BitBooleanTest<int16> bitbool;
    ASSERT_TRUE(bitbool.TestNumberOfBits());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestNumberOfBitsS32) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestNumberOfBits());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestNumberOfBitsS64) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestNumberOfBits());
}


TEST(BareMetal_L0Types_BitBooleanGTest,TestBitOffsetU8) {
    BitBooleanTest<uint8> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBitOffsetU16) {
    BitBooleanTest<uint16> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBitOffsetU32) {
    BitBooleanTest<uint32> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBitOffsetU64) {
    BitBooleanTest<uint64> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBitOffsetS8) {
    BitBooleanTest<int8> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBitOffsetS16) {
    BitBooleanTest<int16> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBitOffsetS32) {
    BitBooleanTest<int32> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

TEST(BareMetal_L0Types_BitBooleanGTest,TestBitOffsetS64) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestBitOffset());
}

