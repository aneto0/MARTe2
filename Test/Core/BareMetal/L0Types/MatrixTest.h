/**
 * @file MatrixTest.h
 * @brief Header file for class MatrixTest
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

 * @details This header file contains the declaration of the class MatrixTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATRIXTEST_H_
#define MATRIXTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Matrix.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the Matrix functions.
 */
class MatrixTest {

public:

    /**
     * @brief Tests if the pointer is set to NULL and dimensions are set to 0.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the memory for the matrix is allocated on heap.
     */
    bool TestConstructorOnHeap();

    /**
     * @brief Tests the constructor passing a double pointer which represents a matrix on heap.
     */
    template<typename T>
    bool TestConstructorByPointerHeap(T **matrix,
                                      uint32 nRows,
                                      uint32 nCols);

    /**
     * @brief Tests the constructor passing a pointer to a static memory.
     */
    template<typename T, typename T2>
    bool TestConstructorByPointerStatic(T2 matrix,
                                        Matrix<T> &matrix2,
                                        uint32 nRows,
                                        uint32 nCols);

    /**
     * @brief Tests the constructor passing directly the static matrix.
     */
    template<typename T, uint32 nRows, uint32 nCols>
    bool TestConstructorByTable(T (&matrix)[nRows][nCols]);

    /**
     * @brief Tests the copy constructor from a matrix created by passing a double pointer which represents a matrix on heap.
     */
    template<typename T>
    bool TestCopyConstructorByPointerHeap(T **matrix,
                                          uint32 nRows,
                                          uint32 nCols);

    /**
     * @brief Tests the copy constructor from a matrix created by passing a pointer to a static memory.
     */
    template<typename T, typename T2>
    bool TestCopyConstructorByPointerStatic(T2 matrix,
                                            Matrix<T> &originalMatrix,
                                            uint32 nRows,
                                            uint32 nCols);

    /**
     * @brief Tests the copy constructor from a matrix created by passing directly the static matrix.
     */
    template<typename T, uint32 nRows, uint32 nCols>
    bool TestCopyConstructorByTable(T (&matrix)[nRows][nCols]);

    /**
     * @brief Tests the copy constructor from a matrix created by passing directly a matrix which manages its own memory and is not static declared
     */
    template<typename T>
    bool TestCopyConstructorByCanDestroyNonStaticMatrix();

    /**
     * @brief Tests the copy assignment from a matrix created by passing a double pointer which represents a matrix on heap.
     */
    template<typename T>
    bool TestCopyAssignmentByPointerHeap(T **matrix,
                                         uint32 nRows,
                                         uint32 nCols);

    /**
     * @brief Tests the copy assignment from a matrix created by passing a pointer to a static memory.
     */
    template<typename T, typename T2>
    bool TestCopyAssignmentByPointerStatic(T2 matrix,
                                           Matrix<T> &originalMatrix,
                                           uint32 nRows,
                                           uint32 nCols);

    /**
     * @brief Tests the copy assignment from a matrix created by passing directly the static matrix.
     */
    template<typename T, uint32 nRows, uint32 nCols>
    bool TestCopyAssignmentByTable(T (&matrix)[nRows][nCols]);

    /**
     * @brief Tests the copy assignment from a matrix created with canDestroy = true & IsStatic = false.
     */
    template<typename T>
    bool TestCopyAssignmentByCanDestroyNonStaticMatrix();

    /**
     * @brief Tests if the function returns the correct number of columns of the matrix.
     */
    bool TestGetNumberOfColumns(uint32 nCols);

    /**
     * @brief Tests if the function returns the correct number of rows of the matrix.
     */
    bool TestGetNumberOfRows(uint32 nRows);

    /**
     * @brief Tests the [][] operator on a static matrix.
     */
    bool TestMatrixOperator_Static();

    /**
     * @brief Tests the [][] operator on a heap matrix.
     */
    bool TestMatrixOperator_Heap();

    /**
     * @brief Tests the [][] operator on a heap matrix the memory was not continuously allocated.
     */
    bool TestMatrixOperator_HeapDisc();

    /**
     * @brief Tests the (i,j) operator on a static matrix
     */
    bool TestMatrixFunctionCallOperator_Static();

    /**
     * @brief Tests the (i,j) operator on a heap matrix
     */
    bool TestMatrixFunctionCallOperator_Heap();

    /**
     * @brief Tests if the function returns the pointer to the matrix memory.
     */
    bool TestGetDataPointer();

    /**
     * @brief Tests if the function returns true if the matrix is static and false if it is allocated on heap.
     */
    bool TestIsStaticDeclared();

    /**
     * @brief Tests if the product between two static matrices returns the correct result.
     */
    bool TestProduct();

    /**
     * @brief Tests if the sum between two static matrices returns the correct result.
     */
    bool TestSum();

    /**
     * @brief Tests if the copy of a static matrix returns the correct result.
     */
    bool TestCopy();

    /**
     * @brief Tests if the function returns the correct sub matrix requested.
     */
    bool TestSubMatrix();

    /**
     * @brief Tests if the function returns correctly the static matrix determinant.
     */
    bool TestDeterminant();

    /**
     * @brief Tests if the function return the static matrix transpose correctly.
     */
    bool TestTranspose();

    /**
     * @brief Tests if the function calculates correctly the inverse of a float32 static matrix.
     */
    bool TestInverse_float32();

    /**
     * @brief Tests if the function calculates correctly the inverse of a float64 static matrix.
     */
    bool TestInverse_float64();

    /**
     * @brief Tests if the function returns true if the source matrix has determinant == 0 (the inverse cannot be calculated)
     */
    bool TestInverse_NullDeterminant();

    /**
     * @see TestProduct allocating the matrix on heap.
     */
    bool TestProduct_Heap();

    /**
     * @see TestSum allocating the matrix on heap.
     */
    bool TestSum_Heap();

    /**
     * @see TestCopy allocating the matrix on heap.
     */
    bool TestCopy_Heap();

    /**
     * @see TestSubMatrix allocating the matrix on heap.
     */
    bool TestSubMatrix_Heap();

    /**
     * @see TestDeterminant allocating the matrix on heap.
     */
    bool TestDeterminant_Heap();

    /**
     * @see TestTranspose allocating the matrix on heap.
     */
    bool TestTranspose_Heap();

    /**
     * @see TestInverse_float32 allocating the matrix on heap.
     */
    bool TestInverse_float32_Heap();

    /**
     * @see Testinverse_float64 allocating the matrix on heap.
     */
    bool TestInverse_float64_Heap();

    /**
     * @brief copy a matrix which has a NULL pointer
     */
    bool TestCopyConstructorNULLPointer();

    /**
     * @brief copy a matrix which has a NULL pointer
     */
    bool TestCopyAssignmentNULLPointer();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool MatrixTest::TestConstructorByPointerHeap(T **matrix,
                                              uint32 nRows,
                                              uint32 nCols) {

    Matrix<T> myMatrix(matrix, nRows, nCols);

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (myMatrix[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }

    return !myMatrix.IsStaticDeclared();

}

template<typename T, typename T2>
bool MatrixTest::TestConstructorByPointerStatic(T2 matrix,
                                                Matrix<T> &matrix2,
                                                uint32 nRows,
                                                uint32 nCols) {

    //Matrix<T> myMatrix(matrix, nRows, nCols);

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (matrix2[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }

    return matrix2.IsStaticDeclared();

}

template<typename T, uint32 nRows, uint32 nCols>
bool MatrixTest::TestConstructorByTable(T (&matrix)[nRows][nCols]) {

    Matrix<T> myMatrix(matrix);

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (myMatrix[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }
    return myMatrix.IsStaticDeclared();
}

template<typename T>
bool MatrixTest::TestCopyConstructorByPointerHeap(T **matrix,
                                                  uint32 nRows,
                                                  uint32 nCols) {

    Matrix<T> originalMatrix(matrix, nRows, nCols);
    Matrix<T> copiedMatrix(originalMatrix);
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (copiedMatrix[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }
    if (originalMatrix.GetDataPointer() != copiedMatrix.GetDataPointer()) {
        return false;
    }
    return !copiedMatrix.IsStaticDeclared();
}

template<typename T, typename T2>
bool MatrixTest::TestCopyConstructorByPointerStatic(T2 matrix,
                                                    Matrix<T> &originalMatrix,
                                                    uint32 nRows,
                                                    uint32 nCols) {
    Matrix<T> copiedMatrix(originalMatrix);
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (copiedMatrix[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }
    if (originalMatrix.GetDataPointer() != copiedMatrix.GetDataPointer()) {
        return false;
    }
    return copiedMatrix.IsStaticDeclared();
}

template<typename T, uint32 nRows, uint32 nCols>
bool MatrixTest::TestCopyConstructorByTable(T (&matrix)[nRows][nCols]) {
    Matrix<T> originalMatrix(matrix);
    Matrix<T> copiedMatrix(originalMatrix);
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (copiedMatrix[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }
    if (originalMatrix.GetDataPointer() != copiedMatrix.GetDataPointer()) {
        return false;
    }
    return copiedMatrix.IsStaticDeclared();
}

template<typename T>
bool MatrixTest::TestCopyConstructorByCanDestroyNonStaticMatrix() {
    uint32 nRows = 2;
    uint32 nCols = 2;
    Matrix<T> originalMatrix(nRows, nCols);
    for (uint32 row = 0u; row < nRows; row++) {
        for (uint32 col = 0u; col < nCols; col++) {
            originalMatrix[row][col] = row * row + col * 2;
        }
    }
    Matrix<T> copiedMatrix(originalMatrix);
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (copiedMatrix[i][j] != originalMatrix[i][j]) {
                return false;
            }
        }
    }
    if (originalMatrix.GetDataPointer() == copiedMatrix.GetDataPointer()) {
        return false;
    }
    return !copiedMatrix.IsStaticDeclared();
}

template<typename T>
bool MatrixTest::TestCopyAssignmentByPointerHeap(T **matrix,
                                                 uint32 nRows,
                                                 uint32 nCols) {

    Matrix<T> originalMatrix(matrix, nRows, nCols);
    Matrix<T> copiedMatrix;
    copiedMatrix = originalMatrix;
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (copiedMatrix[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }
    if (originalMatrix.GetDataPointer() != copiedMatrix.GetDataPointer()) {
        return false;
    }
    return !copiedMatrix.IsStaticDeclared();
}

template<typename T, typename T2>
bool MatrixTest::TestCopyAssignmentByPointerStatic(T2 matrix,
                                                   Matrix<T> &originalMatrix,
                                                   uint32 nRows,
                                                   uint32 nCols) {
    Matrix<T> copiedMatrix;
    copiedMatrix = originalMatrix;
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (copiedMatrix[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }
    if (originalMatrix.GetDataPointer() != copiedMatrix.GetDataPointer()) {
        return false;
    }
    return copiedMatrix.IsStaticDeclared();
}

template<typename T, uint32 nRows, uint32 nCols>
bool MatrixTest::TestCopyAssignmentByTable(T (&matrix)[nRows][nCols]) {
    Matrix<T> originalMatrix(matrix);
    Matrix<T> copiedMatrix;
    copiedMatrix = originalMatrix;
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (copiedMatrix[i][j] != matrix[i][j]) {
                return false;
            }
        }
    }
    if (originalMatrix.GetDataPointer() != copiedMatrix.GetDataPointer()) {
        return false;
    }
    return copiedMatrix.IsStaticDeclared();
}

template<typename T>
bool MatrixTest::TestCopyAssignmentByCanDestroyNonStaticMatrix() {
    uint32 nRows = 2;
    uint32 nCols = 2;
    Matrix<T> originalMatrix(nRows, nCols);
    for (uint32 row = 0u; row < nRows; row++) {
        for (uint32 col = 0u; col < nCols; col++) {
            originalMatrix[row][col] = row * 2 + col * row;
        }
    }
    Matrix<T> copiedMatrix;
    copiedMatrix = originalMatrix;
    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (copiedMatrix[i][j] != originalMatrix[i][j]) {
                return false;
            }
        }
    }
    if (originalMatrix.GetDataPointer() == copiedMatrix.GetDataPointer()) {
        return false;
    }
    return !copiedMatrix.IsStaticDeclared();
}

#endif /* MATRIXTEST_H_ */

