/**
 * @file MemoryPageFile.h
 * @brief Header file for class AnyType
 * @date 15 Feb 2018
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

#ifndef MEMORYPAGEFILE_H_
#define MEMORYPAGEFILE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MemoryPage.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


namespace MARTe{

/**
 * allows Sequential Write to a MemoryPage
 */
class MemoryPageFile: public MemoryPage{

public:

	/**
	 *
	 */
								MemoryPageFile(uint32 defaultPageSizeIn);

	/**
	 *
	 */
								~MemoryPageFile();

	/**
    * @brief Deletes all pages of memory and resets defaultPageSize
    */
	void 						Clean(uint32 defaultPageSizeIn=0);


	/**
	 * @brief Check if the current segment has enough space to store more data of size neededSize.
	 * If there is no need (neededSize== 0) or if the space is not large enough and the page is not new
	 * close the current memory page segment
	 */
	ErrorManagement::ErrorType 	CheckAndTrimPage(uint32 neededSize=0);

	/**
	 * @brief If FreeSizeLeftBytes is 0 allocate a new page of desired size
	 * if newPageSize==0 ==> newPageSize = defaultPageSize
	 */
	ErrorManagement::ErrorType 	CheckAndNewPage(uint32 newPageSize=0);

	/**
	 *  @brief allocate a new page
	 */
	ErrorManagement::ErrorType  Allocate(uint32 size);

	/**
	 *  @brief resize current page
	 */
	ErrorManagement::ErrorType  ReSize(uint32 newSize);

	/**
	 *	@brief Calls Resize and increase page size by amount
	 */
	ErrorManagement::ErrorType 	PageGrow(uint32 amount);

	/**
	 * @brief How many pages in this PageFile
	 */
	inline uint32 				NumberOfPages();

	/**
	 *	@brief  Size of current write page
	 */
	inline uint32 				CurrentPageSize();

	/**
	 * @brief How many bytes left in the current write page
	 */
	uint32 						FreeSizeLeftBytes();

	/**
	 *	@brief reserves a section on the file all on the same page
	 *	Allocate new page if necessary. Uses a larger page size if necessary
	 */
	ErrorManagement::ErrorType  WriteReserveAtomic(uint8 *&reservedBuffer, uint32 numberOfBytes);

	/**
	 *	@brief reserves a section on the file all on the current page
	 *	Extend page size if necessary by permanently increasing defaultPageSize
	 */
	ErrorManagement::ErrorType  WriteReserveExtended(uint8 *&reservedBuffer, uint32 numberOfBytes);

	/**
	 *	@brief marks a section on the file as read. Moves readPage. If readPage is NULL initialises it to firstPage
	 *	If numberOfBytes == 0 consumes up to the byte 0. Otherwise consumes numberOfBytes
	 *	Returns error Overflow if the data consumed spans across two pages
	 */
	ErrorManagement::ErrorType  ConsumeReadAtomic(uint32 numberOfBytes);

	/**
	 * @brief memory location corresponding to pageReadPos
	 */
	inline uint8 * 				CurrentReadPointer();

	/**
	 * @brief current value of defaultPageSize
	 */
	inline uint32 				GetDefaultPageSize();

	/**
	 * @brief return address of the element at the address index
	 * in consecutiveSpan returns the number of consecutive bytes available
	 * returns NULL if outside range
	 */
	uint8 *						Address(uint64 index=0);
private:

	/**
	 *
	 */
	uint32 				pageWritePos;

	/**
	 *
	 */
	uint32 				pageReadPos;

	/**
	 * 	@brief sum of the sizes of all pages before this
	 */
	uint64 				currentPageStartIndex;

	/**
	 * local copy as the function on MemoryPage requires walking the stack of pages
	 */
	uint32				numberOfPages;

	/**
	 *
	 */
	uint32 				defaultPageSize;

	/**
	 *	@brief the top of the list of pages
	 */
	MemoryPageHeader *	currentReadPage;

	/**
	 * 	@brief Last added page. Here the writing goes
	 */
	MemoryPageHeader *  currentWritePage;

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#include "DynamicCString.h"

uint32 MemoryPageFile::GetDefaultPageSize(){
	return defaultPageSize;
}

uint32 	MemoryPageFile::NumberOfPages(){
	return numberOfPages;
}

uint32 MemoryPageFile::CurrentPageSize(){
	uint32 size = 0;
	if (currentWritePage != NULL) {
		size = currentWritePage->pageSize;
	}
	return size;
}

uint8 * MemoryPageFile::CurrentReadPointer(){
	uint8 *ptr = NULL;
	if (currentReadPage != NULL){
		ptr = currentReadPage->Data()+pageReadPos;
	}
	return ptr;
}




} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_MEMORYPAGEFILE_H_ */
