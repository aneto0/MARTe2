/**
 * @file StaticStackTest.h
 * @brief Header file for class StaticStackTest
 * @date 06/04/2020
 * @author Dídac Magriñá
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

 * @details This header file contains the declaration of the class StaticStackTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATICSTACKTEST_H_
#define STATICSTACKTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StaticStack.h"
#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
class StaticStackTest {
public:

    /**
     * @brief Default constructor
     */
    StaticStackTest();

    /**
     * @brief Destructor
     */
    virtual ~StaticStackTest();

    /**
     * @brief Tests the default constructor
     */
    bool TestDefaultConstructor(void);

    /**
     * @brief Tests getting the size of a list before, during and after adding/removing elements from demoValues array
     */
    bool TestGetSize(void);

    /**
     * @brief Tests that stack size becomes 0 after cleaning it
     */
    bool TestClean(void);

    /**
     * @brief Tests that pointer to memory const is retrieved correctly
     */
    bool TestGetAllocatedMemoryConst(void);

    /**
     * @brief Tests pushing an element on an empty stack
     */
    template<elementType addValue>
    bool TestPushOnEmptyStack(void);

    /**
     * @brief Tests pushing an element on a stack initialized with the elements from demoValues array
     */
    template<elementType addValue>
    bool TestPushOnNonEmptyStack(void);

    /**
     * @brief Tests popping elements from a stack initialized with the elements from demoValues array
     */
    bool TestPopOnNonEmptyStack(void);

    /**
     * @brief Tests popping an element from an empty stack
     */
    bool TestPopOnEmptyStack(void);

    /**
     * @brief Tests peeking all the elements of a list initialized with the elements from demoValues array
     */
    bool TestPeek(void);

    /**
     * @brief Test peeking from invalid position.
     */
    bool TestPeekInvalidPosition();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::StaticStackTest() {
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::~StaticStackTest() {

}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestDefaultConstructor(void) {
    bool result = false;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Tests if the object has been properly constructed querying its properties:
    result = (targetStack.GetSize() == 0);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestGetSize(void) {
    bool result = true;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Tests if size is zero after default construction:
    result = result && (targetStack.GetSize() == 0);

    //Tests if size is increased after adding each of the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetStack.Push(demoValues[i]);
        result = result && (targetStack.GetSize() == i + 1);
    }

    //Tests if size is max after adding all demo values:
    result = result && (targetStack.GetSize() == maxDemoValues);

    //Tests if size is decreased after removing each of the values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        uint32 value;
        targetStack.Pop(value); //Or 0 instead of targetStack.GetSize()-1
        result = result && (targetStack.GetSize() == (maxDemoValues - (i + 1)));
    }

    //Tests if size is zero after removing adding all demo values:
    result = result && (targetStack.GetSize() == 0);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestClean(void) {
    bool result = true;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Initialize stack:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetStack.Push(demoValues[i]);
    }

    //Tests if size is zero after cleaning stack
    targetStack.Clean();
    result = (targetStack.GetSize() == 0);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestGetAllocatedMemoryConst(void) {
    bool result = true;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Initialize stack:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetStack.Push(demoValues[i]);
    }


    const elementType* allocatedMem = targetStack.GetAllocatedMemoryConst();

    if (maxDemoValues == 0) {
        // Maybe interface could be improved. When size==0 GetAllocatedMemoryConst returns "slh.GetAllocatedMemoryConst() - 1"
        result &= (allocatedMem == NULL_PTR(elementType*));
    }

    else {
        for (uint32 i = 0; (i < maxDemoValues); i++) {
            uint32 listIndex = maxDemoValues - 1 - i;
            result &= (*(allocatedMem - i) == demoValues[listIndex]);
        }
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType addValue>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestPushOnEmptyStack(void) {
    bool result = false;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Tests adding a value on the target stack:
    {
        elementType peekValue;
        targetStack.Push(addValue);
        targetStack.Peek(0, peekValue);
        result = (peekValue == addValue && targetStack.GetSize() == 1);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType addValue>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestPushOnNonEmptyStack(void) {
    bool result = false;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Initializes the target stack with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetStack.Push(demoValues[i]);
    }

    //Tests adding a value on the target stack:
    {
        elementType peekValue;
        targetStack.Push(addValue);
        targetStack.Peek(0, peekValue);
        result = (peekValue == addValue && targetStack.GetSize() == maxDemoValues + 1);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestPopOnNonEmptyStack(void) {
    bool result = true;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetStack.Push(demoValues[i]);
    }

    //Tests popping values in stack:
    {
        for (uint32 i = 0; i < maxDemoValues; i++) {
            uint32 listIndex;
            elementType popValue;
            targetStack.Pop(popValue);
            listIndex = maxDemoValues - 1 -i;
            result = ((result) && (popValue == demoValues[listIndex]) && (targetStack.GetSize() == listIndex));
        }
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestPopOnEmptyStack(void) {
    bool result = true;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Tests popping from empty stack
    elementType popValue;
    result = !targetStack.Pop(popValue);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestPeek(void) {
    bool result = true;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Initializes the target stack with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetStack.Push(demoValues[i]);
    }

    //Tests peeking all the demo values from the target stack:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        elementType peekValue;
        targetStack.Peek(maxDemoValues - i - 1, peekValue);
        result = (peekValue == demoValues[i]);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticStackTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestPeekInvalidPosition(void) {
    bool result = true;
    StaticStack<elementType, listAllocationGranularity> targetStack;

    //Initializes the target stack with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetStack.Push(demoValues[i]);
    }

    //Tests peeking from invalid position
    elementType peekValue;
    uint32 invalidPosition = maxDemoValues + 5;
    result = !targetStack.Peek(invalidPosition, peekValue);

    return result;
}

}

#endif /* STATICSTACKTEST_H_ */

