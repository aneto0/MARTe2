/**
 * @file StaticListHolder.h
 * @brief Header file for class StaticListHolder
 * @date 31/08/2015
 * @author Filippo Sartori
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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

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
     * param[in] listElementSize The size in bytes of one element of the list
     * - could be sizeof(void) for a list of pointers
     * param[in] listAllocationGranularity The size in bytes of the increment
     * of reserved space when the list grows.
     */
    StaticListHolder(uint32 listElementSize,
                     uint32 listAllocationGranularity);

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
     * @brief Gets the allocated size of the list
     */
    uint32 GetAllocatedSize(void) const;

    /**
     * @brief Adds an element to the list by position
     * @param[in] position The index where the new element has to be added in the range [0..GetSize()]
     * @param[in] copyFrom The pointer to the memory address where the new element must be copied from
     *
     * @pre position>=0 && position<=GetSize() && copyFrom!=NULL
     * @post position'old = this'old->GetSize() => GetSize()==this'old->GetSize()+1 &&
     *       Peek(position'old
     *
     * position = listSize is an append otherwise it is an insert
     */
    bool Add(const uint32 position,
             const void *copyFrom);

    /**
     * @brief Peeks an element of the list by position
     * @param[in] position The index of the requested element in the range [0..GetSize())
     * @param[in] copyTo The pointer to the memory address where the requested element must be copied to
     * @returns true the element was if copy successful and position ok
     *
     * @pre position>=0 && position<GetSize() && copyTo!=NULL
     * @post *copyTo holds a copy of the requested element
     */
    bool Peek(const uint32 position,
              void *copyTo) const;

    /**
     * @brief Extracts (i.e. peeks and deletes) an element from the list by position
     * @param[in] position The index of the requested element in the range [0..GetSize())
     * @param[in] copyTo The pointer to the memory address where the requested element must be copied to
     *
     * @pre position>=0 && position<GetSize() && copyTo!=NULL
     * @post *copyTo!=NULL => holds a copy of the requested element &&
     *       Peek(position'old) does not hold the value copied to *copyTo
     */
    bool Extract(const uint32 position,
                 void *copyTo);

private:

    /**
     * increments listSize by 1
     * mallocs or reallocs if necessary
     */
    bool IncreaseListSize();

private:

    /**
     * size of one elemeTypeCharacteristicsnt in the list
     */
    uint32 listElementSize;

    /**
     *  actual number of elements in list
     */
    uint32 listSize;

    /**
     *  spaces in the list
     */
    uint32 listAllocatedSize;

    /*
     * allocation granularity
     */
    uint32 listAllocationGranularity;

    /*
     * accounts for maxint and listElementSize and listAllocationGranularity
     * it is a multiple of listAllocationGranularity that multiplied by listElementSize fits within maxint
     */
    uint32 maxListSize;

    /**
     * the actual list
     */
    uint8* allocatedMemory;
};

//TODO: listAllocationGranularity could be a template parameter with a default value
template<typename elementType>
class StaticList: public StaticListHolder {
public:

    StaticList(const uint32 &listAllocationGranularity) :
            StaticListHolder(sizeof(elementType), listAllocationGranularity) {
    }

    bool Peek(const uint32 &position,
              elementType &value) const {
        return StaticListHolder::Peek(position, static_cast<void *>(&value));
    }

    bool Add(const uint32 &position,
             const elementType &value) {
        return StaticListHolder::Add(position, static_cast<const void *>(&value));
    }

    bool Remove(const uint32 &position) {
        return StaticListHolder::Extract(position, NULL_PTR(void *));
    }

    bool Extract(const uint32 &position,
                 elementType &value) {
        return StaticListHolder::Extract(position, static_cast<void *>(&value));
    }
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATICLISTHOLDER_H_ */
