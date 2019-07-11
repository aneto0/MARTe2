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
#include "CompositeErrorManagement.h"

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


} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_MEMORYPAGE_H_ */
