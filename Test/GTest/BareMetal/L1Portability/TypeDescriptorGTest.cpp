/**
 * @file TypeDescriptorGTest.cpp
 * @brief Source file for class TypeDescriptorGTest
 * @date 03/09/2015
 * @author Giuseppe Ferro
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

#include "TestSupport.h"
#include "TypeDescriptorTest.h"
#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

#if 0
TEST(TypeDescriptorGTest,TestConstructorUint) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorBasicType(true,TDF_Float,Size32bit,false,0,0));
}

TEST(TypeDescriptorGTest,TestConstructorBasicType) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorFullType(false, sizeof(CCString), TDF_CString));
}

TEST(TypeDescriptorGTest,TestConstructorObject) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorObject(false, 23));
}
#endif

TEST(TypeDescriptorGTest,TestIsEqualOperatorPT) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestIsEqualOperator(PointerType,ConstPointerType,false,true,true));
}

TEST(TypeDescriptorGTest,TestIsEqualOperator) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestIsEqualOperator(FormattedCCString(cdbFormat),ConstCharString,false,true,false));
}

#if 0
TEST(TypeDescriptorGTest,TestFieldSaturation) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestFieldSaturation());
}
#endif

TEST(TypeDescriptorGTest,TestGetTypeDescriptorFromTypeName) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestGetTypeDescriptorFromTypeName());
}

TEST(TypeDescriptorGTest,TestGetTypeNameFromTypeDescriptor) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestGetTypeNameFromTypeDescriptor());
}





