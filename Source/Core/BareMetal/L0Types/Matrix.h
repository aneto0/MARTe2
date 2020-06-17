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
class Matrix: public Pointer {

public:

    /**
     * @brief Default constructor.
     * @post
     *   GetNumberOfRows() == 0u &&
     *   GetNumberOfColumns() == 0u &&
     *   GetDataPointer() == NULL
     */
    Matrix();

    /**
     * @brief Constructs a new matrix with size: [nOfRows]x[nOfColumns]
     * @param[in] nOfRows The number of rows
     * @param[in] nOfColumns The number of columns
     * @post
     *   GetNumberOfRows() == nOfRows &&
     *   GetNumberOfColumns() == nOfColumns &&
     *   GetDataPointer() != NULL
     */
    Matrix(uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * @brief initialises Matrix to point to the given memory area.
     * Use only as first initialisation as it will not check the allocation flag and try deallocating what the pointer addresses
     * Can be used to initialise matrices created in memory.
     * @param[in] existingArray The pointer to the existing array
     * @param[in] nOfRows The number of rows
     * @param[in] nOfColumns The number of columns
     * @param[in] allocationModeIn hows was the memory produced? malloc, new or static?
     * @post
     *   GetNumberOfRows() == nOfRows &&
     *   GetNumberOfColumns() == nOfColumns &&
     *   GetDataPointer() == existingArray
     */
    void InitMatrix(T *existingArray,
           uint32 nOfRows,
           uint32 nOfColumns,
		   AllocationMode allocationModeIn=heapStatic);

    /**
     * @brief Frees any existing memory and allocate enough to store nOfElements
     * if nOfElements is zero, memory is freed and pointer set to NULL
     * @param[in] nOfRows The number of rows
     * @param[in] nOfColumns The number of columns
     * @post
     *    GetNumberOfElements() == nOfElements &&
     *    GetDataPointer() == new allocated array
     */
    void SetSize(uint32 nOfRows,uint32 nOfColumns);

    /**
     * @brief Constructs a new matrix and associates it to an existent table with size: [nOfRows]x[nOfColumns]
     * @param[in] existingArray The pointer to the existing array
     * @param[in] nOfRows The number of rows
     * @param[in] nOfColumns The number of columns
     * @post
     *   GetNumberOfRows() == nOfRows &&
     *   GetNumberOfColumns() == nOfColumns &&
     *   GetDataPointer() == existingArray
     */
    Matrix(T *existingArray,
           uint32 nOfRows,
           uint32 nOfColumns);

    /**
     * @brief Constructs a new matrix by copying the information from an existing one.
     * memory deallocation responsibility is moved from toCopy to this
     * @param[in] toCopy the matrix to copy
     * @post
     *   GetNumberOfRows() 	  == toCopy.GetNumberOfRows() &&
     *   GetNumberOfColumns() == toCopy.GetNumberOfColumns() &&
     *   GetDataPointer()     == toCopy.GetDataPointer()
     */
    Matrix(Matrix<T> &toCopy);

    /**
     * @brief Constructs a new matrix from a statically declared table [][].
     * @tparam nOfRowsStatic Define nOfRowsStatic
     * @tparam nOfColumnsStatic Define nOfColumnsStatic
     * @param[in] source The address of the statically declared table.
     * @post
     *   GetNumberOfRows() == nOfRowsStatic &&
     *   GetNumberOfColumns() == nOfColumnsStatic &&
     *   GetDataPointer() == &source[0]
     */
    template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
    Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]);

    /**
     * @brief Destructor.
     * @details If canDestroy, then it frees the memory pointed
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
     * @brief Gets the total n of elements.
     * @return the total n of elements.
     */
    inline uint32 GetNumberOfElements() const;

    /**
     * @brief Returns the vector associated to the specified row.
     * @param[in] rows The row to retrieve.
     * @return the vector associated to the specified row.
     */
    const Vector<T> operator[](uint32 rows) const;

    /**
     * @brief Returns the vector associated to the specified row.
     * @param[in] rows The row to retrieve.
     * @return the vector associated to the specified row.
     */
    Vector<T> operator[](uint32 rows);

    /**
     * @brief Returns a reference to a cell indexed by row and column.
     * @param[in] row is the index for the row
     * @param[in] col is the index for the column
     * @pre
     *   row >= 0 && row < GetNumberOfRows() &&
     *   col >= 0 && col < GetNumberOfColumns()
     * @return a T& which can be used for reading/writing the cell.
     */
    T& operator()(const uint32 row, const uint32 col);

	
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
    bool Product(Matrix<T> &factor, Matrix<T> &result) const;

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
    bool Sum(Matrix<T> &addend, Matrix<T> &result) const;

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
    bool Copy(Matrix<T> const &matrixToCopy);

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

    /**
     */
//    inline Matrix<T> operator= (Matrix<T> toCopy);

private:


    /**
     * @brief Frees memory if necessary
     * @post
     *   GetDataPointer() == NULL
     */
    void FreeMemory();

    /**
     * The number of rows.
     */
    uint32 numberOfRows;

    /**
     * The number of columns.
     */
    uint32 numberOfColumns;

    /**
     * True if the vector is allocated internally on heap and has to be destroyed by the destructor.
     */
    AllocationMode allocationMode;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/*
template<typename T>
Matrix<T> Matrix<T>::operator= (Matrix<T> toCopy){
    numberOfColumns = toCopy.numberOfColumns;
    numberOfRows    = toCopy.numberOfRows;
    allocationMode  = toCopy.allocationMode;
    toCopy.allocationMode = heapStatic;

    return *this;
}
*/

template<typename T>
Matrix<T>::Matrix(): Pointer() {
    numberOfRows = 0u;
    numberOfColumns = 0u;
    allocationMode = heapStatic;
}

template<typename T>
Matrix<T>::Matrix(uint32 nOfRows,
                  uint32 nOfColumns): Pointer(new T[nOfRows*nOfColumns]) {
    numberOfColumns = nOfColumns;
    numberOfRows    = nOfRows;
    allocationMode = heapNew;
}

template<typename T>
Matrix<T>::Matrix(Matrix<T> &toCopy){
    numberOfColumns = toCopy.numberOfColumns;
    numberOfRows    = toCopy.numberOfRows;
    allocationMode  = toCopy.allocationMode;
    toCopy.allocationMode = heapStatic;
}


template<typename T>
void Matrix<T>::InitMatrix(T *existingArray,
                  uint32 nOfRows,
                  uint32 nOfColumns,
				  AllocationMode allocationModeIn) {
	// does not check and deallocate (FreeMemory) as this is called to perform first initialisation and memory holds random values
	Pointer::Set(existingArray);
    numberOfColumns = nOfColumns;
    numberOfRows 	= nOfRows;
    allocationMode 	= allocationModeIn;
}

template<typename T>
void Matrix<T>::SetSize(uint32 nOfRows,uint32 nOfColumns) {
	FreeMemory();
	if ((nOfRows > 0) && (nOfColumns > 0)){
		Pointer::Set(new T[nOfRows*nOfColumns]);
	    allocationMode = heapNew;
	} else {
		Pointer::Set(NULL_PTR(T*));
	    allocationMode = heapStatic;
	}
    numberOfColumns = nOfColumns;
    numberOfRows = nOfRows;
}


template<typename T>
Matrix<T>::Matrix(T *existingArray,
                  uint32 nOfRows,
                  uint32 nOfColumns):Pointer(existingArray) {
    numberOfColumns = nOfColumns;
    numberOfRows = nOfRows;
    allocationMode = heapStatic;
}

template<typename T>
template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
Matrix<T>::Matrix(T (&source)[nOfRowsStatic][nOfColumnsStatic]):Pointer(&(source[0][0])) {
    numberOfColumns = nOfColumnsStatic;
    numberOfRows 	= nOfRowsStatic;
    allocationMode = heapStatic;
}

template<typename T>
void Matrix<T>::FreeMemory() {
	if (dataPointer != NULL_PTR(T*)){
	    if (allocationMode == heapNew) {
	        delete[] reinterpret_cast<T*>(dataPointer);
	    }
	    if (allocationMode == heapMalloc) {
	    	char8 *p8 = reinterpret_cast<char8 *>(dataPointer);
	    	for (uint32 i = 0;i < numberOfColumns*numberOfRows;i++){
	    		T* pT = reinterpret_cast<T *>(p8);
	    		pT->~T();
	    		p8 += sizeof(T);
	    	}

	        delete(reinterpret_cast<char8 *>(dataPointer));
	    }
	}
    Pointer::Set(NULL);
    numberOfColumns = 0u;
    numberOfRows = 0u;
    allocationMode = heapStatic;
}

template<typename T>
Matrix<T>::~Matrix() {
	FreeMemory();
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
inline uint32 Matrix<T>::GetNumberOfElements() const {
    return numberOfRows * numberOfColumns;
}


template<typename T>
Vector<T> Matrix<T>::operator[](uint32 element) {
    Vector<T> vec;

    T* beginMemory = reinterpret_cast<T*>(dataPointer);
    vec = Vector<T>(&beginMemory[element * numberOfColumns], numberOfColumns);

    return vec;
}

template<typename T>
const Vector<T> Matrix<T>::operator[](uint32 element) const {
    Vector<T> vec;

    T* beginMemory = reinterpret_cast<T*>(dataPointer);
    vec = Vector<T>(&beginMemory[element * numberOfColumns], numberOfColumns);

    return vec;
}

template<typename T>
T& Matrix<T>::operator()(const uint32 row, const uint32 col) {
    T* mat = reinterpret_cast<T*>(dataPointer);
    return mat[row*numberOfColumns+col];
}

template<typename T>
bool Matrix<T>::Product(Matrix<T> &factor,
                        Matrix<T> &result) const {
    bool cond1 = (factor.numberOfRows == numberOfColumns);
    bool cond2 = (result.numberOfRows == numberOfRows);
    bool cond3 = (result.numberOfColumns == factor.numberOfColumns);
    bool ret = ((cond1) && (cond2) && (cond3));
    if (ret) {

        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < factor.numberOfColumns; j++) {
                result[i][j] = static_cast<T>(0);
                for (uint32 k = 0u; k < numberOfColumns; k++) {
                    result[i][j] += (*this)[i][k] * factor[k][j];
                }
            }
        }
    }
    return ret;
}

template<typename T>
bool Matrix<T>::Sum(Matrix<T> & addend, Matrix<T> &result) const {
    bool cond1 = (addend.numberOfRows == numberOfRows) && (result.numberOfRows == numberOfRows);
    bool cond2 = (addend.numberOfColumns == numberOfColumns) && (result.numberOfColumns == numberOfColumns);
    bool ret = (cond1 && cond2);
    if (ret) {

        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                result[i][j] = (*this)[i][j] + addend[i][j];
            }
        }
    }
    return ret;
}

template<typename T>
bool Matrix<T>::Copy(Matrix<T> const &matrixToCopy) {
    bool cond1 = (matrixToCopy.numberOfRows == numberOfRows);
    bool cond2 = (matrixToCopy.numberOfColumns == numberOfColumns);
    bool ret = cond1 && cond2;
    if (ret) {
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                (*this)[i][j] = matrixToCopy[i][j];
            }
        }
    }
    return ret;
}

template<typename T>
bool Matrix<T>::SubMatrix(const uint32 beginRow, const uint32 endRow, const uint32 beginColumn, const uint32 endColumn, Matrix<T> &subMatrix) const {
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
            for (uint32 i = 0u; i < outputNRows; i++) {
                for (uint32 j = 0u; j < outputNCols; j++) {
                    uint32 rowIndex = (beginRow + i);
                    uint32 columnIndex = (beginColumn + j);
                    subMatrix[i][j] = (*this)[rowIndex][columnIndex];
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
        for (uint32 i = 0u; i < numberOfRows; i++) {
            for (uint32 j = 0u; j < numberOfColumns; j++) {
                transpose[j][i] = (*this)[i][j];
            }
        }
    }

    return ret;
}
template<>
bool Matrix<float32>::Determinant(float32 &det) const ;

template<>
bool Matrix<float64>::Determinant(float64 &det) const ;

template<>
bool Matrix<float32>::Inverse(Matrix<float32> &inverse) const ;

template<>
bool Matrix<float64>::Inverse(Matrix<float64> &inverse) const ;



}

#endif /* MATRIX_H_ */

