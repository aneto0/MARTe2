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
 */
template<typename T>
class Matrix {
public:
//TODO
    Matrix();

    /**
     * @brief Constructs a new matrix with size: [nOfRows]x[nOfColumns]
     * @post
     *   GetNumberOfRows() == nOfRows &&
     *   GetNumberOfColumns() == nOfColumns &&
     *   GetDataPointer() != NULL &&
     *   IsStaticDeclared() == false
     */
    Matrix(uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * @brief Constructs a new matrix and associates it to an existent table with size: [nOfRows]x[nOfColumns]
     * @post
     *   GetNumberOfRows() == nOfRows &&
     *   GetNumberOfColumns() == nOfColumns &&
     *   GetDataPointer() == existingArray &&
     *   IsStaticDeclared() == false
     */
    Matrix(T **existingArray,
           uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * @brief Constructs a new matrix from a statically declared table [][].
     * @param[in] nOfRowsStatic number of rows in the table, automatically computed by the compiler.
     * @param[in] nOfColumnsStatic number of columns in the table, automatically computed by the compiler.
     * @param[in] source address of the statically declared table.
     * @post
     *   GetNumberOfRows() == nOfRowsStatic &&
     *   GetNumberOfColumns() == nOfColumnsStatic &&
     *   GetDataPointer() == &source[0] &&
     *   IsStaticDeclared == true
     */
    template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
    Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]);

    /**
     * @brief Destructor.
     * @details If the matrix is allocated internally, the memory will be freed.
     * @post
     *   numberOfRows == 0 &&
     *   numberOfColumns == 0 &&
     *   dataPointer == NULL;
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
     * @param[in] row the row to retrieve.
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
     *   numberOfColumns == factor.numberOfRows &&
     *   numberOfRows == result.numberOfRows &&
     *   factor.numberOfColumns == result.numberOfColumns;
     */
    bool Product(Matrix<T> factor,
                 Matrix<T> &result);

    bool SubMatrix(uint32 beginRow,
                   uint32 endRow,
                   uint32 beginColumn,
                   uint32 endColumn,
                   Matrix<T> &subMatrix);

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
    numberOfRows=0u;
    numberOfColumns=0u;
    staticDeclared=true;
    canDestroy=false;
}

template<typename T>
Matrix<T>::Matrix(uint32 nOfRows,
                  uint32 nOfColumns) {
    T** rows = new T*[nOfRows];
    dataPointer = reinterpret_cast<void *>(rows);
    numberOfColumns = nOfColumns;
    numberOfRows = nOfRows;
    staticDeclared = false;
    uint32 i;
    for (i = 0; i < nOfRows; i++) {
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
        for (uint32 i = 0; i < numberOfRows; i++) {
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
bool Matrix<T>::Product(Matrix<T> factor,
                        Matrix<T> &result) {
    bool cond1 = (factor.numberOfRows == numberOfColumns);
    bool cond2 = (result.numberOfRows == numberOfRows);
    bool cond3 = (result.numberOfColumns == factor.numberOfColumns);
    bool ret = ((cond1) && (cond2) && (cond3));
    if (ret) {
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                result[i][j] = static_cast<T>(0);
                for (uint32 k = 0u; k < numberOfColumns; k++) {
                    result[i][j] += ((*this)[i][k]) * factor[k][j];
                }
            }
        }
    }
    return ret;
}

template<typename T>
bool Matrix<T>::SubMatrix(uint32 beginRow,
                          uint32 endRow,
                          uint32 beginColumn,
                          uint32 endColumn,
                          Matrix<T> &subMatrix) {
    bool cond1 = (endRow > beginRow);
    bool cond2 = (endColumn > beginColumn);
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
            for (uint32 i = 0; i < outputNRows; i++) {
                for (uint32 j = 0; j < outputNCols; j++) {
                    uint32 rowIndex = (beginRow + i);
                    uint32 columnIndex = (beginColumn + j);
                    subMatrix[i][j] = ((*this)[rowIndex][columnIndex]);
                }
            }
        }
    }

    return ret;
}

}
#endif /*MATRIX_H_ */

