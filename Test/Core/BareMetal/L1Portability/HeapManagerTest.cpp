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
#include "HeapI.h"
#include "StringHelper.h"
#include "HeapManager.h"

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
}

HeapManagerTest::~HeapManagerTest() {
}

bool HeapManagerTest::TestMallocDefaultName() {
    uint32 size = 2;
    ptr = Malloc(size * sizeof(int32));
    retVal = (ptr != NULL);
    Free(ptr);
    return retVal;
}

bool HeapManagerTest::TestMallocSpecificName() {
    uint32 size = 2;
    AddHeap(&sh);
    ptr = Malloc(size * sizeof(int32), "StandardHeap");
    retVal = (ptr != NULL);
    Free(ptr);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestMallocInvalidName() {
    uint32 size = 2;
    AddHeap(&sh);
    ptr = Malloc(size * sizeof(int32), "WrongName");
    retVal = (ptr == NULL);
    Free(ptr);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestFree() {
    uint32 size = 2;
    ptr = Malloc(size * sizeof(int32));
    retVal = Free(ptr);
    retVal &= (ptr == NULL);
    return retVal;
}

bool HeapManagerTest::TestFreeInvalidPointer() {
    ptr = &sh;
    retVal = !Free(ptr);
    return retVal;
}

bool HeapManagerTest::TestRealloc() {
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

bool HeapManagerTest::TestReallocInvalidPointer() {
    uint32 size = 2;
    void *ptr1 = NULL;
    void *invalidptr = NULL;
    ptr = Malloc(size * sizeof(int32));
    size++;
    ptr1 = Realloc(invalidptr, size * sizeof(int32));
    retVal = (ptr1 != NULL);
    retVal &= (invalidptr == ptr1);
    Free(ptr1);
    return retVal;
}

bool HeapManagerTest::TestAddHeap() {
    retVal = AddHeap(&sh);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestAddHeapNULL() {
    retVal = !AddHeap(NULL);
    return retVal;
}

bool HeapManagerTest::TestAddHeapRepetedHeap() {
    AddHeap(&sh);
    retVal = !AddHeap(&sh);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestAddHeapTooMuch() {
    StandardHeap sh1;
    StandardHeap sh2;
    StandardHeap sh3;
    StandardHeap sh4;
    StandardHeap sh5;
    StandardHeap sh6;
    StandardHeap sh7;
    StandardHeap sh8;
    StandardHeap sh9;
    StandardHeap sh10;
    StandardHeap sh11;
    StandardHeap sh12;
    StandardHeap sh13;
    StandardHeap sh14;
    StandardHeap sh15;
    StandardHeap sh16;

    AddHeap(&sh);
    AddHeap(&sh1);
    AddHeap(&sh2);
    AddHeap(&sh3);
    AddHeap(&sh4);
    AddHeap(&sh5);
    AddHeap(&sh6);
    AddHeap(&sh7);
    AddHeap(&sh8);
    AddHeap(&sh9);
    AddHeap(&sh10);
    AddHeap(&sh11);
    AddHeap(&sh12);
    AddHeap(&sh13);
    AddHeap(&sh14);
    retVal = AddHeap(&sh15);
    retVal &= !AddHeap(&sh16);

    RemoveHeap(&sh);
    RemoveHeap(&sh1);
    RemoveHeap(&sh2);
    RemoveHeap(&sh3);
    RemoveHeap(&sh4);
    RemoveHeap(&sh5);
    RemoveHeap(&sh6);
    RemoveHeap(&sh7);
    RemoveHeap(&sh8);
    RemoveHeap(&sh9);
    RemoveHeap(&sh10);
    RemoveHeap(&sh11);
    RemoveHeap(&sh12);
    RemoveHeap(&sh13);
    RemoveHeap(&sh14);
    RemoveHeap(&sh15);

    return retVal;
}

bool HeapManagerTest::TestFindHeapByName() {
    void * ptr;
    AddHeap(&sh);
    ptr = FindHeap("StandardHeap");
    retVal = (ptr == &sh);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestFindHeapByNameInvalidName() {
    void * ptr;
    AddHeap(&sh);
    ptr = FindHeap("WrongName");
    retVal = (ptr == NULL);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestFindHeapByAddress() {
    uint32 size = 4;
    void *ptr;
    void *ptr1;
    AddHeap(&sh);
    ptr1 = Malloc(size * sizeof(uint32), "StandardHeap");
    ptr = FindHeap(ptr1);
    retVal = (ptr == &sh);
    Free(ptr1);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestFindHeapByAddressInvalidAddress() {
    void *ptr1 = NULL;
    ptr = &sh;
    ptr1 = FindHeap(ptr);
    retVal = (ptr1 == NULL);
    return retVal;
}

bool HeapManagerTest::TestFindHeapByAddress2Heaps() {
    uint32 size = 4;
    void *ptr1;
    void *shptr[3] = { NULL, NULL, NULL };
    void *ahptr;
    void *copyahptr = NULL;
    AddHeap(&sh);
    AddHeap(&ah);
    ptr1 = Malloc(size * sizeof(uint32), "StandardHeap");
    //Malloc several times in order to increase the "size of the StandarHeap, then free the memory
    //in order to be used by "AuxHeap". As a result the two heaps memory span is shared.
    shptr[0] = Malloc(size * sizeof(uint32), "StandardHeap");
    shptr[1] = Malloc(size * sizeof(uint32), "StandardHeap");
    shptr[2] = Malloc(size * sizeof(uint32), "StandardHeap");
    Free(shptr[0]);
    Free(shptr[1]);
    Free(shptr[2]);
    ahptr = Malloc(size * sizeof(uint32), "AuxHeap");
    ptr = FindHeap(ahptr);
    retVal = (ptr == &ah);
    copyahptr = ahptr;
    Free(ahptr);
    RemoveHeap(&ah);
    ptr = FindHeap(copyahptr);
    retVal &= (ptr == &sh);
    Free(ptr1);
    RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestGetStandardHeap() {
    ptr = GetStandardHeap();
    retVal = (ptr != NULL);
    return retVal;
}

bool HeapManagerTest::TestRemoveHeap() {
    AddHeap(&sh);
    retVal = RemoveHeap(&sh);
    return retVal;
}

bool HeapManagerTest::TestDuplicateDefault() {
    void *ptr1 = NULL;
    uint32 size = 10;
    ptr = Malloc(size * sizeof(int32));
    ptr1 = Duplicate(ptr, size);
    retVal = (ptr1 != NULL);
    Free(ptr);
    Free(ptr1);
    return retVal;
}

bool HeapManagerTest::TestDuplicateSpecificName() {
    uint32 size = 4;
    int32 * auxIntPtr = NULL;
    int32 * auxIntPtr1 = NULL;
    void *ptr1 = NULL;
    AddHeap(&ah);
    ptr = Malloc(size * sizeof(int32), "AuxHeap");
    auxIntPtr = (int32*) (ptr);
    *auxIntPtr = 0;
    *(auxIntPtr + 1) = 1;
    *(auxIntPtr + 2) = 2;
    *(auxIntPtr + 3) = 3;
    ptr1 = Duplicate(ptr, size * sizeof(int32), "AuxHeap");
    auxIntPtr1 = (int32*) ptr1;
    retVal = (*auxIntPtr == *auxIntPtr1);
    retVal &= (*(auxIntPtr + 1) == *(auxIntPtr1 + 1));
    retVal &= (*(auxIntPtr + 2) == *(auxIntPtr1 + 2));
    retVal &= (*(auxIntPtr + 3) == *(auxIntPtr1 + 3));
    Free(ptr);
    Free(ptr1);
    RemoveHeap(&ah);
    return retVal;
}

bool HeapManagerTest::TestDuplicateString() {
    uint32 size = 4;
    char8 *auxCharPtr = NULL;
    char8 *auxCharPtr1 = NULL;
    void *ptr1 = NULL;
    AddHeap(&ah);
    ptr = Malloc(size * sizeof(char8), "AuxHeap");
    auxCharPtr = (char8*) (ptr);
    *auxCharPtr = 'a';
    *(auxCharPtr + 1) = 'b';
    *(auxCharPtr + 2) = 'c';
    *(auxCharPtr + 3) = '\0';
    ptr1 = Duplicate(ptr, 0, "AuxHeap");
    auxCharPtr1 = (char8*) ptr1;
    retVal = (*auxCharPtr == *auxCharPtr1);
    retVal &= (*(auxCharPtr + 1) == *(auxCharPtr1 + 1));
    retVal &= (*(auxCharPtr + 2) == *(auxCharPtr1 + 2));
    retVal &= (*(auxCharPtr + 3) == *(auxCharPtr1 + 3));
    Free(ptr);
    Free(ptr1);
    RemoveHeap(&ah);
    return retVal;
}

bool HeapManagerTest::TestDuplicateNoAllocationMemory() {
    const char8 *s = "hello";
    ptr = Duplicate(s);
    retVal = (StringHelper::Compare(s, (char*) (ptr)) == 0);
    return retVal;
}

