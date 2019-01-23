/**
 * @file AllocationPointer.cpp
 * @brief Header file for class AnyType
 * @date 10 Dec 2018
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

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AllocationPointer.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


ErrorManagement::ErrorType AllocationPointer::Malloc(uint32 byteSize,HeapManager::HeapId id){
	ErrorManagement::ErrorType ret;

	ret.parametersError = (id >= MAX_HEAPS);

	HeapManager::HeapI * h = NULL_PTR(HeapManager::HeapI *);
	if (ret){
		h = HeapManager::heapList[id];

		ret.unsupportedFeature = (h == NULL_PTR(HeapManager::HeapI *));
	}

	if (ret){
		uint64 size = byteSize + sizeof (MallocHeader);
		memory = static_cast<MallocHeader *> (h->Malloc(size));

		ret.outOfMemory = (memory == NULL_PTR(MallocHeader *));
	}

	if (ret){
		memory->byteSize = byteSize;
		memory->heapId = id;
	}

	return ret;

}

ErrorManagement::ErrorType AllocationPointer::Realloc(uint32 byteSize){
	ErrorManagement::ErrorType ret;

	if (memory == NULL_PTR(MallocHeader *)){
		ret = Malloc(byteSize);
	} else {
		HeapManager::HeapId id;
		if (ret){
			id = memory->heapId;
			ret.internalSetupError = (id >= MAX_HEAPS);
		}

		HeapManager::HeapI * h = NULL_PTR(HeapManager::HeapI *);
		if (ret){
			h = HeapManager::heapList[id];
			ret.internalSetupError = (h == NULL_PTR(HeapManager::HeapI *));
		}

		if (ret){
			uint64 size = byteSize + sizeof (MallocHeader);
			void *ptr = static_cast<void *>(memory);
			memory = static_cast<MallocHeader *> (h->Realloc(ptr,size));

			ret.outOfMemory = (memory == NULL_PTR(MallocHeader *));
		}

		if (ret){
			memory->byteSize = byteSize;
			memory->heapId = id;
		}
	}

	return ret;

}

ErrorManagement::ErrorType AllocationPointer::Free(){
	ErrorManagement::ErrorType ret;

	ret.invalidOperation = (memory == NULL_PTR(MallocHeader *));

	HeapManager::HeapId id;
	if (ret){
		id = memory->heapId;
		ret.internalSetupError = (id >= MAX_HEAPS);
	}

	HeapManager::HeapI * h = NULL_PTR(HeapManager::HeapI *);
	if (ret){
		h = HeapManager::heapList[id];
		ret.internalSetupError = (h == NULL_PTR(HeapManager::HeapI *));
	}

	if (ret){
		void *ptr = static_cast<void *>(memory);
		h->Free(ptr);
		memory = static_cast<MallocHeader *>(ptr);
	}

	return ret;

}

} // MARTe

