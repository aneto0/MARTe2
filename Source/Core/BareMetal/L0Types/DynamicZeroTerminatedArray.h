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

#ifndef DYNAMICZEROTERMINATEDARRAY_H_
#define DYNAMICZEROTERMINATEDARRAY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

//#include "ErrorManagement.h"
#include "AllocationPointer.h"
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
 *
 * @warning Try not use any other class in this header to avoid loop inclusions.
 */
template<typename T>
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
     * @brief copies the content.
     */
    inline void operator= (const DynamicZeroTerminatedArray<const T> &data);

    /**
     * @brief Checks if the input \a arrayIn has the same content as the array
     * @details This function allows implementing operator==
     * @param[in] arrayIn is the array to be compared
     * @param[in] limit is the number of characters that will be checked, starting from the first. 0xFFFFFFFF is the max
     * @return true if \a arrayIn is the same.
     */
    inline bool isSameAs(const T *arrayIn,uint32 limit=0xFFFFFFFF) const;

    /**
     * @brief Allows obtaining a tool to perform efficient editing operations on the string
     */
    inline ZeroTerminatedArrayToolT<T> operator()();


protected:

    /**
     * @brief to access pointer as void
     */
    void  *&VoidArray();

    /**
     * @brief to access pointer as void
     */
    T *&TArray();

    /**
     * @brief to write a terminator (zeroes) at the given position
     */
    inline void Terminate(uint32 position);

    /**
     * @brief returns the maximum size this string can hold (the terminator is considered (buffersize-1)).
     * It is obtained thanks to the AllocationPointer
     */
    inline uint32 GetMaxIndex();

    /**
     * @brief Adds one element to the TArray()
     * @return false if realloc fails TODO
     */
    inline ZeroTerminatedArrayToolT<T>  Append(const T &data);

    /**
     * @brief Adds one TArray() of elements to the TArray()
     * @return false if realloc fails TODO
     */
    inline ZeroTerminatedArrayToolT<T>  AppendN(const ZeroTerminatedArray< const T> &  data,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief Adds one TArray() of elements to the TArray()
     * @return false if realloc fails TODO
     */
    inline ZeroTerminatedArrayToolT<T>  AppendN(const ZeroTerminatedArray< T> &  data,uint32 maxAppendSize=0xFFFFFFFF);

    /**
     * @brief shrinks the TArray() size to the minimum between newSize and the current size
     * @return false if realloc fails TODO
     */
    inline ZeroTerminatedArrayToolT<T>   Truncate(uint32 newSize);

    /**
     * @brief removes elements from the top of the ZTA
     * moves the string content and then truncates
     * @return false if not enough characters TODO
     */
    inline ZeroTerminatedArrayToolT<T>    Remove(uint32 elements);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
void DynamicZeroTerminatedArray<T>::Terminate(uint32 position){
	T *p =  TArray();
	ZTAZero(reinterpret_cast<uint8 *>(p+position),sizeof(T));
}

template<typename T>
void  *&DynamicZeroTerminatedArray<T>::VoidArray(){
    return ZeroTerminatedArray<T>::VoidArray();
}

template<typename T>
T *&DynamicZeroTerminatedArray<T>::TArray(){
    return ZeroTerminatedArray<T>::TArray();
}

template<typename T>
DynamicZeroTerminatedArray<T>::DynamicZeroTerminatedArray() :ZeroTerminatedArray<T>(){
}

template<typename T>
DynamicZeroTerminatedArray<T>::DynamicZeroTerminatedArray(const ZeroTerminatedArray<const T> &data) :ZeroTerminatedArray<T>(){
	AppendN(data);
}

template<typename T>
uint32 DynamicZeroTerminatedArray<T>::GetMaxIndex(){
	HeapManager::AllocationPointer p(TArray());

    return  (p.GetByteSize()/sizeof(T))-1;
}

template<typename T>
void DynamicZeroTerminatedArray<T>::operator= (const DynamicZeroTerminatedArray<const T> &data){
	Terminate(0);
	AppendN(data);
}

template<typename T>
DynamicZeroTerminatedArray<T>::~DynamicZeroTerminatedArray(){
	HeapManager::AllocationPointer p(TArray());
	p.Free();
	VoidArray() = NULL;
}

template<typename T>
inline T &DynamicZeroTerminatedArray<T>::operator[](const uint32 index) const {
    return ZeroTerminatedArray<T>::operator[](index);
}

template<typename T>
uint32 DynamicZeroTerminatedArray<T>::GetSize() const {
    return ZeroTerminatedArray<T>::GetSize();
}

template<typename T>
T * DynamicZeroTerminatedArray<T>::GetList() const{
    return ZeroTerminatedArray<T>::GetList();
}

template<typename T>
bool DynamicZeroTerminatedArray<T>::isSameAs(const T *arrayIn,uint32 limit) const {
    return ZeroTerminatedArray<T>::isSameAs(arrayIn,limit);
}

template<typename T>
ZeroTerminatedArrayToolT<T> DynamicZeroTerminatedArray<T>::operator()(){
	return ZeroTerminatedArrayToolT<T>(&array,array,GetSize()+1);
}

template<typename T>
ZeroTerminatedArrayToolT<T> DynamicZeroTerminatedArray<T>::Append(const T &data) {
	return operator ()().Append(data);
}

template<typename T>
ZeroTerminatedArrayToolT<T> DynamicZeroTerminatedArray<T>::AppendN(const ZeroTerminatedArray<const T> & data,uint32 maxAppendSize) {
	return operator ()().AppendN(data,maxAppendSize);
}

template<typename T>
ZeroTerminatedArrayToolT<T> DynamicZeroTerminatedArray<T>::AppendN(const ZeroTerminatedArray<T> & data,uint32 maxAppendSize) {
	return operator ()().AppendN(data,maxAppendSize);
}

template<typename T>
ZeroTerminatedArrayToolT<T> DynamicZeroTerminatedArray<T>::Truncate(uint32 newSize) {
	return operator ()().Truncate(newSize);
}

template<typename T>
ZeroTerminatedArrayToolT<T> DynamicZeroTerminatedArray<T>::Remove(uint32 elements){
	return operator ()().Remove(elements);
}


}

#endif /* ZEROTERMINATEDARRAY_H_ */

