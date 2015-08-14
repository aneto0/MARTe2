/**
 * @file HeapManagerTest.cpp
 * @brief Source file for class HeapManagerTest
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
 * the class HeapManagerTest (public, protected, and private). Be aware that some
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
    int32* allocated = (int32*) HeapManager::Malloc(size * sizeof(int32));

    //check if the pointers to these HeapManager locations are valid
    if ((allocated) == NULL) {
        HeapManager::Free((void*&) allocated);
    }

    //free the allocated HeapManager
    HeapManager::Free((void*&) allocated);

    //allocates nothing
    return HeapManager::Malloc(0) == NULL;

}

bool MemoryTest::TestFree(uint32 size) {

    void *p = NULL;
    //frees a null pointer
    HeapManager::Free(p);

    //allocate a space of size integers
    int32* allocated = (int32*) HeapManager::Malloc(size * sizeof(int32));

    //free the allocated HeapManager
    HeapManager::Free((void*&) allocated);

    //allocated should be null
    return allocated == NULL;
}

bool MemoryTest::TestRealloc(uint32 size1,
                             uint32 size2) {
    //allocate size1 integers
    int32* allocated = (int32*) HeapManager::Malloc(size1 * sizeof(int32));

    //check if the pointers to these HeapManager locations are valid
    if (allocated == NULL) {
        HeapManager::Free((void*&) allocated);
        return false;
    }

    for (uint32 i = 0; i < size1; i++) {
        allocated[i] = i;
    }

    //reallocate the HeapManager adding size2 integers locations
    allocated = (int32*) HeapManager::Realloc((void*&) allocated, (size1 + size2) * sizeof(int32));

    //check if pointers of new HeapManager are valid and if the old HeapManager is not corrupted
    if (allocated == NULL) {
        HeapManager::Free((void*&) allocated);
        return false;
    }

    for (uint32 i = 0; i < size1; i++) {

        if (allocated[i] != (int32) i) {
            HeapManager::Free((void*&) allocated);
            return false;
        }
    }

    HeapManager::Free((void*&) allocated);
    //check if it implements a malloc in case of null pointer in input.
    allocated = NULL;

    allocated = (int32*) HeapManager::Realloc((void*&) allocated, size1 * sizeof(int32));

    //manual check
    if (allocated == NULL) {
        HeapManager::Free((void*&) allocated);
        return false;
    }

    uint32 size = 0;
    //check if implements a free if size is 0.
    allocated = (int32*) HeapManager::Realloc((void*&) allocated, size);

    if (allocated != NULL) {
        HeapManager::Free((void*&) allocated);
        return false;
    }

    //check if it returns NULL in case of NULL input and size equal to zero.
    allocated = (int32*) HeapManager::Realloc((void*&) allocated, size);

    if (allocated != NULL) {
        HeapManager::Free((void*&) allocated);
        return false;
    }

    return true;

}

bool MemoryTest::TestStringDup(const char8 *s) {

    char* p = (char8*) StringHelper::StringDup(s);

    if (!StringTestHelper::Compare(p, s)) {
        return false;
    }

    HeapManager::Free((void*&) p);

    //should return null in case of null input.
    p = StringHelper::StringDup(NULL);

    return p == NULL;
}

bool MemoryTest::TestCheck(uint32 size) {

    //allocate a space of size integers
    int32* allocated = (int32*) HeapManager::Malloc(size * sizeof(int32));

    //checks if all the HeapManager is allocated correctly
    if (!MemoryCheck::Check(allocated, (MemoryCheck::MemoryTestAccessMode) (MemoryCheck::Read | MemoryCheck::Write | MemoryCheck::Execute), size * sizeof(int32))) {
        return false;
    }

    //checks if a part the HeapManager is allocated correctly
    if (!MemoryCheck::Check(allocated, (MemoryCheck::MemoryTestAccessMode) (MemoryCheck::Read | MemoryCheck::Write | MemoryCheck::Execute), (size / 2) * sizeof(int32))) {
        return false;
    }

    //0 as size
    uint32 testSize = 0;
    if (!MemoryCheck::Check(allocated, (MemoryCheck::MemoryTestAccessMode) (MemoryCheck::Read | MemoryCheck::Write | MemoryCheck::Execute), testSize)) {
        return false;
    }

    HeapManager::Free((void*&) allocated);

    //the check function on a null pointer should return false
    return !MemoryCheck::Check(NULL, (MemoryCheck::MemoryTestAccessMode) (MemoryCheck::Read | MemoryCheck::Write | MemoryCheck::Execute), size);

}

bool MemoryTest::TestCopy() {
    int32 myIntArray[5];
    float32 myFloatArray[5];

    for (int32 i = 0; i < 5; i++) {
        myIntArray[i] = i;
    }

    //Copy the int array in the float32 array.
    uint32 sizeToCopy = 5 * sizeof(int32);
    if (!MemoryOperationsHelper::Copy(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }

    //Check that bytes are equal independently from type.
    if (MemoryOperationsHelper::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    //size=0
    sizeToCopy = 0;
    if (!MemoryOperationsHelper::Copy(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }
    //nothing should change
    if (MemoryOperationsHelper::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    sizeToCopy = 1;
    return (!MemoryOperationsHelper::Copy(NULL, NULL, sizeToCopy) && !MemoryOperationsHelper::Copy(NULL, (const void*) myIntArray, sizeToCopy) && !MemoryOperationsHelper::Copy(myFloatArray, NULL, sizeToCopy));

}

bool MemoryTest::TestMove() {

    int32 myIntArray[5];
    float32 myFloatArray[5];

    for (int32 i = 0; i < 5; i++) {
        myIntArray[i] = i;
    }

    //Copy the int array in the float32 array.
    uint32 sizeToCopy = 5 * sizeof(int32);
    if (!MemoryOperationsHelper::Move(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }

    //Check that bytes are equal independently from type.
    if (MemoryOperationsHelper::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    //size=0
    sizeToCopy = 0;
    if (!MemoryOperationsHelper::Move(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }
    //nothing should change
    if (MemoryOperationsHelper::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    sizeToCopy = 1;
    return (!MemoryOperationsHelper::Move(NULL, NULL, sizeToCopy) && !MemoryOperationsHelper::Move(NULL, (const void*) myIntArray, sizeToCopy) && !MemoryOperationsHelper::Move(myFloatArray, NULL, sizeToCopy));

}

bool MemoryTest::TestCompare() {
    const char8 *source = "Hello World";
    const char8 *test = "Hello W0000";

    uint32 sizeToCopy = 7;

    //Source must be equal to test until 6.
    if (MemoryOperationsHelper::Compare(source, test, sizeToCopy) != 0) {
        return false;
    }

    sizeToCopy = 11;

    //Source must be greater than test.
    if (MemoryOperationsHelper::Compare(source, test, sizeToCopy) != 2) {
        return false;
    }

    //Test must be less than source.
    if (MemoryOperationsHelper::Compare(test, source, sizeToCopy) != 1) {
        return false;
    }

    //Test the result in case of NULL argument.
    if (MemoryOperationsHelper::Compare(NULL, source, sizeToCopy) != -1) {
        return false;
    }

    //Test with size=0.
    sizeToCopy = 0;
    if (MemoryOperationsHelper::Compare(test, source, sizeToCopy) != 0) {
        return false;
    }

    sizeToCopy = 1;
    return (MemoryOperationsHelper::Compare(NULL, NULL, sizeToCopy) == -1 && MemoryOperationsHelper::Compare(source, NULL, sizeToCopy) == -1 && MemoryOperationsHelper::Compare(NULL, test, sizeToCopy) == -1);

}

bool MemoryTest::TestSet() {

    uint32 size = 10;
    char8 *buffPointer = (char8*) HeapManager::Malloc(size);

    if (buffPointer == NULL) {
        return false;
    }

    //Set first 5 bytes to 'o'.
    char8 myFavouriteChar = 'o';
    uint32 charSize = 5;
    if (!MemoryOperationsHelper::Set(buffPointer, myFavouriteChar, size)) {
        HeapManager::Free((void*&) buffPointer);
        return false;
    }

    char8 *newBuffPointer = buffPointer + charSize;

    //Set last 5 bytes to 'u'.
    myFavouriteChar = 'u';

    if (!MemoryOperationsHelper::Set(newBuffPointer, myFavouriteChar, size - charSize)) {
        HeapManager::Free((void*&) buffPointer);
        return false;
    }

    char8 test[] = "ooooouuuuu";

    //Check that the Set result is correct.
    if (MemoryOperationsHelper::Compare(test, buffPointer, size) != 0) {
        HeapManager::Free((void*&) buffPointer);
        return false;
    }

    //use size=0
    size = 0;
    if (!MemoryOperationsHelper::Set(newBuffPointer, myFavouriteChar, size)) {
        HeapManager::Free((void*&) buffPointer);
        return false;
    }

    //nothing should change
    if (MemoryOperationsHelper::Compare(test, buffPointer, size) != 0) {
        HeapManager::Free((void*&) buffPointer);
        return false;
    }

    size = 1;
    return !MemoryOperationsHelper::Set(NULL, myFavouriteChar, size);

}

bool MemoryTest::TestSearch() {

    uint32 size = 10;
    const char8 *buffPointer = "Hello World";

    //Test the Search function.
    char8 myFavouriteChar = 'W';

    if (MemoryOperationsHelper::Search(buffPointer, myFavouriteChar, size) != (buffPointer + 6)) {
        return false;
    }

    //Test the result of Search when the character is not found.
    char8 imNotInBuffer = 'a';
    if (MemoryOperationsHelper::Search(buffPointer, imNotInBuffer, size) != NULL) {
        return false;
    }

    //Test the result of Search when the character is over the size passed by argument.
    char8 outOfRanges = 'd';

    if (MemoryOperationsHelper::Search(buffPointer, outOfRanges, size) != NULL) {
        return false;
    }

    size = 0;

    if (MemoryOperationsHelper::Search(buffPointer, myFavouriteChar, size) != NULL) {
        return false;
    }

    size = 1;
    return MemoryOperationsHelper::Search(NULL, myFavouriteChar, size) == NULL;

}
