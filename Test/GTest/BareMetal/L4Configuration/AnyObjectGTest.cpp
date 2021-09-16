/**
 * @file AnyObjectGTest.cpp
 * @brief Source file for class AnyObjectGTest
 * @date 21/11/2015
 * @author Andre Neto
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
 * the class AnyObjectGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyObjectTest.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestConstructor) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_Char8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::char8('a')));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Scalar_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Scalar(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_Char8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::char8('a')));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_CharStar) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar("ThisIsAString"));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Scalar_String) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Scalar_String());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_Char8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::char8('a')));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Vector_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Vector(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_Char8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::char8('a')));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Vector_Static_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Vector_Static(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_ScalarCharStar) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_ScalarCharStar());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_VectorCharArray) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_VectorChar());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_StaticCharArray) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_StaticCharArray());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_StaticArrayCharStar) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_StaticArrayCharStar());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_ScalarString) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_ScalarString());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_StaticStringArray) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_StaticStringArray());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_VectorString) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_VectorString());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Char8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::char8('a')));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_Char8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::char8('a')));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Static_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Static(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_Char8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::char8('a')));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Matrix_Array_Static_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Matrix_Array_Static(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_Char8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::char8('a')));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Matrix_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Matrix(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_Tensor_AnyType_Static_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_Tensor_AnyType_Static(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_UInt8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_Int8) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_UInt16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_Int16) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_UInt32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_Int32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_UInt64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_Int64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_Float32) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetType_Tensor_Float64) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetType_Tensor(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_StaticMatrixCharStar) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_StaticMatrixCharStar());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_MatrixCharStar) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_MatrixCharStar());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_StaticMatrixString) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_StaticMatrixString());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_MatrixString) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_MatrixString());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_StaticTensor) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_StaticTensor());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetClassProperties) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetClassProperties());
}

//TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetClassPropertiesCopy) {
//    AnyObjectTest anyObjTest;
//    ASSERT_TRUE(anyObjTest.TestGetClassPropertiesCopy());
//}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestGetObjectBuildFunction) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestGetObjectBuildFunction());
}


TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_StaticMatrixChar) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_StaticMatrixChar());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestSerialise_StaticMeshChar) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestSerialise_StaticMeshChar());
}

TEST(BareMetal_L4Configuration_AnyObjectGTest,TestCleanUp) {
    AnyObjectTest anyObjTest;
    ASSERT_TRUE(anyObjTest.TestCleanUp());
}

