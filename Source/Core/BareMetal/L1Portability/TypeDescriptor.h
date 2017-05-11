/**
 * @file TypeDescriptor.h
 * @brief Header file for class TypeDescriptor
 * @date 26/08/2015
 * @author Filippo Sartori
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


/**
 * Macros to build one field of the TypeDescriptor
 */
#define TYPE_DESCRIPTOR_RANGEFUN(name,length,start,numberType)\
        BitRange<numberType, length, start> name ;

/**
 * Macros to build one field of the TypeDescriptor
 */
#define TYPE_DESCRIPTOR_BOOLFUN(name,start,numberType)\
        BitBoolean<numberType, start> name ;


/**
 * Macros to create constants to help generate TypeDescriptor constants
 */
#define TYPE_DESCRIPTOR_RANGEDEF(name,length,start,numberType)\
        const numberType TypeDescriptorRange_ ## name ## _Shift = start;

/**
 * Macros to create constants to help generate TypeDescriptor constants
 */
#define TYPE_DESCRIPTOR_BOOLDEF(name,start,numberType)\
        const numberType TypeDescriptorRange_ ## name ## _Shift = start;

/**
 * Macros to create range specific TypeDescriptor constants
 */
#define TYPE_DESCRIPTOR_RANGE_CONST(rangename,name,value,numberType)\
        const numberType TD ## name  = value << TypeDescriptorRange_ ## rangename ## _Shift;
#define TDRANGE(rangename,value)  (value << MARTe::TypeDescriptorRange_ ## rangename ## _Shift)


/**
 * combines macros rangeFun and boolFun with a defined use pattern
 * Allows creating and managing parallel definitions.
 */
#define TYPE_DESCRIPTOR_MACRO(rangeFun,boolFun,numberType)   \
		boolFun (isStructuredData          , 0,numberType)   \
	    boolFun (dataIsConstant            , 1,numberType)   \
	    /*  For isStructuredData = true   */                 \
	    rangeFun(structuredDataIdCode,30   , 2,numberType)   \
	    /*  For isStructuredData = false   */                \
	    rangeFun(type                , 3   , 2,numberType)   \
	    /*  For type = complexType         */                \
	    rangeFun(complexType         , 3   , 5,numberType)   \
	    /*  For type = int uint float char (void)    */      \
	    rangeFun(objectSize          , 3   , 5,numberType)   \
	    /* For objectSize = 7 SizeBits && int or uint */     \
	    rangeFun(bitOffset           , 6   ,10,numberType)   \
	    rangeFun(numberOfBits        ,16   ,16,numberType)   \
	    /* For objectSize<7 SizeBits && i/uint float char */ \
	    rangeFun(arrayProperty       , 3   , 8,numberType)   \
	    /* For arrayProperty sized */                        \
	    rangeFun(arraySize           ,21   ,11,numberType)   \
	    /* type = int,float,char For hasSize = false */      \
	    rangeFun(arrayType           , 3   ,11,numberType)   \
	    rangeFun(combinedArrayType   , 6   , 8,numberType)


#define TYPE_DESCRIPTOR_TYPE uint32

// defines all TypeDescriptoRange constants needed by TDRANGE macro functions
TYPE_DESCRIPTOR_MACRO(TYPE_DESCRIPTOR_RANGEDEF, TYPE_DESCRIPTOR_BOOLDEF, TYPE_DESCRIPTOR_TYPE)

// TDStructure
TYPE_DESCRIPTOR_RANGE_CONST(isStructuredData,Structure,1, TYPE_DESCRIPTOR_TYPE )

// TDConstant
TYPE_DESCRIPTOR_RANGE_CONST(dataIsConstant,Constant,1, TYPE_DESCRIPTOR_TYPE )


class DLL_API TypeDescriptor {
public:

    /**
     * A structure for type informations storing.
     */
    /*lint -e{9018} Use of union allows to use this memory to describe or objects or basic types in an exclusive way.*/
    union {
    	TYPE_DESCRIPTOR_MACRO(TYPE_DESCRIPTOR_RANGEFUN, TYPE_DESCRIPTOR_BOOLFUN, TYPE_DESCRIPTOR_TYPE)

#if 0
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
#endif
		TYPE_DESCRIPTOR_TYPE all;
    };

    TypeDescriptor();
    /**
     * @brief Constructor by 32 bit integer.
     * @param[in] x contains the type informations which must be stored into this memory area.
     * @post x == all
     */
    inline TypeDescriptor(const TYPE_DESCRIPTOR_TYPE x);

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
    uint32 Size() const;

};

#define  ConstCharString_number      (TDRANGE(type,Char)             | TDRANGE(objectSize,Size8bit)   | TDRANGE(combinedArrayType,ZeroTermArray)        | TDRANGE(dataIsConstant,1) )
#define  CharString_number           (TDRANGE(type,Char)             | TDRANGE(objectSize,Size8bit)   | TDRANGE(combinedArrayType,ZeroTermArray)        )
#define  DynamicCharString_number    (TDRANGE(type,Char)             | TDRANGE(objectSize,Size8bit)   | TDRANGE(combinedArrayType,DynamicZeroTermArray) )
#define  StaticCharString_number     (TDRANGE(type,Char)             | TDRANGE(objectSize,Size8bit)   | TDRANGE(combinedArrayType,StaticZeroTermArray)  )


#define  Character8Bit               TypeDescriptor(TDRANGE(type,Char)             | TDRANGE(objectSize,Size8bit)    | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1) )
#define  ConstCharacter8Bit          TypeDescriptor(TDRANGE(type,Char)             | TDRANGE(objectSize,Size8bit)    | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1) | TDRANGE(dataIsConstant,1))
#define  Float32Bit                  TypeDescriptor(TDRANGE(type,Float)            | TDRANGE(objectSize,Size32bit)   | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  Float64Bit                  TypeDescriptor(TDRANGE(type,Float)            | TDRANGE(objectSize,Size64bit)   | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  VoidType                    TypeDescriptor(TDRANGE(type,Void)             | TDRANGE(objectSize,SizeUnknown) | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  SignedInteger8Bit           TypeDescriptor(TDRANGE(type,SignedInteger)    | TDRANGE(objectSize,Size8bit)    | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  SignedInteger16Bit          TypeDescriptor(TDRANGE(type,SignedInteger)    | TDRANGE(objectSize,Size16bit)   | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  SignedInteger32Bit          TypeDescriptor(TDRANGE(type,SignedInteger)    | TDRANGE(objectSize,Size32bit)   | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  SignedInteger64Bit          TypeDescriptor(TDRANGE(type,SignedInteger)    | TDRANGE(objectSize,Size64bit)   | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  UnsignedInteger8Bit         TypeDescriptor(TDRANGE(type,UnsignedInteger)  | TDRANGE(objectSize,Size8bit)    | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  UnsignedInteger16Bit        TypeDescriptor(TDRANGE(type,UnsignedInteger)  | TDRANGE(objectSize,Size16bit)   | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  UnsignedInteger32Bit        TypeDescriptor(TDRANGE(type,UnsignedInteger)  | TDRANGE(objectSize,Size32bit)   | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1))
#define  UnsignedInteger64Bit        TypeDescriptor(TDRANGE(type,UnsignedInteger)  | TDRANGE(objectSize,Size64bit)   | TDRANGE(arrayProperty, SizedCArray_AP) | TDRANGE(arraySize, 1) )

#define  ConstCharString             TypeDescriptor(ConstCharString_number)
#define  CharString                  TypeDescriptor(CharString_number)
#define  DynamicCharString           TypeDescriptor(DynamicCharString_number)
#define  StaticCharString            TypeDescriptor(StaticCharString_number)
#define  VoidPointer                 TypeDescriptor(TDRANGE(type,Void)             | TDRANGE(objectSize,Size8bit)   | TDRANGE(combinedArrayType,PointerArray) )
#define  ConstVoidPointer            TypeDescriptor(TDRANGE(type,Void)             | TDRANGE(objectSize,Size8bit)   | TDRANGE(combinedArrayType,PointerArray)  | TDRANGE(dataIsConstant,1) )
#define  StructuredDataInterfaceType TypeDescriptor(TDRANGE(type,ComplexType)      | TDRANGE(complexType,StructuredDataInterface) | TDRANGE(arraySize,1))
#define  DelegatedType               TypeDescriptor(TDRANGE(type,Delegated)        | TDRANGE(objectSize,SizeUnknown)| TDRANGE(combinedArrayType,SizedCArray) | TDRANGE(arraySize,1))





/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool TypeDescriptor::IsBitType() const {
    return ((objectSize ==  SizeBits ) && (type != ComplexType) );
};

bool TypeDescriptor::IsComplexType() const {
    return (type ==  ComplexType );
};

TypeDescriptor::TypeDescriptor(const uint32 x) {
    all = x;
}

}

#endif /* TYPEDESCRIPTOR_H_ */
