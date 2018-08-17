/**
 * @file TypeDescriptor.cpp
 * @brief Source file for class TypeDescriptor
 * @date 20/01/2016
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

 * @details This source file contains the definition of all the methods for
 * the class TypeDescriptor (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <TypeDescriptor.h>
#include "StringHelper.h"
#include "Vector.h"
#include "Matrix.h"
#include "ClassRegistryIndex.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TypeDescriptor::TypeDescriptor() {
    all = 0;
}

uint32 TypeDescriptor::StorageSize() const{
	uint32 size = 0;
	if (isStructuredData){
		size = 0;
		ClassRegistryIndex *cri = ClassRegistryIndex::Instance();
		if (cri != NULL_PTR(ClassRegistryIndex *)){
			ClassRegistryBrief *crb = (*cri)[structuredDataIdCode];
			if (crb != NULL_PTR(ClassRegistryBrief *)){
				size = crb->GetSizeOfClass();
			}
		}
	} else {
	    if (IsBasicType()){
			size = SizeFromTDBasicTypeSize(basicTypeSize);
	    } else {
			size = objectSize;
	    }
	}
	return size;

}

uint32 TypeDescriptor::FullSize(const uint8 *address)const{
	uint32 size = StorageSize();
	if (IsCharStreamType()) {
		if (address != NULL){
			TD_FullType fullType = this->fullType;
			switch(fullType){
			case TDF_Stream:
				/** only works as StreamString is sole descendant of StreamI */
			case TDF_SString:{
				StreamI *s = reinterpret_cast<StreamI *>(const_cast<uint8 *>(address));
				size += s->Size();
			}break;
			case TDF_CString:
			case TDF_CCString:
			case TDF_DynamicCString:{
				CCString *s = reinterpret_cast<CCString *>(const_cast<uint8 *>(address));
				size += (s->GetSize()+1);
			} break;
			default:{

			}
			}
		}
	}
	return size;
}


/**
 * initialised to NULL
 */
static CCString (*IdToString)(TypeDescriptor) = NULL;

/**
 * @brief gets name of class from structuredDataIdCode
 */
bool TypeDescriptor::SetNameOfClassFromIdFunction(CCString (*fun)(TypeDescriptor)){
	IdToString = fun;
	return true;
}

const struct {
	CCString 	name;
	TD_FullType tdf;
} nameMap[] = {
		{"int"            ,  TDF_SignedInteger  },
		{"uint"           ,  TDF_UnsignedInteger},
		{"float" 		  ,  TDF_Float          },
		{"char" 		  ,  TDF_Char           },
		{"void"		      ,  TDF_Void           },
        {"StreamString"	  ,  TDF_SString        },
		{"StreamI"		  ,  TDF_Stream         },
		{"StructuredDataI",  TDF_StructuredDataI},
		{"Object"		  ,  TDF_Object         },
		{"CCString"       ,  TDF_CCString       },
		{"CString"        ,  TDF_CString        },
		{"DynamicCString" ,  TDF_DynamicCString },
		{"void*" 	      ,  TDF_Pointer        },
		{"void[]" 	      ,  TDF_GenericArray   },
		{"invalid"	 	  ,  TDF_Invalid        },
		{"?*"             ,  TDF_GenericPointer },
		{""               ,  0 },
		};

static CCString BasicTypeName(TD_FullType tdf){
	uint32 ix=0u;
	CCString name = "unknownT";
	bool toContinue = true;
	while ((nameMap[ix].name.GetSize() > 0) && toContinue){
		if (nameMap[ix].tdf == tdf){
			name = nameMap[ix].name;
			toContinue = false;
		}
		ix++;
	}
	return name;
}

/**
 * @brief gets name of class from structuredDataIdCode
 */
CCString TypeDescriptor::GetNameOfClassFromStructureId() const{
	CCString ret = "?";
	if (IdToString != NULL){
		ret = IdToString(*this);
	}
	return ret;
}

#define TYPENAME_CORE()             			\
   	string.Append(constString);				\
	if ((fullType == TDF_Float) && (bits == 32)){\
		string.Append("float");				\
	} else 										\
	if ((fullType == TDF_Float) && (bits == 64)){\
		string.Append("double");				\
	} else {									\
       	string.Append(typeName);   			\
       	string.Append(bits);					\
	}

#define TEMPLATED_TYPENAME_CORE(className)       \
       	string.Append(#className "<");          \
       	string.Append(constString);			 \
       	string.Append(typeName);   			 \
       	string.Append(bits);                  \
       	string.Append('>');


bool TypeDescriptor::ToString(DynamicCString &string) const{
	bool ret= true;

    const char8 *constString2= "const ";
    const char8 *constString= "";
    if (dataIsConstant){
    	constString = constString2;
    }

    if (isStructuredData){
    	string.Append(constString);
    	CCString className = GetNameOfClassFromStructureId();

    	if (className.GetSize()==0){
        	string.Append("unknown_struct_code(");
        	uint32 idCode = structuredDataIdCode;
        	string.Append(idCode);
        	string.Append(')');
    	} else {
           	string.Append(className);
    	}
    }
    else // !isStructuredData
   	if (!IsBasicType()){  //Stream,StructuredData,..
   		string.Append(constString);
   		string.Append(BasicTypeName(fullType));
   	}
   	else { //!IsComplexType()
   		CCString typeName = BasicTypeName(fullType);
   		if (IsBitType()){  // uint5 bitranges
   			string.Append(constString);
   			string.Append("BitRange<");
			uint32 bits = 8*SizeFromTDBasicTypeSize(basicTypeSize);
   			TYPENAME_CORE()
   			string.Append(',');
   			uint32 numberOfBitsR = (int)numberOfBits;
   			uint32 bitOffsetR = (int)bitOffset;
   			string.Append(numberOfBitsR);
   			string.Append(',');
   			string.Append(bitOffsetR);
   			string.Append('>');
   		}
   		else {// not bit type
			uint32 bits = 8*SizeFromTDBasicTypeSize(basicTypeSize);
   			TYPENAME_CORE()
   		}
   	}
	return ret;
}

static const CCString seps = " \n\r\t";
static const CCString nums = "0123456789";
static bool isSep(char8 c){  return (seps.In(c)); }
static bool isNum(char8 c){  return (nums.In(c)); }

static void GetToken(CCString &string,DynamicCString &token){
	token.Truncate(0);
	while (isSep(string[0])){
		string++;
	}
	while (string[0] != '\0'){
		if (!isSep(string[0])) 	token.Append(string[0]);
		string++;
	}
}
static uint32 ToNumber(char8 *p){
	uint32 number = 0;
	while (isNum(*p)){
		number = number * 10 + (static_cast<uint32>(*p) - static_cast<uint32>('0'));
		p++;
	}
	return number;
}


TypeDescriptor::TypeDescriptor(CCString typeName){
	*this = InvalidType(0);
	DynamicCString token;
	bool constant = false;
	TD_FullType ft;
	char8 *numericPart= NULL;

	GetToken(typeName,token);

	if (token.isSameAs("CString")){
		*this = CharString;
	} else
	if (token.isSameAs("CCString")){
		*this = ConstCharString(sizeof(CCString));
	} else
	if (token.isSameAs("DynamicCString")){
		*this = DynamicCharString;
	} else
	if (token.isSameAs("char")){
		all = Character8Bit_number;
	} else
	if (token.isSameAs("float")){
		all = Float32Bit_number;
	} else
	if (token.isSameAs("double")){
		all = Float64Bit_number;
	} else
	if (token.isSameAs("uint",4)){
		ft = TDF_UnsignedInteger;
		numericPart = token.GetList()+4;
	} else
	if (token.isSameAs("int",3)){
		ft = TDF_SignedInteger;
		numericPart = token.GetList()+4;
	} else
	if (token.isSameAs("BitRange<",3)){
		// TODO add error message
		// TODO complete
	} else {
		*this = ClassRegistryIndex::Instance()->GetTypeDescriptor(token);
	}

	if (numericPart != NULL){
		uint32 num = ToNumber(numericPart);
		if (num > 64){
			// TODO add error message
		} else
		if (num <= 8){
			if (num == 8){
				all = TDRANGE(fullType,ft) | TDRANGE(basicTypeSize,Size8bit);
			} else {
				all = TDRANGE(fullType,ft) | TDRANGE(basicTypeSize,Size8bit) | TDRANGE(hasBitSize,1) | TDRANGE(numberOfBits,num) | TDRANGE(bitOffset,0);
			}
		} else
		if (num <= 16){
			if (num == 16){
				all = TDRANGE(fullType,ft) | TDRANGE(basicTypeSize,Size16bit);
			} else {
				all = TDRANGE(fullType,ft) | TDRANGE(basicTypeSize,Size16bit) | TDRANGE(hasBitSize,1) | TDRANGE(numberOfBits,num) | TDRANGE(bitOffset,0);
			}
		} else
		if (num <= 32){
			if (num == 32){
				all = TDRANGE(fullType,ft) | TDRANGE(basicTypeSize,Size32bit);
			} else {
				all = TDRANGE(fullType,ft) | TDRANGE(basicTypeSize,Size32bit) | TDRANGE(hasBitSize,1) | TDRANGE(numberOfBits,num) | TDRANGE(bitOffset,0);
			}
		} else
		if (num == 64){
			if (num == 64){
				all = TDRANGE(fullType,ft) | TDRANGE(basicTypeSize,Size64bit);
			} else {
				all = TDRANGE(fullType,ft) | TDRANGE(basicTypeSize,Size64bit) | TDRANGE(hasBitSize,1) | TDRANGE(numberOfBits,num) | TDRANGE(bitOffset,0);
			}
		}
	}

}



}
