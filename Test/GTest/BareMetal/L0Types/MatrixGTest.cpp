/**
 * @file MatrixGTest.cpp
 * @brief Source file for class MatrixGTest
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
 * the class MatrixGTest (public, protected, and private). Be aware that some 
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
#include "MatrixTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L0Types_MatrixGTest,TestDefaultConstructor) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestDefaultConstructor());
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorOnHeap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestConstructorOnHeap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_Int8) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int8 **matrix = new int8*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int8[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_UInt8) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint8 **matrix = new uint8*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint8[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_Int16) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int16 **matrix = new int16*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int16[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_UInt16) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint16 **matrix = new uint16*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint16[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_Int32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int32 **matrix = new int32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_UInt32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint32 **matrix = new uint32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_Int64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int64 **matrix = new int64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_UInt64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint64 **matrix = new uint64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));

    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_Float32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    float32 **matrix = new float32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new float32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j) + 0.956 * i - 1.112 * j;
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerHeap_Float64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    float64 **matrix = new float64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new float64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j) + 0.956 * i - 1.112 * j;
        }
    }

    ASSERT_TRUE(matrixTest.TestConstructorByPointerHeap(matrix, nRows, nCols));

    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }

    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_Int8) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int8 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int8> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_UInt8) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint8 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }
    Matrix<uint8> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_Int16) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int16 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int16> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_UInt16) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint16 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }
    Matrix<uint16> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_Int32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int32 matrix[nRows][nCols];

    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_UInt32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }
    Matrix<uint32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_Int64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    int64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_UInt64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    uint64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j);
        }
    }
    Matrix<uint64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_Float32) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    float32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j) + 0.956 * i - 1.112 * j;
        }
    }
    Matrix<float32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByPointerStatic_Float64) {
    MatrixTest matrixTest;

    const int32 nRows = 2;
    const int32 nCols = 2;
    float64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i + j) + 0.956 * i - 1.112 * j;
        }
    }
    Matrix<float64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_Int8) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int8 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_UInt8) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint8 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_Int16) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int16 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_UInt16) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint16 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_Int32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int32 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_UInt32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint32 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_Int64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int64 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_UInt64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint64 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_Float32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    float32 matrix[][nCols] = { { 1.99, -0.002 }, { -399E+3, 4.0099 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestConstructorByTable_Float64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    float64 matrix[][nCols] = { { 1.99, -0.002 }, { -399E+3, 4.0099 } };

    ASSERT_TRUE(matrixTest.TestConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_uint8) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint8 **matrix = new uint8*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint8[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_int8) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int8 **matrix = new int8*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int8[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_uint16) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint16 **matrix = new uint16*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint16[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_int16) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int16 **matrix = new int16*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int16[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_uint32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint32 **matrix = new uint32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_int32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int32 **matrix = new int32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_uint64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint64 **matrix = new uint64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_int64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int64 **matrix = new int64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_float32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    float32 **matrix = new float32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new float32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerHeap_float64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    float64 **matrix = new float64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new float64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}



TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_int8) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int8 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int8> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_uint8) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint8 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<uint8> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_int16) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int16 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int16> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_uint16) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint16 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<uint16> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_int32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_uint32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<uint32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_int64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_uint64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<uint64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_float32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    float32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<float32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByPointerStatic_float64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    float64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<float64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyConstructorByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_int8) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int8 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_uint8) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint8 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_int16) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int16 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_uint16) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint16 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_int32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int32 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_uint32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint32 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_int64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int64 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_uint64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint64 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_float32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    float32 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByTable_float64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    float64 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyConstructorByTable(matrix));
}


TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_int8) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<int8>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_uint8) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<uint8>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_int16) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<int16>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_uint16) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<uint16>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_int32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<int32>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_uint32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<uint32>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_int64) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<int64>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_uint64) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<uint64>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_float32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<float32>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorByCanDestroyNonStaticMatrix_float64) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorByCanDestroyNonStaticMatrix<float64>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_uint8) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint8 **matrix = new uint8*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint8[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_int8) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int8 **matrix = new int8*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int8[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_uint16) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint16 **matrix = new uint16*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint16[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_int16) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int16 **matrix = new int16*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int16[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_uint32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint32 **matrix = new uint32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_int32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int32 **matrix = new int32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_uint64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint64 **matrix = new uint64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new uint64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_int64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int64 **matrix = new int64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new int64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_float32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    float32 **matrix = new float32*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new float32[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerHeap_float64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    float64 **matrix = new float64*[nRows];
    for (int32 i = 0; i < nRows; i++) {
        matrix[i] = new float64[nCols];
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerHeap(matrix, nRows, nCols));
    for (int32 i = 0; i < nRows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_int8) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int8 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int8> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_uint8) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint8 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<uint8> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_int16) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int16 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int16> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_uint16) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint16 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<uint16> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_int32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_uint32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<uint32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_int64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    int64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<int64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_uint64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    uint64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<uint64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_float32) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    float32 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<float32> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByPointerStatic_float64) {
    MatrixTest matrixTest;
    const int32 nRows = 2;
    const int32 nCols = 2;
    float64 matrix[nRows][nCols];
    for (int32 i = 0; i < nRows; i++) {
        for (int32 j = 0; j < nCols; j++) {
            matrix[i][j] = (i - j);
        }
    }
    Matrix<float64> matrix2(&matrix[0][0], nRows, nCols);
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByPointerStatic(matrix, matrix2, nRows, nCols));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_int8) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int8 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_uint8) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint8 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_int16) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int16 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_uint16) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint16 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_int32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int32 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_uint32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint32 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_int64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    int64 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_uint64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    uint64 matrix[][nCols] = { { 1, 2 }, { 3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_float32) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    float32 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByTable_float64) {
    MatrixTest matrixTest;

    const uint32 nCols = 2;
    float64 matrix[][nCols] = { { 1, -2 }, { -3, 4 } };

    ASSERT_TRUE(matrixTest.TestCopyAssignmentByTable(matrix));
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_int8) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<int8>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_uint8) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<uint8>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_int16) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<int16>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_uint16) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<uint16>());
}


TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_int32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<int32>());
}


TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_uint32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<uint32>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_int64) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<int64>());
}


TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_uint64) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<uint64>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_float32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<float32>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentByCanDestroyNonStaticMatrix_float64) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentByCanDestroyNonStaticMatrix<float64>());
}

TEST(BareMetal_L0Types_MatrixGTest,TestGetNumberOfColumns_32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetNumberOfColumns(32));
}

TEST(BareMetal_L0Types_MatrixGTest,TestGetNumberOfColumns_0) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetNumberOfColumns(0));
}

TEST(BareMetal_L0Types_MatrixGTest,TestGetNumberOfRows_32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetNumberOfRows(32));
}

TEST(BareMetal_L0Types_MatrixGTest,TestGetNumberOfRows_0) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetNumberOfRows(0));
}

TEST(BareMetal_L0Types_MatrixGTest,TestMatrixOperator_Static) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixOperator_Static());
}

TEST(BareMetal_L0Types_MatrixGTest,TestMatrixOperator_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixOperator_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestMatrixOperator_HeapDisc) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixOperator_HeapDisc());
}

TEST(BareMetal_L0Types_MatrixGTest,TestMatrixFunctionCallOperator_Static) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixFunctionCallOperator_Static());
}

TEST(BareMetal_L0Types_MatrixGTest,TestMatrixFunctionCallOperator_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestMatrixFunctionCallOperator_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestGetDataPointer) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestGetDataPointer());
}

TEST(BareMetal_L0Types_MatrixGTest,TestIsStaticDeclared) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestIsStaticDeclared());
}

TEST(BareMetal_L0Types_MatrixGTest,TestProduct) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestProduct());
}

TEST(BareMetal_L0Types_MatrixGTest,TestSum) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestSum());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopy) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopy());
}

TEST(BareMetal_L0Types_MatrixGTest,TestSubMatrix) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestSubMatrix());
}

TEST(BareMetal_L0Types_MatrixGTest,TestDeterminant) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestDeterminant());
}

TEST(BareMetal_L0Types_MatrixGTest,TestTranspose) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestTranspose());
}

TEST(BareMetal_L0Types_MatrixGTest,TestInverse_float32) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_float32());
}

TEST(BareMetal_L0Types_MatrixGTest,TestInverse_float64) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_float64());
}

TEST(BareMetal_L0Types_MatrixGTest,TestInverse_NullDeterminant) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_NullDeterminant());
}

TEST(BareMetal_L0Types_MatrixGTest,TestProduct_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestProduct_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestSum_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestSum_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopy_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopy_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestSubMatrix_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestSubMatrix_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestDeterminant_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestDeterminant_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestTranspose_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestTranspose_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestInverse_float32_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_float32_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestInverse_float64_Heap) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestInverse_float64_Heap());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyConstructorNULLPointer) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyConstructorNULLPointer());
}

TEST(BareMetal_L0Types_MatrixGTest,TestCopyAssignmentNULLPointer) {
    MatrixTest matrixTest;
    ASSERT_TRUE(matrixTest.TestCopyAssignmentNULLPointer());
}


