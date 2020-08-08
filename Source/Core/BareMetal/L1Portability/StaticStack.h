/**
 * @file StaticStack.h
 * @brief Header file for class StaticStack
 * @date 01/04/2020
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

 * @details This header file contains the declaration of the class StaticStack
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef STATICSTACK_H_
#define STATICSTACK_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StaticList.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief This template class is a ADT (Abstract Data Type) for an unbounded
 * stack which allows pushing, popping, and peeking top elements .
 *
 * @details This class is the template version of the class StaticListHolder,
 * parameterising the elementType and listAllocationGranularity. Thus, the
 * instantiation of the template class determines automatically the size of
 * the element type and sets the allocation granularity for all the instances.
 *
 * @tparam elementType The type of the elements that the list will hold
 * @tparam listAllocationGranularity The number of elements that the list must
 * reserve as space in memory in advance, each time that needs to grow.
 *
 * @see Lists::StaticListHolder
 */
template<typename elementType, uint32 listAllocationGranularity = 10>
class StaticStack {
public:

    /**
     * @brief Default constructor
     */
    StaticStack();

    /**
     * @see StaticListHolder::GetSize()
     */
    uint32 GetSize(void) const;

    /**
     * @see StaticListHolder::Peek()
     * @brief compared to StaticListHolder::Peek note that position is a negative index relative to the end of the list
     */
    bool Peek(const uint32 position, elementType &value) const;

    /**
     * @see StaticListHolder::Add()
     */
    bool Push(const elementType &value);

    /**
     * @see StaticListHolder::Extract()
     * @brief compared to StaticListHolder::Extract note that position is the end of the list
     */
    bool Pop(elementType &value);

    /**
     * @see StaticListHolder::GetAllocatedMemoryConst()
     * @brief Fast peek of the element at the top of stack. Rest of the elements are at negative indexes
     */
    inline const elementType* GetAllocatedMemoryConst() const;

    /**
     * @see StaticListHolder::Clean()
     */
    inline void Clean();


private:

    /**
     * @brief The instance of StaticListHolder to which the implementation is delegated.
     */
    StaticListHolder slh;

    /**
     * do not allow copying!
     */
    //lint -e{1722} assignment operator does not return reference to prevent creation of object copies
    void operator=(const StaticStack<elementType,listAllocationGranularity> &stack){}

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


template<typename elementType, uint32 listAllocationGranularity>
inline const elementType* StaticStack<elementType, listAllocationGranularity>::GetAllocatedMemoryConst() const{
	const elementType* p = static_cast<const elementType*>(slh.GetAllocatedMemoryConst());
    return (p == NULL_PTR(elementType*)) ? p : (p + slh.GetSize() -1);
}

template<typename elementType, uint32 listAllocationGranularity>
inline void StaticStack<elementType, listAllocationGranularity>::Clean() {
	slh.Clean();
}

template<typename elementType, uint32 listAllocationGranularity>
StaticStack<elementType, listAllocationGranularity>::StaticStack() :
        slh(sizeof(elementType), listAllocationGranularity) {
}

template<typename elementType, uint32 listAllocationGranularity>
uint32 StaticStack<elementType, listAllocationGranularity>::GetSize(void) const {
    return slh.GetSize();
}

template<typename elementType, uint32 listAllocationGranularity>
bool StaticStack<elementType, listAllocationGranularity>::Peek(const uint32 position,  elementType &value) const {
    return slh.Peek(slh.GetSize()-position-1, static_cast<void *>(&value));
}

template<typename elementType, uint32 listAllocationGranularity>
bool StaticStack<elementType, listAllocationGranularity>::Push(const elementType &value) {
    return slh.Add(static_cast<const void *>(&value));
}

template<typename elementType, uint32 listAllocationGranularity>
bool StaticStack<elementType, listAllocationGranularity>::Pop(elementType &value) {
    return slh.Extract(slh.GetSize()-1, static_cast<void *>(&value));
}


} // MARTe

#endif /*STATICSTACK_H_ */
