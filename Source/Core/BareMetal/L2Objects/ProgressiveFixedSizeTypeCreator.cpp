/**
 * @file ProgressiveFixedSizeTypeCreator.cpp
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


#include "ProgressiveFixedSizeTypeCreator.h"
#include "TypeConversionManager.h"
#include "GlobalObjectsDatabase.h"

namespace MARTe{



/**
 * @brief a class to contain synthetic objects
 */
class MemoryPageObject: public AnyObjectI{

public:
    CLASS_REGISTER_DECLARATION()


	/**
	 * constructor
	 */
	MemoryPageObject(){	}

	/**
	 * destructor
	 */
	virtual ~MemoryPageObject(){	}

	/**
	 * constructor
	 */
	void Copy(MemoryPage &toStealFrom){
		mp.Copy(toStealFrom);
	}

    /**
     * @brief Allows to setup an Anytype of any type....
     * @param[in] dataDescriptorIn contains the type informations in a TypeDescriptor class.
     * @param[in] modifiers contains the modifiers string in the appropriate language (see VariableDescriptor).
     * @param[in] dataPointerIn is the pointer to the constant data.
     * @post
     *   GetDataPointer() == dataPointerIn &&
     *   GetDataDescriptor() == dataDescriptorIn
     */
    void Setup(TypeDescriptor dataDescriptorIn,CCString modifiers,const void* const dataPointerIn){
    	anyType.Setup(dataDescriptorIn,modifiers,dataPointerIn);
    }

	/**
	 * @brief The only interface provided by an AnyObject is the ability to provide its data via an AnyType.
	 * @return a valid AnyType that describes the content of this object and allows read only access to its content
	 */
	virtual operator AnyType(){
		return anyType;
	}


private:
	/**
	 * contains the memory of the object
	 */
	MemoryPage 	mp;

	/**
	 * the descriptor of the type
	 * and the reference to it
	 */
	AnyType 	anyType;
};

CLASS_REGISTER(MemoryPageObject, "1.0")

/**
 * @brief Creates the object selecting the type to convert to and the default PageSize
 */
ProgressiveFixedSizeTypeCreator::ProgressiveFixedSizeTypeCreator(uint32 pageSizeIn ){
	defaultPageSize 	= pageSizeIn;
	converter 			= NULL;
	Clean();
}

/**
 * @deletes the object and any memory allocated in the pages
 */
ProgressiveFixedSizeTypeCreator::~ProgressiveFixedSizeTypeCreator(){
	delete converter;
}


void ProgressiveFixedSizeTypeCreator::Clean(){
	if (converter != NULL){
		delete converter;
		converter = NULL;
	}
	page.Clean();
	type 				= InvalidType(0);
	objectSize 			= 0;
	pageSize			= 0;
	sizeLeft 			= 0;
	pageWritePos 		= 0;
	status 				= notStarted;
	converter 			= NULL;
	matrixRowSize 		= 0;
	vectorSize 			= 0;
	currentVectorSize 	= 0;
	numberOfPages 		= 0;
	numberOfElements    = 0;
}

ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::Start(TypeDescriptor typeIn){
	ErrorManagement::ErrorType ret;

	Clean();

	type 				= typeIn;
	objectSize 			= typeIn.StorageSize();
	converter 			= TypeConversionManager::Instance().GetOperator(type,ConstCharString(sizeof(CString)),false);
	ret.internalSetupError = (objectSize == 0);
	ret.unsupportedFeature = (converter == NULL);

	if (ret){
		status 				= started;
	} else {
		status 				= error;
		REPORT_ERROR(ret,"Start failed");
	}
	return ret;
}

	/**
	 * @adds an element. It will convert the string to the specified type
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::AddElement(CCString typeStringRepresentation){
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

//printf ("< %i %i\n",pageWritePos,numberOfElements);

		// in case of new vector check if there is space for one based on the size of the previous
		// close page otherwise
		if (ret && newRow){
//printf("CheckAndClosePage %i %i\n",vectorSize,objectSize);
			ret = CheckAndClosePage(vectorSize * objectSize);
		}
//printf ("< %i %i\n",pageWritePos,numberOfElements);

		// check for initial 0 size page or begin of new vector 0 size page (after CheckAndClosePage)
		if (ret){
			// no memory - allocate
			// could be at the beginning or after a vector has been completed
			// if no more vectors of the same size can be fitted
//printf("CheckAndNewPage %i\n",defaultPageSize);
			ret = CheckAndNewPage(defaultPageSize);
		}

		uint32 neededSize = objectSize;
		// check memory availability to complete current vector
		if (ret){
			// not enough space
			if (neededSize > sizeLeft){
printf("neededSize > sizeLeft %i %i \n",neededSize,sizeLeft);

				// need to allocate a new page and shrink this page?
				// or need to move part of current page into a new page and shrink this page?
				// or need to increase page size

				// there were some other vectors in this page
				// increase so that we can fit also this last one
				uint32 currentVectorUsedSize = (currentVectorSize-1) * objectSize;
				if (currentVectorUsedSize <= pageSize){
					// add enough to store the biggest vector so far
					uint32 vectorUsedSize = vectorSize * objectSize;
					uint32 newPageSize = pageSize + vectorUsedSize;

					if (newPageSize < pageSize){
						ret.outOfMemory = true;
						REPORT_ERROR(ret,"Out of Memory");
					}

					if (ret){
						ret = page.Grow(newPageSize);
					}

					if (ret){
						sizeLeft += vectorUsedSize;
						pageSize = newPageSize;
					}
				} else { // means that one vector cannot fit in the page

					uint32 newPageSize = pageSize + defaultPageSize;
					// check math overflow!
					if (newPageSize < pageSize){
						ret.outOfMemory = true;
						REPORT_ERROR(ret,"Out of Memory");
					}

					if (ret){
						ret = page.Grow(newPageSize);
					}

					if (ret){
						sizeLeft +=  pageSize;
						pageSize =  newPageSize;
					}
				}
			}
		}

		if (ret){
			// queue type to memory
			ret = converter->Convert(	reinterpret_cast<uint8 *>(page.Address(pageWritePos)),
								reinterpret_cast<const uint8 *>(&typeStringRepresentation),1);
			if (ret){
				sizeLeft -= neededSize;
				pageWritePos += neededSize;
				numberOfElements++;

//printf ("> %i %i\n",pageWritePos,numberOfElements);
			}
		}

		if (!ret.ErrorsCleared()){
			status = error;
			REPORT_ERROR(ret,"AddElement failed");
		}
		return ret;
	}

	/**
	 * @brief Marks the end of a row of elements
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::EndVector(){
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
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::End(){
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

	/**
	 * @brief Allows retrieving the Object that has been built.
	 * Can only be done after End has been called
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::GetReference(ReferenceT<AnyObjectI> &x){
		ErrorManagement::ErrorType  ret;

		if (!Finished()){
			ret.fatalError = true;
			REPORT_ERROR(ret,"Not Finished");
		}

		void *auxPtr  = NULL;
		uint32 auxSize = 0;

		if (ret){

			if (status == finishedSM){
				auxSize  = sizeof (Vector<uint8>) * matrixRowSize;
printf("SM\n");
			} else
			if ((status == finishedM)&&(numberOfPages > 1)){
printf("MP numberOfPages = %i\n",numberOfPages);
				auxSize = sizeof (void *) * matrixRowSize;
			}

			if (auxSize > 0){
printf("auxSize = %i\n",auxSize);
				// shrink if needed
				ret = CheckAndClosePage(auxSize);

				// new page if needed
				if (ret){
					ret = CheckAndNewPage(auxSize);
				}

				// get the address map and close up memory pages
				if (ret){
					auxPtr = reinterpret_cast<void *>(page.Address(this->pageWritePos));
					pageWritePos += auxSize;
					sizeLeft -= auxSize;
					ret = CheckAndClosePage(0);
				}
			}
		}

		if ((numberOfPages > 0) && ret){
			page.FlipOrder();
		}

		if (ret){
			void *dataPtr =  page.Address(0);
			ret = GetReferencePrivate(x, dataPtr, auxPtr,auxSize);

		} else {
			REPORT_ERROR(ret,"GetReference failed");
		}

		return ret;
	}

	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::GetReferencePrivate(ReferenceT<AnyObjectI> &x, void *dataPtr, void *auxPtr,uint32 auxSize){
		ErrorManagement::ErrorType  ret;

		DynamicCString mods;
		switch (status){
		case finishedS:{
			CheckAndClosePage(0);
		}break;
		case finishedV:{
			CheckAndClosePage(0);
			mods.Append('A');
			mods.AppendNum(vectorSize);
		}break;
		case finishedM:{
			// auzPtr not NULL means fragmented
			if (auxPtr != NULL){
				if (auxSize != sizeof (void *) * matrixRowSize){
					ret.internalSetupError = true;
					REPORT_ERROR(ret,"auxSize is not adequate");
				}

				// reorder the pages correctly to allow access to data
				if (ret){
					void **addressMap = reinterpret_cast<void **>(auxPtr);
printf("addressMap = %p\n",addressMap);

					uint64 pageDepth=0;
					uint32 vectorByteSize = vectorSize * objectSize;
printf("vectorByteSize = %i\n",vectorByteSize);
					for (int i = 0;(i<matrixRowSize) && ret;i++){
						uint32 sizeLeftOnPage = vectorByteSize;
						void *address = page.DeepAddress(pageDepth,sizeLeftOnPage);
						if (address == NULL){
							ret.fatalError = true;
							REPORT_ERROR(ret,"deep address out of boundary");
						}

						if (ret){
printf("depth = %lli address = %p %i\n",pageDepth,address,vectorByteSize);
							addressMap[i] = address;
							pageDepth = pageDepth + vectorByteSize;
						}
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
				mods.Append('A');
				mods.AppendNum(matrixRowSize);
				mods.Append('A');
				mods.AppendNum(vectorSize);
			}
		}break;
		case finishedSM:{
			if ((auxSize != (sizeof (Vector<uint8>) * matrixRowSize)) || (auxPtr == NULL)){
				ret.internalSetupError = true;
				REPORT_ERROR(ret,"auxSize is not adequate or auxPtr is NULL");
			}

			// now get the address and then reorder the pages correctly to allow access to data
			if (ret){
				Vector<uint8> *addressMap = reinterpret_cast<Vector<uint8>*>(auxPtr);

				uint64 pageDepth=0;
				for (int i = 0;(i<matrixRowSize) && ret;i++){
					uint32 vectorByteSize = sizeStack[i] * objectSize;
					uint32 sizeLeftOnPage = vectorByteSize;
					uint8 *address = reinterpret_cast<uint8 *> (page.DeepAddress(pageDepth,sizeLeftOnPage));
					if (address == NULL_PTR(uint8 *)){
						ret.fatalError = true;
						REPORT_ERROR(ret,"deep address out of boundary");
					}

					if (ret){
						addressMap[i].Set(address,sizeStack[i]);
						pageDepth += vectorByteSize;
					}
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

		ReferenceT<MemoryPageObject> mpor;
		if (ret){
			//ReferenceT<MemoryPageObject> mpor2(buildNow);
			mpor = ReferenceT<MemoryPageObject> (buildNow);
			if (!mpor.IsValid()){
				ret.outOfMemory = true;
				REPORT_ERROR(ret,"MemoryPageObject construction failed");
			}
		}

		if (ret){
printf("data Ptr = %p\n",dataPtr);
			mpor->Setup(type,mods,dataPtr);
			mpor->Copy(page);
			x = mpor;

			status = notStarted;
		} else {
			REPORT_ERROR(ret,"GetReferencePrivate failed");
		}
		return ret;
	}


	/**
	 * @brief Check if the current segment has enough space to store another vector.
	 * If there is no need (neededSize== 0) or if the space is not large enough close the current memory page segment and open a new one
	 */
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CheckAndClosePage(uint32 neededSize){
		ErrorManagement::ErrorType ret;
		// do nothing if pageSize == 0
		if (pageSize > 0){
			// check if enough space to store another vector
			if ((sizeLeft < neededSize) || (neededSize == 0)){
				ret = page.Shrink(pageWritePos);
				if (ret){
printf("end page to %i\n", pageWritePos);
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
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CheckAndNewPage(uint32 neededSize){
		ErrorManagement::ErrorType ret;
		// no memory - allocate
		// could be at the beginning or after a vector has been completed
		// if no more vectors of the same size can be fitted
		if (pageSize == 0){
			ret = page.Allocate(neededSize);
			if (ret){
printf("new page %i\n", neededSize);
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
#if 0
	ErrorManagement::ErrorType ProgressiveFixedSizeTypeCreator::CheckAndRenewPage(uint32 neededSize,uint32 newPageSize){
		ErrorManagement::ErrorType ret;
		// do nothing if pageSize == 0
		if (pageSize > 0){
			// check if enough space to store another vector
			if ((sizeLeft < neededSize) || (neededSize == 0)){
				ret = page.Shrink(pageWritePos);
			}
			if (ret){
				pageSize = 0;
				sizeLeft = 0;
				pageWritePos = 0;
			}
		}

		// no memory - allocate
		// could be at the beginning or after a vector has been completed
		// if no more vectors of the same size can be fitted
		if (ret && (pageSize==0)){
			ret = page.Allocate(newPageSize);
			if (ret){
				sizeLeft = newPageSize;
				pageWritePos = 0;
				pageSize = newPageSize;
				numberOfPages++;
			}
		}
		if (!ret){
			DynamicCString errs;
			errs.AppendN("CheckAndRenewPage(");
			errs.AppendNum(neededSize);
			errs.Append(',');
			errs.AppendNum(newPageSize);
			errs.Append(')');
			REPORT_ERROR(ret,errs.GetList());
		}
		return ret;
	}
#endif

	/**
	 * Header used in each page
	 */
	struct MemoryPageHeader{
		/**
		 * Link to previous/next
		 */
		MemoryPageHeader * 	previous;
		/**
		 * page size (payload only - no header)
		 */
		uint32				pageSize;
		/**
		 * Allow access to the payload section
		 */
		uint8 				*Data(){
			MemoryPageHeader *ptr = this +1;
			return reinterpret_cast<uint8 *>(ptr);
		};
		/**
		 * individual access to each member of payload
		 * range checks not performed
		 */
		uint8 &operator[] (uint32 index){
			return Data()[index];
		}
	};


	/**
	 * basic constructor empty pages
	 */
	MemoryPage::MemoryPage(){
		mph = NULL_PTR(MemoryPageHeader *);
	}
	/**
	 * @brief Steals content from another MemoryPage
	 */
	void MemoryPage::Copy (MemoryPage &stealFrom){
		mph = stealFrom.mph;
		stealFrom.mph = NULL_PTR(MemoryPageHeader *);
	}
	/**
	 * @brief Deletes all pages of memory
	 */
	MemoryPage::~MemoryPage(){
		Clean();
	}

	void MemoryPage::Clean(){
		while (mph != NULL_PTR(MemoryPageHeader *)){
			MemoryPageHeader * memoryP = mph->previous;
			free(mph);
			mph = memoryP;
		}
	}

	uint32 MemoryPage::Size(){
		uint32 size = 0;
		if (mph != NULL) {
			size = mph->pageSize;
		}
		return size;
	}

	/**
	 * @brief return address of the element at the deep address index
	 * and checks that the elements from that point to that + span are contiguous
	 * returns NULL if outside range or not contiguous span
	 */
	void *MemoryPage::DeepAddress(uint64 index,uint32 &consecutiveSpan){
		MemoryPageHeader *mphp = mph;
		uint32 localIndex = 0;
		while ((index > 0) && (mphp != NULL_PTR(MemoryPageHeader *))){
			if (index > mphp->pageSize){
				index -= mphp->pageSize;
				mphp = mphp->previous;
			} else {
				localIndex  = index;
				consecutiveSpan = mphp->pageSize - index;
				index = 0;
			}
		}

		void *address = NULL;
		if (mphp != NULL){
			address = reinterpret_cast<void *>((mphp->Data()+localIndex));
		}
		return address;
	}


	/**
	 * @brief return address of element index
	 */
	void *MemoryPage::Address(uint32 index){
		return reinterpret_cast<void *> (mph->Data()+index);
	}


	/**
	 * @brief increases current page to size newBufferSize
	 */
	ErrorManagement::ErrorType MemoryPage::Grow(uint32 newBufferSize){
		ErrorManagement::ErrorType ret;

		if (mph != NULL_PTR(MemoryPageHeader *)){
			// should grow not shrink
			if (newBufferSize <= mph->pageSize){
				ret.parametersError = true;
				REPORT_ERROR(ret,"Grow to a smaller size??");
			}
		}

		MemoryPageHeader *newMemory = NULL_PTR(MemoryPageHeader *);
		if (ret){
			// failure to allocate new page
			newMemory = reinterpret_cast<MemoryPageHeader*>(realloc(mph, newBufferSize + sizeof (MemoryPageHeader)));
			if (newMemory == NULL_PTR(MemoryPageHeader *)){
				ret.outOfMemory = true;
				REPORT_ERROR(ret,"realloc failed");
			}
		}

		if (ret){
			mph = newMemory;
			mph->pageSize = newBufferSize;
		}
		return ret;
	}

	/**
	 * @brief decreases current page to size newBufferSize
	 */
	ErrorManagement::ErrorType MemoryPage::Shrink(uint32 newBufferSize){
		ErrorManagement::ErrorType ret;

		if (mph != NULL_PTR(MemoryPageHeader *)){
			// should shrink not grow
			if (newBufferSize > mph->pageSize){
				ret.parametersError = true;
				DynamicCString errMsg;
				errMsg.AppendN("Shrink to a bigger size??:new= ");
				errMsg.AppendNum(newBufferSize);
				errMsg.AppendN(" old= ");
				errMsg.AppendNum(mph->pageSize);
				REPORT_ERROR(ret,errMsg.GetList());
			}

		}
		MemoryPageHeader *newMemory = NULL_PTR(MemoryPageHeader *);
		if (ret){
			// failure to allocate new page
			newMemory = reinterpret_cast<MemoryPageHeader*>(realloc(mph, newBufferSize + sizeof (MemoryPageHeader)));
			if (newMemory == NULL_PTR(MemoryPageHeader *)){
				ret.outOfMemory = true;
				REPORT_ERROR(ret,"realloc failed");
			}
		}

		if (ret){
			mph = newMemory;
			mph->pageSize = newBufferSize;
		}
		return ret;
	}

	/**
	 *  @brief allocate a new page
	 */
	ErrorManagement::ErrorType  MemoryPage::Allocate(uint32 size){
		ErrorManagement::ErrorType ret;
		MemoryPageHeader *newMemory;
		newMemory = reinterpret_cast<MemoryPageHeader *>(malloc(size+sizeof (MemoryPageHeader)));

		if (newMemory == NULL_PTR(MemoryPageHeader *)){
			ret.outOfMemory = true;
			REPORT_ERROR(ret,"malloc failed");
		}

		if (ret){
			newMemory->pageSize = size;
			newMemory->previous = mph;
			mph = newMemory;
		}
		return ret;
	}

	// last operation before closing the use of this memory
	// change previous to next so that the memory can be parsed in the right order
	void MemoryPage::FlipOrder(){
		MemoryPageHeader *ptr = mph->previous;
		mph->previous = NULL;
		while (ptr != NULL){
			MemoryPageHeader *ptr2 = ptr->previous;
			ptr->previous = mph;
			mph = ptr;
			ptr = ptr2;
		}
	}


} //MARTe
