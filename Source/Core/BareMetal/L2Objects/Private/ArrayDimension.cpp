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

 * @details.
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



ArrayDimension::ArrayDimension(uint32 numberOfElementsIn): Dimension(){
	numberOfElements = numberOfElementsIn;
}

ArrayDimension::~ArrayDimension(){
}

uint32 ArrayDimension::TypeSize() const {
	return 0;
};

char8 ArrayDimension::TypeChar() const {
	return 'A';
}

DimensionSize ArrayDimension::GetLayerCombinedElementSize() const{
	DimensionSize ret(numberOfElements);
	if (next != NULL){
		ret = ret * next->GetLayerCombinedElementSize();
	} else{
		REPORT_ERROR(ErrorManagement::FatalError,"unterminated stack");
	}

return ret;
}

ErrorManagement::ErrorType ArrayDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {

	ErrorManagement::ErrorType ok;

	numberOfRows = 1;
	numberOfColumns = numberOfElements;
	return ok;
}

ErrorManagement::ErrorType ArrayDimension::GetNumberOfElements(uint32 &numberOfElements) const {
	ErrorManagement::ErrorType ok;
	numberOfElements = this->numberOfElements;
	return ok;
}

ErrorManagement::ErrorType ArrayDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {

	return InitStack(ptr,numberOfElements * this->numberOfElements);
}




}
} //MARTe
