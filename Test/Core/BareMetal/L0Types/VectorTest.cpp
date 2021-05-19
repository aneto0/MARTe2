/**
 * @file VectorTest.cpp
 * @brief Source file for class VectorTest
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
 * the class VectorTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "VectorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

bool VectorTest::TestDefaultConstructor() {

    Vector<int32> myVector;

    if (myVector.GetNumberOfElements() != 0) {
        return false;
    }

    if (myVector.GetDataPointer() != NULL) {
        return false;
    }

    return true;
}

bool VectorTest::TestConstructorCreateOnHeap(uint32 nElements) {
    Vector<int32> myVector(nElements);

    if (myVector.GetNumberOfElements() != nElements) {
        return false;
    }

    if (myVector.GetDataPointer() == NULL) {
        return false;
    }

    return true;
}

bool VectorTest::TestGetNumberOfElements() {

    const uint32 nElements = 32;
    bool ret = true;
    int32 staticArray[nElements];

    Vector<int32> vector1(staticArray);

    if (vector1.GetNumberOfElements() != nElements) {
        ret = false;
    }

    int32 *staticPointer = staticArray;
    Vector<int32> vector2(staticPointer, nElements);

    if (vector2.GetNumberOfElements() != nElements) {
        ret = false;
    }

    int32 *heapPointer = (int32*) HeapManager::Malloc(sizeof(int32) * nElements);

    Vector<int32> vector3(heapPointer, nElements);

    if (vector3.GetNumberOfElements() != nElements) {
        ret = false;
    }

    Vector<int32> vector4(nElements);

    ret &= (vector4.GetNumberOfElements() == nElements);
    HeapManager::Free((void*&) heapPointer);
    return ret;
}

bool VectorTest::TestGetDataPointer() {
    const uint32 nElements = 32;
    bool ret = true;
    int32 staticArray[nElements];

    Vector<int32> vector1(staticArray);

    if (vector1.GetDataPointer() != staticArray) {
        ret = false;
    }

    int32 *staticPointer = staticArray;
    Vector<int32> vector2(staticPointer, nElements);

    if (vector2.GetDataPointer() != staticPointer) {
        ret = false;
    }

    int32 *heapPointer = (int32*) HeapManager::Malloc(sizeof(int32) * nElements);

    Vector<int32> vector3(heapPointer, nElements);

    if (vector3.GetDataPointer() != heapPointer) {
        ret = false;
    }

    Vector<int32> vector4(nElements);

    ret &= (vector4.GetDataPointer() != NULL);
    HeapManager::Free((void*&) heapPointer);
    return ret;
}

bool VectorTest::TestVectorOperator_Static() {

    const uint32 nElements = 32;
    int32 array[nElements];

    for (uint32 i = 0; i < nElements; i++) {
        array[i] = i;
    }

    Vector<int32> vector1(array);

    for (uint32 i = 0; i < nElements; i++) {
        if (array[i] != vector1[i]) {
            return false;
        }
    }
    return true;
}

bool VectorTest::TestVectorOperator_Heap() {

    const uint32 nElements = 32;
    int32 *array = (int32*) HeapManager::Malloc(sizeof(int32) * nElements);

    for (uint32 i = 0; i < nElements; i++) {
        array[i] = i;
    }

    Vector<int32> vector1(array, nElements);

    for (uint32 i = 0; i < nElements; i++) {
        if (array[i] != vector1[i]) {
            HeapManager::Free((void*&) array);
            return false;
        }
    }
    HeapManager::Free((void*&) array);
    return true;
}

bool VectorTest::TestProduct() {

    int32 factor1[] = { 1, 2, 3 };
    int32 factor2[] = { 4, 5, -1 };

    Vector<int32> vector1(factor1);
    Vector<int32> vector2(factor2);

    int32 result = 0;

    vector1.Product(vector2, result);
    return result == 11;
}

bool VectorTest::TestSetSize(Vector<int32> &vector1,
                             uint32 newSize) {
    bool ok = true;

    vector1.SetSize(newSize);

    ok &= (vector1.GetNumberOfElements() == newSize);
    if (newSize != 0) {
        ok &= (vector1.GetDataPointer() != NULL_PTR(int32*));
    }
    else {
        ok &= (vector1.GetDataPointer() == NULL_PTR(int32*));
    }

    return ok;
}

bool VectorTest::TestCopyConstructorNULLPointer() {
    Vector<uint32> vec1;
    Vector<uint32> vec2(vec1);
    bool ret = vec2.GetDataPointer() == NULL_PTR(uint32*);
    if (ret) {
        ret = vec2.GetNumberOfElements() == 0u;
    }
    return ret;
}

bool VectorTest::TestCopyAssignmentNULLPointer() {
    Vector<uint32> vec1;
    Vector<uint32> vec2;
    vec2 = vec1;
    bool ret = vec2.GetDataPointer() == NULL_PTR(uint32*);
    if (ret) {
        ret = vec2.GetNumberOfElements() == 0u;
    }
    return ret;
}

