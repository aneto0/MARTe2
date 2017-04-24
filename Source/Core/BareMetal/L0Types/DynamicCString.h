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


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

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
    inline DynamicCString (char8 const * const &s);

    /**
     * TODO
     */
    inline operator CCString() const;

    /**
     * TODO
     */
    inline bool operator==(const CCString &s) const;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

DynamicCString::DynamicCString (){

}

DynamicCString::DynamicCString(DynamicCString const &s):DynamicZeroTerminatedArray<char8,16>(s){
}

DynamicCString::DynamicCString (char8 const * const &s):DynamicZeroTerminatedArray<char8,16>(CCString(s)){
}

DynamicCString::operator CCString() const{
    return CCString(GetList());
}

bool DynamicCString::operator==(const CCString &s) const{
	return isSameAs(s.GetList());
}

}
#endif /* L0TYPES_DYNAMICCSTRING_H_ */
	
