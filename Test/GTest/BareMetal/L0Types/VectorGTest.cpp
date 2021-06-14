/**
 * @file VectorGTest.cpp
 * @brief Source file for class VectorGTest
 * @date 17/11/2015
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
 * the class VectorGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "VectorTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L0Types_VectorGTest,TestDefaultConstructor) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestDefaultConstructor());
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorCreateOnHeap) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestConstructorCreateOnHeap(32));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_Int8) {
    VectorTest vectorTest;

    int8 array[] = { -1, 2, -3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_UInt8) {
    VectorTest vectorTest;

    uint8 array[] = { 1, 2, 3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_Int16) {
    VectorTest vectorTest;

    int16 array[] = { -1, 2, -3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_UInt16) {
    VectorTest vectorTest;

    uint16 array[] = { 1, 2, 3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_Int32) {
    VectorTest vectorTest;

    int32 array[] = { 1, -2, 3, -4 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_UInt32) {
    VectorTest vectorTest;

    uint32 array[] = { 1, 2, 3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_Int64) {
    VectorTest vectorTest;

    int64 array[] = { 1, -2, 3, -4 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_UInt64) {
    VectorTest vectorTest;

    uint64 array[] = { 1, 2, 3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

/*
TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_Float32) {
    VectorTest vectorTest;

    float32 array[] = { 111.5, -902.5E+2, 300.9, -0.004 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}
*/

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_Float64) {
    VectorTest vectorTest;

    float64 array[] = { 111.5, -902.5E+2, 300.9, -0.004 };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerStatic_CCString) {
    VectorTest vectorTest;

    const char8 *array[] = { "Hello", "abcdefghilmnopq", "Hola", "1234567890" };
    ASSERT_TRUE(vectorTest.TestConstructorByPointerStatic(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_Int8) {
    VectorTest vectorTest;

    uint32 size = 4;
    int8 *array = (int8*) HeapManager::Malloc(sizeof(int8) * size);

    for (uint32 i = 0; i < size; i++) {
        array[i] = -i;
    }
    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_UInt8) {
    VectorTest vectorTest;

    uint32 size = 4;
    uint8 *array = (uint8*) HeapManager::Malloc(sizeof(uint8) * size);

    for (uint32 i = 0; i < size; i++) {
        array[i] = i;
    }
    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_Int16) {
    VectorTest vectorTest;

    uint32 size = 4;
    int16 *array = (int16*) HeapManager::Malloc(sizeof(int16) * size);

    for (uint32 i = 0; i < size; i++) {
        array[i] = -i;
    }
    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_UInt16) {
    VectorTest vectorTest;

    uint32 size = 4;
    uint16 *array = (uint16*) HeapManager::Malloc(sizeof(uint16) * size);

    for (uint32 i = 0; i < size; i++) {
        array[i] = i;
    }
    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_Int32) {
    VectorTest vectorTest;

    uint32 size = 4;
    int32 *array = (int32*) HeapManager::Malloc(sizeof(int32) * size);

    for (uint32 i = 0; i < size; i++) {
        array[i] = -i;
    }
    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_UInt32) {
    VectorTest vectorTest;

    uint32 size = 4;
    uint32 *array = (uint32*) HeapManager::Malloc(sizeof(uint32) * size);

    for (uint32 i = 0; i < size; i++) {
        array[i] = i;
    }
    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_Int64) {
    VectorTest vectorTest;

    uint32 size = 4;
    int64 *array = (int64*) HeapManager::Malloc(sizeof(int64) * size);

    for (uint32 i = 0; i < size; i++) {
        array[i] = -i;
    }
    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_UInt64) {
    VectorTest vectorTest;

    uint32 size = 4;
    uint64 *array = (uint64*) HeapManager::Malloc(sizeof(uint64) * size);

    for (uint32 i = 0; i < size; i++) {
        array[i] = i;
    }
    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_Float32) {
    VectorTest vectorTest;

    uint32 size = 4;
    float32 *array = (float32*) HeapManager::Malloc(sizeof(float32) * size);

    array[0] = 100.001E+3;
    array[1] = 12345.12345;
    array[2] = 0.0001E-3;
    array[3] = 9999.9;

    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByPointerHeap_Float64) {
    VectorTest vectorTest;

    uint32 size = 4;
    float64 *array = (float64*) HeapManager::Malloc(sizeof(float64) * size);

    array[0] = 100.001E+3;
    array[1] = 12345.12345;
    array[2] = 0.0001E-3;
    array[3] = 9999.9;

    ASSERT_TRUE(vectorTest.TestConstructorByPointerHeap(array, size));

    HeapManager::Free((void*&) array);
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_Int8) {
    VectorTest vectorTest;

    int8 array[] = { -1, 2, -3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_UInt8) {
    VectorTest vectorTest;

    uint8 array[] = { 1, 2, 3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_Int16) {
    VectorTest vectorTest;

    int16 array[] = { -1, 2, -3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_UInt16) {
    VectorTest vectorTest;

    uint16 array[] = { 1, 2, 3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_Int32) {
    VectorTest vectorTest;

    int32 array[] = { 1, -2, 3, -4 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_UInt32) {
    VectorTest vectorTest;

    uint32 array[] = { 1, 2, 3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_Int64) {
    VectorTest vectorTest;

    int64 array[] = { 1, -2, 3, -4 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_UInt64) {
    VectorTest vectorTest;

    uint64 array[] = { 1, 2, 3, 4 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

/*
TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_Float32) {
    VectorTest vectorTest;

    float32 array[] = { 111.5, -902.5E+2, 300.9, -0.004 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}
*/
TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_Float64) {
    VectorTest vectorTest;

    float64 array[] = { 111.5, -902.5E+2, 300.9, -0.004 };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestConstructorByTable_CCString) {
    VectorTest vectorTest;

    const char8 *array[] = { "Hello", "abcdefghilmnopq", "Hola", "1234567890" };
    ASSERT_TRUE(vectorTest.TestConstructorByTable(array));
}

TEST(BareMetal_L0Types_VectorGTest,TestGetNumberOfElements) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestGetNumberOfElements());
}

TEST(BareMetal_L0Types_VectorGTest,TestGetDataPointer) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestGetDataPointer());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorOperator_Static) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorOperator_Static());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorOperator_Heap) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorOperator_Heap());
}

TEST(BareMetal_L0Types_VectorGTest,TestProduct) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestProduct());
}

TEST(BareMetal_L0Types_VectorGTest,TestSetSizeCanDestroy_NotEmptyToNotEmpty) {
    VectorTest vectorTest;
    Vector<int32> vector1(5);

    ASSERT_TRUE(vectorTest.TestSetSize(vector1, 7));
}

TEST(BareMetal_L0Types_VectorGTest,TestSetSizeCanDestroy_NotEmptyToEmpty) {
    VectorTest vectorTest;
    Vector<int32> vector1(5);

    ASSERT_TRUE(vectorTest.TestSetSize(vector1, 0));
}

TEST(BareMetal_L0Types_VectorGTest,TestSetSizeCanDestroy_EmptyToNotEmpty) {
    VectorTest vectorTest;
    Vector<int32> vector1(0);

    ASSERT_TRUE(vectorTest.TestSetSize(vector1, 10));
}

TEST(BareMetal_L0Types_VectorGTest,TestSetSizeCannotDestroy_Heap) {
    VectorTest vectorTest;
    int32 *array = (int32*) HeapManager::Malloc(sizeof(int32) * 21);
    Vector<int32> vector1(array, 21);

    ASSERT_TRUE(vectorTest.TestSetSize(vector1, 30));
    HeapManager::Free((void*&)array);
}

TEST(BareMetal_L0Types_VectorGTest,TestSetSizeCannotDestroy_Static) {
    VectorTest vectorTest;
    int32 array[15];
    Vector<int32> vector1(array);

    ASSERT_TRUE(vectorTest.TestSetSize(vector1, 5));
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_uint8) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<uint8>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_uint16) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<uint16>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_uint32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<uint32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_uint64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<uint64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_int8) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<int8>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_int16) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<int16>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_int32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<int32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_int64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<int64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_float32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<float32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructor_float64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructor<float64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_uint8) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<uint8>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_uint16) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<uint16>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_uint32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<uint32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_uint64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<uint64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_int8) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<int8>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_int16) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<int16>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_int32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<int32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_int64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<int64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_float32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<float32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignment_float64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignment<float64>());
}


TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunctiont_uint8) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<uint8>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_uint16) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<uint16>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_uint32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<uint32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_uint64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<uint64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_int8) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<int8>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_int16) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<int16>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_int32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<int32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_int64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<int64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_float32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<float32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorPassedToAFunction_float64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorPassedToAFunction<float64>());
}





TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_uint8) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<uint8>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_uint16) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<uint16>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_uint32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<uint32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunctionn_uint64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<uint64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_int8) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<int8>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_int16) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<int16>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_int32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<int32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_int64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<int64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_float32) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<float32>());
}

TEST(BareMetal_L0Types_VectorGTest,TestVectorReturnedFromAFunction_float64) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestVectorReturnedFromAFunction<float64>());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyConstructorNULLPointer) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyConstructorNULLPointer());
}

TEST(BareMetal_L0Types_VectorGTest,TestCopyAssignmentNULLPointer) {
    VectorTest vectorTest;
    ASSERT_TRUE(vectorTest.TestCopyAssignmentNULLPointer());
}
