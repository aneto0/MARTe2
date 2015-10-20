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
     * @brief Tests the constructor by integer number.
     * @param[in] isObject specifies if the type is an object.
     * @param[in] isConst specifies if the type is constant.
     * @param[in] size is the type bit size.
     * @param[in] type is the type.
     * @return True if the TypeDescriptor attributes are filled correctly. False otherwise.
     */
    bool TestConstructorUint(bool isObject,
                             bool isConst,
                             uint16 size,
                             BasicType type);
    /**
     * @brief Tests the constructor by basic types.
     * @param[in] isConst specifies if the type is constant.
     * @param[in] size is the type bit size.
     * @param[in] type is the type.
     * @return True if the TypeDescriptor attributes are filled correctly. False otherwise.
     */
    bool TestConstructorBasicType(bool isConst,
                                  uint16 size,
                                  BasicType type);

    /**
     * @brief Tests the constructor by structured types.
     * @param[in] isConst specifies if the type is constant.
     * @param[in] objCode is the code related to the object.
     * @return True if the TypeDescriptor attributes are filled correctly. False otherwise.
     */
    bool TestConstructorObject(bool isConst,
                               uint16 objCode);

    /**
     * @brief Tests the equal operator.
     * @param[in] size is the code related to the object.
     * @param[in] type is the type.
     * @return True if the operator returns true if between two TypeDescriptors all the fields except the isConstant flag are equal.
     * False otherwise.
     */
    bool TestIsEqualOperator(uint16 size,
                             BasicType type);

    /**
     * @brief Checks if the equal operator fails comparing two TypeDescriptor with different attribute values.
     * @return True if the equal operator fails comparing two TypeDescriptor with different attribute values. False otherwise.
     */
    bool TestIsEqualOperatorFail();

    /**
     * @brief Checks if the attributes are saturated to their maximum value.
     * @return true if initializing attributes with values greater than the maximum allowed by their
     * bit size, the attribute value in TypeDescriptor is saturated. False otherwise.
     */
    bool TestFieldSaturation();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPEDESCRIPTORTEST_H_ */

