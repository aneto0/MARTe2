/**
 * @file StaticListHolder.cpp
 * @brief Source file for class StaticListHolder
 * @date Sep 2, 2015
 * @author fsartori
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
 * the class StaticListHolder (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "StaticListHolder.h"
#include "HeapManager.h"
#include "MemoryOperationsHelper.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Lists{

StaticListHolder::StaticListHolder(uint32 listElementSize, uint32 listAllocationGranularity){
    this->listElementSize           = listElementSize;
    this->listAllocationGranularity = listAllocationGranularity;

    uint32 granuleSize = listAllocationGranularity * listElementSize;

    uint32 maxValue = TypeDefinition::TypeCharacteristics<uint32>::MaxValue();

    maxListSize = maxValue /  granuleSize;
    maxListSize *= granuleSize;
    allocatedMemory = NULL_PTR(uint8 *);
    listAllocatedSize = 0;
    listSize = 0;

}

bool StaticListHolder::IncreaseListSize(){
    bool ret = true;
    uint32 templistSize = listSize+1;
    if (templistSize > listAllocatedSize){

        // cannot increase any more!
        if (listAllocatedSize >= maxListSize){
            ret = false;
        }

        if (ret){
            uint32 tempListAllocatedSize = listAllocatedSize + listAllocationGranularity;
            if (listAllocatedSize == 0){
                allocatedMemory = static_cast<uint8 *>(HeapManager::Malloc(tempListAllocatedSize*listElementSize));
                if (allocatedMemory != NULL_PTR(void *)){
                    listSize = templistSize;
                    listAllocatedSize = tempListAllocatedSize;
                } else {
                }
            } else {
                void *memoryPointer = allocatedMemory;
                allocatedMemory = static_cast<uint8 *>(HeapManager::Realloc(memoryPointer,tempListAllocatedSize*listElementSize));

                if (allocatedMemory != NULL_PTR(void *)){
                    listSize = templistSize;
                    listAllocatedSize = tempListAllocatedSize;
                } else {
                    allocatedMemory = static_cast<uint8 *>(memoryPointer);
                    ret = false;
                }
            }
        }
    }

    return ret;

}


bool StaticListHolder::ListPeek(const uint32 position, void *copyTo) const{
    bool ret = false;
    if (position <= listSize){
        ret = true;
        void * copyFrom = static_cast<void *>(allocatedMemory+listElementSize*position);
        MemoryOperationsHelper::Copy(copyTo,copyFrom,listElementSize);
    }
    return ret;
}

bool StaticListHolder::ListAdd(const uint32 position, const void *copyFrom){
    bool ret = true;
    if (position > listSize) {
        ret = false;
    }

    if (ret){
        ret = IncreaseListSize();
    }

    if (ret){
        uint8* pointer = (allocatedMemory+listElementSize*position);

        // note that listSize was increased earlier!
        if (position != (listSize-1)){
            // move all data in - note that listSize is already incremented by 1
            MemoryOperationsHelper::Move(pointer+listElementSize,pointer,listElementSize*(listSize-1-position));
        }

        MemoryOperationsHelper::Copy(pointer,copyFrom,listElementSize);
    }

    return ret;
}

bool StaticListHolder::ListExtract(const uint32 position, void *copyTo){
    bool ret = true;
    if (position >= listSize) {
        ret = false;
    }

    if (ret){
        uint8* pointer = (allocatedMemory+listElementSize*position);

        if (copyTo != NULL_PTR(void *)){
            MemoryOperationsHelper::Copy(copyTo,static_cast<void *>(pointer),listElementSize);
        }

        // not at the end - therefore must compact
        if (position != (listSize-1)){
            // move all data in - note that listSize is already incremented by 1
            MemoryOperationsHelper::Move(pointer,pointer+listElementSize,listElementSize*(listSize-1-position));
        }

        listSize--;
    }

    return ret;
}

}
