/**
 * @file ProgressiveCStringArrayCreator.cpp
 * @brief Header file for class AnyType
 * @date 22 Jan 2018
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
#define DLL_API


#include "ProgressiveCStringArrayCreator.h"

namespace MARTe{


/**
 * @adds an element. It will convert the string to the specified type
 */
ErrorManagement::ErrorType ProgressiveCStringArrayCreator::AddElement(CCString typeStringRepresentation){
	ErrorManagement::ErrorType ret;
	ret.fatalError = !Started();

	bool newRow = false;
	if (ret){
		/// update status
		switch (status){
		case started:{
			status = scalar;
			vectorSize = 1;
			matrixRowSize = 1;
			currentVectorSize = 1;
		}break;
		case scalar:{
			status = vector;
			vectorSize = 2;
			currentVectorSize = 2;
		}break;
		case vectorEnd:
		case matrixRowEnd:{
			status = matrixRow;
			currentVectorSize = 1;
			matrixRowSize++;
			newRow = true;
		}break;
		case sparseMatrixRE:{
			status = sparseMatrixRow;
			currentVectorSize=1;
			matrixRowSize++;
			newRow = true;
		}break;
		case vector:
		case sparseMatrixRow:
		case matrixRow:{
			currentVectorSize++;
		}break;
		default:{
			status = error;
			ret.fatalError = true;
		}
		}
	}

	uint32 neededSize = typeStringRepresentation.GetSize()+1;
	if (ret){
		ret = CheckAndClosePage(neededSize);
	}

	// check for initial 0 size page or begin of new vector 0 size page (after CheckAndClosePage)
	if (ret){
		// no memory - allocate
		// could be at the beginning or after a vector has been completed
		// if no more vectors of the same size can be fitted
		ret = CheckAndNewPage(defaultPageSize);
	}

	if (ret){
		MemoryOperationsHelper::Copy(page.Address(pageWritePos),typeStringRepresentation.GetList(),neededSize);
		sizeLeft -= neededSize;
		pageWritePos += neededSize;
	}

	if (!ret){
		status = error;
	}
	return ret;
}

static inline bool stringBoundSize(CCString s, uint32 &sizeInOut){
	const char8 *p = s.GetList();
	bool ret = (p!=NULL);

	if (ret){
		uint32 sz = 0;
		while ((sz < sizeInOut) && (*p!='\0')){
			p++;
			sz++;
		}

		if (*p == '\0'){
			sizeInOut = sz;
		} else {
			ret = false;
		}
	}

	return ret;

}

/**
 * @brief Allows retrieving the Object that has been built.
 * Can only be done after End has been called
 */
ErrorManagement::ErrorType ProgressiveCStringArrayCreator::GetReference(ReferenceT<AnyObjectI> &x){
	ErrorManagement::ErrorType  ret;

	ret.fatalError = !Finished();

	uint32 numberOfElements = matrixRowSize * vectorSize;
	if (ret && (status == finishedSM)){
		numberOfElements = 0;
		for (uint32 i= 0;i<matrixRowSize;i++){
			numberOfElements += sizeStack[i];
		}
	}

	uint32 neededSize = numberOfElements * sizeof(CCString);
	if (ret){
		// close if not big enough
		ret = CheckAndClosePage(neededSize);
	}

	if (ret){
		// open if no page
		ret = CheckAndNewPage(neededSize);
	}

	// process memory and create list of pointers
	CCString *strings = NULL;
	if (ret){
		// remember pointer to string array
		strings = reinterpret_cast<CCString *>(page.Address(this->pageWritePos));

		// put oldest page on top
		if (numberOfPages > 0){
			page.FlipOrder();
		}

		uint64 pageDepth=0;
		// prepare table of string addresses
		for (int i = 0;(i<numberOfElements) && ret;i++){
			// consecutiveSpan is how much space is left on page
			uint32 consecutiveSpan;
			// get pointer to element within pages at overall address pageDepth
			CCString s = reinterpret_cast<char8 *> (page.DeepAddress(pageDepth,consecutiveSpan));
			// empty string error?
			ret.fatalError = (s.GetList() == NULL);
			// calculate string length
			uint32 length = consecutiveSpan;
			if (ret){
				// length but do not scan beyond end of
				ret.fatalError = !stringBoundSize(s,length);
			}
			// update pointer to page
			if (ret){
				// include 0
				length = length+1;
				strings[i] = s;
				pageDepth += length;
			}
		}
	}

	if (ret){
		void *dataPtr = reinterpret_cast<void *>(strings);
		ret = GetReferencePrivate(x, dataPtr, NULL,0);
	}

	return ret;
}






} //MARTe
