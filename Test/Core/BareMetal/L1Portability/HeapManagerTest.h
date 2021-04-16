/**
 * @file HeapManagerTest.h
 * @brief Header file for class HeapManagerTest
 * @date 25/08/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class HeapManagerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L0PORTABILITY_HeapManagerTest_H_
#define TEST_CORE_L0PORTABILITY_HeapManagerTest_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AuxHeap.h"
#include INCLUDE_FILE_ENVIRONMENT(BareMetal,L1Portability,ENVIRONMENT,StandardHeap.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class HeapManagerTest {
public:

    /*
     * @brief Default constructor
     */
    HeapManagerTest();

    /*
     * @brief Destructor
     */
    virtual ~HeapManagerTest();

    /*
     * @brief Pointer used by all tests to point to the memory allocated.
     */
    void *ptr;

    /*
     * @brief Flag used by all tests to store the result of the execution.
     */
    bool retVal;

    /*
     * @brief An instance of the HeapManager::StandardHeap to be used on tests
     */
    MARTe::StandardHeap sh;

    /*
     * @brief An instance of the HeapManager::AuxHeap to be used on tests
     */
    MARTe::AuxHeap ah;

    /**
     * @brief Test HeapManager::Malloc function using the default options
     * @return true if the Malloc function return a Non NULL pointer
     */
    bool TestMallocDefaultName();

    /**
     * @brief Test Heapmanager::Malloc function using a specific heap name
     * @return true if the Malloc function return a Non NULL pointer
     */
    bool TestMallocSpecificName();

    /**
     * @brief Test Heapmanager::Malloc function using an invalid heap name
     * @return true if the Malloc function return a NULL pointer
     */
    bool TestMallocInvalidName();

    /**
     * @brief Test HeapManager::Free in the standard conditions
     * @return true if Free returns true and the pointer is NULL
     */
    bool TestFree();

    /**
     * @brief Test HeapManager::Free using an invalid pointer
     * @return true if Free returns true and the pointer is NULL
     */
    bool TestFreeInvalidPointer();

    /**
     * @brief Test HeapManager::Realloc with the standard conditions
     * @return true if the returned pointer == ptr & returned pointer != NULL
     */
    bool TestRealloc();

    /**
     * @brief Test HeapManager::Realloc with an invalid pointer
     * @return true if the returned pointer == ptr & returned pointer = NULL
     */
    bool TestReallocInvalidPointer();

    /**
     * @brief Test HeapManager::AddHeap function in the standard conditions.
     * @return return true if AddHeap returns true.
     */
    bool TestAddHeap();

    /**
     * @brief Test HeapManager::AddHeap function with a NULL pointer
     * @return return true if AddHeap returns false.
     */
    bool TestAddHeapNULL();

    /**
     * @brief Test HeapManager::AddHeap function trying to add the same heap twice
     * @return return true if the second AddHeap returns false.
     */
    bool TestAddHeapRepetedHeap();

    /**
     * @brief Test HeapManager::AddHeap function adding more heaps than the maximum number of heaps allowed
     * @return true if the second-to-last AddHep returns true and the last AddHeap returns false.
     */
    bool TestAddHeapTooMuch();

    /**
     * @brief Test HeapManager::FindHeap using the name.
     * @detail add a heap and then try to find it by name.
     * @return true if the pointer returned by FindHeap points to the heap introduced.
     */
    bool TestFindHeapByName();

    /**
     * @brief Test HeapManager::FindHeap using an invalid name.
     * @return true if the pointer returned by FindHeap is NULL.
     */
    bool TestFindHeapByNameInvalidName();

    /**
     * @brief Test HeapManager::FindHeap using a pointer address memory.
     * @detail add a heap to the list, use the Malloc function to allocate some memory and then find heap
     * at which this allocated memory belongs to.
     * @return true if the returned pointer of the find function is the same than the address of the
     * object added to the list.
     */
    bool TestFindHeapByAddress();

    /**
     * @brief Test HeapManager::FindHeap using an invalid pointer address memory.
     * @return true if the returned pointer is NULL.
     */
    bool TestFindHeapByAddressInvalidAddress();

    /**
     * @brief Test HeapManager::FindHeap using a pointer address.
     * @detail add two heaps to the list, use the Malloc and free memory in order to have the two
     * heaps span memory shared (it is assuming that the memory is consumed one next to the other).
     * Then is used FinHeap using the address. The returned pointer should be the heap with smaller
     * memory span. Finally the smaller memory span is removed from the list and the FindHeap with the
     * same memory pointer is used. The returned pointer should be the address of the remain heap.
     * @return true if the fist FindHeap points to the heap with less memory span and the second FindHeap
     * points to the remain heap.
     */
    bool TestFindHeapByAddress2Heaps();

    /**
     * @brief Basic test HeapManager::GetStandardHeap functions
     * @return true if HeapManager::GetStandardHeap returns true.
     */
    bool TestGetStandardHeap();

    /**
     * @brief Test HeapManager::TestRemoveHeap function
     * @return true if the returned value of the RemoveHeap function is true.
     */
    bool TestRemoveHeap();

    /**
     * @brief Test HeapManager::Duplicate using the default inputs.
     * @return true if Duplicate returns a non NULL pointer.
     */
    bool TestDuplicateDefault();

    /**
     * @brief Test HeapManager::Duplicate using specific name.
     * @brief save, fill and duplicate a region of memory. Then it is check that the values are correctly
     * copied from origin to destination.
     * @return true if the values of the destination are the same than the origin.
     */
    bool TestDuplicateSpecificName();

    /**
     * @brief Test HeapManager::Duplicate using Strings.
     * @brief save, fill and duplicate a region of memory. Then it is check that the values are correctly
     * copied from origin to destination.
     * @return true if the values of the destination are the same than the origin.
     */
    bool TestDuplicateString();

    /**
     * brief Test HeapManager::Duplicate without allocating memory.
     * @return true if returned the copy is equal to the original.
     */
    bool TestDuplicateNoAllocationMemory();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L0PORTABILITY_HeapManagerTest_H_ */

