/**
 * @file VD_Dimension.cpp
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

 * @details .
*/

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#define DLL_API

#include "Dimension.h"
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


Dimension::Dimension(){
	flags = 0;
	next = NULL;
	endStack = NULL;
}

Dimension::~Dimension(){
	if (next != NULL){
		delete next;
		next = NULL;
	}
}


void Dimension::Add(Dimension *newDimension){
	if (next == NULL_PTR(Dimension *)){
		next = newDimension;
	} else {
		next->Add(newDimension);
		if (newDimension->IsBreak() && (endStack==NULL_PTR(Dimension *))){
			endStack = newDimension;
		}
	}
}

void Dimension::AddToEnd(Dimension *&rootPtr,Dimension *newDimension){
	if (newDimension != NULL_PTR(Dimension *)){
		if (rootPtr == NULL_PTR(Dimension *)){
			rootPtr = newDimension;
		} else {
			rootPtr->Add(newDimension);
		}
	}
}

bool Dimension::IsFinalLayer() const{
	bool ret = (next == NULL);
	if (ret == false){
		ret= (next->next == NULL);
	}
	return ret;
}

uint32 Dimension::GetOverHead() const {
	return TypeSize();
}

TypeDescriptor Dimension::GetTypeDescriptor() const{
	return GenericArray;
}

ErrorManagement::ErrorType Dimension::GetNumberOfElements(uint32 &numberOfElements) const{
	return ErrorManagement::IllegalOperation;
}

ErrorManagement::ErrorType Dimension::ReSize(
			uint8 *&		ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const{
	ErrorManagement::ErrorType ret;

	ret.unsupportedFeature = true;

	return ret;
}

/*
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
*/


}
} //MARTe
