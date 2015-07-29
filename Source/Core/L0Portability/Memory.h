/**
 * @file Memory.h
 * @brief Header file for class Memory
 * @date 18/06/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class Memory
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORY_H_
#define MEMORY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Implementation of functions for heap memory management.
 *
 * @details These functions allows to allocate and free memory dynamically on the heap.
 * The MemoryAllocationFlags allow some operating systems to allocate memory in user specified areas.
 *
 */
class Memory {

public:

    /**
     * Memory allocation flags used to specify allocation properties.
     */
    enum MemoryAllocationFlags {
        /**
         * Allows to allocate within 32Mb
         */
        StandardMemory = 0x00000000,

        /**
         * Allows to allocate above 32Mb
         */
        ExtraMemory = 0x00000001

    };

    /**
     * Set of flags indicating desired access modes to the
     * memory by processes.
     */
    enum MemoryTestAccessMode {
        /** read and execute */
        Execute = 0x00000001,

        /** read  */
        Read = 0x00000002,

        /** read  */
        Write = 0x00000004

    };

    /**
     * @brief Allocates a region of memory on the heap.
     * @param[in] size The size in byte of the memory to allocate.
     * @param[in] allocFlag specified the desired allocation properties
     * @return The pointer to the allocated memory. NULL if the allocation failed.
     */
    static void *Malloc(const uint32 size,
                        const MemoryAllocationFlags allocFlag = StandardMemory);

    /**
     * @brief Releases a memory area and sets its pointer to NULL.
     * @param[in,out] data The memory area to be freed.
     * @return true if the memory is freed, false in case of invalid pointer.
     * @post data = NULL
     */
    static void Free(void *&data);

    /**
     * @brief Reallocates a memory portion possibly contiguously with the specified already existent memory area.
     * @details If there is no space available for the new size specified, the system could allocate the new region
     * in a different location, in which case the original memory pointer will change its value.
     * @param[in,out] data the pointer to the new memory block. If this value is NULL a Malloc(newSize) will be called.
     * @param[in] newSize the size of the new memory block. If this value is zero a Free(data) will be called.
     * @return The pointer to the new data block. NULL if reallocation failed.
     */
    static void *Realloc(void *&data,
                         const uint32 newSize);

    /**
     * @brief Duplicates a string in the heap memory.
     * @param[in] s the pointer to the string which must be copied.
     * @return The pointer to the new allocated memory which contains a copy of s.
     */
    static char8 *StringDup(const char8 * const s);

    /**
     * @brief Checks if the process has access to the specified memory area.
     * @param[in] address The pointer to the memory block to be checked.
     * @param[in] accessMode The type of memory access to perform.
     * @param[in] size is the number of bytes to check.
     * @return true if the process has the access to the memory with the specified permissions, false otherwise.
     */
    static bool Check(const void * const address,
                      const MemoryTestAccessMode accessMode,
                      const uint32 size = 4u);

    /**
     * @brief Copies a block of memory into a specified memory destination.
     * @details A copy of size bytes from source location to destination position.
     * @param[in,out] destination the pointer to the destination memory location.
     * @param[in] source the pointer to the source memory location.
     * @param[in] size the size of the memory to be copied.
     * @return true if source, destination and destination after copy are not NULL.
     * @pre the size parameter must be less than the memory blocks size.
     */
    static bool Copy(void * const destination,
                     const void * const source,
                     uint32 size);

    /**
     * @brief Compares the first specified bytes of two blocks of memory.
     * @param[in] mem1 the pointer to the first memory location.
     * @param[in] mem2 the pointer to the second memory location.
     * @param[in] size the number of byte to compare.
     * @return (0 if mem1 == mem2), (1 if mem1 < mem2), (2 if mem1 > mem2).
     * @pre the size parameter must be minor than the memory blocks sizes.
     */
    static int32 Compare(const void * const mem1,
                         const void * const mem2,
                         uint32 size);

    /**
     * @brief Searches a character in the specified memory block.
     * @param[in] mem the memory location.
     * @param[in] c the character to find.
     * @param[in] size the size of the memory area.
     * @return the pointer to the first occurrence of c in the memory. NULL if c is absent.
     * @pre the size parameter must be minor than the memory block size.
     */
    static const void *Search(const void * const mem,
                              const char8 c,
                              const uint32 size);

    /**
     * @brief Copies a block of memory into another allowing overlapping.
     * @param[in,out] destination the pointer to the destination memory location.
     * @param[in] source the pointer to the source memory location.
     * @param[in] size the number of bytes to be copied.
     * @return true if source, destination, and destination after the copy are not NULL.
     * @pre the size parameter must be minor than the memory blocks sizes.
     */
    static bool Move(void * const destination,
                     const void * const source,
                     const uint32 size);

    /**
     * @brief Sets a defined number bytes of the specified memory area equal to a specified character.
     * @param[in,out] mem is the pointer to the memory location.
     * @param[in] c is the character to store.
     * @param[in] size is the number of bytes where c will be written.
     * @pre the size parameter must be minor than the memory block size.
     */
    static bool Set(void * const mem,
                    const char8 c,
                    const uint32 size);

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORY_H_ */
