/**
 * @file MemoryOS.cpp
 * @brief Source file for class MemoryOS
 * @date 27/07/2015
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
 * the class MemoryOS (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "FreeRTOS.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "Memory.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void *Memory::Malloc(const uint32 size, const MemoryAllocationFlags allocFlag) {

    void* data = static_cast<void*>(NULL);

    if (size != 0u) {
        data = pvPortMalloc(static_cast<size_t>(size));
    }

    return data;
}

void Memory::Free(void *&data) {

    if (data != NULL) {
        vPortFree(data);
        data = static_cast<void*>(NULL);
    }

}

void *Memory::Realloc(void *&data, const uint32 newSize) {

    if (data == NULL) {
        data = Malloc(newSize);
    }
    else {
        if (newSize == 0u) {
            Free(data);
        }
        else {
            void *oldData = data;
            data = Malloc(newSize);
            //Will copy garbage as well...
            Copy(data, oldData, newSize);
            Free(oldData);
        }
    }
    return data;

}

char8 *Memory::StringDup(const char8 * const s) {
    //TODO
    return NULL;
}

bool Memory::Check(const void * const address, const MemoryTestAccessMode accessMode, const uint32 size) {
    return address != NULL;
}

bool Memory::Copy(void* const destination, const void * const source, const uint32 size) {

    //TODO
    /*bool ret = false;
     if ((source != NULL) && (destination != NULL)) {
     ret = memcpy(destination, source, static_cast<size_t>(size)) != NULL;
     }

     return ret;*/
    return NULL;

}

int32 Memory::Compare(const void * const mem1, const void * const mem2, const uint32 size) {

    //TODO
    /*int32 ret = -1;

     if ((mem1 != NULL) && (mem2 != NULL)) {
     int32 temp = memcmp(mem1, mem2, static_cast<size_t>(size));
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

     return ret;*/
    return 0;

}

const void* Memory::Search(const void * const mem, const char8 c, const uint32 size) {
    //TODO
    /*const void* ret = static_cast<const void*>(NULL);
     if (mem != NULL) {

     ret = memchr(mem, c, static_cast<size_t>(size));

     }

     return ret;*/
    return NULL;
}

bool Memory::Move(void * const destination, const void * const source, const uint32 size) {

    //TODO
    /*bool ret = false;

     if ((source != NULL) && (destination != NULL)) {

     ret = memmove(destination, source, static_cast<size_t>(size)) != NULL;
     }
     return ret;*/
    return NULL;

}

bool Memory::Set(void * const mem, const char8 c, const uint32 size) {
    //TODO
    /*bool ret = false;
     if (mem != NULL) {

     ret = memset(mem, c, static_cast<size_t>(size)) != NULL;
     }

     return ret;*/
    return NULL;

}
