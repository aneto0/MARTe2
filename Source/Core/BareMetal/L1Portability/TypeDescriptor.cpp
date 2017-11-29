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
				size = crb->sizeOfClass;
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
	if (IsCharStreamType() && (address != NULL)){
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
   	string.AppendN(constString);				\
	if ((fullType == TDF_Float) && (bits == 32)){\
		string.AppendN("float");				\
	} else 										\
	if ((fullType == TDF_Float) && (bits == 64)){\
		string.AppendN("double");				\
	} else {									\
       	string.AppendN(typeName);   			\
       	string.AppendNum(bits);					\
	}

#define TEMPLATED_TYPENAME_CORE(className)       \
       	string.AppendN(#className "<");          \
       	string.AppendN(constString);			 \
       	string.AppendN(typeName);   			 \
       	string.AppendNum(bits);                  \
       	string.Append('>');


bool TypeDescriptor::ToString(DynamicCString &string) const{
	bool ret= true;

    const char8 *constString2= "const ";
    const char8 *constString= "";
    if (dataIsConstant){
    	constString = constString2;
    }

    if (isStructuredData){
    	string.AppendN(constString);
    	CCString className = GetNameOfClassFromStructureId();

    	if (className.GetSize()==0){
        	string.AppendN("unknown_struct_code(");
        	uint32 idCode = structuredDataIdCode;
        	string.AppendNum(idCode);
        	string.Append(')');
    	} else {
           	string.AppendN(className);
    	}
    }
    else // !isStructuredData
   	if (!IsBasicType()){  //Stream,StructuredData,..
   		string.AppendN(constString);
   		string.AppendN(BasicTypeName(fullType));
   	}
   	else { //!IsComplexType()
   		CCString typeName = BasicTypeName(fullType);
   		if (IsBitType()){  // uint5 bitranges
   			string.AppendN(constString);
   			string.AppendN("BitRange<");
			uint32 bits = 8*SizeFromTDBasicTypeSize(basicTypeSize);
   			TYPENAME_CORE()
   			string.Append(',');
   			uint32 numberOfBitsR = (int)numberOfBits;
   			uint32 bitOffsetR = (int)bitOffset;
   			string.AppendNum(numberOfBitsR);
   			string.Append(',');
   			string.AppendNum(bitOffsetR);
   			string.Append('>');
   		}
   		else {// not bit type
			uint32 bits = 8*SizeFromTDBasicTypeSize(basicTypeSize);
   			TYPENAME_CORE()
   		}
   	}
	return ret;
}


}
