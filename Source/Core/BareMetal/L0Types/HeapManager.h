/**
 * @file MarteMalloc.h
 * @brief Header file for class AnyType
 * @date 15 Nov 2018
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

 * @details This header file contains the declaration of the namespace HeapManager
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef HEAP_MANAGER_H_
#define HEAP_MANAGER_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "AllocationHeader.h"
#include "HeapI.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * contains objects and function to manage heaps and alloacte memory and objects
 */
namespace HeapManager{

/**
 * maximum of 32 heap managers
 */
#define MAX_HEAPS 32

/**
 * these pointers need to be defined and assigned somewhere if malloc or new are used during initialisation
 */
extern void *std_malloc(size_t size);

/**
 * these pointers need to be defined and assigned somewhere if free or delete are used
 */
extern void std_free(void *p);

/**
 * these pointers need to be defined and assigned somewhere if realloc is used
 */
extern void *std_realloc(void *p,size_t size);

/**
 * @brief installs a specialised allocator for the give id
 */
ErrorManagement::ErrorType InstallAllocator(HeapI *heapi,HeapId id);

/**
 * @brief removes the specialised allocator at the given id
 * Does not deallocate memory
 */
ErrorManagement::ErrorType RemoveAllocator(HeapId id);

/**
 * @brief converts names to heapIds
 */
HeapManager::HeapId HeapIdFromName(CCString name);

/**
 * @brief replacement of malloc
 * if a specialised allocator for the specified heap is not given singletonHeapId will be used
 * @param[in] id The allocator to use
 * @return  pointer to the allocated memory
 */
void *Malloc(uint32 byteSize,HeapId id=standardHeapId);

/**
 * @brief replacement of free
 */
void Free(void *ptr);

/**
 * @brief replacement of realloc
 */
void *Realloc(void *ptr,uint32 byteSize);

/**
 * @brief net number of allocated blocks on a given pool
 */
extern uint32 allocatedBlocks[MAX_HEAPS];

/**
 * @brief net number of allocated memory on a given pool
 */
extern uint32 allocatedBytes[MAX_HEAPS];

} // HeapManager
} // MARTe


/**
 * @brief replacement of new
 */
void * operator new(size_t size);

/**
 * @brief placement of new
 */
void * operator new(size_t size,MARTe::HeapManager::HeapId heapId);

/**
 * @brief replacement of new[]
 */
void * operator new[](size_t size);

/**
 * @brief placement new[]
 */
void * operator new[](size_t size,MARTe::HeapManager::HeapId heapId);

/**
 * @brief replacement of delete
 * Handles also placement new and malloc (when constructors/destructors are not used)
 */
void operator delete(void * p);

/**
 * @brief replacement of delete[]
 * Handles also placement new[]
 */
void operator delete[](void * p);


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_MARTEMALLOC_H_ */

