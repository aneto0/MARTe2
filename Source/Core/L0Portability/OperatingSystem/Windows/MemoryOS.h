/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/
/**
 * @file
 * @brief Basic memory management in Windows
 */
#ifndef MEMORY_OS_H
#define MEMORY_OS_H

#include <stdlib.h>
#include <memory.h>
#include <string.h>


/**
 * @see MemoryMalloc
 */
void *MemoryOSMalloc(uint32 size, MemoryAllocationFlags allocFlags) {
    if (size <= 0) {
        return NULL;
    }
    return malloc(size);
}

/**
 * @see MemoryFree
 */
void MemoryOSFree(void *&data) {
    if (data != NULL) {
        free(data);
    }
    data = NULL;
}

/**
 * @see MemoryRealloc
 */
void *MemoryOSRealloc(void *&data, uint32 newSize) {
    return realloc(data, newSize);
}

/**
 * @see MemoryStringDup
 */
char *MemoryOSStringDup(const char *s) {
    return strdup(s);
}

/**
 * @see MemorySharedAlloc
 */
void *MemoryOSSharedAlloc(uint32 key, uint32 size, uint32 permMask) {
    return NULL;
}

/**
 * @see MemorySharedFree
 */
void MemoryOSSharedFree(void *address) {
    /* Do nothing */
}

/**
 * @see MemoryMemoryCheck
 */
bool MemoryOSCheck(void *address, MemoryTestAccessMode accessMode,
                   uint32 size) {
    if (accessMode & MTAM_Execute) {
        if (IsBadCodePtr((FARPROC) address))
            return False;
    }
    if (accessMode & MTAM_Read) {
        if (IsBadReadPtr(address, size))
            return False;
    }
    if (accessMode & MTAM_Write) {
        if (IsBadWritePtr(address, size))
            return False;
    }

    return True;
}


/** @see MemoryCopy.
  * @brief Copy source to destination.
  * @param destination is a pointer to the destination memory.
  * @param source is a pointer to the source memory.
  * @param is the number of bytes to copy.
  * @return true if source, destination and destination after the copy are not NULL. */
bool MemoryOsCopy(void* destination, const void* source, uint32 size) {
    if (source == NULL || destination == NULL) {
        return False;
    }

    return memcpy(destination, source, size) != NULL;
}

/** @see MemoryCompare.
  * @brief Compare two memory portions.
  * @param mem1 is a pointer to the first memory.
  * @param mem2 is a pointer to the second memory.
  * @param size is the size to compare.
  * @return (0 if mem1 < mem2), (1 if mem1 < mem2), (2 if mem1 > mem2), (-1 if one of memory arguments is NULL). */
int32 MemoryOsCompare(const void* mem1, const void* mem2, uint32 size) {
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

/** @see MemorySearch.
  * @brief Search a byte in the memory.
  * @param mem is the memory.
  * @param c is the element to search.
  * @param size is the number of bytes to check.
  * @return a pointer to the first occurence of c in mem, NULL if is not found or in case of mem=NULL.  */
void* MemoryOsSearch(void* mem, int32 c, uint32 size) {
    if (mem == NULL) {
        return NULL;
    }
    return memchr(mem, c, size);
}

/** @see MemoryMove.
  * @brief Copy size bytes from source to destination.
  * @param destination is a pointer to the destination memory.
  * @param source is a pointer to source memory.
  * @param size is the number of bytes to copy.
  * @return true if source, destination and destination after the copy are not NULL. */
bool MemoryOsMove(void* destination, const void* source, uint32 size) {
    if (source == NULL || destination == NULL) {
        return False;
    }
    return memmove(destination, source, size) != NULL;

}

/** @see MemorySet.
  * @brief Copy c in the first bytes of a memory.
  * @param mem is a pointer to the memory.
  * @param c is the byte to copy.
  * @param size is the number of bytes to set to c.
  * @return true if mem before and after the operation id not NULL. */
bool MemoryOsSet(void* mem, int32 c, uint32 size) {
    if (mem == NULL) {
        return False;
    }
    return memset(mem, c, size) != NULL;
}


#endif

