/**
 * @file DataDimension.cpp
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#define DLL_API

#include "DataDimension.h"
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

DataDimension::DataDimension(const TypeDescriptor &td): Dimension(){
	endStack = this;
	this->td = td;
	flags = isBreak | isFinal;
	if (td.DataIsConstant()){
		flags |= isConstant;
	}
}

DataDimension::~DataDimension(){
}

uint32 DataDimension::TypeSize() const {
	return td.StorageSize();
};

TypeDescriptor DataDimension::GetTypeDescriptor() const{
	return td;
}

DimensionSize DataDimension::GetLayerCombinedElementSize() const{
	return DimensionSize(td.StorageSize());
}

ErrorManagement::ErrorType DataDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {

	ErrorManagement::ErrorType ok;
	numberOfRows = 1;
	numberOfColumns = 1;
	return ok;
}

ErrorManagement::ErrorType DataDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {

	ErrorManagement::ErrorType ok;

	if (td.SameAs(DynamicCharString)){
		ok = SetPtrToZero(numberOfElements, ptr);
		REPORT_ERROR(ok,"Failed setting memory to 0");
	} else
	{
//TODO handle somehow the copy constructor problem.
// this implementation will fail with structures requiring non-trivial copy constructors
		ok.unsupportedFeature = (!td.IsBasicType() && !td.IsStructuredData());
		REPORT_ERROR(ok,"cannot handle the type");
	}

	return ok;

}

char8 DataDimension::TypeChar() const {
	return 'O';
}



}
} //MARTe
