/**
 * @file TypeDescriptor.h
 * @brief Header file for class TypeDescriptor
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

 * @details This header file contains the declaration of the class TypeDescriptor
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPEDESCRIPTOR_H_
#define TYPEDESCRIPTOR_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "BasicType.h"
#include "BitRange.h"
#include "BitBoolean.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 * Class UUID for the Class Registry Database.
 * To be eliminated - redundant with TypeDescriptor
 */
///typedef uint14 ClassUID;


/**
 * @brief A structure Used to describe the type pointed to by a pointer.
 * @details Depending on the first bit isStructuredData it may contain a code identifying a structure
 * or the remaining bits can be used to identify a specific basic type.
 *
 * Basic types are integers 8-64 bit, floats, doubles, char pointers and void pointers.
 *
 * @note The TypeDescriptor is internally represented as a 32-bit bitfield-like union with one of the following structures (depending on the type
 * described, if a basic one, or a structured one):
 *
 * | isStructuredData   | isConstant  | structuredDataIdCode                                                   |
 * | :----:             | :----:      | :----:                                                                 |
 * |  1  (=1)           | 1           | 30                                                                     |
 *
 * | isStructuredData   | isConstant  | type         | unused  | bitOffset | unused  | numberOfBits            |
 * | :----:             | :----:      | :----:       | :----:  | :----:    | :----:  | :----:                  |
 * |  1  (=0)           | 1           | 4 (Bit-Int)  | 4       | 6         | 2       | 16                      |
 *
 * | isStructuredData   | isConstant  | type               | objectSize    | arrayType      | unused           |
 * | :----:             | :----:      | :----:             | :----:        | :----:         | :----:           |
 * |  1  (=0)           | 1           | 4 (Int,Float,Char) | 3             | 3 (ZeroT,..)   | 20               |
  *
 * | isStructuredData   | isConstant  | type               | objectSize    | arrayType  | arraySize            |
 * | :----:             | :----:      | :----:             | :----:        | :----:     | :----:               |
 * |  1  (=0)           | 1           | 4 (Int,Float,Char) | 3             | 3 (1D,[n]) | 20                   |
 *
// * | isStructuredData   | isConstant  | type               | objectSize    | arrayType  | columns | rows       |
// * | :----:             | :----:      | :----:             | :----:        | :----:     | :----:  | :----:     |
// * |  1  (=0)           | 1           | 4 (Int,Float,Char) | 3             | 3 (2D)     | 10      | 10         |
 *
 * | isStructuredData   | isConstant  | type               | unused        | arrayType      | unused           |
 * | :----:             | :----:      | :----:             | :----:        | :----:         | :----:           |
 * |  1  (=0)           | 1           | 4 (others )        | 3             | 3 (ZeroT,..)   | 20               |
 *
 * | isStructuredData   | isConstant  | type               | unused        | arrayType  | arraySize            | arraySize>0
 * | :----:             | :----:      | :----:             | :----:        | :----:     | :----:               |
 * |  1  (=0)           | 1           | 4 (others)         | 3             | 3 (1D,[N]) | 20                   |
 *
// * | isStructuredData   | isConstant  | type               | unused        | arrayType  | columns | rows       | rows >=1
// * | :----:             | :----:      | :----:             | :----:        | :----:     | :----:  | :----:     | columns >=1
// * |  1  (=0)           | 1           | 4 (others)         | 3             | 3 (2D)     | 10      | 10         |
 *
 *
 */
class DLL_API TypeDescriptor {
public:

    /**
     * A structure for type informations storing.
     */
    /*lint -e{9018} Use of union allows to use this memory to describe or objects or basic types in an exclusive way.*/
    union {

        /**
         * If true then the data is a structure or class and its definition
         * has to be found in the ObjectRegistryDatabase
         */
        BitBoolean<uint32, 0u> isStructuredData;

        /**
         * The data is constant - cannot be written to
         */
        BitBoolean<uint32, 1u> isConstant;

        /*****************************************************
         *
         *        For isStructuredData = false
         *
         *****************************************************/

        /**
         * The actual type of data
         * See table in BasicType
         */
        BitRange<uint32, 4u, 2u> type;

        /*****************************************************
         *
         *        For type = int,float,char
         *
         *****************************************************/

        /**
         * The size of the type unknown/8/16/32/64/128/256/512 bits (1/2/4/8/16/32/64 bytes)
         * Determines the type of integer or float
         * For char[] this is 1
         * Values taken from BasicObjectSize
         */
        BitRange<uint32, 3u, 6u> objectSize;

        /**
         * The array type
         * 0 means unsupported
         * 1 means 1D
         * 2 means 2D
         * 3 means ?D+ too large array. - does not fit within 1D<1M and 2D <(1Kx1K) or too many dimensions
         * 4 means Zero Term Array
         * 5 means Dynamic Zero Term Array (reallocable)
         * 6 means Static Zero Term Array - equivalent to 1D but zero term
         * 7 means unsupported
         *  codes as BasicArryType
         */
        BitRange<uint32, 3u, 9u> arrayType;


        /*****************************************************
         *
         *        For arrayType = Array1D & StaticZeroTermArray
         *
         *****************************************************/

        /**
         * The vector size for 1D array and for ?D+ array the first dimensions size
         * Up to 1M  0 means Vector<T> 1 means scalar >1 means T[arraySize]
         * Used for char[] to indicate size of memory
         */
        BitRange<uint32, 20u, 12u> arraySize;


//      /*****************************************************
//         *
//         *        For arrayType = 2
//         *
//         *****************************************************/
//
//        /**
//         * The vector size
//         * Up to 1K  0 means- not determined
//         * Used for char[] to indicate size of memory
//         */
//        BitRange<uint32, 10u, 12u> numberOfRows;
//
//        /**
//         * The vector size
//         * Up to 1K 0 means- not determined
//         */
//        BitRange<uint32, 10u, 22u> numberOfColumns;



        /*****************************************************
         *
         *        For type = bit int
         *
         *****************************************************/

        /**
         * The bit offset
         * 0-63
         */
        BitRange<uint32, 6u, 8u> bitOffset;

        /**
         * The size in bits
         * Up to 65K
         */
        BitRange<uint32, 16u, 16u> numberOfBits;



        /*****************************************************
         *
         *        For isStructuredData = true
         *
         *****************************************************/

        /**
         * A code related univocally to a record in the ObjectRegistryDatabase
         */
        BitRange<uint32, 30u, 2u> structuredDataIdCode;

        /*****************************************************
         *
         *        as a 32 bit code
         *
         *****************************************************/

        /**
         * Fills all the memory area.
         */
        BitRange<uint32, 32u, 0u> all;
    };

    TypeDescriptor();

    /**
     * @brief Constructor by 32 bit integer.
     * @param[in] x contains the type informations which must be stored into this memory area.
     * @post x == all
     */
    TypeDescriptor(const uint32 x);

    /**
     * @brief Basic Byte Types constructor.
     * @param[in] isConstantIn specifies if the type is constant.
     * @param[in] typeIn is the type.
     * @param[in] numberOfBitsIn the number of bits associated to the type.
     * @param[in] bitsOffsetIn the bit offset of the type from a standard pointer address alignment
     * @post
     *   isConstantIn == isConstant &&
     *   if (bitsOffset=0 and numberOfbits exponential multiple of 8)
     *       type == signedBitInteger if typeIn was signedInteger
     *       type == unsignedBitInteger if typeIn was unsignedInteger
     *       otherwise type = invalid
     *   else
     *       type == typeIn
     *       byteSize = bitSizeIn/8
     *       arrayType = 1
     *       arraySize = 1
     *   end
     */
    TypeDescriptor(const bool isConstantIn,
                   const BasicType typeIn,
                   const uint32 numberOfBitsIn,
                   const uint8  bitsOffsetIn = 0);

    /**
     * @brief Basic Byte Types constructor.
     * TODO
     */
    TypeDescriptor(const bool isConstantIn,
                   const BasicType typeIn,
                   const BasicObjectSize objectSizeIn,
                   const BasicArrayType arrayTypeIn,
                   const uint32 arraySizeIn
//                   ,const uint32 numberOfColumnsIn
//                   ,const uint32 numberOfRowsIn
                   );

    /**
     * @brief Structured objects constructor.
     * @param[in] isConstantIn in specifies if the object is constant.
     * @param[in] structuredDataIdCodeIn is the 14 bit code associated to the object type.
     * @post
     *   isConstantIn == isConstant &&
     *   structuredDataIdCodeIn == structuredDataIdCode
     */
    TypeDescriptor(const bool isConstantIn,const uint32 &structuredDataIdCodeIn);

    /**
     * @brief Equality operator used to compare types.
     * @param[in] typeDescriptor is the type to be compared with.
     * @return In case of native types returns true if type and size fields are equal.
     * If the type is an object compares the structuredDataIdCode.
     */
    /*lint -e(1739) , operation basic_type == TypeDescriptor will not be supported*/
    bool operator==(const TypeDescriptor &typeDescriptor) const;

    /**
     * @brief Inequality operator used to compare types.
     * @param[in] typeDescriptor is the type to be compared with.
     * @return In case of native types returns true if type and size fields are equal.
     * If the type is an object compares the structuredDataIdCode.
     */
    /*lint -e(1739) , operation basic_type != TypeDescriptor will not be supported*/
    bool operator!=(const TypeDescriptor &typeDescriptor) const;

    /**
     * whether it is an integer with fractional bit size or offset
     */
    inline bool IsBitType() const ;

#if 0
    /**
     * @brief Retrieves the TypeDescriptor associated to the type name provided in input.
     * @param[in] typeName is the type name input.
     * @return the TypeDescriptor associated to \a typeName. If \a typeName is not matched returns InvalidType.
     */
    static TypeDescriptor GetTypeDescriptorFromTypeName(const char8 * const typeName);

    /**
     * @brief Retrieves the type name associated to the TypeDescriptor provided in input.
     * @param[in] typeDescriptor is the TypeDescriptor input.
     * @return the type name associated to \a typeDescriptor. If \a typeDescriptor is not matched returns NULL.
     */
    static const char8 *GetTypeNameFromTypeDescriptor(const TypeDescriptor &typeDescriptor);

    /**
     * @brief Provides direct access to the { name - TypeDescriptor } lookup table,
     * returning the TypeDescriptor in the specified position.
     * @details The { name - TypeDescriptor } lookup table is as follows: \n
     * TypeDescriptor        |TypeName
     * ----------------------|--------
     * CharString            | "string"
     * SignedInteger8Bit     | "int8"
     * SignedInteger16Bit    | "int16"
     * SignedInteger32Bit    | "int32"
     * SignedInteger64Bit    | "int64"
     * UnsignedInteger8Bit   | "uint8"
     * UnsignedInteger16Bit  | "uint16"
     * UnsignedInteger32Bit  | "uint32"
     * UnsignedInteger64Bit  | "uint64"
     * Float32Bit            | "float32"
     * Float64Bit            | "float64"
     * Character8Bit         | "char8"
     * VoidType              | "void"
     * InvalidType           | NULL
     * @param[in] index is the position inside the lookup table
     * @return the TypeDescriptor in the \a index position inside the lookup table.
     */
    static TypeDescriptor GetTypeDescriptorFromStaticTable(const uint32 index);

    /**
     * @brief Provides direct access to the type { name - TypeDescriptor } lookup table
     * (see GetTypeDescriptorFromStaticTable), returning the type name in the specified position.
     * @param[in] index is the position inside the lookup table
     * @return the type name in the \a index position inside the lookup table.
     */
    static const char8 *GetTypeNameFromStaticTable(const uint32 index);

#endif
};


/**
 * An array
 */
static const TypeDescriptor Character8Bit(false, Char, 8u);


/**
 * 32 bit float descriptor.
 */
static const TypeDescriptor Float32Bit(false, Float, 32u);

/**
 * 64 bit float descriptor.
 */
static const TypeDescriptor Float64Bit(false, Float, 64u);

/**
 * 128 bit Float descriptor
 */
static const TypeDescriptor Float128Bit(false, Float, 128u);

/**
 * Void descriptor - unknown -
 */
static const TypeDescriptor VoidType(false, Void, 0u);

/**
 * Invalid type descriptor
 */
static const TypeDescriptor InvalidType(false, Invalid, 0u);

/**
 * 8 bit signed integer descriptor
 */
static const TypeDescriptor SignedInteger8Bit(false, SignedInteger, 8u);

/**
 * 8 bit unsigned integer descriptor
 */
static const TypeDescriptor UnsignedInteger8Bit(false, UnsignedInteger, 8u);

/**
 * 16 bit signed integer descriptor
 */
static const TypeDescriptor SignedInteger16Bit(false, SignedInteger, 16u);

/**
 * 16 bit unsigned integer descriptor
 */
static const TypeDescriptor UnsignedInteger16Bit(false, UnsignedInteger, 16u);

/**
 * 32 bit signed integer descriptor
 */
static const TypeDescriptor SignedInteger32Bit(false, SignedInteger, 32u);

/**
 * 32 bit unsigned integer descriptor
 */
static const TypeDescriptor UnsignedInteger32Bit(false, UnsignedInteger, 32u);

/**
 * 64 bit signed integer descriptor
 */
static const TypeDescriptor SignedInteger64Bit(false, SignedInteger, 64u);

/**
 * 64 bit unsigned integer descriptor
 */
static const TypeDescriptor UnsignedInteger64Bit(false, UnsignedInteger, 64u);

/**
 * CCString  descriptor
 */
static const TypeDescriptor ConstCharString(true, Char, Size8bit,ZeroTermArray,  1);

/**
 * CString descriptor
 */
static const TypeDescriptor CharString(false,  Char, Size8bit,ZeroTermArray,  1);

/**
 *  Dynamic String descriptor char * = malloc
 */
static const TypeDescriptor DynamicCharString(false,  Char, Size8bit,DynamicZeroTermArray, 1);

/**
 *  Static char String descriptor char[1]
 */
static const TypeDescriptor StaticCharString(false,  Char, Size8bit,StaticZeroTermArray, 1);

/**
 * ConfigurationDatabase node
 */
static const TypeDescriptor StructuredDataInterfaceType(false, StructuredDataInterface, 0u);

/**
 * Pointer descriptor
 */
static const TypeDescriptor VoidPointer(false, Void, Size8bit,PointerArray,0);

/**
 * @brief Describes one layer of an array. returns as void[size] - size is set to 1 and need to be adjusted - the type is void as the size of the other array layers is unknown
 */
static const TypeDescriptor ArrayLayerType(false, Void, Size8bit,Array1D,0);


/**
 * @brief A large array- too large to fit within the models 1D[1024x1024] or 2D[1024][1024]
 */
//static const TypeDescriptor LargeArrayType(false, Void, ArrayLarge,0,0);


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool TypeDescriptor::IsBitType() const {
    return ((type ==  SignedBitInteger) || (type ==  UnsignedBitInteger));
};

}

#endif /* TYPEDESCRIPTOR_H_ */
