/**
 * @file ZTADimension.cpp
 * @brief Header file for class AnyType
 * @date 20 Oct 2018
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

*/

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#define DLL_API

#include "ZTADimension.h"
#include "MemoryCheck.h"
#include "MemoryOperators.h"
#include "CompositeErrorManagement.h"
#include "VariableDescriptorLib.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe{
namespace VariableDescriptorLib{


static ErrorManagement::ErrorType SetPtrToZero(DimensionSize numberOfElements, uint8 *ptr){
	ErrorManagement::ErrorType ret;
	uint32 n;
	numberOfElements = numberOfElements * sizeof(void *);
	ret = numberOfElements.ToNumber(n);
	REPORT_ERROR(ret,"number of elements overflow");

	if (ret){
		ret = Memory::Set(ptr,0,n);
		REPORT_ERROR(ret,"Memory::Set error");
	}
	return ret;
}




ZTADimension::ZTADimension(bool dynamic,bool constant): Dimension(){
	if (constant){
		flags |= isConstant;
	} else {
		if (dynamic){
			flags |= isDynamic;
		}
	}
	flags |= isBreak;
}

ZTADimension::~ZTADimension(){
}

DimensionSize ZTADimension::GetLayerCombinedElementSize() const {
	return DimensionSize(TypeSize());
}

uint32 ZTADimension::TypeSize() const {
	return sizeof(void *);
}

char8 ZTADimension::TypeChar() const {
	char8 ret = 'Z';
	if ((flags & isConstant)!=0){
		if ((flags & isDynamic)!=0){
			ret = 'd';
		} else {
			ret = 'z';
		}
	} else {
		if ((flags & isDynamic)!=0){
			ret = 'D';
		}
	}
	return ret;
}

uint32 ZTADimension::GetOverHead() const {
	uint32 overHead = TypeSize();
	if (next){
		overHead += next->GetLayerCombinedElementSize().GetData();
	}
	return overHead;
}

//#include <stdio.h>  /// TODO remove

ErrorManagement::ErrorType ZTADimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {
	ErrorManagement::ErrorType ok;

	numberOfColumns = 0U;
	numberOfRows = 1U;
	ok.fatalError = (next == NULL);
	REPORT_ERROR(ok,"next element of a ZTA is missing");

	if (ok){
		// all need to redirect
		// it works as vector is descendant of Pointer class
		ok = VariableDescriptorLib::RedirectP(ptr,true);
		REPORT_ERROR(ok, "ZTA type  redirection failed ");
	}

	uint32 nextElementSize = 0U;
	if (ok){
		ok = next->GetLayerCombinedElementSize().ToNumber(nextElementSize);
	    REPORT_ERROR(ok, "elementSize is infinite/indefinite");
	}


	if (ok){
	// note ZTA do not allow null so NULL is trapped by RedirectP
	// calculate actual size of each ZTA
	    REPORT_ERROR(ok, "diNext.elementSize is infinite/indefinite");
		if (ok){
			numberOfColumns = ZeroTerminatedArrayStaticTools::ZTAGetSize(ptr, nextElementSize);
		}
	}
/*
printf("Redirecting a ZTA of element size %i and N= %i at address %p\n",nextElementSize,numberOfColumns,ptr);  //TODO remove
{
void **pp = (void **)ptr;
for (int i=0;i<64;){
	for (int j=0;j<8;j++)
		printf("%p ",pp[i++]);  //TODO remove
	printf("\n ");  //TODO remove
}
}
*/
	return ok;
}

ErrorManagement::ErrorType ZTADimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {
	ErrorManagement::ErrorType ret;

	return SetPtrToZero(numberOfElements, ptr);
}



}
} //MARTe
