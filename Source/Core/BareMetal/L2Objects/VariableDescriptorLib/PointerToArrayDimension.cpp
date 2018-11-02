/**
 * @file PointerToArrayDimension.cpp
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

#include "PointerToArrayDimension.h"
#include "MemoryCheck.h"
#include "Memory.h"
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



PointerToArrayDimension::PointerToArrayDimension(uint32 numberOfElementsIn,bool constant): ArrayDimension(numberOfElementsIn){
	if (constant){
		flags |= isConstant;
	}
	flags |= isBreak;
}

PointerToArrayDimension::~PointerToArrayDimension(){}


DimensionSize PointerToArrayDimension::GetLayerCombinedElementSize() const{
	return DimensionSize(sizeof(void *));
}

uint32 PointerToArrayDimension::TypeSize() const {
	return sizeof(void *);
};

char8 PointerToArrayDimension::TypeChar() const {
	char8 ret = 'F';
	if ((flags & isConstant)!=0){
		ret = 'f';
	}
	return ret;
}

ErrorManagement::ErrorType PointerToArrayDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {
	ErrorManagement::ErrorType ok;

	numberOfRows = 1;
	numberOfColumns = numberOfElements;

	// all need to redirect
	// it works as vector is descendant of Pointer class
	ok = VariableDescriptorLib::RedirectP(ptr,true);
	COMPOSITE_REPORT_ERROR(ok, "pointer to array type redirection failed ");

	// handle case of null pointers
	if (ptr == NULL) {
		numberOfColumns = 0;
	}

	return ok;
}

ErrorManagement::ErrorType PointerToArrayDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {
	ErrorManagement::ErrorType ret;

	return SetPtrToZero(numberOfElements, ptr);
}



}
} //MARTe
