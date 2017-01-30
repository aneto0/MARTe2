/**
 * @file DynamicZeroTerminatedArray.h
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
#include "MemoryOperationsHelper.h"
#include "ErrorManagement.h"

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
template<typename T,uint32 granularity>
class DynamicZeroTerminatedArray: protected ZeroTerminatedArray<T> {
public:

    /**
     * @brief Default constructor. memory is allocated for granularity characters + terminator
     */
    DynamicZeroTerminatedArray();

    /**
     * @brief Copy constructor.
     */
    DynamicZeroTerminatedArray(const ZeroTerminatedArray<const T> &data);

    /**
     * @brief deallocates memory
     */
    ~DynamicZeroTerminatedArray();

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
     * @brief Adds one element to the TArray()
     * @return false if realloc fails
     */
    inline bool Append(const T &data);

    /**
     * @brief Adds one TArray() of elements to the TArray()
     * @return false if realloc fails
     */
    inline bool AppendN(const ZeroTerminatedArray< T> &  data,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief Adds one TArray() of elements to the TArray()
     * @return false if realloc fails
     */
    inline bool AppendN(const ZeroTerminatedArray< const T> &  data,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief shrinks the TArray() size to the minimum between newSize and the current size
     * @return false if realloc fails
     */
    inline bool Truncate(uint32 newSize);

protected:

    /**
     * to access pointer as void
     */
    void  *&VoidArray();

    /**
     * to access pointer as void
     */
    T *&TArray();

};

// private functions to implement the templated methods
bool DZTInitCopy(uint32 sizeOfData,uint32 sizeOfT,uint32 granularity,void *&dest, void const *src);
bool DZTAppend1(uint32 sizeOfT,uint32 granularity,uint32 sizeOfDest,void *&dest, void const *src);
bool DZTAppendN(uint32 sizeOfT,uint32 granularity,uint32 sizeOfDest,uint32 sizeOfSrc,
		        uint32 maxAppendSize,void *&dest, void const  *src);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T,uint32 granularity>
void  *&DynamicZeroTerminatedArray<T,granularity>::VoidArray(){
    return ZeroTerminatedArray<T>::VoidArray();
}

template<typename T,uint32 granularity>
T *&DynamicZeroTerminatedArray<T,granularity>::TArray(){
    return ZeroTerminatedArray<T>::TArray();
}

template<typename T,uint32 granularity>
DynamicZeroTerminatedArray<T,granularity>::DynamicZeroTerminatedArray() :ZeroTerminatedArray<T>(){
    const uint32 necessarySize = ((1 + granularity)/ granularity)*granularity;
    VoidArray() = HeapManager::Malloc(necessarySize*sizeof(T));
    if (TArray() != NULL_PTR(T *)) {
    	static const T term(0u);
    	TArray()[0] = term;
    }
}


template<typename T,uint32 granularity>
DynamicZeroTerminatedArray<T,granularity>::DynamicZeroTerminatedArray(const ZeroTerminatedArray<const T> &data) :ZeroTerminatedArray<T>(){
	const void *src = static_cast<const void *>(data.GetList());
	DZTInitCopy(data.GetSize(),sizeof(T),granularity,VoidArray(),src);
}

template<typename T,uint32 granularity>
DynamicZeroTerminatedArray<T,granularity>::~DynamicZeroTerminatedArray(){
	bool ok = HeapManager::Free(VoidArray());
    if (!ok) {
        REPORT_ERROR(ErrorManagement::FatalError, "DynamicZeroTerminatedArray: Failed HeapManager::Free()");
    }
}

template<typename T,uint32 granularity>
inline T &DynamicZeroTerminatedArray<T,granularity>::operator[](const uint32 index) const {
    return ZeroTerminatedArray<T>::operator[](index);
}

template<typename T,uint32 granularity>
uint32 DynamicZeroTerminatedArray<T,granularity>::GetSize() const {
    return ZeroTerminatedArray<T>::GetSize();
}

template<typename T,uint32 granularity>
T * DynamicZeroTerminatedArray<T,granularity>::GetList() const{
    return ZeroTerminatedArray<T>::GetList();
}

template<typename T,uint32 granularity>
bool DynamicZeroTerminatedArray<T,granularity>::Append(const T &data) {
	const void *src = static_cast<const void *>(&data);
	uint32 sizeD = GetSize();
    bool ret = DZTAppend1(sizeof(T),granularity,GetSize(),VoidArray(),src);
    if (ret)  {
        operator[](sizeD) = data;
        operator[](sizeD+1) = 0u;
    }
    return ret;
}

template<typename T,uint32 granularity>
bool DynamicZeroTerminatedArray<T,granularity>::AppendN(const ZeroTerminatedArray<T> & data,uint32 maxAppendSize) {
	const void *src = static_cast<const void *>(data.GetList());
	uint32 sizeD = GetSize();
	uint32 sizeS = data.GetSize();
	bool ret = DZTAppendN(sizeof(T),granularity,sizeD,sizeS,maxAppendSize,VoidArray(),src);
    if (ret)  {
        operator[](sizeS+sizeD) = 0u;
    }

    return ret;
}

template<typename T,uint32 granularity>
bool DynamicZeroTerminatedArray<T,granularity>::AppendN(const ZeroTerminatedArray<const T> & data,uint32 maxAppendSize) {
	const void *src = static_cast<const void *>(data.GetList());
	uint32 sizeD = GetSize();
	uint32 sizeS = data.GetSize();
	bool ret = DZTAppendN(sizeof(T),granularity,sizeD,sizeS,maxAppendSize,VoidArray(),src);
    if (ret)  {
        operator[](sizeS+sizeD) = 0u;
    }

    return ret;
}

template<typename T,uint32 granularity>
bool DynamicZeroTerminatedArray<T,granularity>::Truncate(uint32 newSize) {
    bool ret = true;
    uint32 size = GetSize();

    if (newSize < size){
        uint32 necessarySize = ((newSize + 1 + granularity) / granularity) * granularity;
        HeapManager::Realloc(VoidArray(),necessarySize+sizeof(T));
        ret = (TArray() != NULL_PTR(T *));
    }
    if (ret){
        TArray()[newSize] = 0u;
    }
    return ret;
}


}

#endif /* ZEROTERMINATEDARRAY_H_ */

