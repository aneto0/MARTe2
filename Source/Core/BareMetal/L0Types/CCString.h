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

#include "CString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class DynamicCString;
template <uint32 size> class StaticCString;


/**
 * TODO
 * @brief Wrapper for constant char buffers
 * */
class CCString: public ZeroTerminatedArray<const char8>{

public:
    /**
     *TODO
     */
    inline CCString ();

    /**
     * TODO
     */
    inline CCString (CCString const &s);

    /**
     * TODO
     */
    inline CCString (CString const &s);

    /**
     * TODO
     */
//    inline CCString (DynamicCString const &s);

    /**
     *TODO
     */
//    template <uint32 size>
//    inline CCString (StaticCString<size> const &s);

    /**
     * TODO
     */
    inline CCString (char8 const * const &s);

    /**
     * TODO
     */
    operator const char8*() const;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

CCString::CCString (){}

CCString::CCString (CCString const &s):ZeroTerminatedArray<const char8>(s){}

CCString::CCString (CString const &s):ZeroTerminatedArray<const char8>(s){}

CCString::CCString (char8 const * const &s):ZeroTerminatedArray<const char8>(s){}

CCString::operator const char8*() const{
    return ZeroTerminatedArray<const char8>::array;
}

}
#endif /* L0TYPES_CCSTRING_H_ */
	
