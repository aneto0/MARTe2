/**
 * @file MatrixTest.cpp
 * @brief Source file for class MatrixTest
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
 * the class MatrixTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MatrixTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool MatrixTest::TestDefaultConstructor() {

    Matrix<int32> myMatrix;

    if (!myMatrix.IsStaticDeclared()) {
        return false;
    }

    if (myMatrix.GetNumberOfColumns() != 0) {
        return false;
    }

    if (myMatrix.GetNumberOfRows() != 0) {
        return false;
    }

    return myMatrix.GetDataPointer() == NULL;
}

bool MatrixTest::TestConstructorOnHeap() {

    const uint32 nRows = 32;
    const uint32 nCols = 32;

    Matrix<int32> myMatrix(nRows, nCols);

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            myMatrix[i][j] = (i + j);
        }
    }

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            if (myMatrix[i][j] != (i + j)) {
                return false;
            }
        }
    }

    return true;
}

bool MatrixTest::TestGetNumberOfColumns(uint32 nCols) {
    Matrix<int32> myMatrix(1u, nCols);

    return (myMatrix.GetNumberOfColumns() == nCols);
}

bool MatrixTest::TestGetNumberOfRows(uint32 nRows) {
    Matrix<int32> myMatrix(nRows, 1u);
    return (myMatrix.GetNumberOfRows() == nRows);
}

bool MatrixTest::TestMatrixOperator_Static() {

    const uint32 nRow = 32;
    const uint32 nCols = 32;
    int32 matrix[nRow][nCols];

    for (uint32 i = 0; i < nRow; i++) {

        for (uint32 j = 0; j < nCols; j++) {
            matrix[i][j] = i + j;
        }
    }

    Matrix<int32> myMatrix(matrix);

    for (uint32 i = 0; i < nRow; i++) {
        Vector<int32> row = myMatrix[i];
        for (uint32 j = 0; j < nCols; j++) {
            if (row[j] != matrix[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool MatrixTest::TestMatrixOperator_Heap() {

    const uint32 nRows = 32;
    const uint32 nCols = 32;

    int32 ** matrix = new int32*[nRows];

    for (uint32 i = 0; i < nRows; i++) {
        matrix[i] = new int32[nCols];
    }

    for (uint32 i = 0; i < nRows; i++) {

        for (uint32 j = 0; j < nCols; j++) {
            matrix[i][j] = i + j;
        }
    }

    Matrix<int32> myMatrix(matrix, nRows, nCols);

    for (uint32 i = 0; i < nRows; i++) {
        Vector<int32> row = myMatrix[i];
        for (uint32 j = 0; j < nCols; j++) {
            if (row[j] != matrix[i][j]) {
                return false;
            }
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;

    return true;
}

bool MatrixTest::TestGetDataPointer() {
    Matrix<int32> matrix1;

    if (matrix1.GetDataPointer() != NULL) {
        return false;
    }

    int32 array[2][2];

    Matrix<int32> matrix2(array);

    if (matrix2.GetDataPointer() != array) {
        return false;
    }

    int32 **heapArray;
    Matrix<int32> matrix3(heapArray, 2, 2);

    if (matrix3.GetDataPointer() != heapArray) {
        return false;
    }

    Matrix<int32> matrix4(2, 2);

    return matrix4.GetDataPointer() != NULL;
}

bool MatrixTest::TestIsStaticDeclared() {
    Matrix<int32> matrix1;

    if (!matrix1.IsStaticDeclared()) {
        return false;
    }

    int32 array[2][2];

    Matrix<int32> matrix2(array);

    if (!matrix2.IsStaticDeclared()) {
        return false;
    }

    int32 **heapArray;
    Matrix<int32> matrix3(heapArray, 2, 2);

    if (matrix3.IsStaticDeclared()) {
        return false;
    }

    Matrix<int32> matrix4(2, 2);

    return !matrix4.IsStaticDeclared();
}

