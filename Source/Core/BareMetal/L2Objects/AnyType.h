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

#include <typeinfo>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ClassProperties.h"
#include "ClassRegistryDatabase.h"
#include "Matrix.h"
#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Object;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*lint -sem(MARTe::AnyType::Init,initializer)*/
namespace MARTe {

/**
 * @brief Class which provides a smart mechanism for the generic representation
 * of types.
 * @details Each instance of this class is made by:
 * - a void* pointer to the data;
 * - an uint8 representing the bit-shift from the void* pointer to the actual
 * data (used in bitfields);
 * - a TypeDescriptor which defines the type, constantness, signedness, etc.
 * of the data.
 *
 * AnyType works with basic types as well as classes, as long as they are
 * registered in the ClassRegistryDatabase.
 * @remark A constructor for each basic type has been defined and implemented in
 * order to automatically build the relative AnyType object. Some of these
 * constructors are templates.
 */
/*lint -save -e925 -e926 -e929 -e9005 -e1773 .
 * (925) pointer cast required by this implementation of AnyType */
/* (9005,1773) Cast away of const required by this implementation of AnyType
 * and justified because in the TypeDescriptor attribute the flag "isConstant"
 * will be set to true.
 */
class DLL_API AnyType {

public:

    /**
     * @brief Default constructor.
     * @details TypeDescriptor is initialized to VoidType, dataPointer to null and the bit-shift to 0.
     * @remark The empty AnyType is used as the terminator element in a Printf list.
     */
    inline AnyType(void);

    /**
     * @brief Copy constructor.
     * @param[in] x is the AnyType to be copied.
     * @post
     *   GetDataPointer() == x.GetDataPointer() &&
     *   GetBitAddress() == x.GetBitAddress() &&
     *   GetDataDescriptor() == x.GetDataDescriptor() &&
     *   IsStaticDeclared == x.IsStaticDeclared() &&
     *   GetNumberOfDimensions() == x.GetNumberOfDimensions() &&
     *   GetNumberOfElements(0:2) == x.GetNumberOfElements(0:2)
     */
    inline AnyType(const AnyType &x);

    /**
     * @brief Generic constructor for a constant type.
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] bitAddressIn specifies the bit-shift of the data with respect to dataPointer.
     * @param[in] dataPointerIn is the pointer to the constant data.
     * @post
     *   GetDataPointer() == dataPointerIn &&
     *   GetBitAddress() == bitAddressIn &&
     *   GetDataDescriptor() == dataDescriptorIn &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const TypeDescriptor &dataDescriptorIn,
                   const uint8 bitAddressIn,
                   const void *const dataPointerIn);

    /**
     * @brief Generic constructor for a non-constant type.
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] bitAddressIn specifies the bit-shift of the data with respect to dataPointer.
     * @param[in] dataPointerIn is the pointer to the data.
     * @post
     *   GetDataPointer() == dataPointerIn &&
     *   GetBitAddress() == bitAddressIn &&
     *   GetDataDescriptor() == dataDescriptorIn &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const TypeDescriptor &dataDescriptorIn,
                   const uint8 bitAddressIn,
                   void *const dataPointerIn);

    /**
     * @brief Checks if the AnyType is empty.
     * @return true if the TypeDescriptor is VoidType.
     */
    inline bool IsVoid() const;

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == Character8Bit &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(char8 &i);

    /**
     * @brief Constructor from 8 bit character.
     * @param[in] i is the 8 bit character input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == Character8Bit &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const char8 &i);

    /**
     * @brief Constructor from signed 8 bit integer.
     * @param[in] i is the signed 8 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger8Bit &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(int8 &i);

    /**
     * @brief Constructor from unsigned 8 bit integer.
     * @param[in] i is the unsigned 8 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger8Bit &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(uint8 &i);

    /**
     * @brief Constructor from constant signed 8 bit integer.
     * @param[in] i is the constant signed 8 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger8Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */

    inline AnyType(const int8 &i);

    /**
     * @brief Constructor from constant unsigned 8 bit integer.
     * @param[in] i is the constant unsigned 8 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger8Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */

    inline AnyType(const uint8 &i);

    /**
     * @brief Constructor from signed 16 bit integer.
     * @param[in] i is the signed 16 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger16Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */

    inline AnyType(int16 &i);

    /**
     * @brief Constructor from unsigned 16 bit integer.
     * @param[in] i is the unsigned 16 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger16Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */

    inline AnyType(uint16 &i);

    /**
     * @brief Constructor from constant signed 16 bit integer.
     * @param[in] i is the constant signed 16 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger16Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const int16 &i);

    /**
     * @brief Constructor from constant unsigned 16 bit integer.
     * @param[in] i is the constant 16 bit unsigned integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger16Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const uint16 &i);

    /**
     * @brief Constructor from signed 32 bit integer.
     * @param[in] i is the signed 32 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger32Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(int32 &i);

    /**
     * @brief Constructor from unsigned 32 bit integer.
     * @param[in] i is the unsigned 32 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger32Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(uint32 &i);

    /**
     * @brief Constructor from constant signed 32 bit integer.
     * @param[in] i is the constant signed 32 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger32Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const int32 &i);

    /**
     * @brief Constructor from constant unsigned 32 bit integer.
     * @param[in] i is the constant unsigned 32 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger32Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const uint32 &i);

    /**
     * @brief Constructor from signed 64 bit integer.
     * @param[in] i is the signed 64 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger64Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(int64 &i);

    /**
     * @brief Constructor from unsigned 64 bit integer.
     * @param[in] i is the unsigned 64 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger64Bit &&
     *   GetTypeDescriptor().isConstant == false &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(uint64 &i);

    /**
     * @brief Constructor from constant signed 64 bit integer.
     * @param[in] i is the constant signed 64 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == SignedInteger64Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const int64 &i);

    /**
     * @brief Constructor from constant unsigned 64 bit integer.
     * @param[in] i is the constant unsigned 64 bit integer input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetTypeDescriptor() == UnsignedInteger64Bit &&
     *   GetTypeDescriptor().isConstant == true &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const uint64 &i);

    /**
     * @brief Constructor from 32 bit float32 number.
     * @param[in] i is the 32 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(float32 &i);

    /**
     * @brief Constructor from constant 32 bit float32 number.
     * @param[in] i is the the constant 32 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const float32 &i);

    /**
     * @brief Constructor from 64 bit float32 number.
     * @param[in] i the 64 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(float64 &i);

    /**
     * @brief Constructor from constant 64 bit float32 number.
     * @param[in] i is the constant 64 bit float32 number input.
     * @post
     *   GetDataPointer() == &i &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(const float64 &i);

    /**
     * @brief Constructor from void pointer.
     * @param[in] p is the void pointer input.
     * @post
     *   GetDataPointer() == p &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(volatile void *const p);
    /**
     * @brief Constructor from constant void pointer.
     * @param[in] p is the constant void pointer input.
     * @post
     *   GetDataPointer() == p &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(volatile const void *const p);

    /**
     * @brief Constructor from constant char pointer.
     * @param[in] p is the C string input.
     * @post
     *   GetDataPointer() == p &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(volatile const char8 *const p);

    /**
     * @brief Constructor from char pointer.
     * @param[in] p is the C string input.
     * @post
     *   GetDataPointer() == p &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    inline AnyType(volatile char8 *const p);

    /**
     * @brief Constructor from Object (or inherited class).
     * @param[in] obj the source Object.
     * @post
     *   GetDataPointer() == &obj &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    AnyType(Object &obj);

    /**
     * @brief Constructor from const Object (or inherited class).
     * @param[in] obj the source Object.
     * @post
     *   GetDataPointer() == &obj &&
     *   IsStaticDeclared == true &&
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0:2) == 0
     */
    AnyType(const Object &obj);

    /**
     * @brief Copy constructor from another AnyType.
     * @return a copy of this AnyType.
     * @post
     *   GetDataPointer() == src.GetDataPointer() &&
     *   GetBitAddress() == src.GetBitAddress() &&
     *   GetDataDescriptor() == src.GetDataDescriptor() &&
     *   IsStaticDeclared == src.IsStaticDeclared() &&
     *   GetNumberOfDimensions() == src.GetNumberOfDimensions() &&
     *   GetNumberOfElements(0:2) == src.GetNumberOfElements(0:2)
     */
    inline AnyType& operator=(const AnyType &src);

    /**
     * @brief Constructor by BitBoolean.
     * @tparam baseType the standard type which is used as a base for bitBool's type
     * @tparam bitOffset the actual bit offset of the bitBool's type
     * @param[in] bitBool is the BitBoolean object input.
     */
    template<typename baseType, uint8 bitOffset>
    AnyType(BitBoolean<baseType, bitOffset> &bitBool);

    /**
     * @brief Constructor by BitRange.
     * @tparam baseType the standard type which is used as a base for bitRange's type
     * @tparam bitSize the actual bit size of the bitRange's type
     * @tparam bitOffset the actual bit offset of the bitRange's type
     * @param[in] bitRange is the BitRange object input.
     */
    template<typename baseType, uint8 bitSize, uint8 bitOffset>
    AnyType(BitRange<baseType, bitSize, bitOffset> &bitRange);

    /**
     * @brief Constructor by FractionalInteger.
     * @tparam baseType the standard type which is used as a base for fractionalInt's type
     * @tparam bitSize the actual bit size of the fractionalInt's type
     * @param[in] fractionalInt is the FractionalInteger object input.
     */
    template<typename baseType, uint8 bitSize>
    AnyType(FractionalInteger<baseType, bitSize> &fractionalInt);

    /**
     * @brief Constructor by constant FractionalInteger.
     * @tparam baseType the standard type which is used as a base for fractionalInt's type
     * @tparam bitSize the actual bit size of the fractionalInt's type
     * @param[in] fractionalInt is the constant FractionalInteger object input.
     */
    template<typename baseType, uint8 bitSize>
    AnyType(const FractionalInteger<baseType, bitSize> &fractionalInt);

    /**
     * @brief Constructor from a statically declared array [].
     * @tparam T the type of the elements in the array
     * @tparam nOfElementsStatic number of elements in the array, automatically computed by the compiler.
     * @param[in] source address of the statically declared array.
     * @post
     *   GetNumberOfDimensions() == 1 &&
     *   GetNumberOfElements(0) == nOfElementsStatic &&
     *   GetDataPointer() == &source[0] &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == true
     */
    template<typename T, uint32 nOfElementsStatic>
    AnyType(T (&source)[nOfElementsStatic]);

    /**
     * @brief Constructor from a statically declared array of characters [].
     * @tparam nOfElementsStatic number of elements in the array, automatically computed by the compiler.
     * @param[in] source address of the statically declared array.
     * @pre
     *   The C Array must be always 0 terminated like a normal C string.
     * @post
     *   GetNumberOfDimensions() == 0 &&
     *   GetByteSize() == nOfElementsStatic &&
     *   GetDataPointer() == &source[0] &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == true
     */
    template<uint32 nOfElementsStatic>
    AnyType(char8 (&source)[nOfElementsStatic]);

    /**
     * @brief Constructor from a constant statically declared array of characters [].
     * @tparam nOfElementsStatic number of elements in the array, automatically computed by the compiler.
     * @param[in] source address of the statically declared array.
     * @pre
     *   The C Array must be always 0 terminated like a normal C string.
     * @post
     *   GetNumberOfDimensions() == 0 &&
     *   GetByteSize() == nOfElementsStatic &&
     *   GetDataPointer() == &source[0] &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == true
     */
    template<uint32 nOfElementsStatic>
    AnyType(const char8 (&source)[nOfElementsStatic]);

    /**
     * @brief Constructor from a statically declared table [][].
     * @tparam T the type of the elements in the array
     * @tparam nOfRowsStatic number of rows in the table, automatically computed by the compiler.
     * @tparam nOfColumnsStatic number of columns in the table, automatically computed by the compiler.
     * @param[in] source address of the statically declared table.
     * @post
     *   GetNumberOfDimensions() == 2 &&
     *   GetNumberOfElements(0) == nOfColumnsStatic &&
     *   GetNumberOfElements(1) == nOfRowsStatic &&
     *   GetDataPointer() == &source[0] &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == true
     */
    template<typename T, uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
    AnyType(T (&source)[nOfRowsStatic][nOfColumnsStatic]);

    /**
     * @brief Constructor from a statically declared table of characters [][].
     * @tparam nOfRowsStatic number of rows in the table, automatically computed by the compiler.
     * @tparam nOfColumnsStatic number of columns in the table, automatically computed by the compiler.
     * @param[in] source address of the statically declared table.
     * @post
     *   GetNumberOfDimensions() == 1 &&
     *   GetNumberOfElements(0) == nOfRowStatic &&
     *   GetDataPointer() == &source[0] &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == true
     */
    template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
    AnyType(char8 (&source)[nOfRowsStatic][nOfColumnsStatic]);

    /**
     * @brief Constructor from a statically declared table of characters [][][].
     * @tparam nOfRowsStatic number of rows in the table, automatically computed by the compiler.
     * @tparam nOfColumnsStatic number of columns in the table, automatically computed by the compiler.
     * @tparam nOfChars number of chars in each cell of the table, automatically computed by the compiler.
     * @param[in] source address of the statically declared table.
     * @post
     *   GetNumberOfDimensions() == 1 &&
     *   GetNumberOfElements(0) == nOfRowStatic &&
     *   GetDataPointer() == &source[0] &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == true
     */
    template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic, uint32 nOfChars>
    AnyType(char8 (&source)[nOfRowsStatic][nOfColumnsStatic][nOfChars]);

    /**
     * @brief Constructor from an existent Matrix.
     * @tparam T the type of the elements in the matrix
     * @param[in] mat the matrix from whose this AnyType will be constructed.
     * @post
     *   GetNumberOfDimensions() == 2 &&
     *   GetNumberOfElements(0) == mat.GetNumberOfColumns() &&
     *   GetNumberOfElements(1) == mat.GetNumberOfRows() &&
     *   GetDataPointer() == mat.GetDataPointer() &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == mat.IsStaticDeclared()
     */
    template<typename T>
    AnyType(Matrix<T> &mat);

    /**
     * @brief Constructor from an existent Matrix of characters (considered as an array of string buffers).
     * @param[in] mat the matrix from whose this AnyType will be constructed.
     * @post
     *   GetNumberOfDimensions() == 1 &&
     *   GetNumberOfElements(0) == mat.GetNumberOfRows() &&
     *   GetDataPointer() == mat.GetDataPointer() &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == mat.IsStaticDeclared()
     */
    inline AnyType(Matrix<char8> &mat);

    /**
     * @brief Constructor from an existent Vector.
     * @tparam T the type of the elements in the vector
     * @param[in] vec the vector from whose this AnyType will be constructed.
     * @post
     *   GetNumberOfDimensions() == 1 &&
     *   GetNumberOfElements(0) == vec.GetNumberOfElements() &&
     *   GetDataPointer() == vec.GetDataPointer() &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == vec.IsStaticDeclared()
     */
    template<typename T>
    AnyType(Vector<T> &vec);

    /**
     * @brief Constructor from an existent Vector of characters (considered as a string buffer).
     * @param[in] vec the vector from whose this AnyType will be constructed.
     * @post
     *   GetNumberOfDimensions() == 0 &&
     *   GetByteSize() == nOfElementsStatic &&
     *   GetDataPointer() == vec.GetDataPointer() &&
     *   GetTypeDescriptor() == AnyType(T).GetTypeDescriptor() &&
     *   IsStaticDeclared == vec.IsStaticDeclared()
     */
    inline AnyType(Vector<char8> &vec);

    /**
     * @brief Sets the data pointer hold by this AnyType instance.
     * @param[in] p a pointer to the data.
     * @post
     *   GetDataPointer() == p
     */
    inline void SetDataPointer(void *const p);

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
     * @brief Sets the TypeDescriptor of the data pointed by this AnyType.
     */
    inline void SetTypeDescriptor(const TypeDescriptor dataDescriptorIn);

    /**
     * @brief Returns the data bit address (i.e  the bit shift respect to the data pointer).
     * @return the data bit address.
     */
    inline uint8 GetBitAddress() const;

    /**
     * @brief Generate an AnyType from a type registered in the ClassRegistryDatabase.
     * @details The source \a obj does not have to inherit from Object (but must be registered in the ClassRegisteredDatabase).
     * @tparam baseType the type of the source object
     * @param[out] dest the generated AnyType is written in this variable.
     * @param[in] obj the source Object.
     */
    template<typename baseType>
    static void CreateFromOtherType(AnyType &dest,
                                    baseType &obj);

    /**
     * @brief Generate an AnyType from a type registered in the ClassRegistryDatabase.
     * @details The source \a obj does not have to inherit from Object (but must be registered in the ClassRegistryDatabase).
     * @tparam baseType the type of the source object
     * @param[out] dest the generated AnyType is written in this variable.
     * @param[in] obj the source Object.
     */
    template<typename baseType>
    static void CreateFromOtherType(AnyType &dest,
                                    const baseType &obj);

    /**
     * @brief Gets the number of dimensions associated to this AnyType.
     * @details GetNumberOfDimensions() == 0 => scalar, GetNumberOfDimensions() == 1 => vector
     * GetNumberOfDimensions() == 2 => matrix
     * @return the number of dimensions associated to this AnyType.
     */
    inline uint8 GetNumberOfDimensions() const;

    /**
     * @brief Sets the number of dimensions associated to this AnyType.
     * @details nOfDimensions == 0 => scalar, nOfDimensions == 1 => vector
     * @param[in] nOfDimensions the new number of dimensions represented by this AnyType.
     * nOfDimensions == 2 => matrix
     */
    inline void SetNumberOfDimensions(const uint8 nOfDimensions);

    /**
     * @brief Gets the number of elements in a given \a dimension.
     * @param[in] dimension the dimension to be queried.
     * @return the number of elements in a given \a dimension.
     * @pre
     *   dimension < 3
     */
    inline uint32 GetNumberOfElements(const uint32 dimension) const;

    /**
     * @brief Sets the number of elements in a given \a dimension.
     * @param[in] dimension the dimension to be updated.
     * @param[in] nOfElements the new number of elements dimension to be updated.
     * @return the number of elements in a given \a dimension.
     * @pre
     *   dimension < 3
     */
    inline void SetNumberOfElements(const uint32 dimension,
                                    const uint32 nOfElements);

    /**
     * @brief Checks if GetDataPointer() is pointing at a statically allocated memory block.
     * @return true if GetDataPointer() is pointing at a statically allocated memory block.
     */
    inline bool IsStaticDeclared() const;

    /**
     * @brief If \a isStaticDeclared=true => GetDataPointer() is pointing at a statically allocated memory block.
     * @param[in] isStaticDeclared if true, GetDataPointer() is pointing at a statically allocated memory block.
     */
    inline void SetStaticDeclared(const bool isStaticDeclared);

    /**
     * @brief Retrieves the byte size of this type.
     * @return the byte size of this type.
     */
    inline uint32 GetByteSize() const;

    /**
     * @brief Retrieves the bit size of this type.
     * @return the bit size of this type.
     */
    inline uint32 GetBitSize() const;

    /**
     * @brief Retrieves the total size in byte of the memory associated to this AnyType.
     * @return the byte size of the memory associated to this AnyType.
     */
    inline uint32 GetDataSize() const;

    /**
     * @brief Retrieves the element in the specified position.
     * @param[in] position is the position of the required element.
     * @return voidAnyType if this AnyType is scalar or in case of errors, a scalar AnyType if this
     * AnyType is a vector, a vector AnyType if this AnyType is a scalar.
     */
    AnyType operator[](const uint32 position) const;

private:

    /**
     * Pointer to the data.
     */
    void *dataPointer;

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
     * The number of dimensions associated to this AnyType.
     */
    uint8 numberOfDimensions;

    /**
     * The number of elements in each of the three possible dimensions.
     */
    uint32 numberOfElements[3];

    /**
     * If true => GetDataPointer() is a matrix dynamically allocated.
     */
    bool staticDeclared;

    /**
     * @brief Initialises all the dimensions to zero.
     *   GetNumberOfDimensions() == 0 &&
     *   GetNumberOfElements(0) == 1 &&
     *   GetNumberOfElements(1) == 1 &&
     *   GetNumberOfElements(2) == 1 &&
     *   IsStaticDeclared() == true
     */
    inline void Init();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

AnyType::AnyType(void) {
    Init();
}

AnyType::AnyType(const AnyType &x) {
    /*lint -e{1554} the dataPointer is to be shared with the copied AnyType.*/
    this->dataPointer = x.dataPointer;
    this->bitAddress = x.bitAddress;
    this->dataDescriptor = x.dataDescriptor;
    this->staticDeclared = x.IsStaticDeclared();
    this->numberOfDimensions = x.numberOfDimensions;
    this->numberOfElements[0] = x.numberOfElements[0];
    this->numberOfElements[1] = x.numberOfElements[1];
    this->numberOfElements[2] = x.numberOfElements[2];
}

AnyType::AnyType(const TypeDescriptor &dataDescriptorIn,
                 const uint8 bitAddressIn,
                 const void *const dataPointerIn) {
    Init();
    this->dataDescriptor = dataDescriptorIn;
    this->dataDescriptor.isConstant = true;
    this->dataPointer = const_cast<void*>(dataPointerIn);
    this->bitAddress = bitAddressIn;
}

AnyType::AnyType(const TypeDescriptor &dataDescriptorIn,
                 const uint8 bitAddressIn,
                 void *const dataPointerIn) {
    Init();
    this->dataDescriptor = dataDescriptorIn;
    this->dataPointer = dataPointerIn;
    this->bitAddress = bitAddressIn;
}

bool AnyType::IsVoid() const {
    return (dataDescriptor == VoidType);
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(char8 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = Character8Bit;
}

AnyType::AnyType(const char8 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<char8*>(&i));
    bitAddress = 0u;
    dataDescriptor = Character8Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int8 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger8Bit;
}

AnyType::AnyType(uint8 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger8Bit;
}

AnyType::AnyType(const int8 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<int8*>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger8Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const uint8 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<uint8*>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger8Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int16 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger16Bit;
}

AnyType::AnyType(uint16 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger16Bit;
}

AnyType::AnyType(const int16 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<int16*>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger16Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const uint16 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<uint16*>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger16Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int32 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger32Bit;
}

AnyType::AnyType(uint32 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger32Bit;
}

AnyType::AnyType(const int32 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<int32*>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger32Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const uint32 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<uint32*>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger32Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(int64 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = SignedInteger64Bit;
}

AnyType::AnyType(uint64 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger64Bit;
}

AnyType::AnyType(const int64 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<int64*>(&i));
    bitAddress = 0u;
    dataDescriptor = SignedInteger64Bit;
    dataDescriptor.isConstant = true;
}

AnyType::AnyType(const uint64 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<uint64*>(&i));
    bitAddress = 0u;
    dataDescriptor = UnsignedInteger64Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(float32 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = Float32Bit;
}

AnyType::AnyType(const float32 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<float32*>(&i));
    bitAddress = 0u;
    dataDescriptor = Float32Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/
AnyType::AnyType(float64 &i) {
    Init();
    dataPointer = static_cast<void*>(&i);
    bitAddress = 0u;
    dataDescriptor = Float64Bit;
}

AnyType::AnyType(const float64 &i) {
    Init();
    dataPointer = static_cast<void*>(const_cast<float64*>(&i));
    bitAddress = 0u;
    dataDescriptor = Float64Bit;
    dataDescriptor.isConstant = true;
}

/*---------------------------------------------------------------------------*/

AnyType::AnyType(volatile void *const p) {
    Init();
    dataPointer = (void*) p;
    bitAddress = 0u;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = Pointer;
    dataDescriptor.numberOfBits = sizeof(void*) * 8u;
}

AnyType::AnyType(volatile const void *const p) {
    Init();
    dataPointer = const_cast<void*>(p);
    bitAddress = 0u;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = true;
    dataDescriptor.type = Pointer;
    dataDescriptor.numberOfBits = sizeof(void*) * 8u;
}

AnyType::AnyType(volatile const char8 *const p) {
    Init();
    dataPointer = reinterpret_cast<void*>(const_cast<char8*>(p));
    bitAddress = 0u;
    dataDescriptor = ConstCharString;
}

AnyType::AnyType(volatile char8 *const p) {
    Init();
    dataPointer = reinterpret_cast<void*>(const_cast<char8*>(p));
    bitAddress = 0u;
    dataDescriptor = CharString;
}

/*lint -e{1555} Justification: an AnyType copied from another AnyType points at the same under underlying element memory.*/
AnyType& AnyType::operator=(const AnyType &src) {
    if (this != &src) {
        dataPointer = src.dataPointer;
        bitAddress = src.bitAddress;
        dataDescriptor = src.dataDescriptor;
        staticDeclared = src.staticDeclared;
        numberOfDimensions = src.numberOfDimensions;
        numberOfElements[0] = src.numberOfElements[0];
        numberOfElements[1] = src.numberOfElements[1];
        numberOfElements[2] = src.numberOfElements[2];
    }
    return *this;
}

/*---------------------------------------------------------------------------*/
template<typename baseType>
void AnyType::CreateFromOtherType(AnyType &dest,
                                  baseType &obj) {
    dest.dataPointer = static_cast<void*>(&obj);
    dest.bitAddress = 0u;

    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->FindTypeIdName(typeid(obj).name());
    if (classItem != NULL_PTR(ClassRegistryItem*)) {
        dest.dataDescriptor.isStructuredData = true;
        dest.dataDescriptor.isConstant = false;
        dest.dataDescriptor.structuredDataIdCode = static_cast<uint14>(classItem->GetClassProperties()->GetUniqueIdentifier());
    }
}

template<typename baseType>
void AnyType::CreateFromOtherType(AnyType &dest,
                                  const baseType &obj) {
    dest.dataPointer = static_cast<void*>(const_cast<baseType*>(&obj));
    dest.bitAddress = 0u;

    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->FindTypeIdName(typeid(obj).name());
    if (classItem != NULL_PTR(ClassRegistryItem*)) {
        dest.dataDescriptor.isStructuredData = true;
        dest.dataDescriptor.isConstant = true;
        dest.dataDescriptor.structuredDataIdCode = static_cast<uint14>(classItem->GetClassProperties()->GetUniqueIdentifier());
    }
}

template<typename baseType, uint8 bitOffset>
AnyType::AnyType(BitBoolean<baseType, bitOffset> &bitBool) {
    Init();
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = UnsignedInteger;
    dataDescriptor.numberOfBits = 1u;
    bitAddress = bitBool.BitOffset();
    dataPointer = static_cast<void*>(&bitBool);
}

template<typename baseType, uint8 bitSize, uint8 bitOffset>
AnyType::AnyType(BitRange<baseType, bitSize, bitOffset> &bitRange) {
    Init();
    BasicType type = (TypeCharacteristics<baseType>::IsSigned()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = bitRange.GetNumberOfBits();
    bitAddress = bitRange.BitOffset();
    dataPointer = static_cast<void*>(&bitRange);
}

template<typename baseType, uint8 bitSize>
AnyType::AnyType(FractionalInteger<baseType, bitSize> &fractionalInt) {
    Init();
    BasicType type = (TypeCharacteristics<baseType>::IsSigned()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = fractionalInt.GetNumberOfBits();
    bitAddress = 0;
    dataPointer = static_cast<void*>(&fractionalInt);
}

template<typename baseType, uint8 bitSize>
AnyType::AnyType(const FractionalInteger<baseType, bitSize> &fractionalInt) {
    Init();
    BasicType type = (TypeCharacteristics<baseType>::IsSigned()) ? SignedInteger : UnsignedInteger;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = true;
    dataDescriptor.type = type;
    dataDescriptor.numberOfBits = fractionalInt.GetNumberOfBits();
    bitAddress = 0u;
    dataPointer = static_cast<void*>(const_cast<FractionalInteger<baseType, bitSize>*>(&fractionalInt));
}

/*---------------------------------------------------------------------------*/

template<typename T, uint32 nOfElementsStatic>
AnyType::AnyType(T (&source)[nOfElementsStatic]) {
    Init();
    dataPointer = (void*) (&source[0]);
    numberOfDimensions = 1u;
    numberOfElements[0] = nOfElementsStatic;
    staticDeclared = true;

    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
    bitAddress = anyTypeDiscovery.GetBitAddress();
}

template<uint32 nOfElementsStatic>
AnyType::AnyType(char8 (&source)[nOfElementsStatic]) {
    Init();
    dataPointer = (void*) (&source[0]);
    numberOfDimensions = 1u;
    numberOfElements[0] = nOfElementsStatic;
    staticDeclared = true;
    dataDescriptor = Character8Bit;
    dataDescriptor.isConstant = false;
    bitAddress = 0u;
}

template<uint32 nOfElementsStatic>
AnyType::AnyType(const char8 (&source)[nOfElementsStatic]) {
    Init();
    dataPointer = (void*) (&source[0]);
    numberOfDimensions = 1u;
    numberOfElements[0] = nOfElementsStatic;
    staticDeclared = true;
    dataDescriptor = Character8Bit;
    dataDescriptor.isConstant = true;
    bitAddress = 0u;
}

template<typename T, uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
AnyType::AnyType(T (&source)[nOfRowsStatic][nOfColumnsStatic]) {
    Init();
    dataPointer = (void*) (&source);
    numberOfDimensions = 2u;
    numberOfElements[0] = nOfColumnsStatic;
    numberOfElements[1] = nOfRowsStatic;
    staticDeclared = true;
    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
    bitAddress = anyTypeDiscovery.GetBitAddress();
}

template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic>
AnyType::AnyType(char8 (&source)[nOfRowsStatic][nOfColumnsStatic]) {
    Init();
    dataPointer = (void*) (&source);
    numberOfDimensions = 2u;
    numberOfElements[0] = nOfColumnsStatic;
    numberOfElements[1] = nOfRowsStatic;

    staticDeclared = true;
    dataDescriptor = Character8Bit;
    dataDescriptor.isConstant = false;
    bitAddress = 0u;
}

template<uint32 nOfRowsStatic, uint32 nOfColumnsStatic, uint32 nOfChars>
AnyType::AnyType(char8 (&source)[nOfRowsStatic][nOfColumnsStatic][nOfChars]) {
    Init();
    dataPointer = (void*) (&source);
    numberOfDimensions = 3u;
    numberOfElements[0] = nOfColumnsStatic;
    numberOfElements[1] = nOfRowsStatic;
    numberOfElements[2] = nOfChars;
    staticDeclared = true;
    dataDescriptor = Character8Bit;
    dataDescriptor.isConstant = false;
    bitAddress = 0u;
}

template<typename T>
AnyType::AnyType(Matrix<T> &mat) {
    Init();
    dataPointer = mat.GetDataPointer();
    numberOfDimensions = 2u;
    numberOfElements[0] = mat.GetNumberOfColumns();
    numberOfElements[1] = mat.GetNumberOfRows();
    staticDeclared = mat.IsStaticDeclared();

    T typeDiscovery = static_cast<T>(0);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
    bitAddress = anyTypeDiscovery.GetBitAddress();
}

AnyType::AnyType(Matrix<char8> &mat) {
    Init();
    dataPointer = mat.GetDataPointer();
    numberOfDimensions = 2u;
    numberOfElements[0] = mat.GetNumberOfColumns();
    numberOfElements[1] = mat.GetNumberOfRows();
    staticDeclared = mat.IsStaticDeclared();
    dataDescriptor = Character8Bit;
    dataDescriptor.isStructuredData = false;
    dataDescriptor.isConstant = false;
    bitAddress = 0u;
}

template<typename T>
AnyType::AnyType(Vector<T> &vec) {
    Init();
    dataPointer = vec.GetDataPointer();
    numberOfDimensions = 1u;
    numberOfElements[0] = vec.GetNumberOfElements();

    T typeDiscovery = static_cast<T>(vec[0]);
    AnyType anyTypeDiscovery(typeDiscovery);
    dataDescriptor = anyTypeDiscovery.GetTypeDescriptor();
    bitAddress = anyTypeDiscovery.GetBitAddress();

}

AnyType::AnyType(Vector<char8> &vec) {
    Init();
    dataPointer = vec.GetDataPointer();
    numberOfDimensions = 1u;
    numberOfElements[0] = vec.GetNumberOfElements();
    dataDescriptor = Character8Bit;
    dataDescriptor.isConstant = false;
    bitAddress = 0u;
}

/*lint -e{1763} [MISRA C++ Rule 9-3-1]. Justification: function must be constant to allow the usage of the AnyType on function which require
 * a constant input. On purpose, even if the AnyType (which acts as a wrapper) cannot be modified, the value of the underlying element can. */
void* AnyType::GetDataPointer() const {
    return dataPointer;
}

void AnyType::SetDataPointer(void *const p) {
    dataPointer = p;
}

TypeDescriptor AnyType::GetTypeDescriptor() const {
    return dataDescriptor;
}

void AnyType::SetTypeDescriptor(const TypeDescriptor dataDescriptorIn) {
    dataDescriptor = dataDescriptorIn;
}

uint8 AnyType::GetBitAddress() const {
    return bitAddress;
}

uint8 AnyType::GetNumberOfDimensions() const {
    return numberOfDimensions;
}

void AnyType::SetNumberOfDimensions(const uint8 nOfDimensions) {
    numberOfDimensions = nOfDimensions;
}

bool AnyType::IsStaticDeclared() const {
    return staticDeclared;
}

void AnyType::SetStaticDeclared(const bool isStaticDeclared) {
    staticDeclared = isStaticDeclared;
}

uint32 AnyType::GetNumberOfElements(const uint32 dimension) const {
    return numberOfElements[dimension];
}

void AnyType::SetNumberOfElements(const uint32 dimension,
                                  const uint32 nOfElements) {
    numberOfElements[dimension] = nOfElements;
}

inline void AnyType::Init() {
    numberOfDimensions = 0u;
    numberOfElements[0] = 1u;
    numberOfElements[1] = 1u;
    numberOfElements[2] = 1u;
    staticDeclared = true;
    dataPointer = static_cast<void*>(NULL);
    bitAddress = 0u;
    dataDescriptor = VoidType;
}

uint32 AnyType::GetByteSize() const {
    return (GetBitSize() + 7u) / 8u;
}

uint32 AnyType::GetBitSize() const {
    uint32 byteSize = (dataDescriptor.numberOfBits + bitAddress);
    if (static_cast<bool>(dataDescriptor.isStructuredData)) {
        const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Peek(dataDescriptor.structuredDataIdCode);
        if (item != NULL_PTR(const ClassRegistryItem*)) {
            const ClassProperties *prop = item->GetClassProperties();
            if (prop != NULL_PTR(const ClassProperties*)) {
                byteSize = prop->GetSize()*8u;
            }
            else{
                //it is possible that only introspection exists
                const Introspection *intro=item->GetIntrospection();
                if (intro != NULL_PTR(const Introspection*)) {
                    byteSize = intro->GetClassSize()*8u;
                }
            }
        }
    }
    return byteSize;
}

uint32 AnyType::GetDataSize() const {

    uint32 totalDataSize = 0u;
    uint32 arrayLength = 1u;
    uint32 dataByteSize = GetByteSize();

    for (uint32 dimIdx = 0u; dimIdx < numberOfDimensions; dimIdx++) {

        arrayLength = arrayLength * numberOfElements[dimIdx];
    }

    totalDataSize = arrayLength * dataByteSize;

    return totalDataSize;
}

/**
 * Definition of the void AnyType (empty constructor).
 */
static const AnyType voidAnyType;

}

#endif /* ANYTYPE_H_ */

