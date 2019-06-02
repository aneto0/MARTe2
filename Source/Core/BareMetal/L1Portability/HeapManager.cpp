/**
 * @file HeapManager.cpp
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

 * @details This header file contains the declaration of the class HeapManager
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <stdlib.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HeapI.h"
#include "HeapManager.h"
#include "AllocationPointer.h"
#include "Atomic.h"
#include "CompositeErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/


void * operator new(size_t size){
	MARTe::HeapManager::AllocationPointer ap;
    ap.Malloc(static_cast<MARTe::uint32>(size));

    void *ptr = ap;
    return ptr;
}

void * operator new(size_t size,MARTe::HeapManager::HeapId heapId){
	MARTe::HeapManager::AllocationPointer ap;
    ap.Malloc(static_cast<MARTe::uint32>(size),heapId);

    void *ptr = ap;
    return ptr;
}

void operator delete(void * p){
	MARTe::HeapManager::AllocationPointer ap(p);
	ap.Free();
}

void * operator new[](size_t size){
	MARTe::HeapManager::AllocationPointer ap;
    ap.Malloc(static_cast<MARTe::uint32>(size));

    void *ptr = ap;
    return ptr;
}

void * operator new[](size_t size,MARTe::HeapManager::HeapId heapId){
	MARTe::HeapManager::AllocationPointer ap;
    ap.Malloc(static_cast<MARTe::uint32>(size),heapId);

    void *ptr = ap;
    return ptr;
}

void operator delete[](void * p){
	MARTe::HeapManager::AllocationPointer ap(p);
	ap.Free();
}


namespace MARTe{

namespace HeapManager{

void *Malloc(uint32 byteSize,HeapManager::HeapId id){
	MARTe::HeapManager::AllocationPointer ap;
    ap.Malloc(byteSize,id);

    void *ptr = ap;
    return ptr;

}
void Free(void *ptr){
	MARTe::HeapManager::AllocationPointer ap(ptr);
	ap.Free();
}
void *Realloc(void *ptr,uint32 byteSize){
	MARTe::HeapManager::AllocationPointer ap(ptr);
	ap.Realloc(byteSize);
    ptr = ap;
    return ptr;
}

//for convenience
#define nullHeap NULL_PTR(HeapI*)


HeapI *heapList[MAX_HEAPS]= {
    nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap,
    nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap,
    nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap,
    nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap, nullHeap
};

uint32 allocatedBlocks[MAX_HEAPS]={
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
};

uint32 allocatedBytes[MAX_HEAPS]={
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0
};

CCString heapNames[MAX_HEAPS]= {
	"singletonHeap", "standardHeap", "internals", "strings", "customHeap0", "customHeap1", "customHeap2", "customHeap3",
	"customHeap4", "customHeap5", "customHeap6", "customHeap7","customHeap8", "customHeap9", "customHeap10", "customHeap11",
	"customHeap12", "customHeap13", "customHeap14", "customHeap15","customHeap16", "customHeap17", "customHeap18", "customHeap19",
	"customHeap20", "customHeap21", "customHeap22", "customHeap23", "customHeap24", "customHeap25", "customHeap26", "customHeap27"
};


HeapManager::HeapId HeapIdFromName(CCString name){
	bool found = false;
	HeapManager::HeapId index = 0u;
	for (index = 0u;(index < MAX_HEAPS) && !found;index++){
		found = (name == heapNames[index]);
	}

	if (!found) {
        COMPOSITE_REPORT_ERROR(ErrorManagement::Warning, "Could not find an memoryHeap with the name: ", name);
		index = standardHeapId;
	}
	return index;
}

ErrorManagement::ErrorType InstallAllocator(HeapI *heapi,HeapId id){
	ErrorManagement::ErrorType ret;
	// cannot be changed using this interface
	ret.unsupportedFeature = (id == singletonHeapId);
	ret.outOfRange = (id >= MAX_HEAPS);
	ret.parametersError = (heapi == NULL_PTR(HeapI*));

	if (ret){
		ret.initialisationError = (heapList[id] != NULL);
	}

	if (ret){
		heapList[id] = heapi;
	}

	return ret;
}

ErrorManagement::ErrorType RemoveAllocator(HeapId id){
	ErrorManagement::ErrorType ret;
	// cannot be changed using this interface
	ret.unsupportedFeature = (id == singletonHeapId);
	ret.outOfRange = (id >= MAX_HEAPS);
	ret.invalidOperation = (heapList[id] == NULL);

	if (ret){
		heapList[id] = NULL;
	}

	return ret;
}


ErrorManagement::ErrorType AllocationPointer::Malloc(uint32 byteSize,HeapManager::HeapId id){
	ErrorManagement::ErrorType ret;

	ret.parametersError = (id >= MAX_HEAPS);
	uint32 size = byteSize + sizeof (AllocationHeader);
	HeapManager::HeapI * h = NULL_PTR(HeapManager::HeapI *);

	if (ret){
		// check for overflow
		ret.outOfRange = (size < byteSize);
	}

	if (ret){

		h = heapList[id];
		if (h == NULL_PTR(HeapManager::HeapI *)){
			/**
			 * no allocator present
			 * use the singletonHeapId
			 */
			id = singletonHeapId;
			memory = static_cast<AllocationHeader *> (std_malloc(size));
		} else {
			memory = static_cast<AllocationHeader *> (h->Malloc(size));
		}
		ret.outOfMemory = (memory == NULL_PTR(AllocationHeader *));
	}

	if (ret){
		memory->byteSize = byteSize;
		memory->heapId = id;
		Atomic::Increment((int32 *)&allocatedBlocks[id]);
		Atomic::Add((int32 *)&allocatedBytes[id],static_cast<int32>(byteSize));
	}

	return ret;
}

ErrorManagement::ErrorType AllocationPointer::Realloc(uint32 byteSize){
	ErrorManagement::ErrorType ret;

	if (memory == NULL_PTR(AllocationHeader *)){
		ret = Malloc(byteSize);
	} else {
		HeapManager::HeapId id = HeapManager::standardHeapId;

		uint32 size = byteSize + sizeof (AllocationHeader);
		ret.outOfRange = (size < byteSize);

		uint32 oldSize = 0;
		if (ret){
			id = memory->heapId;
			ret.internalSetupError = (id >= MAX_HEAPS);
			oldSize = memory->byteSize;
		}

		HeapManager::HeapI * h = NULL_PTR(HeapManager::HeapI *);
		if (ret){
			h = heapList[id];
			ret.internalSetupError = (h == NULL_PTR(HeapManager::HeapI *));
		}

		if (ret){
			Atomic::Decrement((int32 *)&allocatedBlocks[id]);
			Atomic::Sub((int32 *)&allocatedBytes[id],static_cast<int32>(oldSize));

			void *ptr = static_cast<void *>(memory);
			memory = static_cast<AllocationHeader *> (h->Realloc(ptr,size));

			ret.outOfMemory = (memory == NULL_PTR(AllocationHeader *));
		}

		if (ret){
			memory->byteSize = byteSize;
			memory->heapId = id;
			Atomic::Increment((int32 *)&allocatedBlocks[id]);
			Atomic::Add((int32 *)&allocatedBytes[id],static_cast<int32>(byteSize));
		}
	}

	return ret;
}

ErrorManagement::ErrorType AllocationPointer::Free(){
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (memory == NULL_PTR(AllocationHeader *));

	uint32 oldSize = 0;
	HeapManager::HeapId id = HeapManager::standardHeapId;
	if (ret){
		id = memory->heapId;
		// sets an invalid number in case of attempt of further freeing memory
		memory->heapId = 0xFF;
		ret.internalSetupError = (id >= MAX_HEAPS);
		oldSize = memory->byteSize;
		memory->byteSize = 0;
	}

	HeapManager::HeapI * h = NULL_PTR(HeapManager::HeapI *);
	if (ret){
		h = heapList[id];
		ret.internalSetupError = (h == NULL_PTR(HeapManager::HeapI *));
	}

	if (ret){
		void *ptr = static_cast<void *>(memory);
		h->Free(ptr);
		memory = NULL_PTR(AllocationHeader *);

		Atomic::Decrement((int32 *)&allocatedBlocks[id]);
		Atomic::Sub((int32 *)&allocatedBytes[id],static_cast<int32>(oldSize));
	}

	return ret;

}

}
} //MARTe
