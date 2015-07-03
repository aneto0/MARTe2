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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/** @brief Constructor. */
MemoryTest::MemoryTest() {

}

MemoryTest::~MemoryTest() {
}

//Test the malloc function.
bool MemoryTest::TestMallocAndFree(int32 size) {

    //allocate a space of size integers
    int32* allocated = (int32*) Memory::Malloc(size * sizeof(int32));
    int32 i = 0;

    //check if the pointers to these memory locations are valid
    while (i < size) {
        if ((allocated + i) == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }
        i++;
    }

    //free the allocated memory
    Memory::Free((void*&) allocated);

    return allocated == NULL;
}

//Tests the realloc function.
bool MemoryTest::TestRealloc(int32 size1,
                             int32 size2) {
    //allocate size1 integers
    int32* allocated = (int32*) Memory::Malloc(size1 * sizeof(int32));

    //check if the pointers to these memory locations are valid
    for (int32 i = 0; i < size1; i++) {
        if ((allocated + i) == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }
        allocated[i] = i;
    }

    //reallocate the memory adding size2 integers locations
    allocated = (int32*) Memory::Realloc((void*&) allocated, (size1 + size2) * sizeof(int32));

    //check if pointers of new memory are valid and if the old memory is not corrupted
    for (int32 i = 0; i < size2; i++) {
        if ((allocated + size1 + i) == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }
        if (allocated[i] != i) {
            Memory::Free((void*&) allocated);
            return false;
        }
    }

    Memory::Free((void*&) allocated);
    //check if it implemens a malloc in case of null pointer in input.
    allocated = NULL;

    allocated = (int32*) MemoryRealloc((void*&) allocated, size1 * sizeof(int32));

    //checks if the memory is allocated correctly
    MemoryTestAccessMode mtam(true,true,true);
    if (!Memory::Check(allocated, mtam, size1 * sizeof(int32))) {
        return false;
    }

    //manual check
    for (int32 i = 0; i < size1; i++) {
        if ((allocated + i) == NULL) {
            Memory::Free((void*&) allocated);
            return false;
        }
    }
    uint32 size = 0;
    //check if implements a free if size is 0.
    allocated = (int32*) MemoryRealloc((void*&) allocated, size);
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

    //the check function on a null pointer should return false
    return !Memory::Check(NULL, mtam, size);
}

//Test if the string s is copied without errors.
bool MemoryTest::TestMemoryStringDup(const char8* s) {
    return StringTestHelper::Compare((char8*) MemoryStringDup(s), (char8*) s);
}

//Test the behavior of the shared memory
bool MemoryTest::TestSharedMemory() {

    int32* sharedInt = (int32*) Memory::SharedAlloc(1, sizeof(int32));
    if (sharedInt == NULL) {
        return false;
    }

    Memory::SharedFree(sharedInt);

    return true;

}

bool MemoryTest::TestCopyAndMove() {
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

    //Check that bytes are equal indipendently from type.
    if (Memory::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    const char8* source = "Hello World";
    const char8* test = "Hello W0000";
    char8* buffer[32];

    sizeToCopy = 11;

    //Another way to copy memory.
    if (!Memory::Move(buffer, source, sizeToCopy)) {
        return false;
    }

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

    return true;

}

bool MemoryTest::TestSetAndSearch() {

    uint32 size = 10;
    char8* buffPointer = (char8*) Memory::Malloc(size);

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

    char8* newBuffPointer = buffPointer + charSize;

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

    //Test the Search function.
    if (Memory::Search(buffPointer, myFavouriteChar, size) != newBuffPointer) {
        Memory::Free((void*&) buffPointer);
        return false;
    }

    //Test the result of Search when the character is not found.
    char8 imNotInBuffer = 'a';
    if (Memory::Search(buffPointer, imNotInBuffer, size) != NULL) {
        Memory::Free((void*&) buffPointer);
        return false;
    }

    Memory::Free((void*&) buffPointer);

    return true;

}

bool MemoryTest::TestHeader() {

    bool ret = true;
    uint32 size;
    TID tid;
    //creates an array of 10 integers
    uint32 *arrayInt = (uint32*) Memory::Malloc(sizeof(uint32) * 10);
#ifdef MEMORY_STATISTICS

    //sets the ninth element
    arrayInt[9] = 3;

    //gets the header informations
    if (!Memory::GetHeaderInfo(arrayInt, size, tid)) {
        ret= false;
    }

    //the size should be greater or equal than the memory size + header fields
    if (size < (sizeof(uint32) * 10 + sizeof(uint32) + sizeof(TID))) {
        ret= false;
    }

#else

    //now the function should return false in this case
    if (Memory::GetHeaderInfo(arrayInt, size, tid)) {
        ret = false;
    }

#endif

    Memory::Free((void*&) arrayInt);

    return ret;
}

bool MemoryTest::TestDatabase() {

    int32 chunks = 0;
    int32 size = 0;

#ifdef MEMORY_STATISTICS


    Memory::ClearStatisticsDatabase();

    if(Memory::GetStatisticsDatabaseNElements()!=0) {
        return false;
    }

    if(Memory::GetUsedHeap()!=0) {
        return false;
    }

    void* data1=Memory::Malloc(sizeof(uint32));

    void* data2=Memory::Malloc(sizeof(uint32)*10);

    bool ret=true;

    uint32 minHeaderSize=sizeof(TID)+ sizeof(uint32);

    if (!(Memory::AllocationStatistics(size, chunks))) {
        ret=false;
    }

    if(size<(int32)(sizeof(uint32)*11+2*minHeaderSize) || chunks != 2) {
        ret= false;
    }

    if(Memory::GetUsedHeap()!=size) {
        return false;
    }

    Memory::Free(data2);

    if (!(Memory::AllocationStatistics(size, chunks))) {
        ret=false;
    }

    if(size<(int32)(sizeof(uint32)+minHeaderSize) || chunks != 1) {
        ret = false;
    }

    Memory::Free(data1);

    ret= ret && !Memory::AllocationStatistics(size, chunks);

    return ret;

#else

    return !Memory::AllocationStatistics(size, chunks);

#endif

}

