/**
 * @file ProgressiveTypeCreator.cpp
 * @brief Header file for class AnyType
 * @date 10 Jan 2018
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

#include <new.h>
#include "ProgressiveTypeCreator.h"
#include "TypeConversionManager.h"
#include "GlobalObjectsDatabase.h"
#include "Memory.h"
#include "MemoryPageObject.h"
#include "AnyObject.h"

namespace MARTe{


ProgressiveTypeCreator::SizeStack::SizeStack():StaticListHolder(sizeof(uint32),64,0,0){
}

bool ProgressiveTypeCreator::SizeStack::Push(uint32 size){
	return Add(&size);
}

uint32 ProgressiveTypeCreator::SizeStack::NumberOfElements(){
	return GetSize();
}

uint32 ProgressiveTypeCreator::SizeStack::operator[](uint32 index){
	uint32 value=0;
	Peek(index,&value);
	return value;
}

void ProgressiveTypeCreator::SizeStack::Clean(){
	StaticListHolder::Clean();
}

/**
 * @brief Creates the object selecting the type to convert to and the default PageSize
 */
ProgressiveTypeCreator::ProgressiveTypeCreator(uint32 pageSizeIn ):pageFile(pageSizeIn){
	originalPageSize 	= pageSizeIn;
	converter 			= NULL;
	Clean();
}

/**
 * @deletes the object and any memory allocated in the pages
 */
ProgressiveTypeCreator::~ProgressiveTypeCreator(){
	Clean();
}


void ProgressiveTypeCreator::Clean(){
	if (converter != NULL){
		delete converter;
		converter = NULL;
	}
	pageFile.Clean(originalPageSize);
	type 				= InvalidType(0);
	objectSize 			= 0;
	status 				= notStarted;
	converter 			= NULL;
	matrixRowSize 		= 0;
	vectorSize 			= 0;
	currentVectorSize 	= 0;
	isString            = false;
	sizeStack.Clean();
	numberOfElements    = 0;
}

ErrorManagement::ErrorType ProgressiveTypeCreator::Start(TypeDescriptor typeIn){
	ErrorManagement::ErrorType ret;

	Clean();

	type 				= typeIn;
	objectSize 			= typeIn.StorageSize();
	isString 			= typeIn.IsCharString();
	ret.parametersError = (objectSize == 0);
	CONDITIONAL_REPORT_ERROR(ret,"Type with 0 size");

	if (!(isString || typeIn.IsBasicType())){
		ret.parametersError = !(isString || typeIn.IsBasicType());
		REPORT_ERROR(ret,"Unsupported type. Must be CString or BasicType");
	}
	if (ret && !isString){
		converter 			= TypeConversionManager::Instance().GetOperator(type,ConstCharString(sizeof(CString)),false);
		ret.unsupportedFeature = (converter == NULL);
		CONDITIONAL_REPORT_ERROR(ret,"Cannot find type converter");
	}

	if (ret){
		status 				= started;
	} else {
		status 				= error;
	}
	return ret;
}

/**
 * @adds an element. It will convert the string to the specified type
 */
ErrorManagement::ErrorType ProgressiveTypeCreator::AddElement(CCString typeStringRepresentation){
	ErrorManagement::ErrorType ret;

	ret.internalStateError = (!Started());
	CONDITIONAL_REPORT_ERROR(ret,"AddElement and status not started");

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
			ret.unsupportedFeature = true;
			REPORT_ERROR(ErrorManagement::UnsupportedFeature,"unexpected state");
		}
		}
	}

	uint32 neededSize = objectSize;
	if (ret){
		if (isString){
			neededSize = typeStringRepresentation.GetSize()+1;

			uint8 *pointer;
			ret = pageFile.WriteReserveAtomic(pointer,neededSize);
			CONDITIONAL_REPORT_ERROR(ret,"WriteReserveAtomic Failed");

			if (ret){
				Memory::Copy(pointer,typeStringRepresentation.GetList(),neededSize);
			}

		} else {
			// checks memory available in current page
			// if not enough resize
			// in case of new Rows check if space is enough for a row
			// otherwise opens a new page
			if (newRow){
				uint32 estimatedSizeNeeded = vectorSize * objectSize;
//printf("CheckAndTrimPage(%i)\n",estimatedSizeNeeded); //TODO

				// in case of new vector check if there is space for one based on the size of the previous
				// close page otherwise
				ret = pageFile.CheckAndTrimPage(estimatedSizeNeeded);
				CONDITIONAL_REPORT_ERROR(ret,"CheckAndTrimPage Failed");

				if (ret){
//printf("CheckAndNewPage()\n"); //TODO
					ret = pageFile.CheckAndNewPage();
					CONDITIONAL_REPORT_ERROR(ret,"CheckAndNewPage Failed");
				}

			}
			uint8 *pointer;
			if (ret ){
				ret = pageFile.WriteReserveExtended(pointer,neededSize);
				CONDITIONAL_REPORT_ERROR(ret,"WriteReserveExtended Failed");
			}

			if (ret){
				// queue type to memory
				ret = converter->Convert(pointer,reinterpret_cast<const uint8 *>(&typeStringRepresentation),1);
				CONDITIONAL_REPORT_ERROR(ret,"converter->Convert Failed");
				//					ret = converter->Convert(	reinterpret_cast<uint8 *>(page.Address(pageWritePos)),reinterpret_cast<const uint8 *>(&typeStringRepresentation),1);
			}
		}
	}

	// all ok - means we used some memory
	if (ret){
		numberOfElements++;
	} else {
		status = error;
		REPORT_ERROR(ret,"AddElement failed");
	}
	return ret;
}

ErrorManagement::ErrorType ProgressiveTypeCreator::EndVector(){
	ErrorManagement::ErrorType  ret;

	/// update status
	switch (status){
	case started:{
		ret.illegalOperation = true;
		status = error;
	}break;
	case vector:
	case scalar:{
		status = vectorEnd;
		vectorSize = currentVectorSize;
		currentVectorSize = 0;
	}break;
	case vectorEnd:
	case matrixRowEnd:
	case matrixRow:{
		if (currentVectorSize != vectorSize){
			status = sparseMatrixRE;
			int i;
			for (i=0;i<(matrixRowSize-1);i++){
				sizeStack.Push(vectorSize);
			}
			sizeStack.Push(currentVectorSize);
		} else {
			status = matrixRowEnd;
		}
		if (currentVectorSize > vectorSize){
			vectorSize = currentVectorSize;
		}
		currentVectorSize = 0;
	}break;
	case sparseMatrixRow:
	case sparseMatrixRE:{
//printf("nEl = %i\n",currentVectorSize); TODO
		sizeStack.Push(currentVectorSize);
		currentVectorSize = 0;
		status = sparseMatrixRE;
	}break;
	// if Finished() goes here
	default:{
		ret.illegalOperation = true;
		status = error;
	}
	}

	if (!ret){
		REPORT_ERROR(ret,"EndVector failed");
	}
	return ret;
}

ErrorManagement::ErrorType ProgressiveTypeCreator::End(){
	ErrorManagement::ErrorType  ret;

	switch (status){
	case started:{
		ret.notCompleted = true;
		status = error;
	}break;
	case scalar:{
		status = finishedS;
	}break;
	case vectorEnd:{
		status = finishedV;
	}break;
	case sparseMatrixRE:{
		status = finishedSM;
	}break;
	case matrixRowEnd:{
		status = finishedM;
	}break;
	case error:
	default:{
		status = error;
		REPORT_ERROR(ErrorManagement::InternalStateError,"unexpected state");
		ret.internalStateError = true;
	}
	}
	if (!ret){
		REPORT_ERROR(ret,"End() failed");
	}
	return ret;
}

ErrorManagement::ErrorType ProgressiveTypeCreator::CompleteFixedSizeEl(uint8 *&dataPtr,uint32 &auxSize,uint8 *&auxPtr){
	ErrorManagement::ErrorType  ret;

	dataPtr = pageFile.CurrentReadPointer();

	auxSize = 0;
	if (status == finishedSM){
		auxSize  = sizeof (Vector<uint8>) * matrixRowSize;
	} else
		if ((status == finishedM)&&(pageFile.NumberOfPages() > 1)){
			auxSize = sizeof (void *) * matrixRowSize;
		}


	// if auxSize is not zero
	if (auxSize > 0){

		ret = pageFile.WriteReserveAtomic(auxPtr,auxSize);
		CONDITIONAL_REPORT_ERROR(ret,"WriteReserveAtomic Failed");
	}

	if (ret){
		ret = pageFile.CheckAndTrimPage();
		CONDITIONAL_REPORT_ERROR(ret,"CheckAndTrimPage Failed");
	}


	return ret;
}

ErrorManagement::ErrorType ProgressiveTypeCreator::CompleteStringEl(uint8 *&dataPtr,uint32 &auxSize,uint8 *&auxPtr){
	ErrorManagement::ErrorType  ret;
	// table of CCString (pointers)
	uint32 CCStringTableSize = numberOfElements * sizeof(CCString);

//printf("Strings Pointer Table size= %i\n",CCStringTableSize); // TODO
//printf ("numberOfElements=%i\n",numberOfElements); //TODO

	// table of Vector<CCString>
	if (status == finishedSM){
		auxSize = sizeof (Vector<uint8>) * matrixRowSize;
//printf ("matrixRowSize=%i\n",matrixRowSize); //TODO
	}

	CCString *strings = NULL;

	if (ret){
		ret = pageFile.WriteReserveAtomic(dataPtr,CCStringTableSize);
		CONDITIONAL_REPORT_ERROR(ret,"WriteReserveAtomic Failed");
		strings = reinterpret_cast<CCString *>(dataPtr);
//printf ("dataPtr=%p\n",dataPtr); //TODO
	}

	if (ret  && (auxSize > 0)){
		ret = pageFile.WriteReserveAtomic(auxPtr,auxSize);
		CONDITIONAL_REPORT_ERROR(ret,"WriteReserveAtomic Failed");
//printf ("auxPtr=%p\n",dataPtr); //TODO
	}

	if (ret){
		ret = pageFile.CheckAndTrimPage();
		CONDITIONAL_REPORT_ERROR(ret,"CheckAndTrimPage Failed");
	}

	if (ret){

		uint64 pageDepth=0;
		// prepare table of string addresses
		for (int i = 0;(i<numberOfElements) && ret;i++){
			// consecutiveSpan as input is how many character we need
			uint32 consecutiveSpan=1;

			if (ret){
				CCString s = reinterpret_cast<char8 *> (pageFile.CurrentReadPointer());
				strings[i] = s;
			}

			// get pointer to element within pages at overall address pageDepth
			//				CCString s = reinterpret_cast<char8 *> (page.DeepAddress(pageDepth,consecutiveSpan));
			ret = pageFile.ConsumeReadAtomic(0);

		}
	}

	return ret;
}

/**
 * @brief Allows retrieving the Object that has been built.
 * Can only be done after End has been called
 */
ErrorManagement::ErrorType ProgressiveTypeCreator::GetReference(Reference &x){
	ErrorManagement::ErrorType  ret;

	ret.fatalError = (!Finished());
	CONDITIONAL_REPORT_ERROR(ret,"Not Finished");

	// points to data or to pointers to data (CCString)
	uint8 *dataPtr = NULL;
	// auxiliary structures (array of pointers or array of Vector<>
	uint8 *auxPtr  = NULL;
	uint32 auxSize = 0;

	if (ret){
		if (isString){
			// Flip multi page data
			// prepares pointers to actual strings - returns that as main data
			// allocate Vector<> array in case of sparse matrices
			ret = CompleteStringEl(dataPtr,auxSize,auxPtr);
			CONDITIONAL_REPORT_ERROR(ret,"CompleteStringEl Failed");
		} else {
			// Flip multi page data
			// allocate Vector<> array in case of sparse matrices
			// allocate array of pointers in case of multipage data
			ret = CompleteFixedSizeEl(dataPtr,auxSize,auxPtr);
			CONDITIONAL_REPORT_ERROR(ret,"CompleteFixedSizeEl Failed");
		}
	}

	if (ret){
		// Completes operation
		// fills aux Data
		// creates Object
		ret = GetReferencePrivate(x, dataPtr, auxPtr,auxSize);
		CONDITIONAL_REPORT_ERROR(ret,"GetReferencePrivate Failed");
	}

	if (!ret) {
		REPORT_ERROR(ret,"GetReference failed");
	}

	return ret;
}

ErrorManagement::ErrorType ProgressiveTypeCreator::GetReferencePrivate(Reference &x, uint8 *dataPtr, uint8 *auxPtr,uint32 auxSize){
	ErrorManagement::ErrorType  ret;


	DynamicCString mods;
	switch (status){
	case finishedS:{
	}break;
	case finishedV:{
		if (vectorSize > 1){
			mods.Append('A');
			mods.Append(vectorSize);
		}
	}break;
	case finishedM:{
		// auxPtr not NULL means fragmented
		if (auxPtr != NULL){
			ret.internalSetupError = (auxSize != sizeof (void *) * matrixRowSize);
			CONDITIONAL_REPORT_ERROR(ret,"auxSize is not adequate");

			// reorder the pages correctly to allow access to data
			if (ret){
				uint8**addressMap = reinterpret_cast<uint8**>(auxPtr);
				uint32 vectorByteSize = vectorSize * objectSize;

				for (int i = 0;(i<matrixRowSize) && ret;i++){
					addressMap[i] = pageFile.CurrentReadPointer();

					ret = pageFile.ConsumeReadAtomic(vectorByteSize);
					CONDITIONAL_REPORT_ERROR(ret,"ConsumeReadAtomic failed");
				}
			}

			if (ret){
				mods.Append('A');
				mods.Append(matrixRowSize);
				mods.Append("PA");
				mods.Append(vectorSize);
				dataPtr = auxPtr;
			}

		} else {
			if (matrixRowSize > 1){
				mods.Append('A');
				mods.Append(matrixRowSize);
			}
			if (vectorSize > 1){
				mods.Append('A');
				mods.Append(vectorSize);
			}
		}
	}break;
	case finishedSM:{
		if ((auxSize != (sizeof (Vector<uint8>) * matrixRowSize)) || (auxPtr == NULL)){
			ret.internalSetupError = true;
			if (auxPtr == NULL){
				REPORT_ERROR(ret,"auxPtr is NULL");
			} else {
				REPORT_ERROR(ret,"auxSize is not adequate");
			}
		}

		// now get the address and then reorder the pages correctly to allow access to data
		if (ret){
			Vector<uint8> *addressMap = reinterpret_cast<Vector<uint8>*>(auxPtr);

			for (int i = 0;(i<matrixRowSize) && ret;i++){
				uint32 size  = sizeStack[i];
				uint32 vectorByteSize = size * objectSize;

				addressMap[i].InitVector(pageFile.CurrentReadPointer(),size);

				ret = pageFile.ConsumeReadAtomic(vectorByteSize);
				CONDITIONAL_REPORT_ERROR(ret,"ConsumeReadAtomic failed");

			}
		}

		if (ret){
			mods.Append('A');
			mods.Append(matrixRowSize);
			mods.Append('V');
			dataPtr = auxPtr;
		}

	}break;
	case error:{
		ret.fatalError = true;
		REPORT_ERROR(ret,"On Error");
	}break;
	default:{
		ret.notCompleted = true;
		REPORT_ERROR(ret,"Unexpected state?");
	}
	}

	uint8 *firstElPtr = pageFile.Address();

	if (ret){
		uint32 pageSize = pageFile.CurrentPageSize();
		if ((pageFile.NumberOfPages()==1) && (firstElPtr == dataPtr) && (pageSize <= 64)){
			VariableDescriptor vd(type,mods);

			x = AnyObject::Clone(pageSize,reinterpret_cast<void *>(dataPtr),vd);
		}
		else {
			ReferenceT<MemoryPageObject> mpor;

			mpor = ReferenceT<MemoryPageObject> (buildNow);
			ret.outOfMemory = (!mpor.IsValid());
			CONDITIONAL_REPORT_ERROR(ret,"MemoryPageObject construction failed");

			if (ret){
				mpor->Setup(type,mods,dataPtr,pageFile);
				x = mpor;
			}
		}
	}

	if (!ret){
		REPORT_ERROR(ret,"GetReferencePrivate failed");
	} else {
		status = notStarted;
	}
	return ret;
}


} //MARTe
