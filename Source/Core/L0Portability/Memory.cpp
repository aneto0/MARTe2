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
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,MemoryOS.h)

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
void *Memory::Malloc(uint32 size,
        MemoryAllocationFlags allocFlag) {

    void* data=NULL;

    if (size != 0) {

        data = MemoryOS::Malloc(size, allocFlag);

    }

    return data;

}

void *Memory::Realloc(void *&data,
                      uint32 newSize) {

    bool ok = true;

    if (data == NULL) {
        data = MemoryMalloc(newSize);
    }
    else {
        if (newSize == 0) {
            MemoryFree(data);
        }
        else {
            data = MemoryOS::Realloc(data, newSize);
        }
    }
    return data;
}

char8 *Memory::StringDup(const char8 *s) {

    char8 *sCopy = NULL;

    if (s != NULL) {

        sCopy = MemoryOS::StringDup(s);

    }

    return sCopy;

}

void Memory::Free(void *&data) {

    if (data != NULL) {
        MemoryOS::Free(data);
        data = NULL;
    }

}

bool Memory::Check(void *address,
                   MemoryTestAccessMode accessMode,
                   uint32 size) {
    return MemoryOS::Check(address, accessMode, size);
}

bool Memory::Copy(void* destination,
                  const void* source,
                  uint32 size) {

    bool ret = false;
    if (source != NULL && destination != NULL) {
        ret = MemoryOS::Copy(destination, source, size);
    }

    return ret;
}

int32 Memory::Compare(const void* mem1,
                      const void* mem2,
                      uint32 size) {
    int32 ret = -1;

    if (mem1 != NULL && mem2 != NULL) {

        ret = MemoryOS::Compare(mem1, mem2, size);
    }

    return ret;
}

const void *Memory::Search(const void* mem,
                           char8 c,
                           uint32 size) {

    const void* ret = NULL;
    if (mem != NULL) {

        ret = MemoryOS::Search(mem, c, size);

    }

    return ret;
}

bool Memory::Move(void* destination,
                  const void* source,
                  uint32 size) {

    bool ret = false;

    if (source != NULL && destination != NULL) {

        ret = MemoryOS::Move(destination, source, size);
    }
    return ret;
}

bool Memory::Set(void* mem,
                 char8 c,
                 uint32 size) {

    bool ret = false;
    if (mem != NULL) {

        return MemoryOS::Set(mem, c, size);
    }

    return ret;
}

