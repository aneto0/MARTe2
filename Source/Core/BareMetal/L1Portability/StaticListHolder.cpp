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
#define DLL_API
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

namespace MARTe {

StaticListHolder::StaticListHolder(const uint32 listElementSize,
                                   const uint32 listAllocationGranularity) :
        //Initializes the element size
        listElementSize_(listElementSize),

        //Initializes the allocation granularity
        listAllocationGranularity_(listAllocationGranularity),

        //Calculates and initializes the maximum capacity
        maxListCapacity_(
                ((MAX_UINT32 / (listAllocationGranularity_ * listElementSize_)) * (listAllocationGranularity_ * listElementSize_))
                        / listElementSize_),

        //Initializes the pointer to the first byte of the array of elements
        allocatedMemory_(NULL_PTR(uint8 *)),

        //Initializes the actual capacity
        listCapacity_(0U),

        //Initializes the actual size
        listSize_(0U) {
}

StaticListHolder::~StaticListHolder() {
    if (allocatedMemory_ != NULL_PTR(uint8 *)) {
        void * pointer = static_cast<void *>(allocatedMemory_);
        /*lint -e{1551} HeapManager::Free is expected to be exception free*/
        /*lint -e{534} It is not necessary to check for error/result on
         * HeapManager::Free result, because allocatedMemory_ was initialized
         * by IncreaseCapacity*/
        HeapManager::Free(pointer);
    }
    /*lint -e{1740} allocatedMemory_ was zero or it is freed and zeroed by HeapManager::Free*/
}

uint32 StaticListHolder::GetElementSize(void) const {
    return listElementSize_;
}

uint32 StaticListHolder::GetAllocationGranularity(void) const {
    return listAllocationGranularity_;
}

uint32 StaticListHolder::GetMaxCapacity(void) const {
    return maxListCapacity_;
}

uint32 StaticListHolder::GetSize(void) const {
    return listSize_;
}

uint32 StaticListHolder::GetCapacity(void) const {
    return listCapacity_;
}

bool StaticListHolder::Add(const void * const copyFrom) {
    bool ret = true;

    //Checks the precondition
    if (copyFrom == NULL_PTR(void *)) {
        ret = false;
    }

    // Increases capacity if after adding an element, the list is going to fall short of space.
    if (ret) {
        if ((listSize_ + 1u) > listCapacity_) {
            //Checks if new capacity will be on range and calls IncreaseCapacity or signals error otherwise.
            if ((listCapacity_ + listAllocationGranularity_) <= maxListCapacity_) {
                ret = IncreaseCapacity();
            }
            else {
                ret = false;
            }
        }
    }

    //Copies the element to the back of the list
    if (ret) {
        /*lint -e{9016} This implementation uses pointer arithmetic instead of array indexing*/
        /*lint -e{679} This implementation uses pointer arithmetic instead of array indexing*/
        uint8* pointer = (allocatedMemory_ + (listElementSize_ * listSize_));
        ret = MemoryOperationsHelper::Copy(pointer, copyFrom, listElementSize_);
        if (ret) {
            listSize_++;
        }
    }

    return ret;
}

bool StaticListHolder::Insert(const uint32 position,
                              const void * const copyFrom) {
    bool ret = true;

    //Checks the precondition
    if ((position > listSize_) || (copyFrom == NULL_PTR(void *))) {
        ret = false;
    }

    // Increases capacity if after adding an element, the list is going to fall short of space.
    if (ret) {
        if ((listSize_ + 1u) > listCapacity_) {
            //Checks if new capacity will be on range and calls IncreaseCapacity or signals error otherwise.
            if ((listCapacity_ + listAllocationGranularity_) <= maxListCapacity_) {
                ret = IncreaseCapacity();
            }
            else {
                ret = false;
            }
        }
    }

    //Copies the element to the requested position of the list and moves previous elements to right if required
    if (ret) {
        /*lint -e{9016} This implementation uses pointer arithmetic instead of array indexing*/
        /*lint -e{679} This implementation uses pointer arithmetic instead of array indexing*/
        uint8* pointer = (allocatedMemory_ + (listElementSize_ * position));

        if (position < (listSize_)) {
            //Expands the array moving the elements to their right position
            /*lint -e{9016} This implementation uses pointer arithmetic instead of array indexing*/
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
                            void * const copyTo) const {
    bool ret = true;

    //Checks the precondition
    if ((position >= listSize_) || (copyTo == NULL_PTR(void *))) {
        ret = false;
    }

    //Retrieves the element from the requested position of the list
    if (ret) {
        if (position <= listSize_) {
            /*lint -e{9016} This implementation uses pointer arithmetic instead of array indexing*/
            /*lint -e{679} This implementation uses pointer arithmetic instead of array indexing*/
            void * copyFrom = static_cast<void *>(allocatedMemory_ + (listElementSize_ * position));
            ret = MemoryOperationsHelper::Copy(copyTo, copyFrom, listElementSize_);
        }
    }

    return ret;
}

bool StaticListHolder::Remove(const uint32 position) {
    bool ret = true;

    //Checks the precondition
    if (position >= listSize_) {
        ret = false;
    }

    //Removes the element from the requested position on the list and moves previous elements to left if required
    if (ret) {
        /*lint -e{9016} This implementation uses pointer arithmetic instead of array indexing*/
        /*lint -e{679} This implementation uses pointer arithmetic instead of array indexing*/
        uint8* pointer = (allocatedMemory_ + (listElementSize_ * position));

        if (position < (listSize_ - 1u)) {
            //Compacts the array moving the elements to their left position
            /*lint -e{9016} This implementation uses pointer arithmetic instead of array indexing*/
            ret = MemoryOperationsHelper::Move(pointer, pointer + listElementSize_, listElementSize_ * ((listSize_ - 1U) - position));
        }

        if (ret) {
            listSize_--;
        }

    }

    return ret;
}

bool StaticListHolder::Extract(const uint32 position,
                               void * const copyTo) {
    bool ret = true;

    //Checks the precondition
    if ((position >= listSize_) || (copyTo == NULL_PTR(void *))) {
        ret = false;
    }

    //Peeks and removes the element from the requested position on the list and moves previous elements to left if required
    if (ret) {
        /*lint -e{9016} This implementation uses pointer arithmetic instead of array indexing*/
        /*lint -e{679} This implementation uses pointer arithmetic instead of array indexing*/
        uint8* pointer = (allocatedMemory_ + (listElementSize_ * position));

        ret = MemoryOperationsHelper::Copy(copyTo, static_cast<void *>(pointer), listElementSize_);

        if (ret) {
            if (position < (listSize_ - 1u)) {
                //Compacts the array moving the elements to their left position
                /*lint -e{9016} This implementation uses pointer arithmetic instead of array indexing*/
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

    //Checks the precondition
    if ((listCapacity_ + listAllocationGranularity_) > maxListCapacity_) {
        ret = false;
    }

    //Allocates or reallocates the memory reserved for the array depending on current allocated memory
    if (ret) {
        uint32 tempListAllocatedSize = listCapacity_ + listAllocationGranularity_;
        if (listCapacity_ == 0U) {
            //The array has not memory reserved, yet, so it allocates memory for it
            allocatedMemory_ = static_cast<uint8 *>(HeapManager::Malloc(tempListAllocatedSize * listElementSize_));
            if (allocatedMemory_ != NULL_PTR(void *)) {
                listCapacity_ = tempListAllocatedSize;
            }
            else {
                //Implicit rollback of allocatedMemory_
                ret = false;
            }
        }
        else { // { listAllocatedSize_ > 0U }
               //The array has already memory reserved, so it reallocates memory for it
            void *memoryPointer = allocatedMemory_;
            allocatedMemory_ = static_cast<uint8 *>(HeapManager::Realloc(memoryPointer, tempListAllocatedSize * listElementSize_));
            if (allocatedMemory_ != NULL_PTR(void *)) {
                listCapacity_ = tempListAllocatedSize;
            }
            else {
                //TODO Is it assured that memoryPointer is still pointing to a valid allocated memory?
                //Explicit rollback of allocatedMemory_
                allocatedMemory_ = static_cast<uint8 *>(memoryPointer);
                ret = false;
            }
        }
    }
    return ret;
}

void* StaticListHolder::GetAllocatedMemory() {
    return reinterpret_cast<void *>(allocatedMemory_);
}

const void* StaticListHolder::GetAllocatedMemoryConst() const {
    return reinterpret_cast<const void *>(allocatedMemory_);
}

bool StaticListHolder::Set(const uint32 position,
                           const void * const value) {
    bool ret = (position < listSize_);
    if (ret) {
        uint32 index = (position * listElementSize_);
        ret = MemoryOperationsHelper::Copy(&allocatedMemory_[index], value, listElementSize_);
    }
    return ret;

}

}
