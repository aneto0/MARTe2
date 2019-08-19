/**
 * @file DynamicCString.h
 * @brief Header file for class DynamicCString
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

 * @details This header file contains the declaration of the class DynamicCString
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPES_DYNAMICCSTRING_H_
#define L0TYPES_DYNAMICCSTRING_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "DynamicZeroTerminatedArray.h"
#include "CCString.h"
#include "CStringTool.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


/**
* @brief Wrapper for writable char buffers
* */
class DynamicCString: protected DynamicZeroTerminatedArray<char8>{
public:
    /**
     * @brief returns a CCString containing the same pointer.
     * Note that after any operation affecting the size of this
     * string the pointer may change
     */
    inline operator CCString() const;

    /**
     *
     */
    inline CStringTool operator()();

    /**
     * @brief constructor
     */
    inline DynamicCString ();

    /**
     * @briefs allocates memory and copies the content
     */
    inline DynamicCString (DynamicCString const &s);

    /**
     * @briefs allocates memory and copies the content
     */
    inline DynamicCString (const CCString &s);

    /**
     * @brief Retrieves the size of the array().
     * @return the number of elements in the array() (excluding the terminator Zero).
     */
    inline uint32 GetSize() const;

    /**
     * @brief Returns the pointer to the beginning of the TArray().
     * @return the pointer to the beginning of the TArray().
     */
    inline char8* GetList() const ;

    /**
     * @Brief compare content
     */
    inline bool operator==(const CCString &s) const;

    /**
     * @Brief copies one string to another.
     * Necessary to avoid default copy operator use!
     */
    inline void  operator=(const DynamicCString &s) ;

    /**
     * @Brief copies one string to another.
     * Necessary to avoid default copy operator use!
     */
    inline void  operator=(const CCString &s) ;

    /**
     * @Brief copies one string to another.
     * Necessary to avoid default copy operator use!
     */
    inline void  operator=(char8 const * const &s) ;

    /**
     * @brief Returns the element in the specified position.
     * @param[in] index is the element position in the array().
     * @return the element in the \a index position.
     */
    inline char8 &operator[](const uint32 index) const;

    /**
     * @brief Checks if the input \a arrayIn has the same content as the array
     * @details This function allows implementing operator==
     * @param[in] arrayIn is the array to be compared
     * @param[in] limit is the number of characters that will be checked, starting from the first. 0xFFFFFFFF is the max
     * @return true if \a arrayIn is the same.
     */
    inline bool IsSameAs(const char8 *arrayIn,uint32 limit=0xFFFFFFFF) const;

    /**
     * @brief Get the token using characters as delimiters.
     * @param[in] string is the string to tokenize.
     * @param[in] delimiters contains character delimiters.
     * @param[in] skip contains characters not to be included in token
     * @param[out] limit is 0xFFFFFFFF if the limit was the end of string. Otherwise the index within \a delimiters
     * @param[in] keepTerm if true moves the char pointer in the output up to the terminator, not up to the next character
     * @param[out] token is the substring between delimiters (including delimeters if not in skip).
     * @return a pointer to the next position after the delimiter for a successive tokenize operation or
     * the end of the string if terminator is not found. It returns NULL if at least one of the
     * arguments is NULL.
     */
    static DLL_API CCString  Tokenize(CCString const string, DynamicCString &token,uint32 &limit,CCString const delimiters, CCString const skip,bool keepTerm=false);

    /**
     * @brief Get the token using strings as delimiters.
     * @param[in] string is the string to tokenize.
     * @param[in] delimiters contains string delimiters.
     * @param[in] skip contains characters not to be included in token
     * @param[out] limit is 0xFFFFFFFF if the limit was the end of string. Otherwise the index within \a delimiters
     * @param[out] token is the substring between delimiters (including delimeters if not in skip).
     * @return a pointer to the next position after the delimiter for a successive tokenize operation or
     * the end of the string if terminator is not found. It returns NULL if at least one of the
     * arguments is NULL.
     */
    static DLL_API CCString  Tokenize(CCString const string, DynamicCString &token,uint32 &limit, ZeroTerminatedArray<const CCString> const delimiters, CCString const skip);

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

DynamicCString::DynamicCString (){
}

DynamicCString::DynamicCString(DynamicCString const &s):DynamicZeroTerminatedArray<char8>(s){
}

DynamicCString::DynamicCString (const CCString &s):DynamicZeroTerminatedArray<char8>(s.GetList()){
}

DynamicCString::operator CCString() const{
    return CCString(DynamicZeroTerminatedArray<char8>::GetList());
}

bool DynamicCString::operator==(const CCString &s) const{
	return DynamicZeroTerminatedArray<char8>::IsSameAs(s.GetList());
}

void DynamicCString::operator=(const DynamicCString &s) {
	Truncate(0U);
	DynamicZeroTerminatedArray<char8>::AppendN(s.GetList());
}

void DynamicCString::operator=(const CCString &s) {
	Truncate(0U);
	DynamicZeroTerminatedArray<char8>::AppendN(s.GetList());
}

void  DynamicCString::operator=(char8 const * const &s){
	Truncate(0U);
	DynamicZeroTerminatedArray<char8>::AppendN(s);
}

CStringTool DynamicCString::operator()(){
	return CStringTool(&array,array,DynamicZeroTerminatedArray<char8>::GetMaxIndex()+1);
}

char8 &DynamicCString::operator[](const uint32 index) const{
	return DynamicZeroTerminatedArray<char8>::operator[](index);
}

uint32 DynamicCString::GetSize() const{
	return DynamicZeroTerminatedArray<char8>::GetSize();
}

char8* DynamicCString::GetList() const {
	return DynamicZeroTerminatedArray<char8>::GetList();
}

bool DynamicCString::IsSameAs(const char8 *arrayIn,uint32 limit) const{
	return DynamicZeroTerminatedArray<char8>::IsSameAs(arrayIn,limit);
}



}
#endif /* L0TYPES_DYNAMICCSTRING_H_ */
	
