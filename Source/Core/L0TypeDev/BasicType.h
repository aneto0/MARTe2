/**
 * @file BasicType.h
 * @brief Header file for class BasicType
 * @date Aug 28, 2015
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

 * @details This header file contains the declaration of the class BasicType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICTYPE_H_
#define BASICTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {


/**
 * guarantees that only 4 bits are used
 * cannot use BitFields as they need TypeDescriptor
 */
class BasicType{

    uint8 value;

public:

    /**
     *
     */
    template <typename T> BasicType(T x){
        if (x > static_cast<T>(15)) {
            value = 15;
        } else {
            if (x < static_cast<T>(0)){
                value = 0;
            } else {
                value = x;
            }
        }
    }

    operator uint8() const {
        return value ;
    }
};



/**
 * The type is signed.
 */
const BasicType SignedInteger = 0u;

/**
 * The type is unsigned.
 */
const BasicType UnsignedInteger = 1u;

/**
 * The type is a float number.
 */
const BasicType Float = 2u;

/**
 * The type is a pointer.
 */
const BasicType Pointer = 3u;

/**
 * The type is a C string
 * (const char *)
 */
const BasicType CCString = 8u;

/**
 * The type is a pointer to a C string allocated on heap.
 * (char **)
 */
const BasicType PCString = 9u;

/**
 * The type is an array of bytes with the size
 * specified in the size field.
 */
const BasicType CArray = 10u;

/**
 * The type is a StreamString class.
 */
const BasicType StreamString = 11u;

/**
 * The type is a StreamInterface class.
 */
const BasicType StreamInterface = 12u;

}
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L0TYPEDEV_BASICTYPE_H_ */
	
