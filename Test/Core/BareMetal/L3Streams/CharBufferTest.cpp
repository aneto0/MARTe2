/**
 * @file CharBufferTest.cpp
 * @brief Source file for class CharBufferTest
 * @date 08/10/2015
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
 * the class CharBufferTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "CharBufferTest.h"
#include "DynamicCString.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool CharBufferTest::TestDefaultConstructor() {
    CharBuffer myCharBuffer;
    if (myCharBuffer.Size() != 0u) {
        return false;
    }
    if ((myCharBuffer.Buffer() != NULL) || (myCharBuffer.BufferReference() != NULL)) {
        return false;
    }

    if (myCharBuffer.CanWrite()) {
        return false;
    }

    if (myCharBuffer.IsAllocated()) {
        return false;
    }

    return myCharBuffer.AllocationGranularity() == 1u;
}

bool CharBufferTest::TestGranularityConstructor(uint32 allocationGranularity,
                                                uint32 expected) {

    CharBuffer myCharBuffer(allocationGranularity);
    if (myCharBuffer.Size() != 0u) {
        return false;
    }
    if ((myCharBuffer.Buffer() != NULL) || (myCharBuffer.BufferReference() != NULL)) {
        return false;
    }

    if (myCharBuffer.CanWrite()) {
        return false;
    }

    if (myCharBuffer.IsAllocated()) {
        return false;
    }

    return myCharBuffer.AllocationGranularity() == expected;
}

bool CharBufferTest::TestDestructor() {

    CharBuffer myCharBuffer;
    myCharBuffer.SetBufferSize(1);

    myCharBuffer.~CharBuffer();
    if (myCharBuffer.Size() != 0u) {
        return false;
    }
    if ((myCharBuffer.Buffer() != NULL) || (myCharBuffer.BufferReference() != NULL)) {
        return false;
    }

    if (myCharBuffer.CanWrite()) {
        return false;
    }

    return (!myCharBuffer.IsAllocated());
}

bool CharBufferTest::TestSetBufferSize(uint32 size,
                                       uint32 granularity,
                                       bool expected) {

    CharBuffer myCharBuffer(granularity);

    granularity = myCharBuffer.AllocationGranularity();
    uint32 addChunk = ((size % granularity) != 0);
    bool ret = myCharBuffer.SetBufferSize(size);
    if (ret) {
        if (myCharBuffer.Size() != ((size / granularity) + addChunk) * granularity) {
            return false;
        }
        if (!myCharBuffer.CanWrite()) {
            return false;
        }
        if (!myCharBuffer.IsAllocated()) {
            return false;
        }
    }

    return (ret == expected);
}

bool CharBufferTest::TestSetBufferReferenceConst(const char8* bufferIn, uint32 size) {

    uint32 inputSize = CCString(bufferIn).GetSize();
    CharBuffer myCharBuffer;

    myCharBuffer.SetBufferReference(bufferIn, size);

    if (myCharBuffer.Size() != size) {
        return false;
    }

    if (myCharBuffer.IsAllocated()) {
        return false;
    }

    if (myCharBuffer.CanWrite()) {
        return false;
    }
    if (size > inputSize) {
        size = inputSize;
    }

    return (CCString(bufferIn).CompareContent(myCharBuffer.Buffer(), size) == 0);

}

bool CharBufferTest::TestSetBufferReferenceNonConst(const char8* bufferIn, uint32 size) {

    DynamicCString buffer(bufferIn);

    uint32 inputSize = CCString(bufferIn).GetSize();
    CharBuffer myCharBuffer;

    myCharBuffer.SetBufferReference(buffer.GetList(), size);

    if (myCharBuffer.Size() != size) {
        return false;
    }

    if (myCharBuffer.IsAllocated()) {
        return false;
    }

    if (!myCharBuffer.CanWrite()) {
        return false;
    }
    if (size > inputSize) {
        size = inputSize;
    }

    return (CCString(bufferIn).CompareContent(myCharBuffer.Buffer(), size) == 0);

}

bool CharBufferTest::TestBuffer() {
    CharBuffer myCharBuffer;

    if (myCharBuffer.Buffer() != NULL) {
        return false;
    }

    DynamicCString bufferIn("HelloWorld");
    myCharBuffer.SetBufferReference(bufferIn.GetList(), 1u);

    return (bufferIn == myCharBuffer.Buffer());
}

bool CharBufferTest::TestBufferReference() {
    CharBuffer myCharBuffer;

    if (myCharBuffer.Buffer() != NULL) {
        return false;
    }

    DynamicCString bufferIn("HelloWorld");
    myCharBuffer.SetBufferReference(bufferIn.GetList(), 1u);

    //write access
    char8 *ret = myCharBuffer.BufferReference();

    bufferIn().Append('!');
    return (CCString(ret) == "HelloWorld!");

}

bool CharBufferTest::TestSize_StaticAllocation(uint32 size) {
    CharBuffer myCharBuffer;

    if (myCharBuffer.Size() != 0) {
        return false;
    }

    myCharBuffer.SetBufferReference("", size);

    return myCharBuffer.Size() == size;
}

bool CharBufferTest::TestSize_DynamicAllocation(uint32 size,
                                                uint32 granularity) {
    CharBuffer myCharBuffer(granularity);

    if (myCharBuffer.Size() != 0) {
        return false;
    }
    granularity = myCharBuffer.AllocationGranularity();
    uint32 addChunk = ((size % granularity) != 0);

    myCharBuffer.SetBufferSize(size);

    return (myCharBuffer.Size() == ((size / granularity) + addChunk) * granularity);

}

bool CharBufferTest::TestIsAllocated() {
    CharBuffer myCharBuffer;

    if (myCharBuffer.IsAllocated()) {
        return false;
    }

    const char8 *bufferIn = "HelloWorld";

    myCharBuffer.SetBufferReference(bufferIn, 10);

    if (myCharBuffer.IsAllocated()) {
        return false;
    }

    myCharBuffer.SetBufferSize(10);

    return (myCharBuffer.IsAllocated()) && (CCString(bufferIn) == "HelloWorld") ;

}

bool CharBufferTest::TestAllocationGranularity(uint32 allocationGranularity,
                                               uint32 expected) {
    CharBuffer myCharBuffer(allocationGranularity);

    return myCharBuffer.AllocationGranularity() == expected;

}

bool CharBufferTest::TestCanWrite() {
    CharBuffer myCharBuffer;

    if (myCharBuffer.CanWrite()) {
        return false;
    }

    myCharBuffer.SetBufferSize(10);

    if (!myCharBuffer.CanWrite()) {
        return false;
    }

    const char8 *bufferIn = "HelloWorld";

    myCharBuffer.SetBufferReference(bufferIn, 10);

    if (myCharBuffer.CanWrite()) {
        return false;
    }

    DynamicCString bufferIn2(bufferIn);

    myCharBuffer.SetBufferReference(bufferIn2.GetList(), 10);

    if (!myCharBuffer.CanWrite()) {
        return false;
    }

    return (CCString(bufferIn) ==  "HelloWorld") ;

}
