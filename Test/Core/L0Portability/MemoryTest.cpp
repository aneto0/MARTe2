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


bool MemoryTest::TestMalloc(uint32 size) {

    //allocate a space of size integers
    int32* allocated = (int32*) Memory::Malloc(size * sizeof(int32));

    //check if the pointers to these memory locations are valid
    if ((allocated) == NULL) {
        Memory::Free((void*&) allocated);
    }

    //free the allocated memory
    Memory::Free((void*&) allocated);

    //allocates nothing
    return Memory::Malloc(0) == NULL;

}

bool MemoryTest::TestFree(uint32 size) {

    void *p = NULL;
    //frees a null pointer
    Memory::Free(p);

    //allocate a space of size integers
    int32* allocated = (int32*) Memory::Malloc(size * sizeof(int32));

    //free the allocated memory
    Memory::Free((void*&) allocated);

    //allocated should be null
    return allocated == NULL;
}

bool MemoryTest::TestRealloc(uint32 size1,
                             uint32 size2) {
    //allocate size1 integers
    int32* allocated = (int32*) Memory::Malloc(size1 * sizeof(int32));

    //check if the pointers to these memory locations are valid
    if (allocated == NULL) {
        Memory::Free((void*&) allocated);
        return false;
    }

    for (uint32 i = 0; i < size1; i++) {
        allocated[i] = i;
    }

    //reallocate the memory adding size2 integers locations
    allocated = (int32*) Memory::Realloc((void*&) allocated, (size1 + size2) * sizeof(int32));

    //check if pointers of new memory are valid and if the old memory is not corrupted
    if (allocated == NULL) {
        Memory::Free((void*&) allocated);
        return false;
    }

    for (uint32 i = 0; i < size1; i++) {

        if (allocated[i] != (int32) i) {
            Memory::Free((void*&) allocated);
            return false;
        }
    }

    Memory::Free((void*&) allocated);
    //check if it implements a malloc in case of null pointer in input.
    allocated = NULL;

    allocated = (int32*) Memory::Realloc((void*&) allocated, size1 * sizeof(int32));

    //manual check
    if (allocated == NULL) {
        Memory::Free((void*&) allocated);
        return false;
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

    char* p = (char8*) Memory::StringDup(s);

    if (!StringTestHelper::Compare(p, s)) {
        return false;
    }

    Memory::Free((void*&) p);

    //should return null in case of null input.
    p = Memory::StringDup(NULL);

    return p == NULL;
}

bool MemoryTest::TestCheck(uint32 size) {

    //allocate a space of size integers
    int32* allocated = (int32*) Memory::Malloc(size * sizeof(int32));

    //checks if all the memory is allocated correctly
    if (!Memory::Check(allocated, (Memory::MemoryTestAccessMode) (Memory::Read | Memory::Write | Memory::Execute), size * sizeof(int32))) {
        return false;
    }

    //checks if a part the memory is allocated correctly
    if (!Memory::Check(allocated, (Memory::MemoryTestAccessMode) (Memory::Read | Memory::Write | Memory::Execute), (size / 2) * sizeof(int32))) {
        return false;
    }

    //0 as size
    uint32 testSize = 0;
    if (!Memory::Check(allocated, (Memory::MemoryTestAccessMode) (Memory::Read | Memory::Write | Memory::Execute), testSize)) {
        return false;
    }

    Memory::Free((void*&) allocated);

    //the check function on a null pointer should return false
    return !Memory::Check(NULL, (Memory::MemoryTestAccessMode) (Memory::Read | Memory::Write | Memory::Execute), size);

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

    //Test the result of Search when the character is over the size passed by argument.
    char8 outOfRanges = 'd';

    if (Memory::Search(buffPointer, outOfRanges, size) != NULL) {
        return false;
    }

    size = 0;

    if (Memory::Search(buffPointer, myFavouriteChar, size) != NULL) {
        return false;
    }

    size = 1;
    return Memory::Search(NULL, myFavouriteChar, size) == NULL;

}
