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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "TypeDescriptorTest.h"
#include "BasicType.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestConstructorUint) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorUint(true, false, 10, Float));
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestConstructorBasicType) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorBasicType(false, 10, BT_CCString));
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestConstructorObject) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestConstructorObject(false, 23));
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestIsEqualOperator) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestIsEqualOperator( 21,Pointer));
}


TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestIsEqualOperatorFail) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestIsEqualOperatorFail());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestFieldSaturation) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestFieldSaturation());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestGetTypeDescriptorFromTypeName) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestGetTypeDescriptorFromTypeName());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestGetTypeNameFromTypeDescriptor) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestGetTypeNameFromTypeDescriptor());
}


TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestGetTypeDescriptorFromStaticTable) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestGetTypeDescriptorFromStaticTable());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestGetTypeNameFromStaticTable) {
    TypeDescriptorTest typeDescriptorTest;
    ASSERT_TRUE(typeDescriptorTest.TestGetTypeNameFromStaticTable());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestIsNumericTypeFloat) {
    ASSERT_TRUE(Float64Bit.IsNumericType());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestIsNumericTypeSignedInt) {
    ASSERT_TRUE(SignedInteger8Bit.IsNumericType());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestIsNumericTypeUnsignedInt) {
    ASSERT_TRUE(UnsignedInteger32Bit.IsNumericType());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestIsNumericTypeVoid) {
    ASSERT_TRUE(!VoidType.IsNumericType());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestIsNumericTypeInvalid) {
    ASSERT_TRUE(!InvalidType.IsNumericType());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestIsNumericTypeChar) {
    ASSERT_TRUE(!Character8Bit.IsNumericType());
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestType2TypeDescriptorInt) {
    ASSERT_TRUE(Type2TypeDescriptor<int8>() == SignedInteger8Bit);
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestType2TypeDescriptorUnsignedInt) {
    ASSERT_TRUE(Type2TypeDescriptor<uint32>() == UnsignedInteger32Bit);
}

TEST(BareMetal_L1Portability_TypeDescriptorGTest,TestType2TypeDescriptorFloat) {
    ASSERT_TRUE(Type2TypeDescriptor<float64>() == Float64Bit);
}
