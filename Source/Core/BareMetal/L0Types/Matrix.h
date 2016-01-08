/**
 * @file Matrix.h
 * @brief Header file for class Matrix
 * @date 05/11/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class Matrix
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATRIX_H_
#define MATRIX_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Vector.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Fixed size matrix of values.
 * @details The Matrix can allocate and manage its own memory or it can be associated
 * to an existent memory table.
 * @tparam T TODO Define T
 */
template<typename T>
class Matrix {

public:

    /**
     * @brief Default constructor.
     * @post
     *   GetNumberOfRows() == 0u &&
     *   GetNumberOfColumns() == 0u &&
     *   GetDataPointer() == NULL &&
     *   IsStaticDeclared()
     */
    Matrix();

    /**
     * @brief Constructs a new matrix with size: [nOfRows]x[nOfColumns]
     * @param[in] nOfRows The number of rows
     * @param[in] nOfColumns The number of columns
     * @post
     *   GetNumberOfRows() == nOfRows &&
     *   GetNumberOfColumns() == nOfColumns &&
     *   GetDataPointer() != NULL &&
     *   not IsStaticDeclared()
     */
    Matrix(uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * @brief Constructs a new matrix and associates it to an existent table with size: [nOfRows]x[nOfColumns]
     * @param[in] existingArray The pointer to the existing array
     * @param[in] nOfRows The number of rows
     * @param[in] nOfColumns The number of columns
     * @post
     *   GetNumberOfRows() == nOfRows &&
     *   GetNumberOfColumns() == nOfColumns &&
     *   GetDataPointer() == existingArray &&
     *   not IsStaticDeclared()
     */
    Matrix(T **existingArray,
           uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * @brief Constructs a new matrix and associates it to an existent table with size: [nOfRows]x[nOfColumns]
     * @param[in] existingArray The pointer to the existing array
     * @param[in] nOfRows The number of rows
     * @param[in] nOfColumns The number of columns
     * @post
     *   GetNumberOfRows() == nOfRows &&
     *   GetNumberOfColumns() == nOfColumns &&
     *   GetDataPointer() == existingArray &&
     *   IsStaticDeclared()
     */
    Matrix(T *existingArray,
           uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * @brief Constructs a new matrix from a statically declared table [][].
     * @tparam nOfRowsStatic Define nOfRowsStatic
     * @tparam nOfColumnsStatic Define nOfColumnsStatic
     * @param[in] source The address of the statically declared table.
     * @post
     *   GetNumberOfRows() == nOfRowsStatic &&
     *   GetNumberOfColumns() == nOfColumnsStatic &&
     *   GetDataPointer() == &source[0] &&
     *   IsStaticDeclared()
     */
    template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
    Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]);

    /**
     * @brief Destructor.
     * @details If IsStaticDeclared(), then it frees the memory pointed
     * by \a GetDataPointer().
     */
    ~Matrix();

    /**
     * @brief Gets the number of columns.
     * @return the number of columns.
     */
    inline uint32 GetNumberOfColumns() const;

    /**
     * @brief Gets the number of rows.
     * @return the number of rows.
     */
    inline uint32 GetNumberOfRows() const;

    /**
     * @brief Returns the vector associated to the specified row.
     * @param[in] rows The row to retrieve.
     * @return the vector associated to the specified row.
     */
    Vector<T> operator [](uint32 rows);

    /**
     * @brief Gets the data pointer associated to the raw matrix data.
     * @return the data pointer associated to the raw matrix data.
     */
    inline void *GetDataPointer() const;

    /**
     * @brief Checks if GetDataPointer() is pointing at a statically allocated matrix memory block [][].
     * @return true if GetDataPointer() is pointing at a statically allocated matrix memory block [][].
     */
    inline bool IsStaticDeclared() const;

    /**
     * @brief Performs the matrix product.
     * @param[in] factor is the matrix to be multiplied with.
     * @param[out] result is the matrix product result.
     * @return true if the dimensions of \a factor and \a result are correct, false otherwise.
     * @pre
     *   GetNumberOfColumns() == factor.GetNumberOfRows() &&
     *   GetNumberOfRows() == result.GetNumberOfRows() &&
     *   factor.GetNumberOfColumns() == result.GetNumberOfColumns()
     * @post
     *   result holds the product matrix between *this and factor
     */
    bool Product(Matrix<T> &factor,
                 Matrix<T> &result) const;

    /**
     * @brief Retrieves the sub matrix between the row and columns ranges specified.
     * @param[in] beginRow is the top boundary of the block.
     * @param[in] endRow is the bottom boundary of the block.
     * @param[in] beginColumn is the left boundary of the block.
     * @param[in] endColumn is the right boundary of the block.
     * @param[out] subMatrix is the desired sub matrix in output.
     * @return true if the preconditions are satisfied, false otherwise.
     * @pre
     *   beginRow <= endRow &&
     *   beginColumn <= endColumn &&
     *   endRow < GetNumberOfRows() &&
     *   endColumn < GetNumberOfColumns() &&
     *   subMatrix.GetNumberOfRows() == (endRows - beginRows) + 1 &&
     *   subMatrix.GetNumberOfColumns() == (endColumn - beginColumn) + 1
     * @post
     *   subMatrix holds the requested sub matrix from *this
     */
    bool SubMatrix(const uint32 beginRow,
                   const uint32 endRow,
                   const uint32 beginColumn,
                   const uint32 endColumn,
                   Matrix<T> &subMatrix) const;

    /**
     * @brief Retrieves the transpose of this matrix.
     * @param[out] transpose is the transpose matrix in output.
     * @return true if the preconditions are satisfied, false otherwise.
     * @pre
     *   GetNumberOfRows() == transpose.GetNumberOfColumns() &&
     *   GetNumberOfColumns() == transpose.GetNumberOfRows()
     * @post
     *   transpose holds the transpose matrix of *this
     */
    bool Transpose(Matrix<T> &transpose) const;

    /**
     * @brief Calculates the matrix determinant.
     * @details This operation is allowed only for float matrices.
     * @param[out] det is the calculated determinant in output.
     * @return true if the preconditions are satisfied, false otherwise.
     * @pre
     *   GetNumberOfRows() == GetNumberOfColumns() &&
     *   (typeid(T) == typeid(float32) || typeid(T) == typeid(float64))
     * @post
     *   det holds the determinant of *this
     */
    bool Determinant(T &det) const;

    /**
     * @brief Retrieves the inverse of this matrix.
     * @details This operations is allowed only for float matrices.
     * @param[out] inverse is the calculated inverse matrix in output.
     * @return true if the preconditions are satisfied, false otherwise.
     * @pre
     *   GetNumberOfRows() == GetNumberOfColumns() &&
     *   (typeid(T) == typeid(float32) || typeid(T) == typeid(float64)) &&
     *   this->Determinant(*) returns a determinant != 0.0 &&
     *   GetNumberOfRows() == inverse.GetNumberOfRows() &&
     *   GetNumberOfColumns() == inverse.GetNumberOfColumns()
     * @post
     *   inverse holds the inverse matrix of *this
     */
    bool Inverse(Matrix<T> &inverse) const;

private:

    /**
     * The data pointer to the raw data.
     */
    void *dataPointer;

    /**
     * The number of rows.
     */
    uint32 numberOfRows;

    /**
     * The number of columns.
     */
    uint32 numberOfColumns;

    /**
     * True if this dataPointer is pointing at a statically allocated matrix memory block [][].
     */
    bool staticDeclared;

    /**
     * True if the vector is allocated internally on heap and has to be destroyed by the destructor.
     */
    bool canDestroy;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename T>
Matrix<T>::Matrix() {
    dataPointer = static_cast<void *>(NULL);
    numberOfRows = 0u;
    numberOfColumns = 0u;
    staticDeclared = true;
    canDestroy = false;
}

template<typename T>
Matrix<T>::Matrix(uint32 nOfRows,
                  uint32 nOfColumns) {
    T** rows = new T*[nOfRows];
    dataPointer = reinterpret_cast<void *>(rows);
    numberOfColumns = nOfColumns;
    numberOfRows = nOfRows;
    staticDeclared = false;
    for (uint32 i = 0u; i < nOfRows; i++) {
        rows[i] = new T[nOfColumns];
    }
    canDestroy = true;
}

template<typename T>
Matrix<T>::Matrix(T **existingArray,
                  uint32 nOfRows,
                  uint32 nOfColumns) {
    dataPointer = reinterpret_cast<void *>(existingArray);
    numberOfColumns = nOfColumns;
    numberOfRows = nOfRows;
    staticDeclared = false;
    canDestroy = false;
}

template<typename T>
Matrix<T>::Matrix(T *existingArray,
                  uint32 nOfRows,
                  uint32 nOfColumns) {
    dataPointer = reinterpret_cast<void *>(existingArray);
    numberOfColumns = nOfColumns;
    numberOfRows = nOfRows;
    staticDeclared = true;
    canDestroy = false;
}

template<typename T>
template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
Matrix<T>::Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]) {
    dataPointer = reinterpret_cast<void*>(&(source[0][0]));
    numberOfColumns = nOfColumnsStatic;
    numberOfRows = nOfRowsStatic;
    staticDeclared = true;
    canDestroy = false;
}

template<typename T>
Matrix<T>::~Matrix() {
    if (canDestroy) {
        T** pointerToDestroy = reinterpret_cast<T**>(dataPointer);
        for (uint32 i = 0u; i < numberOfRows; i++) {
            delete[] pointerToDestroy[i];
        }
        delete[] pointerToDestroy;
    }
    dataPointer = NULL;
    numberOfColumns = 0u;
    numberOfRows = 0u;
}

template<typename T>
inline uint32 Matrix<T>::GetNumberOfColumns() const {
    return numberOfColumns;
}

template<typename T>
inline uint32 Matrix<T>::GetNumberOfRows() const {
    return numberOfRows;
}

template<typename T>
Vector<T> Matrix<T>::operator [](uint32 element) {
    Vector<T> vec;

    if (!staticDeclared) {
        T** rows = reinterpret_cast<T**>(dataPointer);
        vec = Vector<T>(rows[element], numberOfColumns);
    }
    else {
        T* beginMemory = reinterpret_cast<T*>(dataPointer);
        vec = Vector<T>(&beginMemory[element * numberOfColumns], numberOfColumns);
    }
    return vec;
}

template<typename T>
inline void* Matrix<T>::GetDataPointer() const {
    return dataPointer;
}

template<typename T>
inline bool Matrix<T>::IsStaticDeclared() const {
    return staticDeclared;
}

template<typename T>
bool Matrix<T>::Product(Matrix<T> &factor,
                        Matrix<T> &result) const {
    bool cond1 = (factor.numberOfRows == numberOfColumns);
    bool cond2 = (result.numberOfRows == numberOfRows);
    bool cond3 = (result.numberOfColumns == factor.numberOfColumns);
    bool ret = ((cond1) && (cond2) && (cond3));
    if (ret) {
        Matrix<T> temp;
        if (staticDeclared) {
            temp = Matrix<T>(static_cast<T*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix<T>(static_cast<T**>(dataPointer), numberOfRows, numberOfColumns);
        }
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                result[i][j] = static_cast<T>(0);
                for (uint32 k = 0u; k < numberOfColumns; k++) {
                    result[i][j] += temp[i][k] * factor[k][j];
                }
            }
        }
    }
    return ret;
}

template<typename T>
bool Matrix<T>::SubMatrix(const uint32 beginRow,
                          const uint32 endRow,
                          const uint32 beginColumn,
                          const uint32 endColumn,
                          Matrix<T> &subMatrix) const {
    bool cond1 = (endRow >= beginRow);
    bool cond2 = (endColumn >= beginColumn);
    bool cond3 = (endRow < numberOfRows);
    bool cond4 = (endColumn < numberOfColumns);

    bool ret = ((cond1) && (cond2) && (cond3) && (cond4));
    if (ret) {
        uint32 outputNRows = (endRow - beginRow) + 1u;
        uint32 outputNCols = (endColumn - beginColumn) + 1u;

        bool cond5 = (subMatrix.numberOfRows == (outputNRows));
        bool cond6 = (subMatrix.numberOfColumns == (outputNCols));
        ret = ((cond5) && (cond6));

        if (ret) {
            Matrix<T> temp;
            if (staticDeclared) {
                temp = Matrix<T>(static_cast<T*>(dataPointer), numberOfRows, numberOfColumns);
            }
            else {
                temp = Matrix<T>(static_cast<T**>(dataPointer), numberOfRows, numberOfColumns);
            }
            for (uint32 i = 0u; i < outputNRows; i++) {
                for (uint32 j = 0u; j < outputNCols; j++) {
                    uint32 rowIndex = (beginRow + i);
                    uint32 columnIndex = (beginColumn + j);
                    subMatrix[i][j] = temp[rowIndex][columnIndex];
                }
            }
        }
    }

    return ret;
}

template<typename T>
bool Matrix<T>::Transpose(Matrix<T> &transpose) const {

    bool cond1 = (numberOfRows == transpose.numberOfColumns);
    bool cond2 = (numberOfColumns == transpose.numberOfRows);

    bool ret = ((cond1) && (cond2));

    if (ret) {
        Matrix<T> temp;
        if (staticDeclared) {
            temp = Matrix<T>(static_cast<T*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix<T>(static_cast<T**>(dataPointer), numberOfRows, numberOfColumns);
        }
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                transpose[j][i] = temp[i][j];
            }
        }
    }

    return ret;
}

template<> inline
bool Matrix<float32>::Determinant(float32 &det) const {
    bool ret = (numberOfRows == numberOfColumns);

    if (ret) {
        Matrix<float32> temp;
        if (staticDeclared) {
            temp = Matrix<float32>(static_cast<float32*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix<float32>(static_cast<float32**>(dataPointer), numberOfRows, numberOfColumns);
        }
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

template<> inline
bool Matrix<float64>::Determinant(float64 &det) const {
    bool ret = (numberOfRows == numberOfColumns);

    if (ret) {
        Matrix<float64> temp;
        if (staticDeclared) {
            temp = Matrix<float64>(static_cast<float64*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix<float64>(static_cast<float64**>(dataPointer), numberOfRows, numberOfColumns);
        }
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

template<> inline
bool Matrix<float32>::Inverse(Matrix<float32> &inverse) const {
    bool cond1 = (numberOfColumns == numberOfRows);
    bool cond2 = (inverse.numberOfRows == numberOfRows);
    bool cond3 = (inverse.numberOfColumns == numberOfColumns);
    float32 determinant = 0.0f;

    bool cond4 = Determinant(determinant);
    bool cond5 = (determinant != 0.0f);
    bool ret = ((cond1) && (cond2) && (cond3) && (cond4) && (cond5));

    if (ret) {
        Matrix<float32> temp;
        if (staticDeclared) {
            temp = Matrix<float32>(static_cast<float32*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix<float32>(static_cast<float32**>(dataPointer), numberOfRows, numberOfColumns);
        }
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
                float32 sign = (((i + j) & (1u)) == 0u) ? (1.0) : (-1.0);
                inverse[j][i] = (subDet * sign) / determinant;
            }
        }
    }

    return ret;
}

template<> inline
bool Matrix<float64>::Inverse(Matrix<float64> &inverse) const {
    bool cond1 = (numberOfColumns == numberOfRows);
    bool cond2 = (inverse.numberOfRows == numberOfRows);
    bool cond3 = (inverse.numberOfColumns == numberOfColumns);
    float64 determinant = 0.0;

    bool cond4 = Determinant(determinant);
    bool cond5 = (determinant != 0.0);
    bool ret = ((cond1) && (cond2) && (cond3) && (cond4) && (cond5));

    if (ret) {
        Matrix<float64> temp;
        if (staticDeclared) {
            temp = Matrix<float64>(static_cast<float64*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix<float64>(static_cast<float64**>(dataPointer), numberOfRows, numberOfColumns);
        }
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

}

#endif /* MATRIX_H_ */

