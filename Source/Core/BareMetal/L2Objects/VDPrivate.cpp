/**
 * @file VDPrivate.cpp
 * @brief Header file for class AnyType
 * @date 8 Feb 2018
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

#include "VDPrivate.h"
#include "Matrix.h"
#include "TypeDescriptor.h"
#include "MemoryCheck.h"
#include "ZeroTerminatedArray.h"



namespace MARTe{

static inline bool isNumber(char8 c){
	return ((c >='0') && (c <='9'));
}

static inline uint32 toNumber(char8 c){
	return  static_cast<uint32>(c - '0') ;
}

static inline char8 toUpper(char8 c){
	if ((c >='a') && (c <= 'z')){
		c = (c - 'a') + 'A';
	}
	return c;
}

/** string to integer */
static inline uint32 readNumber(CCString &buffer){
	uint32 result = 0;
	while (isNumber(buffer[0])){
		result = result * 10u;
		result += toNumber(buffer[0]);
		buffer++;
	}
	return result;
}

/**
 * extracts information about a layer
 * updates the layer string pointer
 * used 8 times
 */
static inline void GetLayerInfo(CCString &modifierString,char8 &modifier,uint32 &size ){
	if (modifierString.IsNullPtr()){
		modifier = '\0';
		size = 0;
	} else {
		modifier = modifierString[0];
		if (modifier == '\0'){
			size = 0;
		} else {
			modifierString++;
			size = readNumber(modifierString);
		}
	}
}

static inline bool IsConst(char8 c){
	bool ret = true;
	if ((c >= 'A')  && (c <= 'Z')) {
		ret = false;
	}
	return ret;
}

//#include <stdio.h>
/**
 *
 */
DimensionHandler::DimensionHandler(CCString modifiers,TypeDescriptor tdIn){
	td = tdIn;
//printf("%i %i\n",dimensions.GetCapacity(),dimensions.GetMaxCapacity());
//printf ("modifier = %s ",modifiers.GetList());
	const CCString terminals = "VMvm";
	const CCString Zterminals = "ZDSzds";
	char8 modifier;
	uint32 size;
	GetLayerInfo(modifiers,modifier,size);
	char8 pointer = '\0';
	bool finished = false;
	while (!finished){
		if (modifier == '\0'){
			if (pointer != '\0') {
				td = PointerType;
				if (IsConst(pointer)){
					td.dataIsConstant = true;
				}
			}
			dimensions.Add(DimensionInfoElement('\0',1));
//printf("{0}\n");
			finished = true;
		} else
		if (modifier == 'A'){
			if (pointer == 'P'){
				dimensions.Add(DimensionInfoElement('F',size));
			} else
			if (pointer == 'p'){
				dimensions.Add(DimensionInfoElement('f',size));
			} else {
				dimensions.Add(DimensionInfoElement('A',size));
//printf("{A}\n");
			}
			pointer = '\0';
		} else {
			if ((modifier == 'P') || (modifier == 'p')){
				// PP double indirection is a terminal case
				if (pointer != '\0') {
					td = PointerType;
					if (IsConst(pointer)){
						td.dataIsConstant = true;
					}
					dimensions.Add(DimensionInfoElement('\0',1));
					finished = true;
				} else {
					pointer = modifier;
				}
			} else
			if (terminals.In(modifier)){
				// PV PM double indirection is a terminal case
				if (pointer != '\0') {
					td = PointerType;
					dimensions.Add(DimensionInfoElement('\0',1));
					finished = true;
				} else {
//printf("{%c}\n",modifier);
					dimensions.Add(DimensionInfoElement(modifier,0));
				}
			} else
			if (Zterminals.In(modifier)){
				// only PZ PD PS are allowed
				if (pointer != '\0') {
					pointer = '\0';
					dimensions.Add(DimensionInfoElement(modifier,0));
				} else {
					td = InvalidType(0);
				}
			}

		}
		if (!finished){
			GetLayerInfo(modifiers,modifier,size);
		}
	}
//printf ("dimensions = %i\n ",dimensions.GetSize());

//printf("%i %i\n",dimensions.GetCapacity(),dimensions.GetMaxCapacity());

	uint64 elementSize = 1;
//	uint64 dimensionsProduct = 1;
	char8 layerEnd = '\0';
	for (int i = (dimensions.GetSize()-1); i >= 0; i--){
		if (dimensions[i].type != 'A'){
//			dimensionsProduct = 1;
			layerEnd = dimensions[i].type;
			elementSize = DimensionSize(dimensions[i].type);
		} else {
			elementSize *= dimensions[i].numberOfElements;
		}
//		dimensionsProduct *= dimensions[i].numberOfElements;
		// this includes the dimension of this layer
//		dimensions.Access(i).dimensionsProduct = dimensionsProduct;
		dimensions.Access(i).layerEnd = layerEnd;
		dimensions.Access(i).elementSize = elementSize;
	}

//printf("%i %i\n",dimensions.GetCapacity(),dimensions.GetMaxCapacity());

}


ErrorManagement::ErrorType  DimensionHandler::HasSameDimensionsAs(const DimensionHandler &other) const{
	ErrorManagement::ErrorType ok = true;
	uint32 nDim = NDimensions();
	uint32 nDimO = other.NDimensions();
	if (nDim == 0){
		ok.internalSetupError=true;
        REPORT_ERROR(ok, "NDimensions == 0");
	}
	if (nDim != nDimO){
		ok.invalidOperation=true;
        REPORT_ERROR(ok, "different number of dimensions");
		DynamicCString errM;
		errM.AppendN("N of dim[");
		for (int i = 0;i < nDim;i++){
			char8 type = dimensions[i].type;
			if (type != '\0') errM.Append(type);
			else errM.Append('0');
		}
		errM.AppendN( "]= ");
		errM.AppendNum(nDimO);
		errM.AppendN(" != [");
		for (int i = 0;i < nDim;i++){
			char8 type = other.dimensions[i].type;
			if (type != '\0') errM.Append(type);
			else errM.Append('0');
		}
		errM.Append(']');
		errM.AppendNum(nDimO);
        REPORT_ERROR(ok, errM.GetList());
	}

	other[0].numberOfElements;
	for (int i = 0;ok && (i < nDim); i++){
		uint32 d1 = dimensions[i].numberOfElements;
		uint32 d2 = other.dimensions[i].numberOfElements;

		if ((d1 != d2) && ( d1!=0 ) && ( d2 != 0)){
			ok.invalidOperation=true;
			DynamicCString errM;
			errM.AppendN("dimension[");
			errM.AppendNum(i);
			errM.AppendN("] d1= ");
			errM.AppendNum(d1);
			errM.AppendN(" d2= ");
			errM.AppendNum(d2);
	        REPORT_ERROR(ok, errM.GetList());
		}
	}

	return ok;
}

uint32 DimensionHandler::DimensionSize(char8 c) const{
	const CCString matrixS = "Mm";
	const CCString vectorS = "Vv";
	const CCString pointerS = "FfSDZPsdzp";
	uint32 size = 0;
	if (pointerS.In(c)){
		size = sizeof(void *);
	} else
	if (matrixS.In(c)){
		size = sizeof(Matrix<char8>);
	} else
	if (vectorS.In(c)){
		size = sizeof(Vector<char8>);
	} else {
		size = td.StorageSize();
	}
	return size;
}

static inline ErrorManagement::ErrorType RedirectP(const uint8* &ptr,bool allowNULL){
	ErrorManagement::ErrorType ret;
	const uint8 **pp = (const uint8 **)(ptr);
	const uint8 *p = *pp;
	if ((p == NULL) && (allowNULL)){
		ptr = p;
	} else
	if ((p == NULL) || (!MemoryCheck::Check(p))){
		ret.exception = true;
		DynamicCString errM;
		errM.AppendN("bad pointer (");
		errM.AppendHex(reinterpret_cast<uint64>(p));
		errM.AppendN(") at (");
		errM.AppendHex(reinterpret_cast<uint64>(pp));
		errM.Append(')');
        REPORT_ERROR(ret, errM.GetList());
	} else {
		ptr = p;
	}
	return ret;
}

#include <stdio.h>
ErrorManagement::ErrorType DimensionHandler::UpdatePointerAndSize(uint32 layerIndex, const uint8 *&ptr,uint32 &numberOfElements,uint64 &elementSize,uint32 &overHead) const {
	ErrorManagement::ErrorType ok;
	bool isZta = false;
	const DimensionInfoElement &di = dimensions[layerIndex];
	const DimensionInfoElement &diNext = dimensions[layerIndex+1];
	elementSize = diNext.elementSize;
	uint8 type = di.type;
	bool allowNULL = false;
	overHead = 0;
/*
printf("[%i] Type=%i ",layerIndex,di.type);
printf("[%i] Type=%i ",layerIndex+1,diNext.type);
*/
	switch (type){

	case '\0':{
		numberOfElements = 1;
		type = 'A';
		elementSize = di.elementSize;
//printf("[0NEL=%i]",numberOfElements);
	} break;
	case 'f':
	case 'F':{
		overHead = sizeof (void *);
		numberOfElements = di.numberOfElements;
		allowNULL = true;
	} break;
	case 'A':{
		numberOfElements = di.numberOfElements;
//printf("[ANEL=%i]",numberOfElements);
	}break;
	case 'd':
	case 's':
	case 'z':
	case 'D':
	case 'S':
	case 'Z':{
		isZta = true;
		overHead = sizeof (void *) + diNext.elementSize;
	}break;
	case 'v':
	case 'V':{
		overHead = sizeof (Vector<char8>);
		const Vector<char8> *pv = reinterpret_cast<const Vector<char8> *>(ptr);
		numberOfElements = pv->GetNumberOfElements();
		if (numberOfElements == 0){
			allowNULL = true;
		}
//printf("[VNEL=%i]",numberOfElements);
	}break;
	case 'm':
	case 'M':{
		overHead = sizeof (Matrix<char8>);
		const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);
		numberOfElements = pm->GetNumberOfElements();
		if (numberOfElements == 0){
			allowNULL = true;
		}
	}break;
	default:{
		ok.fatalError = true;
        REPORT_ERROR(ok, "Default case reached");
	}break;

	}


	if ((type != 'A') && ok){
		// it works as vector is descendant of Pointer class
//printf("X p1=%p ",ptr);
		ok = RedirectP(ptr,allowNULL);
//printf(" p2=%p \n",ptr);
		if (!ok){
			DynamicCString errM;
			errM.AppendN("type ");
			errM.Append(type);
			errM.AppendN(" redirection failed ");
	        REPORT_ERROR(ok, errM.GetList());
		}
		if (ptr == NULL){
			numberOfElements = 0;
		}
	}
//printf("els=%lli \n",elementSize);

	if (isZta && ok){
		numberOfElements = ZeroTerminatedArrayGetSize(ptr, diNext.elementSize);
	}
	return ok;
}




} //MARTe
