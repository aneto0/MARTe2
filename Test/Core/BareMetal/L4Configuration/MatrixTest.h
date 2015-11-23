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

class MatrixTest {

public:

    bool TestDefaultConstructor();

    bool TestConstructorOnHeap();

    template<typename T>
    bool TestConstructorByPointerHeap(T** matrix,
                                      uint32 nRows,
                                      uint32 nCols);

    template<typename T, uint32 nRows, uint32 nCols>
    bool TestConstructorByTable(T (&matrix)[nRows][nCols]);

    bool TestGetNumberOfColumns(uint32 nCols);

    bool TestGetNumberOfRows(uint32 nRows);

    bool TestMatrixOperator_Static();

    bool TestMatrixOperator_Heap();

    bool TestGetDataPointer();

    bool TestIsStaticDeclared();

    bool TestProduct();

    bool TestSubMatrix();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool MatrixTest::TestConstructorByPointerHeap(T** matrix,
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

    return true;

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

    return true;
}

#endif /* MATRIXTEST_H_ */

