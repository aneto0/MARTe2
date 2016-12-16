/**
 * @file StaticZeroTerminatedArray.h
 * @brief Header file for class StaticZeroTerminatedArray
 * @date Dec 14, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class StaticZeroTerminatedArray
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATICZEROTERMINATEDARRAY_H_
#define STATICZEROTERMINATEDARRAY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


#include "CompilerTypes.h"
#include "ZeroTerminatedArray.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Describes a zero-terminated TArray().
 *
 * @warning This class is only a wrapper of a pointer. The implementation assumes that the pointer
 * in input is a zero-terminated TArray() and does not check if it is not zero-terminated or NULL.
 * If this pre-condition is not accomplished, a segmentation fault in runtime could happen.
 */
template<typename T,uint32 size>
class StaticZeroTerminatedArray: protected ZeroTerminatedArray<T> {
public:

    /**
     * @brief Default constructor. memory is allocated for size characters + terminator
     */
    StaticZeroTerminatedArray( T  ( &vector) [size]);

    /**
     * @brief Returns the element in the specified position.
     * @param[in] index is the element position in the TArray().
     * @return the element in the \a index position.
     */
    inline T &operator[](const uint32 index) const;

    /**
     * @brief Retrieves the size of the TArray().
     * @return the number of elements in the TArray() (excluding the terminator Zero).
     */
    inline uint32 GetSize() const;

    /**
     * @brief Returns the pointer to the beginning of the TArray().
     * @return the pointer to the beginning of the TArray().
     */
    inline T * GetList() const ;

    /**
     * @brief Returns the pointer to the beginning of the TArray().
     * @return the pointer to the beginning of the TArray().
     */
    inline operator T*() const;

    /**
     * @brief Adds one element to the TArray()
     * @return false if realloc fails
     */
    inline bool Append(const T &data);

    /**
     * @brief Adds one TArray() of elements to the TArray()
     * @return false if realloc fails
     */
    inline bool Append(const ZeroTerminatedArray<T> &data);

    /**
     * @brief shrinks the TArray() size to the minimum between newSize and the current size
     * @return false if realloc fails
     */
    inline bool Truncate(uint32 newSize);


protected:
    /**
     * to access pointer as void
     */
    inline const void *&VoidArray();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T,uint32 size>
void const *&StaticZeroTerminatedArray<T,size>::VoidArray(){
    return ZeroTerminatedArray<T>::VoidArray();
}


template<typename T,uint32 size>
StaticZeroTerminatedArray<T,size>::StaticZeroTerminatedArray(T (& vector) [size]) :ZeroTerminatedArray<T>(){
    ZeroTerminatedArray<T>::array = &vector[0];
}

template<typename T,uint32 size>
inline T &StaticZeroTerminatedArray<T,size>::operator[](const uint32 index) const {
    return ZeroTerminatedArray<T>::operator[](index);
}

template<typename T,uint32 size>
uint32 StaticZeroTerminatedArray<T,size>::GetSize() const {
    return ZeroTerminatedArray<T>::GetSize();
}

template<typename T,uint32 size>
T * StaticZeroTerminatedArray<T,size>::GetList() const{
    return ZeroTerminatedArray<T>::GetList();
}

template<typename T,uint32 size>
StaticZeroTerminatedArray<T,size>::operator T*() const {
    return ZeroTerminatedArray<T>::GetList();
}

template<typename T,uint32 size>
bool StaticZeroTerminatedArray<T,size>::Append(const T &data) {
    uint32 currentSize = GetSize();
    uint32 maxSize = size - 1;

    bool ret = (currentSize < maxSize);

    if (ret){
        ZeroTerminatedArray<T>::array[currentSize] = data;
        ZeroTerminatedArray<T>::array[currentSize+1] = 0;
    }
    return ret;
}

template<typename T,uint32 size>
bool StaticZeroTerminatedArray<T,size>::Append(const ZeroTerminatedArray<T> &data) {
    bool ret = true;
    uint32 currentSize = GetSize();
    uint32 maxSize = size - 1;

    uint32 index = 0;
    while (!data.Zero(data[index]) && (currentSize < maxSize)){
        ZeroTerminatedArray<T>::array[currentSize] = data[index];
        currentSize++;
        index++;
    }
    ZeroTerminatedArray<T>::array[currentSize] = 0;

    return (data.Zero(data[index]));
}

template<typename T,uint32 size>
bool StaticZeroTerminatedArray<T,size>::Truncate(uint32 newSize) {
    uint32 maxSize = size - 1;
    bool ret = (newSize <= maxSize) ;

    if (ret){
        ZeroTerminatedArray<T>::array[newSize] = 0;
    }

    return ret;
}


}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATICZEROTERMINATEDARRAY_H_ */
	
