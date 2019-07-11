/**
 * @file MatrixDimension.cpp
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

#include "MatrixDimension.h"
#include "MemoryCheck.h"
#include "Matrix.h"
#include "MemoryOperators.h"
#include "CompositeErrorManagement.h"
#include "VariableDescriptorLib.h"
#include "HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe{
namespace VariableDescriptorLib{



MatrixDimension::MatrixDimension(bool constant): Dimension(){
	if (constant){
		flags |= isConstant;
	}else {
		flags |= isDynamic;
	}
	flags |= isBreak;
	flags |= is2D;
}

MatrixDimension::~MatrixDimension(){
}

DimensionSize MatrixDimension::GetLayerCombinedElementSize() const{
	return DimensionSize(TypeSize());
}

uint32 MatrixDimension::TypeSize() const {
	return sizeof(Matrix<char8>);
}

char8 MatrixDimension::TypeChar() const {
	char8 ret = 'M';
	if ((flags & isConstant)!=0){
		ret = 'm';
	}
	return ret;
}

ErrorManagement::ErrorType MatrixDimension::UpdatePointerAndSizeEx(
			const uint8 *&	ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const {
	ErrorManagement::ErrorType ok;

	bool allowNULL = false;
	// get size from structure
	const Matrix<char8> *pm = reinterpret_cast<const Matrix<char8> *>(ptr);

	numberOfRows = pm->GetNumberOfRows();
	numberOfColumns = pm->GetNumberOfColumns();
	if (pm->GetNumberOfElements() == 0U){
		allowNULL = true;
	}

	// all need to redirect
	// it works as vector is descendant of Pointer class
	ok = VariableDescriptorLib::RedirectP(ptr,allowNULL);
	COMPOSITE_REPORT_ERROR(ok, "Matrix type redirection failed ");

    // handle case of null pointers
	if (ptr == NULL) {
		numberOfColumns = 0;
		numberOfRows = 0;
	}
	return ok;
}

uint8 MatrixDimension::NOfDimensions() const{
	return 2;
}

ErrorManagement::ErrorType MatrixDimension::ReSize(
			uint8 *&		ptr,
			uint32 &		numberOfColumns,
			uint32 &		numberOfRows) const{
	ErrorManagement::ErrorType ret;

	DimensionSize totalSizeD ;
	ret = GetNextLayerElementSize(totalSizeD);
    REPORT_ERROR(ret, "GetNextLayerElementSize failed");

    uint32 totalAllocationSize = 0;
    if (ret){
    	totalSizeD = totalSizeD * numberOfColumns;
    	totalSizeD = totalSizeD * numberOfRows;

    	ret = totalSizeD.ToNumber(totalAllocationSize);
        COMPOSITE_REPORT_ERROR(ret, "total size overflow: NCol = ",numberOfColumns,"NRows = ",numberOfRows);
    }

    uint8 *newMemory = NULL_PTR(uint8 *);
	if (ret){
		// allocate memory
		newMemory = reinterpret_cast<uint8 *>(HeapManager::Malloc(totalAllocationSize));
		ret.outOfMemory = (newMemory == NULL_PTR(uint8 *));
		COMPOSITE_REPORT_ERROR(ret, "malloc of ", totalAllocationSize ," failed" );
	}

	Matrix<uint8> *pMat = reinterpret_cast<Matrix<uint8> *>(ptr);
	if (ret){
		// resize vector
		pMat->SetSize(0,0);
		pMat->InitMatrix(newMemory,numberOfRows,numberOfColumns,heapMalloc);// true means allocated

		ret.internalSetupError = (next == NULL_PTR(Dimension *));
		REPORT_ERROR(ret,"next layer to a Matrix is NULL");

		if (ret){
			ret = next->InitStack(newMemory,numberOfColumns*numberOfRows);
			REPORT_ERROR(ret,"memory init failed");
		}

		//redirect to data
		if (ret){
			ptr = newMemory;
		}
	}

	return ret;
}

ErrorManagement::ErrorType MatrixDimension::InitStack(
			uint8 * 		ptr,
			DimensionSize	numberOfElements) const {
	ErrorManagement::ErrorType ret;

	uint32 n;
	ret = numberOfElements.ToNumber(n);
	REPORT_ERROR(ret,"number of elements overflow");

	if (ret){
		Matrix<char8 >* mptr = reinterpret_cast<Matrix<char8 >* >(ptr);
		for (uint32 i = 0; i< n;i++){
			mptr->InitMatrix(NULL,0,0);
			mptr++;
		}
	}

	return ret;
}




}
} //MARTe
