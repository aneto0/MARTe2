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
