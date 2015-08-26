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

#include "../../../Source/Core/L0Portability/HeapI.h"
#include "../../../Source/Core/L0Portability/OperatingSystem/Linux/StandardHeap.h"
#include "../../../Source/Core/L0Portability/HeapManager.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class HeapManagerTest {
public:
    HeapManagerTest();
    virtual ~HeapManagerTest();

    void *ptr;

    bool retVal;

    HeapManager::StandardHeap sh;

    /**
     * @brief Test HeapManager::Malloc function using the default options
     * @return true if the Malloc function return a Non NULL pointer
     */
    bool TestMallocDefaultName();

    /**
     * @brief Test Heapmanager::Malloc function using a specifics heap name
     * @return true if the Malloc function return a Non NULL pointer
     */
    bool TestMallocSpecificName();

    /**
     * @brief Test HeapManager::Free
     * @return true if Free returns true and the pointer is NULL
     */
    bool TestFree();

    /**
     *@brief Allocates some memory and then Reallocate
     *@return true if the returned pointer of the Realloc is not NULL
     */
    bool TestRealloc();

    /**
     * @brief Test AddHeap function
     * @return return true if AddHeap returns true.
     */
    bool TestAddHeap();

    /**
     * @brief Test FindHeap using the name
     * @detail add a heap and then try to find it by his name.
     * @return true if the pointer returned by FindHeap points to the heap introduced.
     */
    bool TestFindHeapByName();

    /**
     * @brief Test FindHeap using a pointer address
     * @detail add a heap to the list, use the Malloc function to allocate some memory and then find heap
     * at which this allocated memory belongs to.
     * @return true if the returned pointer of the find function is the same than the address of the
     * object added to the list.
     */
    bool TestFindHeapByAddress();

    /**
     * @brief Add and remove a heap from the list
     * @return true if the returned value of the RemoveHeap function is true.
     */
    bool TestRemoveHeap();

    /**
     *
     */
    bool TestDuplicateDefault();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L0PORTABILITY_HeapManagerTest_H_ */

