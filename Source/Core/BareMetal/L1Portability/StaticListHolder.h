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

namespace MARTe {

/**
 * @brief This class is an ADT for an unbounded list which allows adding,
 * inserting, removing and peeking elements at arbitrary positions in a
 * range from 0 onwards.
 *
 * @invariant
 *   GetSize() >= 0 &&
 *   GetSize() <= GetCapacity() &&
 *   GetCapacity() <= GetMaxCapacity()
 *
 * @details
 *
 * Abstraction
 * -----------
 *
 * This class represents an unbounded list ADT which is indexed from 0 to
 * the actual size minus 1. It allows adding elements and also inserting
 * and removing ones at any position, remaining always compacted, i.e. there
 * are no holes into the range of the array. The list also allows peeking
 * elements at any position.
 *
 * In order to allow the list to be unbounded, the list will reserve memory
 * in advance for future adds/inserts of elements, so whenever the actual
 * size exceeds the current reserved memory, the list will grow reserving
 * more space in memory.
 *
 * Note 1: Although the list allows peeking (a.k.a. getting) elements by
 * position like in an array, it does not allow to set elements, i.e. it
 * only allows adding or inserting elements, not to substitute them.
 *
 * Note 2: The list is not intended to call the destructors of the elements,
 * so it should be only used with scalars or pointers.
 *
 * Attributes
 * ----------
 *
 * - Position: The numeric index at which an element is referenced into the
 * list. The position must be in the range [0..size-1] for accessing or
 * removing elements but can be [0..size] for inserting new elements.
 * - Size: The number of elements that the list actually holds.
 * - Capacity: The number of elements that the list actually holds plus the
 * number of reserved spaces in memory for storing new elements.
 * of the array. The capacity grows dynamically in increments specified by
 * the allocation granularity.
 * - ElementTypeSize: The size in bytes of the type of the elements. It will
 * typically be sizeof(type) or sizeof(type*) in case the elements were
 * pointers.
 * - MaxCapacity: The maximum theoretically capacity of the list taking
 * into account the element type size, the allocation granularity, and the
 * maximum value for the numeric type used for indexing positions.
 * - AllocationGranularity: The number of elements that the list must reserve
 * as space in memory in advance each time that needs to grow.
 *
 * Formulae
 * --------
 *
 * MAXINDEX = TypeDefinition::TypeCharacteristics::MaxValue<uint32>()
 *
 * MAXCAPACITY = \f$
 * \frac {
 *   \lfloor\frac{MAXINDEX}{AllocationGranularity \times ElementSize}\rfloor
 *   \times
 *   (AllocationGranularity \times ElementSize)
 * }{
 *   ElementSize
 * }
 * \f$
 *
 * Responsibilities
 * ----------------
 *
 * - Adding new elements at the back of the array.
 * - Inserting new elements at any position of the array.
 * - Removing elements from any position of the array.
 * - Peeking elements from any position of the array.
 * - Extracting elements from any position of the array (i.e. peeking
 * and removing at the same time)
 *
 */
class DLL_API StaticListHolder {

public:

    /**
     * @brief Constructor parameterized by element type size and allocation granularity
     * @param[in] listElementSize The value of the element type size for this instance
     * @param[in] listAllocationGranularity The value of the allocation granularity for this instance
     * @pre true
     * @post
     *   GetElementSize() == listElementSize &&
     *   GetAllocationGranularity() == listAllocationGranularity &&
     *   GetMaxCapacity() == MAXIMUMCAPACITY &&
     *   GetSize() == 0 &&
     *   GetCapacity() == 0
     */
    StaticListHolder(const uint32 listElementSize,
                     const uint32 listAllocationGranularity);

    /**
     * @brief Destructor
     */
    ~StaticListHolder();

    /**
     * @brief Gets the element type size
     */
    uint32 GetElementSize(void) const;

    /**
     * @brief Gets the allocation granularity
     */
    uint32 GetAllocationGranularity(void) const;

    /**
     * @brief Get the maximum capacity of the list
     */
    uint32 GetMaxCapacity(void) const;

    /**
     * @brief Gets the size of the list
     */
    uint32 GetSize(void) const;

    /**
     * @brief Gets the capacity of the list
     */
    uint32 GetCapacity(void) const;

    /**
     * @brief Adds an element at the end of the list
     * @param[in] copyFrom The pointer to the memory address where the new element must be copied from
     * @return false if precondition is broken or memory allocation fails
     * @pre
     *   copyFrom != NULL
     * @post
     *   GetSize() == this'old->GetSize() + 1 &&
     *   GetCapacity() >= this'old->GetCapacity() &&
     *   {value:*elementType | Peek(GetSize()-1,value) => *value == *copyFrom}
     * @warning *copyFrom must be a valid allocated memory of size GetElementSize()
     */
    bool Add(const void * const copyFrom);

    /**
     * @brief Remove all elements. Does not shrink list or free memory
     * @post GetSize() = 0
     */
    inline void Clean();

    /**
     * @brief Inserts an element at a given position of the list
     * @param[in] position The index where the new element has to be added
     * @param[in] copyFrom The pointer to the memory address where the new element must be copied from
     * @return false if precondition is broken or memory allocation fails
     * @pre
     *   position>=0 &&
     *   position<=GetSize() &&
     *   copyFrom!=NULL
     * @post
     *   GetSize() == this'old->GetSize() + 1 &&
     *   GetCapacity() >= this'old->GetCapacity() &&
     *   {value:*elementType | Peek(position,value) => *value == *copyFrom} &&
     *   {value1,value2:*elementType; i:uint32 in [position+1..GetSize()-1] |
     *    Peek(i,value1) && this'old->Peek(i-1,value2) => *value1 == *value2}
     * @warning *copyFrom must be valid allocated memory of size GetElementSize()
     */
    bool Insert(const uint32 position,
                const void * const copyFrom);

    /**
     * @brief Peeks an element from a given position of the list
     * @param[in] position The index of the requested element
     * @param[in] copyTo The pointer to the memory address where the requested element must be copied to
     * @return false if precondition is broken or memory operation fails.
     * @pre
     *   position >= 0 &&
     *   position < GetSize() &&
     *   copyTo != NULL
     * @post
     *   copyTo != NULL &&
     *   *copyTo holds a copy of the requested element
     * @warning *copyTo must be a valid allocated memory of size GetElementSize()
     */
    bool Peek(const uint32 position,
              void * const copyTo) const;

    /**
     * @brief Extracts an element from a given position of the list (i.e. peeks and deletes)
     * @param[in] position The index of the element to remove
     * @return false if precondition is broken or memory operation fails.
     * @pre
     *   position >= 0 &&
     *   position < GetSize() &&
     * @post
     *   GetSize() == this'old->GetSize() - 1 &&
     *   {value1,value2:*elementType; i:uint32 in [position..GetSize()-1] |
     *    Peek(i,value1) && this'old->Peek(i+1,value2) => *value1 == *value2}
     */
    bool Remove(const uint32 position);

    /**
     * @brief Extracts an element from a given position of the list (i.e. peeks and deletes)
     * @details This method is equivalent to the execution of {
     *   Peek(position, copyTo);
     *   Remove(position);
     * }
     * @param[in] position The index of the element to peek and remove
     * @param[in] copyTo The pointer to the memory address where the requested element must be copied to
     * @return false if the precondition is broken or the memory operation fails.
     * @pre Peek'pre && Remove'pre
     * @post Peek'post && Remove'post
     */
    bool Extract(const uint32 position,
                 void * const copyTo);

    /**
     * @brief Replaces the element at the specified position
     * @param[in] position The index of the element to replace
     * @param[in] value pointer to the memory address where the requested element will be copied from.
     * @return false if the precondition is broken or the memory operation fails.
     * @pre position < GetSize()
     * @post Peek(position, element) => *element = *value
     */
    bool Set(const uint32 position,
             const void * const value);

    /**
     * @brief Retrieves the pointer to the allocated memory area.
     * @return the pointer to the allocated memory area.
     */
    void* GetAllocatedMemory();

    /**
     * @brief Retrieves the pointer to the allocated memory area.
     * @return the pointer to the allocated memory area.
     */
    const void* GetAllocatedMemoryConst() const;

private:

    /**
     * @brief Increases the capacity of the list
     * @return false if precondition is broken or memory allocation fails
     * @pre GetCapacity() + GetAllocationGranularity() <= GetMaxCapacity()
     * @post GetCapacity() == GetCapacity()'old + GetAllocationGranularity()
     */
    bool IncreaseCapacity(void);

    /**
     * Stores the ElementSize
     */
    const uint32 listElementSize_;

    /**
     * Stores the AllocationGranularity
     */
    const uint32 listAllocationGranularity_;

    /**
     * Stores the MaxCapacity
     */
    const uint32 maxListCapacity_;

    /**
     * Pointer to the first byte of the array of elements
     */
    uint8* allocatedMemory_;

    /**
     * Stores the ListCapacity
     */
    uint32 listCapacity_;

    /**
     * Stores the Size
     */
    uint32 listSize_;

    /*lint -e{1712} This class does not have a default constructor because
     * the element type size must be defined on construction and remain constant
     * during object's lifetime*/
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void StaticListHolder::Clean(){
    listSize_ = 0;
}

}

#endif /* STATICLISTHOLDER_H_ */
