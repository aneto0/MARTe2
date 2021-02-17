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

#include "ClassRegistryItemT.h"
#include "IOBufferTest.h"
#include "IntrospectionT.h"
#include "IntrospectionEntry.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

struct TestIOBufferIntrospectionNestedStructure {
    uint32 nestedMember1;
    const char8 *nestedMember2;
};

struct TestIOBufferIntrospectionStructure {
    uint32 member1;
    float32 *member2;
    float64 member3[32];
    const char8 * member4[2][2];
    TestIOBufferIntrospectionNestedStructure member5;
};

DECLARE_CLASS_MEMBER(TestIOBufferIntrospectionNestedStructure, nestedMember1, uint32, "", "");
DECLARE_CLASS_MEMBER(TestIOBufferIntrospectionNestedStructure, nestedMember2, string, "", "");

static const IntrospectionEntry* nestedFields[] = { &TestIOBufferIntrospectionNestedStructure_nestedMember1_introspectionEntry,
        &TestIOBufferIntrospectionNestedStructure_nestedMember2_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIOBufferIntrospectionNestedStructure, nestedFields);
//INTROSPECTION_REGISTER(TestIOBufferIntrospectionNestedStructure, "1.0", TestIOBufferIntrospectionNestedStructure_introspection);

static IntrospectionEntry member1Field("member1", "uint32", "", "", INTROSPECTION_MEMBER_SIZE(TestIOBufferIntrospectionStructure, member1),
                                       INTROSPECTION_MEMBER_INDEX(TestIOBufferIntrospectionStructure, member1));

DECLARE_CLASS_MEMBER(TestIOBufferIntrospectionStructure, member2, float32, "*", "");

DECLARE_CLASS_MEMBER(TestIOBufferIntrospectionStructure, member3, float64, "[32]", "");

DECLARE_CLASS_MEMBER(TestIOBufferIntrospectionStructure, member4, string, "[2][2]", "");

DECLARE_CLASS_MEMBER(TestIOBufferIntrospectionStructure, member5, TestIOBufferIntrospectionNestedStructure, "", "");

static const IntrospectionEntry* fields[] = { &member1Field, &TestIOBufferIntrospectionStructure_member2_introspectionEntry,
        &TestIOBufferIntrospectionStructure_member3_introspectionEntry, &TestIOBufferIntrospectionStructure_member4_introspectionEntry,
        &TestIOBufferIntrospectionStructure_member5_introspectionEntry, 0 };

DECLARE_STRUCT_INTROSPECTION(TestIOBufferIntrospectionStructure, fields);
//INTROSPECTION_REGISTER(TestIOBufferIntrospectionStructure, "1.0", TestIOBufferIntrospectionStructure_introspection);

class TestIOBufferNotIntrospectable: public Object {
public:
    CLASS_REGISTER_DECLARATION()uint32 member1;
    float32 *member2;
    float64 member3[32];
    const char8 * member4[2][2];
};

CLASS_REGISTER(TestIOBufferNotIntrospectable, "1.0")


char8 * new_str(const char string[]) {
    uint32 length = sizeof(&string) / sizeof(char);
    char8 * str = new char8[length];
    for (uint32 i = 0; i < length; i++){
        str[i] = static_cast<char8>(string[i]);
    } 
    return str;
}

//static ClassProperties TestIOBufferNotIntrospectable_prop("TestIOBufferNotIntrospectable", "TestIOBufferNotIntrospectable", "1.0");
//static ClassRegistryItem TestIOBufferNotIntrospectable_item(TestIOBufferNotIntrospectable_prop, (ObjectBuildFn*) NULL);

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

    if (ioBuffer.GetBufferSize() != 0) {
        return false;
    }
    granularity = ioBuffer.AllocationGranularity();

    uint32 addChunk = ((size % granularity) != 0);

    ioBuffer.SetBufferHeapMemory(size, 0u);

    return (ioBuffer.GetBufferSize() == ((size / granularity) + addChunk) * granularity);
}

bool IOBufferTest::TestMaxUsableAmount(uint32 size,
                                       uint32 endSpace,
                                       uint32 granularity) {

    IOBuffer ioBuffer(granularity, 0u);

    if (ioBuffer.GetBufferSize() != 0) {
        return false;
    }
    ioBuffer.SetBufferHeapMemory(size, endSpace);

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

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

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

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

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

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
            return false;
        }
    }

    ioBuffer.Seek(0);

    //the getC change the position
    for (uint32 i = 0; i < size; i++) {
        char c;
        ioBuffer.GetC(c);

        if (ioBuffer.Position() != (i + 1)) {
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
        return false;
    }

    ioBuffer.Seek(0);

    //the read change the position

    char8 toRead[32];

    ioBuffer.Read(toRead, expectedPosition);
    if (ioBuffer.Position() != expectedPosition) {
        return false;
    }

    //the seek change the position
    ioBuffer.Seek(3);
    if (ioBuffer.Position() != 3) {
        return false;
    }

    //the relative seek change the position
    ioBuffer.RelativeSeek(2);
    if (ioBuffer.Position() != 5) {
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

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

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

    size = ioBuffer.GetBufferSize();
    uint32 expected = (endSpace > size) ? (0) : (size - endSpace);

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
        return false;
    }

    char8 buffer[16];
    StringHelper::Copy(buffer, "Hello");
    uint32 fillSize = 16;

    ioBuffer.Write(buffer, fillSize);

    if (ioBuffer.UsedSize() != fillSize) {
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        return false;
    }

    //increase the size
    uint32 increase = 32;
    size += increase;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != fillSize) {
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        return false;
    }

    //increase also the reserved space to clip the used size
    endSpace += (increase + fillSize + 2);

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    uint32 expectedUsedSize = fillSize - 2;
    if (ioBuffer.UsedSize() != expectedUsedSize) {
        return false;
    }
    if (ioBuffer.Position() != expectedUsedSize) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        return false;
    }

    if (ioBuffer.GetBufferSize() != size) {
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
        return false;
    }

    char8 buffer[16];
    StringHelper::Copy(buffer, "Hello");
    uint32 fillSize = 16;

    ioBuffer.Write(buffer, fillSize);

    if (ioBuffer.UsedSize() != fillSize) {
        return false;
    }
    if (ioBuffer.Position() != fillSize) {
        return false;
    }

    //increase the size
    uint32 clip = 2;
    size = fillSize - clip;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != (size - endSpace)) {
        return false;
    }
    if (ioBuffer.Position() != (size - endSpace)) {
        return false;
    }

    //clip again the used size using the space at end
    clip = 1;
    endSpace += clip;

    ioBuffer.SetBufferHeapMemory(size, endSpace);

    if (ioBuffer.UsedSize() != (size - endSpace)) {
        return false;
    }
    if (ioBuffer.Position() != (size - endSpace)) {
        return false;
    }

    if (ioBuffer.MaxUsableAmount() != (size - endSpace)) {
        return false;
    }

    if (ioBuffer.GetBufferSize() != size) {
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

    if (ioBuffer.GetBufferSize() != size) {
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

    ioBuffer.SetUsedSize(1);
    //clip by endSpace
    endSpace += 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
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

    ioBuffer.SetUsedSize(1);
    //clip by size
    size -= 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReadOnlyReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
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

    if (ioBuffer.GetBufferSize() != size) {
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
    ioBuffer.PutC('a');
    //clip by endSpace
    endSpace += 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
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

    ioBuffer.PutC('a');
    //clip by size
    size -= 3;

    // the set by memory reference should empties the buffer
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, endSpace);

    if (ioBuffer.GetBufferSize() != size) {
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
            if (ioBuffer.Position() != (uint32)(usedSize)) {
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

    char8 ret  = '\0';
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

    char8 ret = '\0';
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

    char8 ret = '\0';
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
        return false;
    }

    const char8 *toWrite = "HelloWorld";
    uint32 usedSize = StringHelper::Length(toWrite);
    ioBuffer.Write(toWrite, usedSize);

    for (uint32 i = 0; i < usedSize; i++) {
        if (!ioBuffer.UnPutC()) {
            return false;
        }
        if (ioBuffer.UsedSize() != (usedSize - i - 1)) {
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

    char8 toRead[64];

    ioBuffer.Read(toRead, size);
    for (uint32 i = 0; i < size; i++) {
        if (!ioBuffer.UnGetC()) {
            return false;
        }
        if (ioBuffer.Position() != (size - i - 1)) {
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

bool IOBufferTest::TestWrite_Memoryreference(const char8 *string,
                                             uint32 writeSize) {

    IOBuffer ioBuffer;

    const uint32 size = 8;
    char8 bufferIn[size];
    ioBuffer.SetBufferReferencedMemory(bufferIn, size, 0);

    if (!ioBuffer.Write(string, writeSize)) {
        return false;
    }

    uint32 compareSize = (size > writeSize) ? (writeSize) : (size);

    if (ioBuffer.UsedSize() != compareSize) {
        return false;
    }

    return StringHelper::CompareN(ioBuffer.Buffer(), string, compareSize) == 0;
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

void IOBufferTest::Clear(IOBuffer &ioBuffer) {
    ioBuffer.Seek(0);
    for (uint32 i = 0; i < ioBuffer.MaxUsableAmount(); i++) {
        ioBuffer.PutC(0);
    }
    ioBuffer.Seek(0);
}

bool IOBufferTest::TestPrintFormatted(uint32 allocationGranularity,
                                      const PrintfNode testTable[]) {

    IOBuffer ioBuffer;

    ioBuffer.SetBufferHeapMemory(allocationGranularity, 0);

    uint32 i = 0;
    while (StringHelper::Compare(testTable[i].format, "") != 0) {
        Clear(ioBuffer);

        ioBuffer.PrintFormatted(testTable[i].format, testTable[i].inputs);
        if (StringHelper::CompareN(testTable[i].expectedResult, ioBuffer.Buffer(), ioBuffer.MaxUsableAmount()) != 0) {
            printf("\r\n|%s| |%s|\r\n", testTable[i].expectedResult, ioBuffer.Buffer());
            return false;
        }
        i++;
    }
    return true;

}

bool IOBufferTest::TestPrintFormatted_CCString() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    uint32 i = 0;
    while (printfCStringTable[i][0] != NULL) {
        Clear(ioBuffer);
        AnyType toPrint = printfCStringTable[i][1];
        if (ioBuffer.PrintFormatted(printfCStringTable[i][0], &toPrint)) {
            if (StringHelper::Compare(ioBuffer.Buffer(), printfCStringTable[i][2]) != 0) {
                return false;
            }
        }
        else {
            return printfCStringTable[i][1] == NULL;
        }
        i++;
    }

    return true;

}

bool IOBufferTest::TestPrintFormatted_Pointer() {

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
    AnyType toPrintUInt64 = UIntPointer;
    ioBuffer1.PrintFormatted("%x", &toPrintChar);
    ioBuffer2.PrintFormatted("%x", &toPrintUInt64);

    if (StringHelper::Compare(ioBuffer1.Buffer(), ioBuffer2.Buffer()) != 0) {
        return false;
    }

    void* pointer = (void*) charPointer;
    AnyType toPrintPointer = pointer;

    Clear(ioBuffer1);
    ioBuffer1.PrintFormatted("%f", &toPrintPointer);

    if (StringHelper::Compare(ioBuffer1.Buffer(), ioBuffer2.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer1);
    Clear(ioBuffer2);
    //%p format as the complete 32 bit pointer with header

    if (sizeof(void*) == 8) {
        ioBuffer1.PrintFormatted("%p", &toPrintPointer);
        ioBuffer2.PrintFormatted("% #0x", &toPrintUInt64);
    }
    if (sizeof(void*) == 4) {
        AnyType toPrintUInt32 = (uint32) UIntPointer;
        ioBuffer1.PrintFormatted("%p", &toPrintPointer);
        ioBuffer2.PrintFormatted("% #0x", &toPrintUInt32);
    }

    if (StringHelper::Compare(ioBuffer1.Buffer(), ioBuffer2.Buffer()) != 0) {
        return false;
    }

    Clear(ioBuffer1);
    ioBuffer1.PrintFormatted("%?", &toPrintPointer);

    if (StringHelper::Compare(ioBuffer1.Buffer(), "Pointer") != 0) {
        return false;
    }

    return true;
}

bool IOBufferTest::TestPrintFormatted_Stream() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    DummySingleBufferedStream stream;
    stream.SetBufferSize(32);

    uint32 i = 0;
    while (printfStreamTable[i][0] != NULL) {
        Clear(ioBuffer);
        stream.Clear();
        const char8 * toWrite = printfStreamTable[i][1];

        uint32 writeSize = StringHelper::Length(toWrite);
        stream.Write(toWrite, writeSize);
        stream.FlushAndResync();
        stream.Seek(0);

        AnyType toPrint = stream;
        ioBuffer.PrintFormatted(printfStreamTable[i][0], &toPrint);
        if (StringHelper::Compare(ioBuffer.Buffer(), printfStreamTable[i][2]) != 0) {
            printf("|%s| |%s|", ioBuffer.Buffer(), printfStreamTable[i][2]);
            return false;
        }
        stream.FlushAndResync();
        i++;
    }

    return true;

}

bool IOBufferTest::TestPrintFormatted_TooBigStream() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    DummySingleBufferedStream stream;
    stream.SetBufferSize(32);

    stream.SetFakeSize(10001);

    AnyType toPrint = stream;
    ioBuffer.PrintFormatted("%s", &toPrint);

    return StringHelper::Compare(ioBuffer.Buffer(), "!! too big > 10000 characters!!") == 0;
}

bool IOBufferTest::TestPrintFormatted_Stream_NotSeekable() {

    IOBuffer ioBuffer;
    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    DummySingleBufferedStream stream(false);
    stream.SetBufferSize(32);

    AnyType toPrint = stream;
    ioBuffer.PrintFormatted("%s", &toPrint);

    return StringHelper::Compare(ioBuffer.Buffer(), "!!stream !seek!!") == 0;
}

bool IOBufferTest::TestPrintFormatted_BitSet_Unsigned() {

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
    for (uint32 size = 4; size < 64; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (uint32 myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            TypeDescriptor td(false, UnsignedInteger, size);
            AnyType toPrint(td, myShift, source);

            ioBuffer.PrintFormatted("%0x", &toPrint);
            char buffer[128];

            end = sizeStr - myShift / 4;
            beg = (end - (size / 4)) + 1;
            StringHelper::Substr(beg, end, streamString, buffer);
            //  printf("\r\n|%s| |%s|\r\n", buffer, ioBuffer.Buffer());

            if (StringHelper::Compare(buffer, ioBuffer.Buffer()) != 0) {
                //printf("\r\n%d %d\r\n", myShift, size);
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

bool IOBufferTest::TestPrintFormatted_BitSet_Signed() {

    int64 data[5] = { (int64) 0x13579BDF02468ACE, (int64) 0x13579BDF02468ACE, (int64) 0x123456789ABCDEF0, (int64) 0xDEADBABEBAB00111 };
    const char streamString[] = "DEADBABEBAB00111123456789ABCDEF013579BDF02468ACE13579BDF02468ACE";
    int32 sizeStr = 63;
    uint32 dataBitSize = 256;
    IOBuffer ioBuffer;
    uint32 allocationSize = 256;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    int32* source = (int32*) data;

    //from size =4 to size = 64
    for (uint32 size = 4; size < 64; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (uint32 myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            TypeDescriptor td(false, SignedInteger, size);
            AnyType toPrint(td, myShift, source);

            ioBuffer.PrintFormatted("%0x", &toPrint);
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

            //     printf("\r\n|%s| |%s|\r\n", buffer, ioBuffer.Buffer());

            if (StringHelper::Compare(buffer, ioBuffer.Buffer()) != 0) {
                //printf("\r\n%d %d\r\n", myShift, size);
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

bool IOBufferTest::TestGetToken_ConstCharOutput(const TokenTestTableRow *table) {

    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    uint32 i = 0u;
    const TokenTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        Clear(ioBuffer);
        uint32 inputSize = StringHelper::Length(row->toTokenize) + 1;
        ioBuffer.Write(row->toTokenize, inputSize);
        ioBuffer.Seek(0);
        const uint32 bufferSize = 32;
        char buffer[bufferSize];
        char saveTerminator;
        uint32 t = 0u;

        while (ioBuffer.GetToken(buffer, row->terminators, bufferSize, saveTerminator, row->skipCharacters)) {
            if (StringHelper::Compare(buffer, row->expectedResult[t]) != 0) {
                result = false;
            }
            if (row->saveTerminatorResult[t] != saveTerminator) {
                //When it gets to the end of the string the terminator is \0
                if (saveTerminator != '\0') {
                    result = false;
                }
            }
            t++;
        }

        row = &table[++i];

    }
    return result;

}

bool IOBufferTest::GetToken_ConstCharOutput_ClipSize() {

    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);
    const char8 * input = "HelloWorl.d";
    uint32 inputSize = StringHelper::Length(input) + 1;
    ioBuffer.Write(input, inputSize);
    ioBuffer.Seek(0);

    const uint32 outSize = 8;
    char8 outBuffer[outSize];
    char8 saveTerminator;
    ioBuffer.GetToken(outBuffer, ".", outSize, saveTerminator, "");
    return StringHelper::Compare(outBuffer, "HelloWo") == 0;
}

bool IOBufferTest::TestGetToken_IOBufferOutput(const TokenTestTableRow *table) {
    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    IOBuffer outBuffer;
    outBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(outBuffer);

    uint32 i = 0u;
    const TokenTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        Clear(ioBuffer);
        uint32 inputSize = StringHelper::Length(row->toTokenize) + 1;
        ioBuffer.Write(row->toTokenize, inputSize);
        ioBuffer.Seek(0);

        char saveTerminator;
        uint32 t = 0u;

        while (ioBuffer.GetToken(outBuffer, row->terminators, saveTerminator, row->skipCharacters)) {

            if (StringHelper::Compare(outBuffer.Buffer(), row->expectedResult[t]) != 0) {
                result = false;
            }
            if (row->saveTerminatorResult[t] != saveTerminator) {
                //When it gets to the end of the string the terminator is \0
                if (saveTerminator != '\0') {
                    result = false;
                }
            }
            Clear(outBuffer);

            t++;
        }

        row = &table[++i];

    }
    return result;

}

bool IOBufferTest::TestSkipToken(const SkipTokensTestTableRow *table) {

    IOBuffer ioBuffer;

    uint32 allocationSize = 64;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);

    Clear(ioBuffer);

    uint32 i = 0u;
    const SkipTokensTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        Clear(ioBuffer);
        uint32 inputSize = StringHelper::Length(row->toTokenize) + 1;
        ioBuffer.Write(row->toTokenize, inputSize);
        ioBuffer.Seek(0);
        ioBuffer.SkipTokens(row->nOfSkipTokens, row->terminators);

        const uint32 bufferSize = 32;
        char buffer[bufferSize];
        uint32 t = 0u;
        while (row->expectedResult[t] != NULL) {
            char saveTerminator;
            ioBuffer.GetToken(buffer, row->terminators, bufferSize, saveTerminator, NULL);
            if (StringHelper::Compare(buffer, row->expectedResult[t]) != 0) {
                result = false;
            }
            t++;
        }
        row = &table[++i];

    }
    return result;

}

bool IOBufferTest::TestPrintTooMuchDimensions() {

    int32 x[1][1][1] = { { { 1 } } };
    AnyType at(SignedInteger32Bit, 0u, &x);

    at.SetNumberOfDimensions(3);
    IOBuffer ioBuffer;
    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    return !ioBuffer.PrintFormatted("%d", &at);
}

bool IOBufferTest::TestPrintFormattedIntrospection() {
    TestIOBufferIntrospectionStructure myStruct; 
    myStruct.member2 = new float32(0);
    for (uint32 i = 0; i < 32; i++) {
        myStruct.member3[i] = static_cast<float32>(i);
    }
    for (uint32 i = 0; i < 2; i++) {
        for (uint32 j = 0; j < 2; j++) {
            myStruct.member4[i][j] = new char8('0');
        }
    }
    myStruct.member5.nestedMember1 = 0;
    myStruct.member5.nestedMember2 = new_str("hello world");
    TypeDescriptor myType(false, ClassRegistryDatabase::Instance()->Find("TestIOBufferIntrospectionStructure")->GetClassProperties()->GetUniqueIdentifier());
    AnyType at(myType, 0, (void*) &myStruct);
    IOBuffer ioBuffer;
    uint32 allocationSize = 1200;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);
    ioBuffer.PrintFormatted("%?", &at);
    const char8* test = "\r\nTestIOBufferIntrospectionStructure = {\r\n"
            "    member1 = {\r\n"
            "        type = \"uint32\"\r\n"
            "        modifiers = \"\"\r\n"
            "        attributes = \"\"\r\n"
            "    }\r\n"
            "    member2 = {\r\n"
            "        type = \"float32\"\r\n"
            "        modifiers = \"*\"\r\n"
            "        attributes = \"\"\r\n"
            "    }\r\n"
            "    member3 = {\r\n"
            "        type = \"float64\"\r\n"
            "        modifiers = \"[32]\"\r\n"
            "        attributes = \"\"\r\n"
            "    }\r\n"
            "    member4 = {\r\n"
            "        type = \"string\"\r\n"
            "        modifiers = \"[2][2]\"\r\n"
            "        attributes = \"\"\r\n"
            "    }\r\n"
            "    member5 = {\r\n"
            "        type = \"TestIOBufferIntrospectionNestedStructure\"\r\n"
            "        modifiers = \"\"\r\n"
            "        attributes = \"\"\r\n"
            "    }\r\n"
            "}\r\n";

    printf("\n|%s||%s|\n", ioBuffer.Buffer(), test);
    bool ret = (StringHelper::Compare(ioBuffer.Buffer(), test) == 0);
    if (ret) {
        Clear(ioBuffer);
        ioBuffer.Seek(0);
        ioBuffer.PrintFormatted("%J?", &at);

        const char8* test1 = "\r\n\"TestIOBufferIntrospectionStructure\": {\r\n"
                "    \"member1\": {\r\n"
                "        \"type\": \"uint32\",\r\n"
                "        \"modifiers\": \"\",\r\n"
                "        \"attributes\": \"\"\r\n"
                "    },\r\n"
                "    \"member2\": {\r\n"
                "        \"type\": \"float32\",\r\n"
                "        \"modifiers\": \"*\",\r\n"
                "        \"attributes\": \"\"\r\n"
                "    },\r\n"
                "    \"member3\": {\r\n"
                "        \"type\": \"float64\",\r\n"
                "        \"modifiers\": \"[32]\",\r\n"
                "        \"attributes\": \"\"\r\n"
                "    },\r\n"
                "    \"member4\": {\r\n"
                "        \"type\": \"string\",\r\n"
                "        \"modifiers\": \"[2][2]\",\r\n"
                "        \"attributes\": \"\"\r\n"
                "    },\r\n"
                "    \"member5\": {\r\n"
                "        \"type\": \"TestIOBufferIntrospectionNestedStructure\",\r\n"
                "        \"modifiers\": \"\",\r\n"
                "        \"attributes\": \"\"\r\n"
                "    }\r\n"
                "}\r\n";
        printf("\n|%s||%s|\n", ioBuffer.Buffer(), test1);
        ret = (StringHelper::Compare(ioBuffer.Buffer(), test1) == 0);
    }

    if (ret) {
        Clear(ioBuffer);
        ioBuffer.Seek(0);
        ioBuffer.PrintFormatted("%X?", &at);

        const char8* test2 = "\r\n<TestIOBufferIntrospectionStructure>\r\n"
                "    <member1>\r\n"
                "        <type> \"uint32\"</type>\r\n"
                "        <modifiers> \"\"</modifiers>\r\n"
                "        <attributes> \"\"</attributes>\r\n"
                "    </member1>\r\n"
                "    <member2>\r\n"
                "        <type> \"float32\"</type>\r\n"
                "        <modifiers> \"*\"</modifiers>\r\n"
                "        <attributes> \"\"</attributes>\r\n"
                "    </member2>\r\n"
                "    <member3>\r\n"
                "        <type> \"float64\"</type>\r\n"
                "        <modifiers> \"[32]\"</modifiers>\r\n"
                "        <attributes> \"\"</attributes>\r\n"
                "    </member3>\r\n"
                "    <member4>\r\n"
                "        <type> \"string\"</type>\r\n"
                "        <modifiers> \"[2][2]\"</modifiers>\r\n"
                "        <attributes> \"\"</attributes>\r\n"
                "    </member4>\r\n"
                "    <member5>\r\n"
                "        <type> \"TestIOBufferIntrospectionNestedStructure\"</type>\r\n"
                "        <modifiers> \"\"</modifiers>\r\n"
                "        <attributes> \"\"</attributes>\r\n"
                "    </member5>\r\n"
                "</TestIOBufferIntrospectionStructure>\r\n";
        printf("\n|%s||%s|\n", ioBuffer.Buffer(), test2);
        ret = (StringHelper::Compare(ioBuffer.Buffer(), test2) == 0);
    }
    return ret;
}

bool IOBufferTest::TestPrintFormattedObject() {
    TestIOBufferIntrospectionStructure myStruct;
    myStruct.member1 = 1;
    float32 x = 2.0;
    myStruct.member2 = &x;
    for (uint32 i = 0; i < 32; i++) {
        myStruct.member3[i] = i;
    }
    myStruct.member4[0][0] = "4";
    myStruct.member4[0][1] = "5";
    myStruct.member4[1][0] = "6";
    myStruct.member4[1][1] = "7";
    myStruct.member5.nestedMember1 = 5;
    myStruct.member5.nestedMember2 = "Hello";
    TypeDescriptor myType(false, ClassRegistryDatabase::Instance()->Find("TestIOBufferIntrospectionStructure")->GetClassProperties()->GetUniqueIdentifier());
    AnyType at(myType, 0, (void*) &myStruct);
    IOBuffer ioBuffer;
    uint32 allocationSize = 1200;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);
    ioBuffer.PrintFormatted("%@", &at);

    StreamString test = "\r\nClass = \"TestIOBufferIntrospectionStructure\"\r\n"
            "member1 = 1\r\n"
            "member2 = ";
    test.Printf("%#x\r\n", ((void*) myStruct.member2));
    test +=
            "member3 = { 0 1.000000 2.000000 3.000000 4.000000 5.000000 6.000000 7.000000 8.000000 9.000000 10.000000 11.000000 12.000000 13.000000 14.000000 15.000000 16.000000 17.000000 18.000000 19.000000 20.000000 21.000000 22.000000 23.000000 24.000000 25.000000 26.000000 27.000000 28.000000 29.000000 30.000000 31.000000 } \r\n"
                    "member4 = { { \"4\" \"5\" } { \"6\" \"7\" } } \r\n"
                    "member5 = {\r\n\r\n"

                    "Class = \"TestIOBufferIntrospectionNestedStructure\"\r\n"
                    "nestedMember1 = 5\r\n"
                    "nestedMember2 = \"Hello\"\r\n"
                    "}\r\n";

    printf("\n|%s||%s|\n", ioBuffer.Buffer(), test.Buffer());
    bool ret = (StringHelper::Compare(ioBuffer.Buffer(), test.Buffer()) == 0);
    if (ret) {
        ioBuffer.SetBufferHeapMemory(allocationSize, 0);
        Clear(ioBuffer);
        ioBuffer.PrintFormatted("%J@", &at);

        StreamString test1 = "\r\n\"Class\": \"TestIOBufferIntrospectionStructure\",\r\n"
                "\"member1\": 1,\r\n"
                "\"member2\": ";
        test1.Printf("%#x,\r\n", ((void*) myStruct.member2));
        test1 +=
                "\"member3\": [ 0, 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000, 10.000000, 11.000000, 12.000000, 13.000000, 14.000000, 15.000000, 16.000000, 17.000000, 18.000000, 19.000000, 20.000000, 21.000000, 22.000000, 23.000000, 24.000000, 25.000000, 26.000000, 27.000000, 28.000000, 29.000000, 30.000000, 31.000000 ] ,\r\n"
                        "\"member4\": [ [ \"4\", \"5\" ] ,[ \"6\", \"7\" ] ] ,\r\n"
                        "\"member5\": {\r\n\r\n"

                        "\"Class\": \"TestIOBufferIntrospectionNestedStructure\",\r\n"
                        "\"nestedMember1\": 5,\r\n"
                        "\"nestedMember2\": \"Hello\"\r\n"
                        "}\r\n";
        printf("\n|%s||%s|\n", ioBuffer.Buffer(), test1.Buffer());
        ret = (StringHelper::Compare(ioBuffer.Buffer(), test1.Buffer()) == 0);

    }

    if (ret) {
        ioBuffer.SetBufferHeapMemory(allocationSize, 0);
        Clear(ioBuffer);
        ioBuffer.PrintFormatted("%X@", &at);

        StreamString test2 = "\r\n<Class> \"TestIOBufferIntrospectionStructure\"</Class>\r\n"
                "<member1> 1</member1>\r\n"
                "<member2> ";
        test2.Printf("%#x</member2>\r\n", ((void*) myStruct.member2));
        test2 +=
                "<member3> [ 0, 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000, 10.000000, 11.000000, 12.000000, 13.000000, 14.000000, 15.000000, 16.000000, 17.000000, 18.000000, 19.000000, 20.000000, 21.000000, 22.000000, 23.000000, 24.000000, 25.000000, 26.000000, 27.000000, 28.000000, 29.000000, 30.000000, 31.000000 ] </member3>\r\n"
                        "<member4> [ [ \"4\", \"5\" ] ,[ \"6\", \"7\" ] ] </member4>\r\n"
                        "<member5>\r\n\r\n"

                        "<Class> \"TestIOBufferIntrospectionNestedStructure\"</Class>\r\n"
                        "<nestedMember1> 5</nestedMember1>\r\n"
                        "<nestedMember2> \"Hello\"</nestedMember2>\r\n"
                        "</member5>\r\n";
        printf("\n|%s||%s|\n", ioBuffer.Buffer(), test2.Buffer());
        ret = (StringHelper::Compare(ioBuffer.Buffer(), test2.Buffer()) == 0);
    }

    return ret;
}

bool IOBufferTest::TestPrintStructuredDataInterface() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A.B");
    int32 one = 1, two = 2, three = 3;
    cdb.Write("x", one);
    cdb.Write("y", two);
    cdb.CreateAbsolute("A.C");
    cdb.Write("z", three);
    cdb.MoveToRoot();

    IOBuffer ioBuffer;
    uint32 allocationSize = 128;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);
    AnyType toPrint(*(StructuredDataI*) (&cdb));
    ioBuffer.PrintFormatted("%s", &toPrint);
    const char8* test = "A = {\r\n"
            "    B = {\r\n"
            "        x = 1\r\n"
            "        y = 2\r\n"
            "    }\r\n"
            "    C = {\r\n"
            "        z = 3\r\n"
            "    }\r\n"
            "}\r\n";

    printf("\r\n%s\r\n", ioBuffer.Buffer());

    bool ret = (StringHelper::Compare(test, ioBuffer.Buffer()) == 0);

    if (ret) {
        Clear(ioBuffer);
        AnyType toPrint(*(StructuredDataI*) (&cdb));
        ioBuffer.PrintFormatted("%Js", &toPrint);
        const char8* test1 = "\"A\": {\r\n"
                "    \"B\": {\r\n"
                "        \"x\": 1,\r\n"
                "        \"y\": 2\r\n"
                "    },\r\n"
                "    \"C\": {\r\n"
                "        \"z\": 3\r\n"
                "    }\r\n"
                "}\r\n";

        printf("\n|%s|\n|%s|\n", ioBuffer.Buffer(), test1);
        ret = (StringHelper::Compare(test1, ioBuffer.Buffer()) == 0);
    }

    if (ret) {
        Clear(ioBuffer);
        AnyType toPrint(*(StructuredDataI*) (&cdb));
        ioBuffer.PrintFormatted("%Xs", &toPrint);
        const char8* test2 = "<A>\r\n"
                "    <B>\r\n"
                "        <x> 1</x>\r\n"
                "        <y> 2</y>\r\n"
                "    </B>\r\n"
                "    <C>\r\n"
                "        <z> 3</z>\r\n"
                "    </C>\r\n"
                "</A>\r\n";

        printf("\n|%s|\n|%s|\n", ioBuffer.Buffer(), test2);
        ret = (StringHelper::Compare(test2, ioBuffer.Buffer()) == 0);
    }

    return ret;
}

bool IOBufferTest::TestPrintFormattedIntrospection_NotIntrospectable() {
    TestIOBufferNotIntrospectable x;
    TypeDescriptor myType(false, ClassRegistryDatabase::Instance()->Find("TestIOBufferNotIntrospectable")->GetClassProperties()->GetUniqueIdentifier());

    AnyType at(myType, 0, &x);
    IOBuffer ioBuffer;
    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    return !ioBuffer.PrintFormatted("%?", &at);
}

bool IOBufferTest::TestPrintFormattedObject_NotIntrospectable() {
    TestIOBufferNotIntrospectable x;
    TypeDescriptor myType(false, ClassRegistryDatabase::Instance()->Find("TestIOBufferNotIntrospectable")->GetClassProperties()->GetUniqueIdentifier());

    AnyType at(myType, 0, &x);
    IOBuffer ioBuffer;
    uint32 allocationSize = 32;
    ioBuffer.SetBufferHeapMemory(allocationSize, 0);
    Clear(ioBuffer);

    return !ioBuffer.PrintFormatted("%?", &at);
}

bool IOBufferTest::TestPrintPointerVector() {
    IOBuffer ioBuffer;
    uint32 allocSize = 128u;
    ioBuffer.SetBufferHeapMemory(allocSize, 0);
    IOBuffer ioBuffer2;
    ioBuffer2.SetBufferHeapMemory(allocSize, 0);

    const char8 * one = "One";
    int32 two = 2;
    float32 three = 3.0;

    void *test[] = { &one, &two, &three };
    uintp test2[] = { (uintp) & one, (uintp) & two, (uintp) & three };

    Clear(ioBuffer);
    Clear(ioBuffer2);

    AnyType toPrint(test);
    ioBuffer.PrintFormatted("%p", &toPrint);
    AnyType toPrint2(test2);
    ioBuffer2.PrintFormatted("%p", &toPrint2);

    return StringHelper::Compare(ioBuffer.Buffer(), ioBuffer2.Buffer()) == 0;
}

bool IOBufferTest::TestPrintPointerMatrix() {

    IOBuffer ioBuffer;
    uint32 allocSize = 128u;
    ioBuffer.SetBufferHeapMemory(allocSize, 0);
    IOBuffer ioBuffer2;
    ioBuffer2.SetBufferHeapMemory(allocSize, 0);

    const char8 * one = "One";
    int32 two = 2;
    float32 three = 3.0;
    uint32 four = 4;

    void *test[][2] = { { &one, &two }, { &three, &four } };
    uintp test2[][2] = { { (uintp) & one, (uintp) & two }, { (uintp) & three, (uintp) & four } };

    Clear(ioBuffer);
    Clear(ioBuffer2);

    AnyType toPrint(test);
    ioBuffer.PrintFormatted("%p", &toPrint);
    AnyType toPrint2(test2);
    ioBuffer2.PrintFormatted("%p", &toPrint2);

    return StringHelper::Compare(ioBuffer.Buffer(), ioBuffer2.Buffer()) == 0;
}

bool IOBufferTest::TestRefill() {
    IOBuffer ioBuffer;
    return !ioBuffer.Refill();
}

bool IOBufferTest::TestFlush() {
    IOBuffer ioBuffer;
    bool ret = !ioBuffer.Flush();
    if (ret) {
        ret = !ioBuffer.Flush(10u);
    }
    return ret;
}

bool IOBufferTest::TestNoMoreSpaceToWriteArg() {
    IOBuffer ioBuffer;
    return !ioBuffer.NoMoreSpaceToWrite(10u);
}

bool IOBufferTest::TestNoMoreSpaceToWrite() {
    IOBuffer ioBuffer;
    return !ioBuffer.NoMoreSpaceToWrite();
}

bool IOBufferTest::TestNoMoreDataToRead() {
    IOBuffer ioBuffer;
    return !ioBuffer.NoMoreDataToRead();
}

