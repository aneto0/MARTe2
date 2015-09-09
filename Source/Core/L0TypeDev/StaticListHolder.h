/**
 * @file StaticListHolder.h
 * @brief Header file for class StaticListHolder
 * @date Aug 31, 2015
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

#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace Lists {

/**
 * It is a managed array of objects
 * it supports:
 * 1) extensions and contraction via realloc
 * 2) granularity in growth and shrinking
 * 3) Add at any position (start middle end)
 */
class StaticListHolder{

    /**
     * increments listSize by 1
     * mallocs or reallocs if necessary
     */
    bool IncreaseListSize();

public:

    /**
     * listElementSize is the size in bytes of one element of the list - could be sizeof (void) for a list of pointers
     */
    StaticListHolder(uint32 listElementSize, uint32 listAllocationGranularity);

    /**
     * @brief Browses the list.
     * @param[in] position the position of the requested element (0 means the first element).
     * @param[in] copyTo a pointer to memory where to copy an element from the list
     * @return true if copy successful and position ok
     */
    bool ListPeek (const uint32 position, void *copyTo)const;

    /**
     * position is the address of the object to be copied in the list
     * position can be from 0 to listSize inclusive
     * position = listSize is an append otherwise it is an insert
     * copyFrom
     */
    bool ListAdd   (const uint32 position, const void *copyFrom);

    /**
     * position is the address of the object to be copied in the list
     * position can be from 0 to listSize-1 inclusive
     * removes the element at position and if copyFrom is not NULL copies it there
     */
    bool ListExtract   (const uint32 position, void *copyTo);

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
     *
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




template <typename elementType>
class StaticList: public StaticListHolder
{
public:

    StaticList(uint32 listAllocationGranularity):StaticListHolder(sizeof(elementType), listAllocationGranularity){
    }

    bool Peek(const uint32 position,elementType &value)const {
        return StaticListHolder::ListPeek(position, static_cast<void *>(&value));
    }

    bool Add(const uint32 position,const elementType &value){
        return StaticListHolder::ListAdd(position, static_cast<const void *>(&value));
    }

    bool Remove(const uint32 position){
        return StaticListHolder::ListExtract(position, NULL_PTR(void *));
    }

    bool Extract(const uint32 position,elementType &value){
        return StaticListHolder::ListExtract(position, static_cast<void *>(&value));
    }
};

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STATICLISTHOLDER_H_ */

