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

#include "../../../Source/Core/L0TypeDev/AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace TypeDefinition;

class AnyTypeTest {
public:
    AnyTypeTest();
    virtual ~AnyTypeTest();

    const int32 dummy;

    bool retVal;

    TypeDescriptor td;

    uint8 defaultBitAddress;

    TypeDescriptor defaultDataDescription;

    const void* const constantPtrToConstant;

    void* const constPtr;

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

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L0TYPEDEV_ANYTYPETEST_H_ */

