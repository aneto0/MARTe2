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

    /**
     * @briefs uses the memory provided by s
     * s is reset to length 0;
    */
    inline StaticCString (char8 const (&s)[size]);

    /*
     * TODO
     */
    operator char8*() const;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <uint32 size>
StaticCString<size>::StaticCString (char8 const (&s)[size]){
    ZeroTerminatedArray<char8 >::array = &s[0];
    s[0] = '\0';
}

template <uint32 size>
StaticCString<size>::operator char8*() const{
    return ZeroTerminatedArray<char8>::array;
}


}
#endif /* L0TYPES_STATICCSTRING_H_ */
	
