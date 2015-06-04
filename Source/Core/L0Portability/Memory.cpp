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
 * $Id: $
 *
 **/

#include "GeneralDefinitions.h"
#ifdef MEMORY_STATISTICS
#include "MemoryStatistics.h"
#endif
#include "Memory.h"

#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,MemoryOS.h)

void *MemoryMalloc(uint32 size, MemoryAllocationFlags allocFlags) {
    if(size == 0) {
        return NULL;
    }
    void *data = NULL;

#if defined(MEMORY_STATISTICS)
    data = MemoryStatisticsMalloc(size, allocFlags);
#else
    data = MemoryOSMalloc(size, allocFlags);
#endif
    return data;
}

void *MemoryRealloc(void *&data, uint32 newSize) {
    if (newSize == 0) {
        if (data == NULL) {
            return NULL;
        }
        MemoryFree(data);
        return NULL;
    }
    if (data == NULL) {
        data = MemoryMalloc(newSize);
        return data;
    }

#if defined(MEMORY_STATISTICS)
    data = MemoryStatisticsRealloc(data, newSize);
#else
    data = MemoryOSRealloc(data, newSize);
#endif

    return data;
}

char *MemoryStringDup(const char *s) {
    return MemoryOSStringDup(s);
}

void MemoryFree(void *&data) {
    if (data == NULL) {
        return;
    }
#if defined(MEMORY_STATISTICS)
    MemorySharedFree(data);
#else
    MemoryOSFree(data);
#endif
    data = NULL;
}

bool MemoryAllocationStatistics(uint32 &size, uint32 &chunks, TID tid) {
#if defined(MEMORY_STATISTICS)
    return MemoryStatisticsAllocationStatistics(size, chunks, tid);
#endif

    return False;
}

bool MemoryCheck(void *address, MemoryTestAccessMode accessMode, uint32 size) {
    return MemoryOSCheck(address, accessMode, size);
}

void *MemorySharedAlloc(uint32 key, uint32 size, uint32 permMask) {
    return MemoryOSSharedAlloc(key, size, permMask);
}

void MemorySharedFree(void *address) {
    MemoryOSSharedFree(address);
}

bool MemoryCopy(void* destination, const void* source, uint32 size) {
    return MemoryOsCopy(destination, source, size);
}

int32 MemoryCompare(const void* mem1, const void* mem2, uint32 size) {
    return MemoryOsCompare(mem1, mem2, size);
}

void *MemorySearch(void* mem, char c, uint32 size) {
    return MemoryOsSearch(mem, c, size);
}

bool MemoryMove(void* destination, const void* source, uint32 size) {
    return MemoryOsMove(destination, source, size);
}

bool MemorySet(void* mem, char c, uint32 size) {
    return MemoryOsSet(mem, c, size);
}

