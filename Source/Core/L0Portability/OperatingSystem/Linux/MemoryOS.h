/**
 * @file MemoryOS.h
 * @brief Header file for class MemoryOS
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

 * @details This header file contains the declaration of the class MemoryOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYOS_H_
#define 		MEMORYOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <string.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief System dependent implementation of functions for memory Management.
 */
class MemoryOS {

public:

    /**
     * @brief Allocate dynamically a portion of memory.
     * @see MemoryMalloc
     * @param[in] size is the number of bytes to allocate.
     * @param[in] allocFlags
     * @return a pointer to the allocated memory.
     */
    static void *Malloc(uint32 size,
                        MemoryAllocationFlags allocFlags) {
        if (size <= 0) {
            return NULL;
        }
        return malloc(size);
    }

    /**
     * @brief Free a portion of memory previously dynamically allocated.
     * @see MemoryFree
     * @param[in,out] data is the pointer to the memory to be freed.
     * @post data=NULL.
     */
    static void Free(void *&data) {
        if (data != NULL) {
            free(data);
        }
        data = NULL;
    }

    /**
     * @brief Reallocate the memory.
     * @see MemoryRealloc.
     * @details The memory pointer could change if there is no space new size.
     * @param[in,out] data is the pointer to the memory which must be reallocated.
     * @param[in] newSize is the desired new size for the memory portion.
     */
    static void *Realloc(void *&data,
                         uint32 newSize) {
        return realloc(data, newSize);
    }

    /**
     * @brief Duplicate a string.
     * @see MemoryStringDup
     * @param[in] s is the string to duplicate.
     * @return a pointer to the string copied.
     */
    static char8 *StringDup(const char8 *s) {
        return strdup(s);
    }

    /**
     * @brief Allocate a shared memory.
     * @see MemorySharedAlloc.
     * @param[in] key is the identifier of the shared memory.
     * @param[in] size is the desired size of the shared memory.
     * @param[in] permMask define the process permissions.
     * @return a pointer to the shared memory created.
     */
    static void *SharedAlloc(uint32 key,
                             uint32 size,
                             uint32 permMask) {
        key_t keyid = (key_t) key;
        int32 shmid = shmget(keyid, size, IPC_CREAT | permMask);
        if (shmid == -1) {
            return NULL;
        }
        void *shm = shmat(shmid, NULL, 0);
        if (shm == (char8 *) -1) {
            return NULL;
        }
        return shm;
    }

    /**
     * @brief Free a shared memory portion.
     * @see MemorySharedFree.
     * @param[in] address is a pointer to the shared memory.
     */
    static void SharedFree(void *address) {
        shmdt(address);
    }

    /**
     * @brief Checks if the memory area is valid.
     * @see MemoryCheck.
     * @param[in] address is a pointer to the memory area target.
     * @param[in] accessMode defines the process permissions.
     * @param[in] size is the number of bytes to check.
     * @return true if the memory area specified is valid.
     */
    static bool Check(void *address,
                      MemoryTestAccessMode accessMode,
                      uint32 size) {

        //check if the memory is valid.
        char8* addressPtr = (char8 *) address;
        while (size > 0) {
            if (addressPtr == NULL) {
                return false;
            }
            size--;
            addressPtr++;
        }
        return address != NULL;
    }

    /**
     * @brief Copy source to destination.
     * @see MemoryCopy.
     * @param[in] destination is a pointer to the destination memory.
     * @param[in] source is a pointer to the source memory.
     * @param[in] size is the number of bytes to copy.
     * @return true if source, destination and destination after the copy are not NULL.
     */
    static bool Copy(void* destination,
                     const void* source,
                     uint32 size) {
        if (source == NULL || destination == NULL) {
            return false;
        }

        return memcpy(destination, source, size) != NULL;
    }

    /**
     * @brief Compare two memory portions.
     * @see MemoryCompare.
     * @param[in] mem1 is a pointer to the first memory.
     * @param[in] mem2 is a pointer to the second memory.
     * @param[in] size is the size to compare.
     * @return (0 if mem1 == mem2), (1 if mem1 < mem2), (2 if mem1 > mem2), (-1 if one of memory arguments is NULL).
     */
    static int32 Compare(const void* mem1,
                         const void* mem2,
                         uint32 size) {
        if (mem1 == NULL || mem2 == NULL) {
            return -1;
        }
        int32 ret = memcmp(mem1, mem2, size);
        if (ret < 0) {
            return 1; // 1 if mem1<mem2
        }
        if (ret > 0) {
            return 2; // 2 if mem1>mem2
        }
        return ret; //0 if mem1=mem2

    }

    /**
     * @brief Search a byte in the memory.
     * @see MemorySearch.
     * @param[in] mem is the memory target.
     * @param[in] c is the element to search.
     * @param[in] size is the number of bytes to check.
     * @return a pointer to the first occurence of c in mem, NULL if is not found or in case of mem==NULL.
     */
    static const void* Search(const void* mem,
                              int32 c,
                              uint32 size) {
        if (mem == NULL) {
            return NULL;
        }
        return memchr(mem, c, size);
    }

    /**
     * @brief Copy size bytes from source to destination.
     * @see MemoryMove.
     * @param[in] destination is a pointer to the destination memory.
     * @param[in] source is a pointer to source memory.
     * @param[in] size is the number of bytes to copy.
     * @return true if source, destination and destination after the copy are not NULL.
     */
    static bool Move(void* destination,
                     const void* source,
                     uint32 size) {
        if (source == NULL || destination == NULL) {
            return false;
        }
        return memmove(destination, source, size) != NULL;

    }

    /**
     * @brief Copy c in the first bytes of a memory.
     * @see MemorySet.
     * @param[in] mem is a pointer to the memory.
     * @param[in] c is the byte to copy.
     * @param[in] size is the number of bytes to set to c.
     * @return true if mem before and after the operation id not NULL.
     */
    static bool Set(void* mem,
                    int32 c,
                    uint32 size) {
        if (mem == NULL) {
            return false;
        }
        return memset(mem, c, size) != NULL;
    }
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYOS_H_ */

