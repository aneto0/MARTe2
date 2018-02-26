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
#include "MemoryPageObject.h"
#include "AnyObjectT.h"

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
//printf("pageFile.Clean()\n"); // TODO
	pageFile.Clean(originalPageSize);
	type 				= InvalidType(0);
	objectSize 			= 0;
	//	pageSize			= 0;
	//	sizeLeft 			= 0;
	//	pageWritePos 		= 0;
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
	if (objectSize == 0){
		ret.parametersError = true;
		REPORT_ERROR(ret,"Type with 0 size");
	}
	if (!(isString || typeIn.IsBasicType())){
		ret.parametersError = !(isString || typeIn.IsBasicType());
		REPORT_ERROR(ret,"Unsupported type. Must be CString or BasicType");
	}
	if (ret && !isString){
		converter 			= TypeConversionManager::Instance().GetOperator(type,ConstCharString(sizeof(CString)),false);
		if (converter == NULL){
			ret.unsupportedFeature = true;
			REPORT_ERROR(ret,"Cannot find type converter");
		}
	}

	if (ret){
		status 				= started;
	} else {
		status 				= error;
	}
	return ret;
}
#if 0
ErrorManagement::ErrorType ProgressiveTypeCreator::CheckMemoryStringEl(uint32 neededSize){
	ErrorManagement::ErrorType ret;

	if (ret){
		ret = pageFile.CheckAndTrimPage(neededSize);
		//		ret = CheckAndClosePage(neededSize);
	}

	// check for initial 0 size page or begin of new vector 0 size page (after CheckAndClosePage)
	if (ret){
		// no memory - allocate
		// could be at the beginning or after a vector has been completed
		// if no more vectors of the same size can be fitted
		ret = pageFile.CheckAndNewPage();
		//		ret = CheckAndNewPage(defaultPageSize);
	}
	return ret;
}

static inline uint32 max(uint32 x,uint32 y){
	uint32 ret = x;
	if (y > x) ret = y;
	return ret;
}

ErrorManagement::ErrorType ProgressiveTypeCreator::CheckMemoryFixedSizeEl(bool newRow){
	ErrorManagement::ErrorType ret;

	uint32 neededSize = vectorSize * objectSize;

	// in case of new vector check if there is space for one based on the size of the previous
	// close page otherwise
	if (ret && newRow){
		ret = pageFile.CheckAndTrimPage(neededSize);

		//		ret = CheckAndClosePage(vectorSize * objectSize);
	}

	// check for initial 0 size page or begin of new vector 0 size page (after CheckAndClosePage)
	if (ret){
		// no memory - allocate
		// could be at the beginning or after a vector has been completed
		// if no more vectors of the same size can be fitted
		ret = pageFile.CheckAndNewPage(max(pageFile.GetDefaultPageSize(),neededSize));
		//		ret = CheckAndNewPage(max(defaultPageSize,vectorSize * objectSize));
	}

	//	uint32 neededSize = objectSize;
	// check memory availability to complete current vector
	if (ret){
		// not enough space
		//		if (neededSize > sizeLeft){
		if (objectSize > pageFile.FreeSizeLeftBytes()){

			// need to allocate a new page and shrink this page?
			// or need to move part of current page into a new page and shrink this page?
			// or need to increase page size

			// there were some other vectors in this page
			// increase so that we can fit also this last one
			uint32 currentVectorUsedSize = (currentVectorSize-1) * objectSize;
			//			if (currentVectorUsedSize <= pageSize){
			if (currentVectorUsedSize <= pageFile.CurrentPageSize()){
				// add enough to store the biggest vector so far
				//				ret = PageGrow(vectorSize * objectSize);
				ret = pageFile.PageGrow(neededSize);

			} else { // means that one vector cannot fit in the page

				// multiply the size
				//				ret = pageFile.PageGrow(defaultPageSize);
				ret = pageFile.PageGrow(pageFile.GetDefaultPageSize());
			}
		}
	}

	return ret;
}
#endif

/**
 * @adds an element. It will convert the string to the specified type
 */
ErrorManagement::ErrorType ProgressiveTypeCreator::AddElement(CCString typeStringRepresentation){
	ErrorManagement::ErrorType ret;

	if (!Started()){
		ret.internalStateError = true;
		REPORT_ERROR(ret,"AddElement and status not started");
	}

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

			// checks memory available in current page
			// if not enough opens a new page
			//				ret = CheckMemoryStringEl(neededSize);

			if (ret){
				MemoryOperationsHelper::Copy(pointer,typeStringRepresentation.GetList(),neededSize);
				//					MemoryOperationsHelper::Copy(page.Address(pageWritePos),typeStringRepresentation.GetList(),neededSize);
			}

		} else {
			// checks memory available in current page
			// if not enough resize
			// in case of new Rows check if space is enough for a row
			// otherwise opens a new page
			//				ret = CheckMemoryFixedSizeEl(newRow);
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
		//			sizeLeft -= neededSize;
		//			pageWritePos += neededSize;
		numberOfElements++;
	} else {
		status = error;
		REPORT_ERROR(ret,"AddElement failed");
	}
	return ret;
}


/**
 * @brief Marks the end of a row of elements
 */
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
		//printf("nEl = %i\n",currentVectorSize);
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

/**
 * @brief Marks the end of the object construction
 */
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

	// shrink if needed
	// if auxSize = 0 or if auxSize bigger than available
	//		ret = CheckAndClosePage(auxSize);

	// if auxSize is not zero
	if (auxSize > 0){

		// new page if needed
		//			ret = CheckAndNewPage(auxSize);

		// get the address map and close up memory pages
		//			if (ret){
		//				auxPtr = reinterpret_cast<void *>(page.Address(this->pageWritePos));
		//				pageWritePos += auxSize;
		//				sizeLeft -= auxSize;
		//				ret = CheckAndClosePage(0);
		//			}

		ret = pageFile.WriteReserveAtomic(auxPtr,auxSize);
		CONDITIONAL_REPORT_ERROR(ret,"WriteReserveAtomic Failed");
	}

	if (ret){
		ret = pageFile.CheckAndTrimPage();
		CONDITIONAL_REPORT_ERROR(ret,"CheckAndTrimPage Failed");
	}


	//		if ((numberOfPages > 0) && ret){
	//			page.FlipOrder();
	//		}

	return ret;
}
#if 0
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
#endif

ErrorManagement::ErrorType ProgressiveTypeCreator::CompleteStringEl(uint8 *&dataPtr,uint32 &auxSize,uint8 *&auxPtr){
	ErrorManagement::ErrorType  ret;
	// table of CCString (pointers)
	uint32 CCStringTableSize = numberOfElements * sizeof(CCString);
	//printf("Strings Pointer Table size= %i\n",CCStringTableSize); //
//printf ("numberOfElements=%i\n",numberOfElements); //TODO

	// table of Vector<CCString>
	if (status == finishedSM){
		auxSize = sizeof (Vector<uint8>) * matrixRowSize;
//printf ("matrixRowSize=%i\n",matrixRowSize); //TODO
	}

	/** try to fit both */
	//		uint32 neededSize = CCStringTableSize + auxSize;

	//		if (ret){
	// close if not big enough
	//			ret = CheckAndClosePage(neededSize);
	//		}

	//		if (ret){
	//			// open if no page
	//			ret = CheckAndNewPage(neededSize);
	//		}

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


	// process memory and create list of pointers
	//		CCString *strings = NULL;
	//		if (ret){
	// remember pointer to string array
	//			strings = reinterpret_cast<CCString *>(page.Address(pageWritePos));
	//printf("Strings Pointer Table base = %p\n",strings); //
	//			if (auxSize > 0){
	//				auxPtr = page.Address(pageWritePos + CCStringTableSize);
	//printf("String Vector Table base = %p\n",auxPtr);   //
	//			}

	// close up memory pages
	//			pageWritePos += neededSize;
	//			sizeLeft -= neededSize;
	//			ret = CheckAndClosePage(0);

	// put oldest page on top
	//			if (numberOfPages > 1){
	//				page.FlipOrder();
	//				numberOfPages = 1;
	//			}
	//		}


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

#if 0
			//printf("String %i address = %p\n",i,s.GetList());  //

			// consecutiveSpan now is how much space is left on page
			//printf("string=%s span = %i\n",s.GetList(),consecutiveSpan);  //
			// empty string error?
			if (s.GetList() == NULL){
				ret.fatalError = true;
				DynamicCString errM;
				errM.AppendN("String ");
				errM.AppendNum(i);
				errM.AppendN(" has zero length");
				REPORT_ERROR(ret,errM.GetList());
			}

			// calculate string length
			uint32 length = consecutiveSpan;
			if (ret){
				// length but do not scan beyond end of
				if (!stringBoundSize(s,length)){
					ret.fatalError = true;
					DynamicCString errM;
					errM.AppendN("String ");
					errM.AppendNum(i);
					errM.AppendN(" exceeding page boundaries ");
					errM.AppendNum(consecutiveSpan);
					REPORT_ERROR(ret,errM.GetList());
				}
			}
			// update pointer to page
			if (ret){
				// include 0
				length = length+1;
				strings[i] = s;
				pageDepth += length;
			}
#endif

		}
	}

	//{//
	//	for (int i = 0;(i<numberOfElements) && ret;i++){
	//		printf("strings[%i] = %p\n",i,strings[i].GetList());
	//	}
	//}

	//if (ret){
	//	dataPtr = reinterpret_cast<uint8 *>(strings);
	//}

	return ret;
}

/**
 * @brief Allows retrieving the Object that has been built.
 * Can only be done after End has been called
 */
ErrorManagement::ErrorType ProgressiveTypeCreator::GetReference(Reference &x){
	ErrorManagement::ErrorType  ret;

	if (!Finished()){
		ret.fatalError = true;
		REPORT_ERROR(ret,"Not Finished");
	}

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
			//				dataPtr =  pageFile.Address(0);
		}
	}

	if (ret){
		// Completes operation
		// fills aux Data
		// creates Object
		//printf("Aux =%p data=%p\n",auxPtr,dataPtr);

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
		//			CheckAndClosePage(0);
	}break;
	case finishedV:{
		//			CheckAndClosePage(0);
		mods.Append('A');
		mods.AppendNum(vectorSize);
	}break;
	case finishedM:{
		// auxPtr not NULL means fragmented
		if (auxPtr != NULL){
printf("AUXPTR !NULL\n");
			if (auxSize != sizeof (void *) * matrixRowSize){
				ret.internalSetupError = true;
				REPORT_ERROR(ret,"auxSize is not adequate");
			}

			//				uint64 pageDepth;
			//				if (ret){
			//					ret = page.Address2Index(dataPtr,pageDepth);
			//				}

			// reorder the pages correctly to allow access to data
			if (ret){
				uint8**addressMap = reinterpret_cast<uint8**>(auxPtr);
				//printf("addressMap = %p\n",addressMap);

				//					uint64 pageDepth=0;
				uint32 vectorByteSize = vectorSize * objectSize;
				//printf("vectorByteSize = %i\n",vectorByteSize);
				for (int i = 0;(i<matrixRowSize) && ret;i++){
					addressMap[i] = pageFile.CurrentReadPointer();

					ret = pageFile.ConsumeReadAtomic(vectorByteSize);
					CONDITIONAL_REPORT_ERROR(ret,"ConsumeReadAtomic failed");

					//						uint32 sizeLeftOnPage = vectorByteSize;
					//						void *address = page.DeepAddress(pageDepth,sizeLeftOnPage);
					//						if (address == NULL){
					//							ret.fatalError = true;
					//							REPORT_ERROR(ret,"deep address out of boundary");
					//						}

					//						if (ret){
					//printf("depth = %lli address = %p %i\n",pageDepth,address,vectorByteSize);
					//							addressMap[i] = address;
					//							pageDepth = pageDepth + vectorByteSize;
					//						}
				}
			}

			if (ret){
				mods.Append('A');
				mods.AppendNum(matrixRowSize);
				mods.AppendN("PA");
				mods.AppendNum(vectorSize);
				dataPtr = auxPtr;
			}

		} else {
			if (matrixRowSize > 1){
				mods.Append('A');
				mods.AppendNum(matrixRowSize);
			}
			if (vectorSize > 1){
				mods.Append('A');
				mods.AppendNum(vectorSize);
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

		//			uint64 pageDepth;
		//			if (ret){
		//				ret = page.Address2Index(dataPtr,pageDepth);
		//printf("Data @ %p pageDepth= %lli\n",dataPtr,pageDepth);
		//printf("Aux  @ %p \n",auxPtr);
		//			}

		// now get the address and then reorder the pages correctly to allow access to data
		if (ret){
			Vector<uint8> *addressMap = reinterpret_cast<Vector<uint8>*>(auxPtr);
			//printf("Vector Table base %p\n",addressMap);
			//printf("aux=%p aMap = %p\n",auxPtr,addressMap);

			//printf("rows = %i\n",matrixRowSize);
			for (int i = 0;(i<matrixRowSize) && ret;i++){
				uint32 size  = sizeStack[i];
				uint32 vectorByteSize = size * objectSize;

				addressMap[i].InitVector(pageFile.CurrentReadPointer(),size);

				ret = pageFile.ConsumeReadAtomic(vectorByteSize);
				CONDITIONAL_REPORT_ERROR(ret,"ConsumeReadAtomic failed");


#if 0
				uint32 sizeLeftOnPage = vectorByteSize;
				uint8 *address = reinterpret_cast<uint8 *> (page.DeepAddress(pageDepth,sizeLeftOnPage));
				if (address == NULL_PTR(uint8 *)){
					ret.fatalError = true;
					DynamicCString errM;
					errM.AppendN("deep address ");
					errM.AppendNum(pageDepth);
					errM.AppendN(" out of boundary");
					REPORT_ERROR(ret,"deep address out of boundary");
				}
				//printf("index = %i size = %i depth = %lli vectorS = %i\n",i,size, pageDepth,vectorByteSize);

				if (ret){
					//printf("Vector %i size %i points to %p\n",i,size,address);
					addressMap[i].InitVector(address,size);
					pageDepth += vectorByteSize;
				}
#endif
			}
		}

		if (ret){
			mods.Append('A');
			mods.AppendNum(matrixRowSize);
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
		//printf("pages = %i size = %i\n",page.NumberOfPages(),pageSize);
		if ((pageFile.NumberOfPages()==1) && (firstElPtr == dataPtr) && (pageSize < 64)){
			if (pageSize <= 4){
				ReferenceT<AnyObjectT<4>> ao(buildNow);
				if (ao.IsValid()){
					ao->Setup(type,mods,dataPtr,pageSize);
					x = ao;
				}
			} else
				if (pageSize <= 8){
					ReferenceT<AnyObjectT<8>> ao(buildNow);
					if (ao.IsValid()){
						ao->Setup(type,mods,dataPtr,pageSize);
						x = ao;
					}
				} else
					if (pageSize <= 16){
						ReferenceT<AnyObjectT<16>> ao(buildNow);
						if (ao.IsValid()){
							ao->Setup(type,mods,dataPtr,pageSize);
							x = ao;
						}
					} else
						if (pageSize <= 32){
							ReferenceT<AnyObjectT<32>> ao(buildNow);
							if (ao.IsValid()){
								ao->Setup(type,mods,dataPtr,pageSize);
								x = ao;
							}
						} else  {
							ReferenceT<AnyObjectT<64>> ao(buildNow);
							if (ao.IsValid()){
								ao->Setup(type,mods,dataPtr,pageSize);
								x = ao;
							}
						}
		}
		else {
			ReferenceT<MemoryPageObject> mpor;

			mpor = ReferenceT<MemoryPageObject> (buildNow);
			if (!mpor.IsValid()){
				ret.outOfMemory = true;
				REPORT_ERROR(ret,"MemoryPageObject construction failed");
			}

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

#if 0
/**
 * @brief Check if the current segment has enough space to store another vector.
 * If there is no need (neededSize== 0) or if the space is not large enough close the current memory page segment and open a new one
 */
ErrorManagement::ErrorType ProgressiveTypeCreator::CheckAndClosePage(uint32 neededSize){
	ErrorManagement::ErrorType ret;
	// do nothing if pageSize == 0
	if (pageSize > 0){
		// check if enough space to store another vector
		if ((sizeLeft < neededSize) || (neededSize == 0)){
			ret = page.Shrink(pageWritePos);
			if (ret){
				//printf("end page to %i\n", pageWritePos);
				pageSize = 0;
				sizeLeft = 0;
				pageWritePos = 0;
			}
		}
	}
	if (!ret){
		DynamicCString errs;
		errs.AppendN("CheckAndClosePage(");
		errs.AppendNum(neededSize);
		errs.Append(')');
		REPORT_ERROR(ret,errs.GetList());
	}
	return ret;
}


/**
 * @brief If pageSize is 0 allocate a new page of desired size
 */
ErrorManagement::ErrorType ProgressiveTypeCreator::CheckAndNewPage(uint32 neededSize){
	ErrorManagement::ErrorType ret;
	// no memory - allocate
	// could be at the beginning or after a vector has been completed
	// if no more vectors of the same size can be fitted
	if (pageSize == 0){
		ret = page.Allocate(neededSize);
		if (ret){
			//printf("new page %i @%p\n", neededSize,page.Address(0));
			sizeLeft = neededSize;
			pageWritePos = 0;
			pageSize = neededSize;
			numberOfPages++;
		}
	}
	if (!ret){
		DynamicCString errs;
		errs.AppendN("CheckAndNewPage(");
		errs.AppendNum(neededSize);
		errs.Append(')');
		REPORT_ERROR(ret,errs.GetList());
	}

	return ret;
}


ErrorManagement::ErrorType ProgressiveTypeCreator::PageGrow(uint32 amount){
	ErrorManagement::ErrorType ret;

	uint32 newPageSize = pageSize + amount;

	// check math overflow!
	if (newPageSize < pageSize){
		ret.fatalError = true;
		REPORT_ERROR(ret,"Overflow");
	}

	if (ret){
		ret = page.Grow(newPageSize);
		if (!ret){
			ret.outOfMemory = true;
			REPORT_ERROR(ret,"Out of Memory");
		}
	}

	if (ret){
		uint32 oldPageSize = pageSize;
		pageSize = page.CurrentPageSize();
		if (pageSize > oldPageSize){
			sizeLeft += (pageSize - oldPageSize);
			//printf("end page to %i\n", pageSize);
		} else {
			ret.fatalError = true;
			REPORT_ERROR(ret,"page.Grow results in a smaller page!");
		}
	}

	return ret;
}
#endif



} //MARTe
