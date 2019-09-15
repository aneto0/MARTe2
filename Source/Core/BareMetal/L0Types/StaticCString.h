/**
 * @file StaticCString.h
 * @brief Header file for class StaticCString
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

 * @details This header file contains the declaration of the class StaticCString
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPES_STATICCSTRING_H_
#define L0TYPES_STATICCSTRING_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StaticZeroTerminatedArray.h"
#include "CCString.h"
#include "CStringTool.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
* TODO
* @brief Wrapper for writable char buffers
* */
template<uint32 size>
class StaticCString: public StaticZeroTerminatedArray<char8,size>{

public:
    /*
     * TODO
     */
    inline operator CCString() const;

    /**
     * @briefs uses the memory provided by s
     * s is reset to length 0;
    */
    inline StaticCString (char8  (&s)[size]);

    /**
     *
     */
    inline CStringTool operator()();

    /**
     * @brief Retrieves the size of the TArray().
     * @return the number of elements in the TArray() (excluding the terminator Zero).
     */
    inline uint32 GetSize() const;

    /**
     * @brief Retrieves the maximum size of the string that can be stored here
     * @return the number of elements of the buffer -1
     */
    inline uint32 MaximumSize() const;

    /**
     * @brief Returns the pointer to the beginning of the TArray().
     * @return the pointer to the beginning of the TArray().
     */
    inline char8 * GetList() const ;


};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <uint32 size>
StaticCString<size>::StaticCString (char8  (&s)[size]):StaticZeroTerminatedArray<char8,size >(s){
//    ZeroTerminatedArray<char8 >::array = &s[0];
    s[0] = '\0';
}

template <uint32 size>
CStringTool StaticCString<size>::operator()(){
	return CStringTool(NULL_PTR(char8 **),ZeroTerminatedArray<char8>::array ,size);
}

template <uint32 size>
StaticCString<size>::operator CCString() const{
	return CCString(ZeroTerminatedArray<char8>::array);
}

template <uint32 size>
uint32 StaticCString<size>::GetSize() const{
	return StaticZeroTerminatedArray<char8,size >::GetSize();
}

template <uint32 size>
inline uint32 StaticCString<size>::MaximumSize() const{
	return size-1;
}

template <uint32 size>
char8 * StaticCString<size>::GetList() const {
	return StaticZeroTerminatedArray<char8,size >::GetList();
}



}
#endif /* L0TYPES_STATICCSTRING_H_ */
	
