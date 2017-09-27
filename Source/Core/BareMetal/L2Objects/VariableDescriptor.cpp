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

/**
 *
 */
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
	//
	CCString			cExpansionPre;
	//
	CCString			cExpansionPost;
	//
	CCString			cExpansionEnd;
};

const CCString nullString;

const APLookUp APLookupTable1[] = {
		{SizedCArray_AP,				'A',	0,						true,true,		"",	"[", "]"},
		{StaticZeroTermArray_AP,		'S',	sizeof(void *),			false,true,     "StaticZeroTerminatedArray<",",",">"},
		{ConstStaticZeroTermArray_AP,	's',	sizeof(void *),			false,true,		"const StaticZeroTerminatedArray<",",",">"},
		{UnSizedA_AP,					'\0',	0,						false,false,    "",nullString,""}
};

const APLookUp APLookupTable2[] = {
		{Array1D,						'V',	sizeof (Vector<char>)	,false,false,	"Vector<",	nullString, ">"},
		{ConstArray1D,					'v',	sizeof (Vector<char>)	,false,false,	"const Vector<",	nullString, ">"},
		{Array2D,						'M',	sizeof (Matrix<char>)	,false,false,	"Matrix<",	nullString, ">"},
		{ConstArray2D,					'm',	sizeof (Matrix<char>)	,false,false,	"const Matrix<",	nullString, ">"},
		{PointerArray,					'P',	sizeof (void *)			,false,false,	"",	nullString, " *"},
		{ConstPointerArray,				'p',	sizeof (void *)			,false,false,	"",	nullString, " * const"},
		{ZeroTermArray,					'Z',	sizeof (void *)			,false,false,	"ZeroTerminatedArray<",	nullString, ">"},
		{ConstZeroTermArray,			'z',	sizeof (void *)			,false,false,	"const ZeroTerminatedArray<",	nullString, ">"},
		{ConstDynamicZeroTermArray,		'd',	sizeof (void *)			,false,false,	"DynamicZeroTerminatedArray<",	nullString, ">"},
		{DynamicZeroTermArray,			'D',	sizeof (void *)			,false,false,	"const DynamicZeroTerminatedArray<",	nullString, ">"},
		{ArrayUnknown,					'\0',	0						,false,false,    "",nullString,""}
};

static const APLookUp *reverseLookUpCode(char8 code){
	const APLookUp *apl = APLookupTable1;
	const APLookUp *ret = NULL;
	while ((apl->keyCode != '\0') && (ret==NULL)) {
		if (apl->keyCode == code){
			ret = apl;
		}
		apl++;
	}
	apl = APLookupTable2;
	while ((apl->keyCode != '\0') && (ret==NULL)) {
		if (apl->keyCode == code){
			ret = apl;
		}
		apl++;
	}

	return ret;
}

static CombinedArrayType reverseLookUpArrayTypeGen(char8 code, const APLookUp *apl){
	CombinedArrayType cat = ArrayUnknown;
	while ((apl->keyCode != '\0') && (cat == ArrayUnknown)){
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
static bool reverseLookUpArrayProperties(char8 code, const APLookUp *&apl){
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

static char8 lookUpArrayTypeGen(CombinedArrayType arrayCode, const APLookUp *apl){
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
//	bool isConst = false;
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



static bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}

static uint32 toNumber(char8 c){
	return  static_cast<uint32>(c - '0') ;
}

static uint32 readNumber(CCString &buffer){
	uint32 result = 0;
	while (isNumber(buffer[0])){
		result = result * 10u;
		result += toNumber(buffer[0]);
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
    CCString buffer = modifiers.GetList();
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

    CCString buffer  = modifiers.GetList();
    char8 token = *buffer[0];
    if (token != '\0'){
    	modifier = buffer[0];
    	buffer++;
    	size = readNumber(buffer);
    	ret = true;
    }
    if (ret && remove){
    	uint32 step = (uint32)(buffer.GetList() - modifiers.GetList());
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

/**
 * extracts information about a layer
 * updates the layer string pointer
 */
static inline bool GetLayerInfo(CCString &modifierString,char8 &c,uint32 &n ){
	bool ret = true;
	c = modifierString[0];
	if (c == '/0'){
		ret = false;
	} else {
		modifierString++;
		n = readNumber(modifierString);
	}
	return ret;
}
/**
 * Returns false when the end of the layers is reached
 * Arrays are used to multiply the size of what comes next
 */
 bool VariableDescriptor::LayerSize(CCString modifierString,uint64 &size){
	char8 c;
	uint32 n;
	size = 1;
	bool ret = true;
	if (GetLayerInfo(modifierString,c,n)){
		ret = true;
		if (c=='A') {
			ret = LayerSize(modifierString, size);
			size = size * n;
		} else {
			if ((c=='v') || (c=='V')){
				size = sizeof(Vector<int>);
			} else
			if ((c=='m') || (c=='M')){
				size = sizeof(Matrix<int>);
			} else {
				// pPzZdDsS
				size = sizeof(void *);
			}
		}
	} else {
		ret = false;
		size = typeDescriptor.Size();
	}
	return ret;
}

uint64 VariableDescriptor::GetSize() const{

	uint64 size = 1;
	LayerSize(modifiers,size);

	return size;
/*
    char8 *buffer = modifiers.GetList();
    uint8 code = *buffer;
	const APLookUp *apl;
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
	*/
}


/**
 * true if the object has a size that depends on the content. (strings)
 */
bool isVariableSize(CCString modifierString){
	const CCString variableSizeModifiers = "VvMmZzDdSs";
	bool isVariable = false;

	if (!modifierString.IsNullPtr()){
		char8 c = modifierString[0];
		while (!isVariable && (c != '\0')){
			isVariable =  (variableSizeModifiers.Find(c) != 0xFFFFFFFFu);
			modifierString++;
		}
	}
	return isVariable;
}

static uint64 GetDeepSizeR(CCString modifierString, const uint8 *pointer,uint64 tdSize) {
	const APLookUp *apl;
	// size for data
	uint64 size = 0;
	// size for data overhead // 0 terminator, pointers etc
	uint64 storageSize = 0;
	uint64 nOfElements = 0;
	uint64 nextLayerSize = 0; // size of next layer /stops at pointers


	char8 code  = '/0';
	uint32 number;
	GetLayerInfo(modifierString,code,number);
	LayerSize(modifierString,nextLayerSize);

	if (code == '/0') {
		size = tdSize;
	} else
	if ((code == 'P') || (code == 'p')){
//		size = sizeof(void *);
		// allows browsing past pointers assuming one element only
		storageSize = sizeof(void *);
		nOfElements = 1;
		uint8 **pp = static_cast<uint8 **>(pointer);
		pointer = *pp;
	} else
	if ((code == 'V') || (code == 'v')){
		storageSize = sizeof (Vector<char8>);
		const Vector<char8> *pv = static_cast<const Vector<char8> *>(pointer);
		// TOD better check validity
		pointer = static_cast<uint8 *>(pv->GetDataPointer());
		nOfElements = pv->GetNumberOfElements();
	} else
	if ((code == 'M') || (code == 'm')){
		storageSize = sizeof (Matrix<char8>);
		const Matrix<char8> *pm = static_cast<const Matrix<char8> *>(pointer);
		// TOD better check validity
		pointer = static_cast<uint8 *>(pm->GetDataPointer());
		nOfElements = pm->GetNumberOfColumns()*pm->GetNumberOfRows();
	} else
	if ((code == 'D') || (code == 'd') ||
	    (code == 'S') || (code == 's') ||
	    (code == 'Z') || (code == 'z')){
		storageSize = sizeof (ZeroTerminatedArray<char8>);
		nOfElements = ZeroTerminatedArrayGetSize(pointer,nextLayerSize);
	} else
	if (code == 'A') {
		nOfElements = 1;
		while (code == 'A') {
			modifierString++;
	    	uint32 arrayLayerSize = readNumber(modifierString);
	    	nOfElements = nOfElements * arrayLayerSize;
	    }
	}

	if (nOfElements > 0){
		if (isVariableSize(modifierString)){
			size = 0;
			uint8 *ptr = pointer;
			for (uint64 i = 0;i<nOfElements;i++){
				size +=  GetDeepSizeR(modifierString, ptr,tdSize) ;
				ptr+= nextLayerSize;
			}
		} else {
			size = nOfElements * GetDeepSizeR(modifierString, pointer,tdSize) ;
		}
	}


	return size;


}
uint64 VariableDescriptor::GetDeepSize(void *pointer) const{

	return GetDeepSizeR(modifiers.GetList(),pointer,this->typeDescriptor.Size());
}




static bool ReadLayer(char8 &modifier,uint64 &size,char8 *&buffer){
	bool ret = false;
	char8 token = *buffer;
    if (token != '\0'){
    	modifier = *buffer;
    	buffer++;
    	size = readNumber(buffer);
    	ret = true;
    }
    return ret;
}

bool VariableDescriptor::BrowseModifiersLayer(char8 &modifier,uint64 &size,uint32 layerNo)const{
	bool ret = true;
    char8 *buffer  = modifiers.GetList();
    char8 token ;

    layerNo++;
	while(ret && (layerNo > 0)){
		ret = ReadLayer(modifier,size,buffer);
		layerNo--;
	}
    return ret;
}


static bool BrowseModifiersLayerR( char8 &modifier,uint64 &size,char *buffer,int32 targetLayer,int32 &reverseDepth) {
	bool ret = true;
	bool isArray = false;
	ret = ReadLayer(modifier,size,buffer);
	if (((modifier == 'A') || (modifier == 'a')) && (ret)){
		isArray = true;
		// remember the target at the beginning of a sequence of A/a
		if (reverseDepth < 0){
			reverseDepth = targetLayer;
		}
	} else {
		// end of sequence of array and not yet found target
		if (targetLayer >= 0){
			reverseDepth = -1;
		}
	}

	if ((isArray) || (targetLayer > 0)){
		char8 modifier2;
		uint64 size2;
		targetLayer--;
		bool ret2 = BrowseModifiersLayerR( modifier2,size2,buffer,targetLayer,reverseDepth);

		// skip results until reverse layers consumed
		if (reverseDepth >= 0){
			reverseDepth--;
		} else {
			modifier 	= modifier2;
			size 		= size2;
			ret 		= ret2;
			reverseDepth = -1;
		}
	}

	return ret;
}

// TODO fix ==> uint32 (* )[32] written as  uint32[32] *
bool VariableDescriptor::ToString(DynamicCString &string)const{
	bool ret=true;
	int32 maxLayer =0;
	char8 modifier;
	uint64 size;
	int32 rd = -1;
	// first loop add all prefix modifiers that encapsulate: Vector<  Matrix<
	while ( BrowseModifiersLayerR(modifier,size,modifiers.GetList(),maxLayer,rd) && ret){
		const APLookUp *apl = reverseLookUpCode(modifier);
		if (apl != NULL){
			string.AppendN(apl->cExpansionPre);
		} else {
			ret = false;
		}
		maxLayer++;
		rd = -1;
	}
	maxLayer--;

	if (ret) {
		ret = typeDescriptor.ToString(string);
	}

	// second loop close encapsulating modifiers and add postfix ones> * [N]
	while ((maxLayer>=0)  && ret){
		rd = -1;
		ret = BrowseModifiersLayerR(modifier,size,modifiers.GetList(),maxLayer,rd);

		if (ret){
			const APLookUp *apl = reverseLookUpCode(modifier);
			if (apl != NULL){
				if (apl->cExpansionPost.GetList() != NULL ){
					string.AppendN(apl->cExpansionPost);
					string.AppendNum(size);
				}
				string.AppendN(apl->cExpansionEnd);
			} else {
				ret = false;
			}
		}
		maxLayer--;
	}

	return ret;

}




}


	
