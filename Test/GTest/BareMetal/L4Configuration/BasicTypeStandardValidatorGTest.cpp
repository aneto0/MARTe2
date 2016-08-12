/**
 * @file BasicTypeStandardValidatorGTest.cpp
 * @brief Source file for class BasicTypeStandardValidatorGTest
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
 * the class BasicTypeStandardValidatorGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicTypeStandardValidatorTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicTypeStandardValidatorGTest,TestValidate_int8) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<int8> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 1, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 121, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 100, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 -20 50 100 120}}", -20, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=-21\n max=120 \n values={1 -20 50 100 120}}", -20, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=-21\n max=120 \n values={1 -20 50 100 120}}", -19, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=2}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=4}", 120, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=4}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=2}", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}

TEST(BasicTypeStandardValidatorGTest,TestValidate_int16) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<int16> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 1, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 121, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 100, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=2}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=4}", 120, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=4}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=2}", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidate_int32) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<int32> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 1, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 121, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 100, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=2}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=4}", 120, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=4}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=2}", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidate_int64) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<int64> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 1, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 121, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 100, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=2}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=4}", 120, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=4}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=2}", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}



TEST(BasicTypeStandardValidatorGTest,TestValidate_uint8) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<uint8> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 1, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 121, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 100, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=2}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=4}", 120, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=4}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=2}", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidate_uint16) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<uint16> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 1, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 121, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 100, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=2}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=4}", 120, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=4}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=2}", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidate_uint32) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<uint32> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 1, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 121, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 100, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=2}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=4}", 120, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=4}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=2}", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidate_uint64) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<uint64> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 1, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", 121, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 100, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}}", 51, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=2}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={1 20 50 100 120}\n min_size=4}", 120, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=4}", 120, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120\n max_size=2}", 120, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidate_float32) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<float32> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=-2.5\n max=123.25 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=-2.5\n max=123.25 \n}", -2.6, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=0\n max=120 \n values={1.25 20.5 50.0 100.3 120}}", 1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=0\n max=120 \n values={1.25 20.5 50.0 100.3 120}}", 1.26, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=0\n max=120 \n values={-1.25 20.5 50.0 100.3 120}}", -1.25, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=-2\n max=120 \n values={-1.25 20.5 50.0 100.3 120}}", -1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120}}", -1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120}}", 100.5, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n min_size=1}", -1.25, true},
            // this will be -1.250000
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n min_size=9}", -1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n min_size=10}", -1.25, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n max_size=9}", -1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n max_size=8}", -1.25, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidate_float64) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<float64> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=-2.5\n max=123.25 \n}", 2, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=-2.5\n max=123.25 \n}", -2.6, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=0\n max=120 \n values={1.25 20.5 50.0 100.3 120}}", 1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=0\n max=120 \n values={1.25 20.5 50.0 100.3 120}}", 1.26, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=0\n max=120 \n values={-1.25 20.5 50.0 100.3 120}}", -1.25, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=-2\n max=120 \n values={-1.25 20.5 50.0 100.3 120}}", -1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120}}", -1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120}}", 100.5, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n min_size=1}", -1.25, true},
            // this will be -1.25000000000000
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n min_size=17}", -1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n min_size=18}", -1.25, false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n max_size=17}", -1.25, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values={-1.25 20.5 50.0 100.3 120} \n max_size=16}", -1.25, false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidate_char8) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<char8> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=b\n max=y \n}", 'c', true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=b\n max=y \n}", 'a', false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=b\n max=y \n values=abc}", 'a', false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=b\n max=y \n values=abc}", 'c', true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=b\n max=y \n values=abc}", 'd', false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values=abc}", 'a', true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values=abc \n min_size=1}", 'a', true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values=abc \n min_size=2}", 'a', false},
            {"+Valid1 = {Class=BasicTypeStandardValidator values=abc \n max_size=1}", 'a', true},
            {"+Valid1 = {Class=BasicTypeStandardValidator values=abc \n max_size=0}", 'a', false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}




TEST(BasicTypeStandardValidatorGTest,TestValidate_string) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTable<const char8 *> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n}", "ciao", true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n}", "ziao", false},
            {"+Valid1 = {Class=BasicTypeStandardValidator max_size=5}", "hello", true},
            {"+Valid1 = {Class=BasicTypeStandardValidator max_size=5}", "helloo", false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n max_size=5}", "ciaoo", true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n min_size=3 \n max_size=5}", "ciaoo", true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n min_size=5 \n max_size=8}", "ciaoo", true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n min_size=6 \n max_size=8}", "ciaoo", false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n max_size=4}", "ciaoo", false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n max_size=5\n values={ miao, ciao, bau }}", "bau", true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n max_size=5\n values={ miao, ciao, bau }}", "ciao", true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n max_size=5\n values={ miao, ziao, bau }}", "ziao", false},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=squit \n max_size=15\n values={ miao, ciao, bau }}", "squit", false},
            {0, 0, 0}
    };

    ASSERT_TRUE(validateTest.TestValidate(table));
}



TEST(BasicTypeStandardValidatorGTest,TestValidateVector_int32) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTableVector<int32, 3> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {2, 50, 100 }, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {2, 50, 120}, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {1, 50, 100}, false},
            {0,{ 0}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateVector(table));
}

TEST(BasicTypeStandardValidatorGTest,TestValidateVector_float32) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTableVector<float32, 3> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {2.5, 50.1, 100.25 }, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {2.3, 50.3, 119.9}, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {1.5, 50.5, 100.25}, false},
            {0,{ 0}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateVector(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidateVector_string) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTableVector<const char8 *, 3> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n}", {"ciao", "miao", "bau"}, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n}", {"ziao", "miao", "bau"}, false},
            {0, {0}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateVector(table));
}



TEST(BasicTypeStandardValidatorGTest,TestValidateMatrix_int32) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTableMatrix<int32, 2,2> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {{2, 50} ,{70, 100 }}, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {{2, 50},{3, 120}}, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {{1, 50}, {70, 100}}, false},
            {0, {{0}}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateMatrix(table));
}

TEST(BasicTypeStandardValidatorGTest,TestValidateMatrix_float32) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTableMatrix<float32, 2,2> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {{2.5, 50.1}, {3.9, 100.25 }}, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {{2.3, 50.3}, {75.2, 119.9}}, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=2\n max=120 \n}", {{1.5, 50.5}, {100.25, -3.5}}, false},
            {0, {{0}}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateMatrix(table));
}


TEST(BasicTypeStandardValidatorGTest,TestValidateMatrix_string) {
    BasicTypeStandardValidatorTest validateTest;

    const BasicTypeStandardValidatorTestTableMatrix<const char8 *, 2,2> table[]={
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n}", {{"ciao", "miao"},{"beee", "bau"}}, true},
            {"+Valid1 = {Class=BasicTypeStandardValidator min=bau\n max=miao \n}", {{"ziao", "miao"}, {"beee","bau"}}, false},
            {0, {{0}}, 0}
    };

    ASSERT_TRUE(validateTest.TestValidateMatrix(table));
}



