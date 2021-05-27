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
    bool TestConstructorByPointerHeap(T *array,
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

    /**
     * @brief Tests if SetSize function allocates a new dataPointer and changes the number of elements.
     */
    bool TestSetSize(Vector<int32> &vector1,
                     uint32 newSize);

    /**
     * @brief Tests copy constructor for all basic types.
     */
    template<class T>
    bool TestCopyConstructor();

    /**
     * @brief Tests copy assignment for all basic types.
     */
    template<class T>
    bool TestCopyAssignment();

    /**
     * @brief Tests copy constructor passing a vector to a functions.
     */
    template<class T>
    bool TestVectorPassedToAFunction();

    /**
     * @brief Tests copy assignment passing returning a vector from a function.
     */
    template<class T>
    bool TestVectorReturnedFromAFunction();

    /**
     * Test copy constructor using a vector with the pointer not initialised
     */
    bool TestCopyConstructorNULLPointer();
    /**
     * Test copy assignment using a vector with the pointer not initialised.
     */
    bool TestCopyAssignmentNULLPointer();

private:
    /**
     * @brief compare a vector with the expected value
     * @return true if the values of the vector are as expected.
     */
    template<typename T>
    bool AuxiliaryFunctionIn(Vector<T> vec);

    /**
     * @brief Return an initialised vector.
     * @brief Creates a vector, initialised its value with a sequential number based on its index, and then return the vector.
     * @return return a vector statically initialised.
     */
    template<typename T>
    Vector<T> AuxiliaryFunctionOut(uint32 nOfElements);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T, uint32 nElements>
bool VectorTest::TestConstructorByPointerStatic(T (&array)[nElements]) {

    T *pointer = array;
    Vector<T> myVector(pointer, nElements);

    for (uint32 i = 0; i < nElements; i++) {
        if (myVector[i] != array[i]) {
            return false;
        }
    }

    return true;
}

template<typename T>
bool VectorTest::TestConstructorByPointerHeap(T *array,
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

template<class T>
bool VectorTest::TestCopyConstructor() {
    uint32 nOfElements = 10u;
    Vector<T> originalVector(nOfElements);
    for (uint32 i = 0u; i < nOfElements; i++) {
        originalVector[i] = i;
    }
    Vector<T> copiedVector(originalVector);
    bool ret = copiedVector.GetDataPointer() != originalVector.GetDataPointer();
    if (ret) {
        ret = copiedVector.GetNumberOfElements() == originalVector.GetNumberOfElements();
    }
    for (uint32 i = 0u; (i < nOfElements) && ret; i++) {
        ret = copiedVector[i] == static_cast<T>(i);
    }
    return ret;
}

template<class T>
bool VectorTest::TestCopyAssignment() {
    uint32 nOfElements = 10u;
    Vector<T> originalVector(nOfElements);
    for (uint32 i = 0u; i < nOfElements; i++) {
        originalVector[i] = i;
    }
    Vector<T> copiedVector(3);
    copiedVector = originalVector;
    bool ret = copiedVector.GetDataPointer() != originalVector.GetDataPointer();
    if (ret) {
        ret = copiedVector.GetNumberOfElements() == originalVector.GetNumberOfElements();
    }
    for (uint32 i = 0u; (i < nOfElements) && ret; i++) {
        ret = copiedVector[i] == static_cast<T>(i);
    }
    return ret;
}

template<class T>
bool VectorTest::TestVectorPassedToAFunction() {
    uint32 nOfElements = 10u;
    Vector<T> originalVector(nOfElements);
    for (uint32 i = 0u; i < nOfElements; i++) {
        originalVector[i] = static_cast<T>(i);
    }
    return AuxiliaryFunctionIn(originalVector);
}

template<class T>
bool VectorTest::TestVectorReturnedFromAFunction() {
    uint32 nOfElements = 10u;
    Vector<T> vec(3);
    vec = AuxiliaryFunctionOut<T>(nOfElements);
    bool ret = true;
    for (uint32 i = 0u; i < nOfElements; i++) {
        ret = vec[i] == static_cast<T>(i);
    }
    return ret;
}

template<typename T>
bool VectorTest::AuxiliaryFunctionIn(Vector<T> vec) {
    bool ret = true;
    for (uint32 i = 0u; (i < vec.GetNumberOfElements()) && ret; i++) {
        ret = vec[i] == static_cast<T>(i);
    }
    return ret;
}

template<typename T>
Vector<T> VectorTest::AuxiliaryFunctionOut(uint32 nOfElements) {
    Vector<T> vec(nOfElements);
    for (uint32 i = 0u; (i < vec.GetNumberOfElements()); i++) {
        vec[i] = i;
    }
    return vec;
}

#endif /* VECTORTEST_H_ */

