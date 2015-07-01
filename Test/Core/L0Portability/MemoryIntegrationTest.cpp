/**
 * @file MemoryIntegrationTest.cpp
 * @brief Source file for class MemoryIntegrationTest
 * @date 29/06/2015
 * @author Giuseppe Ferrò
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
 * the class MemoryIntegrationTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryIntegrationTest.h"
#include "GeneralDefinitions.h"
#include "StringTestHelper.h"
#include "ThreadInformation.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/** @brief Constructor. */
MemoryIntegrationTest::MemoryIntegrationTest() {
    eventSem.Create();
    counter = 0;
    for (uint32 i = 0; i < MAX_NO_OF_MEMORY_MONITORS - 1; i++) {
        signals[i] = False;
        sizeStore[i] = 0;
        chunksStore[i] = 0;
    }
}

MemoryIntegrationTest::~MemoryIntegrationTest() {
    eventSem.Close();
}

void InitializeSharedMemory(MemoryIntegrationTest &myTestMemory) {
    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));
    bool* sharedBool = (bool*) Memory::SharedAlloc(2, sizeof(bool));

    if (sharedBool != NULL) {
        *sharedBool = False;
    }

    if (sharedInt != NULL) {
        (*sharedInt) = 1;
    }

    myTestMemory.eventSem.Post();

}

void IncrementSharedMemory(MemoryIntegrationTest &myTestMemory) {
    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));
    bool* sharedBool = (bool*) Memory::SharedAlloc(2, sizeof(bool));
    if (sharedBool != NULL) {
        *sharedBool = True;
    }
    if (sharedInt != NULL) {
        (*sharedInt)++;
    }
    myTestMemory.eventSem.Post();
}

//Test the behavior of the shared memory
bool MemoryIntegrationTest::TestSharedMemory() {
    //reset an event sem
    eventSem.Reset();

    //launch two threads, one initialize the shared int to one and the shared bool to false.
    Threads::BeginThread((ThreadFunctionType) InitializeSharedMemory, this);
    //wait the inizialization of the shared memory

    eventSem.Wait();
    eventSem.Reset();

    //this thread increment the shared integer and impose true the shared bool
    Threads::BeginThread((ThreadFunctionType) IncrementSharedMemory, this);
    eventSem.Wait();

    //obtain the pointers to the shared memories
    bool* sharedBool = (bool*) Memory::SharedAlloc(2, sizeof(bool));
    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));

    if (sharedInt == NULL) {

        return False;
    }

    int32 j = 0;

    //wait that the second thread increments the shared int
    while ((*sharedInt) < 2) {
        if (j++ > 100) {

            return False;
        }
        SleepSec(20e-3);
    }
    bool returnValue = False;
    if (sharedBool == NULL) {

        return False;
    }
    returnValue = *sharedBool;

    //release the shared memory

    Memory::SharedFree(sharedBool);
    Memory::SharedFree(sharedInt);

    //else return false
    return returnValue;
}

bool MemoryIntegrationTest::TestHeader() {

    uint32 size;
    TID tid;
    bool ret = True;
    //creates an array of 10 integers
    uint32 *arrayInt = (uint32*) Memory::Malloc(sizeof(uint32) * 10);

#ifdef MEMORY_STATISTICS
    //sets the ninth element
    arrayInt[9] = 3;

    //gets the header informations
    if (!Memory::GetHeaderInfo(arrayInt, size, tid)) {
        ret = False;
    }

    if (size < (sizeof(uint32) * 10 + sizeof(uint32) + sizeof(TID))) {
        ret = False;
    }

    if (Threads::Id() != tid) {
        ret = False;
    }

#else

    ret= !(Memory::GetHeaderInfo(arrayInt, size, tid));

#endif

    Memory::Free((void*&) arrayInt);

    return ret;
}

void AllocateFunction(MemoryIntegrationTest &m) {

    uint32 myIndex = m.counter;
    //creates an array on the heap

    //save the previous thread's informations about its allocated memory
    Memory::AllocationStatistics(m.sizeStore[myIndex], m.chunksStore[myIndex]);

    void* p[32];

    int32 i = 0;
    uint32 amount = myIndex;
    while (amount >= 32) {
        p[i] = Memory::Malloc(sizeof(char) * 32);
        amount -= 32;
        i++;
    }

    if (amount > 0) {

        p[i] = Memory::Malloc(sizeof(char) * amount);
    }
    else {
        i--;
    }

    m.signals[myIndex] = True;

    while (m.signals[myIndex]) {
        SleepSec(1e-3);
    }

    while (i >= 0) {
        Memory::Free(p[i]);
        i--;
    }
    m.signals[myIndex] = True;
}

bool MemoryIntegrationTest::TestDatabase() {

    int32 size = 0;
    int32 chunks = 0;

#ifdef MEMORY_STATISTICS

    int32 minHeaderSize = sizeof(uint32) + sizeof(TID);

    Memory::ClearStatisticsDatabase();
    TID tids[MAX_NO_OF_MEMORY_MONITORS - 1];
    for (counter = 0; counter < MAX_NO_OF_MEMORY_MONITORS - 1; counter++) {
        tids[counter] = Threads::BeginThread((ThreadFunctionType) AllocateFunction, this);
        while (!(signals[counter])) {
            SleepSec(1e-3);
        }
    }

    bool ret = True;

    for (counter = 0; counter < MAX_NO_OF_MEMORY_MONITORS - 1; counter++) {

        if (!(Memory::AllocationStatistics(size, chunks, tids[counter]))) {
            ret = False;
        }

        //the first thread allocates space on the threads database, while the last

        //the number of chunks is normally counter/32 +1
        //or counter/32 if counter%32 is zero.
        int32 testChunks = counter >> 5;

        if ((counter & 0x1f) != 0) {
            testChunks++;

        }

        //the minimum size
        int32 testSize = sizeof(char) * counter + sizeStore[counter] + testChunks * minHeaderSize;

        //add the previous already allocated chunks
        testChunks += chunksStore[counter];


        if (size < testSize || chunks != testChunks) {
            ret = False;
        }

        signals[counter] = False;

        while (!(signals[counter])) {
            SleepSec(1e-3);
        }

    }
    return ret;

#else

    //in this case it should return false.
    return !(Memory::AllocationStatistics(size, chunks));

#endif

}

