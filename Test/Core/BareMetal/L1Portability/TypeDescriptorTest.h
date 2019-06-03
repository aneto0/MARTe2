/**
 * @file TypeDescriptorTest.h
 * @brief Header file for class TypeDescriptorTest
 * @date 03/09/2015
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

 * @details This header file contains the declaration of the class TypeDescriptorTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPEDESCRIPTORTEST_H_
#define TYPEDESCRIPTORTEST_H_

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

using namespace MARTe;

/**
 * @brief Tests all the TypeDescriptor functions.
 */
class TypeDescriptorTest {

public:

    /**
     * @brief Tests the constructor by structured types.
     * @param[in] dataIsConstant specifies if the type is constant.
     * @param[in] structuredDataIdCode is the code related to the object.
     * @return True if the TypeDescriptor attributes are filled correctly. False otherwise.
     */
    bool TestConstructorObject(bool dataIsConstant, uint32 structuredDataIdCode);

    /**
     * @brief Tests the constructor by integer number.
     * @param[in] dataIsConstant specifies if the type is constant.
     * @param[in] objectSize is the type bit size.
     * @param[in] fulltype is the type.
     * @return True if the TypeDescriptor attributes are filled correctly. False otherwise.
     */
    bool TestConstructorFullType(bool dataIsConstant,
                             uint32 objectSize,
							 TD_FullType fulltype);
    /**
     * @brief Tests the constructor by basic types.
     * @param[in] dataIsConstant specifies if the type is constant.
     * @param[in] basicType is one of the of BasicTypes
     * @param[in] basicTypeSize is the code specifying the size of the basictype
     * @param[in] hasBitSize is true if the type is a bit range
     * @param[in] bitOffset is the offset of the bit range
     * @param[in] numberOfBits is the n of bits of the bit range
     * @return True if the TypeDescriptor attributes are filled correctly. False otherwise.
     */
    bool TestConstructorBasicType(bool dataIsConstant,
    		                      TD_FullType basicType,
								  TDBasicTypeSize basicTypeSize,
								  bool hasBitSize,
								  uint8 bitOffset,
								  uint8 numberOfBits);
    /**
     * @brief Tests the equal operator.
     * @param[in] t1,t2 type descriptors to compare
     * @param[in] isSame whether isSameAs is expected to be true
     * @param[in] isSameType whether isSameTypeAs is expected to be true
     * @param[in] isSameTypeAndSize whether isSameTypeAndSizeAs is expected to be true
     * @return True if the operator returns true if between two TypeDescriptors all the fields except the isConstant flag are equal.
     * False otherwise.
     */
    bool TestIsEqualOperator(const TypeDescriptor &t1,const TypeDescriptor &t2, bool isSame, bool isSameType, bool isSameTypeAndSize);

    /**
     * @brief Checks if the attributes are saturated to their maximum value.
     * @return true if initializing attributes with values greater than the maximum allowed by their
     * bit size, the attribute value in TypeDescriptor is saturated. False otherwise.
     */
    bool TestFieldSaturation();

    /**
     * @brief Checks if the function returns the TypeDescriptor associated to the type name input correctly
     */
    bool TestGetTypeDescriptorFromTypeName();

    /**
     * @brief Checks if the function returns the type name associated to the TypeDescriptor input correctly
     */
    bool TestGetTypeNameFromTypeDescriptor();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPEDESCRIPTORTEST_H_ */

