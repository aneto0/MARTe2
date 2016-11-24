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
 *
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

#include "../L1Portability/TypeDescriptor_old.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Describes one member of a class or structure
 */
class DLL_API IntrospectionEntry {

public:

    /**
     * @brief Default constructor
     * @post
     *   GetMemberName() == NULL &&
     *   GetMemberTypeName() == NULL &&
     *   GetMemberModifiers() == NULL &&
     *   GetMemberAttributes() == NULL &&
     *   GetMemberSize() == 0 &&
     *   GetMemberByteOffset() == 0
     */
    IntrospectionEntry();

    /**
     * @brief Constructor.
     * @param[in] memberNameIn is the member name.
     * @param[in] typeNameIn is the member type name.
     * @param[in] modifiersIn specifies the member modifiers (@see GetMemberModifiers).
     * @param[in] attributesIn specifies the member attributes (@see GetMemberAttributes).
     * @param[in] sizeIn is the size of the member.
     * @param[in] byteOffsetIn is the relative address of the member with respect to the class beginning pointer.
     * @post
     *   GetMemberName() == memberNameIn &&
     *   GetMemberTypeName() == typeNameIn &&
     *   GetMemberModifiers() == modifiersIn &&
     *   GetMemberAttributes() == attributesIn &&
     *   GetMemberSize() == sizeIn &&
     *   GetMemberByteOffset() == byteOffsetIn
     */
    IntrospectionEntry(const char8 * const memberNameIn,
                       const char8 * const typeNameIn,
                       const char8* const modifiersIn,
                       const char8* const attributesIn,
                       const uint32 sizeIn,
                       const uint32 byteOffsetIn);

    /**
     * @brief Retrieves the member name.
     * @return the member name.
     */
    const char8* GetMemberName() const;

    /**
     * @brief Retrieves the member type name.
     * @return the member type name.
     */
    const char8 *GetMemberTypeName() const;

    /**
     * @brief Retrieves the TypeDescriptor associated to the member type.
     * @return the TypeDescriptor associated to the member type.
     */
    TypeDescriptor GetMemberTypeDescriptor() const;

    /**
     * @brief Retrieves the member modifiers.
     * @details The string \a modifiers is a list of symbols:
     *
     *   '*'   = pointer;\n
     *   'C'   = constant;\n
     *   '[n]' = array of n elements;
     *
     * The string must be a combination of '*'s and 'C's terminated by, if applicable, the specification for the array size.
     * Assuming that the type \a int and that the variable is called \a x, some examples follow:
     *
     *   "**"     --->   double pointer ( int ** x );\n
     *   "**C"    --->   constant double pointer ( int ** const x );\n
     *   "C*C*C"  --->   constant pointer to constant pointer to contant integer ( int const * const * const x );\n
     *   "*C*C"   --->   constant pointer to constant pointer ( int * const * const x );\n
     *   "*C*"    --->   pointer to constant pointer ( int * const * x );\n
     *   "[2]"    --->   array ( int x[2] );\n
     *   "*[2]"   --->   array of pointers ( int *x[2] );\n
     *   "[2][2]" --->   double array (int x[2][2] );
     *
     *   @return the member modifiers.
     */
    const char8 * GetMemberModifiers() const;

    /**
     * @brief Retrieves the member attributes.
     * @details The string attributes must be written in the standard MARTe configuration language and provides a series
     * of attributes to be associated to a basic type member. Most common attributes are:
     *
     *   min        = ...    ---> the minimum value\n
     *   max        = ...    ---> the maximum value\n
     *   values     = {...}  ---> the list of possible values\n
     *   min_size   = ...    ---> the minimum size\n
     *   max_size   = ...    ---> the maximum size
     *
     * The functions in the class ValidateBasicType will take as input these attributes to check if the value to associate to the class
     * member respects the constraints returned by this function.
     *
     * @return the member attributes.
     */
    const char8 * GetMemberAttributes() const;

    /**
     * @brief Retrieves the member size.
     * @return the member size in byte.
     */
    uint32 GetMemberSize() const;

    /**
     * @brief Retrieves the offset from the member with respect to beginning of the class pointer.
     * @return the offset from the member with respect to beginning of the class pointer.
     */
    uint32 GetMemberByteOffset() const;

    /**
     * @brief Specifies if the member type is constant.
     * @param[in] ptrLevel specifies the pointer level to be checked in case the member is a n-th pointer.
     *
     * Example for "const int ** const x":
     *
     *   ptrLevel == 0 --> true (the final type is constant) \n
     *   ptrLevel == 1 --> false (the first pointer is not constant)\n
     *   ptrLevel == 2 --> true (the second pointer is constant) \n
     *
     * If ptrLevel > GetMemberPointerLevel() returns the same result as IsConstant(0) (i.e the final type will be checked).
     * @return true if the member type is constant, false otherwise.
     */
    bool IsConstant(const uint32 ptrLevel = 0u) const;

    /**
     * @brief Retrieves the pointer level of the member (i.e for "int **x" the pointer level is 2).
     * @return the pointer level of the member.
     */
    uint32 GetMemberPointerLevel() const;

    /**
     * @brief Retrieves the number of dimensions related to the member.
     * @details
     *
     *   GetNumberOfDimensions() == 0 --> scalar\n
     *   GetNumberOfDimensions() == 1 --> array\n
     *   GetNumberOfDimensions() == 2 --> matrix\n
     *   GetNumberOfDimensions() == 3 --> mesh
     *
     * @return Retrieves the number of dimensions related to the member.
     */
    uint8 GetNumberOfDimensions() const;

    /**
     * @brief Retrieves the number of elements in a given dimension.
     * @param[in] dimension is the dimension number.
     * @return the number of elements for the dimension specified in input.
     */
    uint32 GetNumberOfElements(const uint32 dimension) const;

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
     * The member modifiers.
     */
    const char8 *modifiers;

    /**
     * The member attributes.
     */
    const char8 *attributes;

    /**
     * The size of the field.
     */
    uint32 size;

    /**
     * The offset from the start of the class of the member as bytes.
     */
    uint32 byteOffset;

    /**
     * The number of dimensions.
     */
    uint8 numberOfDimensions;

    /**
     * The number of elements for each dimension.
     */
    uint32 dimensionSize[3];

};

/**
 * @brief The invalid IntrospectionEntry.
 */
static const IntrospectionEntry InvalidIntrospectionEntry(static_cast<const char8*>(NULL), static_cast<const char8*>(NULL), static_cast<const char8*>(NULL),static_cast<const char8*>(NULL), 0u, 0u);

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTROSPECTIONENTRY_H_ */

