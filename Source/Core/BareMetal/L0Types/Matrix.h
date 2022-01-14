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
 * @tparam T the scalar type for the cells of the matrix
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
    Matrix(const uint32 nOfRows,
           const uint32 nOfColumns);

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
    Matrix(T **const existingArray,
           const uint32 nOfRows,
           const uint32 nOfColumns);

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
    Matrix(T *const existingArray,
           const uint32 nOfRows,
           const uint32 nOfColumns);

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
     * @brief Copy constructor. Makes a copy of that. The memory management depends on the that.canDestroy
     * @details First checks that the Matrix to copy is different from the destination matrix, then frees memory (if needed) and finally
     * copies the matrix based on that.canDestroy:
     *     - if that.canDestroy --> allocates memory and then copies the values. (set this.canDestroy = true)
     *     - if !that.canDestroy --> copies the that.dataPointer. (set this.canDestroy = false)
     * @param[in] that matrix to copy.
     * @post If the matrix to copy is correctly initialised then the new matrix meet the following postconditions:
     * <ul><li> this->dataPointer != NULL </li>
     * <li>this->numberOfRows = that.numberOfRow</li>
     * <li>this->numberOfColumns = that.numberOfColumns</li>
     * <li>this->canDestroy = that->canDestroy</li>
     * <li>this->staticDeclared = that->staticDeclared</li>
     * </ul>
     * If the matrix to copy dataPointer == NULL || numberOfElements == 0 || numberOfRows == 0, then the following postconditions are met:
     * <ul>
     * <li>this->dataPointer = NULL</li>
     * <li>this->numberOfRows = 0</li>
     * <li>numberOfColumns = 0</li>
     * <li>canDestroy = False</li>
     * <li>staticDeclared = True</li>
     * </ul>
     */
    Matrix(const Matrix<T> &that);

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
     * @details Checks if element < numberOfRows, then return the Vector associated with the row if exist
     * @param[in] element The row to retrieve.
     * @return the vector associated to the specified row if exist. Otherwise return a vector not initialised.
     * @pre Matrix dataPointer != NULL
     */
    Vector<T> operator[](const uint32 element);

    /**
     * @brief Copy assignment. Makes a copy of that. The memory management depends on the that.canDestroy
     * @details First checks that the Matrix to copy is different from the destination matrix, then frees memory (if needed) and finally
     * copies the matrix based on that.canDestroy:
     *     - if that.canDestroy --> allocates memory and then copies the values. (set this.canDestroy = true)
     *     - if !that.canDestroy --> copies the that.dataPointer. (set this.canDestroy = false)
     * If the origin and destination of the matrixes is the same, nothing is done.
     * @param[in] that matrix to copy.
     * @return The Matrix copied on succeed.
     * @post If the matrix to copy is correctly initialised  then the new matrix meet the following postconditions:
     * <ul><li> this->dataPointer != NULL </li>
     * <li>this->numberOfRows = that.numberOfRow</li>
     * <li>this->numberOfColumns = that.numberOfColumns</li>
     * <li>this->canDestroy = that.canDestroy</li>
     * <li>this->staticDeclared = that->saticDeclared</li>
     * </ul>
     * If the matrix to copy dataPointer == NULL || numberOfElements == 0 || numberOfRows == 0, then the following postconditions are met:
     * <ul>
     * <li>this->dataPointer = NULL</li>
     * <li>this->numberOfRows = 0</li>
     * <li>numberOfColumns = 0</li>
     * <li>canDestroy = False</li>
     * <li>staticDeclared = True</li>
     * </ul>
     */
    Matrix<T>& operator =(const Matrix<T> &that);
    /**
     * @brief Returns a reference to a cell indexed by row and column.
     * @param[in] row is the index for the row
     * @param[in] col is the index for the column
     * @pre
     *   row >= 0 && row < GetNumberOfRows() &&
     *   col >= 0 && col < GetNumberOfColumns()
     * @return a T& which can be used for reading/writing the cell.
     */
    T& operator()(const uint32 row,
                  const uint32 col);

    /**
     * @brief Gets the data pointer associated to the raw matrix data.
     * @return the data pointer associated to the raw matrix data.
     */
    inline void* GetDataPointer();

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
     * @brief Performs the matrix sum.
     * @param[in] addend is the matrix to be summed with.
     * @param[out] result is the matrix sum result.
     * @return true if the matrix dimensions are consistent
     * @pre
     *   GetNumberOfColumns() == addend.GetNumberOfRows() &&
     *   GetNumberOfRows() == addend.GetNumberOfRows() &&
     *   GetNumberOfColumns() == result.GetNumberOfRows() &&
     *   GetNumberOfRows() == result.GetNumberOfRows() &&
     * @post
     *   result holds the matrix sum between *this and addend.
     */
    bool Sum(Matrix<T> &addend,
             Matrix<T> &result) const;

    /**
     * @brief Performs the matrix copy.
     * @param[in] matrixToCopy is the matrix to be copied.
     * @return true if the matrix dimensions are consistent
     * @pre
     *   GetNumberOfColumns() == matrixToCopy.GetNumberOfRows() &&
     *   GetNumberOfRows() == matrixToCopy.GetNumberOfRows() &&
     * @post
     *   *this[i][j] = matrixToCopy[i][j]
     */
    bool Copy(Matrix<T> &matrixToCopy);

    /**
     * @brief Retrieves the sub matrix between the row and columns ranges ==23886==    at 0x4BF05E: MARTe::Matrix<int>::Product(MARTe::Matrix<int>&, MARTe::Matrix<int>&) const (Matrix.h:398)
     * specified.
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
     * @tparam T the matrix type.
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
     * @param[in, out] inverse is the calculated inverse matrix in output.
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

    /**
     * @brief Frees memory if it was internally allocated.
     * @post
     * <ul>
     * <li>dataPointer = NULL_PTR(void*)</li>
     * <li>numberOfColumns = 0u;</li>
     * <li>numberOfRows = 0u;</li>
     * <li>canDestroy = false;</li>
     * </ul>
     */
    void FreeMemory();

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename T>
Matrix<T>::Matrix() {
    dataPointer = static_cast<void*>(NULL);
    numberOfRows = 0u;
    numberOfColumns = 0u;
    staticDeclared = true;
    canDestroy = false;
}

template<typename T>
Matrix<T>::Matrix(const uint32 nOfRows,
                  const uint32 nOfColumns) {
    T **rows = new T*[nOfRows];
    //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: Operation needed to copy T type to void type
    dataPointer = reinterpret_cast<void*>(rows);
    numberOfColumns = nOfColumns;
    numberOfRows = nOfRows;
    staticDeclared = false;
    for (uint32 i = 0u; i < nOfRows; i++) {
        //lint -e{613} Possible use of null pointer 'rows' in left argument to operator '['. Justification: The memory allocated above
        rows[i] = new T[nOfColumns];
    }
    canDestroy = true;
}

template<typename T>
Matrix<T>::Matrix(T **const existingArray,
                  const uint32 nOfRows,
                  const uint32 nOfColumns) {
    //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: Operation needed to convert from T** to void *
    dataPointer = reinterpret_cast<void*>(existingArray);
    numberOfColumns = nOfColumns;
    numberOfRows = nOfRows;
    staticDeclared = false;
    canDestroy = false;
}

template<typename T>
Matrix<T>::Matrix(T *const existingArray,
                  const uint32 nOfRows,
                  const uint32 nOfColumns) {
    //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: Operation needed to convert from T* to void *
    dataPointer = reinterpret_cast<void*>(existingArray);
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
Matrix<T>::Matrix(const Matrix<T> &that) {
    if (that.dataPointer != NULL_PTR(void*) && (that.numberOfColumns > 0u) && (that.numberOfRows > 0u)) {
        this->numberOfColumns = that.numberOfColumns;
        this->numberOfRows = that.numberOfRows;
        if (that.IsStaticDeclared()) {
            if (that.canDestroy) {// No matrix constructor allows IsStaticDeclared() == true && canDestroy == true, therefore this copy constructor never falls here
                T *auxPointer = reinterpret_cast<T*>(that.dataPointer);
                T **rows = new T*[this->numberOfRows];
                //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: Operation needed to copy T type to void type
                dataPointer = reinterpret_cast<void*>(rows);

                for (uint32 i = 0u; i < this->numberOfRows; i++) {
                    //lint -e{613} Possible use of null pointer 'rows' in left argument to operator '['. Justification: The memory allocated above
                    rows[i] = new T[this->numberOfColumns];
                    for (uint32 c = 0u; c < this->numberOfColumns; c++) {
                        rows[i][c] = auxPointer[i * numberOfColumns + c];
                    }
                }
                this->canDestroy = true;
            }
            else {
                this->canDestroy = false;
                //lint -e{1555} Direct pointer copy. Justification: that matrix does not manage its memory neither the copy.
                this->dataPointer = that.dataPointer;
            }
            staticDeclared = true;
        }
        else {
            if (that.canDestroy) {
                T **auxPointer = reinterpret_cast<T**>(that.dataPointer);
                T **rows = new T*[this->numberOfRows];
                //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: Operation needed to copy T type to void type
                dataPointer = reinterpret_cast<void*>(rows);

                for (uint32 i = 0u; i < this->numberOfRows; i++) {
                    //lint -e{613} Possible use of null pointer 'rows' in left argument to operator '['. Justification: The memory allocated above
                    rows[i] = new T[this->numberOfColumns];
                    for (uint32 c = 0u; c < this->numberOfColumns; c++) {
                        rows[i][c] = auxPointer[i][c];
                    }
                }
                this->canDestroy = true;
            }
            else {
                this->canDestroy = false;
                //lint -e{1555} Direct pointer copy. Justification: that matrix does not manage its memory neither the copy.
                this->dataPointer = that.dataPointer;
            }
            staticDeclared = false;
        }
    }
    else {
        this->numberOfColumns = 0u;
        this->numberOfRows = 0u;
        this->dataPointer = NULL_PTR(void*);
        this->staticDeclared = true;
        this->canDestroy = false;
    }
}

template<typename T>
Matrix<T>::~Matrix() {
    //lint -e{1551} Function may throw exception '...' in destructor. Justification FreeMemory cannot throw an exception
    FreeMemory();
    //lint -e{1579} Pointer member 'MARTe::Matrix<float>::dataPointer' (line 332) might have been freed by a separate function. Justification. FreeMemory frees memory if was internally allocated
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
Vector<T> Matrix<T>::operator[](const uint32 element) {
    Vector<T> vec;
    if (element < numberOfRows) {
        //lint -e{613} Possible use of null pointer 'rows' in left argument to operator. Justification: in the documentation the precondition is dataPointer != NULL.
        if (!staticDeclared) {
            //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: needed to copy from void* to T**
            T **rows = reinterpret_cast<T**>(dataPointer);
            vec = Vector<T>(rows[element], numberOfColumns);
        }
        else {
            //lint -e{613} Possible use of null pointer 'rows' in left argument to operator. Justification: in the documentation the precondition is dataPointer != NULL.
            //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: needed to copy from void* to T*
            T *beginMemory = reinterpret_cast<T*>(dataPointer);
            uint32 index = element * numberOfColumns;
            vec = Vector<T>(&beginMemory[index], numberOfColumns);
        }
    }
    return vec;
}
template<typename T>
Matrix<T>& Matrix<T>::operator =(const Matrix<T> &that) {
    if (this != &that) {
        this->FreeMemory();
        if ((that.dataPointer != NULL_PTR(void*)) && (that.numberOfColumns > 0u) && (that.numberOfRows > 0u)) {
            this->numberOfColumns = that.numberOfColumns;
            this->numberOfRows = that.numberOfRows;
            //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: Cast from pointer to pointer needed.
            if (that.IsStaticDeclared()) {
                if (that.canDestroy) { // No matrix constructor allows IsStaticDeclared() == true && canDestroy == true, therefore this copy assignment never falls here
                    T *auxPointer = reinterpret_cast<T*>(that.dataPointer);
                    T **rows = new T*[this->numberOfRows];
                    //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: Operation needed to copy T type to void type
                    dataPointer = reinterpret_cast<void*>(rows);
                    for (uint32 i = 0u; i < this->numberOfRows; i++) {
                        //lint -e{613} Possible use of null pointer 'rows' in left argument to operator '['. Justification: The memory allocated above
                        rows[i] = new T[this->numberOfColumns];
                        for (uint32 c = 0u; c < this->numberOfColumns; c++) {
                            uint32 auxIdx = (i * numberOfColumns) + c;
                            rows[i][c] = auxPointer[auxIdx];
                        }
                    }
                    canDestroy = true;
                }
                else {
                    this->canDestroy = false;
                    //lint -e{1555} Direct pointer copy. Justification: that matrix does not manage its memory neither the copy.
                    this->dataPointer = that.dataPointer;
                }
                staticDeclared = true;
            }
            else {
                if (that.canDestroy) {
                    T **auxPointer = reinterpret_cast<T**>(that.dataPointer);
                    T **rows = new T*[this->numberOfRows];
                    //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: Operation needed to copy T type to void type
                    dataPointer = reinterpret_cast<void*>(rows);

                    for (uint32 i = 0u; i < this->numberOfRows; i++) {
                        //lint -e{613} Possible use of null pointer 'rows' in left argument to operator '['. Justification: The memory allocated above
                        rows[i] = new T[this->numberOfColumns];
                        for (uint32 c = 0u; c < this->numberOfColumns; c++) {
                            rows[i][c] = auxPointer[i][c];
                        }
                    }
                    canDestroy = true;
                }
                else {
                    this->canDestroy = false;
                    //lint -e{1555} Direct pointer copy. Justification: that matrix does not manage its memory neither the copy.
                    this->dataPointer = that.dataPointer;
                }
                staticDeclared = false;
            }

        }
        else {
            this->numberOfColumns = 0u;
            this->numberOfRows = 0u;
            this->dataPointer = NULL_PTR(void*);
            this->staticDeclared = true;
            this->canDestroy = false;
        }
    }
    return *this;
}

template<typename T>
T& Matrix<T>::operator()(const uint32 row,
                         const uint32 col) {
    T *result;
    if (!staticDeclared) {
        T **mat = reinterpret_cast<T**>(dataPointer);
        result = &mat[row][col];
    }
    else {
        T *mat = reinterpret_cast<T*>(dataPointer);
        T *line = &mat[row * numberOfColumns];
        result = &line[col];
    }
    return (*result);
}

template<typename T>
inline void* Matrix<T>::GetDataPointer() {
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
            for (uint32 j = 0u; j < factor.numberOfColumns; j++) {
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
bool Matrix<T>::Sum(Matrix<T> &addend,
                    Matrix<T> &result) const {
    bool cond1 = (addend.numberOfRows == numberOfRows) && (result.numberOfRows == numberOfRows);
    bool cond2 = (addend.numberOfColumns == numberOfColumns) && (result.numberOfColumns == numberOfColumns);
    bool ret = (cond1 && cond2);
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
                result[i][j] = temp[i][j] + addend[i][j];
            }
        }
    }
    return ret;
}

template<typename T>
bool Matrix<T>::Copy(Matrix<T> &matrixToCopy) {
    bool cond1 = (matrixToCopy.numberOfRows == numberOfRows);
    bool cond2 = (matrixToCopy.numberOfColumns == numberOfColumns);
    bool ret = cond1 && cond2;
    if (ret) {
        if (staticDeclared) {
            T *temp = reinterpret_cast<T*>(dataPointer);
            for (uint32 i = 0u; i < numberOfRows; i++) {
                for (uint32 j = 0u; j < numberOfColumns; j++) {
                    temp[i * numberOfColumns + j] = matrixToCopy[i][j];
                }
            }
        }
        else {
            T **temp = reinterpret_cast<T**>(dataPointer);
            for (uint32 i = 0u; i < numberOfRows; i++) {
                for (uint32 j = 0u; j < numberOfColumns; j++) {
                    temp[i][j] = matrixToCopy[i][j];
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

/**
 * @brief float32 implementation of the Determinant.
 * @param[out] det see Matrix<T>::Determinant.
 * @return see Matrix<T>::Determinant.
 */
template<> inline
bool Matrix<float32>::Determinant(float32 &det) const {
    bool ret = (numberOfRows == numberOfColumns);

    if (ret) {
        Matrix < float32 > temp;
        //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: The constructor shall be invoked with he float32 * or float32** to satisfy the programmer expectation
        if (staticDeclared) {
            temp = Matrix < float32 > (static_cast<float32*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix < float32 > (static_cast<float32**>(dataPointer), numberOfRows, numberOfColumns);
        }
        if (numberOfRows == 1u) {
            //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
            det = temp[0u][0u];
        }
        else {
            det = 0.0F;
            // loop on the first row
            for (uint32 k = 0u; k < numberOfColumns; k++) {
                Matrix < float32 > subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
                uint32 n = 0u;
                for (uint32 i = 1u; i < numberOfRows; i++) {
                    uint32 m = 0u;
                    for (uint32 j = 0u; j < numberOfColumns; j++) {
                        if (j != k) {
                            //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
                            subMatrix[n][m] = temp[i][j];
                            m++;
                        }
                    }
                    n++;
                }
                float32 subDet = 0.0F;
                //lint -e{534} Ignoring return value of function 'MARTe::Matrix<float>::Determinant(float &) const' (compare with line 297) [MISRA C++ Rule 0-1-7], [MISRA C++ Rule 0-3-2]. Justification: the return value depends on the preconditions. The precondition is already check at the beginingof the function (ret = (numberOfRows == numberOfColumns)). Therefore the return always is true.
                subMatrix.Determinant(subDet);
                float32 sign = (((k) & (1u)) == 0u) ? (1.0F) : (-1.0F);
                //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
                det += (sign * temp[0u][k]) * subDet;
            }
        }
    }

    return ret;
}

/**
 * @brief float32 implementation of the Determinant.
 * @param[out] det see Matrix<T>::Determinant.
 * @return see Matrix<T>::Determinant.
 */
template<> inline
bool Matrix<float64>::Determinant(float64 &det) const {
    bool ret = (numberOfRows == numberOfColumns);

    if (ret) {
        Matrix < float64 > temp;
        //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: The constructor shall be invoked with he float32 * or float32** to satisfy the programmer expectation
        if (staticDeclared) {
            temp = Matrix < float64 > (static_cast<float64*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix < float64 > (static_cast<float64**>(dataPointer), numberOfRows, numberOfColumns);
        }
        if (numberOfRows == 1u) {
            //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
            det = temp[0u][0u];
        }
        else {
            det = 0.0;
            // loop on the first row
            for (uint32 k = 0u; k < numberOfColumns; k++) {
                Matrix < float64 > subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
                uint32 n = 0u;
                for (uint32 i = 1u; i < numberOfRows; i++) {
                    uint32 m = 0u;
                    for (uint32 j = 0u; j < numberOfColumns; j++) {
                        if (j != k) {
                            //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
                            subMatrix[n][m] = temp[i][j];
                            m++;
                        }
                    }
                    n++;
                }
                float64 subDet = 0.0;
                //lint -e{534} Ignoring return value of function 'MARTe::Matrix<float>::Determinant(float &) const' (compare with line 297) [MISRA C++ Rule 0-1-7], [MISRA C++ Rule 0-3-2]. Justification: the return value depends on the preconditions. The precondition is already check at the beginingof the function (ret = (numberOfRows == numberOfColumns)). Therefore the return always is true.
                subMatrix.Determinant(subDet);
                float64 sign = (((k) & (1u)) == 0u) ? (1.0) : (-1.0);
                //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
                det += (sign * temp[0u][k]) * subDet;
            }
        }
    }
    return ret;
}

/**
 * @brief float32 implementation of the Inverse.
 * @param[out] inverse see Matrix<T>::Inverse.
 * @return see Matrix<T>::Inverse.
 */
template<> inline
bool Matrix<float32>::Inverse(Matrix<float32> &inverse) const {
    bool cond1 = (numberOfColumns == numberOfRows);
    bool cond2 = (inverse.numberOfRows == numberOfRows);
    bool cond3 = (inverse.numberOfColumns == numberOfColumns);
    float32 determinant = 0.0F;

    bool cond4 = Determinant(determinant);
    //lint -e{9137} floating point test for equality or inequality [MISRA C++ Rule 6-2-2]. Justification: the operation can be performed if determinant is exactly different from 0.0.
    bool cond5 = (determinant != 0.0F);
    bool ret = ((cond1) && (cond2) && (cond3) && (cond4) && (cond5));

    if (ret) {
        Matrix < float32 > temp;
        //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: The constructor shall be invoked with he float32 * or float32** to satisfy the programmer expectation
        if (staticDeclared) {
            temp = Matrix < float32 > (static_cast<float32*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix < float32 > (static_cast<float32**>(dataPointer), numberOfRows, numberOfColumns);
        }
        Matrix < float32 > subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                //Matrix<float32> subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
                uint32 n = 0u;
                for (uint32 h = 0u; h < numberOfRows; h++) {
                    uint32 m = 0u;
                    if (h != i) {
                        for (uint32 k = 0u; k < numberOfColumns; k++) {
                            if (k != j) {
                                //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
                                subMatrix[n][m] = temp[h][k];
                                m++;
                            }
                        }
                        n++;
                    }
                }
                float32 subDet = 0.0F;
                //lint -e{534} Ignoring return value of function 'MARTe::Matrix<float>::Determinant(float &) const' (compare with line 297) [MISRA C++ Rule 0-1-7], [MISRA C++ Rule 0-3-2]. Justification: the return value depends on the preconditions. The preconditions are check above(numberOfColumns == numberOfRows), therefore always return true;
                subMatrix.Determinant(subDet);
                float32 sign = (((i + j) & (1u)) == 0u) ? (1.0F) : (-1.0F);
                //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
                inverse[j][i] = (subDet * sign) / determinant;
            }
        }
    }

    return ret;
}

/**
 * @brief float64 implementation of the Inverse.
 * @param[out] inverse see Matrix<T>::Inverse.
 * @return see Matrix<T>::Inverse.
 */
template<> inline
bool Matrix<float64>::Inverse(Matrix<float64> &inverse) const {
    bool cond1 = (numberOfColumns == numberOfRows);
    bool cond2 = (inverse.numberOfRows == numberOfRows);
    bool cond3 = (inverse.numberOfColumns == numberOfColumns);
    float64 determinant = 0.0;

    bool cond4 = Determinant(determinant);
    //lint -e{9137} floating point test for equality or inequality [MISRA C++ Rule 6-2-2]. Justification: the operation can be performed if determinant is exactly different from 0.0.
    bool cond5 = (determinant != 0.0);
    bool ret = ((cond1) && (cond2) && (cond3) && (cond4) && (cond5));

    if (ret) {
        Matrix < float64 > temp;
        //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: The constructor shall be invoked with he float64 * or float64** to satisfy the programmer expectation
        if (staticDeclared) {
            temp = Matrix < float64 > (static_cast<float64*>(dataPointer), numberOfRows, numberOfColumns);
        }
        else {
            temp = Matrix < float64 > (static_cast<float64**>(dataPointer), numberOfRows, numberOfColumns);
        }
        Matrix < float64 > subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                //Matrix<float64> subMatrix(numberOfRows - 1u, numberOfColumns - 1u);
                uint32 n = 0u;
                for (uint32 h = 0u; h < numberOfRows; h++) {
                    uint32 m = 0u;
                    if (h != i) {
                        for (uint32 k = 0u; k < numberOfColumns; k++) {
                            if (k != j) {
                                //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
                                subMatrix[n][m] = temp[h][k];
                                m++;
                            }
                        }
                        n++;
                    }
                }
                float64 subDet = 0.0;
                //lint -e{534} Ignoring return value of function 'MARTe::Matrix<float>::Determinant(float &) const' (compare with line 297) [MISRA C++ Rule 0-1-7], [MISRA C++ Rule 0-3-2]. Justification: the return value depends on the preconditions. The precondition is already check at the beginning of the function (ret = (numberOfRows == numberOfColumns)). Therefore the return always is true.
                subMatrix.Determinant(subDet);
                float64 sign = (((i + j) & (1u)) == 0u) ? (1.0) : (-1.0);
                //lint -e{1793} While calling 'MARTe::Vector<float>::operator[](unsigned int)': Initializing the implicit object parameter 'MARTe::Vector<float> &' (a non-const reference) with a non-lvalue (a temporary object of type 'MARTe::Vector<float>')
                inverse[j][i] = (subDet * sign) / determinant;
            }
        }
    }
    return ret;
}

template<typename T>
void Matrix<T>::FreeMemory() {
    if (canDestroy) {
        //lint -e{925} cast from pointer to pointer [MISRA C++ Rule 5-2-8], [MISRA C++ Rule 5-2-9]. Justification: needed to copy from void* to T**
        T **pointerToDestroy = reinterpret_cast<T**>(dataPointer);
        if (pointerToDestroy != NULL_PTR(T**)) {
            for (uint32 i = 0u; i < numberOfRows; i++) {
                if (pointerToDestroy[i] != NULL_PTR(T*)) {
                    delete[] pointerToDestroy[i];
                    pointerToDestroy[i] = NULL_PTR(T*);
                }
            }
            delete[] pointerToDestroy;
        }
    }
    dataPointer = NULL_PTR(void*);
    numberOfColumns = 0u;
    numberOfRows = 0u;
    canDestroy = false;
}

}

#endif /* MATRIX_H_ */

