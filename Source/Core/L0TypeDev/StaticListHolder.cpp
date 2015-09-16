/**
 * @file StaticListHolder.cpp
 * @brief Source file for class StaticListHolder
 * @date 31/08/2015
 * @author Filippo Sartori
 * @author Ivan Herrero
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

#include "StaticListHolder.h"
#include "HeapManager.h"
#include "MemoryOperationsHelper.h"
#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Lists {

StaticListHolder::StaticListHolder(const uint32 listElementSize,
                                   const uint32 listAllocationGranularity) {
    listElementSize_ = listElementSize;
    listAllocationGranularity_ = listAllocationGranularity;

    uint32 granuleSize = listAllocationGranularity_ * listElementSize_;

    uint32 maxValue = TypeDefinition::TypeCharacteristics::MaxValue<uint32>();

    maxListCapacity_ = maxValue / granuleSize;
    maxListCapacity_ *= granuleSize;
    allocatedMemory_ = NULL_PTR(uint8 *);
    listCapacity_ = 0U;
    listSize_ = 0U;

}

StaticListHolder::~StaticListHolder() {
    //TODO Free allocatedMemory_
}

uint32 StaticListHolder::GetElementSize(void) const {
    return listElementSize_;
}

uint32 StaticListHolder::GetAllocationGranularity(void) const {
    return listAllocationGranularity_;
}

uint32 StaticListHolder::GetSize(void) const {
    return listSize_;
}

uint32 StaticListHolder::GetCapacity(void) const {
    return listCapacity_;
}

bool StaticListHolder::Add(const void *copyFrom) {
    bool ret = true;

    // Increases capacity if after adding an element, the list is going to fall short of space.
    if ((listSize_ + 1u) >= listCapacity_) {
        //Checks if new capacity will be on range and calls IncreaseCapacity or signals error otherwise.
        if ((listCapacity_ + listAllocationGranularity_) <= maxListCapacity_) {
            ret = IncreaseCapacity();
        }
        else {
            ret = false;
        }
    }

    //Copies the element to the end of the list
    if (ret) {
        uint8* pointer = (allocatedMemory_ + (listElementSize_ * listSize_));
        ret = MemoryOperationsHelper::Copy(pointer, copyFrom, listElementSize_);
        if (ret) {
            listSize_++;
        }
    }

    return ret;
}

bool StaticListHolder::Insert(const uint32 position,
                              const void *copyFrom) {
    bool ret = true;

    if (position > listSize_) {
        ret = false;
    }

    // Increases capacity if after adding an element, the list is going to fall short of space.
    if ((listSize_ + 1u) >= listCapacity_) {
        //Checks if new capacity will be on range and calls IncreaseCapacity or signals error otherwise.
        if ((listCapacity_ + listAllocationGranularity_) <= maxListCapacity_) {
            ret = IncreaseCapacity();
        }
        else {
            ret = false;
        }
    }

    if (ret) {
        uint8* pointer = (allocatedMemory_ + listElementSize_ * position);

        // note that listSize was increased earlier!
        if (position != (listSize_)) {
            // move all data in - note that listSize is already incremented by 1
            ret = MemoryOperationsHelper::Move(pointer + listElementSize_, pointer, listElementSize_ * ((listSize_) - position));
        }

        if (ret) {
            ret = MemoryOperationsHelper::Copy(pointer, copyFrom, listElementSize_);
        }

        if (ret) {
            listSize_++;
        }
    }

    return ret;

}

bool StaticListHolder::Peek(const uint32 position,
                            void *copyTo) const {
    bool ret = true;
    if (position <= listSize_) {
        void * copyFrom = static_cast<void *>(allocatedMemory_ + listElementSize_ * position);
        ret = MemoryOperationsHelper::Copy(copyTo, copyFrom, listElementSize_);
    }
    else {
        ret = false;
    }
    return ret;
}

bool StaticListHolder::Extract(const uint32 position,
                               void *copyTo) {
    bool ret = true;

    if (position >= listSize_) {
        ret = false;
    }

    if (ret) {
        uint8* pointer = (allocatedMemory_ + (listElementSize_ * position));

        if (copyTo != NULL_PTR(void *)) {
            ret = MemoryOperationsHelper::Copy(copyTo, static_cast<void *>(pointer), listElementSize_);
        }

        if (ret) {
            // not at the end - therefore must compact
            if (position != (listSize_ - 1u)) {
                // move all data in - note that listSize is already incremented by 1
                ret = MemoryOperationsHelper::Move(pointer, pointer + listElementSize_, listElementSize_ * ((listSize_ - 1U) - position));
            }
            if (ret) {
                listSize_--;
            }
        }
    }

    return ret;
}

bool StaticListHolder::IncreaseCapacity(void) {
    bool ret = true;
    if ((listCapacity_ + listAllocationGranularity_) > maxListCapacity_) {
        ret = false;
    }
    if (ret) {
        uint32 tempListAllocatedSize = listCapacity_ + listAllocationGranularity_;
        if (listCapacity_ == 0U) {
            allocatedMemory_ = static_cast<uint8 *>(HeapManager::Malloc(tempListAllocatedSize * listElementSize_));
            if (allocatedMemory_ != NULL_PTR(void *)) {
                listCapacity_ = tempListAllocatedSize;
            }
            else {
                ret = false;
            }
        }
        else { // { listAllocatedSize_ > 0U }
            void *memoryPointer = allocatedMemory_;
            allocatedMemory_ = static_cast<uint8 *>(HeapManager::Realloc(memoryPointer, tempListAllocatedSize * listElementSize_));
            if (allocatedMemory_ != NULL_PTR(void *)) {
                listCapacity_ = tempListAllocatedSize;
            }
            else {
                //TODO It is assured that memoryPointer is still pointing to a valid allocated memory?
                allocatedMemory_ = static_cast<uint8 *>(memoryPointer);
                ret = false;
            }
        }
    }
    return ret;
}

}
