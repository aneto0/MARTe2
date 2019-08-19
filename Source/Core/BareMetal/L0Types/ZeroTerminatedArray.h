/**
 * @file ZeroTerminatedArray.h
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

#ifndef ZEROTERMINATEDARRAY_H_
#define ZEROTERMINATEDARRAY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <string.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "ZeroTerminatedArrayStaticTools.h"
#include "ZeroTerminatedArrayToolT.h"
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
class ZeroTerminatedArray: public ZeroTerminatedArrayStaticTools {
public:

    /**
     * @brief Default constructor.
     * @param[in] arrayIn is the zero terminated array input.
     * @pre
     *   arrayIn is zero terminated.
     */
    inline ZeroTerminatedArray(T *arrayIn=NULL_PTR(T *));

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
    inline uint32 GetSize() const;

    /**
     * @brief Finds the first location in the array that contains T
     * @param[in] data is the element to be found.
     * @return the position in the array where T is found, 0xFFFFFFFF if not found.
     */
    inline uint32 Find(const T & data) const;

    /**
     * @brief Finds the first location in the array that contains T
     * @param[in] data is the element to be found.
     * @return the position in the array where T is found, 0xFFFFFFFF if not found.
     */
    inline uint32 FindPattern(const T *data) const;

    /**
     * @brief Finds the first location in the array that contains T
     * @param[in] data is the element to be found.
     * @return the string starting at the found position emptyString otherwise
     */
    inline ZeroTerminatedArray<T> FindString(const T & data) const;

    /**
     * @brief Finds the first location in the array that contains T
     * @param[in] data is the element to be found.
     * @return the string starting at the found position emptyString otherwise
     */
    inline ZeroTerminatedArray<T> FindPatternString(const T *data) const;

    /**
     * @brief Checks if data is in this array
     * @param[in] data is the element to be found.
     * @return true if found
     */
    inline bool In(const T& data) const;

    /**
     * @brief Checks if the array data is in this array
     * @param[in] data is the element to be found.
     * @return true if found
     */
    inline bool In(const ZeroTerminatedArray<T>& data) const;

    /**
     * @brief Returns the pointer to the beginning of the array.
     * @return the pointer to the beginning of the array.
     */
    inline T * GetList() const ;

    /**
     * @brief Allows changing the list
     * @param[in] arrayIn is the zero terminated array input.
     */
    inline void SetList(T *arrayIn)  ;

    /**
     * move pointer ahead of one element. Checks limits
     * if !Zero(*array) && (array != NULL)  array++;
     */
    inline void Skip();

    /**
     * move pointer ahead of one element. Does not checks limits
     * note that the parameter int is only to specify postpending
     */
    inline void operator++(int);

    /**
     * @brief Checks if the input \a data is the array terminal character.
     * @details This function must be specialized for each new template type. The default
     * terminal is (as the name of the class implies) static_cast<T>(0)
     * @param[in] data is the element to be checked.
     * @return true if \a data is the array terminal, false otherwise.
     */
    inline bool IsZero(const T & data) const;

    /**
     * @brief Checks if the ptr is valid
     * @return true if \a array is NULL.
     */
    inline bool IsNullPtr() const;

    /**
     * @brief Checks if the input \a arrayIn has the same content as the array
     * @details The class T must have a defined operator ==
     * @param[in] arrayIn is the array to be compared
     * @param[in] limit is the number of characters that will be checked, starting from the first. 0xFFFFFFFF is the max
     * @return true if \a arrayIn is the same.
     */
    inline bool IsSameAs(const T *arrayIn,uint32 limit=0xFFFFFFFF) const;

    /**
     * @brief establish an order between \a arrayIn and the array
     * @details This function requires that T has the operators < and >
     * @param[in] arrayIn is the array to be compared
     * @param[in] limit is the number of characters that will be checked, starting from the first. 0xFFFFFFFF is the max
     * @return true if \a arrayIn is the same.
     */
    inline int32 CompareContent(const T *arrayIn,uint32 limit=0xFFFFFFFF) const;


    /**
     * @brief Allows obtaining a tool to perform efficient editing operations on the string
     */
    inline ZeroTerminatedArrayToolT<T>& operator()();

    /**
     * @brief Adds one element to the TArray()
     * @return false if realloc fails TODO
     */
    inline ZeroTerminatedArrayToolT<T>  Append(const T &data);

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

 protected:

    /**
     * @brief The array
     */
    T * array;

    /**
     * @brief to access pointer as void
     */
    inline void *&VoidArray();

    /**
     * @brief to access pointer as T
     */
    inline T *&TArray();

};

template<>
inline bool ZeroTerminatedArray<const char8>::IsSameAs(const char8 *arrayIn,uint32 limit) const;

template<>
inline int32 ZeroTerminatedArray<const char8>::CompareContent(const char *arrayIn,uint32 limit) const ;

template<>
inline uint32 ZeroTerminatedArray<const char8>::FindPattern(const char8 * data) const;

template<>
inline uint32 ZeroTerminatedArray<const char8>::Find(const char8 & data) const;

template<>
inline ZeroTerminatedArray<const char8> ZeroTerminatedArray<const char8>::FindPatternString(const char8 * data) const;

template<>
inline ZeroTerminatedArray<const char8> ZeroTerminatedArray<const char8>::FindString(const char8 & data) const;


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
void *&ZeroTerminatedArray<T>::VoidArray(){
    return const_cast<void *&>(reinterpret_cast<void  * &>(array));
}

template<typename T>
T *&ZeroTerminatedArray<T>::TArray(){
    return array;
}

template<typename T>
ZeroTerminatedArray<T>::ZeroTerminatedArray(T *arrayIn) :
    array(arrayIn) {
}

template<typename T>
inline T &ZeroTerminatedArray<T>::operator[](const uint32 index) const {
    return array[index];
}

template<typename T>
uint32 ZeroTerminatedArray<T>::GetSize() const {
	return ZTAGetSize((const uint8 *)array,sizeof(T));
}


template<typename T>
uint32 ZeroTerminatedArray<T>::Find(const T & data) const{
	return ZTAFind((const uint8 *)array,(const uint8 *)&data,sizeof(T));
}

template<>
uint32 ZeroTerminatedArray<const char8>::Find(const char8 & data) const{
	const char8 *found = NULL_PTR(const char8 *);
	if ((array != NULL_PTR(const char8 *)) && (data != '\0')){
		found = strchr(array,data);
	}
	uint32 ret = 0xFFFFFFFFU;
	if (found != NULL_PTR(const char8 *)){
		ret = static_cast<uint32>(found - array);
	}
	return ret;
}

template<typename T>
uint32 ZeroTerminatedArray<T>::FindPattern(const T * data) const{
	uint32 ret = 0xFFFFFFFFu;

	const T *pArray = array;
	const T *pTest = data;
	if ((pArray != NULL_PTR(const T*)) && (pTest != NULL_PTR(const T*))){
		uint32 limit = ZeroTerminatedArray<T>(data).GetSize();
		bool found = false;
    	ZeroTerminatedArray<T> search(this);
	    while (!IsZero(*pArray) && !found) {
	    	found = search.IsSameAs(pTest,limit);
	    	search++;
	    }
	    if (found ){
		    ret = static_cast<uint32>(search.array - array);
	    }
	}
	return ret;
}

template<>
uint32 ZeroTerminatedArray<const char8>::FindPattern(const char8* data) const{
	const char8 *found = NULL_PTR(const char8 *);
	if ((array != NULL_PTR(const char8 *)) && (data != NULL_PTR(const char8 *))){
		found = strstr(array,data);
	}
	uint32 ret = 0xFFFFFFFFU;
	if (found != NULL_PTR(const char8 *)){
		ret = static_cast<uint32>(found - array);
	}
	return ret;
}

template<typename T>
ZeroTerminatedArray<T> ZeroTerminatedArray<T>::FindString(const T & data) const{
	uint32 index = ZTAFind((const uint8 *)array,(const uint8 *)&data,sizeof(T));
	ZeroTerminatedArray<T> retS;
	if (index != 0xFFFFFFFFU){
		retS = array+index;
	}
	return retS;
}

template<>
ZeroTerminatedArray<const char8> ZeroTerminatedArray<const char8>::FindString(const char8 & data) const{
	const char8 *found = NULL_PTR(const char8 *);
	if ((array != NULL_PTR(const char8 *)) && (data != '\0')){
		found = strchr(array,data);
	}
	ZeroTerminatedArray<const char8> retS;
	if (found != NULL_PTR(const char8 *)){
		retS = found;
	}
	return retS;
}

template<typename T>
ZeroTerminatedArray<T> ZeroTerminatedArray<T>::FindPatternString(const T * data) const{
	ZeroTerminatedArray<T> retS;

	const T *pArray = array;
	const T *pTest = data;
	if ((pArray != NULL_PTR(const T*)) && (pTest != NULL_PTR(const T*))){
		uint32 limit = ZeroTerminatedArray<T>(data).GetSize();
		bool found = false;
    	ZeroTerminatedArray<T> search(this);
	    while (!IsZero(*pArray) && !found) {
	    	found = search.IsSameAs(pTest,limit);
	    	search++;
	    }
	    if (found ){
	    	retS = search.array;
	    }
	}
	return retS;
}

template<>
ZeroTerminatedArray<const char8> ZeroTerminatedArray<const char8>::FindPatternString(const char8* data) const{
	const char8 *found = NULL_PTR(const char8 *);
	if ((array != NULL_PTR(const char8 *)) && (data != NULL_PTR(const char8 *))){
		found = strstr(array,data);
	}
	ZeroTerminatedArray<const char8> retS;
	if (found != NULL){
		retS = found;
	}
	return retS;
}

template<typename T>
T * ZeroTerminatedArray<T>::GetList() const{
    return array;
}

template<typename T>
void ZeroTerminatedArray<T>::SetList(T *arrayIn) {
    array = arrayIn;
}

template<typename T>
bool ZeroTerminatedArray<T>::IsZero(const T & data) const {
	return ZTAIsZero((const uint8 *)&data,sizeof(T));
}

template<typename T>
bool ZeroTerminatedArray<T>::IsNullPtr() const {
    return (array == NULL_PTR(T *));
}


template<typename T>
void ZeroTerminatedArray<T>::Skip() {
    if (array != NULL_PTR(T *))  {
        if (!IsZero(*array)){
            array++;
        }
    }
}

template<typename T>
void ZeroTerminatedArray<T>::operator++(int) {//int is for postfix operator!
    array++;
}

template<typename T>
bool ZeroTerminatedArray<T>::IsSameAs(const T *arrayIn,uint32 limit) const {
	bool same = true;
    if ((array != NULL_PTR(T*))&&(arrayIn != NULL_PTR(T*))) {
        const T * listP = array;
        const T * list2P = arrayIn;
        while (!IsZero(*listP) && same && (limit > 0)) {
            same = (*listP == *list2P);
            listP++;
            list2P++;
            limit--;
        }
        // if sane and limit > 0 it means we reached a terminator on listP. Check list2P
        if (same && (limit > 0)){
        	same = ZTAIsZero((const uint8 *)list2P,sizeof(T));
        }
    }
    return same;
}


template<>
bool ZeroTerminatedArray<const char8>::IsSameAs(const char8 *arrayIn,uint32 limit) const {
	bool ret = (array == arrayIn);
	if (!ret && (array != NULL_PTR(const char8 *)) && (arrayIn != NULL_PTR(const char8 *))){
		ret = (strncmp(reinterpret_cast<const char*>(array),reinterpret_cast<const char*>(arrayIn),limit)==0);
	}
	return ret;
}

template<typename T>
int32 ZeroTerminatedArray<T>::CompareContent(const T *arrayIn,uint32 limit) const {
	int32 compResult = 0;
	bool finished = false;

	if (array == NULL_PTR(T*)){
		finished = true;
		compResult--;  // returns -1 unless next is also NULL
	}
	if (arrayIn == NULL_PTR(T*)){
		finished = true; // returns 1 unless previous was NULL
		compResult++;
	}
    const T * listP = array;
    const T * listPIn = arrayIn;

    while (!IsZero(*listP) && !finished && (limit > 0)) {
    	if (*listP > *listPIn){
    		compResult= 1;
    		finished = true;
    	} else
    	if (*listP < *listPIn){
    		compResult= -1;
    		finished = true;
    	}
        listP++;
        listPIn++;
        limit--;
    }

    // exit due to limit or IsZero
    if ((!finished) && (limit > 0)){
    	if (IsZero(*listP)){
    		finished = true;
    		compResult--;
    	}
    	if (IsZero(*listPIn)){
    		finished = true;
    		compResult++;
    	}
    }
    return compResult;
}


template<>
int32 ZeroTerminatedArray<const char8>::CompareContent(const char *arrayIn,uint32 limit) const {
	int32 ret = 0;
	if (arrayIn == NULL_PTR(const char8 *)){
		if (array != NULL_PTR(const char8 *)){
			ret = 1;
		}
	} else
	if (array == NULL_PTR(const char8 *)){
		if (arrayIn != NULL_PTR(const char8 *)){
			ret = -1;
		}
	} else {
		ret =  strncmp(reinterpret_cast<const char*>(array),reinterpret_cast<const char*>(arrayIn),limit);
		if (ret > 0) ret = 1;
		if (ret < 0) ret = -1;
	}
	return ret;
}


template<typename T>
bool ZeroTerminatedArray<T>::In(const T& data) const{
	return (Find(data)!= 0xFFFFFFFF);
}

template<typename T>
ZeroTerminatedArrayToolT<T>& ZeroTerminatedArray<T>::operator()(){
	return ZeroTerminatedArrayToolT<T>(NULL,array,GetSize()+1);
}

template<typename T>
ZeroTerminatedArrayToolT<T> ZeroTerminatedArray<T>::Append(const T &data) {
	return operator ()().Append(data);
}

template<typename T>
ZeroTerminatedArrayToolT<T> ZeroTerminatedArray<T>::AppendN(const ZeroTerminatedArray<T> & data,uint32 maxAppendSize) {
	return operator ()().AppendN(data,maxAppendSize);
}

template<typename T>
ZeroTerminatedArrayToolT<T> ZeroTerminatedArray<T>::Truncate(uint32 newSize) {
	return operator ()().Truncate(newSize);
}

template<typename T>
ZeroTerminatedArrayToolT<T> ZeroTerminatedArray<T>::Remove(uint32 elements){
	return operator ()().Remove(elements);
}


}
 
#endif /* ZEROTERMINATEDARRAY_H_ */

