/**
 * @file Memory.cpp
 * @brief Source file for class Memory
 * @date 18/06/2015
 * @author Giuseppe Ferrò
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

 * @details This source file contains the definition of all the methods for
 * the class Memory (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Memory.h"
#include "Threads.h"
#ifdef MEMORY_STATISTICS
#include "MemoryStatisticsDatabase.h"
#endif

#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
MemoryAllocationFlags Memory::defaultAllocationFlag = MemoryStandardMemory;

#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,MemoryOS.h)

/**
 * @brief A structure with memory informations used as a memory area header.
 * @details if Memory::defaultAllocationFlag is setted to MemoryAddHeader for each allocation an header with the informations
 * defined in this structure will be allocated too. This allows to the user to know all the informations related to the heap memory allocated.
 */
struct MemoryInformation {

    /** A variable used to recognize the header. */
    static const unsigned char code=0xff;

    /** The identifier of the thread that allocates the memory. */
    TID tid;

    /** The size of the memory */
    uint32 size;

    /**
     * @brief Initializes the structure.
     * @details the tid attribute is filled with the id of the thread that calls this function.
     * @param[in] sizeArg is the size of the memory.
     */
    void Init(uint32 sizeArg) {
        size = sizeArg;
        tid = Threads::Id();
    }

};

void *MemoryMalloc(uint32 size) {
    if (size == 0) {
        return NULL;
    }
    void *data = NULL;

    if (Memory::defaultAllocationFlag & MemoryAddHeader) {
        //add the memory information space
        size += sizeof(MemoryInformation);
    }

    data = MemoryOS::Malloc(size, Memory::defaultAllocationFlag);


    //return null if malloc fails
    if (data != NULL) {
        if (Memory::defaultAllocationFlag & MemoryAddHeader) {
            //add the header
            MemoryInformation *header = (MemoryInformation*) data;
            header->Init(size);
            //return the beginning of the memory
            header++;
            data = (void *) header;
        }
#ifdef MEMORY_STATISTICS

        if (Memory::defaultAllocationFlag & MemoryStatistics) {
            //add the information to the database
            MemoryStatisticsDatabase::AddMemoryChunk(Threads::Id(), size);
        }

#endif
    }

    return data;

}

void *MemoryRealloc(void *&data,
                    uint32 newSize) {
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

#ifdef MEMORY_STATISTICS
    uint32 sizeToFree = 0;
#endif

    if (Memory::defaultAllocationFlag & MemoryAddHeader) {
        //add the memory information space
        newSize += sizeof(MemoryInformation);
#ifdef MEMORY_STATISTICS
        MemoryInformation *header = (MemoryInformation*) data - 1;

        //if the header is valid set the size to free
        if(header!=NULL) {
            if(header->code==0xff) {
                sizeToFree = header->size;
            }
        }
#endif
    }

    data = MemoryOS::Realloc(data, newSize);

    //if data is null return directly because the realloc fails.
    if (data != NULL) {

        if (Memory::defaultAllocationFlag & MemoryAddHeader) {
            //add the header
            MemoryInformation *header = (MemoryInformation*) data;
            header->Init(newSize);
            //return to the beginning of the memory
            header++;
            data = (void *) header;
        }

#ifdef MEMORY_STATISTICS

        if (Memory::defaultAllocationFlag & MemoryStatistics) {
            //add the information to the database
            MemoryStatisticsDatabase::FreeMemoryChunk(Threads::Id(), sizeToFree);
            MemoryStatisticsDatabase::AddMemoryChunk(Threads::Id(), newSize);
        }

#endif
    }
    return data;
}

char *MemoryStringDup(const char *s) {
    return MemoryOS::StringDup(s);
}

bool MemoryFree(void *&data) {
    if (data == NULL) {
        return False;
    }
#ifdef MEMORY_STATISTICS
    uint32 sizeToFree = 0;
#endif

    if (Memory::defaultAllocationFlag & MemoryAddHeader) {
        //return at the beginning
        MemoryInformation *header = (MemoryInformation*) data - 1;

        //the pointer is invalid
        if (header == NULL) {
            return False;
        }

        //return false if the test fail. This means that the header is invalid.
        if ((header)->code != 0xff) {
            return False;
        }

        data = (void*) header;

#ifdef MEMORY_STATISTICS
        sizeToFree = header->size;
#endif

    }

    //free the memory
    MemoryOS::Free(data);

#ifdef MEMORY_STATISTICS

    if (Memory::defaultAllocationFlag & MemoryStatistics) {
        MemoryStatisticsDatabase::FreeMemoryChunk(Threads::Id(), sizeToFree);
    }

#endif
    data = NULL;
    return True;
}

bool MemoryAllocationStatistics(uint32 &size,
                                uint32 &chunks,
                                TID tid) {
#ifdef MEMORY_STATISTICS
    printf("\nim here!\n");

    if (tid == 0xFFFFFFFF) {
        tid = Threads::Id();
    }

    if (Memory::defaultAllocationFlag & MemoryStatistics) {
        ThreadAllocationStatistics *ret = MemoryStatisticsDatabase::Find(tid);
        if (ret != NULL) {
            printf("\nim here2!\n");

            size = ret->totalMemorySize;
            chunks = ret->nOfMemoryChunks;
        }
        return ret != NULL;
    }
    else {
        return False;
    }

#else

    return False;

#endif

}

bool MemoryGetHeaderInfo(void *pointer,
                         uint32 &size,
                         TID &tid) {

    //invalid pointer
    if (pointer == NULL) {
        return False;
    }

    if (Memory::defaultAllocationFlag & MemoryAddHeader) {
        MemoryInformation *header = (MemoryInformation*) pointer - 1;

        //invalid pointer
        if (header == NULL) {
            return False;
        }

        //invalid header
        if(header->code != 0xff){
            return False;
        }

        size = header->size;
        tid = header->tid;
        return True;
    }

    //the header is not supported
    return False;
}

bool MemoryCheck(void *address,
                 MemoryTestAccessMode accessMode,
                 uint32 size) {
    return MemoryOS::Check(address, accessMode, size);
}

void *MemorySharedAlloc(uint32 key,
                        uint32 size,
                        uint32 permMask) {
    return MemoryOS::SharedAlloc(key, size, permMask);
}

void MemorySharedFree(void *address) {
    MemoryOS::SharedFree(address);
}

bool MemoryCopy(void* destination,
                const void* source,
                uint32 size) {
    return MemoryOS::Copy(destination, source, size);
}

int32 MemoryCompare(const void* mem1,
                    const void* mem2,
                    uint32 size) {
    return MemoryOS::Compare(mem1, mem2, size);
}

const void *MemorySearch(const void* mem,
                         char c,
                         uint32 size) {
    return MemoryOS::Search(mem, c, size);
}

bool MemoryMove(void* destination,
                const void* source,
                uint32 size) {
    return MemoryOS::Move(destination, source, size);
}

bool MemorySet(void* mem,
               char c,
               uint32 size) {
    return MemoryOS::Set(mem, c, size);
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void *Memory::Malloc(uint32 size) {
    return MemoryMalloc(size);
}

bool Memory::Free(void *&data) {
    return MemoryFree(data);
}

void *Memory::Realloc(void *&data,
                      uint32 newSize) {
    return MemoryRealloc(data, newSize);
}

char *Memory::StringDup(const char *s) {
    return MemoryStringDup(s);
}

bool Memory::AllocationStatistics(uint32 &size,
                                  uint32 &chunks,
                                  TID tid) {
    return MemoryAllocationStatistics(size, chunks, tid);
}

bool Memory::GetHeaderInfo(void *pointer,
                           uint32 &size,
                           TID &tid) {
    return MemoryGetHeaderInfo(pointer, size, tid);
}

bool Memory::Check(void *address,
                   MemoryTestAccessMode accessMode,
                   uint32 size) {
    return MemoryCheck(address, accessMode, size);
}

void *Memory::SharedAlloc(uint32 key,
                          uint32 size,
                          uint32 permMask) {
    return MemorySharedAlloc(key, size, permMask);
}

void Memory::SharedFree(void *address) {
    MemorySharedFree(address);
}

bool Memory::Copy(void *destination,
                  const void *source,
                  uint32 size) {
    return MemoryCopy(destination, source, size);
}

int32 Memory::Compare(const void *mem1,
                      const void *mem2,
                      uint32 size) {
    return MemoryCompare(mem1, mem2, size);
}
const void *Memory::Search(const void *mem,
                           char c,
                           uint32 size) {
    return MemorySearch(mem, c, size);
}

bool Memory::Move(void* destination,
                  const void* source,
                  uint32 size) {
    return MemoryMove(destination, source, size);
}

bool Memory::Set(void* mem,
                 char c,
                 uint32 size) {
    return MemorySet(mem, c, size);
}
