/**
 * @file BufferedStreamTest.cpp
 * @brief Source file for class BufferedStreamTest
 * @date 15/10/2015
 * @author Andre' Neto
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
 * the class BufferedStreamTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamTest.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool BufferedStreamTest::TestDefaultConstructor() {
    DummySingleBufferedStream myStream;
    return (myStream.GetTimeout() == TTInfiniteWait);
}

bool BufferedStreamTest::TestAnyType() {
    DummySingleBufferedStream myStream;
    AnyType anyStream = myStream;
    bool ok = anyStream.GetDataPointer() == &myStream;
    ok = !anyStream.GetTypeDescriptor().isConstant;
    ok = (anyStream.GetTypeDescriptor().numberOfBits == 0u);
    return ok;
}

bool BufferedStreamTest::TestGetTimeout() const {
    TimeoutType tt = 1;
    DummySingleBufferedStream myStream;
    myStream.SetTimeout(tt);
    return (myStream.GetTimeout() == tt);
}

bool BufferedStreamTest::TestSetTimeout() {
    TimeoutType tt = 1;
    DummySingleBufferedStream myStream;
    myStream.SetTimeout(tt);
    return (myStream.GetTimeout() == tt);
}

bool BufferedStreamTest::TestGetToken(uint32 bufferSize,
                                      const TokenTestTableRow *table) {
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);

    uint32 i = 0u;
    const TokenTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        myStream.Clear();
        StringHelper::Copy(myStream.buffer, row->toTokenize);
        const uint32 bufferSize = 32;
        char buffer[bufferSize];
        char saveTerminator;
        uint32 t = 0u;

        while (myStream.GetToken(buffer, row->terminators, bufferSize, saveTerminator, row->skipCharacters)) {
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

bool BufferedStreamTest::TestSkipTokens(uint32 bufferSize,
                                        const SkipTokensTestTableRow *table) {
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);

    uint32 i = 0u;
    const SkipTokensTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        myStream.Clear();
        StringHelper::Copy(myStream.buffer, row->toTokenize);
        myStream.SkipTokens(row->nOfSkipTokens, row->terminators);

        const uint32 bufferSize = 32;
        char buffer[bufferSize];
        uint32 t = 0u;
        while (row->expectedResult[t] != NULL) {
            char saveTerminator;
            myStream.GetToken(buffer, row->terminators, bufferSize, saveTerminator, NULL);
            if (StringHelper::Compare(buffer, row->expectedResult[t]) != 0) {
                result = false;
            }
            t++;
        }
        row = &table[++i];

    }
    return result;
}

bool BufferedStreamTest::TestGetLine(uint32 bufferSize,
                                     bool skipCharacter) {
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);
    bool result = true;
    const char8 *lines[] = { "Lorem ipsum dolor sit amet, consectetuer adipiscing elit.", " Aenean commodo ligula eget dolor.",
            "Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.",
            "Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem.", };
    char8 line[512];
    line[0] = '\0';

    StringHelper::Concatenate(line, lines[0]);
    if (skipCharacter) {
        StringHelper::Concatenate(line, "\r");
    }
    StringHelper::Concatenate(line, "\n");
    StringHelper::Concatenate(line, lines[1]);
    if (skipCharacter) {
        StringHelper::Concatenate(line, "\r");
    }
    StringHelper::Concatenate(line, "\n");
    StringHelper::Concatenate(line, lines[2]);
    if (skipCharacter) {
        StringHelper::Concatenate(line, "\r");
    }
    StringHelper::Concatenate(line, "\n");
    StringHelper::Concatenate(line, lines[3]);

    StringHelper::Copy(myStream.buffer, line);

    bufferSize = 128;
    char8 buffer[bufferSize];
    uint32 i;
    for (i = 0; i < 4; i++) {
        myStream.GetLine(buffer, bufferSize, skipCharacter);
        result &= (StringHelper::Compare(buffer, lines[i]) == 0);
    }
    return result;
}

