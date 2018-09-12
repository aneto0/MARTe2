/**
 * @file StreamMemoryReferenceTest.cpp
 * @brief Source file for class StreamMemoryReferenceTest
 * @date 19/10/2015
 * @author Giuseppe Ferro
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
 * the class StreamMemoryReferenceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StreamMemoryReferenceTest.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool StreamMemoryReferenceTest::TestDefaultConstructor() {
    StreamMemoryReference sMR;
    if (sMR.Position() != 0) {
        return false;
    }

    if (sMR.Size() != 0) {
        return false;
    }

    return (!sMR.CanRead()) && (!sMR.CanWrite());
}

bool StreamMemoryReferenceTest::TestConstructor_CharPointer() {
    const uint32 size = 64;
    char8 preBuffer[size];
    StreamMemoryReference sMR(preBuffer, size);
    if (sMR.Position() != 0) {
        return false;
    }

    if (sMR.Size() != 0) {
        return false;
    }

    if (sMR.Buffer() != preBuffer) {
        return false;
    }

    return (sMR.CanRead()) && (sMR.CanWrite());

}

bool StreamMemoryReferenceTest::TestConstructor_ConstCharPointer() {
    const char8 *preBuffer = "HelloWorld";
    uint32 size = 20;
    StreamMemoryReference sMR(preBuffer, size);
    if (sMR.Position() != 0) {
        return false;
    }

    if (sMR.Size() != size) {
        return false;
    }

    if (sMR.Buffer() != preBuffer) {
        return false;
    }

    return (sMR.CanRead()) && (!sMR.CanWrite());
}

bool StreamMemoryReferenceTest::TestDestructor() {

    const char8 * input = "HelloWorld";

    StreamMemoryReference sMR(input, 5);

    sMR.~StreamMemoryReference();

    return sMR.Buffer() == NULL;
}

bool StreamMemoryReferenceTest::TestAnyTypeOperator(const char8* initializationString) {

    const uint32 size = StringHelper::Length(initializationString);

    StreamMemoryReference sMR(initializationString, size);

    AnyType test = sMR;

    TypeDescriptor td = test.GetTypeDescriptor();

    if (td.isStructuredData) {
        return false;
    }

    if (!td.isConstant) {
        return false;
    }

    if (td.type != BT_CCString) {
        return false;
    }
    if (td.numberOfBits != (sizeof(const char8*) * 8)) {
        return false;
    }

    if (test.GetDataPointer() != sMR.Buffer()) {
        return false;
    }
    return test.GetBitAddress() == 0;
}

bool StreamMemoryReferenceTest::TestCanWrite() {
    StreamMemoryReference sMR;

    if (sMR.CanWrite()) {
        return false;
    }

    const char8 * input = "HelloWorld";
    StreamMemoryReference sMR1(input, 5);
    if (sMR.CanWrite()) {
        return false;
    }

    const uint32 size = 32;
    char8 bufferIn[size];
    StreamMemoryReference sMR2(bufferIn, size);

    return sMR2.CanWrite();
}

bool StreamMemoryReferenceTest::TestCanRead() {

    StreamMemoryReference sMR;

    if (sMR.CanRead()) {
        return false;
    }

    const char8 * input = "HelloWorld";
    StreamMemoryReference sMR1(input, 5);
    if (!sMR1.CanRead()) {
        return false;
    }

    const uint32 size = 32;
    char8 bufferIn[size];
    StreamMemoryReference sMR2(bufferIn, size);

    return sMR2.CanRead();
}

bool StreamMemoryReferenceTest::TestCanSeek() {
    StreamMemoryReference sMR;
    return sMR.CanSeek();
}

bool StreamMemoryReferenceTest::TestRead(const char8 *string,
                                         uint32 readSize,
                                         const uint32 bufferSize) {

    char8 bufferIn[64];
    StreamMemoryReference sMR(bufferIn, bufferSize);
    uint32 writeSize=StringHelper::Length(string);
    sMR.Write(string, writeSize);

    char8 bufferOut[64];
    sMR.Read(bufferOut, readSize);
    uint32 compareSize = (bufferSize > readSize) ? (readSize) : (bufferSize);

    return StringHelper::CompareN(bufferOut, sMR.Buffer(), compareSize) == 0;
}

bool StreamMemoryReferenceTest::TestReadTimeout(const char8 *string,
                                                uint32 readSize,
                                                const uint32 bufferSize) {

    char8 bufferIn[64];
    StreamMemoryReference sMR(bufferIn, bufferSize);
    uint32 writeSize=StringHelper::Length(string);
    sMR.Write(string, writeSize);

    char8 bufferOut[64];
    sMR.Read(bufferOut, readSize, 1);
    uint32 compareSize = (bufferSize > readSize) ? (readSize) : (bufferSize);

    return StringHelper::CompareN(bufferOut, sMR.Buffer(), compareSize) == 0;

}

bool StreamMemoryReferenceTest::TestWrite(const char8 *string,
                                          uint32 writeSize,
                                          const uint32 bufferSize) {

    char8 bufferIn[64];

    StreamMemoryReference sMR(bufferIn, bufferSize);

    if (!sMR.Write(string, writeSize)) {
        return false;
    }

    uint32 compareSize = (bufferSize > writeSize) ? (writeSize) : (bufferSize);

    if (sMR.Size() != compareSize) {
        return false;
    }

    return StringHelper::CompareN(sMR.Buffer(), string, compareSize) == 0;

}

bool StreamMemoryReferenceTest::TestWriteTimeout(const char8 *string,
                      uint32 writeSize,
                      const uint32 bufferSize) {
    char8 bufferIn[64];

    StreamMemoryReference sMR(bufferIn, bufferSize);

    if (!sMR.Write(string, writeSize, 1)) {
        return false;
    }

    uint32 compareSize = (bufferSize > writeSize) ? (writeSize) : (bufferSize);

    if (sMR.Size() != compareSize) {
        return false;
    }

    return StringHelper::CompareN(sMR.Buffer(), string, compareSize) == 0;

}

bool StreamMemoryReferenceTest::TestWrite_RO() {

    const char8 *bufferIn = "HelloWorld";
    const uint32 size = StringHelper::Length(bufferIn);

    StreamMemoryReference sMR(bufferIn, size);

    const char8* string = "Nothing";
    uint32 writeSize = StringHelper::Length(string);

    return !sMR.Write(string, writeSize);
}

bool StreamMemoryReferenceTest::TestSize_Const(const char8 *input,
                                               const uint32 bufferSize) {

    StreamMemoryReference sMRConst(input, bufferSize);

    return sMRConst.Size() == bufferSize;
}

bool StreamMemoryReferenceTest::TestSize(const char8 *input,
                                         const uint32 bufferSize) {

    char8 bufferIn[64];
    StreamMemoryReference sMR(bufferIn, bufferSize);

    uint32 inputSize = StringHelper::Length(input);

    sMR.Write(input, inputSize);

    uint32 compareSize = (inputSize > bufferSize) ? (bufferSize) : (inputSize);

    return sMR.Size() == compareSize;
}

bool StreamMemoryReferenceTest::TestSeek(const uint32 bufferSize,
                                         uint32 writeSize,
                                         uint32 seek,
                                         bool expected) {

    char8 bufferIn[64];
    StreamMemoryReference smr(bufferIn, bufferSize);
    char8 toWrite[64];
    smr.Write(toWrite, writeSize);

    uint32 testSize = (writeSize > bufferSize) ? (bufferSize) : (writeSize);

    if (smr.Position() != testSize) {
        return false;
    }

    smr.Seek(0);
    if (smr.Position() != 0) {
        return false;
    }

    bool ret = smr.Seek(seek);
    if (ret) {
        if (smr.Position() != seek) {
            return false;
        }
    }
    else {
        if (smr.Position() != testSize) {
            return false;
        }
    }

    return ret == expected;
}

bool StreamMemoryReferenceTest::TestRelativeSeek(const uint32 bufferSize,
                                                 uint32 initialPos,
                                                 int32 delta,
                                                 bool expected) {

    char8 bufferIn[64];
    StreamMemoryReference smr(bufferIn, bufferSize);

    uint32 writeSize = 2 * initialPos;

    char8 toWrite[64];
    smr.Write((const char8*) toWrite, writeSize);

    uint32 compareSize = (writeSize > bufferSize) ? (bufferSize) : (writeSize);

    smr.Seek(initialPos);

    if (smr.Position() != initialPos) {
        return false;
    }

    bool ret = smr.RelativeSeek(delta);
    if (ret) {
        if (smr.Position() != (initialPos + delta)) {
            return false;
        }
    }
    else {
        if (delta >= 0) {
            if (smr.Position() != (uint32) (compareSize)) {
                return false;
            }
        }
        else {
            if (smr.Position() != 0) {
                return false;
            }
        }
    }
    return ret == expected;

}

bool StreamMemoryReferenceTest::TestRelativeSeek_OutOfInt32Range(){

    const char8 *bufferIn = "HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH";
    const uint32 size = StringHelper::Length(bufferIn);

    StreamMemoryReference sMR(bufferIn, size);

    int64 toMuchSeek=MAX_INT32;
    toMuchSeek++;

    return !sMR.RelativeSeek(toMuchSeek);
}


bool StreamMemoryReferenceTest::TestPosition(const uint32 bufferSize,
                                             uint32 finalPos) {

    char8 bufferIn[64];

    StreamMemoryReference smr(bufferIn, bufferSize);

    if (smr.Position() != 0) {
        return false;
    }

    char8 toWrite[64];
    uint32 writeSize = finalPos;
    smr.Write(toWrite, writeSize);

    uint32 expectedPosition = (finalPos > bufferSize) ? (bufferSize) : (finalPos);

    if (smr.Position() != expectedPosition) {
        return false;
    }

    //the read change the position

    smr.Seek(0);
    char8 toRead[32];

    uint32 readSize = finalPos;
    smr.Read(toRead, readSize);
    if (smr.Position() != expectedPosition) {
        return false;
    }

    smr.Seek(0);

    //the seek change the position
    smr.Seek(finalPos);
    if (smr.Position() != expectedPosition) {
        return false;
    }

    //the relative seek change the position
    smr.Seek(0);

    smr.RelativeSeek(finalPos);
    return smr.Position() == expectedPosition;

}

bool StreamMemoryReferenceTest::TestSetSize(const uint32 bufferSize,
                                            uint32 size,
                                            bool expected) {

    char8 bufferIn[64];
    StreamMemoryReference smr(bufferIn, bufferSize);

    bool ret = smr.SetSize(size);
    uint32 compareSize = (size > bufferSize) ? (bufferSize) : (size);
    return (smr.Size() == compareSize) && (ret == expected);
}

bool StreamMemoryReferenceTest::TestPrintFormatted(uint32 bufferSize,
                                                   const PrintfNode testTable[]) {
    char8 bufferIn[64];
    StreamMemoryReference smr(bufferIn, bufferSize);

    uint32 i = 0;
    while (StringHelper::Compare(testTable[i].format, "") != 0) {
        smr.Seek(0);

        smr.PrintFormatted(testTable[i].format, testTable[i].inputs);
        uint32 termSize = 1;
        smr.Write("\0", termSize);
        if (StringHelper::CompareN(testTable[i].expectedResult, smr.Buffer(), bufferSize) != 0) {
            AnyType data = testTable[i].inputs[i];
            printf("\n%s %s %d %d\n", smr.Buffer(), testTable[i].expectedResult, i, *((uint8*) data.GetDataPointer()));
            return false;
        }
        i++;
    }
    return true;
}

bool StreamMemoryReferenceTest::TestGetToken(uint32 bufferSize,
                                             const TokenTestTableRow *table) {

    char8 inBuff[64];
    StreamMemoryReference myStream(inBuff, bufferSize);

    uint32 i = 0u;
    const TokenTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        StringHelper::Copy(myStream.BufferReference(), row->toTokenize);
        myStream.Seek(0);
        char saveTerminator;
        uint32 t = 0u;

        uint32 outBuffSize = 64;
        char8 outputBuff[64] = { 0 };

        while (myStream.GetToken(outputBuff, row->terminators, outBuffSize, saveTerminator, row->skipCharacters)) {
            if (StringHelper::CompareN(outputBuff, row->expectedResult[t], bufferSize) != 0) {
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

