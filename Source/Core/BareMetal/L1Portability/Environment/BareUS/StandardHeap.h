/**
 * @file StandardHeap_Generic.h
 * @brief Header file for class StandardHeap
 * @date 13/08/2015
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

 * @details This header file contains the declaration of the class StandardHeap
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STANDARDHEAP_GENERIC_H_
#define STANDARDHEAP_GENERIC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "HeapI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

/**
 * @brief Implementation of standard heap memory management functions.
 * @details These functions allows to allocate, reallocate and free portion of
 * memory dynamically in the heap memory area using the standard C functions
 * (malloc, realloc, free, ...).
 */
class DLL_API StandardHeap: public HeapI {

    /**
     * @brief The minor address returned by a Malloc function.
     */
    uintp firstAddress;
    /**
     * @brief The greater address returned by a Malloc function
     */
    uintp lastAddress;

public:

    /**
     * @brief Default constructor.
     */
    StandardHeap();

    /**
     * @brief Destructor.
     */
    virtual ~StandardHeap();

    /**
     * @brief Allocates a portion of memory on the heap.
     * @param[in] size The size in byte of the memory to allocate.
     * @return The pointer to the allocated memory. NULL if allocation fails.
     */
    virtual void *Malloc(const uint32 size);

    /**
     * @brief Releases a memory area and sets its pointer to NULL.
     * @param[in,out] data The memory area to be freed.
     * @return true if the memory is freed, false in case of invalid pointer.
     * @post data = NULL
     */
    virtual void Free(void *&data);

    /**
     * @brief Reallocates a memory portion possibly contiguously with the specified already existent memory area.
     * @details If there is no space available for the new size specified, the system could allocate the new portion
     * in a different location and in this case the pointer changes its value.
     * @param[in,out] data The pointer to the new memory block.
     * @param[in] newSize The size of the new memory block.
     * @return The pointer to the new data block. NULL if reallocation failed.
     */
    virtual void *Realloc(void *&data,
                          const uint32 newSize);

    /**
     * @brief Duplicates a memory section into a new area from the specified heap.
     * @param[in] data The pointer to the memory which must be copied.
     * @param[in] size size of memory to allocate. if size = 0 then memory is copied until a zero is found
     * @return The pointer to the new allocated memory which contains a copy of s.
     */
    /*lint -e(1735) the derived classes shall use this default parameter or no default parameter at all*/
    virtual void *Duplicate(const void * const data,
                            uint32 size = 0U);

    /**
     * @brief Returns the start of range of memory addresses served by this heap.
     * @return The start of range of memory addresses served by this heap.
     */
    virtual uintp FirstAddress() const;

    /**
     * @brief Returns the end (inclusive) of range of memory addresses served by this heap.
     * @return The end (inclusive) of range of memory addresses served by this heap.
     */
    virtual uintp LastAddress() const;

    /**
     * @brief Returns the name of the heap
     * @return The name of the heap
     */
    virtual const char8 *Name() const;

};


}
#endif /*STANDARDHEAP_GENERIC_H_ */

