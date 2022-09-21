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
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
class AnyTypeTest {
public:

    /**
     * AnyTypeTest constructor.
     */
    AnyTypeTest();

    /**
     * @brief Test AnyType default constructor
     * @return true if GetDataPointer() == NULL && GetBitAddress() == 0 && GetTypeDescriptor() == {false, false, 0, 0}
     */
    bool TestAnyType_Void();

    /**
     * @brief Test AnyType constructor using anyType as an input.
     * @return true if GetDataPointer() == NULL && GetBitAddress() == 0 && GetTypeDescriptor() == {false, false, 0, 0}
     */
    bool TestAnyType_AnyTypeVoid();

    /**
     * @brief Test AnyType constructor defining the TypeDescriptor, the bitAddress and the constant pointer to a constant.
     * @return true if the type is set as expected.
     */
    bool TestAnyType_TypeDescriptor_BitAddress_ConstPointerToConst();

    /**
     * @brief Test AnyType constructor defining the TypeDescriptor the bitAddress and the constant pointer.
     * @return true if the type is set as expected.
     */
    bool TestAnyType_TypeDescriptor_BitAddress_ConstPointer();

    /**
     * @brief Test the AnyType::IsVoid.
     * @details Use this function twice one with a void and another with a different type.
     * @return true if IsVoid() returns true for a void type and false for a different type.
     */
    bool TestIsVoid();

    /**
     * @brief Test the AnyType constructor using int8.
     */
    bool TestAnyType_Int8();

    /**
     * @brief Test AnyType constructor using uint8.
     */
    bool TestAnyType_UInt8();

    /**
     * @brief Test AnyType constructor using const int8.
     */
    bool TestAnyType_ConstInt8();

    /**
     * @brief Test AnyType constructor using const uint8.
     */
    bool TestAnyType_ConstUInt8();

    /**
     * @brief Test AnyType constructor using int16.
     */
    bool TestAnyType_Int16();

    /**
     * @brief Test AnyType constructor using uint16.
     */
    bool TestAnyType_UInt16();

    /**
     * @brief Test AnyType constructor using const int16.
     */
    bool TestAnyType_ConstInt16();

    /**
     * @brief Test AnyType constructor using const uint16.
     */
    bool TestAnyType_ConstUInt16();

    /**
     * @brief Test AnyType constructor using int32.
     */
    bool TestAnyType_Int32();

    /**
     * @brief Test AnyType constructor using uint32.
     */
    bool TestAnyType_UInt32();

    /**
     * @brief Test AnyType constructor using bool.
     */
    bool TestAnyType_Bool();

    /**
     * @brief Test AnyType constructor using const int32.
     */
    bool TestAnyType_ConstInt32();

    /**
     * @brief Test AnyType constructor using const uint32.
     */
    bool TestAnyType_ConstUInt32();

    /**
     * @brief Test AnyType constructor using const bool.
     */
    bool TestAnyType_ConstBool();


    /**
     * @brief Test AnyType constructor using int64.
     */
    bool TestAnyType_Int64();

    /**
     * @brief Test AnyType constructor using uint64.
     */
    bool TestAnyType_UInt64();

    /**
     * @brief Test AnyType constructor using const int64.
     */
    bool TestAnyType_ConstInt64();

    /**
     * @brief Test AnyType constructor using const uint64.
     */
    bool TestAnyType_ConstUInt64();

    /**
     * @brief Test AnyType constructor using float32.
     */
    bool TestAnyType_Float32();

    /**
     * @brief Test AnyType constructor using const float32.
     */
    bool TestAnyType_ConstFloat32();

    /**
     * @brief Test AnyType constructor using float64.
     */
    bool TestAnyType_Float64();

    /**
     * @brief Test AnyType constructor using const float64.
     */
    bool TestAnyType_ConstFloat64();

    /**
     * @brief Test AnyType constructor using const void* const pointer.
     */
    bool TestAnyType_ConstPointerToConts();

    /**
     * @brief Test AnyType constructor using void* const pointer.
     */
    bool TestAnyType_ConstPointer();

    /**
     * @brief Test AnyType constructor using const char *const pointer.
     */
    bool TestAnyType_ConstCharPointerToConst();


    bool TestAnyType_Char8();

    bool TestAnyType_ConstChar8();

    /**
     * @brief Tests the AnyType constructor with a BitBoolean variable.
     */
    template<typename baseType>
    bool TestAnyType_BitBoolean();

    /**
     * @brief Tests the AnyType constructor with a BitRange variable.
     */
    template<typename baseType>
    bool TestAnyType_BitRange();

    /**
     * @brief Tests the AnyType constructor with a FractionalInteger variable.
     */
    template<typename baseType>
    bool TestAnyType_FractionalInteger();

    /**
     * @brief Tests the AnyType constructor with a ConstFractionalInteger variable.
     */
    template<typename baseType>
    bool TestAnyType_ConstFractionalInteger();
    /**
     * @brief Tests the AnyType constructor with an Object variable.
     */
    bool TestAnyType_Object();

    /**
     * @brief Tests the AnyType constructor with a const Object variable.
     */
    bool TestAnyType_ConstObject();

    /**
     * @brief Tests the AnyType::CreateFromOtherType function with an element that is registered in the database.
     */
    bool TestCreateFromOtherType();

    /**
     * @brief Tests the AnyType::CreateFromOtherType function with an element that is registered in the database.
     */
    bool TestCreateFromOtherConstType();

    /**
     * @brief Tests if the function sets the data pointer.
     */
    bool TestSetDataPointer();

    /**
     * @brief Tests if the function returns the data pointer.
     */
    bool TestGetDataPointer();

    /**
     * @brief Tests if the function returns the type descriptor.
     */
    bool TestGetTypeDescriptor();

    /**
     * @brief Tests if the function sets the type descriptor.
     */
    bool TestSetTypeDescriptor();

    /**
     * @brief Tests if the function returns the bit address.
     */
    bool TestGetBitAddress();

    /**
     * @brief Tests if the function sets correctly the number of dimensions.
     */
    bool TestSetNumberOfDimensions();

    /**
     * @brief Tests if the function returns correctly the number of dimensions.
     */
    bool TestGetNumberOfDimensions();

    /**
     * @brief Tests if the function sets correctly the number of elements.
     */
    bool TestSetNumberOfElements();

    /**
     * @brief Tests if the function returns correctly the number of dimensions.
     */
    bool TestGetNumberOfElements();

    /**
     * @brief Tests if the function sets correctly the isDeclared flag.
     */
    bool TestSetStaticDeclared();

    /**
     * @brief Tests if the function returns true if AnyType is declared static and false otherwise.
     */
    bool TestIsStaticDeclared();

    /**
     * @brief Tests if the function returns correctly the bit size.
     */
    bool TestGetBitSize();

    /**
     * @brief Tests if the function returns correctly the byte size.
     */
    bool TestGetByteSize();
    
    /**
     * @brief Tests if the function returns correctly the total size of the data.
     */
    bool TestGetDataSize();

    /**
     * @brief Tests if the operator works for matrix of structures static declared
     */
    bool TestPositionOperator_MatrixStructuredStaticDeclared();

    /**
     * @brief Tests if the operator works for matrix of structures not static declared
     */
    bool TestPositionOperator_MatrixStructuredHeapDeclared();

    /**
     * @brief Tests if the operator works for matrix of basic types static declared
     */
    bool TestPositionOperator_MatrixBasicStaticDeclared();

    /**
     * @brief Tests if the operator works for matrix of basic types not static declared
     */
    bool TestPositionOperator_MatrixBasicHeapDeclared();

    /**
     * @brief Tests if the operator works for vector of structures
     */
    bool TestPositionOperator_VectorStructured();

    /**
     * @brief Tests if the operator works for vector of basic types
     */
    bool TestPositionOperator_VectorBasic();

    /**
     * @brief Tests if the operator works for vector of strings
     */
    bool TestPositionOperator_VectorCString();

    /**
     * @brief Tests if the operator works for vector of pointers
     */
    bool TestPositionOperator_VectorPointer();


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
template<typename baseType>
bool AnyTypeTest::TestAnyType_BitBoolean() {
    BitBoolean<baseType, 2> bitBoolean;
    AnyType anytype(bitBoolean);
    TypeDescriptor td = anytype.GetTypeDescriptor();
    retVal = (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == UnsignedInteger);
    retVal &= (td.numberOfBits == 1u);
    retVal &= (anytype.GetBitAddress() == bitBoolean.BitOffset());
    retVal &= (anytype.GetDataPointer() == static_cast<void *>(&bitBoolean));

    return retVal;
}

template<typename baseType>
bool AnyTypeTest::TestAnyType_BitRange() {
    BitRange<baseType, sizeof(baseType), 2> bitRange;
    AnyType anytype(bitRange);

    BasicType type = (TypeCharacteristics<baseType>::IsSigned()) ? SignedInteger : UnsignedInteger;
    TypeDescriptor td = anytype.GetTypeDescriptor();
    retVal = (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == type);
    retVal &= (td.numberOfBits == sizeof(baseType));
    retVal &= (anytype.GetBitAddress() == bitRange.BitOffset());
    retVal &= (anytype.GetDataPointer() == static_cast<void *>(&bitRange));

    return retVal;
}

template<typename baseType>
bool AnyTypeTest::TestAnyType_FractionalInteger() {
    FractionalInteger<baseType, sizeof(baseType)> fractionalInteger;
    AnyType anytype(fractionalInteger);

    BasicType type = (TypeCharacteristics<baseType>::IsSigned()) ? SignedInteger : UnsignedInteger;
    TypeDescriptor td = anytype.GetTypeDescriptor();
    retVal = (td.isStructuredData == false);
    retVal &= (td.isConstant == false);
    retVal &= (td.type == type);
    retVal &= (td.numberOfBits == sizeof(baseType));
    retVal &= (anytype.GetBitAddress() == 0);
    retVal &= (anytype.GetDataPointer() == static_cast<void *>(&fractionalInteger));

    return retVal;
}

template<typename baseType>
bool AnyTypeTest::TestAnyType_ConstFractionalInteger() {
    const FractionalInteger<baseType, sizeof(baseType)> fractionalInteger;
    AnyType anytype(fractionalInteger);

    BasicType type = (TypeCharacteristics<baseType>::IsSigned()) ? SignedInteger : UnsignedInteger;
    TypeDescriptor td = anytype.GetTypeDescriptor();
    retVal = (td.isStructuredData == false);
    retVal &= (td.isConstant == true);
    retVal &= (td.type == type);
    retVal &= (td.numberOfBits == sizeof(baseType));
    retVal &= (anytype.GetBitAddress() == 0);
    void *dataPointer = (const_cast<FractionalInteger<baseType, sizeof(baseType)> *>(&fractionalInteger));
    retVal &= (anytype.GetDataPointer() == dataPointer);

    return retVal;
}

#endif /* TEST_CORE_L0TYPEDEV_ANYTYPETEST_H_ */

