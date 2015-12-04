/**
 * @file VectorTest.h
 * @brief Header file for class VectorTest
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

 * @details This header file contains the declaration of the class VectorTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef VECTORTEST_H_
#define VECTORTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Vector.h"
#include "MemoryOperationsHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the Vector functions.
 */
class VectorTest {
public:

    /**
     * @brief Tests if the default constructor sets the internal pointer to NULL and numberOfElements == 0.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the vector is allocated on heap.
     */
    bool TestConstructorCreateOnHeap(uint32 nElements);

    /**
     * @brief Tests the constructor passing a pointer to a static array. The
     * Vector class consider the memory as allocated on heap anyway.
     */
    template<typename T, uint32 nElements>
    bool TestConstructorByPointerStatic(T (&array)[nElements]);

    /**
     * @brief Tests the constructor passing a pointer to an heap array.
     */
    template<typename T>
    bool TestConstructorByPointerHeap(T* array,
                                      uint32 nElements);

    /**
     * @brief Tests if passing a static array the Vector class consider the memory as static allocated.
     */
    template<typename T, uint32 nElements>
    bool TestConstructorByTable(T (&array)[nElements]);

    /**
     * @brief Tests if the function returns the correct number of elements in the vector.
     */
    bool TestGetNumberOfElements();

    /**
     * @brief Tests if the function returns correctly the pointer to the vector memory.
     */
    bool TestGetDataPointer();

    /**
     * @brief Tests the operator[] on a static vector.
     */
    bool TestVectorOperator_Static();

    /**
     * @brief Tests the operator[] on a heap vector.
     */
    bool TestVectorOperator_Heap();

    /**
     * @brief Tests if the function calculates correctly the scalar product between two vectors.
     */
    bool TestProduct();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T, uint32 nElements>
bool VectorTest::TestConstructorByPointerStatic(T (&array)[nElements]) {

    T* pointer = array;
    Vector<T> myVector(pointer, nElements);

    for (uint32 i = 0; i < nElements; i++) {
        if (myVector[i] != array[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
bool VectorTest::TestConstructorByPointerHeap(T* array,
                                              uint32 nElements) {

    Vector<T> myVector(array, nElements);

    for (uint32 i = 0; i < nElements; i++) {
        if (MemoryOperationsHelper::Compare(&myVector[i], &array[i], sizeof(T))) {
            return false;
        }
    }

    return true;
}

template<typename T, uint32 nElements>
bool VectorTest::TestConstructorByTable(T (&array)[nElements]) {

    Vector<T> myVector(array);

    for (uint32 i = 0; i < nElements; i++) {
        if (myVector[i] != array[i]) {
            return false;
        }
    }

    return true;

}

#endif /* VECTORTEST_H_ */

