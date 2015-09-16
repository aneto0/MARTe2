/**
 * @file StaticListHolder.h
 * @brief Header file for class StaticListHolder
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

 * @details This header file contains the declaration of the class StaticListHolder
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATICLISTHOLDER_H_
#define STATICLISTHOLDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace Lists {

/**
 * @brief Abstracts an array which can grow and shrink
 *
 * @invariant ... GetSize() <= max(uint32)
 *
 * It is a managed array of objects
 * it supports:
 * 1) extensions and contraction via realloc
 * 2) granularity in growth and shrinking
 * 3) Add at any position (start middle end)
 */
class StaticListHolder {

public:

    /**
     * @brief Constructor parameterised by element size and allocation granularity
     * @param[in] listElementSize The size in bytes of one element of the list
     * - could be sizeof(void) for a list of pointers
     * @param[in] listAllocationGranularity The size in bytes of the increment
     * of reserved space when the list grows.
     * @pre true
     * @post GetElementSize() == listElementSize &&
             GetAllocationGranularity() == listAllocationGranularity &&
             GetSize() == 0 &&
             GetAllocatedSize() == 0
     */
    StaticListHolder(const uint32 listElementSize,
                     const uint32 listAllocationGranularity);

    /**
     * @brief Destructor
     */
    ~StaticListHolder();

    /**
     * @brief Gets the element size
     */
    uint32 GetElementSize(void) const;

    /*
     * @brief Gets the allocation granularity
     */
    uint32 GetAllocationGranularity(void) const;

    /**
     * @brief Gets the size of the list
     */
    uint32 GetSize(void) const;

    /**
     * @brief Gets the capacity of the list, i.e. the actual allocated size of the list in memory
     */
    uint32 GetCapacity(void) const;

    /*
     *
     */
    uint32 GetMaxCapacity(void) const;

    /**
     * @brief Adds an element at the end of the list
     * @param[in] position The index where the new element has to be added in the range [0..GetSize()]
     * @param[in] copyFrom The pointer to the memory address where the new element must be copied from
     *
     * @pre GetSize() < GetCapacity() && copyFrom != NULL
     * @post GetSize() == this'old->GetSize() + 1 &&
     *       [value:elementType | Peek(this'old->GetSize()-1,&value) => value == *copyFrom]
     *
     * exception position > GetSize()
     */
    bool Add(const void *copyFrom);

    /**
     * @brief Inserts an element at a given position
     * @pre position>=0 && position<GetSize() && copyFrom!=NULL
     * @post GetSize() == this'old->GetSize() + 1 &&
     *       [value:elementType | Peek(position,&value) => value == *copyFrom] &&
     *       [value1,value2:elementType; i: uint32, i from position to GetSize()-1 | Peek(i,&value1) && this'old->Peek(i-1,&value2) => *value1 == *value2]
     */
    bool Insert(const uint32 position,
             const void *copyFrom);

    /**
     * @brief Peeks an element at a given position
     * @param[in] position The index of the requested element in the range [0..GetSize()-1]
     * @param[in] copyTo The pointer to the memory address where the requested element must be copied to
     * @returns true the element was if copy successful and position ok
     *
     * @pre position>=0 && position<GetSize() && copyTo!=NULL
     * @post result && copyTo != NULL => *copyTo holds a copy of the requested element
     *
     * O(1)
     */
    bool Peek(const uint32 position,
              void *copyTo) const;

    /**
     * @brief Extracts (i.e. peeks and deletes) an element from the list by position
     * @param[in] position The index of the requested element in the range [0..GetSize()-1]
     * @param[in] copyTo The pointer to the memory address where the requested element must be copied to
     *
     * @pre position>=0 && position<GetSize() && copyTo!=NULL
     * @post result && *copyTo!=NULL => copyTo holds a copy of the requested element &&
     *       Peek(position'old) does not hold the value copied to *copyTo
     */
    bool Extract(const uint32 position,
                 void *copyTo);

private:

    /*
     * @pre GetAllocatedSize() + GetAllocationGranularity() <= GetMaxCapacity()
     * @post GetCapacity() == GetAllocatedSize()'old + GetAllocationGranularity()
     *
     * exception GetCapacity() + GetAllocationGranularity() > GetMaxCapacity()
     */
    bool IncreaseCapacity(void);

    /**
     * size of one element in the list
     */
    uint32 listElementSize_;

    /**
     *  actual number of elements in list
     */
    uint32 listSize_;

    /**
     *  spaces in the list
     */
    uint32 listCapacity_;

    /*
     * allocation granularity
     */
    uint32 listAllocationGranularity_;

    /*
     * accounts for maxint and listElementSize and listAllocationGranularity
     * it is a multiple of listAllocationGranularity that multiplied by listElementSize fits within maxint
     */
    uint32 maxListCapacity_;

    /**
     * the actual list
     */
    uint8* allocatedMemory_;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATICLISTHOLDER_H_ */
