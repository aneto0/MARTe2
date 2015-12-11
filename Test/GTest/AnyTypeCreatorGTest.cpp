/**
 * @file AnyTypeCreatorGTest.cpp
 * @brief Source file for class AnyTypeCreatorGTest
 * @date 11/dic/2015
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
 * the class AnyTypeCreatorGTest (public, protected, and private). Be aware that some 
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
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "AnyTypeCreatorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(AnyTypeCreatorGTest, TestConstructor) {
    AnyTypeCreatorTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestConstructor());
}

TEST(AnyTypeCreatorGTest, TestCleanUp) {
    AnyTypeCreatorTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestCleanUp());
}

TEST(AnyTypeCreatorGTest, TestGetSize) {
    AnyTypeCreatorTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestGetSize(32));
}

TEST(AnyTypeCreatorGTest, TestGetGranularity) {
    AnyTypeCreatorTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestGetGranularity(32));
}

TEST(AnyTypeCreatorGTest, TestToType_uint8) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"uint8", "255",""},
            {"uint8", "-1",NULL},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_int8) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"int8", "127",""},
            {"int8", "-128",""},
            {"int8", "128",NULL},
            {"int8", "-129",NULL},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_uint16) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"uint16", "65535",""},
            {"uint16", "-1",0},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_int16) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"int16", "32767",""},
            {"int16", "-32768",""},
            {"int16", "32768",NULL},
            {"int16", "-32769",NULL},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_uint32) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"uint32", "4294967295",""},
            {"uint32", "-1",0},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_int32) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"int32", "2147483647",""},
            {"int32", "-2147483648",""},
            {"int32", "2147483648",NULL},
            {"int32", "-2147483649",NULL},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_uint64) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"uint64", "18446744073709551615",""},
            {"uint64", "-1",0},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_int64) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"int64", "9223372036854775807",""},
            {"int64", "-9223372036854775808",""},
            {"int64", "9223372036854775808",NULL},
            {"int64", "-9223372036854775809",NULL},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}


TEST(AnyTypeCreatorGTest, TestToType_float32) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"float32", "123.4",""},
            {"float32", "1.5E+50",NULL},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}


TEST(AnyTypeCreatorGTest, TestToType_float64) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"float64", "123.4",""},
            {"float64", "1.5E+520",NULL},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_string) {
    AnyTypeCreatorTest myAnyTypeTest;
    const char8 *table[][3]={
            {"string", "Hello",""},
            {"string", "World",""},
            {"boh", "World",""},
            {0,0,0}
    };

    ASSERT_TRUE(myAnyTypeTest.TestToType(table));
}

TEST(AnyTypeCreatorGTest, TestToType_TypeMismatch) {
    AnyTypeCreatorTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestToType_TypeMismatch());
}


TEST(AnyTypeCreatorGTest, TestSetType_NullMemory) {
    AnyTypeCreatorTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestSetType_NullMemory());
}

TEST(AnyTypeCreatorGTest, TestSetType_SizeMismatch) {
    AnyTypeCreatorTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestSetType_SizeMismatch());
}

TEST(AnyTypeCreatorGTest, TestSetType_int8) {
    AnyTypeCreatorTest myAnyTypeTest;
    int8 x=0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(2, 12, 15,SignedInteger8Bit, "int8", "1", x));
}


TEST(AnyTypeCreatorGTest, TestSetType_uint8) {
    AnyTypeCreatorTest myAnyTypeTest;
    uint8 x=0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(2, 15, 12,UnsignedInteger8Bit, "uint8", "1", x));
}

TEST(AnyTypeCreatorGTest, TestSetType_int16) {
    AnyTypeCreatorTest myAnyTypeTest;
    int16 x=0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(2, 4, 22,SignedInteger16Bit, "int16", "1", x));
}


TEST(AnyTypeCreatorGTest, TestSetType_uint16) {
    AnyTypeCreatorTest myAnyTypeTest;
    uint16 x=0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(1, 13, 1,UnsignedInteger16Bit, "uint16", "1", x));
}


TEST(AnyTypeCreatorGTest, TestSetType_int32) {
    AnyTypeCreatorTest myAnyTypeTest;
    int32 x=0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(2, 24, 2,SignedInteger32Bit, "int32", "1", x));
}


TEST(AnyTypeCreatorGTest, TestSetType_uint32) {
    AnyTypeCreatorTest myAnyTypeTest;
    uint32 x=0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(0, 1, 1,UnsignedInteger32Bit, "uint32", "1", x));
}


TEST(AnyTypeCreatorGTest, TestSetType_int64) {
    AnyTypeCreatorTest myAnyTypeTest;
    int64 x=0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(2, 4, 2,SignedInteger64Bit, "int64", "1", x));
}


TEST(AnyTypeCreatorGTest, TestSetType_uint64) {
    AnyTypeCreatorTest myAnyTypeTest;
    uint64 x=0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(2, 13, 13,UnsignedInteger64Bit, "uint64", "1", x));
}


TEST(AnyTypeCreatorGTest, TestSetType_float32) {
    AnyTypeCreatorTest myAnyTypeTest;
    float32 x=0.0f;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(1, 13, 1,Float32Bit, "float32", "1", x));
}

TEST(AnyTypeCreatorGTest, TestSetType_float64) {
    AnyTypeCreatorTest myAnyTypeTest;
    float64 x=0.0;
    ASSERT_TRUE(myAnyTypeTest.TestSetType(1, 13, 1,Float64Bit, "float64", "1", x));
}

TEST(AnyTypeCreatorGTest, TestSetType_String) {
    AnyTypeCreatorTest myAnyTypeTest;
    ASSERT_TRUE(myAnyTypeTest.TestSetType_String());
}




