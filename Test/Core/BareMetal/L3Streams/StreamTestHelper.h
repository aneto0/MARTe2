/**
 * @file StreamTestHelper.h
 * @brief Header file for class StreamTestHelper
 * @date 16/10/2015
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

 * @details This header file contains the declaration of the class StreamTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */
#ifndef STREAMTESTHELPER_H_
#define STREAMTESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DoubleBufferedStream.h"
#include "SingleBufferedStream.h"
#include "MemoryOperationsHelper.h"
#include "AnyType.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
#define MAX_STREAM_DIMENSION 2048
using namespace MARTe;

/**
 * @brief Utility class to store the test parameters for the static tables that are used
 * in the PrintFormatted tests.
 */
class PrintfNode {

public:
    AnyType inputs[4];
    const char8* expectedResult;
    const char8* format;

    PrintfNode(const char8* formatIn,
               const char8* expectedResultIn,
               const AnyType x0 = voidAnyType,
               const AnyType x1 = voidAnyType,
               const AnyType x2 = voidAnyType,
               const AnyType x3 = voidAnyType) {

        inputs[0] = x0;
        inputs[1] = x1;
        inputs[2] = x2;
        inputs[3] = x3;

        expectedResult = expectedResultIn;
        format = formatIn;

    }

};

/**
 * @brief Utility class to store the test parameters for the static tables that are used
 * in the PrintFormatted tests.
 */
template<typename T, uint32 nDims>
struct TestPrintFormattedTableVector{
    const char8* format;
    T vectorInput[nDims];
    const char8* expected;
};

/**
 * @brief Utility class to store the test parameters for the static tables that are used
 * in the PrintFormatted tests.
 */
template<typename T, uint32 nRows, uint32 nCols>
struct TestPrintFormattedTableMatrix{
    const char8* format;
    T matrixInput[nRows][nCols];
    const char8* expected;
};



/**
 * @brief Minimal StreamI implementation for the Buffer and Stream tests.
 * It is implemented over a char buffer with dimension MAX_STREAM_DIMENSION
 */
class DummyOSStream: public StreamI {

public:

    /**
     * Default constructor.
     * CanSeek() = canSeek
     * CanRead() = canRead
     * CanWrite() = canWrite
     */
    DummyOSStream(bool canSeek = true,
                  bool canRead = true,
                  bool canWrite = true) {
        position = 0;
        seekable = canSeek;
        readable = canRead;
        writable = canWrite;
        size = 0;
        usedTimeout = false;
        buffer = (char8 *) GlobalObjectsDatabase::Instance()->GetStandardHeap()->Malloc(MAX_STREAM_DIMENSION);
        for (uint32 i = 0; i < MAX_STREAM_DIMENSION; i++) {
            buffer[i] = 0;
        }
    }

    ~DummyOSStream() {
        GlobalObjectsDatabase::Instance()->GetStandardHeap()->Free(reinterpret_cast<void *&>(buffer));
    }

    uint64 Size() {
        return size;
    }

    bool Seek(uint64 seek) {
        position = seek;
        return true;
    }

    bool RelativeSeek(int64 delta) {
        position += delta;
        return true;
    }

    uint64 Position() {
        return position;
    }

    bool SetSize(uint64 desSize) {
        size = desSize;
        return true;
    }

    bool SetFakeSize(uint64 desSize) {
        size = desSize;
        return true;
    }

    bool Read(char8 * const outBuffer,
              uint32 &inSize,
              const TimeoutType &timeout) {

        if (timeout.GetTimeoutMSec() < TTDefault.GetTimeoutMSec()) {
            usedTimeout = true;
        }
        else {
            usedTimeout = false;
        }

        if ((position + inSize) >= MAX_STREAM_DIMENSION) {
            inSize = MAX_STREAM_DIMENSION - position - 1;
        }

        if (!MemoryOperationsHelper::Copy(outBuffer, &buffer[position], inSize)) {
            return false;
        }

        position += inSize;
        return true;

    }

    bool Write(const char8 * const inBuffer,
               uint32 &outSize,
               const TimeoutType &timeout) {

        if (timeout.GetTimeoutMSec() < TTDefault.GetTimeoutMSec()) {
            usedTimeout = true;
        }
        else {
            usedTimeout = false;
        }
        if ((size + outSize) >= MAX_STREAM_DIMENSION) {
            outSize = MAX_STREAM_DIMENSION - size - 1;
        }
        if (!MemoryOperationsHelper::Copy(&buffer[position], inBuffer, outSize)) {
            return false;
        }

        position += outSize;

        if (position > size) {
            size = position;
        }

        return true;
    }

    bool CanWrite() const {
        return writable;
    }

    bool CanSeek() const {
        return seekable;
    }

    bool CanRead() const {
        return readable;
    }

    bool CanBlock() {
        return false;
    }

    bool SetBlocking(bool flag) {
        return true;
    }

    void Clear() {
        position = 0;
        size = 0;
        for (uint32 i = 0; i < MAX_STREAM_DIMENSION; i++) {
            buffer[i] = 0;
        }
    }

    char8 * Buffer() {
        return buffer;
    }

    char8 *buffer;

    uint64 position;

    uint32 size;

    bool seekable;

    bool readable;

    bool writable;

    bool usedTimeout;

};

/**
 * @brief Minimal StreamI (and SingleBufferedStream) implementation for the Buffer and Stream tests.
 */
class DummySingleBufferedStream: public DummyOSStream, public SingleBufferedStream {
public:

    DummySingleBufferedStream(uint32 timeout) :
            DummyOSStream(true),
            SingleBufferedStream(timeout) {
    }

    DummySingleBufferedStream(bool canSeek = true,
                              bool canRead = true,
                              bool canWrite = true) :
            DummyOSStream(canSeek, canRead, canWrite),
            SingleBufferedStream() {
    }


    virtual ~DummySingleBufferedStream() {
    }

    uint64 OSSize() {
        return DummyOSStream::Size();
    }

    bool OSSeek(uint64 seek) {
        return DummyOSStream::Seek(seek);
    }

    bool OSRelativeSeek(int64 delta) {
        return DummyOSStream::RelativeSeek(delta);
    }

    uint64 OSPosition() {
        return DummyOSStream::Position();
    }

    bool OSSetSize(uint64 desSize) {
        return DummyOSStream::SetSize(desSize);
    }

    bool OSRead(char8 * const outBuffer,
                uint32 &inSize) {
        return DummyOSStream::Read(outBuffer, inSize, GetTimeout());
    }

    bool OSWrite(const char8 * const inBuffer,
                 uint32 &outSize) {

        return DummyOSStream::Write(inBuffer, outSize, GetTimeout());
    }

    bool Read(char8 * const outBuffer,
              uint32 &inSize) {
        return SingleBufferedStream::Read(outBuffer, inSize);
    }

    bool Write(const char8 * const outBuffer,
               uint32 &inSize) {
        return SingleBufferedStream::Write(outBuffer, inSize);
    }

    bool Read(char8 * const outBuffer,
              uint32 &inSize,
              const TimeoutType &timeout) {
        return SingleBufferedStream::Read(outBuffer, inSize, timeout);
    }

    bool Write(const char8 * const outBuffer,
               uint32 &inSize,
               const TimeoutType &timeout) {
        return SingleBufferedStream::Write(outBuffer, inSize, timeout);
    }

    uint64 Size() {
        return SingleBufferedStream::Size();
    }

    bool Seek(uint64 pos) {
        return SingleBufferedStream::Seek(pos);
    }

    bool RelativeSeek(const int64 deltaPos) {
        return SingleBufferedStream::RelativeSeek(deltaPos);
    }

    uint64 Position() {
        return SingleBufferedStream::Position();
    }

    bool SetSize(uint64 size) {
        return SingleBufferedStream::SetSize(size);
    }

    bool CanWrite() const {
        return DummyOSStream::CanWrite();
    }

    bool CanSeek() const {
        return DummyOSStream::CanSeek();
    }

    bool CanRead() const {
        return DummyOSStream::CanRead();
    }

    bool CanBlock() {
        return DummyOSStream::CanBlock();
    }

    bool SetBlocking(bool flag) {
        return DummyOSStream::SetBlocking(flag);
    }


    BufferedStreamIOBuffer *GetReadBufferX(){
        return (BufferedStreamIOBuffer *)GetReadBuffer();
    }

    BufferedStreamIOBuffer *GetWriteBufferX(){
        return (BufferedStreamIOBuffer *)GetWriteBuffer();
    }

};

/**
 * @brief Minimal DoubleBufferedStream implementation for the Buffer and Stream tests
 */
class DummyDoubleBufferedStream: public DummyOSStream, public DoubleBufferedStream {
public:

    DummyDoubleBufferedStream(uint32 timeout) :
            DummyOSStream(true),
            DoubleBufferedStream(timeout) {
    }

    DummyDoubleBufferedStream(bool canSeek = true,
                              bool canRead = true,
                              bool canWrite = true) :
            DummyOSStream(canSeek, canRead, canWrite),
            DoubleBufferedStream() {
    }

    virtual ~DummyDoubleBufferedStream() {
    }

    uint64 OSSize() {
        return DummyOSStream::Size();
    }

    bool OSSeek(uint64 seek) {
        return DummyOSStream::Seek(seek);
    }

    bool OSRelativeSeek(int64 delta) {
        return DummyOSStream::RelativeSeek(delta);
    }

    uint64 OSPosition() {
        return DummyOSStream::Position();
    }

    bool OSSetSize(uint64 desSize) {
        return DummyOSStream::SetSize(desSize);
    }

    bool OSRead(char8 * const outBuffer,
                uint32 &inSize) {
        return DummyOSStream::Read(outBuffer, inSize, GetTimeout());
    }

    bool OSWrite(const char8 * const inBuffer,
                 uint32 &outSize) {

        return DummyOSStream::Write(inBuffer, outSize, GetTimeout());
    }

    bool Read(char8 * const outBuffer,
              uint32 &inSize) {
        return DoubleBufferedStream::Read(outBuffer, inSize);
    }

    bool Write(const char8 * const outBuffer,
               uint32 &inSize) {
        return DoubleBufferedStream::Write(outBuffer, inSize);
    }

    bool Read(char8 * const outBuffer,
              uint32 &inSize,
              const TimeoutType &timeout) {
        return DoubleBufferedStream::Read(outBuffer, inSize, timeout);
    }

    bool Write(const char8 * const outBuffer,
               uint32 &inSize,
               const TimeoutType &timeout) {
        return DoubleBufferedStream::Write(outBuffer, inSize, timeout);
    }

    uint64 Size() {
        return DoubleBufferedStream::Size();
    }

    bool Seek(uint64 pos) {
        return DoubleBufferedStream::Seek(pos);
    }

    bool RelativeSeek(const int64 deltaPos) {
        return DoubleBufferedStream::RelativeSeek(deltaPos);
    }

    uint64 Position() {
        return DoubleBufferedStream::Position();
    }

    bool SetSize(uint64 size) {
        return DoubleBufferedStream::SetSize(size);
    }

    bool CanWrite() const {
        return DummyOSStream::CanWrite();
    }

    bool CanSeek() const {
        return DummyOSStream::CanSeek();
    }

    bool CanRead() const {
        return DummyOSStream::CanRead();
    }

    bool CanBlock() {
        return DummyOSStream::CanBlock();
    }

    bool SetBlocking(bool flag) {
        return DummyOSStream::SetBlocking(flag);
    }

};

static const uint32 numberOfIntegers = 64;
static const uint32 numberOfFloats = 64;

/**
 * List of functions to generate the tables for the IOBuffer and StreamI PrintFormatted tests
 */
const PrintfNode *GeneratePrintFormattedDecimalTable();
const PrintfNode *GeneratePrintFormattedHexadecimalTable();
const PrintfNode *GeneratePrintFormattedOctalTable();
const PrintfNode *GeneratePrintFormattedBinaryTable();
const PrintfNode *GeneratePrintFormattedFloatFPTable();
const PrintfNode *GeneratePrintFormattedFloatFPRTable();
const PrintfNode *GeneratePrintFormattedFloatExpTable();
const PrintfNode *GeneratePrintFormattedFloatEngTable();
const PrintfNode *GeneratePrintFormattedFloatSmartTable();
const PrintfNode *GeneratePrintFormattedFloatCompactTable();
const PrintfNode *GeneratePrintFormattedGenericTable();
extern const char8 *printfCStringTable[][3];
extern const char8 *printfStreamTable[][3];


/**
 * Each row defines a different GetToken test
 */
struct TokenTestTableRow {
    /**
     * Element to be tokenized
     */
    const char8 *toTokenize;
    /**
     * Terminator to be used
     */
    const char8 *terminators;
    /**
     * If not NULL contains the expected sequence of saved terminators
     */
    const char8 *saveTerminatorResult;
    /**
     * Characters to skip
     */
    const char8 *skipCharacters;

    /**
     * Array of strings with the expected result
     */
    const char8 * expectedResult[32];
};

/**
 * Table focused on testing the correct function of the terminators
 */
const TokenTestTableRow TokenTestTableTerminators[] = { { "This.is.a.test", "", "", NULL, { "This.is.a.test" } }, { "This.is.a.test", ".", "...", NULL, {
        "This", "is", "a", "test" } }, { ".This.is.a.test", ".", "....", NULL, { "This", "is", "a", "test" } }, { ".This.is.a.test.", ".", ".....", NULL, {
        "This", "is", "a", "test" } }, { ".This...is.a.test.", ".", ".......", NULL, { "This", "is", "a", "test" } }, { "This.is:a.test", ".:", ".:.", NULL, {
        "This", "is", "a", "test" } }, { "This.is.a.test:", ".:", "...:", NULL, { "This", "is", "a", "test" } }, { NULL } };

/**
 * Table focused on testing the correct function of the skip characters
 */
const TokenTestTableRow TokenTestTableSkipCharacters[] = { { "This.is.a.test", "", "", "", { "This.is.a.test" } }, { "This.is.a.test", ".", "...", "", { "This",
        "is", "a", "test" } }, { ".This.is.a.test", ".", "....", "", { "", "This", "is", "a", "test" } }, { ".This.is.a.test.", ".", ".....", "", { "", "This",
        "is", "a", "test", "" } }, { ".This...is.a.test.", ".", ".......", "", { "", "This", "", "", "is", "a", "test", "" } }, { "This.is:a.test", ".:", ".:.",
        "", { "This", "is", "a", "test" } }, { "This.is.a.test:", ".:", "...:", "", { "This", "is", "a", "test", "" } }, { "This.is.a.test", "", "", ".", {
        "Thisisatest" } }, { "This.is.a.test", ".", "...", ".", { "This", "is", "a", "test" } }, { ".This.is.a.test", ".", "....", ".", { "This", "is", "a",
        "test" } }, { ".This.is.a.test.", ".", ".....", ".", { "This", "is", "a", "test" } }, { ".This...is.a.test.", ".", ".......", ".", { "This", "is", "a",
        "test" } }, { "This.is:a.test", ".:", ".:.", ".", { "This", "is", "a", "test" } }, { "This.is.a.test:", ".:", "...:", ".", { "This", "is", "a", "test",
        "" } }, { "This.is.a.test:", ".:", "...:", ":.", { "This", "is", "a", "test" } }, { "This.is.a.test:", ".:", "..:", ":.a", { "This", "is", "test" } }, {
        "This.is.a.test:", ".:", "..:", ":i.a", { "Ths", "s", "test" } }, { "This.is.a.test:", ".:", ".:", ":.hisa", { "T", "tet" } }, { NULL } };

/**
 * Each row defines a different SkipTokens test
 */
struct SkipTokensTestTableRow {
    /**
     * Element to be tokenized
     */
    const char8 *toTokenize;
    /**
     * Terminator to be used
     */
    const char8 *terminators;
    /**
     * Number of tokens to skip
     */
    uint32 nOfSkipTokens;
    /**
     * Array of strings with the expected result
     */
    const char8 * expectedResult[32];
};

/**
 * Table focused on testing the correct function of the terminators
 */
const SkipTokensTestTableRow SkipTokensTestTable[] = { { "This.is.a.test", "", 0, { "This.is.a.test", NULL } }, { "This.is.a.test", "", 1, { NULL } }, {
        "This.is.a.test", ".", 0, { "This", "is", "a", "test", NULL } }, { "This.is.a.test", ".", 1, { "is", "a", "test", NULL } }, { "This.is.a.test", ".", 2,
        { "a", "test", NULL } }, { "This.is.a.test", ".", 3, { "test", NULL } }, { "This.is.a.test", ".", 4, { NULL } }, { "...This.is.a.test", ".", 2, { "a",
        "test", NULL } }, { "This..:..is.a.test", ".", 2, { "is", "a", "test", NULL } }, { "This..:..is.a.test", ".:", 2, { "a", "test", NULL } }, {
        "This.:.:.is.a.test", ".:", 2, { "a", "test", NULL } }, { NULL } };

struct BitSetToBitSetTableRow {
    uint8 destShift;
    uint8 sourceShift;
    uint8 destSize;
    uint8 sourceSize;
    uint32 *sPointer;
    uint32 destArrayIndex;
    uint32 result;
    bool sourceIsSigned;
    bool destIsSigned;
};

const BitSetToBitSetTableRow *GeneratedBitSetToBitSetTable();

#endif
