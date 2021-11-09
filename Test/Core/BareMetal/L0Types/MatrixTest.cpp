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
#include <stdio.h>

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

    const int32 nRows = 32;
    const int32 nCols = 32;

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

    int32 **matrix = new int32*[nRows];

    for (uint32 i = 0; i < nRows; i++) {
        matrix[i] = new int32[nCols];
    }

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            matrix[i][j] = i + j;
        }
    }

    Matrix<int32> myMatrix(matrix, nRows, nCols);

    bool ret = true;
    for (uint32 i = 0; (i < nRows) && ret; i++) {
        Vector<int32> row = myMatrix[i];
        for (uint32 j = 0; (j < nCols) && ret; j++) {
            if (row[j] != matrix[i][j]) {
                ret = false;
            }
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;

    return ret;
}

bool MatrixTest::TestMatrixOperator_HeapDisc() {
    const uint32 nRows = 3;
    const uint32 nCols = 3;

    int32 **matrix = new int32*[nRows];
    int32 **auxP1 = new int32*[nRows];
    for (uint32 i = 0; i < nRows; i++) {
        auxP1[i] = new int32[nCols];
        MemoryOperationsHelper::Set(reinterpret_cast<void*>(auxP1[i]), 0, sizeof(int32) * nCols);
        matrix[i] = new int32[nCols];
    }

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            matrix[i][j] = i + j;
        }
    }

    Matrix<int32> myMatrix(matrix, nRows, nCols);

    bool ret = true;
    for (uint32 i = 0; (i < nRows) && ret; i++) {
        Vector<int32> row = myMatrix[i];
        for (uint32 j = 0; (j < nCols) && ret; j++) {
            if (row[j] != static_cast<int32>(i + j)) {
                ret = false;
            }
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
        delete[] auxP1[i];
        auxP1[i] = NULL;
    }

    delete[] matrix;
    delete[] auxP1;
    return ret;
}

bool MatrixTest::TestMatrixFunctionCallOperator_Static() {
    {
        const uint32 nRow = 32;
        const uint32 nCols = 32;
        int32 matrix[nRow][nCols];
        Matrix<int32> myMatrix(matrix);

        for (uint32 i = 0; i < nRow; i++) {
            for (uint32 j = 0; j < nCols; j++) {
                matrix[i][j] = i + j;
            }
        }

        for (uint32 i = 0; i < nRow; i++) {
            for (uint32 j = 0; j < nCols; j++) {
                if (myMatrix(i, j) != matrix[i][j]) {
                    return false;
                }
            }
        }
    }
    {
        const uint32 nRow = 32;
        const uint32 nCols = 32;
        int32 matrix[nRow][nCols];
        Matrix<int32> myMatrix(matrix);

        for (uint32 i = 0; i < nRow; i++) {
            for (uint32 j = 0; j < nCols; j++) {
                myMatrix(i, j) = i + j;
            }
        }

        for (uint32 i = 0; i < nRow; i++) {
            for (uint32 j = 0; j < nCols; j++) {
                if (myMatrix(i, j) != matrix[i][j]) {
                    return false;
                }
            }
        }
    }
    return true;
}

bool MatrixTest::TestMatrixFunctionCallOperator_Heap() {
    bool ret = true;

    const uint32 nRows = 32;
    const uint32 nCols = 32;

    Matrix<int32> myMatrix(nRows, nCols);

    int32 **matrix = new int32*[nRows];

    for (uint32 i = 0; i < nRows; i++) {
        matrix[i] = new int32[nCols];
    }

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            myMatrix(i, j) = i + j;
            matrix[i][j] = i + j;
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        for (uint32 j = 0; j < nCols; j++) {
            if (myMatrix(i, j) != matrix[i][j]) {
                ret = false;
            }
        }
    }

    for (uint32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;

    return ret;
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

    int32 **heapArray = NULL;
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

    int32 **heapArray = NULL;
    Matrix<int32> matrix3(heapArray, 2, 2);

    if (matrix3.IsStaticDeclared()) {
        return false;
    }

    Matrix<int32> matrix4(2, 2);

    return !matrix4.IsStaticDeclared();
}

bool MatrixTest::TestProduct() {

    int32 matrix1[4][3] = { { 1, 2, -3 }, { -4, 5, 6 }, { 7, 8, 9 }, { -3, 2, 1 } };
    int32 matrix2[3][2] = { { 1, 2 }, { -4, 5 }, { 7, 8 } };

    int32 matrix3[4][2];

    Matrix<int32> factor1(matrix1);
    Matrix<int32> factor2(matrix2);
    Matrix<int32> result(matrix3);

    if (!factor1.Product(factor2, result)) {
        return false;
    }

    int32 testResult[4][2] = { { -28, -12 }, { 18, 65 }, { 38, 126 }, { -4, 12 } };

    for (int32 i = 0; i < 4; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (result[i][j] != testResult[i][j]) {
                return false;
            }
        }
    }

    // test the failure
    int32 matrix4[5][2];
    Matrix<int32> result_failure(matrix4);

    return (!factor1.Product(factor2, result_failure));
}

bool MatrixTest::TestSum() {

    int32 matrix1[3][3] = { { 1, 2, -3 }, { -4, 5, 6 }, { 7, 8, 9 } };
    int32 matrix2[3][3] = { { 1, 2, 1 }, { -4, 1, 5 }, { 1, 7, 8 } };

    int32 matrix3[3][3];

    Matrix<int32> addend1(matrix1);
    Matrix<int32> addend2(matrix2);
    Matrix<int32> result(matrix3);

    if (!addend1.Sum(addend2, result)) {
        return false;
    }

    int32 testResult[3][3] = { { 2, 4, -2 }, { -8, 6, 11 }, { 8, 15, 17 } };

    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 3; j++) {
            if (result[i][j] != testResult[i][j]) {
                return false;
            }
        }
    }
    // test the failure
    int32 matrix4[5][2];
    Matrix<int32> result_failure(matrix4);

    return (!addend1.Sum(addend2, result_failure));
}

bool MatrixTest::TestCopy() {

    int32 matrix1[3][3] = { { 1, 2, -3 }, { -4, 5, 6 }, { 7, 8, 9 } };
    int32 matrix2[3][3] = { { 1, 2, 1 }, { -4, 1, 5 }, { 1, 7, 8 } };

    Matrix<int32> addend1(matrix1);
    Matrix<int32> addend2(matrix2);

    if (!addend1.Copy(addend2)) {
        return false;
    }
    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 3; j++) {
            if (addend1[i][j] != addend2[i][j]) {
                return false;
            }
        }
    }
    // test the failure
    int32 matrix4[5][2];
    Matrix<int32> result_failure(matrix4);
    return (!addend1.Copy(result_failure));
}

bool MatrixTest::TestSubMatrix() {
    int32 matrix1[4][3] = { { 1, 2, -3 }, { -4, 5, 6 }, { 7, 8, 9 }, { -3, 2, 1 } };

    int32 matrix2[2][2];

    Matrix<int32> bigMatrix(matrix1);
    Matrix<int32> subMatrix(matrix2);

    if (!bigMatrix.SubMatrix(1, 2, 1, 2, subMatrix)) {
        return false;
    }
    int32 test[][2] = { { 5, 6 }, { 8, 9 } };

    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (subMatrix[i][j] != test[i][j]) {

                return false;
            }
        }
    }

    Matrix<int32> subMatrix2(1, 1);

    if (!bigMatrix.SubMatrix(2, 2, 2, 2, subMatrix2)) {
        printf("\n1\n");
        return false;
    }

    if (subMatrix2[0][0] != bigMatrix[2][2]) {
        printf("\n%d %d\n", subMatrix[0][0], bigMatrix[2][2]);
        return false;
    }

    //incorrect output sizes
    if (bigMatrix.SubMatrix(2, 2, 2, 2, subMatrix)) {
        printf("\n3\n");

        return false;
    }

    // incorrect indexes
    if (bigMatrix.SubMatrix(2, 1, 2, 2, subMatrix)) {
        printf("\n4\n");
        return false;
    }

    // incorrect indexes 2
    if (bigMatrix.SubMatrix(2, 3, 2, 3, subMatrix)) {
        printf("\n5\n");
        return false;
    }

    return true;
}

bool MatrixTest::TestDeterminant() {
    float32 matrix1[2][2] = { { 1.0, 2.0 }, { 3.0, 4.0 } };

    Matrix<float32> testMatrix(matrix1);

    float32 determinant = 0.0;

    if (!testMatrix.Determinant(determinant)) {
        return false;
    }

    if (determinant != -2.0) {
        return false;
    }

    float64 matrix2[3][3] = { { 1.0, 2.0, -1.0 }, { 3.0, 4.0, 2.0 }, { -2.0, -3.0, 4.0 } };

    Matrix<float64> testMatrix2(matrix2);

    float64 determinant64 = 0.0;

    if (!testMatrix2.Determinant(determinant64)) {
        return false;
    }

    if (determinant64 != -9.0) {
        return false;
    }

    float32 matrix3[2][3] = { { 1.0, 2.0, -1.0 }, { 3.0, 4.0, 2.0 } };
    Matrix<float32> testFail(matrix3);

    return !testFail.Determinant(determinant);
}

bool MatrixTest::TestTranspose() {
    int32 matrix1[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
    int32 matrix2[3][2];

    Matrix<int32> testMatrix(matrix1);
    Matrix<int32> transpose(matrix2);
    if (!testMatrix.Transpose(transpose)) {
        return false;
    }

    int32 transposeTest[3][2] = { { 1, 4 }, { 2, 5 }, { 3, 6 } };

    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (transpose[i][j] != transposeTest[i][j]) {
                return false;
            }
        }
    }

    int32 matrix3[3][3];
    Matrix<int32> testFail(matrix3);

    return !testMatrix.Transpose(testFail);
}

bool MatrixTest::TestInverse_float32() {
    float32 matrix1[2][2] = { { 1, 2 }, { 3, 4 } };

    float32 matrix2[2][2];
    Matrix<float32> testMatrix(matrix1);
    Matrix<float32> inverse(matrix2);

    if (!testMatrix.Inverse(inverse)) {
        return false;
    }

    float32 inverseTest[2][2] = { { -2.0, 1.0 }, { 1.5, -0.5 } };

    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (inverse[i][j] != inverseTest[i][j]) {
                return false;
            }
        }
    }

    float32 matrix3[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
    Matrix<float32> testFailNotSquare(matrix3);
    float32 matrix4[2][3];
    Matrix<float32> inverseFailNotSquare(matrix4);

    // not square matrices
    if (testFailNotSquare.Inverse(inverseFailNotSquare)) {
        return false;
    }

    // not equal dimensions
    return !testMatrix.Inverse(inverseFailNotSquare);
}

bool MatrixTest::TestInverse_float64() {
    float64 matrix1[2][2] = { { 1, 2 }, { 3, 4 } };

    float64 matrix2[2][2];
    Matrix<float64> testMatrix(matrix1);
    Matrix<float64> inverse(matrix2);

    if (!testMatrix.Inverse(inverse)) {
        return false;
    }

    float64 inverseTest[2][2] = { { -2.0, 1.0 }, { 1.5, -0.5 } };

    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (inverse[i][j] != inverseTest[i][j]) {
                return false;
            }
        }
    }
    float64 matrix3[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
    Matrix<float64> testFailNotSquare(matrix3);
    float64 matrix4[2][3];
    Matrix<float64> inverseFailNotSquare(matrix4);

    // not square matrices
    if (testFailNotSquare.Inverse(inverseFailNotSquare)) {
        return false;
    }

    // not equal dimensions
    return !testMatrix.Inverse(inverseFailNotSquare);
}

bool MatrixTest::TestInverse_NullDeterminant() {
    float32 matrix1[2][2] = { { 1, 2 }, { 2, 4 } };

    float32 matrix2[2][2];
    Matrix<float32> testMatrix(matrix1);
    Matrix<float32> inverse(matrix2);

    return !testMatrix.Inverse(inverse);

}

bool MatrixTest::TestProduct_Heap() {

    int32 matrix1[4][3] = { { 1, 2, -3 }, { -4, 5, 6 }, { 7, 8, 9 }, { -3, 2, 1 } };
    int32 matrix2[3][2] = { { 1, 2 }, { -4, 5 }, { 7, 8 } };

    int32 matrix3[4][2];

    Matrix<int32> factor1(4, 3);

    for (int32 i = 0; i < 4; i++) {
        for (int32 j = 0; j < 3; j++) {
            factor1[i][j] = matrix1[i][j];
        }
    }

    Matrix<int32> factor2(3, 2);
    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 2; j++) {
            factor2[i][j] = matrix2[i][j];
        }
    }

    Matrix<int32> result(matrix3);

    if (!factor1.Product(factor2, result)) {
        return false;
    }

    int32 testResult[4][2] = { { -28, -12 }, { 18, 65 }, { 38, 126 }, { -4, 12 } };

    for (int32 i = 0; i < 4; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (result[i][j] != testResult[i][j]) {
                return false;
            }
        }
    }

    // test the failure
    int32 matrix4[5][2];
    Matrix<int32> result_failure(matrix4);

    return (!factor1.Product(factor2, result_failure));
}

bool MatrixTest::TestSum_Heap() {

    int32 matrix1[3][3] = { { 1, 2, -3 }, { -4, 5, 6 }, { 7, 8, 9 } };
    int32 matrix2[3][3] = { { 1, 2, 1 }, { -4, 1, 5 }, { 1, 7, 8 } };

    int32 matrix3[3][3];

    Matrix<int32> addend1(3, 3);

    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 3; j++) {
            addend1[i][j] = matrix1[i][j];
        }
    }

    Matrix<int32> addend2(3, 3);
    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 3; j++) {
            addend2[i][j] = matrix2[i][j];
        }
    }

    Matrix<int32> result(matrix3);

    if (!addend1.Sum(addend2, result)) {
        return false;
    }
    int32 testResult[3][3] = { { 2, 4, -2 }, { -8, 6, 11 }, { 8, 15, 17 } };
    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 3; j++) {
            if (result[i][j] != testResult[i][j]) {
                return false;
            }
        }
    }
    // test the failure
    int32 matrix4[5][2];
    Matrix<int32> result_failure(matrix4);

    return (!addend1.Sum(addend2, result_failure));
}

bool MatrixTest::TestCopy_Heap() {

    int32 matrix1[3][3] = { { 1, 2, -3 }, { -4, 5, 6 }, { 7, 8, 9 } };
    int32 matrix2[3][3] = { { 1, 2, 1 }, { -4, 1, 5 }, { 1, 7, 8 } };

    Matrix<int32> addend1(3, 3);

    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 3; j++) {
            addend1[i][j] = matrix1[i][j];
        }
    }

    Matrix<int32> addend2(3, 3);
    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 3; j++) {
            addend2[i][j] = matrix2[i][j];
        }
    }
    if (!addend1.Copy(addend2)) {
        return false;
    }
    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 3; j++) {
            if (addend1[i][j] != addend2[i][j]) {
                return false;
            }
        }
    }
    // test the failure
    int32 matrix4[5][2];
    Matrix<int32> result_failure(matrix4);

    return (!addend1.Copy(result_failure));
}

bool MatrixTest::TestSubMatrix_Heap() {
    int32 matrix1[4][3] = { { 1, 2, -3 }, { -4, 5, 6 }, { 7, 8, 9 }, { -3, 2, 1 } };

    int32 matrix2[2][2];

    Matrix<int32> bigMatrix(4, 3);
    for (int32 i = 0; i < 4; i++) {
        for (int32 j = 0; j < 3; j++) {
            bigMatrix[i][j] = matrix1[i][j];
        }
    }

    Matrix<int32> subMatrix(matrix2);

    if (!bigMatrix.SubMatrix(1, 2, 1, 2, subMatrix)) {
        return false;
    }
    int32 test[][2] = { { 5, 6 }, { 8, 9 } };

    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (subMatrix[i][j] != test[i][j]) {

                return false;
            }
        }
    }
    return true;
}

bool MatrixTest::TestDeterminant_Heap() {
    float32 matrix1[2][2] = { { 1.0, 2.0 }, { 3.0, 4.0 } };

    Matrix<float32> testMatrix(2, 2);
    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            testMatrix[i][j] = matrix1[i][j];
        }
    }

    float32 determinant = 0.0;

    if (!testMatrix.Determinant(determinant)) {
        return false;
    }

    if (determinant != -2.0) {
        return false;
    }

    float64 matrix2[3][3] = { { 1.0, 2.0, -1.0 }, { 3.0, 4.0, 2.0 }, { -2.0, -3.0, 4.0 } };

    Matrix<float64> testMatrix2(matrix2);

    float64 determinant64 = 0.0;

    if (!testMatrix2.Determinant(determinant64)) {
        return false;
    }

    if (determinant64 != -9.0) {
        return false;
    }

    float32 matrix3[2][3] = { { 1.0, 2.0, -1.0 }, { 3.0, 4.0, 2.0 } };
    Matrix<float32> testFail(matrix3);

    return !testFail.Determinant(determinant);
}

bool MatrixTest::TestTranspose_Heap() {
    int32 matrix1[2][3] = { { 1, 2, 3 }, { 4, 5, 6 } };
    int32 matrix2[3][2];

    Matrix<int32> testMatrix(2, 3);
    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 3; j++) {
            testMatrix[i][j] = matrix1[i][j];
        }
    }

    Matrix<int32> transpose(matrix2);
    if (!testMatrix.Transpose(transpose)) {
        return false;
    }

    int32 transposeTest[3][2] = { { 1, 4 }, { 2, 5 }, { 3, 6 } };

    for (int32 i = 0; i < 3; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (transpose[i][j] != transposeTest[i][j]) {
                return false;
            }
        }
    }

    int32 matrix3[3][3];
    Matrix<int32> testFail(matrix3);

    return !testMatrix.Transpose(testFail);
}

bool MatrixTest::TestInverse_float32_Heap() {
    float32 matrix1[2][2] = { { 1, 2 }, { 3, 4 } };

    float32 matrix2[2][2];
    Matrix<float32> testMatrix(2, 2);
    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            testMatrix[i][j] = matrix1[i][j];
        }
    }

    Matrix<float32> inverse(matrix2);

    if (!testMatrix.Inverse(inverse)) {
        return false;
    }

    float32 inverseTest[2][2] = { { -2.0, 1.0 }, { 1.5, -0.5 } };

    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (inverse[i][j] != inverseTest[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool MatrixTest::TestInverse_float64_Heap() {
    float64 matrix1[2][2] = { { 1, 2 }, { 3, 4 } };

    float64 matrix2[2][2];
    Matrix<float64> testMatrix(2, 2);
    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            testMatrix[i][j] = matrix1[i][j];
        }
    }

    Matrix<float64> inverse(matrix2);

    if (!testMatrix.Inverse(inverse)) {
        return false;
    }

    float64 inverseTest[2][2] = { { -2.0, 1.0 }, { 1.5, -0.5 } };

    for (int32 i = 0; i < 2; i++) {
        for (int32 j = 0; j < 2; j++) {
            if (inverse[i][j] != inverseTest[i][j]) {
                return false;
            }
        }
    }
    return true;
}

bool MatrixTest::TestCopyConstructorNULLPointer() {
    Matrix<int32> originalMatrix;
    Matrix<int32> copiedMatrix(originalMatrix);
    bool ret = copiedMatrix.GetDataPointer() == NULL_PTR(void*);
    if (ret) {
        ret = copiedMatrix.GetNumberOfColumns() == 0u;
    }
    if (ret) {
        ret = copiedMatrix.GetNumberOfRows() == 0u;
    }
    if (ret) {
        ret = copiedMatrix.IsStaticDeclared();
    }
    return ret;
}

bool MatrixTest::TestCopyAssignmentNULLPointer() {
    int32 *pointer = NULL_PTR(int32*);
    Matrix<int32> originalMatrix(pointer, 2, 2);
    Matrix<int32> copiedMatrix;
    copiedMatrix = originalMatrix;
    bool ret = copiedMatrix.GetDataPointer() == NULL_PTR(void*);
    if (ret) {
        ret = copiedMatrix.GetNumberOfColumns() == 0u;
    }
    if (ret) {
        ret = copiedMatrix.GetNumberOfRows() == 0u;
    }
    if (ret) {
        ret = copiedMatrix.IsStaticDeclared();
    }
    return ret;
}

