/**
 * @file CCString.h
 * @brief Header file for class CCString
 * @date Dec 16, 2016
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

 * @details This header file contains the declaration of the class CCString
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPES_CCSTRING_H_
#define L0TYPES_CCSTRING_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ZeroTerminatedArray.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * TODO
 * @brief Wrapper for constant char buffers
 * */
class CCString: protected ZeroTerminatedArray<const char8>{

public:

    /**
     * @brief generates a null value for CCString
     */
    inline CCString (uint32 zero=0);

    /**
     * TODO
     */
    inline CCString (CCString const &s);

    /**
     * TODO
     */
    template <uint32 size>
    inline CCString (char8 const (&vector) [size]);

    /**
     * TODO
     */
    inline CCString (char8 const * const &s);

    /**
     * @brief Returns the element in the specified position.
     * @param[in] index is the element position in the TArray().
     * @return the element in the \a index position.
     */
    inline char8 operator[](uint32 index) const;

    /**
     * TODO
     */
    inline bool operator==(const CCString &s) const;

    /**
     * TODO
     */
    inline bool operator==(const char8 *s) const;

    /**
     * TODO
     */
    inline bool operator==(const char8 c) const;

    /**
     * @brief Retrieves the size of the array().
     * @return the number of elements in the array() (excluding the terminator Zero).
     */
    inline uint32 GetSize() const;

    /**
     * @brief Returns the pointer to the beginning of the TArray().
     * @return the pointer to the beginning of the TArray().
     */
    inline const char8* GetList() const ;

    /**
     * @brief Returns the pointer to the beginning of the array().
     * @return the pointer to the beginning of the array().
     */
    inline operator const char8*() const;

    /**
     * @brief Checks if the ptr is valid
     * @return true if \a array is NULL.
     */
    inline bool IsNullPtr() const;

    /**
     * move pointer ahead of one element. Does not checks limits
     * note that the parameter int is only to specify postpending
     */
    inline void operator++(int);

    /**
     * @brief Checks if the input \a arrayIn has the same content as the array
     * @details This function allows implementing operator==
     * @param[in] arrayIn is the array to be compared
     * @param[in] limit is the number of characters that will be checked, starting from the first. 0xFFFFFFFF is the max
     * @return true if \a arrayIn is the same.
     */
    inline bool IsSameAs(const char8 *arrayIn,uint32 limit=0xFFFFFFFF) const;

    /**
     * @brief establish an order between \a arrayIn and the array
     * @details This function requires that T has the operators < and >
     * @param[in] arrayIn is the array to be compared
     * @param[in] limit is the number of characters that will be checked, starting from the first. 0xFFFFFFFF is the max
     * @return 0 if \a arrayIn is the same, -1 is lower 1 if higher. NULL is equal to NULL and  lower than any valid string
     */
    inline int32 CompareContent(const char8 *arrayIn,uint32 limit=0xFFFFFFFF) const;

    /**
     * @brief Checks if data is in this array
     * @param[in] data is the element to be found.
     * @return true if found
     */
    inline bool In(const char8& data) const;

    /**
     * @brief Finds the first location in the array that contains pattern
     * @param[in] pattern is the substring to be found.
     * @return the position in the array where T is found, 0xFFFFFFFF if not found.
     */
    inline uint32 FindPattern(CCString pattern) const;

    /**
     * @brief Finds the first location in the array that contains pattern
     * @param[in] pattern is the substring to be found.
     * @return the substring in the array where T is found, 0xFFFFFFFF if not found.
     */
    inline CCString FindPatternString(CCString pattern) const;

    /**
     * @brief Finds the first location in the array that contains pattern
     * @param[in] pattern is the substring to be found.
     * @return the position in the array where T is found, 0xFFFFFFFF if not found.
     */
    inline uint32 Find(char8 c) const;

    /**
     * @brief Finds the first location in the array that contains pattern
     * @param[in] pattern is the substring to be found.
     * @return the substring in the array where T is found, 0xFFFFFFFF if not found.
     */
    inline CCString FindString(char8 c) const;

};

static const CCString emptyString(static_cast<uint32>(0));


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

CCString::CCString (uint32 zero){ }

CCString::CCString (CCString const &s):ZeroTerminatedArray<const char8>(s){}

template <uint32 size>
CCString::CCString (char8 const (&vector) [size]):ZeroTerminatedArray<const char8>(&vector[0]){}

CCString::CCString (char8 const * const &s):ZeroTerminatedArray<const char8>(s){}

char8 CCString::operator[](uint32 index) const{
    return ZeroTerminatedArray<const char8>::operator[](index);
}

bool CCString::operator==(const CCString &s) const{
	return ZeroTerminatedArray<const char8>::IsSameAs(s.GetList());
}

bool CCString::operator==(const char8 *s) const{
	return ZeroTerminatedArray<const char8>::IsSameAs(s);
}

bool CCString::operator==(const char8 c) const{
	bool ret = false;
	if (GetSize() == 1){
		ret = (*GetList() == c);
	}
	return ret;
}

uint32 CCString::GetSize() const{
	return ZeroTerminatedArray<const char8>::GetSize();
}

const char8* CCString::GetList() const {
	return ZeroTerminatedArray<const char8>::GetList();
}

CCString::operator const char8*() const {
	return ZeroTerminatedArray<const char8>::GetList();
}

bool CCString::IsNullPtr() const{
	return ZeroTerminatedArray<const char8>::IsNullPtr();
}

void CCString::operator++(int) {//int is for postfix operator!
	ZeroTerminatedArray<const char8>::SetList(GetList()+1);
}

bool CCString::IsSameAs(const char8 *arrayIn,uint32 limit) const{
	return ZeroTerminatedArray<const char8>::IsSameAs(arrayIn,limit);
}

int32 CCString::CompareContent(const char8 *arrayIn,uint32 limit) const{
	return ZeroTerminatedArray<const char8>::CompareContent(arrayIn,limit);
}

bool CCString::In(const char8& data) const{
	return ZeroTerminatedArray<const char8>::In(data);
}

uint32 CCString::FindPattern(CCString pattern) const{
	return ZeroTerminatedArray<const char8>::FindPattern(pattern.GetList());
}

CCString CCString::FindPatternString(CCString pattern) const{
	return ZeroTerminatedArray<const char8>::FindPatternString(pattern.GetList()).GetList();
}

uint32 CCString::Find(char8 c) const{
	return ZeroTerminatedArray<const char8>::Find(c);
}

CCString CCString::FindString(char8 c) const{
	return ZeroTerminatedArray<const char8>::FindString(c).GetList();
}

}
#endif /* L0TYPES_CCSTRING_H_ */
	
