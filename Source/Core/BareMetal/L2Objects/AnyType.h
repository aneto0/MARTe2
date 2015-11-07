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
#include <typeinfo>
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief AnyType class.
 *
 * @details This class provides a smart mechanism for the generic representation of types.\n
 * An AnyType is made by:
 *   - a void* pointer to the data;
 *   - an uint8 representing the bit-shift from the void* pointer to the actual data (used in bitfields);
 *   - a TypeDescriptor which defines the type, constantness, signedness, etc. of the data.
 *
 * AnyType works with basic types as well as classes, as long as they are registered in the ClassRegistryDatabase.\n
 * @note A constructor for each basic type has been defined and implemented in order to
 * automatically build the relative AnyType object.
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
     * @details TypeDescriptor is initialized to VoidType, dataPointer to null and the bit-shift to 0.
     * @note The empty AnyType is used as the terminator element in a Printf list.
     */
    inline AnyType(void);

    /**
     * @brief Copy constructor.
     * @param[in] x is the AnyType to be copied.
     */
    inline AnyType(const AnyType &x);

    /**
     * @brief Generic constructor for a constant type.
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] bitAddressIn specifies the bit-shift of the data with respect to dataPointer.
     * @param[in] dataPointerIn is the pointer to the constant data.
     */
    inline AnyType(const TypeDescriptor &dataDescriptorIn,
                   const uint8 bitAddressIn,
                   const void* const dataPointerIn);

    /**
     * @brief Generic constructor for a non-constant type.
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] bitAddressIn specifies the bit-shift of the data with respect to dataPointer.
     * @param[in] dataPointerIn is the pointer to the data.
     */
    inline AnyType(const TypeDescriptor &dataDescriptorIn,
                   const uint8 bitAddressIn,
                   void* const dataPointerIn);

    /**
     * @brief Checks if the AnyType is empty.
     * @return true if the TypeDescriptor is VoidType.
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
     * @brief Constructor from 32 bit float32 number.
     * @param[in] i is the 32 bit float32 number input.
     */
    inline AnyType(float32 &i);

    /**
     * @brief Constructor from constant 32 bit float32 number.
     * @param[in] i is the the constant 32 bit float32 number input.
     */
    inline AnyType(const float32 &i);

    /**
     * @brief Constructor from 64 bit float32 number.
     * @param[in] i the 64 bit float32 number input.
     */
    inline AnyType(float64 &i);

    /**
     * @brief Constructor from constant 64 bit float32 number.
     * @param[in] i is the constant 64 bit float32 number input.
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
     * TODO
     */
    template<typename T, uint32 nOfElements>
    inline AnyType(T (&source)[nOfElements]);

    /**
     * TODO
     */
    template<typename T, uint32 nOfRows, uint32 nOfColumns>
    inline AnyType(T (&source)[nOfRows][nOfColumns]);

    /**
     * TODO
     */
    template<typename T>
    inline AnyType(T *source,
                   uint32 nOfElements);

    /**
     * TODO
     */
    template<typename T>
    inline AnyType(const T *source,
                   uint32 nOfElements);

    /**
     * TODO
     */
    template<typename T>
    inline AnyType(T **source,
                   uint32 nOfRows,
                   uint32 nOfColumns);

    /**
     * TODO
     */
    template<typename T>
    inline AnyType(const T **source,
                   uint32 nOfRows,
                   uint32 nOfColumns);

    /**
     * TODO
     */
    template<typename T>
    inline AnyType(T ***source,
                   uint32 nOfRows,
                   uint32 nOfColumns,
                   uint32 nOfZ);

    /**
     * TODO
     */
    template<typename T>
    inline AnyType(const T ***source,
                   uint32 nOfRows,
                   uint32 nOfColumns,
                   uint32 nOfZ);
    /**
     * TODO
     */
    inline AnyType & operator=(const AnyType &src);

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
     * @brief Sets the data pointer hold by this AnyType instance.
     * @param[in] p a pointer to the data.
     */
    inline void SetDataPointer(void* const p);

    /**
     * @brief Returns the pointer to the data.
     * @return the pointer to the data.
     */
    inline void* GetDataPointer() const;

    /**
     * @brief Returns the pointed data TypeDescriptor.
     * @return the data TypeDescriptor.
     */
    inline TypeDescriptor GetTypeDescriptor() const;

    /**
     * @brief Returns the data bit address (i.e  the bit shift respect to the data pointer).
     * @return the data bit address.
     */
    inline uint8 GetBitAddress() const;

    /**
     * @brief Generate an AnyType from a type registered in the ClassRegistryDatabase.
     * @details The source \a obj does not have to inherit from Object (but must be registered in the ClassRegisteredDatabase).
     * @param[out] dest the generated AnyType is written in this variable.
     * @param[in] obj the source Object.
     */
    template<typename baseType>
    static void CreateFromOtherType(AnyType &dest,
                                    baseType &obj);

    /**
     * @brief Generate an AnyType from a type registered in the ClassRegistryDatabase.
     * @details The source \a obj does not have to inherit from Object (but must be registered in the ClassRegistryDatabase).
     * @param[out] dest the generated AnyType is written in this variable.
     * @param[in] obj the source Object.
     */
    template<typename baseType>
    static void CreateFromOtherType(AnyType &dest,
                                    const baseType &obj);

    /**
     * TODO
     */
    inline uint32 GetNumberOfDimensions() const;

    /**
     * TODO
     */
    inline uint32 GetNumberOfElements(uint32 dimension) const;

    /**
     * TODO
     */
    inline void SetNumberOfElements(uint32 dimension,
                                    uint32 nOfElements);

    /**
     * TODO
     */
    inline bool IsStaticDeclared() const;

protected:

    /**
     * Pointer to the data.
     */
    void * dataPointer;

    /**
     * The descriptor of the element. It gives
     * all the necessary information about the specific element
     * type. See TypeDescriptor.
     */
    TypeDescriptor dataDescriptor;

    /**
     * The bit-shift of the actual data from the dataPointer.
     * It is used for BitSet types,and the maximum range is 255.
     */
    uint8 bitAddress;

    /**
     * TODO
     */
    uint8 numberOfDimensions;

    /**
     * TODO
     */
    uint32 numberOfElements[3];

    /**
     * TODO
     */
    bool staticDeclared;

private:
    /**
     * TODO
     */
    inline void InitDimensions();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

AnyType::AnyType(void) {
    dataPointer = static_cast<void *>(NULL);
    bitAddress = 0u;
    dataDescriptor = VoidType;
    InitDimensions();
}

AnyType::AnyType(const AnyType &x) {
    /*lint -e{1554} the dataPointer is to be shared with the copied AnyType.*/
    this->dataPointer = x.dataPointer;
    this->bitAddress = x.bitAddress;
    this->dataDescriptor = x.dataDescriptor;
    this->numberOfDimensions = x.numberOfDimensions;
    this->numberOfElements[0] = x.numberOfElements[0];
    this->numberOfElements[1] = x.numberOfElements[1];
    this->numberOfElements[2] = x.numberOfElements[2];
}

AnyType::AnyType(const TypeDescriptor &dataDescriptorIn,
                 const uint8 bitAddressIn,
                 const void* const dataPointerIn) {
    this->dataDescriptor = dataDescriptorIn;
    this->dataDescriptor.isConstant = true;
    this->dataPointer = const_cast<void*>(dataPointerIn);
    this->bitAddress = bitAddressIn;
    InitDimensions();
}

AnyType::AnyType(const TypeDescriptor &dataDescriptorIn,
                 const uint8 bitAddressIn,
                 void* const dataPointerIn) {
    this->dataDescriptor = dataDescriptorIn;
    this->dataPointer = dataPointerIn;
    this->bitAddress = bitAddressIn;
    InitDimensions();
}

bool AnyType::IsVoid() const {
    return (dataDescriptor == VoidType);
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int8 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger8Bit;
    InitDimensions();
}

AnyType::AnyType(uint8 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger8Bit;
    InitDimensions();
}

AnyType::AnyType(const int8 &i) {
    dataPointer = static_cast<void *>(const_cast<int8 *>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger8Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

AnyType::AnyType(const uint8 &i) {
    dataPointer = static_cast<void *>(const_cast<uint8 *>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger8Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int16 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger16Bit;
    InitDimensions();
}

AnyType::AnyType(uint16 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger16Bit;
    InitDimensions();
}

AnyType::AnyType(const int16 &i) {
    dataPointer = static_cast<void *>(const_cast<int16 *>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger16Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

AnyType::AnyType(const uint16 &i) {
    dataPointer = static_cast<void *>(const_cast<uint16 *>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger16Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger32Bit;
    InitDimensions();
}

AnyType::AnyType(uint32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger32Bit;
    InitDimensions();
}

AnyType::AnyType(const int32 &i) {
    dataPointer = static_cast<void *>(const_cast<int32 *>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger32Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

AnyType::AnyType(const uint32 &i) {
    dataPointer = static_cast<void *>(const_cast<uint32 *>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger32Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger64Bit;
    InitDimensions();
}

AnyType::AnyType(uint64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger64Bit;
    InitDimensions();
}

AnyType::AnyType(const int64 &i) {
    dataPointer = static_cast<void *>(const_cast<int64 *>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger64Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

AnyType::AnyType(const uint64 &i) {
    dataPointer = static_cast<void *>(const_cast<uint64 *>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger64Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(float32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = Float32Bit;
    InitDimensions();
}

AnyType::AnyType(const float32 &i) {
    dataPointer = static_cast<void *>(const_cast<float32 *>(&i));
    bitAddress = 0u;
    dataDescriptor = Float32Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(float64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = Float64Bit;
    InitDimensions();
}

AnyType::AnyType(const float64 &i) {
    dataPointer = static_cast<void *>(const_cast<float64 *>(&i));
    bitAddress = 0u;
    dataDescriptor = Float64Bit;
    dataDescriptor.isConstant = true;
    InitDimensions();
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(void * const p) {
    dataPointer = p;
    bitAddress = 0u;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = Pointer;
    dataDescriptor.numberOfBits = sizeof(void*) * 8u;
    InitDimensions();
}

AnyType::AnyType(const void * const p) {
    dataPointer = const_cast<void *>(p);
    bitAddress = 0u;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = true;
    dataDescriptor.type = Pointer;
    dataDescriptor.numberOfBits = sizeof(void*) * 8u;
    InitDimensions();
}

AnyType::AnyType(const char8 * const p) {
    dataPointer = static_cast<void *>(const_cast<char8 *>(p)); // we will either print the variable or the string
    bitAddress = 0u;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = true;
    dataDescriptor.type = CCString;
    dataDescriptor.numberOfBits = sizeof(const char8*) * 8u;
    InitDimensions();
}

AnyType &AnyType::operator=(const AnyType &src) {
    if (this != &src) {
        dataPointer = src.dataPointer;
        bitAddress = src.bitAddress;
        dataDescriptor = src.dataDescriptor;
        numberOfDimensions = src.numberOfDimensions;
        numberOfElements[0] = src.numberOfElements[0];
        numberOfElements[1] = src.numberOfElements[1];
        numberOfElements[2] = src.numberOfElements[2];
    }
    return *this;
}

/*---------------------------------------------------------------------------*/
/*AnyType::AnyType(Object &obj) {
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
 }*/

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
    InitDimensions();
}

template<typename baseType, uint8 bitSize, uint8 bitOffset>
AnyType::AnyType(BitRange<baseType, bitSize, bitOffset> &bitRange) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = bitRange.GetNumberOfBits();
    bitAddress = bitRange.BitOffset();
    dataPointer = static_cast<void *>(&bitRange);
    InitDimensions();
}

template<typename baseType, uint8 bitSize>
AnyType::AnyType(FractionalInteger<baseType, bitSize> &fractionalInt) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = fractionalInt.GetNumberOfBits();
    bitAddress = 0;
    dataPointer = static_cast<void *>(&fractionalInt);
    InitDimensions();
}

template<typename baseType, uint8 bitSize>
AnyType::AnyType(const FractionalInteger<baseType, bitSize> &fractionalInt) {
    BasicType type = (TypeCharacteristics::IsSigned<baseType>()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = true;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = fractionalInt.GetNumberOfBits();
    bitAddress = 0;
    dataPointer = static_cast<void *>(const_cast<FractionalInteger<baseType, bitSize> *>(&fractionalInt));
    InitDimensions();
}

template<typename T, uint32 nOfElements>
AnyType::AnyType(T (&source)[nOfElements]) {
    dataPointer = static_cast<void *>(&source);
    bitAddress = 0u;
    dataDescriptor.isConstant = false;
    InitDimensions();
    numberOfDimensions = 1u;
    if (nOfElements > 0u) {
        AnyType descriptor(source[0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = nOfElements;
    }
}

template<typename T, uint32 nOfRows, uint32 nOfColumns>
AnyType::AnyType(T (&source)[nOfRows][nOfColumns]) {
    dataPointer = static_cast<void *>(&source);
    bitAddress = 0u;
    dataDescriptor.isConstant = false;
    InitDimensions();
    numberOfDimensions = 2u;
    if ((nOfRows > 0u) && (nOfColumns > 0u)) {
        AnyType descriptor(source[0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = nOfColumns;
        numberOfElements[1] = nOfRows;
    }
}

template<typename T>
AnyType::AnyType(T *source,
                 uint32 nOfElements) {
    dataPointer = static_cast<void *>(source);
    bitAddress = 0u;
    dataDescriptor.isConstant = false;
    InitDimensions();
    numberOfDimensions = 1u;
    if (nOfElements > 0u) {
        AnyType descriptor(source[0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = nOfElements;
    }
}

template<typename T>
AnyType::AnyType(const T *source,
                 uint32 nOfElements) {
    dataPointer = static_cast<void *>(source);
    bitAddress = 0u;
    dataDescriptor.isConstant = true;
    InitDimensions();
    numberOfDimensions = 1u;
    if (nOfElements > 0u) {
        AnyType descriptor(source[0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = nOfElements;
    }
}

template<typename T>
AnyType::AnyType(const T **source,
                 uint32 nOfRows,
                 uint32 nOfColumns) {
    dataPointer = static_cast<void *>(source);
    bitAddress = 0u;
    dataDescriptor.isConstant = true;
    InitDimensions();
    numberOfDimensions = 2u;
    if ((nOfRows > 0u) && (nOfColumns > 0u)) {
        AnyType descriptor(source[0][0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = nOfColumns;
        numberOfElements[1] = nOfRows;
    }
}

template<typename T>
AnyType::AnyType(T **source,
                 uint32 nOfRows,
                 uint32 nOfColumns) {
    dataPointer = static_cast<void *>(source);
    bitAddress = 0u;
    dataDescriptor.isConstant = false;
    InitDimensions();
    numberOfDimensions = 2u;
    if ((nOfRows > 0u) && (nOfColumns > 0u)) {
        AnyType descriptor(source[0][0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = nOfColumns;
        numberOfElements[1] = nOfRows;
    }
}

template<typename T>
AnyType::AnyType(const T ***source,
                 uint32 nOfRows,
                 uint32 nOfColumns,
                 uint32 nOfZ) {
    dataPointer = static_cast<void *>(source);
    bitAddress = 0u;
    dataDescriptor.isConstant = true;
    InitDimensions();
    numberOfDimensions = 2u;
    if ((nOfRows > 0u) && (nOfColumns > 0u) && (nOfZ > 0u)) {
        AnyType descriptor(source[0][0][0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = nOfColumns;
        numberOfElements[1] = nOfRows;
        numberOfElements[2] = nOfZ;
    }
}

template<typename T>
AnyType::AnyType(T ***source,
                 uint32 nOfRows,
                 uint32 nOfColumns,
                 uint32 nOfZ) {
    dataPointer = static_cast<void *>(source);
    bitAddress = 0u;
    dataDescriptor.isConstant = false;
    InitDimensions();
    numberOfDimensions = 2u;
    if ((nOfRows > 0u) && (nOfColumns > 0u) && (nOfZ > 0u)) {
        AnyType descriptor(source[0][0][0]);
        dataDescriptor = descriptor.GetTypeDescriptor();
        numberOfElements[0] = nOfColumns;
        numberOfElements[1] = nOfRows;
        numberOfElements[2] = nOfZ;
    }
}
/*---------------------------------------------------------------------------*/

void* AnyType::GetDataPointer() const {
    return dataPointer;
}

void AnyType::SetDataPointer(void * const p) {
    dataPointer = p;
}

TypeDescriptor AnyType::GetTypeDescriptor() const {
    return dataDescriptor;
}

uint8 AnyType::GetBitAddress() const {
    return bitAddress;
}

uint32 AnyType::GetNumberOfDimensions() const {
    return numberOfDimensions;
}

bool AnyType::IsStaticDeclared() const {
    return staticDeclared;
}

uint32 AnyType::GetNumberOfElements(uint32 dimension) const {
    return numberOfElements[dimension];
}

void AnyType::SetNumberOfElements(uint32 dimension,
                                  uint32 nOfElements) {
    numberOfElements[dimension] = nOfElements;
}

inline void AnyType::InitDimensions() {
    numberOfDimensions = 0u;
    numberOfElements[0] = 0u;
    numberOfElements[1] = 0u;
    numberOfElements[2] = 0u;
    staticDeclared = false;
}

/**
 * Definition of the void AnyType (empty constructor).
 */
static const AnyType voidAnyType;

}

#endif /* ANYTYPE_H_ */

