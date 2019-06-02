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

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "HeapManagerTest.h"
//#include <GlobalObjectsDatabase.h>
//#include "StringHelper.h"
#include "HeapManager.h"
#include "HeapI.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe::HeapManager;
using namespace MARTe;

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

    ErrorManagement::ErrorType ret;

    AuxHeap sh;

    ret = InstallAllocator(&sh,customHeapId);

    REPORT_ERROR(ret,"InstallAllocator customHeapId failed");

    if (ret){
        ptr = Malloc(size * sizeof(int32), customHeapId);

        ret.outOfMemory = (ptr==NULL);
        REPORT_ERROR(ret,"Malloc @customHeapId failed");

        if (ret){
            Free(ptr);
        }

        ret = RemoveAllocator(customHeapId);
        REPORT_ERROR(ret,"RemoveAllocator failed");
    }

    return ret;
}

/*
 * Allocates from a non-initialised Heap.
 * Memory will be taken from the singleton heap
 */
bool HeapManagerTest::TestMallocInvalidName() {

    uint32 cnt = allocatedBlocks[singletonHeapId];
    ptr = Malloc(2 * sizeof(int32), customHeapId);

    //check that the singleton heap use count has grown
    retVal = (ptr != NULL) && (allocatedBlocks[singletonHeapId] == cnt + 1);

    Free(ptr);
    return retVal;
}

bool HeapManagerTest::TestFree() {
    uint32 size = 2;
    uint32 cnt = allocatedBlocks[standardHeapId];
    ptr = Malloc(size * sizeof(int32),standardHeapId);
    Free(ptr);
    return (cnt == allocatedBlocks[standardHeapId]);
}

bool HeapManagerTest::TestFreeInvalidPointer() {
    uint32 size = 2;
    uint32 cnt = allocatedBlocks[standardHeapId];
    ptr = Malloc(size * sizeof(int32),standardHeapId);
    Free(ptr);
    uint32 cnt2 = allocatedBlocks[standardHeapId];
    // MARTe produces an error message here
    Free(ptr);
    uint32 cnt3 = allocatedBlocks[standardHeapId];
    return (cnt == cnt2) && (cnt2 == cnt3);
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
    Free(ptr);
    Free(ptr1);
    return retVal;
}

bool HeapManagerTest::TestAddHeap() {
    ErrorManagement::ErrorType ret;

    AuxHeap sh;

    ret = InstallAllocator(&sh,customHeapId);

    REPORT_ERROR(ret,"InstallAllocator customHeapId failed");

    if (ret){
        ret = RemoveAllocator(customHeapId);
        REPORT_ERROR(ret,"RemoveAllocator failed");
    }
    return ret;
}

bool HeapManagerTest::TestAddHeapNULL() {
    ErrorManagement::ErrorType ret;

    ret = InstallAllocator(NULL,customHeapId);

    return ret.IsErrorCode();
}

bool HeapManagerTest::TestAddHeapRepetedHeap() {
    ErrorManagement::ErrorType ret;
    ErrorManagement::ErrorType ret2;

    AuxHeap sh;

    ret = InstallAllocator(&sh,customHeapId);
    REPORT_ERROR(ret,"InstallAllocator customHeapId failed");

    if (ret){
        ret2 = InstallAllocator(&sh,customHeapId);

        RemoveAllocator(customHeapId);
    }

    // an error is needed here!
    return ret2.IsErrorCode();
}

bool HeapManagerTest::TestAddHeapTooMany() {
    ErrorManagement::ErrorType ret;

    AuxHeap sh;

    ret = InstallAllocator(&sh,MAX_HEAPS);
    return ret.IsErrorCode();
}

bool HeapManagerTest::TestFindHeapByName() {
    HeapId id = HeapIdFromName("standardHeap");

    return ((id > 0) && (id < MAX_HEAPS));

}

bool HeapManagerTest::TestFindHeapByNameInvalidName() {
    HeapId id = HeapIdFromName("wrongHeap");

    // returns standardHeapId and an error message on the log
    return (id == standardHeapId);
}

bool HeapManagerTest::TestRemoveHeap() {
    ErrorManagement::ErrorType ret;

    AuxHeap sh;

    ret = InstallAllocator(&sh,customHeapId);
    REPORT_ERROR(ret,"InstallAllocator customHeapId failed");

    if (ret){
        ret = RemoveAllocator(customHeapId);
    }

    return ret;
}

