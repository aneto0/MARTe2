/**
 * @file StaticListTest.h
 * @brief Header file for class StaticListTest
 * @date 14/09/2015
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

 * @details This header file contains the declaration of the class StaticListTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATICLISTTEST_H_
#define STATICLISTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StaticList.h"
#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
class StaticListTest {
public:

    /**
     * @brief Default constructor
     */
    StaticListTest();

    /**
     * @brief Destructor
     */
    virtual ~StaticListTest();

    /**
     * @brief Tests the default constructor
     */
    bool TestDefaultConstructor(void);

    /**
     * @brief Tests getting the element size of a list before, during and after adding/removing elements from demoValues array
     */
    bool TestConstantness(void);

    /**
     * @brief Tests getting the size of a list before, during and after adding/removing elements from demoValues array
     */
    bool TestGetSize(void);

    /**
     * @brief Tests getting the capacity of a list before, during and after adding elements from demoValues array
     */
    bool TestGetCapacity(void);

    /**
     * @brief Tests adding an element on an empty list
     */
    template<elementType addValue>
    bool TestAddOnEmptyList(void);

    /**
     * @brief Tests adding an element on a list initialized with the elements from demoValues array
     */
    template<elementType addValue>
    bool TestAddOnNonEmptyList(void);

    /**
     * @brief Stress test. Add elements until the table capacity arrive to the MAXCAPACITY
     */
    template<elementType value>
    bool TestAddOnFullList(void);

    /**
     * @brief Tests inserting an element on an empty list
     */
    template<elementType insertValue>
    bool TestInsertOnEmptyList(void);

    /**
     * @brief Tests inserting an element at the beginning on a list initialized with the elements from demoValues array
     */
    template<elementType insertValueBeginning>
    bool TestInsertOnNonEmptyListAtBeginning(void);

    /**
     * @brief Tests inserting an element at the middle on a list initialized with the elements from demoValues array
     */
    template<elementType insertValueMiddle>
    bool TestInsertOnNonEmptyListAtMiddle(void);

    /**
     * @brief Tests inserting an element at the end on a list initialized with the elements from demoValues array
     */
    template<elementType insertValueEnd>
    bool TestInsertOnNonEmptyListAtEnd(void);

    /**
     * @brief Test insert an element at an invalid position.
     */
    template<elementType insertValueInvalidPosition>
    bool TestInsertOnNonEmptyListAtInvalidPosition(void);

    /**
     * @brief Tests removing the element at the beginning of a list initialized with the elements from demoValues array
     */
    bool TestRemoveAtBeginning(void);

    /**
     * @brief Tests removing the element at the middle of a list initialized with the elements from demoValues array
     */
    bool TestRemoveAtMiddle(void);

    /**
     * @brief Tests removing the element at the end of a list initialized with the elements from demoValues array
     */
    bool TestRemoveAtEnd(void);

    /**
     * @brief Tests removing the element from invalid position
     */
    bool TestRemoveInvalidPosition(void);

    /**
     * @brief Tests extracting the element at the beginning of a list initialized with the elements from demoValues array
     */
    bool TestExtractAtBeginning(void);

    /**
     * @brief Tests extracting the element at the middle of a list initialized with the elements from demoValues array
     */
    bool TestExtractAtMiddle(void);

    /**
     * @brief Tests extracting the element at the end of a list initialized with the elements from demoValues array
     */
    bool TestExtractAtEnd(void);

    /**
     * @brief Tests extracting the element from invalid position
     */
    bool TestExtractInvalidPosition(void);

    /**
     * @brief Tests peeking all the elements of a list initialized with the elements from demoValues array
     */
    bool TestPeek(void);

    /**
     * @brief Test peeking from invalid position.
     */
    bool TestPeekInvalidPosition();

    /**
     * @brief Tests if the [] operator returns the element at the specified position
     */
    bool TestSquareOperator();

    /**
     * @brief Tests if the [] operator returns the last element is the position is input is greater than the list size
     */
    bool TestSquareOperatorInvalidPosition();

    /**
     * @brief Tests the Set function
     */
    template<elementType value>
    bool TestSet(uint32 positionToSet);

    /**
     * @brief Tests if size goes down to 0 and capacity maintained after calling Clean function
     */
    bool TestClean();

    /**
     * @brief Tests that pointer to memory const is retrieved correctly
     */
    bool TestGetAllocatedMemoryConst();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::StaticListTest() {
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::~StaticListTest() {

}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestDefaultConstructor(void) {
    const uint32 MAXINDEX = MAX_UINT32;
    const uint32 MAXCAPACITY = (((MAXINDEX / (listAllocationGranularity * sizeof(elementType))) * (listAllocationGranularity * sizeof(elementType)))
            / sizeof(elementType));

    bool result = false;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Tests if the object has been properly constructed querying its properties:
    result = (targetList.GetElementSize() == sizeof(elementType) && targetList.GetAllocationGranularity() == 10 && targetList.GetMaxCapacity() == MAXCAPACITY
            && targetList.GetSize() == 0 && targetList.GetCapacity() == 0);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestConstantness(void) {
    const uint32 MAXINDEX = MAX_UINT32;
    const uint32 MAXCAPACITY = (((MAXINDEX / (listAllocationGranularity * sizeof(elementType))) * (listAllocationGranularity * sizeof(elementType)))
            / sizeof(elementType));

    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Tests constants after default construction:
    result = result && (targetList.GetElementSize() == sizeof(elementType));
    result = result && (targetList.GetAllocationGranularity() == listAllocationGranularity);
    result = result && (targetList.GetMaxCapacity() == MAXCAPACITY);

    //Tests constants after adding each of the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
        result = result && (targetList.GetElementSize() == sizeof(elementType));
        result = result && (targetList.GetAllocationGranularity() == listAllocationGranularity);
        result = result && (targetList.GetMaxCapacity() == MAXCAPACITY);
    }

    //Tests constants after removing each of the values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Remove(targetList.GetSize() - 1); //Or 0 instead of targetList.GetSize()-1
        result = result && (targetList.GetElementSize() == sizeof(elementType));
        result = result && (targetList.GetAllocationGranularity() == listAllocationGranularity);
        result = result && (targetList.GetMaxCapacity() == MAXCAPACITY);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestGetSize(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Tests if size is zero after default construction:
    result = result && (targetList.GetSize() == 0);

    //Tests if size is increased after adding each of the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
        result = result && (targetList.GetSize() == i + 1);
    }

    //Tests if size is max after adding all demo values:
    result = result && (targetList.GetSize() == maxDemoValues);

    //Tests if size is decreased after removing each of the values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Remove(targetList.GetSize() - 1); //Or 0 instead of targetList.GetSize()-1
        result = result && (targetList.GetSize() == (maxDemoValues - (i + 1)));
    }

    //Tests if size is zero after removing adding all demo values:
    result = result && (targetList.GetSize() == 0);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestGetCapacity(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Tests if capacity is zero after default construction:
    result = result && (targetList.GetCapacity() == 0);

    //Tests if capacity is increased after adding groups of demo values:
    {
        uint32 capacity = 0;
        for (uint32 i = 0; i < maxDemoValues; i++) {
            targetList.Add(demoValues[i]);
            if (i % listAllocationGranularity == 0) {
                capacity = capacity + listAllocationGranularity;
            }
            result = result && (targetList.GetCapacity() == capacity);
        }
    }

    //Tests if capacity is ok after adding all demo values:
    result = result && (targetList.GetCapacity() == listAllocationGranularity * ((maxDemoValues / listAllocationGranularity) + 1));

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType addValue>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestAddOnEmptyList(void) {
    bool result = false;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Tests adding a value on the target list:
    {
        elementType peekValue;
        targetList.Add(addValue);
        targetList.Peek(0, peekValue);
        result = (peekValue == addValue && targetList.GetSize() == 1);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType addValue>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestAddOnNonEmptyList(void) {
    bool result = false;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests adding a value on the target list:
    {
        elementType peekValue;
        targetList.Add(addValue);
        targetList.Peek(maxDemoValues, peekValue);
        result = (peekValue == addValue && targetList.GetSize() == maxDemoValues + 1);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType value>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestAddOnFullList(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Tests adding elements until exhausting capacity:
    {
        bool ret = true;
        while (ret == true) {
            ret = targetList.Add(value);
        }
    }

    result = (targetList.GetCapacity() == targetList.GetMaxCapacity());

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType insertValue>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestInsertOnEmptyList(void) {
    bool result = false;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Tests inserting a value at the only position available after default construction:
    {
        elementType peekValue;
        targetList.Insert(0, insertValue);
        targetList.Peek(0, peekValue);
        result = (peekValue == insertValue && targetList.GetSize() == 1);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType insertValueBeginning>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestInsertOnNonEmptyListAtBeginning(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests inserting a value at the beginning:
    {
        const uint32 indexValueBeginning = 0;
        elementType peekValue;
        targetList.Insert(indexValueBeginning, insertValueBeginning);
        targetList.Peek(indexValueBeginning, peekValue);
        result = (result && peekValue == insertValueBeginning && targetList.GetSize() == maxDemoValues + 1);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType insertValueMiddle>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestInsertOnNonEmptyListAtMiddle(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests inserting a value at the middle
    {
        const uint32 indexValueMidldle = maxDemoValues / 2;
        elementType peekValue;
        targetList.Insert(indexValueMidldle, insertValueMiddle);
        targetList.Peek(indexValueMidldle, peekValue);
        result = (result && peekValue == insertValueMiddle && targetList.GetSize() == maxDemoValues + 1);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType insertValueEnd>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestInsertOnNonEmptyListAtEnd(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests inserting a value at the end
    {
        const uint32 indexValueEnd = maxDemoValues;
        elementType peekValue;
        targetList.Insert(indexValueEnd, insertValueEnd);
        targetList.Peek(indexValueEnd, peekValue);
        result = (result && peekValue == insertValueEnd && targetList.GetSize() == maxDemoValues + 1);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType insertValueInvalidPosition>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestInsertOnNonEmptyListAtInvalidPosition(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests inserting a value at invalid position
    const uint32 indexValueInvalidPosition = maxDemoValues + 5;
    result = !targetList.Insert(indexValueInvalidPosition, insertValueInvalidPosition);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestRemoveAtBeginning(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests extracting a value at the beginning:
    {
        const uint32 indexValueBeginning = 0;
        targetList.Remove(indexValueBeginning);
        result = (result && targetList.GetSize() == maxDemoValues - 1);
        for (uint32 i = 0; i < maxDemoValues - 1; i++) {
            elementType peekValue;
            targetList.Peek(i, peekValue);
            result = (result && peekValue == demoValues[i + 1]);
        }
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestRemoveAtMiddle(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests extracting a value at the middle:
    {
        const uint32 indexValueMiddle = maxDemoValues / 2;
        targetList.Remove(indexValueMiddle);
        result = (result && targetList.GetSize() == maxDemoValues - 1);
        for (uint32 i = 0; i < indexValueMiddle - 1; i++) {
            elementType peekValue;
            targetList.Peek(i, peekValue);
            result = (result && peekValue == demoValues[i]);
        }
        for (uint32 i = indexValueMiddle; i < maxDemoValues - 1; i++) {
            elementType peekValue;
            targetList.Peek(i, peekValue);
            result = (result && peekValue == demoValues[i + 1]);
        }
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestRemoveAtEnd(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests extracting a value at the end:
    {
        const uint32 indexValueEnd = maxDemoValues - 1;
        targetList.Remove(indexValueEnd);
        result = (result && targetList.GetSize() == maxDemoValues - 1);
        for (uint32 i = 0; i < maxDemoValues - 1; i++) {
            elementType peekValue;
            targetList.Peek(i, peekValue);
            result = (result && peekValue == demoValues[i]);
        }
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestRemoveInvalidPosition(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests extracting a value from invalid position:

    const uint32 indexValueInvalidPosition = maxDemoValues + 5;
    result = !targetList.Remove(indexValueInvalidPosition);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestExtractAtBeginning(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests extracting a value at the beginning:
    {
        const uint32 indexValueBeginning = 0;
        elementType extractValue;
        targetList.Extract(indexValueBeginning, extractValue);
        result = (result && extractValue == demoValues[indexValueBeginning] && targetList.GetSize() == maxDemoValues - 1);
        for (uint32 i = 0; i < maxDemoValues - 1; i++) {
            elementType peekValue;
            targetList.Peek(i, peekValue);
            result = (result && peekValue == demoValues[i + 1]);
        }
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestExtractAtMiddle(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests extracting a value at the middle:
    {
        const uint32 indexValueMiddle = maxDemoValues / 2;
        elementType extractValue;
        targetList.Extract(indexValueMiddle, extractValue);
        result = (result && extractValue == demoValues[indexValueMiddle] && targetList.GetSize() == maxDemoValues - 1);
        for (uint32 i = 0; i < indexValueMiddle - 1; i++) {
            elementType peekValue;
            targetList.Peek(i, peekValue);
            result = (result && peekValue == demoValues[i]);
        }
        for (uint32 i = indexValueMiddle; i < maxDemoValues - 1; i++) {
            elementType peekValue;
            targetList.Peek(i, peekValue);
            result = (result && peekValue == demoValues[i + 1]);
        }
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestExtractAtEnd(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests extracting a value at the end:
    {
        const uint32 indexValueEnd = maxDemoValues - 1;
        elementType extractValue;
        targetList.Extract(indexValueEnd, extractValue);
        result = (result && extractValue == demoValues[indexValueEnd] && targetList.GetSize() == maxDemoValues - 1);
        for (uint32 i = 0; i < maxDemoValues - 1; i++) {
            elementType peekValue;
            targetList.Peek(i, peekValue);
            result = (result && peekValue == demoValues[i]);
        }
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestExtractInvalidPosition(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests extracting a value from invalid position
    const uint32 indexValueInvalid = maxDemoValues + 5;
    elementType extractValue;
    result = !targetList.Extract(indexValueInvalid, extractValue);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestPeek(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests peeking all the demo values from the target list:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        elementType peekValue;
        targetList.Peek(i, peekValue);
        result = result && (peekValue == demoValues[i]);
    }

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestPeekInvalidPosition(void) {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests peeking from invalid position
    elementType peekValue;
    uint32 invalidPosition = maxDemoValues + 5;
    result = !targetList.Peek(invalidPosition, peekValue);

    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestSquareOperator() {
    bool result = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests peeking all the demo values from the target list:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        result = result && (targetList[i] == demoValues[i]);
    }
    return result;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestSquareOperatorInvalidPosition() {
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    //Tests peeking from invalid position
    elementType lastValue;
    targetList.Peek(maxDemoValues - 1, lastValue);

    uint32 invalidPosition = maxDemoValues + 5;

    return targetList[invalidPosition] == lastValue;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
template<elementType value>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestSet(uint32 positionToSet) {
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    elementType toSet = value;
    targetList.Set(positionToSet, toSet);

    elementType test;
    targetList.Peek(positionToSet, test);
    return value == test;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestClean() {
    bool ok = true;
    uint32 initialCapacity;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    initialCapacity = targetList.GetCapacity();

    targetList.Clean();

    ok &= (targetList.GetSize() == 0);
    ok &= (targetList.GetCapacity() == initialCapacity);

    return ok;
}

template<typename elementType, uint32 listAllocationGranularity, elementType demoValues[], uint32 maxDemoValues>
bool StaticListTest<elementType, listAllocationGranularity, demoValues, maxDemoValues>::TestGetAllocatedMemoryConst() {
    bool ok = true;
    StaticList<elementType, listAllocationGranularity> targetList;

    //Initializes the target list with the demo values:
    for (uint32 i = 0; i < maxDemoValues; i++) {
        targetList.Add(demoValues[i]);
    }

    const elementType* allocatedMem = targetList.GetAllocatedMemoryConst();

    if (maxDemoValues == 0) {
        ok &= (allocatedMem == NULL_PTR(elementType));
    }
    else {
        for (uint32 i = 0; (ok) && (i < maxDemoValues); i++) {
            ok &= (allocatedMem[i] == demoValues[i]);
        }
    }

    return ok;
}

}

#endif /* STATICLISTTEST_H_ */

