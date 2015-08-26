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
#include "TypeDescriptor.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace TypeDefinition {

/**
 * @brief AnyType class.
 *
 * @details This class provide a smart mechanism for the manipulation of each type.\n
 * It is composed by a void* pointer to the element and by a descriptor
 * which gives information about the type (the size, if it is a signed or
 * unsigned number, if it is constant, if it is an object or a native element, ecc).\n
 * For each type a constructor can automatically built the relative AnyType
 * object and this is very useful for example in the Printf function.
 */
/*lint -save -e925 -e929 -e9005 -e1773 .
 * (925) pointer cast required by this implementation of AnyType */
/* (9005,1773) Cast away of const required by this implementation of AnyType and justified because the information is stored.
 */
class AnyType {

public:
    /**
     * @brief Default constructor.
     *
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
     * @brief Most General constructor.
     *
     * @details It is used as the terminated element in a Printf list.
     * The data descriptor is void, the pointer null.
     */
    inline AnyType(const TypeDescriptor &dataDescriptor, uint8 bitAddress, void* const dataPointer);

    /**
     * @brief Checks if the AnyType is empty.
     * @return true if the data descriptor is VoidType.
     */
    inline bool IsVoid() const;

    /**
     * @brief Constructor by float32.
     * @param[in] i is a pointer to the float32 number.
     */
    inline AnyType(float32 &i);

    /**
     * @brief Constructor by const float32.
     * @param[in] i is a pointer to the const float32 number.
     *
     * The flag isConstant is settes to true. */
    inline AnyType(const float32 &i);

    /**
     * @brief Constructor from a float64.
     * @param[in] i is a pointer to the data number.
     */
    inline AnyType(float64 &i);

    /**
     * @brief Constructor from a const float64.
     * @param[in] i is a pointer to the const float64 number.
     */
    inline AnyType(const float64 &i);
    /**
     * @brief Constructor from an 8 bit integer constant.
     * @param[in] i is the const 8 bit integer (const char8).
     */
    inline AnyType(const int8 &i);

    /**
     * @brief Constructor from an integer 8 bit.
     * @param[in] i is the 8 bit integer (char8).
     */
    inline AnyType(int8 &i);

    /**
     * @brief Constructor from an unsigned integer 8 bit.
     * @param[in] i is the unsigned 8 bit integer.
     */
    inline AnyType(uint8 &i);

    /**
     * @brief Constructor from an integer 16 bit.
     * @param[in] i is the 16 bit integer.
     */
    inline AnyType(int16 &i);

    /**
     * @brief Constructor from an unsigned integer 16 bit.
     * @param[in] i is the 16 bit unsigned integer.
     */
    inline AnyType(uint16 &i);

    /**
     * @brief Constructor from an unsigned integer 32 bit.
     * @param[in] i is the unsigned 32 bit integer.
     */
    inline AnyType(uint32 &i);

    /**
     * @brief Constructor from an unsigned constant integer 32 bit.
     * @param[in] i is the constant unsigned 32 bit integer.
     */
    inline AnyType(const uint32 &i);

    /**
     * @brief Constructor from an integer 32 bit.
     * @param[in] i is the 32 bit integer.
     */
    inline AnyType(int32 &i);

    /**
     * @brief Constructor from a constant integer 32 bit.
     * @param[in] i is the constant 32 bit integer.
     */
    inline AnyType(const int32 &i);

    /**
     * @brief Constructor from an unsigned integer 64 bit.
     * @param[in] i is the unsigned 64 bit integer.
     */
    inline AnyType(uint64 &i);

    /**
     * @brief Constructor from an integer 64 bit.
     * @param[in] i is the 64 bit integer.
     */
    inline AnyType(int64 &i);

    /**
     * @brief Constructor from a constant 64 bit integer.
     * @param[in] i is the 64 bit constant integer.
     */
    inline AnyType(const int64 &i);

    /**
     * @brief Constructor from a const void* pointer.
     * @param[in] i is the const void* pointer.
     */
    inline AnyType(const void * const p);

    /**
     * @brief Constructor from a void* pointer.
     * @param[in] i is the const void* pointer.
     */
    inline AnyType(void * const p);

    /**
     * @brief Constructor from a const char8* string.
     * @param[in] i is the const char8 pointer.
     */
    inline AnyType(const char8 * const p);

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
     * Used for BitSet typer.
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
    dataPointer = x.dataPointer;
    bitAddress = x.bitAddress;
    dataDescriptor = x.dataDescriptor;
}

AnyType::AnyType(const TypeDescriptor &dataDescriptor, uint8 bitAddress, void* const dataPointer){
    this->dataDescriptor = dataDescriptor;
    this->dataPointer =  dataPointer;
    this->bitAddress =  bitAddress;
}

bool AnyType::IsVoid() const {
    return (dataDescriptor == VoidType);
}

AnyType::AnyType(float32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = Float32Bit;
}

AnyType::AnyType(const float32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = Float32Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(float64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = Float64Bit;
}

AnyType::AnyType(const float64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = Float64Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const int8 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger8Bit;
    dataDescriptor.isConstant = true;
}

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

AnyType::AnyType(uint32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger32Bit;
}

AnyType::AnyType(const uint32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger32Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(int32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger32Bit;
}

AnyType::AnyType(const int32 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger32Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(uint64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger64Bit;
}

AnyType::AnyType(int64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger64Bit;
}

AnyType::AnyType(const int64 &i) {
    dataPointer = static_cast<void *>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger64Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const void * const p) {
    dataPointer = static_cast<void *>(p);
    bitAddress = 0u;
    dataDescriptor = VoidPointer;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(void * const p) {
    dataPointer = static_cast<void *>(p);
    bitAddress = 0u;
    dataDescriptor = VoidPointer;
}

AnyType::AnyType(const char8 * const p) {
    dataPointer = static_cast<void *>(p); // we will either print the variable or the string
    bitAddress = 0u;
    dataDescriptor = ConstCString;
}

/**
 * Definition of the void AnyType (empty contructor).
 */
static const AnyType voidAnyType;



}

#endif /* ANYTYPE_H_ */

