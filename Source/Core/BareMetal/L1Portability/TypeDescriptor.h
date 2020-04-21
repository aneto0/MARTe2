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
#include "CString.h"


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
 * @remark The TypeDescriptor is internally represented as a 32-bit bitfield-like union with one of the following structures (depending on the type
 * described, if a basic one, or a structured one):
 *
 *
 * | isStructuredData |dataIsConstant| structuredDataIdCode                                                   |
 * | :----:           | :----:       | :----:                                                                 |
 * |  1  (=1)         | 1            | 30                                                                     |
 *
 * | isStructuredData |dataIsConstant| fulltype | basicTypeSize | hasBitSize | sp | bitOffset | numberOfBits             |
 * | :----:           | :----:       | :----:   | :---------:   | :---------:|    | :----:    | :----:                   |
 * |  1  (=0)         | 1            | 6=0x0*   | 4             |  1         | 3  | 8         | 8                        |
 *
 * | isStructuredData |dataIsConstant| fulltype | format      |
 * | :----:           | :----:       | :----:   | :---------: |
 * |  1  (=0)         | 1            | 6=0x18*  | 24          |
 *
 * | isStructuredData |dataIsConstant| fulltype | objectSize  |
 * | :----:           | :----:       | :----:   | :---------: |
 * |  1  (=0)         | 1            | 6=0x2*   | 24          |
 */

// used to encode fullType in the TypeDescriptor
typedef uint8 TD_FullType;

/**
 * @brief mask to extract the category.
 */
const TD_FullType TDF_Category 	    = 0x30u;

/**
 * @brief const to mark a basic Type.
 * @details a basic type can be copied bit by bit
 */
const TD_FullType TDF_BasicType 	= 0x00u;

/**
 * @brief const to mark a type that is a sequence of characters
 * @details a basic type that can be copied by character streaming
 */
const TD_FullType TDF_CharStreamType= 0x10u;

/**
 * @brief  bits that apply to all STREAM types and disable the size information and enables the format information
 */
const TD_FullType TDF_HasFormatInfo = 0x08u;

/**
 * @brief const to mark a type that is none of the above.
 * @details SyntheticType types are special modifications of basic types
 * In general they cannot be simply copied. Special treatment is required
 */
const TD_FullType TDF_SpecialType   = 0x20u;

/**
 * @brief const to mark a summary/incomplete Type.
 * @details a summary type is not a complete type description. It represent part of a type
 * They cannot be used as part of any operation
 */
const TD_FullType TDF_SummaryType 	= 0x30u;


/*****  BASIC TYPES *****/
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
 * @brief  The pointer to something
 */
const TD_FullType  TDF_Pointer 			= TDF_BasicType + 4u;


/*****  CHAR STREAM TYPES *****/

/**
 * @brief The type is a StreamString class.
 */
const TD_FullType  TDF_SString 			= TDF_CharStreamType + 0u;

/**
 * @brief The type is a StreamInterface class. // storage size unavailable
 */
const TD_FullType  TDF_Stream 			= TDF_CharStreamType + 1u;

/**
 * @brief  The const char * (CCString, const DynamicCString etc...)
 */
const TD_FullType  TDF_CCString 		= TDF_CharStreamType + 2u;

/**
 * @brief  The const char * (CString)
 */
const TD_FullType  TDF_CString 		    = TDF_CharStreamType + 3u;

/**
 * @brief  The DynamicCString : can be written to
 */
const TD_FullType  TDF_DynamicCString 	= TDF_CharStreamType + 4u;


/*****  SPECIAL TYPES *****/

/**
 * @brief  have the StructuredDataI interface
 */
const TD_FullType  TDF_StructuredDataI   = TDF_SpecialType + 0u;

/**
 * @brief  Descendant from Object
 */
const TD_FullType  TDF_Object            = TDF_SpecialType + 1u;

/*****  SUMMARY TYPES *****/

/**
 * @brief  The something[]
 */
const TD_FullType  TDF_GenericArray 	= TDF_SummaryType + 0u;

/**
 * @brief  The something wrong
 */
const TD_FullType  TDF_Invalid	 		= TDF_SummaryType + 1u;

/**
 * @brief The type is nothing
 */
const TD_FullType  TDF_Void				= TDF_SummaryType + 2u;

/**
 * @brief  Pointer to ZTA, Pointer to [], Vector or Matrix
 */
const TD_FullType  TDF_GenericPointer   = TDF_SummaryType + 3u;

/**
 * @brief  returned when asking for the FullType of a structure...
 */
const TD_FullType  TDF_Structure        = TDF_SummaryType + 4u;


/**
 * @brief Definition of BasicArrayType
 */
typedef uint8 TDBasicTypeSize;

/**
 * @brief The type has an unknown size
 */
const TDBasicTypeSize SizeUnknown = 0u;

/**
 * @brief The type has size 8bit
 */
const TDBasicTypeSize Size8bit = 1u;

/**
 * @brief The type has size 16bit
 */
const TDBasicTypeSize Size16bit = 2u;

/**
 * @brief The type has size 32bit
 */
const TDBasicTypeSize Size32bit = 3u;

/**
 * @brief The type has size 64bit
 */
const TDBasicTypeSize Size64bit = 4u;

/**
 * @brief The size of a pointer
 */
const TDBasicTypeSize SizePointer = (sizeof(void*)==8) ? Size64bit : (sizeof(void*)==4) ? Size32bit : Size16bit;

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
		/* For type char streams  */        \
	    rangeFun(format              ,24   , 8,numberType)   \
		/* For type =special                       */        \
	    rangeFun(objectSize          ,24   , 8,numberType)   \
	    /*  For type = int uint float char (void)    */      \
	    rangeFun(basicTypeSize       , 4   , 8,numberType)   \
		boolFun (hasBitSize                ,12,numberType)   \
	    /* For basicTypeSize with hasBitSize    */           \
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

	/**
	 * allow access to internals only to the test class
	 */
	friend class TypeDescriptorTest;

    /**
     * A structure for type informations storing.
     */
    /*lint -e{9018} Use of union allows to use this memory to describe or objects or basic types in an exclusive way.*/
    union {
    	TYPE_DESCRIPTOR_MACRO(TYPE_DESCRIPTOR_RANGEFUN, TYPE_DESCRIPTOR_BOOLFUN, TYPE_DESCRIPTOR_TYPE)

		TYPE_DESCRIPTOR_TYPE all;
    };

public:

    /**
     *
     */
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
    inline TypeDescriptor(const bool isConstantIn,const uint32 &structuredDataIdCodeIn);

    /**
     * @brief String to typeDescriptor
     * recognizes basic Types, CCString,CString, and BitRange<type,nn,nn>
     * recognizes registered classes
     * does not handle modifiers like const
     */
    TypeDescriptor(CCString typeName);

    /**
     * @brief allows accessing the full code;
     */
    inline TYPE_DESCRIPTOR_TYPE GetFullCode() const;

    /**
     * @brief allows accessing the full type code
     * returns TDF_Structure if isStructureData is true
     */
    inline TD_FullType GetFullTypeCode() const;

    /**
     * @brief allows accessing the id code of a structure;
     */
    inline uint32 GetStructuredDataIdCode() const;

    /**
     * @brief allows accessing the size in bits of a bitrange
     * use StorageSize() to get size of integer/floats
     * returns -1 for non bitrange
     */
    inline int32 GetNumericBitSize() const;

    /**
     * @brief allows accessing the offset in bits of a bitrange
     * returns -1 for non numeric
     */
    inline int32 GetNumericBitOffset() const;

    /**
     * @brief sets the dataIsConstant flag;
     */
    inline void  SetDataConstant(bool dataIsConstant);

    /**
     * @brief whether it is an basic type
     */
    inline bool DataIsConstant() const ;

    /**
     * @brief whether it is an basic type
     */
    inline bool IsBasicType() const ;

    /**
     * @brief whether it is a number (int or float, no fractional integers)
     */
    inline bool IsNumericType() const ;

    /**
     * @brief whether it is a character string/stream of sort
     */
    inline bool IsCharStreamType() const ;

    /**
     * @brief whether it is a character string/stream of sort and has format information (CDB,XML etc..)
     */
    inline bool IsFormattedCharStreamType() const;

    /**
     * @brief whether it is a structure
     */
    inline bool IsStructuredData() const ;

    /**
     * @brief whether it is a form of char *
     */
    inline bool IsCharString() const ;

    /**
     * @brief whether it is a StreamString
     */
    inline bool IsStreamString() const ;

    /**
     * @brief whether it is any of the special types
     */
    inline bool IsSpecialType() const;

    /**
     * @brief whether it is an integer with fractional bit size or offset
     */
    inline bool IsBitType() const ;

    /**
     * @brief whether the type is float or signed int
     */
    inline bool IsSigned() const;

    /**
     * @brief whether the type is invalid
     */
    inline bool IsInvalid() const ;

    /**
     * @brief matches all details
     */
    inline bool SameAs(const TypeDescriptor &td) const;

    /**
     * @brief matches all details but the constantness
     */
    inline bool SameTypeAndSizeAs(const TypeDescriptor &td) const;

    /**
     * @brief matches only the type. It is the same structure or the same fullType
     */
    inline bool SameTypeAs(const TypeDescriptor &td) const;

    /**
     * @brief returns real size in byte of the object including storage size
     * @details - for character streams, it will compute the actual size
     */
    uint32 FullSize(const uint8 *address= NULL_PTR(const uint8 *)) const;

    /**
     * @brief the size needed to store the type (sizeof(T) )
     */
    uint32 StorageSize() const;

    /**
     * @brief gets name of class from structuredDataIdCode
     * only valid if the type is a structure
     * if not found returns an empty string
     */
    CCString GetNameOfClassFromStructureId() const;

    /**
     * @brief Sets the global function to translate a TypeDescriptor containing a structure id to String.
     * not to be used
     * TODO Move to private header
     */
    static bool SetNameOfClassFromIdFunction(CCString (*fun)(TypeDescriptor));

    /**
     * @brief Converts type descriptor to C/c++ equivalent
     *
     */
    bool ToString(CStringTool &string) const;

    /**
     * @brief retrieves the format of the formatted char stream
     * only valid if IsFormattedCharStreamType
     */
    bool GetStreamFormat(CStringTool &string) const;

    /**
     * @brief converts a 1-4 byte string into a 24 bit code that can be used as format
     */
    static uint32 String2FormatNumber(CCString formatString);

private:

    /**
     * @brief Equality operator used to compare types. Forced private to avoid its use as its meaning is ambiguous.
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

};

/*** REGISTERED STRUCTURES */
#define  StructuredDataType(id)           TypeDescriptor(TDRANGE(isStructuredData,true)    | TDRANGE(structuredDataIdCode,id)    )


/***  BASIC TYPES */
#define  Character8Bit_number        TDRANGE(fullType,TDF_Char)  | TDRANGE(basicTypeSize,Size8bit)
#define  Character8Bit               TypeDescriptor(Character8Bit_number)
#define  ConstCharacter8Bit_number   TDRANGE(fullType,TDF_Char)  | TDRANGE(basicTypeSize,Size8bit) | TDRANGE(dataIsConstant,1)
#define  ConstCharacter8Bit          TypeDescriptor(ConstCharacter8Bit_number)
#define  Float32Bit_number           TDRANGE(fullType,TDF_Float) | TDRANGE(basicTypeSize,Size32bit)
#define  Float32Bit                  TypeDescriptor(Float32Bit_number)
#define  Float64Bit_number           TDRANGE(fullType,TDF_Float) | TDRANGE(basicTypeSize,Size64bit)
#define  Float64Bit                  TypeDescriptor(Float64Bit_number)
#define  VoidType_number             TDRANGE(fullType,TDF_Void)  | TDRANGE(basicTypeSize,SizeUnknown)
#define  VoidType                    TypeDescriptor(VoidType_number)
#define  SignedInteger8Bit           TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger)    | TDRANGE(basicTypeSize,Size8bit)    )
#define  SignedInteger16Bit          TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger)    | TDRANGE(basicTypeSize,Size16bit)   )
#define  SignedInteger32Bit          TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger)    | TDRANGE(basicTypeSize,Size32bit)   )
#define  SignedInteger64Bit          TypeDescriptor(TDRANGE(fullType,TDF_SignedInteger)    | TDRANGE(basicTypeSize,Size64bit)   )
#define  UnsignedInteger8Bit         TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger)  | TDRANGE(basicTypeSize,Size8bit)    )
#define  UnsignedInteger16Bit        TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger)  | TDRANGE(basicTypeSize,Size16bit)   )
#define  UnsignedInteger32Bit        TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger)  | TDRANGE(basicTypeSize,Size32bit)   )
#define  UnsignedInteger64Bit        TypeDescriptor(TDRANGE(fullType,TDF_UnsignedInteger)  | TDRANGE(basicTypeSize,Size64bit)   )
#define  PointerType                 TypeDescriptor(TDRANGE(fullType,TDF_Pointer)          | TDRANGE(basicTypeSize,SizePointer) )
#define  ConstPointerType            TypeDescriptor(TDRANGE(fullType,TDF_Pointer)          | TDRANGE(basicTypeSize,SizePointer) | TDRANGE(dataIsConstant,1))
#define  GenericPointer              TypeDescriptor(TDRANGE(fullType,TDF_GenericPointer)   | TDRANGE(basicTypeSize,SizePointer) )
#define  BooleanType                 UnsignedInteger8Bit;


/**  STREAM TYPES */
#define  CharString(size)       	 TypeDescriptor(TDRANGE(fullType,TDF_CString)       | TDRANGE(objectSize,size))
#define  ConstCharString             TypeDescriptor(TDRANGE(fullType,TDF_CCString)      | TDRANGE(objectSize,0))
#define  StreamStringType(size)      TypeDescriptor(TDRANGE(fullType,TDF_SString)       | TDRANGE(objectSize,size))
#define  DynamicCharString           TypeDescriptor(TDRANGE(fullType,TDF_DynamicCString)| TDRANGE(objectSize,0))
#define  StreamIType(size)           TypeDescriptor(TDRANGE(fullType,TDF_Stream)        | TDRANGE(objectSize,size))

/**  FORMATTED STREAM TYPES */
const CCString cdbFormat("CDB");
const CCString jsonFormat("JSON");
const CCString xmlFormat("XML");
#define  FormattedConstCharString(f)   TypeDescriptor(TDRANGE(fullType,TDF_CCString       | TDF_HasFormatInfo) | TDRANGE(format,TypeDescriptor::String2FormatNumber(f)))
#define  FormattedDynamicCharString(f) TypeDescriptor(TDRANGE(fullType,TDF_DynamicCString)| TDF_HasFormatInfo) | TDRANGE(format,TypeDescriptor::String2FormatNumber(f)))
#define  FormattedStreamStringType(f)  TypeDescriptor(TDRANGE(fullType,TDF_SString)       | TDF_HasFormatInfo) | TDRANGE(format,TypeDescriptor::String2FormatNumber(f)))

/** OTHER */
#define  StructuredDataIType(size)   TypeDescriptor(TDRANGE(fullType,TDF_StructuredDataI) | TDRANGE(objectSize,size))
#define  ObjectType(size)            TypeDescriptor(TDRANGE(fullType,TDF_Object)          | TDRANGE(objectSize,size))
#define  InvalidType(size)           TypeDescriptor(TDRANGE(fullType,TDF_Invalid)         | TDRANGE(objectSize,size))
#define  GenericArray                TypeDescriptor(TDRANGE(fullType,TDF_GenericArray)    | TDRANGE(objectSize,0))

/**
 * @return returns the TDBasicTypeSize of any type
 */
#define TDBasicTypeSizeOf(x)\
	(sizeof(x)<=2?(sizeof(x)==1?Size8bit:Size16bit):(sizeof(x)==4?Size32bit:(sizeof(x)==8?Size64bit:SizeUnknown)))

/**
 * @return returns the byte size from the TDBasicTypeSize
 */
#define SizeFromTDBasicTypeSize(x)	(x==0?0u:(1u << (x-1)))

/**
 * @return returns the TypeDescriptor from a type
 * support only integers and floats
 */
template <typename T> const TypeDescriptor Type2TypeDescriptor();

/**
 * @return for the unsupported types just report the size
 */
template <typename T> static inline const TypeDescriptor Type2TypeDescriptor(){
	return InvalidType(sizeof(T));
}
template <> inline const TypeDescriptor Type2TypeDescriptor<int8>();
template <> inline const TypeDescriptor Type2TypeDescriptor<int16>();
template <> inline const TypeDescriptor Type2TypeDescriptor<int32>();
template <> inline const TypeDescriptor Type2TypeDescriptor<int64>();
template <> inline const TypeDescriptor Type2TypeDescriptor<uint8>();
template <> inline const TypeDescriptor Type2TypeDescriptor<uint16>();
template <> inline const TypeDescriptor Type2TypeDescriptor<uint32>();
template <> inline const TypeDescriptor Type2TypeDescriptor<uint64>();
template <> inline const TypeDescriptor Type2TypeDescriptor<float32>();
template <> inline const TypeDescriptor Type2TypeDescriptor<float64>();

/*
template<typename baseType, uint8 numberOfBits, uint8 bitOffset>
template <> const TypeDescriptor Type2TypeDescriptor<BitRange<baseType,numberOfBits,bitOffset> >(){
	return TypeCharacteristics<baseType>::IsSigned()?:SignedBitSet_number(baseType,numberOfBits,bitOffset),UnsignedBitSet_number(baseType,numberOfBits,bitOffset);
}
*/


#define  UnsignedBitSet_number(base,bits,offset) \
	TDRANGE(fullType,TDF_UnsignedInteger) | TDRANGE(hasBitSize,1) | TDRANGE(basicTypeSize,TDBasicTypeSizeOf(base)) | TDRANGE(numberOfBits,bits) | TDRANGE(bitOffset,offset)
#define  UnsignedBitSet(base,bits,offset) TypeDescriptor(UnsignedBitSet_number(base,bits,offset))

#define  SignedBitSet_number(base,bits,offset) \
	TDRANGE(fullType,TDF_SignedInteger)   | TDRANGE(hasBitSize,1) | TDRANGE(basicTypeSize,TDBasicTypeSizeOf(base))| TDRANGE(numberOfBits,bits) | TDRANGE(bitOffset,offset)
#define  SignedBitSet(base,bits,offset) TypeDescriptor(SignedBitSet_number(base,bits,offset))

#define  BitSetBoolean(base,offset) UnsignedBitSet(base,1,offset)

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void TypeDescriptor::SetDataConstant(bool dataIsConstant){
	this->dataIsConstant = dataIsConstant;
}

bool TypeDescriptor::DataIsConstant() const {
    return dataIsConstant;
}

TYPE_DESCRIPTOR_TYPE TypeDescriptor::GetFullCode() const{
	return all;
}

TD_FullType TypeDescriptor::GetFullTypeCode() const{
	TD_FullType ret = fullType;
	if (isStructuredData){
		ret = TDF_Structure;
	}
	return ret;
}


uint32 TypeDescriptor::GetStructuredDataIdCode() const{
	uint32 ret = this->structuredDataIdCode;
	if (!IsStructuredData()){
		ret = 0xFFFFFFFF;  // invalid as will never be in the database
	}
	return ret;
}

int32 TypeDescriptor::GetNumericBitSize() const{
	int32 ret = -1;
	if (IsBitType()) {
		ret = numberOfBits;
	}
	return ret;
}

int32 TypeDescriptor::GetNumericBitOffset() const{
	int32 ret = -1;
	if (IsBitType()) {
		ret = this->bitOffset;
	}
	return ret;
}


bool TypeDescriptor::IsStructuredData() const {
    return isStructuredData;
}

bool TypeDescriptor::IsBasicType() const {
    return ( !isStructuredData && ((fullType & TDF_Category) == TDF_BasicType) );
};

bool TypeDescriptor::IsNumericType() const {
    return ( !isStructuredData && ((fullType == TDF_Float) || (fullType == TDF_SignedInteger) || (fullType == TDF_UnsignedInteger)) );
}

bool TypeDescriptor::IsBitType() const {
    return hasBitSize;
};

bool TypeDescriptor::IsCharStreamType() const {
    return ( !isStructuredData && ((fullType & TDF_Category) == TDF_CharStreamType) );
};

bool TypeDescriptor::IsFormattedCharStreamType() const {
    return ( !isStructuredData && ((fullType & TDF_Category) == TDF_CharStreamType) && ((fullType & TDF_HasFormatInfo)!=0));
};

bool TypeDescriptor::IsCharString() const {
	uint32 ft = fullType & (~TDF_HasFormatInfo);
	return (!isStructuredData && ((ft == TDF_CCString) || (ft == TDF_CString) || (ft == TDF_DynamicCString)))   ;
}

bool TypeDescriptor::IsStreamString() const {
	// mask out TDF_HasFormatInfo
	return (!isStructuredData && ((fullType == TDF_SString) || (fullType == (TDF_SString | TDF_HasFormatInfo))));
}

bool TypeDescriptor::IsSpecialType() const {
    return ( !isStructuredData && ((fullType & TDF_Category) == TDF_SpecialType) );
};

bool TypeDescriptor::IsSigned() const {
	return !isStructuredData && ((fullType == TDF_SignedInteger)||(fullType == TDF_Float));
};

bool TypeDescriptor::IsInvalid() const {
	return !isStructuredData && (fullType == TDF_Invalid);
};

TypeDescriptor::TypeDescriptor(const uint32 x) {
    all = x;
}

TypeDescriptor::TypeDescriptor(const bool isConstantIn,const uint32 &structuredDataIdCodeIn){
	this->structuredDataIdCode = structuredDataIdCodeIn;
	this->dataIsConstant = isConstantIn;
	this->isStructuredData = true;
}


bool TypeDescriptor::operator==(const TypeDescriptor &typeDescriptor) const {
    return all == typeDescriptor.all;
}

bool TypeDescriptor::operator!=(const TypeDescriptor &typeDescriptor) const {
    return all != typeDescriptor.all;
}

/**
 * @brief matches all details
 */
bool TypeDescriptor::SameAs(const TypeDescriptor &td) const {
    return all == td.all;
}

/**
 * @brief matches all details but the constantness
 */
bool TypeDescriptor::SameTypeAndSizeAs(const TypeDescriptor &td) const {
	uint32 mask = TDRANGE(dataIsConstant,1);
	mask = ~mask;
    return (all & mask) == (td.all & mask);
}

/**
 * @brief matches only the type. It is the same structure or the same fullType
 */
bool TypeDescriptor::SameTypeAs(const TypeDescriptor &td) const {
	bool ret=false;
	if (isStructuredData){
		ret = (all == td.all);
	} else {
		uint32 ft = fullType;
		uint32 ft2= td.fullType;
		ret = (ft == ft2);
	}
	return ret;
}

template <> const TypeDescriptor Type2TypeDescriptor<int8>(){
	return SignedInteger8Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<int16>(){
	return SignedInteger16Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<int32>(){
	return SignedInteger32Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<int64>(){
	return SignedInteger64Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<uint8>(){
	return UnsignedInteger8Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<uint16>(){
	return UnsignedInteger16Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<uint32>(){
	return UnsignedInteger32Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<uint64>(){
	return UnsignedInteger64Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<float32>(){
	return Float32Bit;
}
template <> const TypeDescriptor Type2TypeDescriptor<float64>(){
	return Float64Bit;
}


}

#endif /* TYPEDESCRIPTOR_H_ */
