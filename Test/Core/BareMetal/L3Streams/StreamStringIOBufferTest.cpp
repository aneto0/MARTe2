/**
 * @file StreamStringIOBufferTest.cpp
 * @brief Source file for class StreamStringIOBufferTest
 * @date 14/10/2015
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
 * the class StreamStringIOBufferTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StreamStringIOBufferTest.h"
#include "StringHelper.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
bool StreamStringIOBufferTest::TestDefaultConstructor() {

    StreamStringIOBuffer stringBuffer;

    if (stringBuffer.AmountLeft() != 0) {
        return false;
    }

    if (stringBuffer.UsedAmountLeft() != 0) {
        return false;
    }

    if (stringBuffer.MaxUsableAmount() != 0) {
        return false;
    }

    if (stringBuffer.Buffer() != NULL) {
        return false;
    }

    if (stringBuffer.UndoLevel() != 0) {
        return false;
    }

    return stringBuffer.AllocationGranularity() == 64;
}

bool StreamStringIOBufferTest::TestGranularityConstructor(uint32 granularity,
                                                          uint32 expected) {
    StreamStringIOBuffer stringBuffer(granularity);

    if (stringBuffer.AmountLeft() != 0) {
        return false;
    }

    if (stringBuffer.UsedAmountLeft() != 0) {
        return false;
    }

    if (stringBuffer.MaxUsableAmount() != 0) {
        return false;
    }

    if (stringBuffer.Buffer() != NULL) {
        return false;
    }

    if (stringBuffer.UndoLevel() != 0) {
        return false;
    }

    return stringBuffer.AllocationGranularity() == expected;

}

bool StreamStringIOBufferTest::TestDestructor() {
    StreamStringIOBuffer stringBuffer;
    uint32 allocationSize;
    stringBuffer.SetBufferAllocationSize(allocationSize);

    if (stringBuffer.Buffer() == NULL) {
        return false;
    }

    stringBuffer.~StreamStringIOBuffer();

    return stringBuffer.Buffer() == NULL;
}

bool StreamStringIOBufferTest::TestSetBufferAllocationSize(uint32 size,
                                                           uint32 granularity) {
    StreamStringIOBuffer stringBuffer(granularity);

    // the function allocates space for the 0 char.
    size++;

    if (stringBuffer.GetBufferSize() != 0) {
        return false;
    }
    granularity = stringBuffer.AllocationGranularity();

    uint32 addChunk = ((size % granularity) != 0);

    if (!stringBuffer.SetBufferAllocationSize(size)) {
        return false;
    }

    return (stringBuffer.GetBufferSize() == ((size / granularity) + addChunk) * granularity) && (stringBuffer.MaxUsableAmount() == (stringBuffer.GetBufferSize() - 1));
}

bool StreamStringIOBufferTest::TestWrite(uint32 writeSize,
                                         const char8 *string) {

    StreamStringIOBuffer stringBuffer;

    if (!stringBuffer.Write(string, writeSize)) {
        return false;
    }

    return StringHelper::CompareN(stringBuffer.Buffer(), string, writeSize) == 0;
}

bool StreamStringIOBufferTest::TestWrite_NULL_Buffer() {
    StreamStringIOBuffer stringBuffer;

    uint32 size = 32;
    const char8 *bufferIn = NULL;
    return !stringBuffer.Write(bufferIn, size);
}

bool StreamStringIOBufferTest::TestTerminate(uint32 writeSize,
                                             const char8 *string) {

    StreamStringIOBuffer stringBuffer;

    char8 buffer[32];

    StringHelper::Copy(buffer, string);

    if (!stringBuffer.Write(buffer, writeSize)) {
        return false;
    }

    uint32 filledSize = stringBuffer.UsedSize();
    stringBuffer.Terminate();
    if (writeSize < StringHelper::Length(string)) {
        buffer[writeSize] = '\0';
    }

    return (StringHelper::Compare(stringBuffer.Buffer(), buffer) == 0) && (filledSize == stringBuffer.UsedSize());

}


bool StreamStringIOBufferTest::TestWriteAll(uint32 writeSize,
                                            const char8 *string){
    StreamStringIOBuffer stringBuffer;

    if (!stringBuffer.WriteAll(string, writeSize)) {
        return false;
    }

    return StringHelper::CompareN(stringBuffer.Buffer(), string, writeSize) == 0;

}

