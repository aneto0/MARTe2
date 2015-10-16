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
    ioBuffer.SetUsedSize(desiredSize);

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

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfDecTable[i].format, "") != 0) {
        Clear(ioBuffer);

        ioBuffer.PrintFormattedToStream(printfDecTable[i].format, printfDecTable[i].inputs);
        if (StringHelper::Compare(printfDecTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            AnyType data = printfDecTable[i].inputs[i];
            printf("\n%s %s %d %d\n", ioBuffer.Buffer(), printfDecTable[i].expectedResult, i, *((uint8*) data.GetDataPointer()));

            return false;
        }
        i++;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Integer_Hexadecimal() {

    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfHexTable[i].format, "") != 0) {
        Clear(ioBuffer);

        ioBuffer.PrintFormattedToStream(printfHexTable[i].format, printfHexTable[i].inputs);
        if (StringHelper::Compare(printfHexTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfHexTable[i].expectedResult, i);

            return false;
        }
        i++;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Integer_Octal() {

    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfOctTable[i].format, "") != 0) {
        Clear(ioBuffer);

        ioBuffer.PrintFormattedToStream(printfOctTable[i].format, printfOctTable[i].inputs);
        if (StringHelper::Compare(printfOctTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfOctTable[i].expectedResult, i);

            return false;
        }
        i++;
    }
    return true;

}

bool IOBufferTest::TestPrintFormattedToStream_Integer_Binary() {

    IOBuffer ioBuffer;

    uint32 allocationGranularity = 128;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfBinTable[i].format, "") != 0) {
        Clear(ioBuffer);

        ioBuffer.PrintFormattedToStream(printfBinTable[i].format, printfBinTable[i].inputs);
        if (StringHelper::Compare(printfBinTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfBinTable[i].expectedResult, i);

            return false;
        }
        i++;
    }
    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_FixedPoint() {

    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfFloatFPTable[i].format, "") != 0) {
        Clear(ioBuffer);
        ioBuffer.PrintFormattedToStream(printfFloatFPTable[i].format, printfFloatFPTable[i].inputs);
        if (StringHelper::Compare(printfFloatFPTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfFloatFPTable[i].expectedResult, i);
            return false;
        }
        i++;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_FixedRelativePoint() {
    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfFloatFPRTable[i].format, "") != 0) {
        Clear(ioBuffer);
        ioBuffer.PrintFormattedToStream(printfFloatFPRTable[i].format, printfFloatFPRTable[i].inputs);
        if (StringHelper::Compare(printfFloatFPRTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfFloatFPRTable[i].expectedResult, i);
            return false;
        }
        i++;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_Exponential() {
    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfFloatExpTable[i].format, "") != 0) {
        Clear(ioBuffer);
        ioBuffer.PrintFormattedToStream(printfFloatExpTable[i].format, printfFloatExpTable[i].inputs);
        if (StringHelper::Compare(printfFloatExpTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfFloatExpTable[i].expectedResult, i);
            return false;
        }
        i++;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_Engeneering() {
    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfFloatEngTable[i].format, "") != 0) {
        Clear(ioBuffer);
        ioBuffer.PrintFormattedToStream(printfFloatEngTable[i].format, printfFloatEngTable[i].inputs);
        if (StringHelper::Compare(printfFloatEngTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfFloatEngTable[i].expectedResult, i);
            return false;
        }
        i++;
    }

    return true;
}

bool IOBufferTest::TestPrintFormattedToStream_Float_Smart() {

    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfFloatSmartTable[i].format, "") != 0) {
        Clear(ioBuffer);

        ioBuffer.PrintFormattedToStream(printfFloatSmartTable[i].format, printfFloatSmartTable[i].inputs);
        if (StringHelper::Compare(printfFloatSmartTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfFloatSmartTable[i].expectedResult, i);

            return false;
        }
        i++;
    }

    return true;

}

bool IOBufferTest::TestPrintFormattedToStream_Float_Compact() {
    IOBuffer ioBuffer;

    uint32 allocationGranularity = 64;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(printfFloatCompactTable[i].format, "") != 0) {
        Clear(ioBuffer);

        ioBuffer.PrintFormattedToStream(printfFloatCompactTable[i].format, printfFloatCompactTable[i].inputs);
        if (StringHelper::Compare(printfFloatCompactTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
            printf("\n%s %s %d\n", ioBuffer.Buffer(), printfFloatCompactTable[i].expectedResult, i);

            return false;
        }
        i++;
    }

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

    AnyType toPrint = stream;

    ioBuffer.PrintFormattedToStream("%s", &toPrint);

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


bool IOBufferTest::TestPrintFormattedMultiple(){
    IOBuffer ioBuffer;

     uint32 allocationGranularity = 64;

     ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

     uint32 i = 0;
     while (StringHelper::Compare(printfGenericTable[i].format, "") != 0) {
         Clear(ioBuffer);
         ioBuffer.PrintFormattedToStream(printfGenericTable[i].format, printfGenericTable[i].inputs);
         if (StringHelper::Compare(printfGenericTable[i].expectedResult, ioBuffer.Buffer()) != 0) {
             printf("\n%s %s %d\n", ioBuffer.Buffer(), printfGenericTable[i].expectedResult, i);
             return false;
         }
         i++;
     }

     return true;
}

bool IOBufferTest::TestGetTokenFromStream_ConstCharOutput() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    const char8 * toWrite = "HelloWorld::This...:.Is:.ATest";
    uint32 writeSize = StringHelper::Length(toWrite) + 1;

    ioBuffer.Write(toWrite, writeSize);
    ioBuffer.Seek(0);

    const uint32 outSize = 32;
    char8 outBuffer[outSize];

    char8 terminator;
    ioBuffer.GetTokenFromStream(outBuffer, ":.", outSize, terminator, "");

    printf("\n%s\n", outBuffer);
    if (StringHelper::Compare(outBuffer, "HelloWorld") != 0) {
        return false;
    }
    if (terminator != ':') {
        return false;
    }

    //if the skipchars is null consecutive terminators are skipped
    ioBuffer.GetTokenFromStream(outBuffer, ":.", outSize, terminator, NULL);

    printf("\n%s\n", outBuffer);
    if (StringHelper::Compare(outBuffer, "This") != 0) {
        return false;
    }
    if (terminator != '.') {
        return false;
    }

    uint32 positionTest = ioBuffer.Position() + 3;
    //if the skip chars is empty, none will be skipped
    ioBuffer.GetTokenFromStream(outBuffer, ":.", outSize, terminator, ".");

    printf("\n%s\n", outBuffer);
    if ((StringHelper::Compare(outBuffer, "") != 0) || (ioBuffer.Position() != positionTest)) {
        return false;
    }
    if (terminator != ':') {
        return false;
    }
    //skip only the dot
    ioBuffer.GetTokenFromStream(outBuffer, ":.", outSize, terminator, "");

    printf("\n%s\n", outBuffer);
    if (StringHelper::Compare(outBuffer, "") != 0) {
        return false;
    }
    if (terminator != '.') {
        return false;
    }
    //if the skip a token putting it in the skip chars list
    ioBuffer.GetTokenFromStream(outBuffer, ":.", outSize, terminator, "Is.:");

    printf("\n%s\n", outBuffer);
    if (StringHelper::Compare(outBuffer, "ATet") != 0) {
        return false;
    }
    if (terminator != '\0') {
        return false;
    }

    ioBuffer.Seek(0);
    ioBuffer.GetTokenFromStream(outBuffer, "()", outSize, terminator, NULL);
    return StringHelper::Compare(outBuffer, toWrite) == 0;

}

bool IOBufferTest::TestGetTokenFromStream_IOBufferOutput() {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    const char8 * toWrite = "HelloWorld::This...:.Is:.ATest";
    uint32 writeSize = StringHelper::Length(toWrite) + 1;

    ioBuffer.Write(toWrite, writeSize);
    ioBuffer.Seek(0);

    const uint32 outSize = 32;
    IOBuffer outBuffer;
    outBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(outBuffer);

    char8 terminator;
    ioBuffer.GetTokenFromStream(outBuffer, ":.", terminator, "");

    if (StringHelper::Compare(outBuffer.Buffer(), "HelloWorld") != 0) {
        return false;
    }
    if (terminator != ':') {
        return false;
    }
    Clear(outBuffer);

    //if the skipchars is null consecutive terminators are skipped
    ioBuffer.GetTokenFromStream(outBuffer, ":.", terminator, NULL);

    if (StringHelper::Compare(outBuffer.Buffer(), "This") != 0) {
        return false;
    }
    if (terminator != '.') {
        return false;
    }
    Clear(outBuffer);

    uint32 positionTest = ioBuffer.Position() + 3;
    //if the skip chars is empty, none will be skipped
    ioBuffer.GetTokenFromStream(outBuffer, ":.", terminator, ".");

    if ((StringHelper::Compare(outBuffer.Buffer(), "") != 0) || (ioBuffer.Position() != positionTest)) {
        return false;
    }
    if (terminator != ':') {
        return false;
    }

    Clear(outBuffer);

    //skip only the dot
    ioBuffer.GetTokenFromStream(outBuffer, ":.", terminator, "");

    if (StringHelper::Compare(outBuffer.Buffer(), "") != 0) {
        return false;
    }
    if (terminator != '.') {
        return false;
    }

    Clear(outBuffer);

    //if the skip a token putting it in the skip chars list
    ioBuffer.GetTokenFromStream(outBuffer, ":.", terminator, "Is.:");

    if (StringHelper::Compare(outBuffer.Buffer(), "ATet") != 0) {
        return false;
    }
    if (terminator != '\0') {
        return false;
    }
    Clear(outBuffer);

    ioBuffer.Seek(0);
    ioBuffer.GetTokenFromStream(outBuffer, "()", terminator, NULL);
    return StringHelper::Compare(outBuffer.Buffer(), toWrite) == 0;
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
