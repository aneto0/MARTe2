/**
 * @file APLookup.cpp
 * @brief Header file for class AnyType
 * @date 8 Oct 2017
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/


#include "APLookup.h"

namespace MARTe{


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

 const APLookUp *reverseLookUpCode(char8 code){
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

 CombinedArrayType reverseLookUpArrayTypeGen(char8 code, const APLookUp *apl){
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
 bool reverseLookUpArrayProperties(char8 code, const APLookUp *&apl){
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

 char8 lookUpArrayTypeGen(CombinedArrayType arrayCode, const APLookUp *apl){
	char8 keyCode = '\0';
	while ((apl->keyCode != '\0') && (keyCode == '\0')){
		if (apl->arrayCode == arrayCode){
			keyCode = apl->keyCode;
		}
		apl++;
	}
	return keyCode;
}

 char8 lookUpArrayType(CombinedArrayType arrayCode){
	return lookUpArrayTypeGen(arrayCode,&APLookupTable1[0]);
}

 char8 lookUpCompositeArrayCode(CombinedArrayType arrayCode){
	return lookUpArrayTypeGen(arrayCode,&APLookupTable2[0]);
}


} //MARTe
