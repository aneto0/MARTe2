/**
 * @file AllocationPointer.h
 * @brief Header file for class AnyType
 * @date 9 Dec 2018
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

 * @details This header file contains the declaration of the class AllocationPointer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef ALLOCATIONPOINTER_H_
#define ALLOCATIONPOINTER_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AllocationHeader.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace HeapManager{

/**
 * @brief class containing the result of a memory allocation
 */
class AllocationPointer{

	/**
	 * The actual pointer to memory allocated.
	 */
	AllocationHeader *					memory;
public:

	/**
	 * just sets pointer to NULL
	 */
	inline AllocationPointer();

	/**
	 * @brief reconstruct AllocationPointer from a pointer.
	 * Be sure that the pointer is a valid allocation
	 */
	template<class T>
	AllocationPointer(T *ptr);

	/**
	 * @brief auto convert to any T*. No need to do a cast
	 */
	template<class T> operator T *() ;

	/**
	 * @brief recovers size of memory allocation
	 */
	inline uint32 GetByteSize() const;

	/**
	 * @brief identity of heap used in memory allocation
	 */
	inline HeapManager::HeapId GetHeapId() const;

	/**
	 * @brief This is the main allocation function
	 * All memory will be allocated through this call
	 */
	ErrorManagement::ErrorType Malloc(uint32 byteSize,HeapId id=standardHeapId);

	/**
	 * @brief This is the re-allocation function
	 * elementSize is not changed
	 */
	ErrorManagement::ErrorType Realloc(uint32 byteSize);

	/**
	 * @brief This is the main deallocation function
	 * All memory will be deallocated through this call
	 */
	ErrorManagement::ErrorType Free();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

AllocationPointer::AllocationPointer(){
	memory = NULL_PTR(AllocationHeader *);
}

template<class T>
AllocationPointer::AllocationPointer(T *ptr){
	memory = NULL_PTR(AllocationHeader *);
	if (ptr != NULL_PTR(T *)){
		memory = reinterpret_cast<AllocationHeader *>(ptr) - 1;
	}
}

template<class T> AllocationPointer::operator T *() {
	T *ptr = NULL_PTR(T*);
	if (memory != NULL_PTR(AllocationHeader *)){
		ptr = reinterpret_cast<T *>(memory + 1);
	}
	return ptr;
}

uint32 AllocationPointer::GetByteSize() const{
	uint32 size = 0;
	if (memory != NULL_PTR(AllocationHeader *)){
		size = memory->byteSize;
	}
	return size;
}

HeapManager::HeapId AllocationPointer::GetHeapId() const{
	HeapManager::HeapId id = 0;
	if (memory != NULL_PTR(AllocationHeader *)){
		id = memory->heapId;
	}
	return id;
}


}
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_ALLOCATIONPOINTER_H_ */
