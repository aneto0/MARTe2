/**
 * @file ManagedZeroTerminatedArray.h
 * @brief Header file for class ZeroTerminatedArray
 * @date 22/09/2015
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

 * @details This header file contains the declaration of the class ZeroTerminatedArray
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MANAGEDZEROTERMINATEDARRAY_H_
#define MANAGEDZEROTERMINATEDARRAY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "ZeroTerminatedArray.h"
#include "HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 * @brief Describes a zero-terminated array.
 *
 * @warning This class is only a wrapper of a pointer. The implementation assumes that the pointer
 * in input is a zero-terminated array and does not check if it is not zero-terminated or NULL.
 * If this pre-condition is not accomplished, a segmentation fault in runtime could happen.
 */
template<typename T>
class ManagedZeroTerminatedArray: protected ZeroTerminatedArray<T> {
public:

    /**
     * @brief Default constructor. pointer is set  to zero.
     */
    ManagedZeroTerminatedArray();

    /**
     * @brief Returns the element in the specified position.
     * @param[in] index is the element position in the array.
     * @return the element in the \a index position.
     */
    inline T &operator[](const uint32 index) const;

    /**
     * @brief Retrieves the size of the array.
     * @return the number of elements in the array (excluding the terminator Zero).
     */
    uint32 GetSize() const;

    /**
     * @brief Returns the pointer to the beginning of the array.
     * @return the pointer to the beginning of the array.
     */
    T * GetList() const ;

    /**
     * @brief Returns the pointer to the beginning of the array.
     * @return the pointer to the beginning of the array.
     */
    operator T*() const;

    /**
     * @brief Adds one element to the array
     * @return false if realloc fails
     */
    bool Append(const T &data);

    /**
     * @brief Adds one array of elements to the array
     * @return false if realloc fails
     */
    bool Append(const ZeroTerminatedArray<T> &data);

    /**
     * @brief shrinks the array size to the minimum between newSize and the current size
     * @return false if realloc fails
     */
    bool Truncate(uint32 newSize);

protected:

    void *&VoidArray();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
ManagedZeroTerminatedArray<T>::ManagedZeroTerminatedArray() :ZeroTerminatedArray<T>(){
    HeapManager::Malloc(1,array);
    if (array != NULL_PTR(T *)) array[0] = 0;
}

template<typename T>
inline T &ManagedZeroTerminatedArray<T>::operator[](const uint32 index) const {
    return ZeroTerminatedArray<T>::operator[](index);
}

template<typename T>
uint32 ManagedZeroTerminatedArray<T>::GetSize() const {
    return ZeroTerminatedArray<T>::GetSize();
}

template<typename T>
T * ManagedZeroTerminatedArray<T>::GetList() const{
    return ZeroTerminatedArray<T>::GetList();
}

template<typename T>
ManagedZeroTerminatedArray<T>::operator T*() const {
    return ZeroTerminatedArray<T>::GetList();
}

template<typename T>
void *&ManagedZeroTerminatedArray<T>::VoidArray(){

    return reinterpret_cast<void *>(array);
}


template<typename T>
bool ManagedZeroTerminatedArray<T>::Append(const T &data) {
    bool ret = true;
    uint32 size = GetSize();

    HeapManager::Realloc(VoidArray(),size+2);

    ret = (array != NULL_PTR(T *));
    if (ret)  {
        operator[](size) = data;
    }

    return ret;
}

template<typename T>
bool ManagedZeroTerminatedArray<T>::Append(const ZeroTerminatedArray<T> &data) {
    bool ret = true;
    uint32 size = GetSize();
    uint32 size2 = data.GetSize();

    HeapManager::Realloc(VoidArray(),size+size2+1);

    ret = (array != NULL_PTR(T *));

    if (ret)  {
        void *dest      = static_cast<void *>(array+size);
        const void *src = static_cast<void *>(data.GetList());
        MemoryOperationsHelper::Copy(dest,src,size2);
    }

    return ret;
}

template<typename T>
bool ManagedZeroTerminatedArray<T>::Truncate(uint32 newSize) {
    bool ret = true;
    uint32 size = GetSize();

    if (newSize < size){

        HeapManager::Realloc(VoidArray(),newSize+1);

        ret = (array != NULL_PTR(T *));

    }
    return ret;
}


}

#endif /* ZEROTERMINATEDARRAY_H_ */

