/**
 * @file AnyObjectTest.h
 * @brief Header file for class AnyObjectTest
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

 * @details This header file contains the declaration of the class AnyObjectTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ANYOBJECTTEST_H_
#define ANYOBJECTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyObject.h"
#include "Vector.h"
#include "Matrix.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the AnyObject functions.
 */
class AnyObjectTest {
public:
    /**
     * @brief Tests the default constructor and verifies that GetType().GetDataPointer() == NULL
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the Serialise function with a scalar
     */
    template<typename T>
    bool TestSerialise_Scalar(T value);

    /**
     * @brief Tests the Serialise function with a Vector of type T
     */
    template<typename T>
    bool TestSerialise_Vector(T value);

    /**
     * @brief Tests the Serialise function with a statically defined Vector of type T
     */
    template<typename T>
    bool TestSerialise_Vector_Static(T value);

    /**
     * @brief Tests the GetType function with a scalar
     */
    template<typename T>
    bool TestGetType_Scalar(T value);

    /**
     * @brief Tests the GetType function with a StreamString
     */
    bool TestGetType_Scalar_String();

    /**
     * @brief Tests the GetType function with a Vector of type T
     */
    template<typename T>
    bool TestGetType_Vector(T value);

    /**
     * @brief Tests the Serialise function with a char *
     */
    bool TestSerialise_ScalarCharStar();

    /**
     * @brief Tests the Serialise function with a Vector<char>
     */
    bool TestSerialise_VectorChar();

    /**
     * @brief Tests the Serialise function with a char[]
     */
    bool TestSerialise_StaticCharArray();

    /**
     * @brief Tests the Serialise function with a char*[]
     */
    bool TestSerialise_StaticArrayCharStar();

    /**
     * @brief Tests the Serialise function with a char[][].
     */
    bool TestSerialise_StaticMatrixChar();

    /**
     * @brief Tests the Serialise function with a char[][][].
     */
    bool TestSerialise_StaticMeshChar();

    /**
     * @brief Tests the Serialise function with a StreamString
     */
    bool TestSerialise_ScalarString();

    /**
     * @brief Tests the Serialise function with a static array of StreamString
     */
    bool TestSerialise_StaticStringArray();

    /**
     * @brief Tests the Serialise function with a Vector<StreamString>
     */
    bool TestSerialise_VectorString();

    /**
     * @brief Tests the Serialise function with a Matrix of type T
     */
    template<typename T>
    bool TestSerialise_Matrix(T value);

    /**
     * @brief Tests the Serialise function with a statically defined Matrix of type T
     */
    template<typename T>
    bool TestSerialise_Matrix_Static(T value);

    /**
     * @brief Tests the Serialise function with a statically defined array of T**
     */
    template<typename T>
    bool TestSerialise_Matrix_Array_Static(T value);

    /**
     * @brief Tests the GetType function with a Matrix of type T
     */
    template<typename T>
    bool TestGetType_Matrix(T value);

    /**
     * @brief Tests the Serialise function with a statically defined tensor of type T
     */
    template<typename T>
    bool TestSerialise_Tensor_AnyType_Static(T value);

    /**
     * @brief Tests the GetType function with a tensor of type T
     */
    template<typename T>
    bool TestGetType_Tensor(T value);

    /**
     * @brief Tests the Serialise function with a char*[][]
     */
    bool TestSerialise_StaticMatrixCharStar();

    /**
     * @brief Tests the Serialise function with a Matrix<char *>
     */
    bool TestSerialise_MatrixCharStar();

    /**
     * @brief Tests the Serialise function with a StreamString[][]
     */
    bool TestSerialise_StaticMatrixString();

    /**
     * @brief Tests the Serialise function with a Matrix<StreamString>
     */
    bool TestSerialise_MatrixString();

    /**
     * @brief Tests the Serialise function with a uint32[][]
     */
    bool TestSerialise_StaticTensor();

    /**
     * @brief Tests the GetClassProperties (from Object)
     */
    bool TestGetClassProperties();

//    /**
//     * @brief Tests the GetClassPropertiesCopy (from Object)
//     */
//    bool TestGetClassPropertiesCopy();

    /**
     * @brief Tests the TestGetObjectBuildFunction (from Object)
     */
    bool TestGetObjectBuildFunction();


    /**
     * @brief Tests the CleanUp function.
     */
    bool TestCleanUp();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
bool AnyObjectTest::TestSerialise_Scalar(T value) {
    MARTe::AnyObject anyObj;
    bool ok = anyObj.Serialise(value);
    MARTe::AnyType testType(value);
    ok = (anyObj.GetType().GetTypeDescriptor() == testType.GetTypeDescriptor());
    T sourceValue = *static_cast<T *>(anyObj.GetType().GetDataPointer());
    ok = (sourceValue == value);
    return ok;
}

template<typename T>
bool AnyObjectTest::TestGetType_Scalar(T value) {
    MARTe::AnyObject anyObj;
    bool ok = anyObj.Serialise(value);
    MARTe::AnyType testType(value);
    ok = (anyObj.GetType().GetTypeDescriptor() == testType.GetTypeDescriptor());
    return ok;
}

template<typename T>
bool AnyObjectTest::TestSerialise_Vector(T value) {
    MARTe::uint32 nOfElements = 127;
    MARTe::AnyObject anyObj;
    MARTe::Vector<T> sourceVec(nOfElements);
    MARTe::uint32 i;
    for (i = 0; i < nOfElements; i++) {
        sourceVec[i] = i;
    }

    bool ok = anyObj.Serialise(sourceVec);
    MARTe::Vector<T> testVec(static_cast<T *>(anyObj.GetType().GetDataPointer()), nOfElements);
    for (i = 0; ok && (i < nOfElements); i++) {
        ok = (sourceVec[i] == testVec[i]);
    }
    return ok;
}

template<typename T>
bool AnyObjectTest::TestSerialise_Vector_Static(T value) {
    MARTe::uint32 nOfElements = 8;
    T arr[] = { 0, 1, 2, 3, 4, 3, 2, 1 };
    MARTe::AnyObject anyObj;
    MARTe::Vector<T> sourceVec(arr);

    bool ok = anyObj.Serialise(sourceVec);
    MARTe::Vector<T> testVec(static_cast<T *>(anyObj.GetType().GetDataPointer()), nOfElements);

    MARTe::uint32 i;
    for (i = 0; ok && (i < nOfElements); i++) {
        ok = (sourceVec[i] == testVec[i]);
    }
    return ok;
}

template<typename T>
bool AnyObjectTest::TestGetType_Vector(T value) {
    MARTe::uint32 nOfElements = 128;
    MARTe::AnyObject anyObj;
    MARTe::Vector<T> sourceVec(nOfElements);

    bool ok = anyObj.Serialise(sourceVec);
    MARTe::Vector<T> testVec(static_cast<T *>(anyObj.GetType().GetDataPointer()), nOfElements);
    MARTe::AnyType testType(testVec);
    ok = (anyObj.GetType().GetTypeDescriptor() == testType.GetTypeDescriptor());

    return ok;
}

template<typename T>
bool AnyObjectTest::TestSerialise_Matrix(T value) {
    MARTe::uint32 nOfRows = 10;
    MARTe::uint32 nOfColumns = 12;
    MARTe::AnyObject anyObj;
    MARTe::Matrix<T> sourceMat(nOfRows, nOfColumns);
    MARTe::uint32 i;
    MARTe::uint32 j;
    for (i = 0; i < nOfRows; i++) {
        for (j = 0; j < nOfColumns; j++) {
            sourceMat[i][j] = i * j;
        }
    }

    bool ok = anyObj.Serialise(sourceMat);
    MARTe::Matrix<T> testMat(static_cast<T **>(anyObj.GetType().GetDataPointer()), nOfRows, nOfColumns);
    for (i = 0; i < nOfRows; i++) {
        for (j = 0; j < nOfColumns; j++) {
            ok = (sourceMat[i][j] == testMat[i][j]);
        }
    }
    return ok;
}

template<typename T>
bool AnyObjectTest::TestSerialise_Matrix_Static(T value) {
    MARTe::uint32 nOfRows = 2;
    MARTe::uint32 nOfColumns = 3;
    T arr[][3] = { { 1, 2, 3 }, { 6, 5, 4 } };

    MARTe::AnyObject anyObj;
    MARTe::Matrix<T> sourceMat(arr);

    bool ok = anyObj.Serialise(sourceMat);
    MARTe::uint32 i;
    MARTe::uint32 j;
    for (i = 0; i < nOfRows; i++) {
        T* beginMemory = reinterpret_cast<T*>(anyObj.GetType().GetDataPointer());
        MARTe::Vector<T> vec = MARTe::Vector<T>(&beginMemory[i * nOfColumns], nOfColumns);
        for (j = 0; j < nOfColumns; j++) {
            ok = (sourceMat[i][j] == vec[j]);
        }
    }
    return ok;
}

template<typename T>
bool AnyObjectTest::TestSerialise_Matrix_Array_Static(T value) {
    MARTe::uint32 nOfRows = 2;
    MARTe::uint32 nOfColumns = 3;
    T arr[][3] = { { 1, 2, 3 }, { 6, 5, 4 } };

    MARTe::AnyObject anyObj;

    bool ok = anyObj.Serialise(arr);
    MARTe::Matrix<T> testMat(static_cast<T **>(anyObj.GetType().GetDataPointer()), nOfRows, nOfColumns);

    MARTe::uint32 i;
    MARTe::uint32 j;
    for (i = 0; i < nOfRows; i++) {
        T* beginMemory = reinterpret_cast<T*>(anyObj.GetType().GetDataPointer());
        MARTe::Vector<T> vec = MARTe::Vector<T>(&beginMemory[i * nOfColumns], nOfColumns);
        for (j = 0; j < nOfColumns; j++) {
            ok = (arr[i][j] == vec[j]);
        }
    }
    return ok;
}

template<typename T>
bool AnyObjectTest::TestGetType_Matrix(T value) {
    MARTe::uint32 nOfRows = 33;
    MARTe::uint32 nOfColumns = 2;
    MARTe::AnyObject anyObj;
    MARTe::Matrix<T> sourceMat(nOfRows, nOfColumns);

    bool ok = anyObj.Serialise(sourceMat);
    MARTe::Matrix<T> testMat(static_cast<T **>(anyObj.GetType().GetDataPointer()), nOfRows, nOfColumns);
    MARTe::AnyType testType(testMat);
    ok = (anyObj.GetType().GetTypeDescriptor() == testType.GetTypeDescriptor());

    return ok;
}

template<typename T>
bool AnyObjectTest::TestSerialise_Tensor_AnyType_Static(T value) {
    MARTe::uint32 nOfRows = 3;
    MARTe::uint32 nOfColumns = 2;
    MARTe::uint32 nOfPages = 2;
    T arr[][3][2] = {
                    { { 1, 2 },  {3, 4 }, {5,  6 } },
                    { { 7, 8 },  {9, 10}, {11, 12} },
                    };

    MARTe::AnyObject anyObj;
    MARTe::TypeDescriptor type = MARTe::Type2TypeDescriptor<T>();
    MARTe::AnyType at(type, 0u, arr);
    at.SetNumberOfDimensions(3u);
    at.SetNumberOfElements(0u, nOfRows);
    at.SetNumberOfElements(1u, nOfColumns);
    at.SetNumberOfElements(2u, nOfPages);

    bool ok = anyObj.Serialise(at);
    T* testTensor = reinterpret_cast<T*>(anyObj.GetType().GetDataPointer());
    
    MARTe::uint32 i;
    MARTe::uint32 j;
    MARTe::uint32 k;
    for (k = 0; k < nOfPages; k++) {
        for (i = 0; i < nOfRows; i++) {
            for (j = 0; j < nOfColumns; j++) {
                ok = (arr[k][i][j] == *( testTensor + (j + nOfColumns*(i + nOfRows*k)) ) );
            }
        }
    }
    return ok;
}

template<typename T>
bool AnyObjectTest::TestGetType_Tensor(T value) {
    MARTe::uint32 nOfRows = 3;
    MARTe::uint32 nOfColumns = 2;
    MARTe::uint32 nOfPages = 2;
    T arr[][3][2] = {
                    { { 1, 2 },  {3, 4 }, {5,  6 } },
                    { { 7, 8 },  {9, 10}, {11, 12} },
                    };

    MARTe::AnyObject anyObj;
    MARTe::TypeDescriptor type = MARTe::Type2TypeDescriptor<T>();
    MARTe::AnyType at(type, 0u, arr);
    at.SetNumberOfDimensions(3u);
    at.SetNumberOfElements(0u, nOfRows);
    at.SetNumberOfElements(1u, nOfColumns);
    at.SetNumberOfElements(2u, nOfPages);

    bool ok = anyObj.Serialise(at);
    
    ok = (anyObj.GetType().GetTypeDescriptor() == MARTe::Type2TypeDescriptor<T>());

    return ok;
}

#endif /* ANYOBJECTTEST_H_ */

