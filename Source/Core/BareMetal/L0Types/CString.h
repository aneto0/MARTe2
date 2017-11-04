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
#include "CCString.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

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
     * TODO
     */
    inline CString (char8 * const &s);

    /**
     * TODO
     */
//    inline operator const char8*() const;
    inline operator CCString() const;

    /**
     * TODO
     */
    inline char8 & operator[](uint32 index) const;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



CString::CString (){
}

CString::CString (CString const &s):ZeroTerminatedArray<char8>(s){

}

CString::CString (char8 * const &s):ZeroTerminatedArray<char8>(s){

}

//CString::operator const char8*() const{
//    return ZeroTerminatedArray<char8>::array;
//}

char8 &
CString::operator[](uint32 index) const{
    return ZeroTerminatedArray<char8>::operator[](index);
}

inline CString::operator CCString() const{
	return CCString(GetList());
}


};

#endif /* CSTRING_H_ */
	
