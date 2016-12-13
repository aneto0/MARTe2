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
template<typename T,uint32 granularity>
class ManagedZeroTerminatedArray: protected ZeroTerminatedArray<T> {
public:

    /**
     * @brief Default constructor. memory is allocated for granularity characters + terminator
     */
    ManagedZeroTerminatedArray();

    /**
     * @brief Returns the element in the specified position.
     * @param[in] index is the element position in the array.
     * @return the element in the \a index position.
     */
    inline inline T &operator[](const uint32 index) const;

    /**
     * @brief Retrieves the size of the array.
     * @return the number of elements in the array (excluding the terminator Zero).
     */
    inline uint32 GetSize() const;

    /**
     * @brief Returns the pointer to the beginning of the array.
     * @return the pointer to the beginning of the array.
     */
    inline T * GetList() const ;

    /**
     * @brief Returns the pointer to the beginning of the array.
     * @return the pointer to the beginning of the array.
     */
    inline operator T*() const;

    /**
     * @brief Adds one element to the array
     * @return false if realloc fails
     */
    inline bool Append(const T &data);

    /**
     * @brief Adds one array of elements to the array
     * @return false if realloc fails
     */
    inline bool Append(const ZeroTerminatedArray<T> &data);

    /**
     * @brief shrinks the array size to the minimum between newSize and the current size
     * @return false if realloc fails
     */
    inline bool Truncate(uint32 newSize);

protected:
    /**
     * to access pointer as void
     */
    inline void *&VoidArray();

    /**
     * @brief generic allocator function
     * @brief reallocates memory in order to fit fitSize object and terminator
     * @return false if reallocation fails
     */
    static bool GranularMalloc(uint32 sizeofT,uint32 granularity, uint32 fitSize);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T,uint32 granularity>
ManagedZeroTerminatedArray<T,granularity>::ManagedZeroTerminatedArray() :ZeroTerminatedArray<T>(){
    uint32 necessarySize = ((1 + granularity)/ granularity)+granularity;
    HeapManager::Malloc(necessarySize*sizeof(T),array);
    if (array != NULL_PTR(T *)) array[0] = 0;
}

template<typename T,uint32 granularity>
inline T &ManagedZeroTerminatedArray<T,granularity>::operator[](const uint32 index) const {
    return ZeroTerminatedArray<T>::operator[](index);
}

template<typename T,uint32 granularity>
uint32 ManagedZeroTerminatedArray<T,granularity>::GetSize() const {
    return ZeroTerminatedArray<T>::GetSize();
}

template<typename T,uint32 granularity>
T * ManagedZeroTerminatedArray<T,granularity>::GetList() const{
    return ZeroTerminatedArray<T>::GetList();
}

template<typename T,uint32 granularity>
ManagedZeroTerminatedArray<T,granularity>::operator T*() const {
    return ZeroTerminatedArray<T>::GetList();
}

template<typename T,uint32 granularity>
void *&ManagedZeroTerminatedArray<T,granularity>::VoidArray(){
    return reinterpret_cast<void *>(array);
}


template<typename T,uint32 granularity>
bool ManagedZeroTerminatedArray<T,granularity>::Append(const T &data) {
    bool ret = true;
    uint32 size = GetSize();

    // assuming memory is allocated in a granular way
    // we can use this indicator to assess whether we need to allocate or we can simply write
    uint32 freeSpace = (size + 1) % granularity ;

    // extreme case indicating in the worst case no more memory
    if (freeSpace == 0){
        uint32 necessarySize = (size + 1 + granularity);

        HeapManager::Realloc(VoidArray(),necessarySize*sizeof(T));

        ret = (array != NULL_PTR(T *));
    }

    if (ret)  {
        operator[](size) = data;
    }

    return ret;
}

template<typename T,uint32 granularity>
bool ManagedZeroTerminatedArray<T,granularity>::Append(const ZeroTerminatedArray<T> &data) {
    bool ret = true;
    uint32 size = GetSize();
    uint32 size2 = data.GetSize();

    uint32 necessarySize = ((size + size2 + 1 + granularity) / granularity) * granularity;

    HeapManager::Realloc(VoidArray(),necessarySize*sizeof(T));

    ret = (array != NULL_PTR(T *));

    if (ret)  {
        void *dest      = static_cast<void *>(array+size);
        const void *src = static_cast<void *>(data.GetList());
        MemoryOperationsHelper::Copy(dest,src,size2);
    }

    return ret;
}

template<typename T,uint32 granularity>
bool ManagedZeroTerminatedArray<T,granularity>::Truncate(uint32 newSize) {
    bool ret = true;
    uint32 size = GetSize();

    if (newSize < size){
        uint32 necessarySize = ((newSize + 1 + granularity) / granularity) * granularity;

        HeapManager::Realloc(VoidArray(),necessarySize+sizeof(T));

        ret = (array != NULL_PTR(T *));

    }
    if (ret){
        array[newSize] = 0;

    }


    return ret;
}


}

#endif /* ZEROTERMINATEDARRAY_H_ */

