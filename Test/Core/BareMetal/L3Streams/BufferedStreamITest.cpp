/**
 * @file BufferedStreamITest.cpp
 * @brief Source file for class BufferedStreamITest
 * @date 26/10/2015
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
 * the class BufferedStreamITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BufferedStreamITest.h"
#include "StringHelper.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool BufferedStreamITest::TestDefaultConstructor() {
    DummySingleBufferedStream myStream;
    return (myStream.GetTimeout() == TTInfiniteWait);
}

bool BufferedStreamITest::TestAnyType() {
    DummySingleBufferedStream myStream;
    AnyType anyStream = myStream;
    bool ok = anyStream.GetDataPointer() == &myStream;
    ok = !anyStream.GetTypeDescriptor().isConstant;
    ok = (anyStream.GetTypeDescriptor().numberOfBits == 0u);
    return ok;
}

bool BufferedStreamITest::TestGetToken(uint32 bufferSize,
                                       const TokenTestTableRow *table) {
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);

    //Force a switch to write mode (in order to force full coverage).
    myStream.Printf("%d", 4);
    myStream.Seek(0);

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

bool BufferedStreamITest::TestGetToken_Stream(uint32 bufferSize,
                                              const TokenTestTableRow *table) {
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);

    uint32 i = 0u;
    const TokenTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        myStream.Clear();
        StringHelper::Copy(myStream.buffer, row->toTokenize);
        char saveTerminator;
        uint32 t = 0u;

        DummySingleBufferedStream outputStream;
        outputStream.SetBufferSize(bufferSize);
        outputStream.Seek(0);
        outputStream.Clear();

        while (myStream.GetToken(outputStream, row->terminators, saveTerminator, row->skipCharacters)) {
            outputStream.FlushAndResync();
            if (StringHelper::Compare(outputStream.Buffer(), row->expectedResult[t]) != 0) {
                result = false;
            }
            if (row->saveTerminatorResult[t] != saveTerminator) {
                //When it gets to the end of the string the terminator is \0
                if (saveTerminator != '\0') {
                    result = false;
                }
            }
            outputStream.Seek(0);
            outputStream.Clear();
            t++;
        }

        row = &table[++i];

    }
    return result;
}

bool BufferedStreamITest::TestSkipTokens(uint32 bufferSize,
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

bool BufferedStreamITest::TestGetLine(uint32 bufferSize,
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
    char8 buffer[128];
    uint32 i;
    for (i = 0; i < 4; i++) {
        myStream.GetLine(buffer, bufferSize, skipCharacter);
        result &= (StringHelper::Compare(buffer, lines[i]) == 0);
    }
    return result;
}

bool BufferedStreamITest::TestGetLine_Stream(uint32 bufferSize,
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
    DummySingleBufferedStream output;
    output.SetBufferSize(bufferSize);
    uint32 i;
    for (i = 0; i < 4; i++) {
        output.Clear();
        myStream.GetLine(output, skipCharacter);
        output.FlushAndResync();
        result &= (StringHelper::Compare(output.Buffer(), lines[i]) == 0);
    }
    return result;
}

bool BufferedStreamITest::TestCopy(uint32 bufferSize) {
    const char *line = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit.\nAenean commodo ligula eget dolor. "
            "Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus."
            "Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem.";
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);
    myStream.Copy(line);
    return (StringHelper::Compare(myStream.buffer, line) == 0);
}

bool BufferedStreamITest::TestCopy_Stream(uint32 bufferSize) {
    const char *line = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit.\nAenean commodo ligula eget dolor. "
            "Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus."
            "Donec quam felis, ultricies nec, pellentesque eu, pretium quis, sem.";
    DummySingleBufferedStream myStream;
    DummySingleBufferedStream source;
    myStream.SetBufferSize(bufferSize);
    source.SetBufferSize(bufferSize);
    myStream.Clear();
    source.Clear();
    StringHelper::Copy(source.buffer, line);

    myStream.Copy(source);
    myStream.FlushAndResync();
    return (StringHelper::Compare(myStream.Buffer(), line) == 0);
}

bool BufferedStreamITest::TestPrintFormatted(uint32 bufferSize,
                                             const PrintfNode testTable[]) {
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);
    uint32 i = 0;
    while (StringHelper::Compare(testTable[i].format, "") != 0) {
        myStream.Clear();

        myStream.PrintFormatted(testTable[i].format, testTable[i].inputs);
        myStream.FlushAndResync();
        if (StringHelper::Compare(testTable[i].expectedResult, myStream.Buffer()) != 0) {
            return false;
        }
        i++;
    }
    return true;
}

bool BufferedStreamITest::TestPrintFormatted_Pointer(uint32 bufferSize) {

    DummySingleBufferedStream stream1;
    DummySingleBufferedStream stream2;

    stream1.Clear();
    stream2.Clear();

    stream1.SetBufferSize(bufferSize);
    stream2.SetBufferSize(bufferSize);
    //return the pointer thanks to hex notation.

    const char *charPointer = "Hello";
    AnyType toPrintChar = charPointer;
    uint64 UIntPointer = (uint64) charPointer;
    AnyType toPrintUInt = UIntPointer;

    stream1.PrintFormatted("%x", &toPrintChar);
    stream2.PrintFormatted("%x", &toPrintUInt);

    stream1.FlushAndResync();
    stream2.FlushAndResync();
    if (StringHelper::Compare(stream1.Buffer(), stream2.Buffer()) != 0) {
        return false;
    }

    stream1.Clear();
    stream2.Clear();
    //%p format as the complete 32 bit pointer with header

    void* pointer = (void*) charPointer;
    if (sizeof(void*) == 8) {
        AnyType toPrintPointer = pointer;

        stream1.PrintFormatted("%p", &toPrintPointer);
        stream2.PrintFormatted("% #0x", &toPrintUInt);
    }
    else {
        AnyType toPrintPointer = pointer;
        AnyType toPrintUInt32 = (uint32) UIntPointer;
        stream1.PrintFormatted("%p", &toPrintPointer);
        stream2.PrintFormatted("% #0x", &toPrintUInt32);

    }
    stream1.FlushAndResync();
    stream2.FlushAndResync();
    if (StringHelper::Compare(stream1.Buffer(), stream2.Buffer()) != 0) {
        return false;
    }

    return true;
}

bool BufferedStreamITest::TestPrintf() {
    DummySingleBufferedStream myStream;
    myStream.Clear();
    myStream.Printf("%d", 8);
    myStream.FlushAndResync();
    bool result = (StringHelper::Compare("8", myStream.Buffer()) == 0);
    myStream.Clear();
    myStream.Printf("%d.%s", 8, "hello");
    myStream.FlushAndResync();
    result &= (StringHelper::Compare("8.hello", myStream.Buffer()) == 0);
    myStream.Clear();
    myStream.Printf("%d.%s", 8, "hello");
    myStream.FlushAndResync();
    result &= (StringHelper::Compare("8.hello", myStream.Buffer()) == 0);
    myStream.Clear();
    myStream.Printf("%d.%s:%d", 8, "hello", 7);
    myStream.FlushAndResync();
    result &= (StringHelper::Compare("8.hello:7", myStream.Buffer()) == 0);
    myStream.Clear();
    myStream.Printf("%d.%s:%d----%s", 8, "hello", 7, "world");
    myStream.FlushAndResync();
    result &= (StringHelper::Compare("8.hello:7----world", myStream.Buffer()) == 0);

    return result;
}

bool BufferedStreamITest::TestPrintFormatted_CCString(uint32 bufferSize) {

    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);
    myStream.Clear();

    uint32 i = 0;
    while (printfCStringTable[i][0] != NULL) {
        myStream.Clear();
        AnyType toPrint = printfCStringTable[i][1];
        if (myStream.PrintFormatted(printfCStringTable[i][0], &toPrint)) {
            myStream.FlushAndResync();
            if (StringHelper::Compare(myStream.Buffer(), printfCStringTable[i][2]) != 0) {
                printf("\n%s %s %d\n", myStream.Buffer(), printfCStringTable[i][2], i);
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

bool BufferedStreamITest::TestPrintFormatted_BitSet_Unsigned(uint32 bufferSize) {

    uint64 data[5] = { 0x13579BDF02468ACE, 0x13579BDF02468ACE, 0x123456789ABCDEF0, 0xDEADBABEBAB00111 };
    const char streamString[] = "DEADBABEBAB00111123456789ABCDEF013579BDF02468ACE13579BDF02468ACE";
    int32 sizeStr = 63;
    uint32 dataBitSize = 256;
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);
    myStream.Clear();

    uint32* source = (uint32*) data;

    //from size =4 to size = 64
    for (uint32 size = 4; size < 64; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (uint32 myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            TypeDescriptor td(false, UnsignedInteger, size);
            AnyType toPrint(td, myShift, source);

            myStream.Clear();
            myStream.PrintFormatted("%0x", &toPrint);
            myStream.FlushAndResync();
            char buffer[128];

            end = sizeStr - myShift / 4;
            beg = (end - (size / 4)) + 1;
            StringHelper::Substr(beg, end, streamString, buffer);
            //  printf("\n|%s| |%s|\n", buffer, ioBuffer.Buffer());

            if (StringHelper::Compare(buffer, myStream.Buffer()) != 0) {
                printf("\n%d %d\n", myShift, size);
                return false;
            }

            //Avoids to print shit. (it remains less than size)
            if ((dataBitSize - myShift) < (2 * size)) {
                break;
            }
        }
    }

    return true;

}

bool BufferedStreamITest::TestPrintFormatted_BitSet_Signed(uint32 bufferSize) {

    int64 data[5] = { (int64) 0x13579BDF02468ACE, (int64) 0x13579BDF02468ACE, (int64) 0x123456789ABCDEF0, (int64) 0xDEADBABEBAB00111 };
    const char streamString[] = "DEADBABEBAB00111123456789ABCDEF013579BDF02468ACE13579BDF02468ACE";
    int32 sizeStr = 63;
    uint32 dataBitSize = 256;
    DummySingleBufferedStream myStream;
    myStream.SetBufferSize(bufferSize);
    myStream.Clear();

    int32* source = (int32*) data;

    //from size =4 to size = 64
    for (uint32 size = 4; size < 64; size += 4) {
        int32 beg = 0;
        int32 end = 0;

        for (uint32 myShift = 0; myShift < dataBitSize; myShift += size) {
            //source and shift are automatically modified by the function.
            TypeDescriptor td(false, SignedInteger, size);
            AnyType toPrint(td, myShift, source);

            myStream.Clear();
            myStream.PrintFormatted("%0x", &toPrint);
            myStream.FlushAndResync();
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

            if (StringHelper::Compare(buffer, myStream.Buffer()) != 0) {
                printf("\n%d %d\n", myShift, size);
                return false;
            }

            //Avoids to print shit. (it remains less than size)
            if ((dataBitSize - myShift) < (2 * size)) {
                break;
            }
        }
    }

    return true;
}

bool BufferedStreamITest::TestFlush() {

    DummySingleBufferedStream test;
    //force to use buffer
    test.SetCalibWriteParam(0);

    uint32 buffSize = 8u;
    test.SetBufferSize(buffSize);
    const char8 *str = "HelloWorld";
    uint32 size = StringHelper::Length(str);
    test.Write(str, size);

    bool ret = StringHelper::CompareN(test.buffer, str, buffSize) == 0;
    ret &= StringHelper::Length(test.buffer) == buffSize;
    if (ret) {
        test.Flush();
        ret = StringHelper::Compare(test.buffer, str) == 0;
    }

    return ret;
}

bool BufferedStreamITest::TestRefill() {
    DummySingleBufferedStream test;
    //force to use buffer
    test.SetCalibReadParam(0);

    uint32 buffSize = 8u;
    test.SetBufferSize(buffSize);
    const char8 *str = "HelloWorld";
    uint32 strSize = StringHelper::Length(str);
    MemoryOperationsHelper::Copy(test.buffer, str, strSize);

    uint32 size = 5u;

    char8 outBuff[32];
    MemoryOperationsHelper::Set(outBuff, '\0', 32);
    test.Read(outBuff, size);

    bool ret = StringHelper::CompareN(test.buffer, outBuff, size) == 0;
    ret &= test.position == buffSize;
    if (ret) {
        test.Refill();
        test.Read(outBuff, size);
        ret = StringHelper::CompareN(test.buffer + buffSize, outBuff, size) == 0;
    }

    return ret;
}

bool BufferedStreamITest::TestSetCalibReadParam() {

    DummySingleBufferedStream test;

    //force to use buffer
    test.SetCalibReadParam(2u);

    uint32 buffSize = 8u;
    test.SetBufferSize(buffSize);
    const char8 *str = "HelloWorld";
    uint32 size = StringHelper::Length(str);
    MemoryOperationsHelper::Copy(test.buffer, str, size);

    char8 outBuff[32];
    MemoryOperationsHelper::Set(outBuff, 0, 32);
    uint32 sizeToRead = 5;

    test.Read(outBuff, sizeToRead);

    //read directly from the stream
    bool ret = test.position == sizeToRead;

    if (ret) {
        test.Seek(0);
        MemoryOperationsHelper::Set(outBuff, 0, 32);

        //force to use buffer
        test.SetCalibReadParam(0);
        test.Read(outBuff, sizeToRead);
        //use the buffer!
        ret = test.position == buffSize;
    }

    return ret;

}

bool BufferedStreamITest::TestSetCalibWriteParam() {
    DummySingleBufferedStream test;

    //force to use buffer
    test.SetCalibWriteParam(2u);

    uint32 buffSize = 8u;
    test.SetBufferSize(buffSize);
    const char8 *str = "HelloWorld";
    uint32 size = StringHelper::Length(str);
    test.Write(str, size);

    bool ret = StringHelper::Compare(test.buffer, str) == 0;

    if (ret) {
        test.Seek(0);
        MemoryOperationsHelper::Set(test.buffer, 0, size);

        //force to use buffer
        test.SetCalibWriteParam(0);
        test.Write(str, size);

        bool ret = StringHelper::CompareN(test.buffer, str, buffSize) == 0;
        ret &= StringHelper::Length(test.buffer) == buffSize;
        if (ret) {
            test.Flush();
            ret = StringHelper::Compare(test.buffer, str) == 0;
        }
    }

    return ret;

}
