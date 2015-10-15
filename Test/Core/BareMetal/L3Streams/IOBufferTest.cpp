/**
 * @file IOBufferTest.cpp
 * @brief Source file for class IOBufferTest
 * @date 09/10/2015
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
 * the class IOBufferTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "IOBufferTest.h"
#include "StringHelper.h"
#include "StreamTestHelper.h"
#include "printf.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool IOBufferTest::TestDefaultConstructor() {
    IOBuffer ioBuffer;
    if (ioBuffer.AmountLeft() != 0) {
        return false;
    }

    if (ioBuffer.UsedAmountLeft() != 0) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != 0) {
        return false;
    }

    if (ioBuffer.Buffer() != NULL) {
        return false;
    }

    return ioBuffer.UndoLevel() == 0;
}

bool IOBufferTest::TestFullConstructor(uint32 undoLevel) {
    IOBuffer ioBuffer(33, undoLevel);

    if (ioBuffer.AmountLeft() != 0) {
        return false;
    }

    if (ioBuffer.UsedAmountLeft() != 0) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != 0) {
        return false;
    }

    if (ioBuffer.Buffer() != NULL) {
        return false;
    }

    return ioBuffer.UndoLevel() == undoLevel;
}

bool IOBufferTest::TestDestructor() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    char8 buffer[size];

    ioBuffer.SetBufferReferencedMemory(buffer, size, 0);
    if (ioBuffer.Buffer() == NULL) {
        return false;
    }

    ioBuffer.~IOBuffer();
    return ioBuffer.Buffer() == NULL;
}

bool IOBufferTest::TestBufferSize(uint32 size,
                                  uint32 granularity) {
    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.BufferSize() != 0) {
        return false;
    }
    granularity = ioBuffer.AllocationGranularity();

    uint32 addChunk = ((size % granularity) != 0);

    ioBuffer.SetBufferHeapMemory(size, 0u);

    return (ioBuffer.BufferSize() == ((size / granularity) + addChunk) * granularity);
}

bool IOBufferTest::TestMaxUsableAmount(uint32 size,
                                       uint32 endSpace,
                                       uint32 granularity) {

    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.BufferSize() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.BufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    return ioBuffer.MaxUsableAmount() == expected;
}

bool IOBufferTest::TestAmountLeft(uint32 size,
                                  uint32 endSpace,
                                  uint32 granularity) {
    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.AmountLeft() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.BufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    if (ioBuffer.AmountLeft() != expected) {
        return false;
    }

    for (uint32 i = 0; i < expected; i++) {
        ioBuffer.PutC('a');
        expected--;

        if (ioBuffer.AmountLeft() != expected) {
            return false;
        }
    }

    return true;

}

bool IOBufferTest::TestUsedAmountLeft(uint32 size,
                                      uint32 endSpace,
                                      uint32 granularity) {

    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.UsedAmountLeft() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.BufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    if (ioBuffer.UsedAmountLeft() != 0) {
        return false;
    }

    //the cursor now is always at the end of the filled memory
    for (uint32 i = 0; i < expected; i++) {
        ioBuffer.PutC('a');

        if (ioBuffer.UsedAmountLeft() != 0) {
            return false;
        }
    }

    //return at the beginning, now the buffer is full
    ioBuffer.Seek(0);

    for (uint32 i = 0; i < expected; i++) {
        ioBuffer.PutC('a');
        expected--;

        if (ioBuffer.UsedAmountLeft() != expected) {
            return false;
        }
    }

    return true;
}

bool IOBufferTest::TestBuffer() {
    IOBuffer ioBuffer;

    if (ioBuffer.Buffer() != NULL) {
        return false;
    }

    char8 bufferIn[32];
    StringHelper::Copy(bufferIn, "HelloWorld");
    ioBuffer.SetBufferReferencedMemory(bufferIn, 1u, 0u);

    return (StringHelper::Compare(bufferIn, ioBuffer.Buffer()) == 0);
}

bool IOBufferTest::TestBufferReference() {
    IOBuffer ioBuffer;

    if (ioBuffer.Buffer() != NULL) {
        return false;
    }

    char8 bufferIn[32];
    StringHelper::Copy(bufferIn, "HelloWorld");
    ioBuffer.SetBufferReferencedMemory(bufferIn, 1u, 0u);

    //write access
    char8 *ret = ioBuffer.BufferReference();
    ret[10] = '!';
    ret[11] = '\0';
    return (StringHelper::Compare(ret, "HelloWorld!") == 0);

}

bool IOBufferTest::TestPosition() {
    IOBuffer ioBuffer;

    if (ioBuffer.Position() != 0) {
        return false;
    }

    const uint32 size = 32;
    ioBuffer.SetBufferHeapMemory(size, 0u);

    //the putC change the position
    for (uint32 i = 0; i < size; i++) {
        ioBuffer.PutC('a');

        if (ioBuffer.Position() != (i + 1)) {
            printf("\n1\n");
            return false;
        }
    }

    ioBuffer.Seek(0);

    //the getC change the position
    for (uint32 i = 0; i < size; i++) {
        char c;
        ioBuffer.GetC(c);

        if (ioBuffer.Position() != (i + 1)) {
            printf("\n2\n");
            return false;
        }
    }

    //check if the position is reset after a reallocation
    char8 buffer[size];
    ioBuffer.SetBufferReferencedMemory(buffer, size, 0);
    //the write change the position

    const char8 *toWrite = "HelloWorld";
    uint32 expectedPosition = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, expectedPosition);
    if (ioBuffer.Position() != expectedPosition) {
        printf("\n3\n");
        return false;
    }

    ioBuffer.Seek(0);

    //the read change the position

    char8 toRead[32];

    ioBuffer.Read(toRead, expectedPosition);
    if (ioBuffer.Position() != expectedPosition) {
        printf("\n4\n");
        return false;
    }

    //the seek change the position
    ioBuffer.Seek(3);
    if (ioBuffer.Position() != 3) {
        printf("\n5\n");
        return false;
    }

    //the relative seek change the position
    ioBuffer.RelativeSeek(2);
    if (ioBuffer.Position() != 5) {
        printf("\n6 %d\n", ioBuffer.Position());
        return false;
    }

    ioBuffer.Empty();
    return ioBuffer.Position() == 0;

}

bool IOBufferTest::TestUsedSize(uint32 size,
                                uint32 endSpace,
                                uint32 granularity) {
    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.UsedSize() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.BufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    if (ioBuffer.UsedSize() != 0) {
        return false;
    }

    for (uint32 i = 0; i < expected; i++) {
        ioBuffer.PutC('a');

        if (ioBuffer.UsedSize() != (i + 1)) {
            return false;
        }
    }

    return true;
}

bool IOBufferTest::TestAllocationGranularity(uint32 granularity,
                                             uint32 expected) {

    IOBuffer ioBuffer(granularity, 0u);

    return ioBuffer.AllocationGranularity() == expected;

}

bool IOBufferTest::TestUndoLevel(uint32 undoLevel) {
    IOBuffer ioBuffer(33, undoLevel);

    return ioBuffer.UndoLevel() == undoLevel;

}

bool IOBufferTest::TestCanWrite() {
    IOBuffer ioBuffer;

    const uint32 allocationSize = 32;

    char8 bufferIn[allocationSize];

    ioBuffer.SetBufferReferencedMemory(bufferIn, allocationSize, 0);
    if (!ioBuffer.CanWrite()) {
        return false;
    }

    const char8 *bufferInConst = "";

    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, allocationSize, 0);
    if (ioBuffer.CanWrite()) {
        return false;
    }

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    return ioBuffer.CanWrite();
}

bool IOBufferTest::TestSetBufferHeapMemory(uint32 size,
                                           uint32 endSpace,
                                           uint32 granularity) {

    const char8* initialMemory = "Untouchable";

    IOBuffer ioBuffer(granularity, 0u);
    ioBuffer.SetBufferReadOnlyReferencedMemory(initialMemory, 10, 0);
    ioBuffer.SetUsedSize(10);
    ioBuffer.Seek(3);

    if ((!ioBuffer.SetBufferHeapMemory(size, endSpace)) && (size != 0)) {
        return false;
    }

    size = ioBuffer.BufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

    printf("\nexpected=%d\n", expected);

    if (ioBuffer.MaxUsableAmount() != expected) {
        return false;
    }

    return (StringHelper::Compare(initialMemory, "Untouchable") == 0) && (ioBuffer.Position() == 0);
}

bool IOBufferTest::TestSetBufferHeapMemoryIncrease() {

    IOBuffer ioBuffer;

    uint32 size = 32;
    uint32 endSpace = 0;
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n1\n");
        return false;
    }

    char8 buffer[16];
    StringHelper::Copy(buffer, "Hello");
    uint32 fillSize = 16;

    ioBuffer.Write(buffer, fillSize);

    if (ioBuffer.UsedSize() != fillSize) {
        printf("\n1\n");
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        printf("\n2\n");
        return false;
    }

    //increase the size
    uint32 increase = 32;
    size += increase;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != fillSize) {
        printf("\n3\n");
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        printf("\n4\n");
        return false;
    }

    //increase also the reserved space to clip the used size
    endSpace += (increase + fillSize + 2);

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    uint32 expectedUsedSize = fillSize - 2;
    if (ioBuffer.UsedSize() != expectedUsedSize) {
        printf("\n5 %d\n", ioBuffer.UsedSize());
        return false;
    }
    if (ioBuffer.Position() != expectedUsedSize) {
        printf("\n6\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n1\n");
        return false;
    }

    if (ioBuffer.BufferSize() != size) {
        printf("\n1\n");
        return false;
    }

    return (StringHelper::Compare(ioBuffer.Buffer(), "Hello") == 0);
}

bool IOBufferTest::TestSetBufferHeapMemoryDecrease() {

    IOBuffer ioBuffer;

    uint32 size = 32;
    uint32 endSpace = 0;
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n1\n");
        return false;
    }

    char8 buffer[16];
    StringHelper::Copy(buffer, "Hello");
    uint32 fillSize = 16;

    ioBuffer.Write(buffer, fillSize);

    if (ioBuffer.UsedSize() != fillSize) {
        printf("\n1\n");
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        printf("\n2\n");
        return false;
    }

    //increase the size
    uint32 clip = 2;
    size = fillSize - clip;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != (size - endSpace)) {
        printf("\n3\n");
        return false;
    }
    if (ioBuffer.Position() != (size - endSpace)) {
        printf("\n4\n");
        return false;
    }

    //clip again the used size using the space at end
    clip = 1;
    endSpace += clip;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != (size - endSpace)) {
        printf("\n5 %d\n", ioBuffer.UsedSize());
        return false;
    }
    if (ioBuffer.Position() != (size - endSpace)) {
        printf("\n6\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n1\n");
        return false;
    }

    if (ioBuffer.BufferSize() != size) {
        printf("\n1\n");
        return false;
    }

    return (StringHelper::Compare(ioBuffer.Buffer(), "Hello") == 0);
}

bool IOBufferTest::TestSetBufferReadOnlyReferencedMemory() {

    const char8* bufferIn = "HelloWorld";
    uint32 size = StringHelper::Length(bufferIn);
    uint32 endSpace = 0;
    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(size, endSpace);
    ioBuffer.PutC('a');

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.BufferSize() != size) {
        printf("\n1\n");
        return false;
    }

    if (ioBuffer.Position() != 0) {
        printf("\n2\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n3\n");
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        printf("\n4\n");
        return false;
    }

    ioBuffer.SetUsedSize(1);
    //clip by endSpace
    endSpace += 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.BufferSize() != size) {
        printf("\n5\n");
        return false;
    }

    if (ioBuffer.Position() != 0) {
        printf("\n6\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n7\n");
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        printf("\n8\n");
        return false;
    }

    ioBuffer.SetUsedSize(1);
    //clip by size
    size -= 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.BufferSize() != size) {
        return false;
    }

    if (ioBuffer.Position() != 0) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestSetBufferReferencedMemory() {

    char8 bufferIn[32];
    StringHelper::Copy(bufferIn, "HelloWorld");
    uint32 size = StringHelper::Length(bufferIn);
    uint32 endSpace = 0;
    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(size, endSpace);
    ioBuffer.PutC('a');

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.BufferSize() != size) {
        printf("\n1\n");
        return false;
    }

    if (ioBuffer.Position() != 0) {
        printf("\n2\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n3\n");
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        printf("\n4\n");
        return false;
    }
    ioBuffer.PutC('a');
    //clip by endSpace
    endSpace += 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.BufferSize() != size) {
        printf("\n5\n");
        return false;
    }

    if (ioBuffer.Position() != 0) {
        printf("\n6\n");
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        printf("\n7\n");
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        printf("\n8\n");
        return false;
    }

    ioBuffer.PutC('a');
    //clip by size
    size -= 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.BufferSize() != size) {
        return false;
    }

    if (ioBuffer.Position() != 0) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        return false;
    }

    if (ioBuffer.UsedSize() != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestResync() {
    IOBuffer ioBuffer;
    return !ioBuffer.Resync();
}

bool IOBufferTest::TestSeek(uint32 usedSize,
                            uint32 seek,
                            bool expected) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(usedSize, 0);

    char8 toWrite[64];
    ioBuffer.Write(toWrite, usedSize);

    if (ioBuffer.Position() != usedSize) {
        return false;
    }

    bool ret = ioBuffer.Seek(seek);
    if (ret) {
        if (ioBuffer.Position() != seek) {
            return false;
        }
    }
    else {
        if (ioBuffer.Position() != usedSize) {
            return false;
        }
    }

    return ret == expected;
}

bool IOBufferTest::TestRelativeSeek(uint32 initialPos,
                                    int32 delta,
                                    bool expected) {
    IOBuffer ioBuffer;

    uint32 usedSize = 2 * initialPos;
    ioBuffer.SetBufferHeapMemory(usedSize, 0);

    char8 toWrite[64];
    ioBuffer.Write((const char8*) toWrite, usedSize);

    ioBuffer.Seek(initialPos);

    if (ioBuffer.Position() != initialPos) {
        return false;
    }

    bool ret = ioBuffer.RelativeSeek(delta);
    if (ret) {
        if (ioBuffer.Position() != (initialPos + delta)) {
            return false;
        }
    }
    else {
        if (delta >= 0) {
            if (ioBuffer.Position() != (uint32) (usedSize)) {
                return false;
            }
        }
        else {
            if (ioBuffer.Position() != 0) {
                return false;
            }
        }
    }

    return ret == expected;
}

bool IOBufferTest::TestPutC_Heap() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    char bufferTest[size];
    // heap memory
    ioBuffer.SetBufferHeapMemory(size, 0);
    for (uint32 i = 0; i < size; i++) {
        if (!ioBuffer.PutC('a')) {
            printf("\n1\n");
            return false;
        }
        bufferTest[i] = 'a';
    }
    if (StringHelper::CompareN(bufferTest, ioBuffer.Buffer(), size) != 0) {
        printf("\n2\n");
        return false;
    }
    //size is finished
    if (ioBuffer.PutC('a')) {
        printf("\n3\n");
        return false;
    }
    ioBuffer.Empty();
    return ioBuffer.PutC('a');
}

bool IOBufferTest::TestPutC_MemoryReference() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    char bufferIn[size];
    char bufferTest[size];
    // heap memory
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, 0);
    for (uint32 i = 0; i < size; i++) {
        if (!ioBuffer.PutC('a')) {
            return false;
        }
        bufferTest[i] = 'a';
    }
    if (StringHelper::CompareN(bufferTest, ioBuffer.Buffer(), size) != 0) {
        return false;
    }
    //size is finished
    if (ioBuffer.PutC('a')) {
        return false;
    }
    ioBuffer.Empty();
    return ioBuffer.PutC('a');
}

bool IOBufferTest::TestPutC_MemoryReferenceRO() {

    IOBuffer ioBuffer;

    const char8* bufferIn = "";
    const uint32 size = 32;
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, 0);
    return !ioBuffer.PutC('a');

}

bool IOBufferTest::TestGetC_Heap() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    ioBuffer.SetBufferHeapMemory(size, 0);

    char8 ret;
    if (ioBuffer.GetC(ret)) {
        return false;
    }

    const char8 *toWrite = "HelloWorld";
    uint32 usedSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, usedSize);

    ioBuffer.Seek(0);
    for (uint32 i = 0; i < usedSize; i++) {
        ioBuffer.GetC(ret);
        if (ret != toWrite[i]) {
            return false;
        }
    }

    //finished size
    return !ioBuffer.GetC(ret);
}

bool IOBufferTest::TestGetC_MemoryReference() {
    IOBuffer ioBuffer;

    const uint32 size = 32;
    char8 bufferIn[size];

    ioBuffer.SetBufferReferencedMemory(bufferIn, size, 0);

    char8 ret;
    if (ioBuffer.GetC(ret)) {
        return false;
    }

    const char8 *toWrite = "HelloWorld";
    uint32 usedSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, usedSize);

    ioBuffer.Seek(0);
    for (uint32 i = 0; i < usedSize; i++) {
        ioBuffer.GetC(ret);
        if (ret != toWrite[i]) {
            return false;
        }
    }

    //finished size
    return !ioBuffer.GetC(ret);
}

bool IOBufferTest::TestGetC_MemoryReferenceRO() {
    IOBuffer ioBuffer;

    const char8 *bufferIn = "HelloWorld";
    const uint32 size = StringHelper::Length(bufferIn);

    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, 0);

    char8 ret;
    if (ioBuffer.GetC(ret)) {
        return false;
    }
    ioBuffer.SetUsedSize(size);

    for (uint32 i = 0; i < size; i++) {
        ioBuffer.GetC(ret);
        if (ret != bufferIn[i]) {
            return false;
        }
    }

    //finished size
    return !ioBuffer.GetC(ret);

}

bool IOBufferTest::TestUnPutC() {

    IOBuffer ioBuffer;

    uint32 size = 32;
    ioBuffer.SetBufferHeapMemory(size, 0);

    if (ioBuffer.UnPutC()) {
        printf("\n1\n");
        return false;
    }

    const char8 *toWrite = "HelloWorld";
    uint32 usedSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, usedSize);

    for (uint32 i = 0; i < usedSize; i++) {
        if (!ioBuffer.UnPutC()) {
            printf("\n2\n");
            return false;
        }
        if (ioBuffer.UsedSize() != (usedSize - i - 1)) {
            printf("\n3\n");
            return false;
        }
    }
    // position = 0 !
    return !ioBuffer.UnPutC();
}

bool IOBufferTest::TestUnGetC() {

    IOBuffer ioBuffer;

    const char8 *bufferIn = "HelloWorld";
    uint32 size = StringHelper::Length(bufferIn);
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, 0);

    ioBuffer.SetUsedSize(size);

    char8 toRead[size];

    ioBuffer.Read(toRead, size);
    for (uint32 i = 0; i < size; i++) {
        if (!ioBuffer.UnGetC()) {
            printf("\n2\n");
            return false;
        }
        if (ioBuffer.Position() != (size - i - 1)) {
            printf("\n3\n");
            return false;
        }
    }
    // position = 0 !
    return !ioBuffer.UnGetC();
}

bool IOBufferTest::TestSetUsedSize(uint32 allocatedSize,
                                   uint32 desiredSize) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocatedSize, 0);
    if (!ioBuffer.SetUsedSize(desiredSize)) {
        return false;
    }

    return (desiredSize > allocatedSize) ? (ioBuffer.UsedSize() == allocatedSize) : (ioBuffer.UsedSize() == desiredSize);
}

bool IOBufferTest::TestWrite_Heap(uint32 allocationSize,
                                  uint32 writeSize,
                                  const char8 *string) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    if (!ioBuffer.Write(string, writeSize)) {
        return false;
    }

    uint32 compareSize = (allocationSize > writeSize) ? (writeSize) : (allocationSize);

    return StringHelper::CompareN(ioBuffer.Buffer(), string, compareSize) == 0;
}

bool IOBufferTest::TestWrite_Memoryreference() {

    IOBuffer ioBuffer;

    const uint32 size = 32;
    char8 bufferIn[size];
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, 0);

    const char8* string = "HelloWorld";
    uint32 writeSize = StringHelper::Length(string);
    if (!ioBuffer.Write(string, writeSize)) {
        return false;
    }

    if (StringHelper::CompareN(ioBuffer.Buffer(), string, writeSize) != 0) {
        return false;
    }

    writeSize -= 5;

    if (!ioBuffer.Write(string, writeSize)) {
        return false;
    }

    return StringHelper::CompareN(ioBuffer.Buffer(), string, writeSize) == 0;
}

bool IOBufferTest::TestWrite_MemoryreferenceRO() {

    IOBuffer ioBuffer;

    const uint32 size = 32;
    const char8 *bufferIn = "HelloWorld";
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, 0);

    const char8* string = "HelloWorld";
    uint32 writeSize = StringHelper::Length(string);
    return !ioBuffer.Write(string, writeSize);
}

bool IOBufferTest::TestWrite_NULL_Buffer() {
    IOBuffer ioBuffer;

    uint32 allocSize = 32;
    ioBuffer.SetBufferHeapMemory(allocSize, 0);

    const char8 *bufferIn = NULL;
    return !ioBuffer.Write(bufferIn, allocSize);

}

bool IOBufferTest::TestRead_Heap(uint32 allocatedSize,
                                 uint32 readSize) {
    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocatedSize, 0);

    for (uint32 i = 0; i < allocatedSize; i++) {
        ioBuffer.PutC('a');
    }

    char8 buffer[32];

    ioBuffer.Read(buffer, readSize);

    if (readSize > allocatedSize) {
        readSize = allocatedSize;
    }

    for (uint32 i = 0; i < readSize; i++) {
        if (buffer[i] != 'a') {
            return false;
        }
    }
    return true;
}

bool IOBufferTest::TestRead_MemoryReference() {
    IOBuffer ioBuffer;

    char8 bufferIn[32];
    uint32 allocatedSize = 32;
    ioBuffer.SetBufferReferencedMemory(bufferIn, allocatedSize, 0);

    for (uint32 i = 0; i < allocatedSize; i++) {
        ioBuffer.PutC('a');
    }

    char8 bufferOut[32];

    // greater size
    uint32 readSize = 34;

    ioBuffer.Seek(0);
    ioBuffer.Read(bufferOut, readSize);

    for (uint32 i = 0; i < allocatedSize; i++) {
        if (bufferOut[i] != 'a') {
            return false;
        }
    }

    //empty the out buffer
    for (uint32 i = 0; i < 32; i++) {
        bufferOut[i] = '\0';
    }

    ioBuffer.Seek(0);
    readSize = 5;
    ioBuffer.Read(bufferOut, readSize);

    return StringHelper::Compare(bufferOut, "aaaaa") == 0;
}

bool IOBufferTest::TestRead_NULL_Buffer() {

    const char8 *bufferIn = "Hello";
    IOBuffer ioBuffer;
    uint32 allocSize = 16;

    ioBuffer.SetBufferHeapMemory(allocSize, 0);

    uint32 writeSize = StringHelper::Length(bufferIn);
    ioBuffer.Write(bufferIn, writeSize);

    ioBuffer.Seek(0);
    char8* bufferOut = NULL;
    return !ioBuffer.Read(bufferOut, allocSize);
}

bool IOBufferTest::TestWriteAll(uint32 allocationSize,
                                uint32 writeSize,
                                const char8* string) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    if (allocationSize > writeSize) {
        if (!ioBuffer.WriteAll(string, writeSize)) {
            return false;
        }
    }
    else {
        if (ioBuffer.WriteAll(string, writeSize)) {
            return false;
        }
        writeSize = allocationSize;
    }

    return StringHelper::CompareN(ioBuffer.Buffer(), string, writeSize) == 0;
}

bool IOBufferTest::TestEmpty() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    //at the beginnig the buffer is empty
    if (ioBuffer.Position() != 0) {
        return false;
    }

    if ((ioBuffer.AmountLeft() != allocationSize) || (ioBuffer.UsedAmountLeft() != 0)) {
        return false;
    }

    // put something in the buffer
    const char8 *toWrite = "HelloWorld";
    uint32 writeSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, writeSize);

    if (ioBuffer.Position() != writeSize) {
        return false;
    }
    if (ioBuffer.AmountLeft() != (allocationSize - writeSize)) {
        return false;
    }

    ioBuffer.Seek(0);
    if (ioBuffer.UsedAmountLeft() != writeSize) {
        return false;
    }

    ioBuffer.Seek(writeSize);

    ioBuffer.Empty();
    //the buffer is empty
    if (ioBuffer.Position() != 0) {
        return false;
    }

    if ((ioBuffer.AmountLeft() != allocationSize) || (ioBuffer.UsedAmountLeft() != 0)) {
        return false;
    }

    return true;

}

void Clear(IOBuffer &ioBuffer) {
    ioBuffer.Seek(0);
    for (uint32 i = 0; i < ioBuffer.MaxUsableAmount(); i++) {
        ioBuffer.PutC(0);
    }
    ioBuffer.Seek(0);
}

bool IOBufferTest::TestPrintFormattedToStream_Integer_Decimal() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    uint8 ubit8 = 255;
    AnyType toPrint = ubit8;

    Clear(ioBuffer);

    ioBuffer.PrintFormattedToStream("%d", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());
    if (StringHelper::Compare("255", ioBuffer.Buffer()) != 0) {
        printf("\n1\n");
        return false;
    }

    Clear(ioBuffer);

    //with size but and not padded (nothing happen)
    ioBuffer.PrintFormattedToStream("%5d", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("255", ioBuffer.Buffer()) != 0) {
        printf("\n2\n");
        return false;
    }

    Clear(ioBuffer);

    //RightAligned
    ioBuffer.PrintFormattedToStream("% 5d", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("  255", ioBuffer.Buffer()) != 0) {
        printf("\n2\n");
        return false;
    }

    Clear(ioBuffer);

    //LeftAligned
    ioBuffer.PrintFormattedToStream("%-5d", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("255  ", ioBuffer.Buffer()) != 0) {
        printf("\n2\n");
        return false;
    }

    Clear(ioBuffer);

    Clear(ioBuffer);
    int8 sbit8 = -127;
    toPrint = sbit8;

    ioBuffer.PrintFormattedToStream("%d", &toPrint);

    if (StringHelper::Compare("-127", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //LeftAligned and padded "...  "
    ioBuffer.PrintFormattedToStream("%-6i", &toPrint);
    if (StringHelper::Compare("-127  ", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //Max int8
    sbit8 = 0x80;
    toPrint = sbit8;

    ioBuffer.PrintFormattedToStream("%d", &toPrint);

    if (StringHelper::Compare("-128", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    uint16 ubit16 = 12345;
    toPrint = ubit16;
    ioBuffer.PrintFormattedToStream("%u", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("12345", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //LeftAligned and padded with positive sign "+...  "
    ioBuffer.PrintFormattedToStream("%-#8d", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("+12345  ", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    int16 sbit16 = -12345;

    toPrint = sbit16;

    ioBuffer.PrintFormattedToStream("%d", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("-12345", ioBuffer.Buffer()) != 0) {

        return false;
    }
    Clear(ioBuffer);

    //Max int16
    sbit16 = 0x8000;
    toPrint = sbit16;
    ioBuffer.PrintFormattedToStream("%i", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("-32768", ioBuffer.Buffer()) != 0) {

        return false;
    }

    Clear(ioBuffer);

    uint32 ubit32 = 123456789;
    toPrint = ubit32;
    printf("\n|%s|\n", ioBuffer.Buffer());

    ioBuffer.PrintFormattedToStream("%d", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("123456789", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //RightAligned and padded with sign " +..."
    ioBuffer.PrintFormattedToStream("% #11u", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare(" +123456789", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    int32 sbit32 = -123456789;
    toPrint = sbit32;

    ioBuffer.PrintFormattedToStream("%i", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("-123456789", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //Max int32
    sbit32 = 0x80000000;
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("%i", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());

    if (StringHelper::Compare("-2147483648", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    uint64 ubit64 = 12345678912345678;
    toPrint = ubit64;
    ioBuffer.PrintFormattedToStream("%u", &toPrint);

    if (StringHelper::Compare("12345678912345678", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    int64 sbit64 = -12345678912345678;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%i", &toPrint);

    if (StringHelper::Compare("-12345678912345678", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Max int64
    sbit64 = 0x8000000000000000;
    toPrint = sbit64;

    ioBuffer.PrintFormattedToStream("%i", &toPrint);

    if (StringHelper::Compare("-9223372036854775808", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //? if the maxSize is incorrect (padded)
    ioBuffer.PrintFormattedToStream("% #11d", &toPrint);

    if (StringHelper::Compare("          ?", ioBuffer.Buffer()) != 0) {
        return false;
    }

    return true;

}

bool IOBufferTest::TestPrintFormattedToStream_Integer_Hexadecimal() {
    IOBuffer ioBuffer;
    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);
    uint8 ubit8 = 0xea;

    AnyType toPrint = ubit8;

    ioBuffer.PrintFormattedToStream("%x", &toPrint);
    if (StringHelper::Compare("EA", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //Check if works also for negative signed numbers.
    int8 sbit8 = 0xea;
    toPrint = sbit8;
    ioBuffer.PrintFormattedToStream("%x", &toPrint);

    if (StringHelper::Compare("EA", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //size but not padded (nothing happen)
    ioBuffer.PrintFormattedToStream("%5x", &toPrint);
    if (StringHelper::Compare("EA", ioBuffer.Buffer()) != 0) {
        return false;
    }

    //Add Header and trailing zeros
    Clear(ioBuffer);
    ioBuffer.PrintFormattedToStream("%#0x", &toPrint);
    if (StringHelper::Compare("0xEA", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit8 = 0xf;
    toPrint = ubit8;
    //Add only trailing zeros
    Clear(ioBuffer);

    ioBuffer.PrintFormattedToStream("%0x", &toPrint);

    if (StringHelper::Compare("0F", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //All true with a space more... it must print only a space after.
    uint16 ubit16 = 0xabcd;
    toPrint = ubit16;
    ioBuffer.PrintFormattedToStream("%-#07x", &toPrint);

    if (StringHelper::Compare("0xABCD ", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    ubit16 = 0xcd;
    toPrint = ubit16;
    //With zeros and 3 as number of digits (without header).
    ioBuffer.PrintFormattedToStream("%#05x", &toPrint);

    if (StringHelper::Compare("0x0CD", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //Only right aligned with header
    uint32 ubit32 = 0xabcdef78;
    toPrint = ubit32;
    ioBuffer.PrintFormattedToStream("% #12x", &toPrint);

    if (StringHelper::Compare("  0xABCDEF78", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    ubit32 = 0x00abcd0f;
    toPrint = ubit32;
    //Right align with zero and header
    ioBuffer.PrintFormattedToStream("% #011x", &toPrint);

    if (StringHelper::Compare(" 0x00ABCD0F", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //Right align without zeros and header
    ioBuffer.PrintFormattedToStream("% 11x", &toPrint);

    if (StringHelper::Compare("     ABCD0F", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //padded=false
    uint64 ubit64 = 0x89abcdef0123fff0;
    toPrint = ubit64;
    ioBuffer.PrintFormattedToStream("%120x", &toPrint);

    if (StringHelper::Compare("89ABCDEF0123FFF0", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //?
    ubit64 = 0x123fff0;
    toPrint = ubit64;
    ioBuffer.PrintFormattedToStream("% 5x", &toPrint);

    if (StringHelper::Compare("    ?", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    ubit64 = 0xfff0;
    toPrint = ubit64;

    ioBuffer.PrintFormattedToStream("%-07x", &toPrint);

    if (StringHelper::Compare("000FFF0", ioBuffer.Buffer()) != 0) {

        return false;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Integer_Octal() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);
    uint8 ubit8 = 0xea;
    AnyType toPrint = ubit8;

    ioBuffer.PrintFormattedToStream("%o", &toPrint);
    if (StringHelper::Compare("352", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Test if works also for negative signed numbers.
    int8 sbit8 = -22;
    toPrint = sbit8;
    ioBuffer.PrintFormattedToStream("%o", &toPrint);

    if (StringHelper::Compare("352", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //with size but not padded (nothing happen)
    ioBuffer.PrintFormattedToStream("%5o", &toPrint);

    if (StringHelper::Compare("352", ioBuffer.Buffer()) != 0) {
        return false;
    }

    //Add Header and trailing zeros
    Clear(ioBuffer);
    ioBuffer.PrintFormattedToStream("%#0o", &toPrint);

    if (StringHelper::Compare("0o352", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit8 = 0xf;
    toPrint = ubit8;

    //Add only trailing zeros
    ioBuffer.PrintFormattedToStream("%0o", &toPrint);

    if (StringHelper::Compare("017", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //All true with a space more... it must print a space after.
    uint16 ubit16 = 0x7bcd; //6 is the maximum now are 5
    toPrint = ubit16;

    ioBuffer.PrintFormattedToStream("%-#09o", &toPrint);

    if (StringHelper::Compare("0o075715 ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit16 = 0xcd;
    toPrint = ubit16;
    //With zeros and 5 as number of digits (without header).

    ioBuffer.PrintFormattedToStream("%#07o", &toPrint);

    if (StringHelper::Compare("0o00315", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Only right aligned with header
    uint32 ubit32 = 0xabcdef78;
    toPrint = ubit32;
    ioBuffer.PrintFormattedToStream("% #15o", &toPrint);

    if (StringHelper::Compare("  0o25363367570", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit32 = 0xcd0f;
    toPrint = ubit32;
    //Right align with zero and header
    ioBuffer.PrintFormattedToStream("% #015o", &toPrint);

    if (StringHelper::Compare("  0o00000146417", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Right align without zeros and header
    ioBuffer.PrintFormattedToStream("% 15o", &toPrint);

    if (StringHelper::Compare("         146417", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //padded=false
    uint64 ubit64 = 0x89abcdef01240000;
    toPrint = ubit64;
    ioBuffer.PrintFormattedToStream("%120o", &toPrint);

    if (StringHelper::Compare("1046536336740111000000", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);
    //?
    ubit64 = 0x123fff0;
    toPrint = ubit64;
    ioBuffer.PrintFormattedToStream("% 5o", &toPrint);

    if (StringHelper::Compare("    ?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit64 = 0xfff0;
    toPrint = ubit64;

    ioBuffer.PrintFormattedToStream("%-07o", &toPrint);

    if (StringHelper::Compare("0177760", ioBuffer.Buffer()) != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Integer_Binary() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 128;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    uint8 ubit8 = 0xea;
    AnyType toPrint = ubit8;

    ioBuffer.PrintFormattedToStream("%b", &toPrint);

    if (StringHelper::Compare("11101010", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Test if works also for negative signed numbers
    int8 sbit8 = 0xea;
    toPrint = sbit8;

    ioBuffer.PrintFormattedToStream("%b", &toPrint);
    if (StringHelper::Compare("11101010", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //with size but not padded (nothing happen)
    ioBuffer.PrintFormattedToStream("%50b", &toPrint);

    if (StringHelper::Compare("11101010", ioBuffer.Buffer()) != 0) {
        return false;
    }

    //Add header and trailing zeros
    Clear(ioBuffer);
    ioBuffer.PrintFormattedToStream("%#0b", &toPrint);
    if (StringHelper::Compare("0b11101010", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit8 = 0xf;
    toPrint = ubit8;

    //Add only trailing zeros
    ioBuffer.PrintFormattedToStream("%0b", &toPrint);
    if (StringHelper::Compare("00001111", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //All true with a space more... it must print a space after.
    uint16 ubit16 = 0x7bcd; //
    toPrint = ubit16;

    ioBuffer.PrintFormattedToStream("%-#019b", &toPrint);
    if (StringHelper::Compare("0b0111101111001101 ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit16 = 0xcd;
    toPrint = ubit16;
    //With zeros and 10 as number of digits (without header).
    ioBuffer.PrintFormattedToStream("%#012b", &toPrint);
    if (StringHelper::Compare("0b0011001101", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //right aligned with header
    uint32 ubit32 = 0xabcdef78;
    toPrint = ubit32;

    ioBuffer.PrintFormattedToStream("% #36b", &toPrint);
    printf("\n|%s|\n", ioBuffer.Buffer());
    if (StringHelper::Compare("  0b10101011110011011110111101111000", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit32 = 0xcd0f;
    toPrint = ubit32;

    ioBuffer.PrintFormattedToStream("% 034b", &toPrint);
    //Right align with zeros
    if (StringHelper::Compare("  00000000000000001100110100001111", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Right align without zeros and header
    ioBuffer.PrintFormattedToStream("% 34b", &toPrint);
    if (StringHelper::Compare("                  1100110100001111", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //padded=false
    uint64 ubit64 = 0x8000000000000001;
    toPrint = ubit64;
    ioBuffer.PrintFormattedToStream("%120b", &toPrint);
    if (StringHelper::Compare("1000000000000000000000000000000000000000000000000000000000000001", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //?
    ubit64 = 0x123fff0;
    toPrint = ubit64;
    ioBuffer.PrintFormattedToStream("% 5b", &toPrint);
    if (StringHelper::Compare("    ?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    ubit64 = 0xff0;
    toPrint = ubit64;
    ioBuffer.PrintFormattedToStream("%-014b", &toPrint);
    if (StringHelper::Compare("00111111110000", ioBuffer.Buffer()) != 0) {
        return false;
    }
    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_FixedPoint() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    float32 sbit32 = -1.1234567;
    AnyType toPrint = sbit32;

    //Left padded, negative number, exact precision.
    ioBuffer.PrintFormattedToStream("%- 12.7f", &toPrint);
    printf("%12.7f %s", sbit32, ioBuffer.Buffer());
    if (StringHelper::Compare("-1.1234567  ", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //6 decimal numbers by default.
    ioBuffer.PrintFormattedToStream("%f", &toPrint);
    if (StringHelper::Compare("-1.123457", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Less precision.
    ioBuffer.PrintFormattedToStream("%- 12.3f", &toPrint);
    if (StringHelper::Compare("-1.123      ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Right aligned padding.
    sbit32 = 112345.67; //112345,...
    toPrint = sbit32;

    ioBuffer.PrintFormattedToStream("% 12.2f", &toPrint);
    if (StringHelper::Compare("   112345.67", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Print 0
    sbit32 = 0; //112345,...
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("% 3.2f", &toPrint);
    if (StringHelper::Compare("  0", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Underflow

    sbit32 = 0.09; //112345,...
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("% 1.2f", &toPrint);

    if (StringHelper::Compare("0", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up correction.
    float64 sbit64 = 12345.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 10.3f", &toPrint);

    if (StringHelper::Compare(" 12346.000", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Get the integer part.
    ioBuffer.PrintFormattedToStream("% 10.0f", &toPrint);

    if (StringHelper::Compare("     12346", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Over precision
    sbit64 = 999999.99996;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 17.9f", &toPrint);

    if (StringHelper::Compare(" 999999.999960000", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Overflow and not enough space, because the rounding up we need at least seven digits.
    ioBuffer.PrintFormattedToStream("% 6.4f", &toPrint);
    if (StringHelper::Compare("     ?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Point removing and overflow.
    ioBuffer.PrintFormattedToStream("%-8.4f", &toPrint);
    if (StringHelper::Compare("1000000 ", ioBuffer.Buffer()) != 0) {
        return false;
    }

    //Enough space for the point and a decimal number.
    Clear(ioBuffer);

    ioBuffer.PrintFormattedToStream("%-9.4f", &toPrint);
    if (StringHelper::Compare("1000000.0", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //No space for the fraction part.
    sbit64 = -222222.5255;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%7.4f", &toPrint);
    if (StringHelper::Compare("-222223", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //No space for the fraction part again.
    ioBuffer.PrintFormattedToStream("% 8.4f", &toPrint);
    if (StringHelper::Compare(" -222223", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Space enough for one decimal number. Approximation at the second digit after .
    ioBuffer.PrintFormattedToStream("% 9.4f", &toPrint);
    if (StringHelper::Compare("-222222.5", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Space enouh for two decimal numbers. Approximation at the third digit after .
    ioBuffer.PrintFormattedToStream("% 10.4f", &toPrint);
    if (StringHelper::Compare("-222222.53", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space for the integer part.
    ioBuffer.PrintFormattedToStream("% 5.4f", &toPrint);
    if (StringHelper::Compare("    ?", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //Negative exponent.
    sbit64 = 0.009;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 5.2f", &toPrint);

    if (StringHelper::Compare(" 0.01", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //Not enough space (it must be at least 12+1(zero)+1(point)=14)
    sbit64 = 1e-12;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 12.4f", &toPrint);
    if (StringHelper::Compare("           0", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space with 1 as maxSize and for negative number (it must be at least 12+1(zero)+1(point)=14)
    sbit64 = -1e-12;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 1.10f", &toPrint);
    if (StringHelper::Compare("0", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Positive inf.
    float32 inf = 1.0 / 0.0;
    toPrint = inf;
    ioBuffer.PrintFormattedToStream("%10.10f", &toPrint);
    if (StringHelper::Compare("+Inf", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Negative inf
    float64 ninf = -1.0 / 0.0;
    toPrint = ninf;
    ioBuffer.PrintFormattedToStream("% 10.10f", &toPrint);
    if (StringHelper::Compare("      -Inf", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space to print -Inf
    ioBuffer.PrintFormattedToStream("%-1.10f", &toPrint);
    if (StringHelper::Compare("?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //NaN
    float64 nan = 0.0 / 0.0;
    toPrint = nan;
    ioBuffer.PrintFormattedToStream("%-10.10f", &toPrint);
    if (StringHelper::Compare("NaN       ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Nan with automatic size
    ioBuffer.PrintFormattedToStream("% 0.4f", &toPrint);
    if (StringHelper::Compare("NaN", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space to print Nan
    ioBuffer.PrintFormattedToStream("% 2.10f", &toPrint);
    if (StringHelper::Compare(" ?", ioBuffer.Buffer()) != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_FixedRelativePoint() {
    IOBuffer ioBuffer;
    uint32 allocationSize = 64;

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    float32 sbit32 = -1.1234567;
    AnyType toPrint = sbit32;
    //Left padded, negative number, exact precision

    ioBuffer.PrintFormattedToStream("%-12.8F", &toPrint);
    if (StringHelper::Compare("-1.1234567  ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Automatic precision set to 7 for 32 bit numbers.
    ioBuffer.PrintFormattedToStream("%-12F", &toPrint);
    if (StringHelper::Compare("-1.123457   ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Precision 0.
    ioBuffer.PrintFormattedToStream("%-12.0F", &toPrint);
    if (StringHelper::Compare("0           ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Less precision.
    ioBuffer.PrintFormattedToStream("%-12.4F", &toPrint);
    if (StringHelper::Compare("-1.123      ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Right aligned padding
    sbit32 = 112345.67; //112345,...
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("% 12.8F", &toPrint);
    if (StringHelper::Compare("   112345.67", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up correction
    float64 sbit64 = 12345.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 10.8F", &toPrint);
    if (StringHelper::Compare(" 12346.000", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Less precision than the size of integer part.
    ioBuffer.PrintFormattedToStream("% 10.3F", &toPrint);
    if (StringHelper::Compare("     12300", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Over precision.
    sbit64 = 999999.99996;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 17.15F", &toPrint);
    if (StringHelper::Compare(" 999999.999960000", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Overflow and not enough space because the round up.
    ioBuffer.PrintFormattedToStream("% 6.10F", &toPrint);
    if (StringHelper::Compare("     ?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Overflow, no space for decimals.
    ioBuffer.PrintFormattedToStream("%-8.10F", &toPrint);
    if (StringHelper::Compare("1000000 ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Enough space for the point and a decimal number.
    ioBuffer.PrintFormattedToStream("%-9.10F", &toPrint);
    if (StringHelper::Compare("1000000.0", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //No space for decimal numbers
    sbit64 = -222222.5255;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%7.10F", &toPrint);
    if (StringHelper::Compare("-222223", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //No space for decimal numbers again.
    ioBuffer.PrintFormattedToStream("% 8.10F", &toPrint);
    if (StringHelper::Compare(" -222223", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Space enough for one decimal number. Approximation at the second digit after .
    ioBuffer.PrintFormattedToStream("% 9.10F", &toPrint);
    if (StringHelper::Compare("-222222.5", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Space enough for two numbers. Approximation at the third digit after .
    ioBuffer.PrintFormattedToStream("% 10.10F", &toPrint);
    if (StringHelper::Compare("-222222.53", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space for the integer part
    ioBuffer.PrintFormattedToStream("% 5.10F", &toPrint);
    if (StringHelper::Compare("    ?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space (it must be at least 12+1(zero)+1(point)=14)
    sbit64 = 1e-12;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 12.12F", &toPrint);
    if (StringHelper::Compare("           0", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Poisitive inf.
    float32 inf = 1.0 / 0.0;
    toPrint = inf;
    ioBuffer.PrintFormattedToStream("%10.10F", &toPrint);
    if (StringHelper::Compare("+Inf", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Negative inf
    float64 ninf = -1.0 / 0.0;
    toPrint = ninf;
    ioBuffer.PrintFormattedToStream("% 10.10F", &toPrint);
    if (StringHelper::Compare("      -Inf", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space to print -Inf
    ioBuffer.PrintFormattedToStream("%-1.10F", &toPrint);
    if (StringHelper::Compare("?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //NaN
    float64 nan = 0.0 / 0.0;
    toPrint = nan;
    ioBuffer.PrintFormattedToStream("%-10.10F", &toPrint);
    if (StringHelper::Compare("NaN       ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Nan with automatic size
    ioBuffer.PrintFormattedToStream("% 0.10F", &toPrint);
    if (StringHelper::Compare("NaN", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space to print Nan
    ioBuffer.PrintFormattedToStream("% 2.10F", &toPrint);
    if (StringHelper::Compare(" ?", ioBuffer.Buffer()) != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_Exponential() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    float32 sbit32 = -11.234567;
    AnyType toPrint = sbit32;

    ioBuffer.PrintFormattedToStream("%-15.8e", &toPrint);
    //Left alignment padded
    if (StringHelper::Compare("-1.1234567E+1  ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Left alignment, clip the number without rounding (the next is 4).
    ioBuffer.PrintFormattedToStream("%-9.4e", &toPrint);
    if (StringHelper::Compare("-1.123E+1", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Right Alignment with round up because the size (the next number is 6 because precision became 10(size)-3(exp)-1(point)=6)
    sbit32 = 112345.67; //112345,...
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("% 10.8e", &toPrint);
    if (StringHelper::Compare("1.12346E+5", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Automatic precision=15 for 64 bit.
    float64 sbit64 = -12345.9999;
    toPrint = sbit64;

    ioBuffer.PrintFormattedToStream("% 21.e", &toPrint);
    if (StringHelper::Compare(" -1.23459999000000E+4", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up and zero added because the chosen precision.
    sbit64 = -12345.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 13.8e", &toPrint);
    if (StringHelper::Compare("-1.2346000E+4", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //In case of round up overflow.
    sbit64 = 999999.55556;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%.6e", &toPrint);
    if (StringHelper::Compare("1.00000E+6", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Clip the precision because of the size.
    ioBuffer.PrintFormattedToStream("%-5.6e", &toPrint);
    if (StringHelper::Compare("1E+6 ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Too few space for number + exponent.
    ioBuffer.PrintFormattedToStream("%-3.2e", &toPrint);
    if (StringHelper::Compare("?  ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number initially does not fit but later yes thanks to the approximation.

    sbit64 = 9.5e-10;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%-4.2e", &toPrint);
    if (StringHelper::Compare("1E-9", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The approximation is not sufficient now.
    sbit64 = 9.4999e-10;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%-4.2e", &toPrint);
    if (StringHelper::Compare("?   ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Support exponent with 3 digits
    /*    sbit64 = 9.9999e-101;
     toPrint = sbit64;
     ioBuffer.PrintFormattedToStream("%-6.2e", &toPrint);
     if (StringHelper::Compare("1E-100", ioBuffer.Buffer()) != 0) {
     return false;
     }
     Clear(ioBuffer);

     //Support exponent with 4 digits
     sbit64 = 9.9999e+999;
     toPrint = sbit64;
     ioBuffer.PrintFormattedToStream("%-7.2e", &toPrint);
     if (StringHelper::Compare("1E+1000", ioBuffer.Buffer()) != 0) {
     return false;
     }
     Clear(ioBuffer);
     */
    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_Engeneering() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    //Precision lost (with float32 this happens frequently)!
    float32 sbit32 = -11.234567;
    AnyType toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("%.9E", &toPrint);
    if (StringHelper::Compare("-11.2345671", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Left Align without correction (the next number is 4)
    sbit32 *= 100;
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("%-10.4E", &toPrint);
    if (StringHelper::Compare("-1.123E+3 ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Right align padded with round up correction (the next number is 5)

    sbit32 = 112345.67;
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("% 10.8E", &toPrint);
    if (StringHelper::Compare("112.346E+3", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up and zero added because the precision clip caused by the size (precision become 6).
    float64 sbit64 = 12345.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 10.9E", &toPrint);
    if (StringHelper::Compare("12.3460E+3", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Without rounding up it can't print (999e-3) but with approximation it can.
    sbit64 = -0.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 3.3E", &toPrint);
    if (StringHelper::Compare(" -1", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //If precision is too much we don't have approximation, then the size is not sufficient.
    sbit64 = -0.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 3.5E", &toPrint);
    if (StringHelper::Compare(" -1", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up and right aligned padd.
    sbit64 = 999999.99999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 20.10E", &toPrint);
    if (StringHelper::Compare("      1.000000000E+6", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Automatic precision=34 for 128 bit number (clipped).
    sbit64 = 999999.9999900000000000000000000000;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 19E", &toPrint);
    if (StringHelper::Compare("999.999999990000E+3", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);
    //Not enough space for decimal numbers.
    sbit64 = -9.99999e9;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%-7.3E", &toPrint);
    if (StringHelper::Compare("-10E+9 ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Not enough space to print the approximated number -100E+9.
    sbit64 *= 10; //-99e9
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 6.4E", &toPrint);
    if (StringHelper::Compare("     ?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Also increasing the precision, it is clipped because the size then with the overflow the size is not sufficient.
    ioBuffer.PrintFormattedToStream("% 6.6E", &toPrint);
    if (StringHelper::Compare("     ?", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);

    //? the size is less than the minimum required
    sbit64 *= 10; //-1E+12 min_size=6
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%5.2E", &toPrint);

    if (StringHelper::Compare("?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number enter as 999 not approximated, and then approximation enter again in the size
    sbit64 = 999.9;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 4.10E", &toPrint);
    if (StringHelper::Compare("1E+3", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number enter as 999 not approximated, but with approximation exceed the size then ?
    ioBuffer.PrintFormattedToStream("% 3.10E", &toPrint);
    if (StringHelper::Compare("  ?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number does not enter as 999E+3 but after approximation enter in the size as 1E+6
    sbit64 = 999.9e3;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 4.10E", &toPrint);
    if (StringHelper::Compare("1E+6", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number initially does not fit, but it fit later thanks to approximation.
    sbit64 = 950.0e-6;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 4.10E", &toPrint);
    if (StringHelper::Compare("1E-3", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The approximation is not enough for the rounding.
    sbit64 = 949.9e-6;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 4.10E", &toPrint);
    if (StringHelper::Compare("   ?", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_Smart() {

    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    Clear(ioBuffer);
    float32 sbit32 = -11.234567;
    AnyType toPrint = sbit32;

    ioBuffer.PrintFormattedToStream("%.9g", &toPrint);
    //Precision lost (with float32 this happens frequently)!
    if (StringHelper::Compare("-11.2345671", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Left Align without correction (the next is 4)
    sbit32 *= 100; //1123.4567
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("%- 10.4g", &toPrint);
    if (StringHelper::Compare("-1.123K   ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Right align padded with round up correction (the next is 5)
    sbit32 = 112345.67; //112345,...
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("% 8.6g", &toPrint);
    if (StringHelper::Compare("112.346K", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up and zero added because the chosen precision.
    float64 sbit64 = 12345.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 10.6g", &toPrint);
    if (StringHelper::Compare("  12.3460K", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    sbit64 = -0.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 3.3g", &toPrint);
    if (StringHelper::Compare(" -1", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up and right aligned padd.
    sbit64 = 999999.99999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 20.10g", &toPrint);

    if (StringHelper::Compare("        1.000000000M", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Automatic size definition
    sbit64 = -9.99999e9;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%-7.3g", &toPrint);
    if (StringHelper::Compare("-10.0G ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Clip the precision because the size
    sbit64 *= 10; //-9.9e10
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%-4.4g", &toPrint);
    if (StringHelper::Compare("?   ", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);
    //? the size is less than the minimum required
    sbit64 *= 10; //-999E+9 min_size=7
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 4.5g", &toPrint);
    if (StringHelper::Compare(" -1T", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number enter as 999 not approximated, and then approximation enter again in the size
    sbit64 = 999.9;
    ioBuffer.PrintFormattedToStream("% 4.10g", &toPrint);
    if (StringHelper::Compare("1.0K", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number does not enter as 999K but after approximation enter in the size as 1M
    sbit64 = 999.9e3;
    ioBuffer.PrintFormattedToStream("% 3.10g", &toPrint);
    if (StringHelper::Compare(" 1M", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //print as engineering for exponents >24 and <-24
    sbit64 = 9e28;
    ioBuffer.PrintFormattedToStream("%-7.10g", &toPrint);
    if (StringHelper::Compare("90E+27 ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    sbit64 = -9.9e-28;
    ioBuffer.PrintFormattedToStream("% 9.10g", &toPrint);
    if (StringHelper::Compare(" -990E-30", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_Compact() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    float32 sbit32 = -11.234567;
    AnyType toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("%.9G", &toPrint);
    //Precision lost (with float32 this happens frequently)!
    if (StringHelper::Compare("-11.2345671", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Left Align without correction (the next is 4)
    sbit32 *= 100; //1123.4567
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("%-10.4G", &toPrint);
    if (StringHelper::Compare("-1.123K   ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Right align padded with round up correction (the next is 5)
    sbit32 = 112345.67; //112345,...
    toPrint = sbit32;
    ioBuffer.PrintFormattedToStream("% 8.6G", &toPrint);
    if (StringHelper::Compare("112.346K", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up and zero added because the chosen precision.
    float64 sbit64 = 12345.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 10.6G", &toPrint);
    if (StringHelper::Compare("  12.3460K", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    sbit64 = -0.9999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 3.3G", &toPrint);
    if (StringHelper::Compare(" -1", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Round up and right aligned padd.
    sbit64 = 999999.99999;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 20.10G", &toPrint);
    if (StringHelper::Compare("        1.000000000M", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Automatic size definition
    sbit64 = -9.99999e9;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%-7.3G", &toPrint);
    if (StringHelper::Compare("-10.0G ", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //Clip the precision because the size
    sbit64 *= 10; //-9.9e10
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%-4.4G", &toPrint);
    if (StringHelper::Compare("?   ", ioBuffer.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer);
    //? the size is less than the minimum required
    sbit64 *= 10; //-999E+9 min_size=7
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 4.54G", &toPrint);

    if (StringHelper::Compare(" -1T", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number enter as 999 not approximated, and then approximation enter again in the size
    sbit64 = 999.9;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 4.10G", &toPrint);
    if (StringHelper::Compare("1.0K", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //The number does not enter as 999K but after approximation enter in the size as 1M
    sbit64 = 999.9e3;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 3.10G", &toPrint);
    if (StringHelper::Compare(" 1M", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    //print as engineering for exponents >24 and <-24
    sbit64 = 9e28;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("%-7.10G", &toPrint);
    if (StringHelper::Compare("9.0E+28", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    sbit64 = -9.9e-28;
    toPrint = sbit64;
    ioBuffer.PrintFormattedToStream("% 9.10G", &toPrint);
    if (StringHelper::Compare("-9.90E-28", ioBuffer.Buffer()) != 0) {
        return false;
    }
    Clear(ioBuffer);

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_CCString() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    const char* Hello = "HelloWorld";

    AnyType toPrint = Hello;

    ioBuffer.PrintFormattedToStream("string:%s", &toPrint);
    if (StringHelper::Compare(ioBuffer.Buffer(), "string:HelloWorld") != 0) {
        return false;
    }
    //Clip the string
    Clear(ioBuffer);
    ioBuffer.PrintFormattedToStream("string:%5s", &toPrint);
    if (StringHelper::Compare(ioBuffer.Buffer(), "string:Hello") != 0) {
        return false;
    }
    //Padd right the string
    Clear(ioBuffer);
    ioBuffer.PrintFormattedToStream("string:% 11s", &toPrint);
    if (StringHelper::Compare(ioBuffer.Buffer(), "string: HelloWorld") != 0) {
        return false;
    }

    //Padd left the string
    Clear(ioBuffer);
    ioBuffer.PrintFormattedToStream("string:%- 11s", &toPrint);
    if (StringHelper::Compare(ioBuffer.Buffer(), "string:HelloWorld ") != 0) {
        return false;
    }
    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Pointer() {

    IOBuffer ioBuffer1;
    IOBuffer ioBuffer2;

    uint32 allocationSize = 64;
    ioBuffer1.SetBufferHeapMemory(allocationSize, 0);
    ioBuffer2.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer1);
    Clear(ioBuffer2);
    //return the pointer thanks to hex notation.

    const char *charPointer = "Hello";
    AnyType toPrintChar = charPointer;
    uint64 UIntPointer = (uint64) charPointer;
    AnyType toPrintUInt = UIntPointer;
    ioBuffer1.PrintFormattedToStream("%x", &toPrintChar);
    ioBuffer2.PrintFormattedToStream("%x", &toPrintUInt);

    if (StringHelper::Compare(ioBuffer1.Buffer(), ioBuffer2.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer1);
    Clear(ioBuffer2);
    //%p format as the complete 32 bit pointer with header

    void* pointer = (void*) charPointer;
    AnyType toPrintPointer = pointer;

    ioBuffer1.PrintFormattedToStream("%p", &toPrintPointer);
    ioBuffer2.PrintFormattedToStream("% #0x", &toPrintUInt);

    if (StringHelper::Compare(ioBuffer1.Buffer(), ioBuffer2.Buffer()) != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Stream() {

    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);
    DummySingleBufferedStream stream;
    stream.SetBufferSize(32);

    const char8 * toWrite = "HelloWorldThisIsATest";
    uint32 writeSize = StringHelper::Length(toWrite);
    stream.Write(toWrite, writeSize);
    stream.FlushAndResync();
    stream.Seek(0);

    printf("\n%s\n", stream.Buffer());

    AnyType toPrint = stream;

    ioBuffer.PrintFormattedToStream("\n%s\n", &toPrint);

    printf("\n%s\n", ioBuffer.Buffer());

    return StringHelper::Compare(ioBuffer.Buffer(), stream.Buffer()) == 0;

}

bool IOBufferTest::TestPrintFormattedToStream_BitSet_Unsigned() {

    uint64 data[5] = { 0x13579BDF02468ACE, 0x13579BDF02468ACE, 0x123456789ABCDEF0, 0xDEADBABEBAB00111 };
    const char streamString[] = "DEADBABEBAB00111123456789ABCDEF013579BDF02468ACE13579BDF02468ACE";
    int32 sizeStr = 63;
    uint32 dataBitSize = 256;
    IOBuffer ioBuffer;
    uint32 allocationSize = 256;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    uint32* source = (uint32*) data;

    //from size =4 to size = 64
    for (uint32 size = 4; size < 32; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (uint32 myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            TypeDescriptor td(false, UnsignedInteger, size);
            AnyType toPrint(td, myShift, source);

            ioBuffer.PrintFormattedToStream("%0x", &toPrint);
            char buffer[128];

            end = sizeStr - myShift / 4;
            beg = (end - (size / 4)) + 1;
            StringHelper::Substr(beg, end, streamString, buffer);
            //  printf("\n|%s| |%s|\n", buffer, ioBuffer.Buffer());

            if (StringHelper::Compare(buffer, ioBuffer.Buffer()) != 0) {
                printf("\n%d %d\n", myShift, size);
                return false;
            }
            Clear(ioBuffer);

            //Avoids to print shit. (it remains less than size)
            if ((dataBitSize - myShift) < (2 * size)) {
                break;
            }
        }
    }

    return true;

}

bool IOBufferTest::TestPrintFormattedToStream_BitSet_Signed() {

    int64 data[5] = { 0x13579BDF02468ACE, 0x13579BDF02468ACE, 0x123456789ABCDEF0, 0xDEADBABEBAB00111 };
    const char streamString[] = "DEADBABEBAB00111123456789ABCDEF013579BDF02468ACE13579BDF02468ACE";
    int32 sizeStr = 63;
    uint32 dataBitSize = 256;
    IOBuffer ioBuffer;
    uint32 allocationSize = 256;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    int32* source = (int32*) data;

    //from size =4 to size = 64
    for (uint32 size = 4; size < 32; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (uint32 myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            TypeDescriptor td(false, SignedInteger, size);
            AnyType toPrint(td, myShift, source);

            ioBuffer.PrintFormattedToStream("%0x", &toPrint);
            char buffer[128];

            end = sizeStr - myShift / 4;
            beg = (end - (size / 4)) + 1;
            StringHelper::Substr(beg, end, streamString, buffer);

            //the number is negative
            if (buffer[0] > ('0' + 7)) {
                uint8 numberSize = 2;
                uint32 index = 0;
                char8 prefix[32];
                while (numberSize < (size / 4)) {
                    numberSize *= 2;
                }
                for (uint32 k = index; k < (numberSize - (size / 4)); k++) {
                    prefix[k] = 'F';
                    index++;
                }
                prefix[index] = '\0';
                char result[32];
                StringHelper::Concatenate(prefix, buffer, result);
                StringHelper::Copy(buffer, result);
            }

            //     printf("\n|%s| |%s|\n", buffer, ioBuffer.Buffer());

            if (StringHelper::Compare(buffer, ioBuffer.Buffer()) != 0) {
                printf("\n%d %d\n", myShift, size);
                return false;
            }
            Clear(ioBuffer);

            //Avoids to print shit. (it remains less than size)
            if ((dataBitSize - myShift) < (2 * size)) {
                break;
            }
        }
    }

    return true;
}

bool IOBufferTest::TestGetTokenFromStream_ConstCharOutput() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);
    DummySingleBufferedStream stream;
    stream.SetBufferSize(32);

    const char8 * toWrite = "HelloWorldThisIsATest";
    uint32 writeSize = StringHelper::Length(toWrite);
    StringHelper::Copy(stream.Buffer(), toWrite);
    stream.Write(toWrite, writeSize);
    stream.FlushAndResync();
    stream.Seek(0);

    //TODO
    return true;

}

bool IOBufferTest::TestGetTokenFromStream_IOBufferOutput() {
    //TODO
    return true;
}

bool IOBufferTest::TestSkipTokenInStream() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    const char8* toWrite = "HelloWorld:This.:Is..A:.:Test.";

    Clear(ioBuffer);
    uint32 writeSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, writeSize);
    ioBuffer.Seek(0);

    //count=0
    ioBuffer.SkipTokensInStream(0u, ".:");
    if (ioBuffer.Position() != 0) {
        return false;
    }

    ioBuffer.SkipTokensInStream(1u, ".:");

    if (ioBuffer.Position() != 11) {
        return false;
    }

    // if there is no data between two terminators they are considered as one
    ioBuffer.SkipTokensInStream(2u, ".:");

    if (ioBuffer.Position() != 20) {
        return false;
    }

    ioBuffer.SkipTokensInStream(1u, ".");

    if (ioBuffer.Position() != 24) {
        return false;
    }

    //the end of the filled memory
    ioBuffer.SkipTokensInStream(1u, ":");
    if (ioBuffer.Position() != 31) {
        return false;
    }

    ioBuffer.Seek(0);
    ioBuffer.SkipTokensInStream(3u, ".:");

    return (ioBuffer.Position() == 20);
}

bool IOBufferTest::TestSkipTokenInStream_NULL_Terminator() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;

    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    const char8* toWrite = "HelloWorld:This.:Is..A:.:Test.";

    Clear(ioBuffer);
    uint32 writeSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, writeSize);
    ioBuffer.Seek(0);

    ioBuffer.SkipTokensInStream(1u, NULL);

    // returns the end
    return (ioBuffer.Position() == 31);
}
