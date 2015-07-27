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
#define 		MEMORY_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/**
 * @brief Memory management.
 *
 * @details Here are defined functions to allocate, reallocate and deallocate memory dinamically,
 * other functions for the shared memory management which allows allocation of a memory
 * accessible by more tasks and at last functions which allows copy, search, set and compare
 * memory areas.
 *
 * @details Most of the implementation is delegated to MemoryOS.h which provides system calls for the
 * memory management that are different for each operating system.
 *
 * @details These methods are particularly useful in the implementation of functionality that requires
 * a dynamic management of the memory, like for example databases, static lists, streams ecc.
 */







/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Implementation of functions for heap memory management.
 *
 * @details These functions allows to allocate and free memory portions dynamically on the heap. It is possible use different functionalities using the flags
 * defined in MemoryAllocationFlags enumeration.\n
 * - MemoryStandardMemory: is the default flag and imposes that the memory to be allocated is minor than 32Mb.\n
 * - MemoryExtraMemory: allows to allocate more than 32Mb.\n
 *
 * If the symbol MEMORY_STATISTIC is defined:
 *
 * The informations related to the heap memory allocated by a fixed number of threads
 * are stored in a database (MemoryStatisticsDatabase). The maximum number of elements in the database is defined in GeneralDefinitionsOS.h
 * The informations about the memory allocated by a certain thread are accessible using Memory::AllocationStatistics function.
 *
 * Moreover an header which contains informations about the size and thread id who allocated the memory is created before each memory chunk.\n
 * These informations are accessible using Memory::GetHeaderInfo function.
 *
 * @details Most of the implementation is OS dependent and it is delegated to MemoryOS class in MemoryOS.h.
 */
class Memory {

public:

    enum MemoryAllocationFlags {
        /** within 32Mb */
        MemoryStandardMemory = 0x00000000,

        /** above 32Mb */
        MemoryExtraMemory = 0x00000001,

    };

    /** Set of bits indicating desired access modes */
    enum MemoryTestAccessMode {
        /** read and execute */
        Execute = 0x00000001,

        /** read  */
        Read = 0x00000002,

        /** read  */
        Write = 0x00000004

    };





    /**
     * @brief Allocate a portion of memory on the heap.
     * @param[in] size The size in byte of the memory to allocate.
     * @param[in] allocFlag specified the desired allocation properties
     * @return The pointer to the allocated memory. NULL if allocation failed.
     */
    static void *Malloc(uint32 size,
                        MemoryAllocationFlags allocFlag = MemoryStandardMemory);

    /**
     * @brief Release a memory area and set its pointer to NULL.
     * @param[in,out] data The memory area to be freed.
     * @return true if the memory is freed, false in case of invalid pointer.
     * @post data = NULL
     */
    static void Free(void *&data);

    /**
     * @brief Reallocate a memory portion possibly contiguously with the specified already existent memory area.
     * @param[in,out] data The pointer to the new memory block.
     * @param[in] newSize The size of the new memory block.
     * @return The pointer to the new data block. NULL if reallocation failed.
     * @details If there is no space available for the new size specified, the system could allocate the new portion
     * in a different location and in this case the pointer changes its value.
     */
    static void *Realloc(void *&data,
                         uint32 newSize);

    /**
     * @brief Wrap the strdup standard C function.
     * @param[in] s The pointer to the memory to be copied.
     * @return The pointer to the new allocated memory which contains a copy of s.
     */
    static char8 *StringDup(const char8 *s);



    /**
     * @brief Checks if the process has the access to the specified memory area.
     * @param[in] address The pointer to the memory block to be checked.
     * @param[in] accessMode The type of memory access to perform.
     * @param[in] size is the number of bytes to check.
     * @return true if the check was successful. false otherwise.
     */
    static bool Check(void *address,
                      MemoryTestAccessMode accessMode,
                      uint32 size = 4);


    /**
     * @brief Copy source to destination.
     * @details A copy of size bytes from source location to destination position.
     * @param[in] destination is the pointer to the destination memory location.
     * @param[in] source is the pointer to the source memory location.
     * @param[in] size is the size of the memory to be copied.
     * @return true if source, destination and destination after copy are != NULL.
     */
    static bool Copy(void *destination,
                     const void *source,
                     uint32 size);

    /**
     * @brief Compare the first specified bytes of mem1 and mem2.
     * @param[in] mem1 is the pointer to the first memory location.
     * @param[in] mem2 is the pointer to the second memory location.
     * @param[in] size is the number of byte to compare.
     * @return (0 if mem1 == mem2), (1 if mem1 < mem2), (2 if mem1 > mem2).
     */
    static int32 Compare(const void *mem1,
                         const void *mem2,
                         uint32 size);

    /**
     * @brief Search a character in the specified memory area.
     * @param[in] mem is the memory location.
     * @param[in] c is the character to find.
     * @param[in] size is the size of the memory area.
     * @return the pointer to the first occurrence of c in the memory. NULL if c is absent.
     */
    static const void *Search(const void *mem,
                              char8 c,
                              uint32 size);

    /**
     * @brief Copy source to destination.
     * @param[in] destination is the pointer to the destination memory location.
     * @param[in] source is the pointer to the source memory location.
     * @param[in] size is the number of bytes to be copied.
     * @return true if source, destination, and destination after the copy are != NULL.
     */
    static bool Move(void* destination,
                     const void* source,
                     uint32 size);

    /**
     * @brief Set a defined number bytes of the specified memory area equal to a specified character.
     * @param[in] mem is the pointer to the memory location.
     * @param[in] c is the character to store.
     * @param[in] size is the number of bytes where c will be written.
     */
    static bool Set(void* mem,
                    char8 c,
                    uint32 size);

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
/**
 * @brief MemoryTestAccessMode and operator.
 * @param[in] a is the first MemoryTestAccessMode.
 * @param[in] b is the second MemoryTestAccessMode.
 * @return a & b.
 */
static inline Memory::MemoryTestAccessMode operator &(Memory::MemoryTestAccessMode a,
                                              Memory::MemoryTestAccessMode b) {
    return (Memory::MemoryTestAccessMode) ((int) a & (int) b);
}

/**
 * @brief MemoryTestAccessMode or operator.
 * @param[in] a is the first MemoryTestAccessMode.
 * @param[in] b is the second MemoryTestAccessMode.
 * @return a | b.
 */
static inline Memory::MemoryTestAccessMode operator |(Memory::MemoryTestAccessMode a,
                                              Memory::MemoryTestAccessMode b) {
    return (Memory::MemoryTestAccessMode) ((int) a | (int) b);
}

/**
 * @brief MemoryAllocationFlags and operator.
 * @param[in] a is the first MemoryAllocationFlags.
 * @param[in] b is the second MemoryAllocationFlags.
 * @return a & b.
 */static inline Memory::MemoryAllocationFlags operator &(Memory::MemoryAllocationFlags a,
                                                          Memory::MemoryAllocationFlags b) {
    return (Memory::MemoryAllocationFlags) ((int) a & (int) b);
}

/**
 * @brief MemoryAllocationFlags or operator.
 * @param[in] a is the first MemoryAllocationFlags.
 * @param[in] b is the second MemoryAllocationFlags.
 * @return a | b.
 */static inline Memory::MemoryAllocationFlags operator |(Memory::MemoryAllocationFlags a,
                                                          Memory::MemoryAllocationFlags b) {
    return (Memory::MemoryAllocationFlags) ((int) a | (int) b);
}
#endif /* MEMORY_H_ */

