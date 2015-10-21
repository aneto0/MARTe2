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
#include "StreamString.h"
#include "MemoryOperationsHelper.h"
#include "AnyType.h"
#include "stdio.h"
#include "OperatingSystemStream.h"

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
 * @brief Minimal OperatingSystemStream implementation for the Buffer and Stream tests.
 * It is implemented over a char buffer with dimension MAX_STREAM_DIMENSION
 */
class DummyOSStream: public OperatingSystemStream {

public:

    /**
     * Default constructor.
     * CanSeek() = canSeek
     * CanRead() = canRead
     * CanWrite() = canWrite
     */
    DummyOSStream(bool canSeek = true, bool canRead = true, bool canWrite = true) {
        position = 0;
        seekable = canSeek;
        readable = canRead;
        writable = canWrite;
        size = 0;
        buffer = (char8 *)malloc(MAX_STREAM_DIMENSION);
        for (uint32 i = 0; i < MAX_STREAM_DIMENSION; i++) {
            buffer[i] = 0;
        }
    }

    ~DummyOSStream(){
        free(buffer);
    }

    uint64 OSSize() {
        return size;
    }

    bool OSSeek(uint64 seek) {
        position = seek;
        return true;
    }

    bool OSRelativeSeek(int32 delta) {
        position += delta;
        return true;
    }

    uint64 OSPosition() {
        return position;
    }

    bool OSSetSize(uint64 desSize) {
        size = desSize;
        return true;
    }

    bool SetFakeSize(uint64 desSize) {
        size = desSize;
        return true;
    }

    bool OSRead(char8 * const outBuffer,
                        uint32 &inSize,
                        const TimeoutType &timeout) {


        if ((position + inSize) >= MAX_STREAM_DIMENSION) {
            inSize = MAX_STREAM_DIMENSION - position - 1;
        }

        if (!MemoryOperationsHelper::Copy(outBuffer, &buffer[position], inSize)) {
            return false;
        }

        position += inSize;
        return true;

    }

    bool OSWrite(const char8 * const inBuffer,
                         uint32 &outSize,
                         const TimeoutType &timeout) {


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

    bool OSCanWrite() const {
        return writable;
    }

    bool OSCanSeek() const {
        return seekable;
    }

    bool OSCanRead() const {
        return readable;
    }

    bool OSCanBlock() {
        return false;
    }

    bool OSSetBlocking(bool flag) {
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

    uint32 position;

    uint32 size;

    bool seekable;

    bool readable;

    bool writable;

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


    DummySingleBufferedStream(bool canSeek=true, bool canRead = true, bool canWrite = true) :
            DummyOSStream(canSeek, canRead, canWrite),
            SingleBufferedStream() {
    }

    virtual ~DummySingleBufferedStream() {
    }

    uint64 OSSize() {
        return DummyOSStream::OSSize();
    }

    bool OSSeek(uint64 seek) {
        return DummyOSStream::OSSeek(seek);
    }

    bool OSRelativeSeek(int32 delta) {
        return DummyOSStream::OSRelativeSeek(delta);
    }

    uint64 OSPosition() {
        return DummyOSStream::OSPosition();
    }

    bool OSSetSize(uint64 desSize) {
        return DummyOSStream::OSSetSize(desSize);
    }

    bool OSRead(char8 * const outBuffer,
                        uint32 &inSize) {
        return DummyOSStream::OSRead(outBuffer, inSize, GetTimeout());
    }

    bool OSWrite(const char8 * const inBuffer,
                         uint32 &outSize) {

        return DummyOSStream::OSWrite(inBuffer, outSize, GetTimeout());
    }

    bool CanWrite() const {
        return DummyOSStream::OSCanWrite();
    }

    bool CanSeek() const {
        return DummyOSStream::OSCanSeek();
    }

    bool CanRead() const {
        return DummyOSStream::OSCanRead();
    }

    bool CanBlock() {
        return DummyOSStream::OSCanBlock();
    }

    bool SetBlocking(bool flag) {
        return DummyOSStream::OSSetBlocking(flag);
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


    DummyDoubleBufferedStream(bool canSeek=true, bool canRead = true, bool canWrite = true) :
            DummyOSStream(canSeek, canRead, canWrite),
            DoubleBufferedStream() {
    }

    virtual ~DummyDoubleBufferedStream() {
    }

    uint64 OSSize() {
        return DummyOSStream::OSSize();
    }

    bool OSSeek(uint64 seek) {
        return DummyOSStream::OSSeek(seek);
    }

    bool OSRelativeSeek(int32 delta) {
        return DummyOSStream::OSRelativeSeek(delta);
    }

    uint64 OSPosition() {
        return DummyOSStream::OSPosition();
    }

    bool OSSetSize(uint64 desSize) {
        return DummyOSStream::OSSetSize(desSize);
    }

    bool OSRead(char8 * const outBuffer,
                        uint32 &inSize) {
        return DummyOSStream::OSRead(outBuffer, inSize, GetTimeout());
    }

    bool OSWrite(const char8 * const inBuffer,
                         uint32 &outSize) {

        return DummyOSStream::OSWrite(inBuffer, outSize, GetTimeout());
    }

    bool CanWrite() const {
        return DummyOSStream::OSCanWrite();
    }

    bool CanSeek() const {
        return DummyOSStream::OSCanSeek();
    }

    bool CanRead() const {
        return DummyOSStream::OSCanRead();
    }

    bool CanBlock() {
        return DummyOSStream::OSCanBlock();
    }

    bool SetBlocking(bool flag) {
        return DummyOSStream::OSSetBlocking(flag);
    }

};

static const uint32 numberOfIntegers = 32;
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
const TokenTestTableRow TokenTestTableTerminators[] = {
        {"This.is.a.test",     "",   "",        NULL, {"This.is.a.test"}},
        {"This.is.a.test",     ".",  "...",     NULL, {"This", "is", "a", "test"}},
        {".This.is.a.test",    ".",  "....",    NULL, {"This", "is", "a", "test"}},
        {".This.is.a.test.",   ".",  ".....",   NULL, {"This", "is", "a", "test"}},
        {".This...is.a.test.", ".",  ".......", NULL, {"This", "is", "a", "test"}},
        {"This.is:a.test",     ".:", ".:.",     NULL, {"This", "is", "a", "test"}},
        {"This.is.a.test:",    ".:", "...:",    NULL, {"This", "is", "a", "test"}},
        {NULL}
};

/**
 * Table focused on testing the correct function of the skip characters
 */
const TokenTestTableRow TokenTestTableSkipCharacters[] = {
        {"This.is.a.test",     "",   "",        "",     {"This.is.a.test"}},
        {"This.is.a.test",     ".",  "...",     "",     {"This", "is", "a", "test"}},
        {".This.is.a.test",    ".",  "....",    "",     {"", "This", "is", "a", "test"}},
        {".This.is.a.test.",   ".",  ".....",   "",     {"", "This", "is", "a", "test", ""}},
        {".This...is.a.test.", ".",  ".......", "",     {"", "This", "", "", "is", "a", "test", ""}},
        {"This.is:a.test",     ".:", ".:.",     "",     {"This", "is", "a", "test"}},
        {"This.is.a.test:",    ".:", "...:",    "",     {"This", "is", "a", "test", ""}},
        {"This.is.a.test",     "",   "",        ".",    {"Thisisatest"}},
        {"This.is.a.test",     ".",  "...",     ".",    {"This", "is", "a", "test"}},
        {".This.is.a.test",    ".",  "....",    ".",    {"This", "is", "a", "test"}},
        {".This.is.a.test.",   ".",  ".....",   ".",    {"This", "is", "a", "test"}},
        {".This...is.a.test.", ".",  ".......", ".",    {"This", "is", "a", "test"}},
        {"This.is:a.test",     ".:", ".:.",     ".",    {"This", "is", "a", "test"}},
        {"This.is.a.test:",    ".:", "...:",    ".",    {"This", "is", "a", "test", ""}},
        {"This.is.a.test:",    ".:", "...:",    ":.",   {"This", "is", "a", "test"}},
        {"This.is.a.test:",    ".:", "..:",    ":.a",   {"This", "is", "test"}},
        {"This.is.a.test:",    ".:", "..:",    ":i.a",  {"Ths", "s", "test"}},
        {"This.is.a.test:",    ".:", ".:",    ":.hisa", {"T", "tet"}},
        {NULL}
};


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
const SkipTokensTestTableRow SkipTokensTestTable[] = {
        {"This.is.a.test",     "",   0,   {"This.is.a.test", NULL}},
        {"This.is.a.test",     "",   1,   {NULL}},
        {"This.is.a.test",     ".",  0,   {"This", "is", "a", "test", NULL}},
        {"This.is.a.test",     ".",  1,   {"is", "a", "test", NULL}},
        {"This.is.a.test",     ".",  2,   {"a", "test", NULL}},
        {"This.is.a.test",     ".",  3,   {"test", NULL}},
        {"This.is.a.test",     ".",  4,   {NULL}},
        {"...This.is.a.test",  ".",  2,   {"a", "test", NULL}},
        {"This..:..is.a.test", ".",  2,   {"is", "a", "test", NULL}},
        {"This..:..is.a.test", ".:", 2,   {"a", "test", NULL}},
        {"This.:.:.is.a.test", ".:", 2,   {"a", "test", NULL}},
        {NULL}
};






struct BitSetToBitSetTableRow{
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
