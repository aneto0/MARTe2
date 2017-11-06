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
#include "BitRange.h"
#include "BitBoolean.h"
#include "DynamicCString.h"


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
 * | fullType                                          | objectSize  | bitOffset | numberOfBits             |
 * | isStructuredData | dataConst  | category | type   | objectSize  | bitOffset | numberOfBits             |
 * | :----:           | :----:     | :----:   | :----: | :---------: | :----:    | :----:                   |
 * |  1  (=1)         | 1          | 2        | 4      | 8           | 8         | 8                        |
 *
 * | isStructuredData | dataConst  | category | type   | spare                                              |
 * | :----:           | :----:     | :----:   | :----: | :---------:                                        |
 * |  1  (=1)         | 1          | 2        | 4      | 24                                                 |
 *
 * type = complex....
 */

// used to encode fullType in the TypeDescriptor
typedef uint8 TD_FullType;

/**
 * @brief mask to extract the category.
 */
const TD_FullType TDF_Category 	    = 0x30u;

/**
 * @brief const to mark a basic Type.
 */
const TD_FullType TDF_BasicType 	= 0x00u;

/**
 * @brief const to mark a complex Type.
 */
const TD_FullType TDF_ComplexType 	= 0x10u;

/**
 * @brief const to mark a complex Type.
 */
const TD_FullType TDF_SyntheticType = 0x20u;

/**
 * @brief const to mark a complex Type.
 */
const TD_FullType TDF_SummaryType 	= 0x30u;

/**
 * @brief The type is a signed integer.
 */
const TD_FullType TDF_SignedInteger 	= TDF_BasicType + 0u;

/**
 * @brief The type is an unsigned integer.
 */
const TD_FullType TDF_UnsignedInteger 	= TDF_BasicType + 1u;

/**
 * @brief The type is a float number.
 */
const TD_FullType TDF_Float 			= TDF_BasicType + 2u;

/**
 * @brief The type is a char .
 */
const TD_FullType TDF_Char 				= TDF_BasicType + 3u;

/**
 * @brief The type is a char .
 */
const TD_FullType TDF_Void				= TDF_BasicType + 4u;

/**
 * @brief The type is a StreamString class.
 */
const TD_FullType  TDF_SString 			= TDF_ComplexType + 0u;

/**
 * @brief The type is a StreamInterface class.
 */
const TD_FullType  TDF_Stream 			= TDF_ComplexType + 1u;

/**
 * @brief  The StructuredDataI type
 */
const TD_FullType  TDF_StructuredDataI   = TDF_ComplexType + 2u;

/**
 * @brief  The CCString/CString
 */
const TD_FullType  TDF_CString 			= TDF_SyntheticType + 0u;

/**
 * @brief  The void *
 */
const TD_FullType  TDF_VoidPointer 		= TDF_SyntheticType + 1u;

/**
 * @brief  The something[]
 */
const TD_FullType  TDF_GenericArray 	= TDF_SummaryType + 0u;

/**
 * @brief  The something wrong
 */
const TD_FullType  TDF_Invalid	 		= TDF_SummaryType + 1u;

/**
 * @brief  The something*
 */
const TD_FullType  TDF_GenericPointer   = TDF_SummaryType + 2u;

/**
 * @brief Definition of BasicArrayType
 */
typedef uint8 TDObjectSize;

/**
 * @brief The type has an unknown size
 */
const TDObjectSize SizeUnknown = 0u;

/**
 * @brief The type has size 8bit
 */
const TDObjectSize Size8bit = 1u;

/**
 * @brief The type has size 16bit
 */
const TDObjectSize Size16bit = 2u;

/**
 * @brief The type has size 32bit
 */
const TDObjectSize Size32bit = 3u;

/**
 * @brief The type has size 64bit
 */
const TDObjectSize Size64bit = 4u;

/**
 * @brief The type has size 128bit
 */
const TDObjectSize Size128bit = 5u;

/**
 * @brief The type has size 256bit
 */
const TDObjectSize Size256bit = 6u;

/**
 * @brief The type has bit size
 */
const TDObjectSize SizeBits = 7u;

/**
 * @brief The size of a pointer
 */
const TDObjectSize SizePointer = (sizeof(void*)==8) ? Size64bit : (sizeof(void*)==4) ? Size32bit : Size16bit;

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
	    rangeFun(fullType            , 6   , 2,numberType)   \
	    /*  For type = int uint float char (void)    */      \
	    rangeFun(objectSize          , 8   , 8,numberType)   \
	    /* For objectSize = 7 SizeBits && int or uint */     \
	    rangeFun(bitOffset           , 8   ,16,numberType)   \
	    rangeFun(numberOfBits        , 8   ,24,numberType)


#define TYPE_DESCRIPTOR_TYPE uint32

// defines all TypeDescriptoRange constants needed by TDRANGE macro functions
TYPE_DESCRIPTOR_MACRO(TYPE_DESCRIPTOR_RANGEDEF, TYPE_DESCRIPTOR_BOOLDEF, TYPE_DESCRIPTOR_TYPE)

// generates TDStructure macro
TYPE_DESCRIPTOR_RANGE_CONST(isStructuredData,Structure,1, TYPE_DESCRIPTOR_TYPE )

// generates TDConstant macro
TYPE_DESCRIPTOR_RANGE_CONST(dataIsConstant,Constant,1, TYPE_DESCRIPTOR_TYPE )

/**
 *
 */
class DLL_API TypeDescriptor {
public:

    /**
     * A structure for type informations storing.
     */
    /*lint -e{9018} Use of union allows to use this memory to describe or objects or basic types in an exclusive way.*/
    union {
    	TYPE_DESCRIPTOR_MACRO(TYPE_DESCRIPTOR_RANGEFUN, TYPE_DESCRIPTOR_BOOLFUN, TYPE_DESCRIPTOR_TYPE)

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
    inline bool operator==(const TypeDescriptor &typeDescriptor) const;

    /**
     * @brief Inequality operator used to compare types.
     * @param[in] typeDescriptor is the type to be compared with.
     * @return In case of native types returns true if type and size fields are equal.
     * If the type is an object compares the structuredDataIdCode.
     */
    /*lint -e(1739) , operation basic_type != TypeDescriptor will not be supported*/
    inline bool operator!=(const TypeDescriptor &typeDescriptor) const;

    /**
     * @brief whether it is an basic type
     */
    inline bool IsBasicType() const ;

    /**
     * whether it is an integer with fractional bit size or offset
     */
    inline bool IsBitType() const ;

    /**
     * @brief whether it is one of the special types
     */
    inline bool IsComplexType() const ;

    /**
     * size in byte of the object top layer.
     */
    uint32 Size() const;

    /**
     * @brief gets name of class from structuredDataIdCode
     * only valid if the type is a structure
     * if not found returns an empty string
     */
    CCString GetNameOfClassFromStructureId() const;

    /**
     * @brief gets name of class from structuredDataIdCode
     */
    static bool SetNameOfClassFromIdFunction(CCString (*fun)(TypeDescriptor));

    /**
     * @brief Converts type descriptor to C/c++ equivalent
     *
     */
    bool ToString(DynamicCString &string) const;
};


#define  Character8Bit_number        TDRANGE(fullType,TDF_Char)  | TDRANGE(objectSize,Size8bit)
#define  Character8Bit               TypeDescriptor(Character8Bit_number)
#define  ConstCharacter8Bit_number   TDRANGE(fullType,TDF_Char)  | TDRANGE(objectSize,Size8bit) | TDRANGE(dataIsConstant,1)
#define  ConstCharacter8Bit          TypeDescriptor(ConstCharacter8Bit_number)
#define  Float32Bit_number           TDRANGE(fullType,TDF_Float) | TDRANGE(objectSize,Size32bit)
#define  Float32Bit                  TypeDescriptor(Float32Bit_number)
#define  Float64Bit_number           TDRANGE(fullType,TDF_Float) | TDRANGE(objectSize,Size64bit)
#define  Float64Bit                  TypeDescriptor(Float64Bit_number)
#define  VoidType_number             TDRANGE(fullType,TDF_Void)  | TDRANGE(objectSize,SizeUnknown)
#define  VoidType                    TypeDescriptor(VoidType_number)

#define  SignedInteger8Bit           TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger)    | TDRANGE(objectSize,Size8bit)    )
#define  SignedInteger16Bit          TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger)    | TDRANGE(objectSize,Size16bit)   )
#define  SignedInteger32Bit          TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger)    | TDRANGE(objectSize,Size32bit)   )
#define  SignedInteger64Bit          TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger)    | TDRANGE(objectSize,Size64bit)   )
#define  UnsignedInteger8Bit         TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger)  | TDRANGE(objectSize,Size8bit)    )
#define  UnsignedInteger16Bit        TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger)  | TDRANGE(objectSize,Size16bit)   )
#define  UnsignedInteger32Bit        TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger)  | TDRANGE(objectSize,Size32bit)   )
#define  UnsignedInteger64Bit        TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger)  | TDRANGE(objectSize,Size64bit)   )

#define  StreamType                  TypeDescriptor(TDRANGE(fullType,TDF_Stream) )
#define  StructuredDataInterfaceType TypeDescriptor(TDRANGE(fullType,TDF_StructuredDataI) )
#define  StreamStringType            TypeDescriptor(TDRANGE(fullType,TDF_SString) )

#define  VoidPointer                 TypeDescriptor(TDRANGE(fullType,TDF_VoidPointer) | TDRANGE(objectSize,SizePointer) )
#define  ConstVoidPointer            TypeDescriptor(TDRANGE(fullType,TDF_VoidPointer) | TDRANGE(objectSize,SizePointer) | TDRANGE(dataIsConstant,1) )
#define  GenericPointer              TypeDescriptor(TDRANGE(fullType,TDF_GenericPointer) | TDRANGE(objectSize,SizePointer) )



/*******************************************************/
/* Types only used as a summary of a more complex type */
/*******************************************************/
//#define  GenericPointer_number       TDRANGE(type,Delegated)        | TDRANGE(objectSize,SizeUnknown) | TDRANGE(combinedArrayType,PointerArray)
//#define  GenericPointer              TypeDescriptor(GenericPointer_number )
#define  ConstCharString_number      (TDRANGE(fullType,TDF_CString) | TDRANGE(objectSize,SizePointer)   | TDRANGE(dataIsConstant,1) )
#define  ConstCharString             TypeDescriptor(ConstCharString_number)
#define  GenericArray_number         TDRANGE(fullType,TDF_GenericArray) | TDRANGE(objectSize,SizeUnknown)
#define  GenericArray                TypeDescriptor(InvalidType_number)
#define  InvalidType_number          TDRANGE(fullType,TDF_Invalid)
#define  InvalidType                 TypeDescriptor(InvalidType_number)

#define  UnsignedBitSet_number(bits,offset) TDRANGE(fullType,TDF_UnsignedInteger) | TDRANGE(objectSize,SizeBits) | TDRANGE(numberOfBits,bits) | TDRANGE(bitOffset,offset)
#define  UnsignedBitSet(bits,offset) TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger) | TDRANGE(objectSize,SizeBits) | TDRANGE(numberOfBits,bits) | TDRANGE(bitOffset,offset))
#define  SignedBitSet_number(bits,offset) TDRANGE(fullType,TDF_SignedInteger) | TDRANGE(objectSize,SizeBits) | TDRANGE(numberOfBits,bits) | TDRANGE(bitOffset,offset)
#define  SignedBitSet(bits,offset) TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger) | TDRANGE(objectSize,SizeBits) | TDRANGE(numberOfBits,bits) | TDRANGE(bitOffset,offset))
#define  BitSetBoolean(offset) UnsignedBitSet(1,offset)


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool TypeDescriptor::IsBasicType() const {
    return ( !isStructuredData && ((fullType & TDF_Category) == TDF_BasicType) );
};

bool TypeDescriptor::IsBitType() const {
    return ((objectSize ==  SizeBits ) && IsBasicType() );
};

bool TypeDescriptor::IsComplexType() const {
    return ( !isStructuredData && ((fullType & TDF_Category) == TDF_ComplexType) );
};

TypeDescriptor::TypeDescriptor(const uint32 x) {
    all = x;
}

bool TypeDescriptor::operator==(const TypeDescriptor &typeDescriptor) const {
    return all == typeDescriptor.all;
}

bool TypeDescriptor::operator!=(const TypeDescriptor &typeDescriptor) const {
    return all == typeDescriptor.all;
}
}

#endif /* TYPEDESCRIPTOR_H_ */
