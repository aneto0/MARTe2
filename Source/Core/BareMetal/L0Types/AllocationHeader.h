/**
 * @file AllocationHeader.h
 * @brief Header file for class AnyType
 * @date 19 Dec 2018
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

 * @details This header file contains the declaration of the class AllocationHeader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef ALLOCATIONHEADER_H_
#define ALLOCATIONHEADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace HeapManager{

/**
 * @brief unique identifier of Heaps
 * @details used to specify from what heap to allocate memory
 */
typedef uint8 HeapId;

/**
 * @brief heap used to build singletons.
 * This heap is active from before the object initialisation phase has been completed
 */
static const HeapId singletonHeapId = 0;

/**
 * @brief default heap
 */
static const HeapId standardHeapId = 1;

/**
 * @brief Marte internals Heap
 */
static const HeapId internalsHeapId = 2;

/**
 * @brief used for strings
 */
static const HeapId stringsHeapId = 3;

/**
 * @brief customs heaps first index
 */
static const HeapId customHeapId = 4;


/**
 * 32 bit + 32 bit allocation header
 */
struct AllocationHeader{
	/**
	 * the size in bytes of the memory allocated
	 */
	uint32 						byteSize;

// TODO develop more sophisticated header
	/**
	 * should be new32bDataCount or new32bDataSize
	 */
//	uint16		 				flags;

	/**
	 * 0 = default heap 1-63 custom heaps
	 */
	HeapId 						heapId;
};





/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

};
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_ALLOCATIONHEADER_H_ */
