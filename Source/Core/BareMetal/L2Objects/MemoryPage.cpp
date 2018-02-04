/**
 * @file MemoryPage.cpp
 * @brief Header file for class AnyType
 * @date 3 Feb 2018
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

#include "MemoryPage.h"
#include "DynamicCString.h"

namespace MARTe{

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

uint32 MemoryPage::CurrentPageSize(){
	uint32 size = 0;
	if (mph != NULL) {
		size = mph->pageSize;
	}
	return size;
}

uint32 MemoryPage::NumberOfPages(){
	uint32 n = 0;
	MemoryPageHeader * memoryP = mph;
	while (memoryP != NULL){
		n++;
		memoryP = memoryP->previous;
	}
	return n;
}


ErrorManagement::ErrorType MemoryPage::Address2Index(void * address,uint64 &index) const{
	ErrorManagement::ErrorType ret;

	uint8 *addressU8 = reinterpret_cast<uint8 *>(address);
	index = 0;
	bool found = false;
	MemoryPageHeader *mphp = mph;
	while (!found && (mphp != NULL_PTR(MemoryPageHeader *))){
		uint8 *start = mphp->Data();
		uint8 *end   = mphp->Data() + mphp->pageSize;
		if ((addressU8 >= start) && (addressU8 <end)){
			found = true;
			index += (addressU8 - start);
		} else {
			index += mphp->pageSize;
			mphp = mphp->previous;
		}
	}

	if (!found){
		ret.outOfRange = true;
		REPORT_ERROR(ret,"address out of range");
	}

	return ret;
}


void *MemoryPage::DeepAddress(uint64 index,uint32 &consecutiveSpan){
//printf("%index = lli index\n",index);
	MemoryPageHeader *mphp = mph;

	uint32 localIndex = 0;
	while ((index > 0) && (mphp != NULL_PTR(MemoryPageHeader *))){
		if (index >= mphp->pageSize) {
			index -= mphp->pageSize;
			mphp = mphp->previous;
		} else {
			localIndex  = index;
			index = 0;
		}
	}

	void *address = NULL;
	if (mphp != NULL){
		uint32 requiredSpan = consecutiveSpan;
		consecutiveSpan = mphp->pageSize - localIndex;
		if (requiredSpan <= consecutiveSpan){
			address = reinterpret_cast<void *>((mphp->Data()+localIndex));
		}
//printf("%p req=%i avail=%i pos=%i\n",address, requiredSpan, consecutiveSpan,localIndex);
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
//printf("malloc %i @%p\n", size,newMemory);

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
