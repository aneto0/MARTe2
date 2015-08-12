/**
 * @file HeapManager.h
 * @brief Header file for class HeapManager
 * @date Aug 6, 2015
 * @author fsartori
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

#ifndef HEAPMANAGER_H_
#define HEAPMANAGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "HeapInterface.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/



/**
 * @brief TODO
 */
namespace HeapManager{


/**
 * @brief Finds the Heap that manages the specified memory location
 * @param address is a memory address that the target heap should manage
 * returns NULL if not found
 */
HeapInterface *FindHeap(const void * address);

/**
 * @brief Finds the Heap that has the specified name
 * returns GetStandardHeap() if not found
 */
HeapInterface *FindHeap(const char8 * const name);


/**
 * @brief free the pointer data and its associated memory.
 * @param data the data to be freed.
 * returns false if no heap is found that can handle this data
 */
bool Free(void *&data);

/**
 * @brief allocates memory from heap (by default standard heap)
 * @param size size of memory to allocate.
 * @param name name of heap to use
 * returns false if no heap is found that can handle this data
 */
void *Malloc(uint32 size, const char8 *name = NULL_PTR(char8 *));

/**
 * @brief
 * @param
 * returns
 */
bool AddHeap(HeapInterface *newHeap);

/**
 * @brief
 * @param
 * returns
 */
bool RemoveHeap(HeapInterface *heap);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L0PORTABILITY_HEAPMANAGER_H_ */
	
