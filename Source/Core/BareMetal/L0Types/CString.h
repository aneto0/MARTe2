/**
 * @file CString.h
 * @brief Header file for class CString
 * @date 05/04/2016
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

 * @details This header file contains the declaration of the class CString
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CSTRING_H_
#define CSTRING_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ZeroTerminatedArray.h"
#include "DynamicZeroTerminatedArray.h"
#include "StaticZeroTerminatedArray.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class CString;

class CCString;

/**
* TODO
* @brief Wrapper for writable char buffers
* */
class DynamicCString: public DynamicZeroTerminatedArray<char8,16>{

public:
    /**
     *TODO
     */
    inline DynamicCString ();

    /**
     * @briefs allocates memory and copies the content
     */
    inline DynamicCString (DynamicCString const &s);

    /**
     * @briefs allocates memory and copies the content
     */
    inline DynamicCString (CString const &s);

    /**
     * @briefs allocates memory and copies the content
     */
    inline DynamicCString (CCString const &s);

    /**
     * @briefs allocates memory and copies the content
     */
    inline DynamicCString (char8 * const &s);

};

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
};


/**
* TODO
* @brief Wrapper for writable char buffers
* */
class CString: public ZeroTerminatedArray<char8>{

public:
    /**
     *TODO
     */
    inline CString ();

    /**
     *TODO
     */
    inline CString (CString const &s);

    /**
     *TODO
     */
    inline CString (DynamicCString const &s);

    /**
     *TODO
     */
    template <uint32 size>
    inline CString (StaticCString<size> const &s);

    /**
     * TODO
     */
    inline CString (char8 * const &s);
};

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
    inline CCString (DynamicCString const &s);

    /**
     *TODO
     */
    template <uint32 size>
    inline CCString (StaticCString<size> const &s);

    /**
     * TODO
     */
    inline CCString (char8 const * const &s);
};





/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <uint32 size>
StaticCString<size>::StaticCString (char8 const (&s)[size]){
    ZeroTerminatedArray<char8 >::array = &s[0];
    s[0] = '\0';
}

DynamicCString::DynamicCString (){

}

DynamicCString::DynamicCString(DynamicCString const &s):DynamicZeroTerminatedArray<char8,16>(s){
}

DynamicCString::DynamicCString (CString const &s):DynamicZeroTerminatedArray<char8,16>(CCString(s)){
}

DynamicCString::DynamicCString (CCString const &s):DynamicZeroTerminatedArray<char8,16>(s){
}

DynamicCString::DynamicCString (char8 * const &s):DynamicZeroTerminatedArray<char8,16>(CCString(s)){
}



CString::CString (){
}

CString::CString (CString const &s):ZeroTerminatedArray<char8>(s){

}

CString::CString (char8 * const &s):ZeroTerminatedArray<char8>(s){

}



CCString::CCString (){

}
CCString::CCString (CCString const &s):ZeroTerminatedArray<const char8>(s){

}

CCString::CCString (CString const &s):ZeroTerminatedArray<const char8>(s){

}

CCString::CCString (char8 const * const &s):ZeroTerminatedArray<const char8>(s){

}


};

#endif /* CSTRING_H_ */
	
