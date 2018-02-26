/**
 * @file MemoryPage.h
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

#ifndef MEMORYPAGE_H_
#define MEMORYPAGE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


/**
 * Header used in each page
 */
struct MemoryPageHeader{
	/**
	 * Link to previous/next
	 */
	MemoryPageHeader * 	next;
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

class MemoryPage{
public:

	/**
	 *
	 */
								MemoryPage();

	/**
	 *
	 */
								~MemoryPage();

	/**
	 * @brief Deletes all pages of memory
	 */
	void 						Clean();


	/**
	* @brief Steals content from another MemoryPage and joins it at the end
	*/
	ErrorManagement::ErrorType 	StealAndJoinAtEnd  (MemoryPage &stealFrom);


protected:
	/**
	 *	@brief the top of the list of pages
	 */
	MemoryPageHeader *	firstPage;
};


#if 0

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
 * A set of memory pages linked together.
 * To be used by an application to load a memory structure into memory
 * Object destruction deletes all pages
 */
class MemoryPage{

public:
	/**
	 * basic constructor empty pages
	 */
								MemoryPage();
	/**
	 * @brief Steals content from another MemoryPage
	 */
	void 						Copy (MemoryPage &stealFrom);
	/**
	 * @brief Deletes all pages of memory
	 */
								~MemoryPage();
	/**
	 * @brief Deletes all pages of memory
	 */
	void 						Clean();

	/**
	 * @brief return address of the element at the deep address index
	 * and checks that the elements from that point to that + span are contiguous
	 * in consecutiveSpan returns the number of consecutive bytes available
	 * returns NULL if outside range
	 */
	void *						DeepAddress(uint64 index,uint32 &consecutiveSpan);

	/**
	 * @brief finds the index corresponding to the given address
	 * @param [in] address the address to find
	 * @param [out] index
	 */
	ErrorManagement::ErrorType 	Address2Index(void * address,uint64 &index) const;

	/**
	 * @brief return address of element index
	 */
	void *						Address(uint32 index);

	/**
	 * @brief increases current page to size newBufferSize
	 */
	ErrorManagement::ErrorType 	Grow(uint32 newBufferSize);

	/**
	 * @brief decreases current page to size newBufferSize
	 */
	ErrorManagement::ErrorType 	Shrink(uint32 newBufferSize);

	/**
	 *  @brief allocate a new page
	 */
	ErrorManagement::ErrorType 	Allocate(uint32 size);

	/**
	 * @brief  last operation before closing
	 * the use of this memory change previous to next so that the memory can be parsed in the right order
	 */
	void 						FlipOrder();

	/**
	 *  @brief size of current page
	 */
	uint32 						CurrentPageSize();

	/**
	 *  @brief How many pages
	 */
	uint32 						NumberOfPages();

protected:
	/**
	 * The pointer to the current page
	 */
	MemoryPageHeader *mph;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif

} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_MEMORYPAGE_H_ */
