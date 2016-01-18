/**
 * @file IntrospectionEntry.h
 * @brief Header file for class IntrospectionEntry
 * @date 18/01/2016
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class IntrospectionEntry
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTIONENTRY_H_
#define INTROSPECTIONENTRY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * Describes one member of a class/structure
 */
struct IntrospectionEntry {

    /**
     * The name of the class member
     */
    const char8 * memberName;

    /**
     * The type of the class member
     */
    TypeDescriptor type;

    /**
     * Modifiers:
     *   space separated only necessary to separate numbers
     *   '*' pointer
     *   'nnnn' vector of
     *   Z zero terminated vector of
     *   C constant
     * Examples
     *   int            --> ""
     *   int *          --> "*"
     *   int[3]         --> "3"
     *   int *a[3]      --> "3*"
     *   int **         --> "**"
     *   int [3] *a     --> "3*"  // if C supported it!!!
     *   const int      --> ""    // const is in the typeDescriptor
     *   const int *    --> "*"
     *   int * const    --> "C*"
     *   const int * *  --> "**"
     *   int* const *   --> "*C*"
     *   int* * const   --> "**C"
     *   int* const * const   --> "*C*C"
     *   int[3][4]      --> "4 3"
     *   CString        --> "Z"  (char as type)
     *   const CString  --> "Z"  (const char as type)
     */
    const char8* modifiers;

    /**
     * offset from the start of the class of the member as bytes
     */
    uint32 byteOffset;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTROSPECTIONENTRY_H_ */

