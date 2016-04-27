/**
 * @file MemoryArea.cpp
 * @brief Source file for class MemoryArea
 * @date 23/feb/2016
 * @author pc
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
 * the class MemoryArea (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryArea.h"
#include "MemoryOperationsHelper.h"
#include "StringHelper.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
MemoryArea::MemoryArea() {
    memory = NULL_PTR(void*);
    size = 0u;
    heapName = NULL_PTR(char8*);
}

/*lint -e{1579} the pointer "memory" is freed by the function Free() */
/*lint -e{1551} the destructor does not throw exceptions */
MemoryArea::~MemoryArea() {
    Free();
}

bool MemoryArea::InitMemory(void * initialMemory,
                            uint32 initialSize) {
    bool ret = (memory == NULL);
    if (ret) {
        memory = initialMemory;
        size = initialSize;
    }
    else {
        REPORT_ERROR(ErrorManagement::RecoverableError, "The memory is already in use. Call MemoryArea::Free(*) before");
    }
    return ret;
}

void MemoryArea::Free() {
    if (memory != NULL) {
        if(HeapManager::Free(reinterpret_cast<void*&>(memory))) {
            memory=NULL_PTR(void*);
        }
    }
    if(heapName!=NULL) {
        if(HeapManager::Free(reinterpret_cast<void*&>(heapName))) {
            heapName=NULL_PTR(char8*);
        }
    }
    size = 0u;
}

void MemoryArea::SetHeapName(const char8 * heapName) {
    heapName = StringHelper::StringDup(heapName);
}

bool MemoryArea::Add(const uint32 memorySize,
                     uint32 &offset) {
    bool ret = false;

    if (size == 0u) {
        memory = HeapManager::Malloc(memorySize, heapName);
        if (memory != NULL) {
            size += memorySize;
            offset=0u;
            ret=true;
        }
    }
    else {
        void* temp = memory;
        memory = HeapManager::Realloc(temp, (size + memorySize));
        if (memory != NULL) {
            offset=size;
            size+=memorySize;
            ret=true;
        }
    }
    return ret;
}

bool MemoryArea::Add(const void * const element,
                     const uint32 memorySize,
                     uint32 &offset) {
    bool ret = Add(memorySize, offset);
    if (ret) {
        ret = (MemoryOperationsHelper::Copy(&reinterpret_cast<char8*>(memory)[offset], element, memorySize));
    }
    return ret;
}

void* MemoryArea::GetMemoryStart() {
    return memory;
}

uint32 MemoryArea::GetMemorySize() const {
    return size;
}

void* MemoryArea::GetPointer(const uint32 offset) {
    void* ret = NULL_PTR(void*);
    if (offset < size) {
        /*lint -e{613} memory cannot be NULL here (because size==0)*/
        ret = &reinterpret_cast<char8*>(memory)[offset];
    }
    return ret;
}

}

