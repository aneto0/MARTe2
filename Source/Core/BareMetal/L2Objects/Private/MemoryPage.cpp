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

#include "CompositeErrorManagement.h"
#include "DynamicCString.h"
#include "MemoryPageFile.h"

#include <stdlib.h>  //TODO

namespace MARTe{


MemoryPage::MemoryPage(){
	firstPage       		= NULL_PTR(MemoryPageHeader *);
}

MemoryPage::~MemoryPage(){
	Clean();
}

ErrorManagement::ErrorType 	MemoryPage::StealAndJoinAtEnd  (MemoryPage &stealFrom){
	ErrorManagement::ErrorType ret;
	if (stealFrom.firstPage != NULL){
		if (firstPage == NULL){
			firstPage = stealFrom.firstPage;
		} else {
			MemoryPageHeader *p = firstPage;
			while (p->next != NULL){
				p = p->next;
			}
			p->next = stealFrom.firstPage;
		}
		stealFrom.firstPage = NULL;
	} else {
		ret.parametersError = true;
	}
	return ret;
}

void MemoryPage::Clean(){
	while (firstPage != NULL_PTR(MemoryPageHeader *)){
		MemoryPageHeader * memoryP = firstPage->next;
		::free(firstPage);
		firstPage = memoryP;
	}
}

MemoryPageFile::MemoryPageFile(uint32 defaultPageSizeIn){
	numberOfPages 			= 0;
	pageWritePos  			= 0;
	pageReadPos  			= 0;
	currentWritePage 		= NULL_PTR(MemoryPageHeader *);
	currentReadPage 		= NULL_PTR(MemoryPageHeader *);
	currentPageStartIndex 	= 0;
	defaultPageSize 		= defaultPageSizeIn;
}

MemoryPageFile::~MemoryPageFile(){
	Clean();
}

void MemoryPageFile::Clean(uint32 defaultPageSizeIn){
//printf("clean(CWP=%p,CRP=%p,FP=%p,DPZ=%i)\n",currentWritePage,currentReadPage,firstPage,defaultPageSize); // TODO
	MemoryPage::Clean();
	numberOfPages 			= 0;
	pageWritePos  			= 0;
	pageReadPos  			= 0;
	currentWritePage 		= NULL_PTR(MemoryPageHeader *);
	currentReadPage 		= NULL_PTR(MemoryPageHeader *);
	currentPageStartIndex 	= 0;
	if (defaultPageSizeIn != 0){
		defaultPageSize			= defaultPageSizeIn;
	}
//printf("clean(CWP=%p,CRP=%p,FP=%p,DPZ=%i)\n",currentWritePage,currentReadPage,firstPage,defaultPageSize); // TODO
}


uint32 MemoryPageFile::FreeSizeLeftBytes(){
	uint32 pageSize = CurrentPageSize();
	uint32 freeSize = 0;
	if (pageWritePos < pageSize){
		freeSize = pageSize - pageWritePos;
	}
	return freeSize;
}

/**
 *  @brief allocate a new page
 */
ErrorManagement::ErrorType  MemoryPageFile::Allocate(uint32 size){
	ErrorManagement::ErrorType ret;
	MemoryPageHeader *newPage;
	newPage = reinterpret_cast<MemoryPageHeader *>(::malloc(size+sizeof (MemoryPageHeader)));

	ret.outOfMemory = (newPage == NULL_PTR(MemoryPageHeader *));
	COMPOSITE_REPORT_ERROR(ret,"Allocate(",size,") failed");

	if (ret){
		numberOfPages++;
		pageWritePos = 0;

		newPage->pageSize = size;
		newPage->next = NULL;
		if (currentWritePage != NULL){
			currentWritePage->next = newPage;
			currentPageStartIndex += currentWritePage->pageSize;
		} else {
			firstPage = newPage;
			currentReadPage = newPage;
		}
		currentWritePage = newPage;
	}
	return ret;
}

ErrorManagement::ErrorType  MemoryPageFile::ReSize(uint32 newSize){
	ErrorManagement::ErrorType ret;
	MemoryPageHeader *newPage = NULL;

	ret.invalidOperation = (currentWritePage == NULL);
	REPORT_ERROR(ret,"Cannot ReSize an empty page");

	uint32 actualSize = newSize + sizeof (MemoryPageHeader);

	if (actualSize < newSize){
		ret.fatalError = true;
		REPORT_ERROR(ret,"Math Overflow");
	}

	if (ret){
		newPage = reinterpret_cast<MemoryPageHeader *>(::realloc(reinterpret_cast<MemoryPageHeader *>(currentWritePage),actualSize));

		ret.outOfMemory = (newPage == NULL_PTR(MemoryPageHeader *));
		COMPOSITE_REPORT_ERROR(ret,"Realloc(",actualSize,") failed");

		if (ret){
			newPage->pageSize = newSize;
		}
	}
//printf("realloc(%p->%p,%i)\n",currentWritePage,newPage,actualSize); // TODO
	// replace newPage to currentPage in the list
	// currentPage is now invalid
	if (ret){
		if (newPage != currentWritePage){
			// just one page
			if (firstPage == currentWritePage){
				firstPage 			= newPage;
				currentWritePage 	= newPage;
				currentReadPage 	= newPage;
			} else {
				// more than one
				MemoryPageHeader *page = firstPage;
				while (page != NULL) {
					if (page->next == currentWritePage){
						page->next = newPage;
						currentWritePage = newPage;
						newPage = NULL;
						page = NULL;
					} else {
						page = page->next;
					}
				}
				ret.internalSetupError = (newPage != NULL);
				REPORT_ERROR(ret,"currentPage was not linked to firstPage");
			}
		}
	}
	return ret;

}


ErrorManagement::ErrorType MemoryPageFile::CheckAndTrimPage(uint32 neededSize){
	ErrorManagement::ErrorType ret;
	uint32 pageSize = CurrentPageSize();
	// do nothing if pageSize == 0
	if ( pageSize > 0){
		// check if enough space to store another vector
		// or just trim the fat
		// if neededSize > FreeSizeLeftBytes but pageWritePos == 0 do nothing
		// memory will be expanded by next call of  WriteAtomic
		if (((FreeSizeLeftBytes() < neededSize) && (pageWritePos != 0)) || (neededSize == 0)){
			ret = ReSize(pageWritePos);
		}
	}
	COMPOSITE_REPORT_ERROR(ret,"CheckAndClosePage(",neededSize,") failed");

	return ret;
}

/**
 * @brief If FreeSizeLeftBytes() is 0 allocate a new page of desired size
 */
ErrorManagement::ErrorType MemoryPageFile::CheckAndNewPage(uint32 newPageSize){
	ErrorManagement::ErrorType ret;
	// no memory - allocate
	// could be at the beginning or after a vector has been completed
	// if no more vectors of the same size can be fitted
//printf("CheckAndNewPage FSLB=%i CPSZ=%i NPSZ=%i DPSZ=%i \n",FreeSizeLeftBytes(),CurrentPageSize(),newPageSize,defaultPageSize); // TODO
	if (FreeSizeLeftBytes() == 0){
		if (newPageSize==0){
			newPageSize = defaultPageSize;
		}
		ret = Allocate(newPageSize);
	}
//printf("CheckAndNewPage2 CPSZ=%i NPSZ=%i DPSZ=%i \n",CurrentPageSize(),newPageSize,defaultPageSize); // TODO
	COMPOSITE_REPORT_ERROR(ret,"CheckAndNewPage(",newPageSize,')');
	return ret;
}

ErrorManagement::ErrorType MemoryPageFile::PageGrow(uint32 amount){

	ErrorManagement::ErrorType ret;

	// check math overflow!
	uint32 newPageSize = CurrentPageSize() + amount;
	ret.fatalError =  (newPageSize < CurrentPageSize());
	REPORT_ERROR(ret,"Math Overflow");

	if (ret){
		ret = ReSize(newPageSize);
		REPORT_ERROR(ret,"PageGrow failed");
	}

	return ret;
}


ErrorManagement::ErrorType MemoryPageFile::WriteReserveAtomic( uint8 *&reservedBuffer, uint32 numberOfBytes){
	ErrorManagement::ErrorType ret;

	if (FreeSizeLeftBytes() < numberOfBytes){
		if (defaultPageSize < numberOfBytes){
			defaultPageSize = numberOfBytes;
		}
		if (pageWritePos > 0){
			ret = ReSize(pageWritePos);
			REPORT_ERROR(ret,"Shrink Failed");
		}
		if (ret){
			ret = Allocate(defaultPageSize);
		}
	}

	if (ret){
		reservedBuffer = reinterpret_cast<uint8 *> (currentWritePage->Data()+pageWritePos);
		pageWritePos += numberOfBytes;
	}

	REPORT_ERROR(ret,"ReserveAtomic failed");
	return ret;
}

ErrorManagement::ErrorType  MemoryPageFile::WriteReserveExtended(uint8 *&reservedBuffer, uint32 numberOfBytes){
	ErrorManagement::ErrorType ret;
	if (FreeSizeLeftBytes() < numberOfBytes){
		uint32 newPageSize = CurrentPageSize() +  defaultPageSize/2 + numberOfBytes;
		if (defaultPageSize < newPageSize){
			defaultPageSize = newPageSize;
		}
		if (newPageSize < CurrentPageSize()){
			ret.outOfRange = true;
			REPORT_ERROR(ret,"defaultPageSize growing failed");
		}
		if (ret){
			if (CurrentPageSize()==0){
				ret = Allocate(defaultPageSize);
			} else {
//printf("CPSZ=%i NPSZ=%i DPSZ=%i NOB=%o\n",CurrentPageSize(),newPageSize,defaultPageSize,numberOfBytes); // TODO
				ret = ReSize(newPageSize);
				REPORT_ERROR(ret,"Page Growth Failed");
			}
		}
	}

	if (ret){
//printf("FSLB=%i CPSZ=%i DPSZ=%i\n",FreeSizeLeftBytes(),CurrentPageSize(),defaultPageSize); // TODO
		if (FreeSizeLeftBytes() >= numberOfBytes){
			reservedBuffer = reinterpret_cast<uint8 *> (currentWritePage->Data()+pageWritePos);
			pageWritePos += numberOfBytes;
		} else {
			ret.fatalError = true;
			REPORT_ERROR(ret,"Unexpected Error");
		}
	}

	return ret;
}

ErrorManagement::ErrorType  MemoryPageFile::ConsumeReadAtomic(uint32 numberOfBytes){
	ErrorManagement::ErrorType ret;

	if (currentReadPage == NULL){
		ret.completed = true;
		REPORT_ERROR(ret,"End of File Reached");
	}

	if (ret){
		if (numberOfBytes > 0){
			uint32 pageEndPos = pageReadPos + numberOfBytes;

			if (pageEndPos < pageReadPos){
				ret.outOfRange = true;
				REPORT_ERROR(ret,"Numeric OverFlow");
			}

			if (ret){
				if (pageEndPos > currentReadPage->pageSize){
					ret.outOfRange = true;
					REPORT_ERROR(ret,"Page Boundary OverFlow");
				} else
				if (pageEndPos == currentReadPage->pageSize){
					currentReadPage = currentReadPage->next;
					pageReadPos = 0;
				} else {
					pageReadPos = pageEndPos;
				}
			}
		} else {
			uint8 *p = currentReadPage->Data();
			uint32 limit = currentReadPage->pageSize;

			while ((pageReadPos < limit) && (p[pageReadPos] != 0)){
				pageReadPos++;
			}

			if (p[pageReadPos] != 0){
				ret.outOfRange = true;
				REPORT_ERROR(ret,"Page Boundary OverFlow");
			} else {
				pageReadPos++;
			}

			if (ret){
				if (pageReadPos == limit){
					pageReadPos = 0;
					currentReadPage = currentReadPage->next;
				}
			}
		}
	}

	REPORT_ERROR(ret,"ConsumeReadAtomic failed");
	return ret;
}

uint8 *MemoryPageFile::Address(uint64 index){
	MemoryPageHeader *mphp = firstPage;

	uint64 localIndex = 0;
	while ((index > 0) && (mphp != NULL_PTR(MemoryPageHeader *))){
		if (index >= mphp->pageSize) {
			index -= mphp->pageSize;
			mphp = mphp->next;
		} else {
			localIndex  = index;
			index = 0;
		}
	}

	uint8 *address = NULL;
	if (mphp != NULL){
		address = (mphp->Data()+localIndex);
	}
	return address;
}

} //MARTe
