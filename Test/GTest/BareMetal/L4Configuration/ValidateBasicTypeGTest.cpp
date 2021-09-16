/**
 * @file ValidateBasicTypeGTest.cpp
 * @brief Source file for class ValidateBasicTypeGTest
 * @date 26/01/2016
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
 * the class ValidateBasicTypeGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ValidateBasicTypeTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_int8) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<int8> table[]={
            {"min=2\n max=120 \n", 2, true},
            {"min=2\n max=120 \n", 120, true},
            {"min=2\n max=120 \n", 1, false},
            {"min=2\n max=120 \n", 121, false},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 100, true},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 51, false},
            {"min=2\n max=120 \n values={1 -20 50 100 120}", -20, false},
            {"min=-21\n max=120 \n values={1 -20 50 100 120}", -20, true},
            {"min=-21\n max=120 \n values={1 -20 50 100 120}", -19, false},
            {"values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}\n min_size=2", 120, true},
            {"values={1 20 50 100 120}\n min_size=4", 120, false},
            {"min=2\n max=120\n max_size=4", 120, true},
            {"min=2\n max=120\n max_size=2", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_int16) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<int16> table[]={
            {"min=2\n max=120 \n", 2, true},
            {"min=2\n max=120 \n", 120, true},
            {"min=2\n max=120 \n", 1, false},
            {"min=2\n max=120 \n", 121, false},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 100, true},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}\n min_size=2", 120, true},
            {"values={1 20 50 100 120}\n min_size=4", 120, false},
            {"min=2\n max=120\n max_size=4", 120, true},
            {"min=2\n max=120\n max_size=2", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_int32) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<int32> table[]={
            {"min=2\n max=120 \n", 2, true},
            {"min=2\n max=120 \n", 120, true},
            {"min=2\n max=120 \n", 1, false},
            {"min=2\n max=120 \n", 121, false},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 100, true},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}\n min_size=2", 120, true},
            {"values={1 20 50 100 120}\n min_size=4", 120, false},
            {"min=2\n max=120\n max_size=4", 120, true},
            {"min=2\n max=120\n max_size=2", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_int64) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<int64> table[]={
            {"min=2\n max=120 \n", 2, true},
            {"min=2\n max=120 \n", 120, true},
            {"min=2\n max=120 \n", 1, false},
            {"min=2\n max=120 \n", 121, false},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 100, true},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}\n min_size=2", 120, true},
            {"values={1 20 50 100 120}\n min_size=4", 120, false},
            {"min=2\n max=120\n max_size=4", 120, true},
            {"min=2\n max=120\n max_size=2", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}



TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_uint8) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<uint8> table[]={
            {"min=2\n max=120 \n", 2, true},
            {"min=2\n max=120 \n", 120, true},
            {"min=2\n max=120 \n", 1, false},
            {"min=2\n max=120 \n", 121, false},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 100, true},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}\n min_size=2", 120, true},
            {"values={1 20 50 100 120}\n min_size=4", 120, false},
            {"min=2\n max=120\n max_size=4", 120, true},
            {"min=2\n max=120\n max_size=2", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_uint16) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<uint16> table[]={
            {"min=2\n max=120 \n", 2, true},
            {"min=2\n max=120 \n", 120, true},
            {"min=2\n max=120 \n", 1, false},
            {"min=2\n max=120 \n", 121, false},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 100, true},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}\n min_size=2", 120, true},
            {"values={1 20 50 100 120}\n min_size=4", 120, false},
            {"min=2\n max=120\n max_size=4", 120, true},
            {"min=2\n max=120\n max_size=2", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_uint32) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<uint32> table[]={
            {"min=2\n max=120 \n", 2, true},
            {"min=2\n max=120 \n", 120, true},
            {"min=2\n max=120 \n", 1, false},
            {"min=2\n max=120 \n", 121, false},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 100, true},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}\n min_size=2", 120, true},
            {"values={1 20 50 100 120}\n min_size=4", 120, false},
            {"min=2\n max=120\n max_size=4", 120, true},
            {"min=2\n max=120\n max_size=2", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_uint64) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<uint64> table[]={
            {"min=2\n max=120 \n", 2, true},
            {"min=2\n max=120 \n", 120, true},
            {"min=2\n max=120 \n", 1, false},
            {"min=2\n max=120 \n", 121, false},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 100, true},
            {"min=2\n max=120 \n values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}", 51, false},
            {"values={1 20 50 100 120}\n min_size=2", 120, true},
            {"values={1 20 50 100 120}\n min_size=4", 120, false},
            {"min=2\n max=120\n max_size=4", 120, true},
            {"min=2\n max=120\n max_size=2", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_float32) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<float32> table[]={
            {"min=-2.5\n max=123.25 \n", 2, true},
            {"min=-2.5\n max=123.25 \n", -2.6, false},
            {"min=0\n max=120 \n values={1.25 20.5 50.0 100.3 120}", 1.25, true},
            {"min=0\n max=120 \n values={1.25 20.5 50.0 100.3 120}", 1.26, false},
            {"min=0\n max=120 \n values={-1.25 20.5 50.0 100.3 120}", -1.25, false},
            {"min=-2\n max=120 \n values={-1.25 20.5 50.0 100.3 120}", -1.25, true},
            {"values={-1.25 20.5 50.0 100.3 120}", -1.25, true},
            {"values={-1.25 20.5 50.0 100.3 120}", 100.5, false},
            {"values={-1.25 20.5 50.0 100.3 120} \n min_size=1", -1.25, true},
            // this will be -1.250000
            {"values={-1.25 20.5 50.0 100.3 120} \n min_size=9", -1.25, true},
            {"values={-1.25 20.5 50.0 100.3 120} \n min_size=10", -1.25, false},
            {"values={-1.25 20.5 50.0 100.3 120} \n max_size=9", -1.25, true},
            {"values={-1.25 20.5 50.0 100.3 120} \n max_size=8", -1.25, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_float64) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<float64> table[]={
            {"min=-2.5\n max=123.25 \n", 2, true},
            {"min=-2.5\n max=123.25 \n", -2.6, false},
            {"min=0\n max=120 \n values={1.25 20.5 50.0 100.3 120}", 1.25, true},
            {"min=0\n max=120 \n values={1.25 20.5 50.0 100.3 120}", 1.26, false},
            {"min=0\n max=120 \n values={-1.25 20.5 50.0 100.3 120}", -1.25, false},
            {"min=-2\n max=120 \n values={-1.25 20.5 50.0 100.3 120}", -1.25, true},
            {"values={-1.25 20.5 50.0 100.3 120}", -1.25, true},
            {"values={-1.25 20.5 50.0 100.3 120}", 100.5, false},
            {"values={-1.25 20.5 50.0 100.3 120} \n min_size=1", -1.25, true},
            // this will be -1.25000000000000
            {"values={-1.25 20.5 50.0 100.3 120} \n min_size=17", -1.25, true},
            {"values={-1.25 20.5 50.0 100.3 120} \n min_size=18", -1.25, false},
            {"values={-1.25 20.5 50.0 100.3 120} \n max_size=17", -1.25, true},
            {"values={-1.25 20.5 50.0 100.3 120} \n max_size=16", -1.25, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_char8) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<char8> table[]={
            {"min=b\n max=y \n", 'c', true},
            {"min=b\n max=y \n", 'a', false},
            {"min=b\n max=y \n values=abc", 'a', false},
            {"min=b\n max=y \n values=abc", 'c', true},
            {"min=b\n max=y \n values=abc", 'd', false},
            {"values=abc", 'a', true},
            {"values=abc \n min_size=1", 'a', true},
            {"values=abc \n min_size=2", 'a', false},
            {"values=abc \n max_size=1", 'a', true},
            {"values=abc \n max_size=0", 'a', false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}




TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidate_string) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTable<const char8 *> table[]={
            {"min=bau\n max=miao \n", "ciao", true},
            {"min=bau\n max=miao \n", "ziao", false},
            {"max_size=5", "hello", true},
            {"max_size=5", "helloo", false},
            {"min=bau\n max=miao \n max_size=5", "ciaoo", true},
            {"min=bau\n max=miao \n min_size=3 \n max_size=5", "ciaoo", true},
            {"min=bau\n max=miao \n min_size=5 \n max_size=8", "ciaoo", true},
            {"min=bau\n max=miao \n min_size=6 \n max_size=8", "ciaoo", false},
            {"min=bau\n max=miao \n max_size=4", "ciaoo", false},
            {"min=bau\n max=miao \n max_size=5\n values={ miao, ciao, bau }", "bau", true},
            {"min=bau\n max=miao \n max_size=5\n values={ miao, ciao, bau }", "ciao", true},
            {"min=bau\n max=miao \n max_size=5\n values={ miao, ziao, bau }", "ziao", false},
            {"min=bau\n max=squit \n max_size=15\n values={ miao, ciao, bau }", "squit", false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicType(table));
}



TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidateVector_int32) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTableVector<int32, 3> table[]={
            {"min=2\n max=120 \n", {2, 50, 100 }, true},
            {"min=2\n max=120 \n", {2, 50, 120}, true},
            {"min=2\n max=120 \n", {1, 50, 100}, false},
            {0,{ 0}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicTypeVector(table));
}

TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidateVector_float32) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTableVector<float32, 3> table[]={
            {"min=2\n max=120 \n", {2.5, 50.1, 100.25 }, true},
            {"min=2\n max=120 \n", {2.3, 50.3, 119.9}, true},
            {"min=2\n max=120 \n", {1.5, 50.5, 100.25}, false},
            {0,{ 0}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicTypeVector(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidateVector_string) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTableVector<const char8 *, 3> table[]={
            {"min=bau\n max=miao \n", {"ciao", "miao", "bau"}, true},
            {"min=bau\n max=miao \n", {"ziao", "miao", "bau"}, false},
            {0, {0}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicTypeVector(table));
}



TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidateMatrix_int32) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTableMatrix<int32, 2,2> table[]={
            {"min=2\n max=120 \n", {{2, 50} ,{70, 100 }}, true},
            {"min=2\n max=120 \n", {{2, 50},{3, 120}}, true},
            {"min=2\n max=120 \n", {{1, 50}, {70, 100}}, false},
            {0, {{0}}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicTypeMatrix(table));
}

TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidateMatrix_float32) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTableMatrix<float32, 2,2> table[]={
            {"min=2\n max=120 \n", {{2.5, 50.1}, {3.9, 100.25 }}, true},
            {"min=2\n max=120 \n", {{2.3, 50.3}, {75.2, 119.9}}, true},
            {"min=2\n max=120 \n", {{1.5, 50.5}, {100.25, -3.5}}, false},
            {0, {{0}}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicTypeMatrix(table));
}


TEST(BareMetal_L4Configuration_ValidateBasicTypeGTest,TestValidateMatrix_string) {
    ValidateBasicTypeTest validateTest;

    const ValidateBasicTypeTestTableMatrix<const char8 *, 2,2> table[]={
            {"min=bau\n max=miao \n", {{"ciao", "miao"},{"beee", "bau"}}, true},
            {"min=bau\n max=miao \n", {{"ziao", "miao"}, {"beee","bau"}}, false},
            {0, {{0}}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateBasicTypeMatrix(table));
}




