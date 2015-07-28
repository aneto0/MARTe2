/**
 * @file MemoryTest.cpp
 * @brief Source file for class MemoryTest
 * @date 29/06/2015
 * @author Giuseppe Ferr�
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
#include "Threads.h"
#include "Sleep.h"
#include "Atomic.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryTest::MemoryTest() {

    eventSem.Create();
    counter = 0;
    for (uint32 i = 0; i < MAX_NO_OF_MEMORY_MONITORS - 1; i++) {
        signals[i] = false;
        sizeStore[i] = 0;
        chunksStore[i] = 0;
    }
}

MemoryTest::~MemoryTest() {
}

bool MemoryTest::TestMalloc(uint32 size) {

    //allocate a space of size integers
    int32* allocated = (int32*) Memory::Malloc(size * sizeof(int32));
    uint32 i = 0;

    //check if the pointers to these memory locations are valid
    while (i < size) {
        if ((allocated + i) == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }
        i++;
    }

    //free the allocated memory
    if (!Memory::Free((void*&) allocated)) {
        return false;
    }

    //allocates nothing
    void* p = Memory::Malloc(0);
    return p == NULL;

}

bool MemoryTest::TestFree(uint32 size) {

    void *p = NULL;
    //frees a null pointer
    if (Memory::Free(p)) {
        return false;
    }

    //allocate a space of size integers
    int32* allocated = (int32*) Memory::Malloc(size * sizeof(int32));

    //free the allocated memory
    if (!Memory::Free((void*&) allocated)) {
        return false;
    }

    //allocated should be null
    return allocated == NULL;
}

bool MemoryTest::TestRealloc(uint32 size1,
                             uint32 size2) {
    //allocate size1 integers
    int32* allocated = (int32*) Memory::Malloc(size1 * sizeof(int32));

    //check if the pointers to these memory locations are valid
    for (uint32 i = 0; i < size1; i++) {
        if ((allocated + i) == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }
        allocated[i] = i;
    }

    //reallocate the memory adding size2 integers locations
    allocated = (int32*) Memory::Realloc((void*&) allocated, (size1 + size2) * sizeof(int32));

    //check if pointers of new memory are valid and if the old memory is not corrupted
    for (uint32 i = 0; i < size2; i++) {
        if ((allocated + size1 + i) == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }

    }

    for (uint32 i = 0; i < size1; i++) {
        if (allocated + i == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }

        if (allocated[i] != (int32) i) {
            Memory::Free((void*&) allocated);
            return false;
        }
    }

    Memory::Free((void*&) allocated);
    //check if it implements a malloc in case of null pointer in input.
    allocated = NULL;

    allocated = (int32*) MemoryRealloc((void*&) allocated, size1 * sizeof(int32));

    //checks if the memory is allocated correctly
    MemoryTestAccessMode mtam(true,true,true);
    if (!Memory::Check(allocated, mtam, size1 * sizeof(int32))) {
        return false;
    }

    //manual check
    for (uint32 i = 0; i < size1; i++) {
        if ((allocated + i) == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }
    }

    uint32 size = 0;
    //check if implements a free if size is 0.
    allocated = (int32*) Memory::Realloc((void*&) allocated, size);
    if (allocated != NULL) {
        Memory::Free((void*&) allocated);
        return false;
    }

    //check if it returns NULL in case of NULL input and size equal to zero.
    allocated = (int32*) Memory::Realloc((void*&) allocated, size);

    if (allocated != NULL) {
        Memory::Free((void*&) allocated);
        return false;
    }

    return true;

}

bool MemoryTest::TestStringDup(const char8 *s) {

    char* p = (char8*) MemoryStringDup(s);

    if (!StringTestHelper::Compare(p, s)) {
        return false;
    }

    if (!Memory::Free((void*&) p)) {
        return false;
    }

    //should return null in case of null input.
    p = MemoryStringDup(NULL);

    return p == NULL;
}

bool MemoryTest::TestCheck(uint32 size) {

    //allocate a space of size integers
    int32* allocated = (int32*) Memory::Malloc(size * sizeof(int32));

    MemoryTestAccessMode mtam(true,true,true);

    //checks if all the memory is allocated correctly
    if (!Memory::Check(allocated, mtam, size * sizeof(int32))) {
        return false;
    }

    //checks if a part the memory is allocated correctly
    if (!Memory::Check(allocated, mtam, (size / 2) * sizeof(int32))) {
        return false;
    }

    //0 as size
    uint32 testSize = 0;
    if (!Memory::Check(allocated, mtam, testSize)) {
        return false;
    }

    if (!Memory::Free((void*&) allocated)) {
        return false;
    }

    //the check function on a null pointer should return false
    return !Memory::Check(NULL, mtam, size);

}

bool MemoryTest::TestSharedAlloc() {

    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));
    if (sharedInt == NULL) {
        return false;
    }

    Memory::SharedFree((void const *&) sharedInt);

    //key=0
    sharedInt = (int32*) Memory::SharedAlloc(0, sizeof(int32));
    if (sharedInt == NULL) {
        return false;
    }

    Memory::SharedFree((void const *&) sharedInt);

    //size=0
    sharedInt = (int32*) Memory::SharedAlloc(1, 0);
    if (sharedInt != NULL) {
        Memory::SharedFree((void const *&) sharedInt);

        return false;
    }

    return true;

}

bool MemoryTest::TestSharedFree() {
    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));
    if (sharedInt == NULL) {
        return false;
    }

    Memory::SharedFree((void const *&) sharedInt);

    if (sharedInt != NULL) {
        return false;
    }

    void const * p = NULL;
    Memory::SharedFree(p);

    return p == NULL;

}

void InitializeSharedMemory(MemoryTest &myTestMemory) {
    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));
    bool* sharedBool = (bool*) Memory::SharedAlloc(2, sizeof(bool));

    if (sharedBool != NULL) {
        *sharedBool = false;
    }

    if (sharedInt != NULL) {
        (*sharedInt) = 1;
    }

    myTestMemory.eventSem.Post();

}

void IncrementSharedMemory(MemoryTest &myTestMemory) {
    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));
    bool* sharedBool = (bool*) Memory::SharedAlloc(2, sizeof(bool));
    if (sharedBool != NULL) {
        *sharedBool = true;
    }
    if (sharedInt != NULL) {
        (*sharedInt)++;
    }
    myTestMemory.eventSem.Post();
}

bool MemoryTest::TestSharedMemory() {
    ErrorType err;
    //reset an event sem
    eventSem.Reset();

    //launch two threads, one initialize the shared int to one and the shared bool to false.
    Threads::BeginThread((ThreadFunctionType) InitializeSharedMemory, this);
    //wait the inizialization of the shared memory

    err = eventSem.Wait();
    eventSem.Reset();

    //this thread increment the shared integer and impose true the shared bool
    Threads::BeginThread((ThreadFunctionType) IncrementSharedMemory, this);
    err = eventSem.Wait();

    //obtain the pointers to the shared memories
    bool* sharedBool = (bool*) Memory::SharedAlloc(2, sizeof(bool));
    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));

    if (sharedInt == NULL || sharedBool == NULL) {

        return false;
    }

    int32 j = 0;

    //wait that the second thread increments the shared int
    while ((*sharedInt) < 2) {
        if (j++ > 100) {
            Memory::SharedFree((void const *&) sharedBool);
            Memory::SharedFree((void const *&) sharedInt);
            return false;
        }
        Sleep::Sec(20e-3);
    }
    bool returnValue = false;

    returnValue = *sharedBool;

    //release the shared memory

    Memory::SharedFree((void const *&) sharedBool);
    Memory::SharedFree((void const *&) sharedInt);

    //else return false
    return returnValue;
}

bool MemoryTest::TestCopy() {
    int32 myIntArray[5];
    float32 myFloatArray[5];

    for (int32 i = 0; i < 5; i++) {
        myIntArray[i] = i;
    }

    //Copy the int array in the float32 array.
    uint32 sizeToCopy = 5 * sizeof(int32);
    if (!Memory::Copy(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }

    //Check that bytes are equal independently from type.
    if (Memory::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    //size=0
    sizeToCopy = 0;
    if (!Memory::Copy(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }
    //nothing should change
    if (Memory::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    sizeToCopy = 1;
    return (!Memory::Copy(NULL, NULL, sizeToCopy) && !Memory::Copy(NULL, (const void*) myIntArray, sizeToCopy) && !Memory::Copy(myFloatArray, NULL, sizeToCopy));

}

bool MemoryTest::TestMove() {

    int32 myIntArray[5];
    float32 myFloatArray[5];

    for (int32 i = 0; i < 5; i++) {
        myIntArray[i] = i;
    }

    //Copy the int array in the float32 array.
    uint32 sizeToCopy = 5 * sizeof(int32);
    if (!Memory::Move(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }

    //Check that bytes are equal independently from type.
    if (Memory::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    //size=0
    sizeToCopy = 0;
    if (!Memory::Move(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }
    //nothing should change
    if (Memory::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    sizeToCopy = 1;
    return (!Memory::Move(NULL, NULL, sizeToCopy) && !Memory::Move(NULL, (const void*) myIntArray, sizeToCopy) && !Memory::Move(myFloatArray, NULL, sizeToCopy));

}

bool MemoryTest::TestCompare() {
    const char8 *source = "Hello World";
    const char8 *test = "Hello W0000";

    uint32 sizeToCopy = 7;

    //Source must be equal to test until 6.
    if (Memory::Compare(source, test, sizeToCopy) != 0) {
        return false;
    }

    sizeToCopy = 11;

    //Source must be greater than test.
    if (Memory::Compare(source, test, sizeToCopy) != 2) {
        return false;
    }

    //Test must be less than source.
    if (Memory::Compare(test, source, sizeToCopy) != 1) {
        return false;
    }

    //Test the result in case of NULL argument.
    if (Memory::Compare(NULL, source, sizeToCopy) != -1) {
        return false;
    }

    //Test with size=0.
    sizeToCopy = 0;
    if (Memory::Compare(test, source, sizeToCopy) != 0) {
        return false;
    }

    sizeToCopy = 1;
    return (Memory::Compare(NULL, NULL, sizeToCopy) == -1 && Memory::Compare(source, NULL, sizeToCopy) == -1 && Memory::Compare(NULL, test, sizeToCopy) == -1);

}

bool MemoryTest::TestSet() {

    uint32 size = 10;
    char8 *buffPointer = (char8*) Memory::Malloc(size);

    if (buffPointer == NULL) {
        return false;
    }

    //Set first 5 bytes to 'o'.
    char8 myFavouriteChar = 'o';
    uint32 charSize = 5;
    if (!Memory::Set(buffPointer, myFavouriteChar, size)) {
        Memory::Free((void*&) buffPointer);
        return false;
    }

    char8 *newBuffPointer = buffPointer + charSize;

    //Set last 5 bytes to 'u'.
    myFavouriteChar = 'u';

    if (!Memory::Set(newBuffPointer, myFavouriteChar, size - charSize)) {
        Memory::Free((void*&) buffPointer);
        return false;
    }

    char8 test[] = "ooooouuuuu";

    //Check that the Set result is correct.
    if (Memory::Compare(test, buffPointer, size) != 0) {
        Memory::Free((void*&) buffPointer);
        return false;
    }

    //use size=0
    size = 0;
    if (!Memory::Set(newBuffPointer, myFavouriteChar, size)) {
        Memory::Free((void*&) buffPointer);
        return false;
    }

    //nothing should change
    if (Memory::Compare(test, buffPointer, size) != 0) {
        Memory::Free((void*&) buffPointer);
        return false;
    }

    size = 1;
    return !Memory::Set(NULL, myFavouriteChar, size);

}

bool MemoryTest::TestSearch() {

    uint32 size = 10;
    const char8 *buffPointer = "Hello World";

    //Test the Search function.
    char8 myFavouriteChar = 'W';

    if (Memory::Search(buffPointer, myFavouriteChar, size) != (buffPointer + 6)) {
        return false;
    }

    //Test the result of Search when the character is not found.
    char8 imNotInBuffer = 'a';
    if (Memory::Search(buffPointer, imNotInBuffer, size) != NULL) {
        return false;
    }

    size = 0;

    if (Memory::Search(buffPointer, myFavouriteChar, size) != NULL) {
        return false;
    }

    size = 1;
    return Memory::Search(NULL, myFavouriteChar, size) == NULL;

}


void UsedHeapFunction(MemoryTest &m) {
    ErrorType err;
    uint32 size = 1;
    void* p = Memory::Malloc(size);
    err = m.eventSem.Wait();

    Memory::Free(p);

    Atomic::Increment((volatile int32*) &m.counter);

}



void AllocateFunction(MemoryTest &m) {

    uint32 myIndex = m.counter;
    //creates an array on the heap


    void* p[32];

    int32 i = 0;
    uint32 amount = myIndex;
    while (amount >= 32) {
        p[i] = Memory::Malloc(sizeof(char8) * 32);
        amount -= 32;
        i++;
    }

    if (amount > 0) {

        p[i] = Memory::Malloc(sizeof(char8) * amount);
    }
    else {
        i--;
    }

    m.signals[myIndex] = true;

    while (m.signals[myIndex]) {
        Sleep::Sec(1e-3);
    }

    while (i >= 0) {
        Memory::Free(p[i]);
        i--;
    }
    m.signals[myIndex] = true;
}


