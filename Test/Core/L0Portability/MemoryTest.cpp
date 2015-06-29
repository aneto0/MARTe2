/**
 * @file MemoryTest.cpp
 * @brief Source file for class MemoryTest
 * @date 26/06/2015
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
 * the class MemoryTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryTest.h"
#include "GeneralDefinitions.h"
#include "StringTestHelper.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/** @brief Constructor. */
MemoryTest::MemoryTest() {
    eventSem.Create();
    counter = 0;
    for (uint32 i = 0; i < MAX_NO_OF_MEMORY_MONITORS; i++) {
        signals[i] = False;
    }
}

MemoryTest::~MemoryTest() {
    eventSem.Close();
}

void InitializeSharedMemory(MemoryTest &myTestMemory) {
    int32* sharedInt = (int32*) MemorySharedAlloc(1, sizeof(int32));
    bool* sharedBool = (bool*) MemorySharedAlloc(2, sizeof(bool));

    if (sharedBool != NULL) {
        *sharedBool = False;
    }

    if (sharedInt != NULL) {
        (*sharedInt) = 1;
    }

    myTestMemory.eventSem.Post();

}

void IncrementSharedMemory(MemoryTest &myTestMemory) {
    int32* sharedInt = (int32*) MemorySharedAlloc(1, sizeof(int32));
    bool* sharedBool = (bool*) MemorySharedAlloc(2, sizeof(bool));
    if (sharedBool != NULL) {
        *sharedBool = True;
    }
    if (sharedInt != NULL) {
        (*sharedInt)++;
    }
    myTestMemory.eventSem.Post();
}

//Test the malloc function.
bool MemoryTest::TestMallocAndFree(int32 size) {

    //allocate a space of size integers
    int32* allocated = (int32*) MemoryMalloc(size * sizeof(int32));
    int32 i = 0;

    //check if the pointers to these memory locations are valid
    while (i < size) {
        if ((allocated + i) == NULL) {
            return False;
        }
        i++;
    }

    //free the allocated memory
    MemoryFree((void*&) allocated);
    uint32 nothing = 0;
    return allocated == NULL && !MemoryAllocationStatistics(nothing, nothing, (TID) nothing);
}

//Tests the realloc function.
bool MemoryTest::TestRealloc(int32 size1,
                             int32 size2) {
    //allocate size1 integers
    int32* allocated = (int32*) MemoryMalloc(size1 * sizeof(int32));

    //check if the pointers to these memory locations are valid
    for (int32 i = 0; i < size1; i++) {
        if ((allocated + i) == NULL) {
            MemoryFree((void*&) allocated);
            return False;
        }
        allocated[i] = i;
    }

    //reallocate the memory adding size2 integers locations
    allocated = (int32*) MemoryRealloc((void*&) allocated, (size1 + size2) * sizeof(int32));

    //check if pointers of new memory are valid and if the old memory is not corrupted
    for (int32 i = 0; i < size2; i++) {
        if ((allocated + size1 + i) == NULL) {
            MemoryFree((void*&) allocated);
            return False;
        }
        if (allocated[i] != i) {
            MemoryFree((void*&) allocated);
            return False;
        }
    }

    MemoryFree((void*&) allocated);
    //check if it implemens a malloc in case of null pointer in input.
    allocated = NULL;

    allocated = (int32*) MemoryRealloc((void*&) allocated, size1 * sizeof(int32));
    for (int32 i = 0; i < size1; i++) {
        if ((allocated + i) == NULL) {
            MemoryFree((void*&) allocated);
            return False;
        }
    }
    uint32 size = 0;
    //check if implements a free if size is 0.
    allocated = (int32*) MemoryRealloc((void*&) allocated, size);
    if (allocated != NULL) {
        MemoryFree((void*&) allocated);
        return False;
    }

    //check if it returns NULL in case of NULL input and size equal to zero.
    allocated = (int32*) MemoryRealloc((void*&) allocated, size);

    if (allocated != NULL) {
        MemoryFree((void*&) allocated);
        return False;
    }

    //this function is not implemented in linux and should return true.
    return MemoryCheck(NULL, (MemoryTestAccessMode) 0, 0);
}

//Test if the string s is copied without errors.
bool MemoryTest::TestMemoryStringDup(const char* s) {
    return StringTestHelper::Compare((char*) MemoryStringDup(s), (char*) s);
}

//Test the behavior of the shared memory
bool MemoryTest::TestSharedMemory() {
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
    bool* sharedBool = (bool*) MemorySharedAlloc(2, sizeof(bool));
    int32* sharedInt = (int32*) MemorySharedAlloc(1, sizeof(int32));

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

    MemorySharedFree(sharedBool);
    MemorySharedFree(sharedInt);

    //else return false
    return returnValue;
    return True;
}

bool MemoryTest::TestCopyAndMove() {
    int32 myIntArray[5];
    float myFloatArray[5];

    for (int32 i = 0; i < 5; i++) {
        myIntArray[i] = i;
    }

    //Copy the int array in the float array.
    uint32 sizeToCopy = 5 * sizeof(int32);
    if (!MemoryCopy(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return False;
    }

    //Check that bytes are equal indipendently from type.
    if (MemoryCompare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return False;
    }

    const char* source = "Hello World";
    const char* test = "Hello W0000";
    char* buffer[32];

    sizeToCopy = 11;

    //Another way to copy memory.
    if (!MemoryMove(buffer, source, sizeToCopy)) {
        return False;
    }

    //Source must be greater than test.
    if (MemoryCompare(source, test, sizeToCopy) != 2) {
        return False;
    }

    //Test must be less than source.
    if (MemoryCompare(test, source, sizeToCopy) != 1) {
        return False;
    }

    //Test the result in case of NULL argument.
    if (MemoryCompare(NULL, source, sizeToCopy) != -1) {
        return False;
    }

    return True;

}

bool MemoryTest::TestSetAndSearch() {

    uint32 size = 10;
    char* buffPointer = (char*) MemoryMalloc(size);

    if (buffPointer == NULL) {
        return False;
    }

    //Set first 5 bytes to 'o'.
    char myFavouriteChar = 'o';
    uint32 charSize = 5;
    if (!MemorySet(buffPointer, myFavouriteChar, size)) {
        MemoryFree((void*&) buffPointer);
        return False;
    }

    char* newBuffPointer = buffPointer + charSize;

    //Set last 5 bytes to 'u'.
    myFavouriteChar = 'u';

    if (!MemorySet(newBuffPointer, myFavouriteChar, size - charSize)) {
        MemoryFree((void*&) buffPointer);
        return False;
    }

    char test[] = "ooooouuuuu";

    //Check that the Set result is correct.
    if (MemoryCompare(test, buffPointer, size) != 0) {
        MemoryFree((void*&) buffPointer);
        return False;
    }

    //Test the Search function.
    if (MemorySearch(buffPointer, myFavouriteChar, size) != newBuffPointer) {
        MemoryFree((void*&) buffPointer);
        return False;
    }

    //Test the result of Search when the character is not found.
    char imNotInBuffer = 'a';
    if (MemorySearch(buffPointer, imNotInBuffer, size) != NULL) {
        MemoryFree((void*&) buffPointer);
        return False;
    }

    MemoryFree((void*&) buffPointer);

    return True;

}

bool MemoryTest::TestHeader() {

    //set the header flag
    Memory::defaultAllocationFlag = MemoryStandardMemory | MemoryAddHeader;

    //creates an array of 10 integers
    uint32 *arrayInt = (uint32*) MemoryMalloc(sizeof(uint32) * 10);

    //sets the ninth element
    arrayInt[9] = 3;
    uint32 size;
    TID tid;

    //gets the header informations
    if (!MemoryGetHeaderInfo(arrayInt, size, tid)) {
        return False;
    }

    if (size != (sizeof(uint32) * 10 + sizeof(uint32) + sizeof(TID))) {
        printf("\nerror 1\n");
        return False;
    }

    if (Threads::Id() != tid) {
        return False;
    }

    //change the flag
    Memory::defaultAllocationFlag = MemoryStandardMemory;

    //now the function should return false
    if (MemoryGetHeaderInfo(arrayInt, size, tid)) {
        return False;
    }

    //to free the memory the flag must be the same at the moment of the creation otherwise
    //the pointer is invalid
    Memory::defaultAllocationFlag = MemoryStandardMemory | MemoryAddHeader;

    MemoryFree((void*&) arrayInt);

    return True;
}

void AllocateFunction(MemoryTest &m) {

    //printf("\n%d\n", m.counter);
    //creates an array of i integers on the heap
    void* p = Memory::Malloc(sizeof(uint32));
    while (!(m.signals[m.counter])){
        SleepSec(1e-3);
    }
    Memory::Free(p);
    m.signals[m.counter] = False;
}

bool MemoryTest::TestDatabase() {

    Memory::defaultAllocationFlag = MemoryAddHeader | MemoryStatistics;
    TID tids[MAX_NO_OF_MEMORY_MONITORS];
    for (counter = 0; counter < MAX_NO_OF_MEMORY_MONITORS; counter++) {
        Threads::BeginThread((ThreadFunctionType) AllocateFunction, this);
    }

    bool ret = True;

    for (counter = 0; counter < MAX_NO_OF_MEMORY_MONITORS; counter++) {
        uint32 size;
        uint32 chunks;

        if (!(Memory::AllocationStatistics(size, chunks, tids[counter]))) {
            printf("\nError as\n");
            ret = False;
        }

        if (size != (sizeof(uint32)) || chunks != 1) {
            printf("\nerror infos\n");
            ret = False;
        }

        signals[counter] = True;

        while (signals[counter]) {
            SleepSec(1e-3);
        }

    }

    return ret;

}

