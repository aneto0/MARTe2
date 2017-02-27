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
/*
static char8 tolower(char8 c){
    if ((c >='A') && (c <='Z')) {
        c = c + ('a'-'A');
    }
    return c;
}
*/
#if 0
uint32 FromHex(char8 c){
	uint32 ret = 0u;
	if ((c >='0') && (c <='9')){
		ret  = c - '0';
	} else
		if ((c >='A') && (c <='F')){
			ret  = c - 'A' + 10u;
		}
	return ret;
}

static uint32 ReadHex(char8 *&buffer,uint32 nOfChars){
	uint32 i=0u;

	uint32 ret = 0u;
	for (i=0u;i<nOfChars;i++){
		uint32 n = FromHex(*buffer++);
		ret+= n << (4u*i);
	}
	return ret;
}



/**
 * returns tokens A=array P=pointer V=Vector M=Matrix
 * returns size as size of Arrays
 * buffer is input and is modified
 */
static void BrowseModifierBuffer(char8 &token, bool &constant, uint32 &size,char8  *&buffer){
    token = '\0';
    size = 0u;
    constant = false;
    if (buffer != NULL){
        char8 c = *buffer++;
        constant = (c >= 'a');
        c = toupper(c);
        token = c ;
        switch (c){
        case '\0':{
        	buffer--;
        	token = '\0';
        	size = 0u;
        } break;
        case 'A':{
            token = 'A' ;
            size = ReadHex(buffer,2);
        } break;
        case 'B':{
            token = 'A' ;
            size = ReadHex(buffer,4);
        } break;
        case 'C':{
            token = 'A' ;
            size = ReadHex(buffer,8);

        } break;
        }
    }
}

void VariableDescriptor::GetModifier(char8 &token, bool &constant, uint32 &size,uint32 depth) const {
    char8 *buffer = modifiers.GetList();
    depth++;
    do {
        BrowseModifierBuffer(token, constant, size,buffer);
        depth--;
    } while ((token != '\0') && (depth > 0u));
}


void VariableDescriptor::AddGenericToModifiers(char8 token,bool &constant){
    if (constant){
        constant = false;
        modifiers.Append(tolower(token));
    } else {
        modifiers.Append(token);
    }
}

static char8 ToHex(uint32 nibble){
	char8 ret = '?';
	if (nibble < 10u){
		ret = '0'+nibble;
	} else
		if (nibble < 16u){
			ret = 'A' + nibble - 10u;
		}
	return ret;
}

static void AppendHex(DynamicCString &s, uint32 n, uint32 size){
	uint32 i;
	for (i=0u;i<size;i++){
		uint32 nibble = n & 0x0000000F;
		n >>=4;
		s.Append(ToHex(nibble));
	}
}

void VariableDescriptor::AddArrayToModifiers(uint32 vectorSize,bool &constant){
    char8 c = '\0';
    if (constant){
        constant = false;
        c = ('a' - 'A');
    }
    if (vectorSize < 256u) {
        c += 'A';
        modifiers.Append(c);
        AppendHex(modifiers,vectorSize,2);
    } else
    if (vectorSize < 65536u) {
        c += 'B';
        modifiers.Append(c);
        AppendHex(modifiers,vectorSize,4);
    } else
    {
        c += 'C';
        modifiers.Append(c);
        AppendHex(modifiers,vectorSize,8);
    }
}

void StoreCode(const ModifierCode &code){
	switch (code.)

}


void VariableDescriptor::QueueCode(ModifierCode &code){
	if (code.HasCode()) {
		StoreCode(mf);
		code.Clear();
	}
}
#endif

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

struct APLookUp{ CombinedArrayType arrayCode; char8 keyCode;};

APLookUp APLookupTable1[] = {
		{SizedCArray_AP,'A'},
		{StaticZeroTermArray_AP,'S'},
		{ConstStaticZeroTermArray_AP,'s'},
		{UnSizedA_AP,'\0'}

};

APLookUp APLookupTable2[] = {
		{Array1D,'V'},
		{ConstArray1D,'v'},
		{Array2D,'M'},
		{ConstArray2D,'m'},
		{PointerArray,'P'},
		{ConstPointerArray,'p'},
		{ZeroTermArray,'Z'},
		{ConstZeroTermArray,'z'},
		{ConstDynamicZeroTermArray,'d'},
		{DynamicZeroTermArray,'D'},
		{ArrayUnknown,'\0'}
};

static char8 lookUpArrayType(CombinedArrayType arrayCode){
	APLookUp *apl = &APLookupTable1[0];
	char8 keyCode = '\0';
	while ((apl->keyCode != '\0') && (keyCode == '\0')){
		if (apl->arrayCode == arrayCode){
			keyCode = apl->keyCode;
		}
		apl++;
	}
	return keyCode;
}

static char8 lookUpCompositeArrayCode(uint32 arrayCode){
	APLookUp *apl = &APLookupTable2[0];
	char8 keyCode = '\0';
	while ((apl->keyCode != '\0') && (keyCode == '\0')){
		if (apl->arrayCode == arrayCode){
			keyCode = apl->keyCode;
		}
		apl++;
	}
	return keyCode;
}


void VariableDescriptor::MoveCodeToModifiers(){
//	int i1 = typeDescriptor.arrayProperty;
//	int i2 = typeDescriptor.combinedArrayType;
	// do not save scalars
	if ((typeDescriptor.arrayProperty == SizedCArray_AP) && (typeDescriptor.arraySize == 1)){
		return;
	}
//	printf("MCM %i %i \n",i1,i2);

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

void VariableDescriptor::FinaliseCode(TypeDescriptor td){
	bool useFullSpace = false;
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
	// make space
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
/*
static bool isToken(char8 c){
	return (((c >='A') && (c <='Z')) || ((c >='a') && (c <='z')));
}
*/
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

void VariableDescriptor::GetModifier(char8 &token, bool &constant, uint32 &size,uint32 depth) const {
    char8 *buffer = modifiers.GetList();

    depth++;
    do {
    	token = *buffer;
    	constant = isConstantToken(token);
    	token = toupper(token);
    	buffer++;
    	size = readNumber(buffer);

    	depth--;
    } while ((token != '\0') && (depth > 0u));
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

const TypeDescriptor &VariableDescriptor::GetFullTypeDescriptor() const {
    return typeDescriptor;
}

const TypeDescriptor &VariableDescriptor::GetTopTypeDescriptor() const {

     return VoidType;
}


}


	
