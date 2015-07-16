/**
 * @file Memory.cpp
 * @brief Source file for class Memory
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


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,MemoryOS.h)



void *MemoryMalloc(const uint32 size, const MemoryAllocationFlag &allocFlag) {

    void *data = static_cast<void*>(NULL);

    if (size != 0U) {

        data = MemoryOS::Malloc(size, allocFlag);

    }
    return data;

}

void *MemoryRealloc(void *&data, const uint32 newSize) {
    if ( (newSize == 0U) && (data != NULL) ){
        // newSize == 0 and data not null -> we are asking for a free()
        // MemoryFree already changes data to null
        if (!MemoryFree(data)) {
            data = static_cast<void*>(NULL);
        }
    } else if ( (newSize != 0U) && (data == NULL) ) {
        // newSize != 0 and data null -> we are asking for a malloc()
        data = MemoryMalloc(newSize);
    } else if ( (newSize == 0U) && (data == NULL) ) {
        // newSize == 0 and data null -> no sense, just return NULL
        data = static_cast<void*>(NULL);
    } else {
        // newSize != 0 and data not null -> we are really asking for a realloc()

        data = MemoryOS::Realloc(data, newSize);
    }

    return data;
}

char8 *MemoryStringDup(char8 const * const s) {


    char8 *sCopy = static_cast<char8*>(NULL);
    if (s != NULL) {
        sCopy = MemoryOS::StringDup(s);
    }

    return sCopy;

}

bool MemoryFree(void *&data) {
    bool ret = false;
    if (data != NULL) {
        //free the memory
        MemoryOS::Free(data);
        ret = true;
    }
    data = static_cast<void*>(NULL);
    return ret;
}


bool MemoryCheck(const void* const address, const MemoryTestAccessMode &accessMode, const uint32 size) {
    return MemoryOS::Check(address, accessMode, size);
}

void *MemorySharedAlloc(const uint32 key, const uint32 size, const uint32 permMask) {
    return MemoryOS::SharedAlloc(key, size, permMask);
}

void MemorySharedFree(void const *&address) {
    if (address != NULL) {
        MemoryOS::SharedFree(address);
        address = static_cast<void*>(NULL);
    }

    return;
}

bool MemoryCopy(void* const destination, const void* const source, const uint32 size) {
    bool ret = false;

    if ( (source != NULL) && (destination != NULL) ) {
        ret = MemoryOS::Copy(destination, source, size);
    }

    return ret;
}

int32 MemoryCompare(const void* const mem1, const void* const mem2, const uint32 size) {
    int32 ret = -1;

    if ( (mem1 != NULL) && (mem2 != NULL) ) {
        ret = MemoryOS::Compare(mem1, mem2, size);
    }
    return ret;
}

const void *MemorySearch(void const * const mem, const char8 c, const uint32 size) {
    const void* ret = static_cast<void*>(NULL);

    if (mem != NULL) {
        ret = MemoryOS::Search(mem, c, size);
    }
    return ret;
}

bool MemoryMove(void * const destination, void const * const source, const uint32 size) {
    bool ret = false;

    if ( (source != NULL) && (destination != NULL) ) {
        ret = MemoryOS::Move(destination, source, size);
    }
    return ret;
}

bool MemorySet(void* const mem, const char8 c, const uint32 size) {
    bool ret = false;

    if (mem != NULL) {
        ret = MemoryOS::Set(mem, c, size);
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void *Memory::Malloc(const uint32 size, const MemoryAllocationFlag &allocFlag) {
    return MemoryMalloc(size, allocFlag);
}

bool Memory::Free(void *&data) {
    return MemoryFree(data);
}

void *Memory::Realloc(void *&data, const uint32 newSize) {
    return MemoryRealloc(data, newSize);
}

char8 *Memory::StringDup(char8 const * const s) {
    return MemoryStringDup(s);
}


bool Memory::Check(const void* const address, const MemoryTestAccessMode &accessMode, const uint32 size) {
    return MemoryCheck(address, accessMode, size);
}

void *Memory::SharedAlloc(const uint32 key, const uint32 size, const uint32 permMask) {
    void* ret = static_cast<void*>(NULL);

    if (size != 0U) {
        ret = MemorySharedAlloc(key, size, permMask);
    }
    return ret;
}

void Memory::SharedFree(void const *&address) {
    MemorySharedFree(address);
}

bool Memory::Copy(void* const destination, const void* const source, const uint32 size) {
    return MemoryCopy(destination, source, size);
}

int32 Memory::Compare(const void* const mem1, const void* const mem2, const uint32 size) {
    return MemoryCompare(mem1, mem2, size);
}
const void *Memory::Search(void const * const mem, const char8 c, const uint32 size) {
    return MemorySearch(mem, c, size);
}

bool Memory::Move(void* const destination, void const * const source, const uint32 size) {
    return MemoryMove(destination, source, size);
}

bool Memory::Set(void* const mem, const char8 c, const uint32 size) {
    return MemorySet(mem, c, size);
}
