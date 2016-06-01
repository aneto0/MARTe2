/**
 * @file TypeConversionTest.cpp
 * @brief Source file for class TypeConversionTest
 * @date 12/nov/2015
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
 * the class TypeConversionTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TypeConversionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool TypeConversionTest::TestCArrayToCCStringScalar() {

    bool ret = true;
    char8 input[32];

    StringHelper::Copy(input, "HelloWorld");

    char8 *output = (char8*) HeapManager::Malloc(32);

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(input, output) != 0) {
        ret = false;
    }

    HeapManager::Free((void*&) output);

    return ret;
}
bool TypeConversionTest::TestCArrayToCCStringVector() {
    bool ret = true;
    char8 input[2][32];

    StringHelper::Copy(&input[0][0], "HelloWorld_1");
    StringHelper::Copy(&input[1][0], "HelloWorld_2");

    char8 **vector = (char8**) HeapManager::Malloc(2 * sizeof(char*));
    vector[0] = (char8*) HeapManager::Malloc(32 * sizeof(char));
    vector[1] = (char8*) HeapManager::Malloc(32 * sizeof(char));

    Vector<char8*> output(vector, 2);

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(&input[0][0], output[0]) != 0) {
        ret = false;
    }
    if (StringHelper::Compare(&input[1][0], output[1]) != 0) {
        ret = false;
    }

    HeapManager::Free((void*&) output[0]);
    HeapManager::Free((void*&) output[1]);
    HeapManager::Free((void*&) output);

    return ret;
}

bool TypeConversionTest::TestCArrayToCCStringMatrix() {

    bool ret = true;
    const uint32 nRows = 2;
    const uint32 nCols = 3;
    const uint32 buffSize = 32;
    char8 input[nRows][nCols][buffSize];

    StringHelper::Copy(&input[0][0][0], "HelloWorld_1_1");
    StringHelper::Copy(&input[0][1][0], "HelloWorld_1_2");
    StringHelper::Copy(&input[0][2][0], "HelloWorld_1_3");
    StringHelper::Copy(&input[1][0][0], "HelloWorld_2_1");
    StringHelper::Copy(&input[1][1][0], "HelloWorld_2_2");
    StringHelper::Copy(&input[1][2][0], "HelloWorld_2_3");

    char8 ***matrix = (char8***) HeapManager::Malloc(2 * sizeof(char**));

    for (uint32 i = 0; i < nRows; i++) {
        matrix[i] = (char8**) HeapManager::Malloc(nCols * sizeof(char**));
        for (uint32 j = 0; j < nCols; j++) {
            matrix[i][j] = (char8*) HeapManager::Malloc(buffSize * sizeof(char));
        }
    }

    Matrix<char8*> output(matrix, 2, 3);

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (StringHelper::Compare(&input[i][j][0], output[i][j]) != 0) {
                ret = false;
            }
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            HeapManager::Free((void*&) matrix[i][j]);
        }
        HeapManager::Free((void*&) matrix[i]);
    }

    HeapManager::Free((void*&) matrix);

    return ret;

}


bool TypeConversionTest::TestIntegerToCArrayScalar_Trunc(){
    bool ret = true;
    char8 output[5];

    int32 input=12345;

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(output, "1234") != 0) {
        ret = false;
    }

    return ret;
}

bool TypeConversionTest::TestFloatToCArrayScalar_Trunc(){
    bool ret = true;
    char8 output[5];

    float32 input=1.0;

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(output, "1.00") != 0) {
        ret = false;
    }

    return ret;
}


bool TypeConversionTest::TestCCStringToCArrayScalar_Trunc() {

    bool ret = true;
    char8 output[5];

    const char8* input = "HelloWorld";

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(output, "Hell") != 0) {
        ret = false;
    }

    return ret;
}

bool TypeConversionTest::TestCCStringToCArrayVector_Trunc() {
    bool ret = true;
    char8 output[2][6];

    const char8* input[] = { "HelloWorld", "12345" };

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(&output[0][0], "Hello") != 0) {
        ret = false;
    }

    if (StringHelper::Compare(&output[1][0], "12345") != 0) {
        ret = false;
    }

    return ret;

}

bool TypeConversionTest::TestCCStringToCArrayMatrix_Trunc() {
    bool ret = true;
    char8 output[2][3][6];

    const char8* input[][3] = { { "HelloWorld", "12345", "a" }, { "abc", "abcdefghilmnopq", "**********" } };

    if (!TypeConvert(output, input)) {
        ret = false;
    }

    if (StringHelper::Compare(&output[0][0][0], "Hello") != 0) {
        ret = false;
    }

    if (StringHelper::Compare(&output[0][1][0], "12345") != 0) {
        ret = false;
    }
    if (StringHelper::Compare(&output[0][2][0], "a") != 0) {
        ret = false;
    }

    if (StringHelper::Compare(&output[1][0][0], "abc") != 0) {
        ret = false;
    }

    if (StringHelper::Compare(&output[1][1][0], "abcde") != 0) {
        ret = false;
    }
    if (StringHelper::Compare(&output[1][2][0], "*****") != 0) {
        ret = false;
    }

    return ret;

}
