/**
 * @file Matrix.cpp
 * @brief Header file for class AnyType
 * @date 1 Oct 2017
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include "Matrix.h"

namespace MARTe{


template<>
bool Matrix<float32>::Determinant(float32 &det) const {

    bool ret = (numberOfRows == numberOfColumns);

    if (ret) {
        const Matrix<float32> &temp = *this;
        if (numberOfRows == 1u) {
            det = temp[0][0];
        }
        else {
            det = 0.0f;
            // loop on the first row
            for (uint32 k = 0u; k < numberOfColumns; k++) {
                Matrix<float32> subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
                uint32 n = 0u;
                for (uint32 i = 1u; i < numberOfRows; i++) {
                    uint32 m = 0u;
                    for (uint32 j = 0u; j < numberOfColumns; j++) {
                        if (j != k) {
                            subMatrix[n][m] = temp[i][j];
                            m++;
                        }
                    }
                    n++;
                }
                float32 subDet = 0.0f;
                subMatrix.Determinant(subDet);
                float32 sign = (((k) & (1u)) == 0u) ? (1.0f) : (-1.0f);
                det += (sign * temp[0][k]) * subDet;
            }
        }
    }

    return ret;
}

template<>
bool Matrix<float64>::Determinant(float64 &det) const {
    bool ret = (numberOfRows == numberOfColumns);

    if (ret) {
        const Matrix<float64> &temp = *this;
        if (numberOfRows == 1u) {
            det = temp[0][0];
        }
        else {
            det = 0.0;
            // loop on the first row
            for (uint32 k = 0u; k < numberOfColumns; k++) {
                Matrix<float64> subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
                uint32 n = 0u;
                for (uint32 i = 1u; i < numberOfRows; i++) {
                    uint32 m = 0u;
                    for (uint32 j = 0u; j < numberOfColumns; j++) {
                        if (j != k) {
                            subMatrix[n][m] = temp[i][j];
                            m++;
                        }
                    }
                    n++;
                }
                float64 subDet = 0.0;
                subMatrix.Determinant(subDet);
                float64 sign = (((k) & (1u)) == 0u) ? (1.0) : (-1.0);
                det += (sign * temp[0][k]) * subDet;
            }
        }
    }
    return ret;
}

template<>
bool Matrix<float32>::Inverse(Matrix<float32> &inverse) const {
    bool cond1 = (numberOfColumns == numberOfRows);
    bool cond2 = (inverse.numberOfRows == numberOfRows);
    bool cond3 = (inverse.numberOfColumns == numberOfColumns);
    float32 determinant = 0.0f;

    bool cond4 = Determinant(determinant);
    bool cond5 = (determinant != 0.0f);
    bool ret = ((cond1) && (cond2) && (cond3) && (cond4) && (cond5));

    if (ret) {
        const Matrix<float32> &temp = *this;
        Matrix<float32> subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                Matrix<float32> subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
                uint32 n = 0u;
                for (uint32 h = 0u; h < numberOfRows; h++) {
                    uint32 m = 0u;
                    if (h != i) {
                        for (uint32 k = 0u; k < numberOfColumns; k++) {
                            if (k != j) {
                                subMatrix[n][m] = temp[h][k];
                                m++;
                            }
                        }
                        n++;
                    }
                }
                float32 subDet = 0.0f;
                subMatrix.Determinant(subDet);
                float32 sign = (((i + j) & (1u)) == 0u) ? (1.0F) : (-1.0F);
                inverse[j][i] = (subDet * sign) / determinant;
            }
        }
    }

    return ret;
}

template<>
bool Matrix<float64>::Inverse(Matrix<float64> &inverse) const {
    bool cond1 = (numberOfColumns == numberOfRows);
    bool cond2 = (inverse.numberOfRows == numberOfRows);
    bool cond3 = (inverse.numberOfColumns == numberOfColumns);
    float64 determinant = 0.0;

    bool cond4 = Determinant(determinant);
    bool cond5 = (determinant != 0.0);
    bool ret = ((cond1) && (cond2) && (cond3) && (cond4) && (cond5));

    if (ret) {
        const Matrix<float64> &temp = *this;
        Matrix<float64> subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                Matrix<float64> subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
                uint32 n = 0u;
                for (uint32 h = 0u; h < numberOfRows; h++) {
                    uint32 m = 0u;
                    if (h != i) {
                        for (uint32 k = 0u; k < numberOfColumns; k++) {
                            if (k != j) {
                                subMatrix[n][m] = temp[h][k];
                                m++;
                            }
                        }
                        n++;
                    }
                }
                float64 subDet = 0.0;
                subMatrix.Determinant(subDet);
                float64 sign = (((i + j) & (1u)) == 0u) ? (1.0) : (-1.0);
                inverse[j][i] = (subDet * sign) / determinant;
            }
        }
    }
    return ret;
}



} //MARTe
