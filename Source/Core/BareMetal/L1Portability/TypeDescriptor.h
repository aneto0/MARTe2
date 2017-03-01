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
 * | isStructuredData | dataConst  | structuredDataIdCode                                                   |
 * | :----:           | :----:     | :----:                                                                 |
 * |  1  (=1)         | 1          | 30                                                                     |
 *
 * | isStructuredData | dataConst  | type    | objectSize | spare | bitOffset | numberOfBits     |
 * | :----:           |:----:      | :----:  | :----:     | :---- | :----:    | :----:           |
 * |  1  (=0)         | 1          | 3=u/int | 3=7        | 2     | 6         | 16               |
 *
 * | isStructuredData | dataConst  | type    | objectSize | ArrayProperty | arraySize  |
 * | :----:           |:----:      | :----:  | :----:     | :----:        | :----:     |
 * |  1  (=0)         | 1          | 3=basic | 3          | 3  (sized)    | 21         |
 *
 * | isStructuredData | dataConst  | type    | objectSize | ArrayProperty |  ArrayType    | spare2
 * | :----:           |:----:      | :----:  | :----:     | :----:        | :----:        | :----:
 * |  1  (=0)         | 1          | 3=basic | 3          | 3 (unsized)   | 3             | 18
 *
 * | isStructuredData | dataConst  | type    | not used   | ArrayProperty |  ArrayType    | spare2     |
 * | :----:           |:----:      | :----:  | :----:     | :----:        | :----:        | :----:     |
 * |  1  (=0)         | 1          | 3=void  | 3          | 3             | 3=Ptr         | 18         |VOID*
 * |  1  (=0)         | 1          | 3=void  | 3          | 3             | n/a           | 18         |VOID
 *
 *
 * type = complex....
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
         * void const *
         */
        BitBoolean<uint32, 0u> isStructuredData;

        /**
         * The data is constant - cannot be written to
         */
        BitBoolean<uint32, 1u> dataIsConstant;

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
         *        For isStructuredData = false
         *
         *****************************************************/

        /**
         * The actual type of data
         * See table in BasicType
         */
        BitRange<uint32, 3u, 2u> type;

        /*****************************************************
         *
         *        For type = complexType
         *
         *****************************************************/
        /**
         * The subType related to complexType
         */
        BitRange<uint32, 3u, 5u> complexType;

        /**
         * The type and complexType joined
         */
        //BitRange<uint32, 6u, 2u> combinedType;

        /*****************************************************
         *
         *        For type = int uint float char (void)
         *
         *****************************************************/

        /**
         * The size of the type unknown/8/16/32/64/128/256/512 bits (1/2/4/8/16/32/64 bytes)
         * Determines the type of integer or float
         * For char[] this is 1
         * Values taken from BasicObjectSize
         */
        BitRange<uint32, 3u, 5u> objectSize;

        /*****************************************************
         *
         *        For objectSize = 7 SizeBits && int or uint
         *
         *****************************************************/

        /**
         * The bit offset
         * 0-63
         */
        BitRange<uint32, 6u, 10u> bitOffset;

        /**
         * The size in bits
         * Up to 65K
         */
        BitRange<uint32, 16u, 16u> numberOfBits;

        /*****************************************************
         *
         *        For objectSize < 7 SizeBits && i/uint float char
         *
         *****************************************************/

        /**
         * const ptr, sized array etc...
         */
        BitRange<uint32,3u, 8u> arrayProperty;

        /*****************************************************
         *
         *        For arrayProperty sized
         *
         *****************************************************/

        /**
         * The vector size for 1D array and for ?D+ array the first dimensions size
         * Up to 2M  0 means larger than 2M 1 means scalar
         * Used for char[] to indicate size of memory
         */
         BitRange<uint32, 21u, 11u> arraySize;

        /*****************************************************
         *		  type = int,float,char
         *        For hasSize = false
         *
         *****************************************************/

        /**
         * The array type
         *  codes as BasicArrayType
         */
         BitRange<uint32, 3u, 11u> arrayType;

         /**
          * arrayProperty & arrayType
          */
         BitRange<uint32, 6u, 8u> combinedArrayType;


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
     * TODO
     * Allows setting the complex subtypes
     */
    TypeDescriptor(const bool isConstantIn,const ComplexSubType 	subType);
    /**
     * TODO correct
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
                   const CombinedArrayType arrayTypeIn,
                   const uint32 arraySizeIn
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
     * whether it is an basic type or one of the special types
     */
    inline bool IsComplexType() const ;

    /**
     * whether it is an integer with fractional bit size or offset
     */
    inline bool IsBitType() const ;

    /**
     * size in byte of the object top layer.
     */
    uint32 Size();

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
extern const TypeDescriptor Character8Bit;


/**
 * 32 bit float descriptor.
 */
extern const TypeDescriptor Float32Bit;

/**
 * 64 bit float descriptor.
 */
extern const TypeDescriptor Float64Bit;

/**
 * 128 bit Float descriptor
 */
extern const TypeDescriptor Float128Bit;

/**
 * Void descriptor - unknown/invalid -
 */
extern const TypeDescriptor VoidType;

/**
 * 8 bit signed integer descriptor
 */
extern const TypeDescriptor SignedInteger8Bit;

/**
 * 8 bit unsigned integer descriptor
 */
extern const TypeDescriptor UnsignedInteger8Bit;

/**
 * 16 bit signed integer descriptor
 */
extern const TypeDescriptor SignedInteger16Bit;

/**
 * 16 bit unsigned integer descriptor
 */
extern const TypeDescriptor UnsignedInteger16Bit;

/**
 * 32 bit signed integer descriptor
 */
extern const TypeDescriptor SignedInteger32Bit;

/**
 * 32 bit unsigned integer descriptor
 */
extern const TypeDescriptor UnsignedInteger32Bit;

/**
 * 64 bit signed integer descriptor
 */
extern const TypeDescriptor SignedInteger64Bit;

/**
 * 64 bit unsigned integer descriptor
 */
extern const TypeDescriptor UnsignedInteger64Bit;

/**
 * CCString  descriptor
 */
extern const TypeDescriptor ConstCharString;

/**
 * CString descriptor
 */
extern const TypeDescriptor CharString;

/**
 *  Dynamic String descriptor char * = malloc
 */
extern const TypeDescriptor DynamicCharString;

/**
 *  Static char String descriptor char[1]
 */
extern const TypeDescriptor StaticCharString;

/**
 * ConfigurationDatabase node
 */
extern const TypeDescriptor StructuredDataInterfaceType;

/**
 * Pointer descriptor
 */
extern const TypeDescriptor VoidPointer;

/**
 * Pointer descriptor
 */
extern const TypeDescriptor ConstVoidPointer;


/**
 * Describes any unknown entity where the knowledge is delegate to a further TypeDescriptor
 */
extern const TypeDescriptor DelegatedType;



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool TypeDescriptor::IsBitType() const {
    return ((objectSize ==  SizeBits ) && (type != ComplexType) );
};

bool TypeDescriptor::IsComplexType() const {
    return (type ==  ComplexType );
};

}

#endif /* TYPEDESCRIPTOR_H_ */
