/**
 * @file HeapManager.h
 * @brief Header file for module HeapManager
 * @date 06/08/2015
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

 * @details This header file contains the declaration of the module HeapManager
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

#include "HeapI.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief Implementation of functions for the heap memory management.
 * @details Using these functions is possible adding different HeapI
 * objects in a hidden static database and using their specific functions for the
 * desired heap management.
 */
namespace HeapManager {

/**
 * @brief Finds the HeapI that manages the specified memory location in the database.
 * @param[in] address is a memory address that the target heap should manage.
 * @return a pointer to the HeapI that manages the specified memory location or
 * GetStandardHeap() if no one in HeapI objects' database manages that address.
 */
HeapI *FindHeap(const void * const address);

/**
 * @brief Finds the HeapI object by name in the database.
 * @param[in] name is the name of the desired heap.
 * @return a pointer to the HeapI with the specified name or
 * NULL if the name is not found in the HeapI objects' database.
 */
HeapI *FindHeap(const char8 * const name);

/**
 * @brief Registers a HeapI object in the database.
 * @param[in] newHeap is a pointer to the HeapI object which must be added.
 * @return true if newHeap has been correctly registered in the database,
 * false if the object is already registered or if there is no space to add it.
 * @pre All the heaps added have to have different names. AddHeap does not check if it is already include
 * a heap with the same name.
 */
bool AddHeap(HeapI *const newHeap);

/**
 * @brief Removes a specified HeapI object from the database.
 * @param[in] heap is the HeapI object which must be removed.
 * @return true if the specified heap was in the database and
 * it has been correctly removed, false otherwise.
 */
bool RemoveHeap(const HeapI * const heap);

/**
 * @brief Calls the HeapI::Free function of the HeapI
 * object which owns the specified address.
 * @details The address in input could belong to more than one HeapI registered in the database and
 * in this case the free of the one with the smaller range of memory will be performed.
 * @param[in,out] data The memory area to be freed.
 * @return true if the memory is freed, false in case of belonging to no heap.
 * @post data = NULL
 */
bool Free(void *&data);

/**
 * @brief Allocates a portion of memory on the specified heap.
 * @details If the desired heap is not found in the database, the allocation
 * will be performed on the standard heap.
 * @param[in] size The size in byte of the memory to allocate.
 * @param[in] heapName name of heap to use.
 * @return The pointer to the allocated memory. NULL if allocation failed.
 */
void *Malloc(uint32 size,
             const char8 * const heapName = NULL_PTR(char8 *));

/**
 * @brief Reallocates a memory portion possibly contiguously with the specified already existent memory area.
 * @details If there is no space available for the new size specified, the system could allocate the new portion
 * in a different location and in this case the pointer changes its value.
 * @param[in,out] data The pointer to the new memory block.
 * @param[in] newSize The size of the new memory block.
 * @return The pointer to the new data block. NULL if reallocation failed.
 */
void *Realloc(void *&data,
              const uint32 newSize);

/**
 * @brief Duplicates a memory section into a new area from the specified heap.
 * @details If \a size != 0 the function duplicates the specified number of bytes of
 * the memory area pointed by \a data.\n
 * If \a size = 0 the function considers the memory area pointed by \a data as a zero
 * terminated string and copies the memory until a zero is found (C string duplication).
 * @param[in] data The pointer to the memory which must be copied.
 * @param[in] size size of memory to allocate. if size = 0 then memory is copied until a zero is found
 * @param[in] heapName name of heap to use. default = copy on the same heap as data
 * @return The pointer to the new allocated memory which contains a copy of s.
 * @pre If size = 0 the memory area pointed by \a data must be a zero terminated string.
 */
void *Duplicate(const void * const data,
                const uint32 size = 0U,
                const char8 * const heapName = NULL_PTR(char8 *));

}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HEAPMANAGER_H_ */

