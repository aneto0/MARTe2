/**
 * @file HeapManagerTest.cpp
 * @brief Source file for class HeapManagerTest
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

 * @details This source file contains the definition of all the methods for
 * the class HeapManagerTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HeapManagerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace HeapManager;

HeapManagerTest::HeapManagerTest() {
    ptr = NULL;
    retVal = false;
    // Auto-generated constructor stub for HeapManagerTest
    // TODO Verify if manual additions are needed
}

HeapManagerTest::~HeapManagerTest() {
    // Auto-generated destructor stub for HeapManagerTest
    // TODO Verify if manual additions are needed
}

bool HeapManagerTest::TestMallocDefaultName(){
    uint32 size = 2;
    ptr = Malloc(size * sizeof(int32));
    retVal = (ptr != NULL);
    Free(ptr);
    return retVal;
}

bool HeapManagerTest::TestMallocSpecificName(){
    uint32 size = 2;
    AddHeap(&sh);
    ptr = Malloc(size*sizeof(int32), "StandardHeap");
    retVal = (ptr != NULL);
    Free(ptr);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestFree(){
    uint32 size = 2;
    ptr = Malloc(size * sizeof(int32));
    retVal = (ptr != NULL);
    retVal &= Free(ptr);
    retVal &= (ptr == NULL);
    return retVal;
}

bool HeapManagerTest::TestRealloc(){
    uint32 size = 2;
    void *ptr1 = NULL;
    ptr = Malloc(size * sizeof(int32));
    size++;
    ptr1 = Realloc(ptr, size * sizeof(int32));
    retVal = (ptr1 != NULL);
    retVal &= (ptr == ptr1);
    Free(ptr);
    return retVal;
}

bool HeapManagerTest::TestAddHeap(){
    retVal = AddHeap(&sh);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestFindHeapByName(){
    void * ptr;
    AddHeap(&sh);
    ptr = FindHeap("StandardHeap");
    retVal = (ptr == &sh);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestFindHeapByAddress(){
    uint32 size = 4;
    void *ptr;
    void *ptr1;
    AddHeap(&sh);
    ptr1 = Malloc(size*sizeof(uint32), "StandardHeap");
    ptr = FindHeap(ptr1);
    retVal = (ptr == &sh);
    Free(ptr1);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestRemoveHeap(){
    AddHeap(&sh);
    retVal = RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestDuplicateDefault(){
    void *ptr1 = NULL;
    ptr = Malloc(10 * sizeof(int32));
    ptr1 = Duplicate(ptr);
    retVal = (ptr1 != NULL);
    return retVal;
}


