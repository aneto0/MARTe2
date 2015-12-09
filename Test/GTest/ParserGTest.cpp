/**
 * @file ParserGTest.cpp
 * @brief Source file for class ParserGTest
 * @date 30/11/2015
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
 * the class ParserGTest (public, protected, and private). Be aware that some 
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

#include "ParserTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ParserGTest,TestConstructor) {
    ParserTest parserTest;
    ASSERT_TRUE(parserTest.TestConstructor());
}

TEST(ParserGTest,TestGetGrammar) {
    ParserTest parserTest;
    ASSERT_TRUE(parserTest.TestGetGrammar());
}

TEST(ParserGTest,TestParseScalar) {
    ParserTest parserTest;
    ASSERT_TRUE(parserTest.TestParseScalar());
}

TEST(ParserGTest,TestParseVector) {
    ParserTest parserTest;
    ASSERT_TRUE(parserTest.TestParseVector());
}

TEST(ParserGTest,TestParseMatrix) {
    ParserTest parserTest;
    ASSERT_TRUE(parserTest.TestParseMatrix());
}

TEST(ParserGTest,TestNestedBlocks) {
    ParserTest parserTest;
    ASSERT_TRUE(parserTest.TestNestedBlocks());
}

TEST(ParserGTest,TestTypeCast_Scalar_int8) {
    ParserTest parserTest;
    const TypeCastTableTest<int8> table[]={
            {"var1=(int8) 127", "var1", 127, true},
            {"var2= (int8)-128", "var2", -128, true},
            {"var3=(int8)0b1", "var3", 1, true},
            {"var4=(int8)0x80", "var4", -128, true},
            {"var5=(int8) 0xFF", "var5", -1, true},
            {"var6=(int8) 129", "var6", 0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}


TEST(ParserGTest,TestTypeCast_Scalar_uint8) {
    ParserTest parserTest;
    const TypeCastTableTest<uint8> table[]={
            {"var1=(uint8) 127", "var1", 127, true},
            {"var2= (uint8)255", "var2", 255, true},
            {"var3=(uint8)0b1", "var3", 1, true},
            {"var4=(uint8) 0xFF", "var4", 255, true},
            {"var5=(uint8) 0x100", "var5", 0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}


TEST(ParserGTest,TestTypeCast_Scalar_int16) {
    ParserTest parserTest;
    const TypeCastTableTest<int16> table[]={
            {"var1=(int16) 32767", "var1", 32767, true},
            {"var2= (int16)-32768", "var2", -32768, true},
            {"var3=(int16)0b1", "var3", 1, true},
            {"var4=(int16)0x8000", "var4", -32768, true},
            {"var5=(int16) 0xFFFF", "var5", -1, true},
            {"var6=(int16) 32768", "var6", 0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}


TEST(ParserGTest,TestTypeCast_Scalar_uint16) {
    ParserTest parserTest;
    const TypeCastTableTest<uint16> table[]={
            {"var1=(uint16) 32767", "var1", 32767, true},
            {"var2= (uint16)65535", "var2", 65535, true},
            {"var3=(uint16)0b1", "var3", 1, true},
            {"var4=(uint16) 0xFFFF", "var4", 65535, true},
            {"var4=(uint16) 0x10000", "var4", 0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}




TEST(ParserGTest,TestTypeCast_Scalar_int32) {
    ParserTest parserTest;
    const TypeCastTableTest<int32> table[]={
            {"var1=(int32) 2147483647", "var1", 2147483647, true},
            {"var2= (int32)-2147483648", "var2", -2147483648, true},
            {"var3=(int32)0b1", "var3", 1, true},
            {"var4=(int32)0x80000000", "var4", -2147483648, true},
            {"var5=(int32) 0xFFFFFFFF", "var5", -1, true},
            {"var6=(int32) 2147483648", "var6", 0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}


TEST(ParserGTest,TestTypeCast_Scalar_uint32) {
    ParserTest parserTest;
    const TypeCastTableTest<uint32> table[]={
            {"var1=(uint32) 2147483647", "var1", 2147483647, true},
            {"var2= (uint32)4294967295", "var2", 4294967295, true},
            {"var3=(uint32)0b1", "var3", 1, true},
            {"var4=(uint32) 0xFFFFFFFF", "var4", 4294967295, true},
            {"var4=(uint32) 0x100000000", "var4", 0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}



TEST(ParserGTest,TestTypeCast_Scalar_int64) {
    ParserTest parserTest;
    const TypeCastTableTest<int64> table[]={
            {"var1=(int64) 9223372036854775807", "var1", (int64)9223372036854775807ul, true},
            {"var2= (int64)-9223372036854775808", "var2", (int64)-9223372036854775808ul, true},
            {"var3=(int64)0b1", "var3", 1, true},
            {"var4=(int64)0x8000000000000000", "var4", (int64)-9223372036854775808ul, true},
            {"var5=(int64) 0xFFFFFFFFFFFFFFFF", "var5", -1, true},
            {"var6=(int64) 9223372036854775808", "var6", 0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}


TEST(ParserGTest,TestTypeCast_Scalar_uint64) {
    ParserTest parserTest;
    const TypeCastTableTest<uint64> table[]={
            {"var1=(uint64) 9223372036854775807", "var1", 9223372036854775807ul, true},
            {"var2= (uint64)18446744073709551615", "var2", 18446744073709551615ul, true},
            {"var3=(uint64)0b1", "var3", 1, true},
            {"var4=(uint64) 0xFFFFFFFFFFFFFFFF", "var4", 18446744073709551615ul, true},
            {"var4=(uint64) 0x10000000000000000", "var4", 0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}


TEST(ParserGTest,TestTypeCast_Scalar_float32) {
    ParserTest parserTest;
    const TypeCastTableTest<float32> table[]={
            {"var1=(float32) 0xFF", "var1", 255.0f, true},
            {"var2= (float32)-340282346638528859811704183484516925440", "var2", -340282346638528859811704183484516925440.0f, true},
            {"var3=(float32)0b1", "var3", 1.0, true},
            {"var4=(float32) 340282346638528859811704183484516925440", "var4", 340282346638528859811704183484516925440.0f, true},
            // overflow
            {"var4=(float32) 123E+42", "var4", 0.0f, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}


TEST(ParserGTest,TestTypeCast_Scalar_float64) {
    ParserTest parserTest;
    const TypeCastTableTest<float64> table[]={
            {"var1=(float64) 0xFF", "var1", 255.0, true},
            {"var2= (float64)-340282346638528859811704183484516925440", "var2", -340282346638528859811704183484516925440.0f, true},
            {"var3=(float64)0b1", "var3", 1.0, true},
            {"var4=(float64) 340282346638528859811704183484516925440", "var4", 340282346638528859811704183484516925440.0f, true},
            // overflow
            {"var4=(float64) 123E+510", "var4", 0.0, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Scalar(table));
}

TEST(ParserGTest,TestTypeCast_Vector_int8) {
    ParserTest parserTest;
    const TypeCastTableTestVector<int8, 5> table[]={
            {"var1=(int8) {127 -128 0b1, 0x80 0xFF}", "var1", {127, -128, 1 , -128, -1}, true},
            {"var1=(int8) {127 -129 0b1, 0x80 0xFF}", "var1", {127, -128, 1 , -128, -1}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}


TEST(ParserGTest,TestTypeCast_Vector_uint8) {
    ParserTest parserTest;
    const TypeCastTableTestVector<uint8, 4> table[]={
            {"var1=(uint8) {127 255 0b1 0xFF}", "var1", {127, 255, 1, 255}, true},
            {"var1=(uint8) {1 0 -1 2}", "var1", {0}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}




TEST(ParserGTest,TestTypeCast_Vector_int16) {
    ParserTest parserTest;
    const TypeCastTableTestVector<int16, 5> table[]={
            {"var1=(int16) {32767 -32768 0b1 0x8000 0xFFFF}", "var1", {32767,-32768,1, -32768, -1}, true},
            {"var1=(int16) {32768 -32768 0b1 0x8000 0xFFFF}", "var1", {32767,-32768,1, -32768, -1}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}


TEST(ParserGTest,TestTypeCast_Vector_uint16) {
    ParserTest parserTest;
    const TypeCastTableTestVector<uint16, 4> table[]={
            {"var1=(uint16) {32767 65535 0b1 0xFFFF}", "var1", {32767, 65535, 1, 65535}, true},
            {"var1=(uint8) {1 0 -1 2}", "var1", {0}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}


TEST(ParserGTest,TestTypeCast_Vector_int32) {
    ParserTest parserTest;
    const TypeCastTableTestVector<int32, 5> table[]={
            {"var1=(int32) {2147483647 -2147483648 0b1 0x80000000 0xFFFFFFFF}", "var1", {2147483647, -2147483648, 1,-2147483648, -1}, true},
            {"var1=(int32) {2147483647 -2147483648 0b1 0x80000000 0x1FFFFFFFF}", "var1", {2147483647, -2147483648, 1,-2147483648, -1}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}


TEST(ParserGTest,TestTypeCast_Vector_uint32) {
    ParserTest parserTest;
    const TypeCastTableTestVector<uint32, 4> table[]={
            {"var1=(uint32) {2147483647, 4294967295 0b1 0xFFFFFFFF}", "var1", {2147483647, 4294967295, 1, 4294967295}, true},
            {"var1=(uint8) {1 0 -1 2}", "var1", {0}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}



TEST(ParserGTest,TestTypeCast_Vector_int64) {
    ParserTest parserTest;
    const TypeCastTableTestVector<int64, 5> table[]={
            {"var1=(int64) {9223372036854775807 -9223372036854775808 0b1 0x8000000000000000 0xFFFFFFFFFFFFFFFF}", "var1", {(int64)9223372036854775807ul, (int64)-9223372036854775808ul,1,(int64)-9223372036854775808ul,-1}, true},
            {"var1=(int64) {9223372036854775808 -9223372036854775808 0b1 0x8000000000000000 0xFFFFFFFFFFFFFFFF}", "var1", {(int64)9223372036854775807ul, (int64)-9223372036854775808ul,1,(int64)-9223372036854775808ul,-1}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}


TEST(ParserGTest,TestTypeCast_Vector_uint64) {
    ParserTest parserTest;
    const TypeCastTableTestVector<uint64, 4> table[]={
            {"var1=(uint64) {9223372036854775807 18446744073709551615 0b1 0xFFFFFFFFFFFFFFFF}", "var1", {9223372036854775807ul, 18446744073709551615ul, 1, 18446744073709551615ul}, true},
            {"var1=(uint8) {1 0 -1 2}", "var1", {0}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}

TEST(ParserGTest,TestTypeCast_Vector_float32) {
    ParserTest parserTest;
    const TypeCastTableTestVector<float32, 4> table[]={
            {"var1=(float32) {0xFF -340282346638528859811704183484516925440.0 0b1 340282346638528859811704183484516925440.0}", "var1", {255.0f, -340282346638528859811704183484516925440.0f, 1.0f, 340282346638528859811704183484516925440.0f}, true},
            {"var1=(uint8) {1.2 0.01 -1.2E+42 2}", "var1", {0}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}


TEST(ParserGTest,TestTypeCast_Vector_float64) {
    ParserTest parserTest;
    const TypeCastTableTestVector<float64, 4> table[]={
            {"var1=(float64) {0xFF -340282346638528859811704183484516925440.0 0b1 340282346638528859811704183484516925440.0}", "var1", {255.0, -340282346638528859811704183484516925440.0, 1.0, 340282346638528859811704183484516925440.0}, true},
            {"var1=(uint8) {1.2 0.01 -1.2E+510 2}", "var1", {0}, false},
            {0, 0, {0}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Vector(table));
}


TEST(ParserGTest,TestTypeCast_Matrix_int8) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<int8, 2,3> table[]={
            {"var1=(int8) {{127 -128 0b1} {0x80 0xFF, -1}}", "var1", {{127, -128, 1 },{ -128, -1, -1}}, true},
            {"var1=(int8) {{127 -128 0b1} {0x180 0xFF, -1}}", "var1", {{127, -128, 1 },{ -128, -1, -1}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}


TEST(ParserGTest,TestTypeCast_Matrix_uint8) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<uint8, 2,2> table[]={
            {"var1=(uint8) {{127 255}{0b1 0xFF}}", "var1", {{127, 255},{1, 255}}, true},
            {"var1=(uint8) {{127 -1}{0b1 0xFF}}", "var1", {{127, 255},{1, 255}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}




TEST(ParserGTest,TestTypeCast_Matrix_int16) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<int16, 2,3> table[]={
            {"var1=(int16) {{32767 -32768 0b1}{ 0x8000 0xFFFF -1}}", "var1", {{32767,-32768,1},{ -32768, -1, -1}}, true},
            {"var1=(int16) {{32767 -32769 0b1}{ 0x8000 0xFFFF -1}}", "var1", {{32767,-32768,1},{ -32768, -1, -1}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}


TEST(ParserGTest,TestTypeCast_Matrix_uint16) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<uint16, 2,2> table[]={
            {"var1=(uint16) {{32767 65535}{ 0b1 0xFFFF}}", "var1", {{32767, 65535},{ 1, 65535}}, true},
            {"var1=(uint8) {{127 1}{0b1 0x10000}}", "var1", {{127, 255},{1, 255}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}


TEST(ParserGTest,TestTypeCast_Matrix_int32) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<int32, 2,3> table[]={
            {"var1=(int32) {{2147483647 -2147483648 0b1}{ 0x80000000 0xFFFFFFFF -1}}", "var1", {{2147483647, -2147483648, 1},{-2147483648, -1, -1}}, true},
            {"var1=(int32) {{2147483648 -2147483648 0b1}{ 0x80000000 0xFFFFFFFF -1}}", "var1", {{2147483647, -2147483648, 1},{-2147483648, -1, -1}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}


TEST(ParserGTest,TestTypeCast_Matrix_uint32) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<uint32, 2,2> table[]={
            {"var1=(uint32) {{2147483647, 4294967295 }{0b1 0xFFFFFFFF}}", "var1", {{2147483647, 4294967295},{ 1, 4294967295}}, true},
            {"var1=(uint32) {{2147483647, 4294967296 }{0b1 0xFFFFFFFF}}", "var1", {{2147483647, 4294967295},{ 1, 4294967295}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}



TEST(ParserGTest,TestTypeCast_Matrix_int64) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<int64, 2,3> table[]={
            {"var1=(int64) {{9223372036854775807 -9223372036854775808 0b1}{ 0x8000000000000000 0xFFFFFFFFFFFFFFFF -1}}", "var1", {{(int64)9223372036854775807ul, (int64)-9223372036854775808ul,1},{(int64)-9223372036854775808ul,-1, -1}}, true},
            {"var1=(int64) {{9223372036854775807 -9223372036854775808 0b1}{ 0x18000000000000000 0xFFFFFFFFFFFFFFFF -1}}", "var1", {{(int64)9223372036854775807ul, (int64)-9223372036854775808ul,1},{(int64)-9223372036854775808ul,-1, -1}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}


TEST(ParserGTest,TestTypeCast_Matrix_uint64) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<uint64, 2,2> table[]={
            {"var1=(uint64) {{9223372036854775807 18446744073709551615}{ 0b1 0xFFFFFFFFFFFFFFFF}}", "var1", {{9223372036854775807ul, 18446744073709551615ul},{ 1, 18446744073709551615ul}}, true},
            {"var1=(uint64) {{9223372036854775807 18446744073709551615}{ -1 0xFFFFFFFFFFFFFFFF}}", "var1", {{9223372036854775807ul, 18446744073709551615ul},{ 1, 18446744073709551615ul}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}

TEST(ParserGTest,TestTypeCast_Matrix_float32) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<float32, 2,2> table[]={
            {"var1=(float32) {{0xFF -340282346638528859811704183484516925440.0}{ 0b1 340282346638528859811704183484516925440.0}}", "var1", {{255.0f, -340282346638528859811704183484516925440.0f}, {1.0f, 340282346638528859811704183484516925440.0f}}, true},
            {"var1=(uint8) {1.2 0.01 -1.2E+42 2}", "var1", {{0}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}


TEST(ParserGTest,TestTypeCast_Matrix_float64) {
    ParserTest parserTest;
    const TypeCastTableTestMatrix<float64, 2,2> table[]={
            {"var1=(float64) {{0xFF -340282346638528859811704183484516925440.0}{ 0b1 340282346638528859811704183484516925440.0}}", "var1", {{255.0, -340282346638528859811704183484516925440.0},{ 1.0, 340282346638528859811704183484516925440.0}}, true},
            {"var1=(uint8) {1.2 0.01 -1.2E+510 2}", "var1", {{0}}, false},
            {0, 0, {{0}}, 0}
    };

    ASSERT_TRUE(parserTest.TestTypeCast_Matrix(table));
}

TEST(ParserGTest,TestParseErrors_NotLValue_variable_1) {
    ParserTest parserTest;
    const char8* configString="block={\n"
            "                        var{10}\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_NotLValue_variable_2) {
    ParserTest parserTest;
    const char8* configString="block={\n"
            "                    //this is ok\n"
            "                        var=2\n"
            "                    /*this should \n"
            "                      fail */ \n"
            "                        1var=10\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}



TEST(ParserGTest,TestParseErrors_NotLValue_Block) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var={10}\n"
            "                        block2{\n"
            "                            var2=5\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_empty_vector) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={}\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_unterminated_vector) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={ 1 2 3  \n"
            "                            var3=5 \n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}

TEST(ParserGTest,TestParseErrors_mixTypes_vector) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={ 1 2 3 \"ciao\" }  \n"
            "                            var3=5 \n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}

TEST(ParserGTest,TestParseErrors_badCloseTerminal_vector) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={ 1 2 3 4 )  \n"
            "                            var3=5 \n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}

TEST(ParserGTest,TestParseErrors_empty_matrix) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={{}}\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}

TEST(ParserGTest,TestParseErrors_badVectorOpenTerminal_matrix) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={{1, 2} (3, 4} }\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_noVectorOpenTerminal_matrix) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={{1, 2} 3, 4} }\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_badVectorCloseTerminal_matrix) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={{1, 2) {3, 4} }\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_noVectorCloseTerminal_matrix) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={{1, 2 {3, 4} }\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}

TEST(ParserGTest,TestParseErrors_mixTypes_matrix) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={{1, 2} {3, Hello} }\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_badFormat_matrix) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={{1, 2} {3, 4, 5} }\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_typeCast_terminal_1) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2=(int8} {{1,2}{3,4}}\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_typeCast_terminal_2) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2=(int8 3\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}




TEST(ParserGTest,TestParseErrors_Too_many_CloseBlockTerminal_1) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2=3\n"
            "                        }\n"
            "                 }}";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}

TEST(ParserGTest,TestParseErrors_Too_many_CloseBlockTerminal_2) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={3 2}\n"
            "                        }\n"
            "                 }}";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}

TEST(ParserGTest,TestParseErrors_Too_many_CloseBlockTerminal_3) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2={{1 2}, {3,4}}\n"
            "                        }\n"
            "                 }}";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_Too_many_OpenBlockTerminal) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={{\n"
            "                            var2=3\n"
            "                        }\n"
            "                 }";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestParseErrors_Unexpected_EOF) {
    ParserTest parserTest;
    const char8* configString="block1={\n"
            "                        var=1\n"
            "                        block2={\n"
            "                            var2=3";
    ASSERT_TRUE(parserTest.TestParseErrors(configString));
}


TEST(ParserGTest,TestStandardCast) {
    ParserTest parserTest;
    ASSERT_TRUE(parserTest.TestStandardCast());
}
/*
TEST(ParserGTest,TestExistentFile) {
    ParserTest parserTest;
    ASSERT_TRUE(parserTest.TestExistentFile());
}

*/

