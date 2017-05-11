/**
 * @file VariableDescriptor.cpp
 * @brief Source file for class VariableDescriptor
 * @date Nov 29, 2016
 * @author fsartori
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
 * the class VariableDescriptor (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "VariableDescriptor.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"
#include "DynamicCString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

static char8 toupper(char8 c){
    if ((c >='a') && (c <='z')) {
        c = c - ('a'-'A');
    }
    return c;
}


// syntax Annn where nn is numeric only
static void AddArrayDataToModifiers(DynamicCString &modifiers, uint32 size){
	char8 buffer[12];
	char8 *pBuffer = &buffer[11];
	*pBuffer-- = '\0';
	while (size > 0){
		uint32 n = size % 10;
		size = size / 10;
		*pBuffer-- = '0'+ n;
	}
	pBuffer++;
	modifiers.AppendN(pBuffer);
}

//
struct APLookUp{
	//
	CombinedArrayType 	arrayCode;
	//
	char8 				keyCode;
	/**
	 * 0 means unknown size
	 */
	uint32 				layerSize;
	// true for simple arrays[] - they multiply the size of the layer below
	// false for pointers or objects with pointers inside - data access is indirect
	bool 				isMultiplyingLayer;
	//
	bool 				sizeFollows;
};

APLookUp APLookupTable1[] = {
		{SizedCArray_AP,'A',0,true,true},
		{StaticZeroTermArray_AP,'S',sizeof(void *),false,true},
		{ConstStaticZeroTermArray_AP,'s',sizeof(void *),false,true},
		{UnSizedA_AP,'\0',0,false,false}
};

APLookUp APLookupTable2[] = {
		{Array1D,'V',sizeof (Vector<char>),false,false},
		{ConstArray1D,'v',sizeof (Vector<char>),false,false},
		{Array2D,'M',sizeof (Matrix<char>),false,false},
		{ConstArray2D,'m',sizeof (Matrix<char>),false,false},
		{PointerArray,'P',sizeof (void *),false,false},
		{ConstPointerArray,'p',sizeof (void *),false,false},
		{ZeroTermArray,'Z',sizeof (void *),false,false},
		{ConstZeroTermArray,'z',sizeof (void *),false,false},
		{ConstDynamicZeroTermArray,'d',sizeof (void *),false,false},
		{DynamicZeroTermArray,'D',sizeof (void *),false,false},
		{ArrayUnknown,'\0',0,false,false}
};

static CombinedArrayType reverseLookUpArrayTypeGen(char8 code, APLookUp *apl){
	CombinedArrayType cat = ArrayUnknown;
	while ((apl->keyCode != '\0') && (cat == ArrayUnknown)){
//		printf("{%c}",apl->keyCode);
		if (apl->keyCode == code){
			cat = apl->arrayCode;
		}
		apl++;
	}
	return cat;
}

/**
 * returns false if not found.
 * looks in both tables
 */
static bool reverseLookUpArrayProperties(char8 code, APLookUp *&apl){
	apl = &APLookupTable1[0];

	while ((apl->keyCode != '\0') && (apl->keyCode != code)){
		apl++;
	}

	if (apl->keyCode != code){
		apl = &APLookupTable2[0];

		while ((apl->keyCode != '\0') && (apl->keyCode != code)){
			apl++;
		}
	}

	if (apl->keyCode != code) apl = NULL;

		return (apl != NULL);
}

static char8 lookUpArrayTypeGen(CombinedArrayType arrayCode, APLookUp *apl){
	char8 keyCode = '\0';
	while ((apl->keyCode != '\0') && (keyCode == '\0')){
		if (apl->arrayCode == arrayCode){
			keyCode = apl->keyCode;
		}
		apl++;
	}
	return keyCode;
}

inline char8 lookUpArrayType(CombinedArrayType arrayCode){
	return lookUpArrayTypeGen(arrayCode,&APLookupTable1[0]);
}

static char8 lookUpCompositeArrayCode(CombinedArrayType arrayCode){
	return lookUpArrayTypeGen(arrayCode,&APLookupTable2[0]);
}

void VariableDescriptor::MoveCodeToModifiers(){
	// do not save scalars
	if ((typeDescriptor.arrayProperty == SizedCArray_AP) && (typeDescriptor.arraySize == 1)){
		return;
	}

	char8 keyCode = lookUpArrayType(typeDescriptor.arrayProperty);

	if (keyCode != '\0'){
		modifiers.Append(keyCode);
		AddArrayDataToModifiers(modifiers,typeDescriptor.arraySize);
	} else {
		keyCode = lookUpCompositeArrayCode(typeDescriptor.combinedArrayType);
		if (keyCode != '\0'){
			modifiers.Append(keyCode);
		}
	}
	// renormalise to scalar
	typeDescriptor.arrayProperty = SizedCArray_AP;
	typeDescriptor.arraySize = 1;
}

void VariableDescriptor::AddArrayCode(CombinedArrayType cat, uint32 size){
//printf("AAC %i %i \n",cat,size);

	uint32 max = 1 << typeDescriptor.arraySize.GetNumberOfBits();
	bool isConst = false;
	switch(cat){
	// new layer of vector/matrix
	// just shift out existing layer without moving const
	case SizedCArray:{
		// if this was a Pointer/ZTA to xxx now drop the Pointer/ZTA and replace with the vector
		MoveCodeToModifiers();
		typeDescriptor.combinedArrayType = cat;
		if (size >= max) size = 0;
		typeDescriptor.arraySize = size;
	}break;
	case ConstStaticZeroTermArray:
	case StaticZeroTermArray:{
		// drop any existing modifier
		MoveCodeToModifiers();
		// if it was a const ? type now it will be a const ZTA<?>
		if (typeDescriptor.dataIsConstant){
			cat = ConstStaticZeroTermArray;
			typeDescriptor.dataIsConstant = false;
		}
		typeDescriptor.combinedArrayType = cat;
		if (size >= max) size = 0;
		typeDescriptor.arraySize = size;
	}break;
	default:{
		MoveCodeToModifiers();
		if (typeDescriptor.dataIsConstant){
			cat |= ConstArrayMask;
			typeDescriptor.dataIsConstant = false;
		}
		typeDescriptor.combinedArrayType = cat;
	}
	}
}

void VariableDescriptor::AddConstantCode(){
	//printf("ACC\n");
	typeDescriptor.dataIsConstant = true;
}

#if COMPACT_TYPEDESCRIPTOR
void VariableDescriptor::FinaliseCode(TypeDescriptor td){

	bool useFullSpace = false;

	// identify wether the final type allows for embedding array modifiers
	if (td.isStructuredData){
		useFullSpace = true;
	} else {
		if (td.objectSize == SizeBits){
			useFullSpace = true;
		} else {
			if (td.arrayProperty == SizedCArray){
				useFullSpace = (td.arraySize != 1);
			} else {
				useFullSpace = true;
			}
		}
	}
	if (useFullSpace) {
		MoveCodeToModifiers();
		bool isConst = typeDescriptor.dataIsConstant;
		typeDescriptor = td;
		if (isConst) typeDescriptor.dataIsConstant = true;
	} else { // merge
		typeDescriptor.isStructuredData  = false;
		if (td.dataIsConstant)	typeDescriptor.dataIsConstant = true;
		uint32 temp = td.type;
		typeDescriptor.type = temp;
		temp = td.objectSize;
		typeDescriptor.objectSize = temp;

	}
}

#else

void VariableDescriptor::FinaliseCode(TypeDescriptor td){
	MoveCodeToModifiers();
	bool isConst = typeDescriptor.dataIsConstant;
	typeDescriptor = td;
	if (isConst) typeDescriptor.dataIsConstant = true;
}


#endif

static bool isConstantToken(char8 c){
	return ((c >='a') && (c <='z'));
}

static bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}

static uint32 toNumber(char8 c){
	return  static_cast<uint32>(c - '0') ;
}

static uint32 readNumber(char8 *&buffer){
	uint32 result = 0;
	while (isNumber(*buffer)){
		result = result * 10u;
		result += toNumber(*buffer);
		buffer++;
	}
	return result;
}

VariableDescriptor::VariableDescriptor(){
    typeDescriptor = VoidType;
}

VariableDescriptor::~VariableDescriptor(){
}

VariableDescriptor::VariableDescriptor(const TypeDescriptor &td){
    typeDescriptor = td;
}

VariableDescriptor::VariableDescriptor( VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.AppendN(x.modifiers.GetList());
}

VariableDescriptor::VariableDescriptor( const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.AppendN(x.modifiers.GetList());
}

VariableDescriptor &VariableDescriptor::operator=(const VariableDescriptor &x ){
    typeDescriptor = x.typeDescriptor;
    modifiers.Truncate(0);
    modifiers.AppendN(x.modifiers.GetList());
    return *this;
}



bool VariableDescriptor::GetTopTypeDescriptor(TypeDescriptor &td, uint32 depth) const {
    char8 *buffer = modifiers.GetList();
    char8 token;
    uint32 size;
    depth++;
    do {
    	token = *buffer;
    	if (token != '\0'){
    		buffer++;
    		size = readNumber(buffer);
    	}
    	depth--;
    } while ((token != '\0') && (depth > 0u));

    td = DelegatedType;

    if (token != '\0'){
    	//printf("[%c]",token);
   		CombinedArrayType cat = reverseLookUpArrayTypeGen(token, &APLookupTable1[0]);
   		if (cat != ArrayUnknown){
   			td.arraySize = size;
   			td.arrayProperty = cat;
		} else {
			cat = reverseLookUpArrayTypeGen(token, &APLookupTable2[0]);
   			td.combinedArrayType = cat;
		}
    } else {
    	if (depth == 0){
    		td = GetFullTypeDescriptor();
    		token = ' '; // force true
    	}
    	//return  GetFullTypeDescriptor();
    }
	return (token != '\0');
}

bool VariableDescriptor::GetModifiersLayer(char8 &modifier,uint64 &size,bool remove){
	bool ret = false;

    char8 *buffer  = modifiers.GetList();
    char8 *pStart = buffer;
    char8 token = *buffer;
    if (token != '\0'){
    	modifier = *buffer;
    	buffer++;
    	size = readNumber(buffer);
    	ret = true;
    }
    if (ret && remove){
    	uint32 step = (uint32)(buffer - pStart);
    	ret = modifiers.Remove(step);
    }

    return ret;
}

bool VariableDescriptor::InsertModifiersLayer(char8 modifier,uint64 size){
	DynamicCString save;

	bool ret;

	ret = save.AppendN(modifiers.GetList());

	if (ret){
		ret = modifiers.Truncate(0);
	}

	if (ret){
		ret = modifiers.Append(modifier);
		AddArrayDataToModifiers(modifiers,size);
	}
	if (ret){
		ret = modifiers.AppendN(save.GetList());
	}
	return ret;
}



uint64 VariableDescriptor::GetSize() const{
	uint64 size = 1;

    char8 *buffer = modifiers.GetList();
    uint8 code = *buffer;
	APLookUp *apl;
	bool toContinue = true;
	while ((code != '\0') && (toContinue)){
		buffer++;
    	uint32 layerSize = readNumber(buffer);
    	toContinue = reverseLookUpArrayProperties(code, apl);
    	if (toContinue){
    		if (apl->layerSize != 0) size *= apl->layerSize;
    		else {
    			if ((apl->sizeFollows) && (layerSize > 0)) {
    				size *= layerSize;
    			}
    		}
    		toContinue = apl->isMultiplyingLayer;
    	}

    	code = *buffer;
    }

	if (toContinue){
		size *= this->typeDescriptor.Size();
	}

	return size;
}




}


	
