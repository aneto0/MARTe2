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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryArea.h"
#include "MemoryOperationsHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
MemoryArea::MemoryArea() {
    memory = NULL;
    size=0u;
}

MemoryArea::~MemoryArea() {
    if(Free()){

    }
}

bool MemoryArea::Free(){
    bool ret=false;
    if (memory != NULL) {
        if(HeapManager::Free(reinterpret_cast<void*&>(memory))) {
            ret=true;
            memory=NULL;
        }
    }
    size=0u;
    return ret;
}

void* MemoryArea::Add(uint32 memorySize) {
    char8* ret = NULL_PTR(char8*);

    if (size == 0u) {
        memory = HeapManager::Malloc(memorySize);
        if (memory != NULL) {
            ret = reinterpret_cast<char8 *>(memory);
            size += memorySize;
        }
    }
    else {
        void* temp = memory;
        memory = HeapManager::Realloc(temp, (size + memorySize));
        if (memory != NULL) {
            ret=&reinterpret_cast<char8*>(memory)[size];
            size+=memorySize;
        }
    }
    return reinterpret_cast<void*>(ret);
}


void* MemoryArea::Add(void* element, uint32 memorySize) {
    void* ret = Add(memorySize);

    if(!MemoryOperationsHelper::Copy(ret, element, memorySize)){
        //TODO
    }

    return ret;
}


}

