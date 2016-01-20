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
 * @brief Describes one member of a class or structure
 */
class IntrospectionEntry {

public:

    /**
     * @brief Constructor.
     * @param[in] memberNameIn is the member name.
     * @param[in] typeNameIn is the member type name.
     * @param[in] isConstantIn specifies if the member type is constant or not.
     * @param[in] modifiersIn specifies the member attributes.
     * @param[in] sizeIn is the size of the member.
     * @param[in] byteOffsetIn is the address of the member respect to the class begin pointer.
     * @post
     *   GetMemberName() == memberNameIn &&
     *   GetMemberModifiers() == modifiersIn &&
     *   GetMemberSize() == sizeIn &&
     *   GetMemberByteOffset() == byteOffsetIn &&
     *   IsConstant() == isConstantIn
     */
    IntrospectionEntry(const char8 * const memberNameIn,
                       const char8 * const typeNameIn,
                       const bool isConstantIn,
                       const char8* const modifiersIn,
                       const uint32 sizeIn,
                       const uint32 byteOffsetIn);

    /**
     * @brief Retrieves the member name.
     * @return the member name.
     */
    const char8* GetMemberName() const;

    /**
     * @brief Retrieves the TypeDescriptor associated to the member type.
     * @return the TypeDescriptor associated to the member type.
     */
    TypeDescriptor GetMemberTypeDescriptor() const;

    /**
     * @brief Retrieves the member modifiers.
     * @details the string \a modifiers is a list of symbols:
     *   '*'   = pointer;
     *   'C'   = constant;
     *   '[n]' = array of n elements;
     * The string must be a combination of '*' and 'C' with at the end the specification for the array if needed.
     * Assuming that the type \a int and the variable is called \a x, some examples follow:
     *   "**"     --->   double pointer ( int ** x );
     *   "**C"    --->   constant double pointer ( int ** const x );
     *   "*C*C"   --->   constant pointer to constant pointer ( int * const * const x );
     *   "*C*"    --->   pointer to constant pointer ( int * const * x );
     *   "[2]"    --->   array ( int x[2] );
     *   "*[2]"   --->   array of pointers ( int *x[2] );
     *   "[2][2]" --->   double array (int x[2][2] );
     */
    const char8 * GetMemberModifiers() const;

    /**
     * @brief Retrieves the member size.
     * @return the member size in byte.
     */
    uint32 GetMemberSize() const;

    /**
     * @brief Retrieves the offset from the member and its class pointers.
     * @return the offset from the member and its class pointers in byte.
     */
    uint32 GetMemberByteOffset() const;

    /**
     * @brief Specifies if the member type is constant.
     * @return true if the member type is contant, false otherwise.
     */
    bool IsConstant() const;

private:

    /**
     * The name of the class member.
     */
    const char8 *memberName;

    /**
     * The type of the class member.
     */
    const char8* typeName;

    /**
     * A flag to specify if the type is constant.
     */
    bool isConstant;

    /**
     * The member modifiers.
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
    const char8 *modifiers;

    /**
     * The size of the field
     */
    uint32 size;

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

