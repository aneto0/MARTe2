/**
 * @file MemoryOperationsHelperTest.cpp
 * @brief Source file for class MemoryOperationsHelperTest
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
 * the class MemoryOperationsHelperTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MemoryOperationsHelperTest.h"
#include "MemoryOperationsHelper.h"
#include "HeapManager.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

MemoryOperationsHelperTest::MemoryOperationsHelperTest() {
}

MemoryOperationsHelperTest::~MemoryOperationsHelperTest() {
}

bool MemoryOperationsHelperTest::TestCopy() {
    int32 myIntArray[5];
    float32 myFloatArray[5];

    for (int32 i = 0; i < 5; i++) {
        myIntArray[i] = i;
        myFloatArray[i] = 0.0;
    }

    //Copy the int array in the float32 array.
    uint32 sizeToCopy = 4 * sizeof(int32);
    if (!MemoryOperationsHelper::Copy(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }

    //Check that bytes are equal independently from type.
    if (MemoryOperationsHelper::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    uint32 checkSize = sizeToCopy + sizeof(int32);

    if (MemoryOperationsHelper::Compare((const void*) myFloatArray, (const void*) myIntArray, checkSize) == 0) {
        return false;
    }

    return true;

}

bool MemoryOperationsHelperTest::TestCopyZeroSize() {
    int32 myIntArray[5] = { 1 };
    float32 myFloatArray[5] = { 2.0 };

    //size=0
    uint32 sizeToCopy = 0;
    if (!MemoryOperationsHelper::Copy(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }

    return (myIntArray[0] == 1) && (myFloatArray[0] == 2.0);

}

bool MemoryOperationsHelperTest::TestCopyNullPointer() {
    int32 myIntArray[5] = { 1 };
    float32 myFloatArray[5] = { 0.0 };
    uint32 sizeToCopy = 1;
    return (!MemoryOperationsHelper::Copy(NULL, NULL, sizeToCopy) && !MemoryOperationsHelper::Copy(NULL, (const void*) myIntArray, sizeToCopy)
            && !MemoryOperationsHelper::Copy(myFloatArray, NULL, sizeToCopy));
}

bool MemoryOperationsHelperTest::TestMove() {

    int32 myIntArray[5];
    float32 myFloatArray[5];

    for (int32 i = 0; i < 5; i++) {
        myIntArray[i] = i;
        myFloatArray[i] = 0.0;
    }

    //Copy the int array in the float32 array.
    uint32 sizeToCopy = 4 * sizeof(int32);
    if (!MemoryOperationsHelper::Move(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }

    //Check that bytes are equal independently from type.
    if (MemoryOperationsHelper::Compare((const void*) myFloatArray, (const void*) myIntArray, sizeToCopy) != 0) {
        return false;
    }

    uint32 checkSize = sizeToCopy + sizeof(int32);

    return (MemoryOperationsHelper::Compare((const void*) myFloatArray, (const void*) myIntArray, checkSize) != 0);

}

bool MemoryOperationsHelperTest::TestMoveZeroSize() {
    int32 myIntArray[5] = { 1 };
    float32 myFloatArray[5] = { 2.0 };

    //size=0
    uint32 sizeToCopy = 0;
    if (!MemoryOperationsHelper::Move(myFloatArray, (const void*) myIntArray, sizeToCopy)) {
        return false;
    }
    //nothing should change
    return (myIntArray[0] == 1) && (myFloatArray[0] == 2.0);
}

bool MemoryOperationsHelperTest::TestMoveNullPointer() {
    int32 myIntArray[5];
    float32 myFloatArray[5];

    uint32 sizeToCopy = 1;
    return (!MemoryOperationsHelper::Move(NULL, NULL, sizeToCopy) && !MemoryOperationsHelper::Move(NULL, (const void*) myIntArray, sizeToCopy)
            && !MemoryOperationsHelper::Move(myFloatArray, NULL, sizeToCopy));

}

bool MemoryOperationsHelperTest::TestCompare() {
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
    return (MemoryOperationsHelper::Compare(NULL, NULL, sizeToCopy) == -1 && MemoryOperationsHelper::Compare(source, NULL, sizeToCopy) == -1
            && MemoryOperationsHelper::Compare(NULL, test, sizeToCopy) == -1);

}

bool MemoryOperationsHelperTest::TestSet() {

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
    HeapManager::Free((void*&) buffPointer);

    size = 1;
    return !MemoryOperationsHelper::Set(NULL, myFavouriteChar, size);

}

bool MemoryOperationsHelperTest::TestSetZeroSize() {
    char buffPointer[32] = { 'a' };

    char myFavouriteChar = 'b';

    //use size=0
    uint32 size = 0;
    if (!MemoryOperationsHelper::Set(buffPointer, myFavouriteChar, size)) {
        return false;
    }

    return buffPointer[0] == 'a';
}

bool MemoryOperationsHelperTest::TestSearch() {

    uint32 size = 10;
    const char8 *buffPointer = "Hello World";

    //Test the Search function.
    char8 myFavouriteChar = 'W';

    if (MemoryOperationsHelper::Search(buffPointer, myFavouriteChar, size) != (buffPointer + 6)) {
        return false;
    }

    size = 1;
    return MemoryOperationsHelper::Search(NULL, myFavouriteChar, size) == NULL;

}

bool MemoryOperationsHelperTest::TestSearchNotInBuffer() {
    uint32 size = 10;
    const char8 *buffPointer = "Hello World";

    //Test the result of Search when the character is not found.
    char8 imNotInBuffer = 'a';
    return (MemoryOperationsHelper::Search(buffPointer, imNotInBuffer, size) == NULL);
}

bool MemoryOperationsHelperTest::TestSearchOutOfRanges() {
    uint32 size = 10;
    const char8 *buffPointer = "Hello World";

    //Test the result of Search when the character is over the size passed by argument.
    char8 outOfRanges = 'd';

    return (MemoryOperationsHelper::Search(buffPointer, outOfRanges, size) == NULL);
}

bool MemoryOperationsHelperTest::TestSearchZeroSize() {
    uint32 size = 0;

    const char8 *buffPointer = "Hello World";

    char8 myFavouriteChar = 'W';

    return (MemoryOperationsHelper::Search(buffPointer, myFavouriteChar, size) == NULL);

}

bool MemoryOperationsHelperTest::TestInterleavedToFlat() {

    const uint32 memSize = 150;
    const uint32 nSamples = 10;

    uint8 mem[memSize];

    uint8 dest[memSize];

    uint8 *ptr = &mem[0];

    uint32 cnt = 0u;
    for (uint32 i = 0u; i < nSamples; i++) {
        *ptr = cnt;
        ptr++;
        cnt++;
        *(uint16*) ptr = cnt;
        ptr += 2;
        cnt++;
        *(uint32*) ptr = cnt;
        ptr += 4;
        cnt++;
        *(uint64*) ptr = cnt;
        ptr += 8;
        cnt++;
    }

    uint32 packetInputChunkSize[] = { 1, 2, 4, 8 };
    uint32 inputByteSize = 15;
    uint32 numberOfInputPacketChunks = 4;
    uint32 numberOfInputSamples = nSamples;

    MemoryOperationsHelper::InterleavedToFlat(&mem[0], &dest[0], 0, &packetInputChunkSize[0], inputByteSize, numberOfInputPacketChunks, numberOfInputSamples);

    bool ret = true;

    ptr = &dest[0];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret = (ptr[i] == 4 * i);
    }

    uint16 *ptr16 = (uint16*) &dest[nSamples];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret = (ptr16[i] == 4 * i + 1);
    }

    uint32 *ptr32 = (uint32*) &dest[3 * nSamples];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret = (ptr32[i] == 4 * i + 2);
    }

    uint64 *ptr64 = (uint64*) &dest[7 * nSamples];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret = (ptr64[i] == 4 * i + 3);
    }

    return ret;

}

bool MemoryOperationsHelperTest::TestFlatToInterleaved() {
    /*    void
     */

    const uint32 memSize = 150;
    const uint32 nSamples = 10;

    uint8 mem[memSize];

    uint8 dest[memSize];

    uint8 *ptr = &mem[0];
    uint32 cnt = 0u;

    for (uint32 i = 0u; (i < nSamples); i++) {
        ptr[i] = cnt;
        cnt++;
    }

    uint16 *ptr16 = (uint16*) &mem[nSamples];
    for (uint32 i = 0u; (i < nSamples); i++) {
        ptr16[i] = cnt;
        cnt++;
    }

    uint32 *ptr32 = (uint32*) &mem[3 * nSamples];
    for (uint32 i = 0u; (i < nSamples); i++) {
        ptr32[i] = cnt;
        cnt++;
    }

    uint64 *ptr64 = (uint64*) &mem[7 * nSamples];
    for (uint32 i = 0u; (i < nSamples); i++) {
        ptr64[i] = cnt;
        cnt++;
    }

    uint32 packetOutputChunkSize[] = { 1, 2, 4, 8 };
    uint32 outputByteSize = 15;
    uint32 numberOfOutputPacketChunks = 4;
    uint32 numberOfOutputSamples = nSamples;

    MemoryOperationsHelper::FlatToInterleaved(&mem[0], &dest[0], 0, &packetOutputChunkSize[0], outputByteSize, numberOfOutputPacketChunks, numberOfOutputSamples);

    bool ret = true;

    ptr = &dest[0];
    for (uint32 i = 0u; (i < nSamples) && (ret); i++) {
        ret&=(*ptr==(i));
        ptr++;
        ret&=(*ptr==(10+i));
        ptr += 2;
        ret&=(*ptr==(20+i));
        ptr += 4;
        ret&=(*ptr==(30+i));
        ptr += 8;
    }
    return ret;

}
