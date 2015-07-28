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
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <Windows.h>
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Memory.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

void *Memory::Malloc(uint32 size,
                     MemoryAllocationFlags allocFlags) {

    void* data = NULL;

    if (size != 0) {
        return malloc(size);
    }

    return data;
}

void Memory::Free(void *&data) {

    if (data != NULL) {
        free(data);
        data = NULL;
    }

}

void *Memory::Realloc(void *&data,
                      uint32 newSize) {
    bool ok = true;

    if (data == NULL) {
        data = Malloc(newSize);
    }
    else {
        if (newSize == 0) {
            Free(data);
        }
        else {
            data = realloc(data, newSize);
        }
    }
    return data;

}

char8 *Memory::StringDup(const char8 *s) {

    char8 *sCopy = NULL;

    if (s != NULL) {
        sCopy = strdup(s);
    }
    return sCopy;
}

bool Memory::Check(void *address,
                   MemoryTestAccessMode accessMode,
                   uint32 size) {
    uint8 check = 0;
    //determines if the calling process has read access to the specified address (if the process has read and execute permissions).
    if (accessMode & MTAM_Execute) {
        check++;
        if (IsBadCodePtr((FARPROC) address)) {
            return false;
        }
    }

    //determines if the calling process has the read access to the specified range of memory (if the process has read permissions).
    if (accessMode & MTAM_Read) {
        check++;
        if (IsBadReadPtr(address, size)) {
            return false;
        }
    }

    //determines if the calling process has the write access to the specified range of memory (if the process has write permissions).
    if (accessMode & MTAM_Write) {
        check++;
        if (IsBadWritePtr(address, size)) {
            return false;
        }
    }

    return address != NULL && check != 0;
}

bool Memory::Copy(void* destination,
                  const void* source,
                  uint32 size) {

    bool ret = false;
    if (source != NULL && destination != NULL) {
        ret = memcpy(destination, source, size) != NULL;
    }

    return ret;

}

int32 Memory::Compare(const void* mem1,
                      const void* mem2,
                      uint32 size) {

    int32 ret = -1;

    if (mem1 != NULL && mem2 != NULL) {
        int32 temp = memcmp(mem1, mem2, size);
        if (temp < 0) {
            ret = 1; // 1 if mem1<mem2
        }
        if (temp > 0) {
            ret = 2; // 2 if mem1>mem2
        }
        if (temp == 0) {
            ret = 0; // 0 if mem1==mem2
        }
    }

    return ret;

}

const void* Memory::Search(const void* mem,
                           int32 c,
                           uint32 size) {
    const void* ret = NULL;
    if (mem != NULL) {

        ret = memchr(mem, c, size);

    }

    return ret;
}

bool Memory::Move(void* destination,
                  const void* source,
                  uint32 size) {

    bool ret = false;

    if (source != NULL && destination != NULL) {

        ret = memmove(destination, source, size) != NULL;
    }
    return ret;

}

bool Memory::Set(void* mem,
                 int32 c,
                 uint32 size) {
    bool ret = false;
    if (mem != NULL) {

        return memset(mem, c, size) != NULL;
    }

    return ret;

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYOS_H_ */

