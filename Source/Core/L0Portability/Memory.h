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

enum MemoryAllocationFlags {
    /** within 32Mb */
    MemoryStandardMemory = 0x00000000,

    /** above 32Mb */
    MemoryExtraMemory = 0x00000001,

};

/** Set of bits indicating desired access modes */
enum MemoryTestAccessMode {
    /** read and execute */
    MTAM_Execute = 0x00000001,

    /** read  */
    MTAM_Read = 0x00000002,

    /** read  */
    MTAM_Write = 0x00000004

};

/**
 * @brief MemoryTestAccessMode and operator.
 * @param[in] a is the first MemoryTestAccessMode.
 * @param[in] b is the second MemoryTestAccessMode.
 * @return a & b.
 */
static inline MemoryTestAccessMode operator &(MemoryTestAccessMode a,
                                              MemoryTestAccessMode b) {
    return (MemoryTestAccessMode) ((int) a & (int) b);
}

/**
 * @brief MemoryTestAccessMode or operator.
 * @param[in] a is the first MemoryTestAccessMode.
 * @param[in] b is the second MemoryTestAccessMode.
 * @return a | b.
 */
static inline MemoryTestAccessMode operator |(MemoryTestAccessMode a,
                                              MemoryTestAccessMode b) {
    return (MemoryTestAccessMode) ((int) a | (int) b);
}

/**
 * @brief MemoryAllocationFlags and operator.
 * @param[in] a is the first MemoryAllocationFlags.
 * @param[in] b is the second MemoryAllocationFlags.
 * @return a & b.
 */static inline MemoryAllocationFlags operator &(MemoryAllocationFlags a,
                                                MemoryAllocationFlags b) {
    return (MemoryAllocationFlags) ((int) a & (int) b);
}

/**
 * @brief MemoryAllocationFlags or operator.
 * @param[in] a is the first MemoryAllocationFlags.
 * @param[in] b is the second MemoryAllocationFlags.
 * @return a | b.
 */static inline MemoryAllocationFlags operator |(MemoryAllocationFlags a,
                                                MemoryAllocationFlags b) {
    return (MemoryAllocationFlags) ((int) a | (int) b);
}

extern "C" {

/**
 * @see Memory::Malloc
 */
void *MemoryMalloc(uint32 size,
                   MemoryAllocationFlags allocFlag = MemoryStandardMemory);

/**
 * @see Memory::Free
 */
bool MemoryFree(void *&data);

/**
 * @see Memory::Realloc
 */
void *MemoryRealloc(void *&data,
                    uint32 newSize);

/**
 * @see Memory::AllocationStatistics
 */
bool MemoryAllocationStatistics(int32 &size,
                                int32 &chunks,
                                TID tid = (TID) 0xFFFFFFFF);

/**
 * @see Memory::GetHeaderInfo
 */
bool MemoryGetHeaderInfo(void *pointer,
                         uint32 &size,
                         TID &tid);

/**
 * @see Memory::ClearStatisticsDatabase
 */
void MemoryClearStatisticsDatabase();

/**
 * @see Memory::GetStatisticsDatabaseNElements
 */
uint32 MemoryGetStatisticsDatabaseNElements();

/**
 * @see Memory::GetUsedHeap
 */
int32 MemoryGetUsedHeap();

/**
 * @see Memory::Check
 */
bool MemoryCheck(void *address,
                 MemoryTestAccessMode accessMode,
                 uint32 size = 4);

/**
 * @see Memory::SharedAlloc
 */
void *MemorySharedAlloc(uint32 key,
                        uint32 size,
                        uint32 permMask = 0666);

/**
 * @see Memory::SharedFree
 */
void MemorySharedFree(void *address);

/**
 * @see Memory::Copy
 */
bool MemoryCopy(void *destination,
                const void *source,
                uint32 size);

/**
 * @see Memory::Compare
 */
int32 MemoryCompare(const void *mem1,
                    const void *mem2,
                    uint32 size);

/**
 * @see Memory::Search
 */
const void *MemorySearch(const void *mem,
                         char c,
                         uint32 size);

/**
 * @see Memory::Move
 */
bool MemoryMove(void* destination,
                const void* source,
                uint32 size);

/**
 * @see Memory::Set
 */
bool MemorySet(void* mem,
               char c,
               uint32 size);

/**
 * @see Memory::StringDup
 */
char *MemoryStringDup(const char *s);

}

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
     * @return True if the memory is freed, false in case of invalid pointer.
     * @post data = NULL
     */
    static bool Free(void *&data);

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
    static char *StringDup(const char *s);

    /*static void AllocationStatistics(StreamInterface *out){
     MemoryAllocationStatistics(out);
     }*/

    /**
     * @brief Gets the memory statistics associated to a thread if MEMORY_STATISTICS is defined.
     * @param[out] size The size of the memory associated with the thread.
     * @param[out] chunks The number of chunks of memory.
     * @param[in] tid The Thread id to be investigated.
     * @return false is MEMORY_STATISTICS is not defined, true if it is defined and in case of success.
     */
    static bool AllocationStatistics(int32 &size,
                                     int32 &chunks,
                                     TID tid = (TID) 0xFFFFFFFF);

    /**
     * @brief Gets the informations from the header of the memory area if MEMORY_STATISTICS is defined.
     * @param[in] pointer is a pointer to the beginning of the usable memory area.
     * @param[out] size is the size of the memory area allocated.
     * @param[out] tid is the identifier of the thread which allocates the memory.
     * @return If MEMORY_STATISTICS is defined returns true if the memory pointer is valid.
     * False if MEMORY_STATISTICS is not defined.
     * @pre The input pointer must point to the beginning of the memory area, otherwise the outputs can be incorrect.
     */
    static bool GetHeaderInfo(void *pointer,
                              uint32 &size,
                              TID &tid);

    /**
     * @brief If MEMORY_STATISTICS is defined resets the memory database.
     */
    static void ClearStatisticsDatabase();

    /**
     * @brief if MEMORY_STATISTICS is defined returns the number of elements in the memory database.
     * @return the number of elements in the memory database, 0 if MEMORY_STATISTICS is not defined.
     */
    static uint32 GetStatisticsDatabaseNElements();

    /**
     * @brief if MEMORY_STATISTICS is defined returns the total amount of the current allocated heap memory.
     * @return the amount of the current allocated heap memory, 0 if MEMORY_STATISTICS is not defined.
     */
    static int32 GetUsedHeap();

    /**
     * @brief Checks if the process has the access to the specified memory area.
     * @param[in] address The pointer to the memory block to be checked.
     * @param[in] accessMode The type of memory access to perform.
     * @param[in] size is the number of bytes to check.
     * @return True if the check was successful. False otherwise.
     */
    static bool Check(void *address,
                      MemoryTestAccessMode accessMode,
                      uint32 size = 4);

    /**
     * @brief Creates or accesses an area of shared memory which can be used to communicate between different processes.
     * @details Even if you are accessing a shared memory created by another process, you should free it using
     * MemorySharedFree, as soon as you finish.\n
     * The permMask value has 3 digits, the first gives permissions for the owner, the second for the group, the third for common users:\n
     * read permission = 4.\n
     * write permission = 2.\n
     * execute permission = 1.\n
     * Sum the values to give more permissions, for example 777 means that all users has read, write and execute permissions.\n
     * Default: 666 (read + write).
     * @param[in] key the desired identifier for the shared memory.
     * @param[in] size the size of the shared memory.
     * @param[in] permMask the process permissions.Usually you will set this as 0666.
     * @return the address or NULL if it fails to allocate the memory.
     */
    static void *SharedAlloc(uint32 key,
                             uint32 size,
                             uint32 permMask = 0666);

    /**
     * @brief Frees an area of shared memory which was previously created with MemorySharedAlloc.
     * @param[in] address the address for the shared memory.
     */
    static void SharedFree(void *address);

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
                              char c,
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
                    char c,
                    uint32 size);

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORY_H_ */

