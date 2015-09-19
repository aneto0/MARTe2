/**
 * @file AnyTypeTest.h
 * @brief Header file for class AnyTypeTest
 * @date 31/08/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class AnyTypeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L0TYPEDEV_ANYTYPETEST_H_
#define TEST_CORE_L0TYPEDEV_ANYTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace TypeDefinition;

class AnyTypeTest {
public:

    /**
     * AnyTypeTest constructor.
     */
    AnyTypeTest();

    /**
     * @brief Test AnyType default constructor
     * @return True if GetDataPointer() == NULL && GetBitAddress() == 0 && GetTypeDescriptor() == {false, false, 0, 0}
     */
    bool TestAnyType_Void();

    /**
     * @brief Test AnyType constructor using anyType as an input.
     * @return True if GetDataPointer() == NULL && GetBitAddress() == 0 && GetTypeDescriptor() == {false, false, 0, 0}
     */
    bool TestAnyType_AnyTypeVoid();

    /**
     * @brief Test AnyType constructor defining the TypeDescriptor the bitAddress and the constant pointer to a constant.
     * @return True if the type is set as expected.
     */
    bool TestAnyType_TypeDescriptor_BitAddress_ConstPointerToConst();

    /**
     * @brief Test AnyType constructor defining the TypeDescriptor the bitAddress and the constant pointer.
     * @return True if the type is set as expected.
     */
    bool TestAnyType_TypeDescriptor_BitAddress_ConstPointer();

    /**
     * @brief Test the AnyType::IsVoid.
     * @details Use this function twice one with a void and another with a different type.
     * @return True if IsVoid() returns true for a void type and false for a different type.
     */
    bool TestIsVoid();

    /**
     * @brief Test the AnyType constructor using int8.
     * @return True if the type is set as expected.
     */
    bool TestAnyType_Int8();

    /**
     * @brief Test AnyType constructor using uint8.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_UInt8();

    /**
     * @brief Test AnyType constructor using const int8.
     * @return True if the anyTYpe is constructed as expected.
     */
    bool TestAnyType_ConstInt8();

    /**
     * @brief Test AnyType constructor using const uint8.
     * @return True if anyType is constructed as expected.
     */
    bool TestAnyType_ConstUInt8();

    /**
     * @brief Test AnyType constructor using int16.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_Int16();

    /**
     * @brief Test AnyType constructor using uint16.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_UInt16();

    /**
     * @brief Test AnyType constructor using const int16.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstInt16();

    /**
     * @brief Test AnyType constructor using const uint16.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstUInt16();

    /**
     * @brief Test AnyType constructor using int32.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_Int32();

    /**
     * @brief Test AnyType constructor using uint32.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_UInt32();

    /**
     * @brief Test AnyType constructor using const int32.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstInt32();

    /**
     * @brief Test AnyType constructor using const uint32.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstUInt32();

    /**
     * @brief Test AnyType constructor using int64.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_Int64();

    /**
     * @brief Test AnyType constructor using uint64.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_UInt64();

    /**
     * @brief Test AnyType constructor using const int64.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstInt64();

    /**
     * @brief Test AnyType constructor using const uint64.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstUInt64();

    /**
     * @brief Test AnyType constructor using float32.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_Float32();

    /**
     * @brief Test AnyType constructor using const float32.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstFloat32();

    /**
     * @brief Test AnyType constructor using float64.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_Float64();

    /**
     * @brief Test AnyType constructor using const float64.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstFloat64();

    /**
     * @brief Test AnyType constructor using const void* const pointer.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstPointerToConts();

    /**
     * @brief Test AnyType constructor using void* const pointer.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstPointer();

    /**
     * @brief Test AnyType constructor using const char *const pointer.
     * @return True if the anyType is constructed as expected.
     */
    bool TestAnyType_ConstCharPointerToConst();

    //TODO
    template<typename T>
    bool TestAnyTypeCastFromBitBoolean();

    /**
     * @brief Tests the AnyType cast by constant FractionalInteger.
     * @return true if all AnyType attributes are initialized correctly.
     */
    template<typename T>
    bool TestAnyTypeCastNonConstFromFractionalInteger();

    /**
     * @brief Tests the AnyType cast by FractionalInteger.
     * @return true if all AnyType attributes are initialized correctly.
     */
    template<typename T>
    bool TestAnyTypeCastConstFromFractionalInteger();

    /**
     * @brief Tests the cast to AnyType.
     * @return true if the AnyType attributes are initialized correctly. False otherwise.
     */
    template<typename T>
    bool TestAnyTypeCastFromBitRange();

    /**
     * @brief Tests the cast to const AnyType.
     * @return true if the AnyType attributes are initialized correctly. False otherwise.
     */
    template<typename T>
    bool TestAnyTypeCastConstFromBitRange();

private:

    /**
     * Target variable. Only used to be pointed to.
     */
    const int32 targetVariable;

    /**
     * Returned value of the function
     */
    bool retVal;

    /**
     * TypeDescriptor variable used to save the information of anyType.
     */
    TypeDescriptor td;

    /**
     * Default arbitrary bit address
     */
    uint8 defaultBitAddress;

    /**
     * Default arbitrary TypeDescriptor variable used to initialize AnyType.
     */
    TypeDescriptor defaultDataDescription;

    /**
     * Constant void pointer to constant used to initialize AnyType.
     */
    const void* const constantPtrToConstant;

    /**
     * Constant void pointer used to initialize AnyType.
     */
    void* const constPtr;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
bool AnyTypeTest::TestAnyTypeCastFromBitBoolean() {

    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;
    const uint8 zero = 0;

    TypeDefinition::BitBoolean<T, zero> myZeroShiftedBool;
    myZeroShiftedBool = true;

    TypeDefinition::AnyType atTest = myZeroShiftedBool;

    if (atTest.GetDataPointer() != (&myZeroShiftedBool)) {
        return false;
    }

    TypeDefinition::TypeDescriptor tdTest = atTest.GetTypeDescriptor();

    if (tdTest.isStructuredData || tdTest.isConstant || tdTest.type != TypeDefinition::UnsignedInteger || tdTest.size != 1) {
        return false;
    }

    if (atTest.GetBitAddress() != zero) {
        return false;
    }

    TypeDefinition::BitBoolean<T, half> myHalfShiftedBool;

    myHalfShiftedBool = true;
    atTest = myHalfShiftedBool;

    if (atTest.GetDataPointer() != (&myHalfShiftedBool)) {
        return false;
    }

    tdTest = atTest.GetTypeDescriptor();
    if (tdTest.isStructuredData || tdTest.isConstant || tdTest.type != TypeDefinition::UnsignedInteger || tdTest.size != 1) {
        return false;
    }

    if (atTest.GetBitAddress() != half) {
        return false;
    }

    TypeDefinition::BitBoolean<T, max> myMaxShiftedBool;

    myMaxShiftedBool = true;
    atTest = myMaxShiftedBool;

    if (atTest.GetDataPointer() != (&myMaxShiftedBool)) {
        return false;
    }

    tdTest = atTest.GetTypeDescriptor();
    if (tdTest.isStructuredData || tdTest.isConstant || tdTest.type != TypeDefinition::UnsignedInteger || tdTest.size != 1) {
        return false;
    }

    if (atTest.GetBitAddress() != max) {
        return false;
    }

    return true;
}

template<typename T>
bool AnyTypeTest::TestAnyTypeCastNonConstFromFractionalInteger() {

    const uint8 size = 8;

    TypeDefinition::FractionalInteger<T, size> myFractionalInteger;
    myFractionalInteger = 0;

    TypeDefinition::AnyType atTest = myFractionalInteger;

    if (atTest.GetDataPointer() != (&myFractionalInteger)) {
        return false;
    }

    TypeDefinition::TypeDescriptor tdTest = atTest.GetTypeDescriptor();
    bool isSigned = T(-1) < 0;

    TypeDefinition::BasicType type = (isSigned) ? TypeDefinition::SignedInteger : TypeDefinition::UnsignedInteger;

    if ((tdTest.isStructuredData) || (tdTest.isConstant) || (tdTest.type != type) || (tdTest.size != size)) {
        return false;
    }

    if (atTest.GetBitAddress() != 0) {
        return false;
    }

    return true;
}

template<typename T>
bool AnyTypeTest::TestAnyTypeCastConstFromFractionalInteger() {
    const uint8 size = 8;

    const TypeDefinition::FractionalInteger<T, size> myFractionalInteger(0);

    TypeDefinition::AnyType atTest = myFractionalInteger;

    if (atTest.GetDataPointer() != (&myFractionalInteger)) {
        return false;
    }

    TypeDefinition::TypeDescriptor tdTest = atTest.GetTypeDescriptor();

    bool isSigned = T(-1) < 0;

    TypeDefinition::BasicType type = (isSigned) ? TypeDefinition::SignedInteger : TypeDefinition::UnsignedInteger;

    if ((tdTest.isStructuredData) || (!tdTest.isConstant) || (tdTest.type != type) || (tdTest.size != size)) {
        return false;
    }

    if (atTest.GetBitAddress() != 0) {
        return false;
    }

    return true;
}

template<typename T>
bool AnyTypeTest::TestAnyTypeCastConstFromBitRange() {
    const uint8 max = sizeof(T) * 8 - 1;
    const uint8 half = max / 2;

    const uint8 size = 8;

    TypeDefinition::BitRange<T, size, half> myBitRange;
    myBitRange = 0;

    TypeDefinition::AnyType atTest = myBitRange;

    if (atTest.GetDataPointer() != (&myBitRange)) {
        return false;
    }

    TypeDefinition::TypeDescriptor tdTest = atTest.GetTypeDescriptor();
    bool isSigned = T(-1) < 0;

    TypeDefinition::BasicType type = (isSigned) ? TypeDefinition::SignedInteger : TypeDefinition::UnsignedInteger;

    if ((tdTest.isStructuredData) || (tdTest.isConstant) || (tdTest.type != type) || (tdTest.size != size)) {
        return false;
    }

    if (atTest.GetBitAddress() != half) {
        return false;
    }

    return true;
}

#endif /* TEST_CORE_L0TYPEDEV_ANYTYPETEST_H_ */

