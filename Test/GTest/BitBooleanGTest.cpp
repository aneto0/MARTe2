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

TEST(BitBooleanGTest,TestCopyOperatorUnion) {
    BitBooleanTest<int64> bitbool;
    ASSERT_TRUE(bitbool.TestCopyOperatorUnion());
}



TEST(BitBooleanGTest,TestBoolCastU8) {
    BitBooleanTest<uint8> bitbool;
    uint8 x=0;
    ASSERT_TRUE(bitbool.TestBoolCast(x));
}


TEST(BitBooleanGTest,TestBoolCastU16) {
    BitBooleanTest<uint16> bitbool;
    uint16 x=0;
    ASSERT_TRUE(bitbool.TestBoolCast(x));
}

TEST(BitBooleanGTest,TestBoolCastU32) {
    BitBooleanTest<uint32> bitbool;
    uint32 x=0;
    ASSERT_TRUE(bitbool.TestBoolCast(x));
}

TEST(BitBooleanGTest,TestBoolCastU64) {
    BitBooleanTest<uint64> bitbool;
    uint64 x=0;
    ASSERT_TRUE(bitbool.TestBoolCast(x));
}

TEST(BitBooleanGTest,TestBoolCastS8) {
    BitBooleanTest<int8> bitbool;
    int8 x=0;
    ASSERT_TRUE(bitbool.TestBoolCast(x));
}

TEST(BitBooleanGTest,TestBoolCastS16) {
    BitBooleanTest<int16> bitbool;
    int16 x=0;
    ASSERT_TRUE(bitbool.TestBoolCast(x));
}

TEST(BitBooleanGTest,TestBoolCastS32) {
    BitBooleanTest<int32> bitbool;
    int32 x=0;
    ASSERT_TRUE(bitbool.TestBoolCast(x));
}

TEST(BitBooleanGTest,TestBoolCastS64) {
    BitBooleanTest<int64> bitbool;
    int64 x=0;
    ASSERT_TRUE(bitbool.TestBoolCast(x));
}


TEST(BitBooleanGTest,TestAnyTypeCast) {
    BitBooleanTest<int32> bitbool;
    int32 x=0;
    ASSERT_TRUE(bitbool.TestAnyTypeCast(x));
}
