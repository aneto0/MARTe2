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
class CCString: public ZeroTerminatedArray<const char8>{

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
     * TODO
     */
    inline char8 operator[](uint32 index) const;

    /**
     * TODO
     */
    inline bool operator==(const CCString &s) const;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

CCString::CCString (uint32 zero){ }

CCString::CCString (CCString const &s):ZeroTerminatedArray<const char8>(s){}

template <uint32 size>
CCString::CCString (char8 const (&vector) [size]):ZeroTerminatedArray<const char8>(&vector[0]){}

CCString::CCString (char8 const * const &s):ZeroTerminatedArray<const char8>(s){}

char8 
CCString::operator[](uint32 index) const{
    return ZeroTerminatedArray<const char8>::operator[](index);
}

bool CCString::operator==(const CCString &s) const{
	return ZeroTerminatedArray<const char8>::isSameAs(s.GetList());
//	return s.array == array;
}


}
#endif /* L0TYPES_CCSTRING_H_ */
	
