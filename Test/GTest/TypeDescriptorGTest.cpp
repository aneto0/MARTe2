/**
 * @file TypeDescriptorGTest.cpp
 * @brief Source file for class TypeDescriptorGTest
 * @date 03/set/2015
 * @author pc
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

 * @details This source file contains the definition of all the methods for
 * the class TypeDescriptorGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "TypeDescriptorTest.h"
#include "BasicType.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TypeDescriptorGTest,TestConstructorUint) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorUint(true, false, 10, TypeDefinition::Float));
}

TEST(TypeDescriptorGTest,TestConstructorBasicType) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorBasicType(false, 10, TypeDefinition::CCString));
}

TEST(TypeDescriptorGTest,TestConstructorObject) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorObject(false, 23));
}

TEST(TypeDescriptorGTest,TestIsEqualOperator) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestIsEqualOperator( 21,TypeDefinition::Pointer));
}


TEST(TypeDescriptorGTest,TestIsEqualOperatorFail) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestIsEqualOperatorFail());
}

TEST(TypeDescriptorGTest,TestFieldSaturation) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestFieldSaturation());
}
