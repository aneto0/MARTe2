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

bool IOBufferTest::TestSetBufferHeapMemory(uint32 size,
                                           uint32 endSpace,
                                           uint32 granularity) {

    const char8* initialMemory = "Untouchable";

    IOBuffer ioBuffer(granularity, 0u);
    ioBuffer.SetBufferReadOnlyReferencedMemory(initialMemory, 10, 0);
    ioBuffer.SetUsedSize(10);
    ioBuffer.Seek(3);

    ioBuffer.SetBufferHeapMemory(size, endSpace);

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

    ioBuffer.Write(string, writeSize);

    uint32 compareSize = (allocationSize > writeSize) ? (writeSize) : (allocationSize);

    return StringHelper::CompareN(ioBuffer.Buffer(), string, compareSize) == 0;
}

bool IOBufferTest::TestWrite_Memoryreference() {

    IOBuffer ioBuffer;

    const uint32 size = 32;
    char8 bufferIn[size];
    ioBuffer.SetBufferReferencedMemory(bufferIn,size, 0);

    const char8* string = "HelloWorld";
    uint32 writeSize = StringHelper::Length(string);
    ioBuffer.Write(string, writeSize);

    if (StringHelper::CompareN(ioBuffer.Buffer(), string, writeSize) != 0) {
        return false;
    }

    writeSize -= 5;

    ioBuffer.Write(string, writeSize);

    return StringHelper::CompareN(ioBuffer.Buffer(), string, writeSize) == 0;
}

bool IOBufferTest::TestWrite_MemoryreferenceRO() {

    IOBuffer ioBuffer;

    const uint32 size = 32;
    const char8 *bufferIn="HelloWorld";
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn,size, 0);

    const char8* string = "HelloWorld";
    uint32 writeSize = StringHelper::Length(string);
    return !ioBuffer.Write(string, writeSize);
}
