/**
 * @file AnyType.h
 * @brief Header file for class AnyType
 * @date 26/08/2015
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ANYTYPE_H_
#define ANYTYPE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "ClassRegistryDatabase.h"
#include "TypeDescriptor.h"
#include "BitBoolean.h"
#include "FractionalInteger.h"
#include "BitRange.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace TypeDefinition {

/**
 * @brief AnyType class.
 *
 * @details This class provides a smart mechanism for the manipulation of types.\n
 * It is composed by a void* pointer to the element and by a descriptor
 * which gives information about the type (the size, if it is a signed or
 * unsigned number, if it is constant, if it is an object or a native element, etc).\n
 * For each type an implicit constructor can automatically build the relative AnyType
 * object (which is very useful for example in the Printf function).
 */
/*lint -save -e925 -e926 -e929 -e9005 -e1773 .
 * (925) pointer cast required by this implementation of AnyType */
/* (9005,1773) Cast away of const required by this implementation of AnyType and justified because in the TypeDescriptor
 * attribute the flag "isConstant" will be set to true.
 */
class AnyType {

public:

    /**
     * @brief Default constructor.
     * @details It is used as the terminated element in a Printf list.
     * The data descriptor is void, the pointer null.
     */
    inline AnyType(void);

    /**
     * @brief Copy constructor.
     * @param[in] x is the AnyType to be copied.
     */
    inline AnyType(const AnyType &x);

    /**
     * @brief Most general constructor for a constant type.
     * @param[in] dataDescriptorIn contains the type informations.
     * @param[in] bitAddressIn specifies the bit-shift respect to dataPointer.
     * @param[in] dataPointerIn is the pointer to a generic constant type which must be stored.
     */
    inline AnyType(const TypeDescriptor &dataDescriptorIn,
                   const uint8 bitAddressIn,
                   const void* const dataPointerIn);

    /**
     * @brief Most general constructor for a non-constant type.
     * @param[in] dataDescriptorIn contains the type informations.
     * @param[in] bitAddressIn specifies the bit-shift respect to dataPointer.
     * @param[in] dataPointerIn is the pointer to a generic type which must be stored.
     */
    inline AnyType(const TypeDescriptor &dataDescriptorIn,
                   const uint8 bitAddressIn,
                   void* const dataPointerIn);

    /**
     * @brief Checks if the AnyType is empty.
     * @return true if the data descriptor is VoidType.
     */
    inline bool IsVoid() const;

    /**
     * @brief Constructor from signed 8 bit integer.
     * @param[in] i is the signed 8 bit integer input.
     */
    inline AnyType(int8 &i);

    /**
     * @brief Constructor from unsigned 8 bit integer.
     * @param[in] i is the unsigned 8 bit integer input.
     */
    inline AnyType(uint8 &i);

    /**
     * @brief Constructor from constant signed 8 bit integer.
     * @param[in] i is the constant signed 8 bit integer input.
     */
    inline AnyType(const int8 &i);

    /**
     * @brief Constructor from constant unsigned 8 bit integer.
     * @param[in] i is the constant unsigned 8 bit integer input.
     */
    inline AnyType(const uint8 &i);

    /**
     * @brief Constructor from signed 16 bit integer.
     * @param[in] i is the signed 16 bit integer input.
     */
    inline AnyType(int16 &i);

    /**
     * @brief Constructor from unsigned 16 bit integer.
     * @param[in] i is the unsigned 16 bit integer input.
     */
    inline AnyType(uint16 &i);

    /**
     * @brief Constructor from constant signed 16 bit integer.
     * @param[in] i is the constant signed 16 bit integer input.
     */

    inline AnyType(const int16 &i);

    /**
     * @brief Constructor from constant unsigned 16 bit integer.
     * @param[in] i is the constant 16 bit unsigned integer input.
     */
    inline AnyType(const uint16 &i);

    /**
     * @brief Constructor from signed 32 bit integer.
     * @param[in] i is the signed 32 bit integer input.
     */
    inline AnyType(int32 &i);

    /**
     * @brief Constructor from unsigned 32 bit integer.
     * @param[in] i is the unsigned 32 bit integer input.
     */
    inline AnyType(uint32 &i);

    /**
     * @brief Constructor from constant signed 32 bit integer.
     * @param[in] i is the constant signed 32 bit integer input.
     */
    inline AnyType(const int32 &i);

    /**
     * @brief Constructor from constant unsigned 32 bit integer.
     * @param[in] i is the constant unsigned 32 bit integer input.
     */
    inline AnyType(const uint32 &i);

    /**
     * @brief Constructor from signed 64 bit integer.
     * @param[in] i is the signed 64 bit integer input.
     */
    inline AnyType(int64 &i);

    /**
     * @brief Constructor from unsigned 64 bit integer.
     * @param[in] i is the unsigned 64 bit integer input.
     */
    inline AnyType(uint64 &i);

    /**
     * @brief Constructor from constant signed 64 bit integer.
     * @param[in] i is the constant signed 64 bit integer input.
     */
    inline AnyType(const int64 &i);

    /**
     * @brief Constructor from constant unsigned 64 bit integer.
     * @param[in] i is the constant unsigned 64 bit integer input.
     */
    inline AnyType(const uint64 &i);

    /**
     * @brief Constructor from 32 bit float number.
     * @param[in] i is the 32 bit float number input.
     */
    inline AnyType(float32 &i);

    /**
     * @brief Constructor from constant 32 bit float number.
     * @param[in] i is the the constant 32 bit float number input.
     */
    inline AnyType(const float32 &i);

    /**
     * @brief Constructor from 64 bit float number.
     * @param[in] i the 64 bit float number input.
     */
    inline AnyType(float64 &i);

    /**
     * @brief Constructor from constant 64 bit float number.
     * @param[in] i is the constant 64 bit float number input.
     */
    inline AnyType(const float64 &i);

    /**
     * @brief Constructor from void pointer.
     * @param[in] p is the void pointer input.
     */
    inline AnyType(void * const p);

    /**
     * @brief Constructor from constant void pointer.
     * @param[in] p is the constant void pointer input.
     */
    inline AnyType(const void * const p);

    /**
     * @brief Constructor from C string.
     * @param[in] p is the C string input.
     */
    inline AnyType(const char8 * const p);

    /**
     * @brief Constructor from Object (or inherited class).
     * @param[in] obj the source Object.
     */
    inline AnyType(Object &obj);

    /**
     * @brief Constructor from const Object (or inherited class).
     * @param[in] obj the source Object.
     */
    inline AnyType(const Object &obj);

    /**
     * @brief Constructor by BitBoolean.
     * @param[in] bitBool is the BitBoolean object input.
     */
    template<typename baseType, uint8 bitOffset>
    AnyType(BitBoolean<baseType, bitOffset> &bitBool);

    /**
     * @brief Constructor by BitRange.
     * @param[in] bitRange is the BitRange object input.
     */
    template<typename baseType, uint8 bitSize, uint8 bitOffset>
    AnyType(BitRange<baseType, bitSize, bitOffset> &bitRange);

    /**
     * @brief Constructor by FractionalInteger.
     * @param[in] fractionalInt is the FractionalInteger object input.
     */
    template<typename baseType, uint8 bitSize>
    AnyType(FractionalInteger<baseType, bitSize> &fractionalInt);

    /**
     * @brief Constructor by constant FractionalInteger.
     * @param[in] fractionalInt is the constant FractionalInteger object input.
     */
    template<typename baseType, uint8 bitSize>
    AnyType(const FractionalInteger<baseType, bitSize> &fractionalInt);

    /**
     * @brief Returns the data pointer.
     * @return the data pointer.
     */
    inline void* GetDataPointer();

    /**
     * @brief Returns the data type descriptor.
     * @return the data type descriptor.
     */
    inline TypeDescriptor GetTypeDescriptor() const;

    /**
     * @brief Returns the data bit address.
     * @return the data bit address (i.e  the bit shift respect to the data pointer).
     */
    inline uint8 GetBitAddress() const;

    /**
     * @brief Generate an AnyType from a type registered in the ClassRegisteredDatabase.
     * @details The source \a obj does not have to inherit from Object (but must be registered in the ClassRegisteredDatabase).
     * @param[out] dest the generated AnyType is written in this variable.
     * @param[in] obj the source Object.
     */
    template<typename baseType>
    static void CreateFromOtherType(AnyType &dest,
                                    baseType &obj);

    /**
     * @brief Generate an AnyType from a type registered in the ClassRegisteredDatabase.
     * @details The source \a obj does not have to inherit from Object (but must be registered in the ClassRegisteredDatabase).
     * @param[out] dest the generated AnyType is written in this variable.
     * @param[in] obj the source Object.
     */
    template<typename baseType>
    static void CreateFromOtherType(AnyType &dest,
                                    const baseType &obj);

private:

    /**
     * A pointer to the element.
     */
    void * dataPointer;

    /**
     * The descriptor of the element. It gives
     * all the necessary information about the specific element
     * type. See TypeDescriptor.
     */
    TypeDescriptor dataDescriptor;

    /**
     * Used for BitSet types.
     * The maximum range is 255.
     */
    uint8 bitAddress;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

AnyType::AnyType(void) {
    dataPointer = static_cast<void *>(NULL);
    bitAddress = 0u;
    dataDescriptor = VoidType;
}

AnyType::AnyType(const AnyType &x) {
    /*lint -e{1554} the dataPointer is to be shared with the copied AnyType.*/
    this->dataPointer = x.dataPointer;
    this->bitAddress = x.bitAddress;
    this->dataDescriptor = x.dataDescriptor;
}

AnyType::AnyType(const TypeDescriptor &dataDescriptorIn,
                 const uint8 bitAddressIn,
                 const void* const dataPointerIn) {
    this->dataDescriptor = dataDescriptorIn;
    this->dataDescriptor.isConstant = true;
    this->dataPointer = const_cast<void*>(dataPointerIn);
    this->bitAddress = bitAddressIn;
}

AnyType::AnyType(const TypeDescriptor &dataDescriptorIn,
                 const uint8 bitAddressIn,
                 void* const dataPointerIn) {
    this->dataDescriptor = dataDescriptorIn;
    this->dataPointer = dataPointerIn;
    this->bitAddress = bitAddressIn;
}

bool AnyType::IsVoid() const {
    return (dataDescriptor == VoidType);
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int8 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger8Bit;
}

AnyType::AnyType(uint8 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger8Bit;
}

AnyType::AnyType(const int8 &i) {
    dataPointer = static_cast<void *>(const_cast<int8 *>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger8Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const uint8 &i) {
    dataPointer = static_cast<void *>(const_cast<uint8 *>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger8Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int16 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger16Bit;
}

AnyType::AnyType(uint16 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger16Bit;
}

AnyType::AnyType(const int16 &i) {
    dataPointer = static_cast<void *>(const_cast<int16 *>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger16Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const uint16 &i) {
    dataPointer = static_cast<void *>(const_cast<uint16 *>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger16Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger32Bit;
}

AnyType::AnyType(uint32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger32Bit;
}

AnyType::AnyType(const int32 &i) {
    dataPointer = static_cast<void *>(const_cast<int32 *>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger32Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const uint32 &i) {
    dataPointer = static_cast<void *>(const_cast<uint32 *>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger32Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger64Bit;
}

AnyType::AnyType(uint64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger64Bit;
}

AnyType::AnyType(const int64 &i) {
    dataPointer = static_cast<void *>(const_cast<int64 *>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger64Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const uint64 &i) {
    dataPointer = static_cast<void *>(const_cast<uint64 *>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger64Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(float32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = Float32Bit;
}

AnyType::AnyType(const float32 &i) {
    dataPointer = static_cast<void *>(const_cast<float32 *>(&i));
    bitAddress = 0u;
    dataDescriptor = Float32Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(float64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = Float64Bit;
}

AnyType::AnyType(const float64 &i) {
    dataPointer = static_cast<void *>(const_cast<float64 *>(&i));
    bitAddress = 0u;
    dataDescriptor = Float64Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(void * const p) {
    dataPointer = p;
    bitAddress = 0u;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = Pointer;
    dataDescriptor.numberOfBits = sizeof(void*) * 8u;
}

AnyType::AnyType(const void * const p) {
    dataPointer = const_cast<void *>(p);
    bitAddress = 0u;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = true;
    dataDescriptor.type = Pointer;
    dataDescriptor.numberOfBits = sizeof(void*) * 8u;
}

AnyType::AnyType(const char8 * const p) {
    dataPointer = static_cast<void *>(const_cast<char8 *>(p)); // we will either print the variable or the string
    bitAddress = 0u;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = true;
    dataDescriptor.type = CCString;
    dataDescriptor.numberOfBits = sizeof(const char8*) * 8u;
}

/*---------------------------------------------------------------------------*/
AnyType::AnyType(Object &obj) {
    dataPointer = static_cast<void *>(&obj);
    bitAddress = 0u;

    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const char8 *className = obj.GetClassProperties()->GetName();
    const ClassRegistryItem *classItem = classDatabase->Find(className);
    if (classItem != NULL_PTR(ClassRegistryItem *)) {
        dataDescriptor.isStructuredData = true;
        dataDescriptor.isConstant = false;
        dataDescriptor.structuredDataIdCode = static_cast<uint14>(classItem->GetClassProperties()->GetUniqueIdentifier());
    }
}

AnyType::AnyType(const Object &obj) {
    dataPointer = static_cast<void *>(const_cast<Object *>(&obj));
    bitAddress = 0u;

    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const char8 *className = obj.GetClassProperties()->GetName();
    const ClassRegistryItem *classItem = classDatabase->Find(className);
    if (classItem != NULL_PTR(ClassRegistryItem *)) {
        dataDescriptor.isStructuredData = true;
        dataDescriptor.isConstant = true;
        dataDescriptor.structuredDataIdCode = static_cast<uint14>(classItem->GetClassProperties()->GetUniqueIdentifier());
    }
}

template<typename baseType>
void AnyType::CreateFromOtherType(AnyType &dest,
                                  baseType &obj) {
    dest.dataPointer = static_cast<void *>(&obj);
    dest.bitAddress = 0u;

    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->FindTypeIdName(typeid(obj).name());
    if (classItem != NULL_PTR(ClassRegistryItem *)) {
        dest.dataDescriptor.isStructuredData = true;
        dest.dataDescriptor.isConstant = false;
        dest.dataDescriptor.structuredDataIdCode = static_cast<uint14>(classItem->GetClassProperties()->GetUniqueIdentifier());
    }
}

template<typename baseType>
void AnyType::CreateFromOtherType(AnyType &dest,
                                  const baseType &obj) {
    dest.dataPointer = static_cast<void *>(const_cast<baseType *>(&obj));
    dest.bitAddress = 0u;

    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->FindTypeIdName(typeid(obj).name());
    if (classItem != NULL_PTR(ClassRegistryItem *)) {
        dest.dataDescriptor.isStructuredData = true;
        dest.dataDescriptor.isConstant = true;
        dest.dataDescriptor.structuredDataIdCode = static_cast<uint14>(classItem->GetClassProperties()->GetUniqueIdentifier());
    }
}

template<typename baseType, uint8 bitOffset>
AnyType::AnyType(BitBoolean<baseType, bitOffset> &bitBool) {
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = UnsignedInteger;
    dataDescriptor.numberOfBits = 1u;
    bitAddress = bitBool.BitOffset();
    dataPointer = static_cast<void *>(&bitBool);
}

template<typename baseType, uint8 bitSize, uint8 bitOffset>
AnyType::AnyType(BitRange<baseType, bitSize, bitOffset> &bitRange) {
    TypeDefinition::BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = bitRange.GetNumberOfBits();
    bitAddress = bitRange.BitOffset();
    dataPointer = static_cast<void *>(&bitRange);
}

template<typename baseType, uint8 bitSize>
AnyType::AnyType(FractionalInteger<baseType, bitSize> &fractionalInt) {
    TypeDefinition::BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = fractionalInt.GetNumberOfBits();
    bitAddress = 0;
    dataPointer = static_cast<void *>(&fractionalInt);
}

template<typename baseType, uint8 bitSize>
AnyType::AnyType(const FractionalInteger<baseType, bitSize> &fractionalInt) {
    TypeDefinition::BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = true;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = fractionalInt.GetNumberOfBits();
    bitAddress = 0;
    dataPointer = static_cast<void *>(const_cast<FractionalInteger<baseType, bitSize> *>(&fractionalInt));
}

/*---------------------------------------------------------------------------*/

void* AnyType::GetDataPointer() {
    return dataPointer;
}

TypeDescriptor AnyType::GetTypeDescriptor() const {
    return dataDescriptor;
}

uint8 AnyType::GetBitAddress() const {
    return bitAddress;
}

/**
 * Definition of the void AnyType (empty constructor).
 */
static const AnyType voidAnyType;

}

#endif /* ANYTYPE_H_ */

