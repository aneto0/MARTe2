/**
 * @file Vector.h
 * @brief Header file for class Vector
 * @date 04/11/2015
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class Vector
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef VECTOR_H_
#define VECTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HeapManager.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Fixed size array of values.
 * @details The Vector can allocate and manage its own memory or it can be
 * associated to an existent memory array.
 * @tparam T the scalar type for the cells of the matrix
 */
template<typename T>
class Vector {

public:

    /**
     * @brief Default constructor
     * @post
     *    GetNumberOfElements() == 0u &&
     *    GetDataPointer() == NULL &&
     *    IsStaticDeclared()
     */
    Vector();

    /**
     * @brief Copy constructor
     * @details Creates a copy of that. The copy mechanism depends on that.canDestroy, as follow:
     * <ul>
     * <li>If that.canDestroy = true, the copied vector allocates its own memory, copy the values from that and sets this.canDestroy = true</li>
     * <li>if that.canDestroy = false, this.canDestroy = false and this.GetDataPointer() = that.GetDataPointer()</li>
     * </ul>
     * @param[in] that is the object to copy
     * @return the copied Vector
     */
    Vector(const Vector<T> &that);

    /**
     * @brief Constructs a new vector with a given size
     * @param[in] nOfElements The number of elements of the vector
     * @post
     *    GetNumberOfElements() == nOfElements &&
     *    GetDataPointer() != NULL &&
     *    not IsStaticDeclared()
     */
    Vector(uint32 nOfElements);

    /**
     * @brief Constructs a new vector and associates it to an existent
     * array with a given size.
     * @param[in] existingArray The pointer to the existing array
     * @param[in] nOfElements The number of elements of the vector
     * @post
     *    GetNumberOfElements() == nOfElements &&
     *    GetDataPointer() == existingArray &&
     *    not IsStaticDeclared()
     */
    Vector(T *existingArray,
           uint32 nOfElements);

    /**
     * @brief Frees any existing memory and allocate enough to store nOfElements
     * if nOfElements is zero, memory is freed and pointer set to NULL
     * @param[in] nOfElements The number of elements of the vector
     * @post
     *    GetNumberOfElements() == nOfElements &&
     *    GetDataPointer() == new allocated array
     */
    void SetSize(uint32 nOfElements);

    /**
     * @brief Constructs a new matrix from a statically declared table [].
     * @tparam nOfElementsStatic Define nOfElementsStatic
     * @param[in] source The address of the statically declared table.
     * @post
     *   GetNumberOfElements() == nOfElementsStatic &&
     *   GetDataPointer() == &source[0] &&
     *   IsStaticDeclared
     */
    template<uint32 nOfElementsStatic>
    Vector(T (&source)[nOfElementsStatic]);

    /**
     * @brief Destructor.
     * @details If IsStaticDeclared(), then it frees the memory pointed
     * by \a GetDataPointer().
     */
    ~Vector();

    /**
     * @brief Returns the element at position \a idx.
     * @param[in] idx The index of the element to retrieve.
     * @return the element at position \a idx.
     */
    T& operator [](uint32 idx);

    /**
     * @brief Makes a copy of the Vector
     * @details Creates a copy of the Vector that. First, it frees the current vector memory(if needed) and then copies the vector based on that.canDestroy:
     * <ul>
     * <li>If that.canDestroy = true, the copied vector allocates its own memory, copy the values from that and sets this.canDestroy = true</li>
     * <li>if that.canDestroy = false, this.canDestroy = false and this.GetDataPointer() = that.GetDataPointer()</li>
     * </ul>
     * @param[in] that is the Vector to copy
     * @return the copied Vector
     *
     * @warning if the vector to copy from has a NULL pointer or numberOfElements = 0, the old information of the destination vector is destroyed (if it was internally allocated) but the dataPointer is not initialised.
     */
    Vector<T>& operator =(const Vector<T> &that);

    /**
     * @brief Gets the data pointer associated to the raw matrix data.
     * @return the data pointer associated to the raw matrix data.
     */
    inline void* GetDataPointer() const;

    /**
     * @brief Checks if GetDataPointer() is pointing at a statically allocated array memory block [].
     * @return true if GetDataPointer() is pointing at a statically allocated array memory block [].
     */
    inline bool IsStaticDeclared() const;

    /**
     * @brief Gets the number of elements in the vector.
     * @return the number of elements in the vector.
     */
    inline uint32 GetNumberOfElements() const;

    /**
     * @brief Performs the vector scalar product.
     * @param[in] factor is the vector to be multiplied with.
     * @param[out] result is the result of the scalar product of this vector multiplied with \a factor.
     * @return true if \a factor has the same number of elements of this vector, false otherwise.
     * @pre
     *   numberOfElements == factor.numberOfElements;
     * @post
     *   result holds the result of the scalar product of this vector multiplied with \a factor
     */
    bool Product(Vector<T> factor,
                 T &result) const;

private:
    /**
     * @brief Frees memory if necessary
     * @post
     *   GetDataPointer() == NULL
     */
    void FreeMemory();

    /**
     * The data pointer to the raw data.
     */
    void *dataPointer;

    /**
     * The number of elements.
     */
    uint32 numberOfElements;

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
Vector<T>::Vector() {
    numberOfElements = 0u;
    dataPointer = NULL_PTR(T*);
    canDestroy = false;
}

template<typename T>
Vector<T>::Vector(const Vector<T> &that) {
    this->numberOfElements = that.GetNumberOfElements();
    if ((numberOfElements > 0u) && (that.GetDataPointer() != NULL_PTR(void*))) {
        if (that.canDestroy) {
            this->dataPointer = new T[that.numberOfElements];
            this->canDestroy = true;
            T *arrayD = reinterpret_cast<T*>(this->GetDataPointer());
            T *arrayO = reinterpret_cast<T*>(that.GetDataPointer());
            for (uint32 i = 0u; i < this->numberOfElements; i++) {
                arrayD[i] = arrayO[i];
            }
        }
        else {
            this->canDestroy = false;
            this->dataPointer = that.GetDataPointer();
        }
    }
}

template<typename T>
Vector<T>::Vector(uint32 nOfElements) {
    dataPointer = new T[nOfElements];
    numberOfElements = nOfElements;
    canDestroy = true;
}

template<typename T>
Vector<T>::Vector(T *existingArray,
                  uint32 nOfElements) {
    dataPointer = existingArray;
    numberOfElements = nOfElements;
    canDestroy = false;
}

template<typename T>
void Vector<T>::SetSize(uint32 nOfElements) {
    FreeMemory();
    if (nOfElements > 0) {
        dataPointer = new T[nOfElements];
        canDestroy = true;
    }
    else if (nOfElements == 0) {
        dataPointer = NULL_PTR(T*);
        canDestroy = false;
    }
    numberOfElements = nOfElements;
}

template<typename T>
template<uint32 nOfElementsStatic>
Vector<T>::Vector(T (&source)[nOfElementsStatic]) {
    dataPointer = reinterpret_cast<T*>(&source[0]);
    numberOfElements = nOfElementsStatic;
    canDestroy = false;
}

template<typename T>
Vector<T>::~Vector() {
    FreeMemory();
}

template<typename T>
T& Vector<T>::operator [](uint32 idx) {
    T *array = reinterpret_cast<T*>(dataPointer);
    return array[idx];
}

template<typename T>
Vector<T>& Vector<T>::operator =(const Vector<T> &that) {
    this->FreeMemory();
    this->numberOfElements = that.GetNumberOfElements();
    if ((numberOfElements > 0u) && (that.GetDataPointer() != NULL_PTR(void*))) {
        if (that.canDestroy) {
            this->dataPointer = new T[that.numberOfElements];
            this->canDestroy = true;
            T *arrayD = reinterpret_cast<T*>(this->GetDataPointer());
            T *arrayO = reinterpret_cast<T*>(that.GetDataPointer());
            for (uint32 i = 0u; i < this->numberOfElements; i++) {
                arrayD[i] = arrayO[i];
            }
        }
        else {
            this->canDestroy = false;
            this->dataPointer = that.GetDataPointer();
        }
    }
    return *this;
}

template<typename T>
inline void* Vector<T>::GetDataPointer() const {
    return dataPointer;
}

template<typename T>
inline uint32 Vector<T>::GetNumberOfElements() const {
    return numberOfElements;
}

template<typename T>
bool Vector<T>::Product(Vector<T> factor,
                        T &result) const {
    bool ret = (factor.numberOfElements == numberOfElements);
    if (ret) {
        result = static_cast<T>(0);
        T *array = reinterpret_cast<T*>(dataPointer);
        for (uint32 i = 0u; i < numberOfElements; i++) {
            result += array[i] * factor[i];
        }
    }
    return ret;
}

template<typename T>
void Vector<T>::FreeMemory() {
    if (canDestroy) {
        delete[] reinterpret_cast<T*>(dataPointer);
        dataPointer = NULL;
        numberOfElements = 0;
        canDestroy = false;
    }
}

}

#endif /* VECTOR_H_ */

