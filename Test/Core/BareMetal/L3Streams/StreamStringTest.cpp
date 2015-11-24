/**
 * @file StreamStringTest.cpp
 * @brief Source file for class StreamStringTest
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
 * the class StreamStringTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StreamStringTest.h"

#include "GeneralDefinitions.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
static void cleanOutputBuffer(char8* buffer,
                              int32 size) {
    for (int32 i = 0; i < size; i++) {
        buffer[i] = 0;
    }
}

bool StreamStringTest::TestDefaultConstructor() {
    StreamString string;

    if (string.Position() != 0) {
        return false;
    }

    if (string.Size() != 0) {
        return false;
    }

    if (string.Buffer() == NULL) {
        return false;
    }

    return (string.CanWrite()) && (string.CanRead()) && (string.CanSeek());
}

bool TestConstructor_CCString(const char8 * initializationString) {
    StreamString string(initializationString);

    uint32 size = StringHelper::Length(initializationString);

    if (string.Position() != size) {
        return false;
    }

    if (string.Size() != size) {
        return false;
    }

    if (StringHelper::Compare(string.Buffer(), initializationString) != 0) {

        return false;
    }

    return (string.CanWrite()) && (string.CanRead()) && (string.CanSeek());

}

bool StreamStringTest::TestCopyConstructor(const char8 * initializationString) {
    StreamString toCopy = initializationString;
    StreamString string(toCopy);

    uint32 size = toCopy.Size();

    if (string.Position() != size) {
        return false;
    }

    if (string.Size() != size) {
        return false;
    }

    if (StringHelper::Compare(string.Buffer(), toCopy.Buffer()) != 0) {

        return false;
    }

    return (string.CanWrite()) && (string.CanRead()) && (string.CanSeek());
}

bool StreamStringTest::TestDestructor() {

    const char8 * input = "HelloWorld";

    StreamString string(input);

    string.~StreamString();

    return true;
}

bool StreamStringTest::TestAnyTypeOperator(const char8* initializationString) {

    StreamString string(initializationString);

    AnyType test = string;

    TypeDescriptor td = test.GetTypeDescriptor();

    if (td.isStructuredData) {
        return false;
    }

    if (td.isConstant) {
        return false;
    }

    if (td.type != SString) {
        return false;
    }
    if (td.numberOfBits != (sizeof(StreamString) * 8)) {
        return false;
    }

    if (test.GetDataPointer() != &string) {
        return false;
    }
    return test.GetBitAddress() == 0;
}

bool StreamStringTest::TestRead(const char8* inputString,
                          uint32 sizeToRead) {
    StreamString myString;
    myString = inputString;

    myString.Seek(0);

    char8 outputBuffer[32];
    cleanOutputBuffer(outputBuffer, 32);

    //size equal to inputString length (+1 for the terminated char8).
    uint32 size = StringHelper::Length(inputString);
    myString.Read(outputBuffer, sizeToRead);

    return (sizeToRead > size) ? (StringHelper::Compare(inputString, outputBuffer) == 0) : (StringHelper::CompareN(inputString, outputBuffer, sizeToRead) == 0);
}

bool StreamStringTest::TestReadTimeout(const char8* inputString,
                          uint32 sizeToRead) {
    StreamString myString;
    myString = inputString;

    myString.Seek(0);

    char8 outputBuffer[32];
    cleanOutputBuffer(outputBuffer, 32);

    //size equal to inputString length (+1 for the terminated char8).
    uint32 size = StringHelper::Length(inputString);
    myString.Read(outputBuffer, sizeToRead, 500);

    return (sizeToRead > size) ? (StringHelper::Compare(inputString, outputBuffer) == 0) : (StringHelper::CompareN(inputString, outputBuffer, sizeToRead) == 0);
}


bool StreamStringTest::TestWrite(const char8* inputString,
                           uint32 sizeToWrite) {

    StreamString myString;
    myString = inputString;

    myString.Seek(0);

    const uint32 bufferSize = 32;
    char8 inputBuffer[bufferSize];
    StringHelper::Copy(inputBuffer, inputString);

    //size equal to inputString length (+1 for the terminated char8).
    uint32 size = StringHelper::Length(inputString);
    myString.Write(inputBuffer, sizeToWrite);

    return (sizeToWrite > size) ?
            (StringHelper::Compare(inputString, myString.Buffer()) == 0) : (StringHelper::CompareN(inputString, myString.Buffer(), sizeToWrite) == 0);

}

bool StreamStringTest::TestWriteTimeout(const char* inputString,
                      uint32 sizeToWrite) {
    StreamString myString;
    myString = inputString;

    myString.Seek(0);

    const uint32 bufferSize = 32;
    char8 inputBuffer[bufferSize];
    StringHelper::Copy(inputBuffer, inputString);

    //size equal to inputString length (+1 for the terminated char8).
    uint32 size = StringHelper::Length(inputString);
    myString.Write(inputBuffer, sizeToWrite, 500);

    return (sizeToWrite > size) ?
            (StringHelper::Compare(inputString, myString.Buffer()) == 0) : (StringHelper::CompareN(inputString, myString.Buffer(), sizeToWrite) == 0);
}

bool StreamStringTest::TestCanWrite() {
    StreamString string;
    return string.CanWrite();
}

bool StreamStringTest::TestCanRead() {
    StreamString string;
    return string.CanRead();
}

bool StreamStringTest::TestCanSeek() {
    StreamString string;
    return string.CanSeek();
}

bool StreamStringTest::TestSize(const char8 * input) {
    StreamString string(input);

    return string.Size() == StringHelper::Length(input);
}

bool StreamStringTest::TestSetSize(uint32 size) {
    StreamString string;

    // set the size of the buffer
    string.SetSize(size);

    return string.Buffer() != NULL;

}

bool StreamStringTest::TestBuffer(const char8 * input) {

    StreamString string(input);

    return StringHelper::Compare(input, string.Buffer()) == 0;

}

bool StreamStringTest::TestBufferReference(const char8 * input) {

    StreamString string(input);

    char8 *buff = string.BufferReference();
    if (StringHelper::Compare(buff, input)) {
        return false;
    }

    const char8* newInput = "";
    StringHelper::Copy(buff, newInput);

    return StringHelper::Compare(newInput, string.Buffer()) == 0;
}

bool StreamStringTest::TestSeek(uint32 usedSize,
                          uint32 seek,
                          bool expected) {

    StreamString myString;
    char8 toWrite[64];
    myString.Write(toWrite, usedSize);

    if (myString.Position() != usedSize) {
        return false;
    }

    myString.Seek(0);
    if (myString.Position() != 0) {
        return false;
    }

    bool ret = myString.Seek(seek);
    if (ret) {
        if (myString.Position() != seek) {
            return false;
        }
    }
    else {
        if (myString.Position() != usedSize) {
            return false;
        }
    }

    return ret == expected;

}

bool StreamStringTest::TestRelativeSeek(uint32 initialPos,
                                  int32 delta,
                                  bool expected) {
    StreamString string;

    uint32 usedSize = 2 * initialPos;

    char8 toWrite[64];
    string.Write((const char8*) toWrite, usedSize);

    string.Seek(initialPos);

    if (string.Position() != initialPos) {
        return false;
    }

    bool ret = string.RelativeSeek(delta);
    if (ret) {
        if (string.Position() != (initialPos + delta)) {
            return false;
        }
    }
    else {
        if (delta >= 0) {
            if (string.Position() != (uint32) (usedSize)) {
                return false;
            }
        }
        else {
            if (string.Position() != 0) {
                return false;
            }
        }
    }

    return ret == expected;
}

bool StreamStringTest::TestPosition() {

    StreamString string;

    if (string.Position() != 0) {
        return false;
    }

    const char8 *toWrite = "HelloWorld";
    uint32 expectedPosition = StringHelper::Length(toWrite);
    string = toWrite;

    if (string.Position() != expectedPosition) {
        return false;
    }

    string.Write(toWrite, expectedPosition);
    expectedPosition *= 2;

    if (string.Position() != expectedPosition) {
        return false;
    }

    string.Seek(0);

    //the read change the position

    char8 toRead[32];

    string.Read(toRead, expectedPosition);
    if (string.Position() != expectedPosition) {
        return false;
    }

    //the seek change the position
    string.Seek(3);
    if (string.Position() != 3) {
        return false;
    }

    //the relative seek change the position
    string.RelativeSeek(2);
    if (string.Position() != 5) {
        return false;
    }
    return true;
}

bool StreamStringTest::TestTail(const char8* input,
                          uint32 index) {
    StreamString string(input);
    uint32 size = StringHelper::Length(input);

    const char8 *ret = string.Tail(index);

    if (ret == NULL) {
        if (index < (size - 1)) {
            return false;
        }
        else {
            return true;
        }
    }

    return ret[0] == input[size - index - 1];
}

bool StreamStringTest::TestCopyOperator_Char(char8 input) {

    StreamString string = "DUMMY";

    string = input;

    return string.Buffer()[0] == input;

}

bool StreamStringTest::TestCopyOperator_CCString(const char8 * input) {

    StreamString string = "DUMMY";

    string = input;

    return StringHelper::Compare(input, string.Buffer()) == 0;

}

bool StreamStringTest::TestCopyOperator_String(const char8 * input) {

    StreamString inputString = input;

    StreamString outputString = "DUMMY";
    outputString = inputString;

    return StringHelper::Compare(inputString.Buffer(), outputString.Buffer()) == 0;

}

bool StreamStringTest::TestConcatenateOperator_Char(char8 input) {

    const char8 *initial = "DUMMY";
    uint32 size = StringHelper::Length(initial);
    StreamString string = initial;
    string += input;

    char8 test[64];
    StringHelper::Copy(test, initial);
    test[size] = input;
    test[size + 1] = 0;

    return StringHelper::Compare(test, string.Buffer()) == 0;
}

bool StreamStringTest::TestConcatenateOperator_CCString(const char8 *input) {

    const char8 *initial = "DUMMY";
    StreamString string = initial;
    string += input;
    char8 test[64];
    StringHelper::Copy(test, initial);
    StringHelper::Concatenate(test, input);
    return StringHelper::Compare(test, string.Buffer()) == 0;
}

bool StreamStringTest::TestConcatenateOperator_String(const char8 *input) {
    const char8 *initial = "DUMMY";
    StreamString string = initial;
    StreamString toConcatenate = input;
    string += toConcatenate;
    char8 test[64];
    StringHelper::Copy(test, initial);
    StringHelper::Concatenate(test, input);
    return StringHelper::Compare(test, string.Buffer()) == 0;
}

bool StreamStringTest::TestIsEqualOperator_CCString(const char8 *input) {
    StreamString string = input;

    char8 testFalse[32];
    StringHelper::Copy(testFalse, input);
    StringHelper::Concatenate(testFalse, "a");

    if (string == testFalse) {
        return false;
    }

    return string == input;
}

bool StreamStringTest::TestIsEqualOperator_String(const char8 *input) {
    StreamString inputString = input;
    StreamString string = inputString;

    StreamString testFalse = input;
    testFalse += "a";

    if (string == testFalse) {
        return false;
    }

    return string == inputString;
}

bool StreamStringTest::TestIsDifferentOperator_CCString(const char8 *input) {
    StreamString string = input;

    if (string != input) {
        return false;
    }
    char8 testFalse[32];
    StringHelper::Copy(testFalse, input);
    StringHelper::Concatenate(testFalse, "a");

    return string != testFalse;

}

bool StreamStringTest::TestIsDifferentOperator_String(const char8 *input) {
    StreamString inputString = input;
    StreamString string = inputString;

    if (string != input) {
        return false;
    }
    StreamString testFalse = input;
    testFalse += "a";

    return string != testFalse;

}

bool StreamStringTest::TestGetCharacterOperator(const char8 *input,
                                          uint32 index) {
    StreamString string = input;
    uint32 size = StringHelper::Length(input);

    return (index > size) ? (string[index] == 0) : string[index] == input[index];

}

bool StreamStringTest::TestLocate_Char(const char8 *input,
                                 char8 c,
                                 int32 expected) {
    StreamString string = input;
    return (string.Locate(c) == expected);
}

bool StreamStringTest::TestLocate_String(const char8 *input,
                                   const char8* toSearch,
                                   int32 expected) {
    StreamString string = input;
    return (string.Locate(toSearch) == expected);
}

bool StreamStringTest::TestPrintFormatted(const PrintfNode testTable[]) {
    StreamString string;
    uint32 i = 0;
    while (StringHelper::Compare(testTable[i].format, "") != 0) {
        string = "";

        string.PrintFormatted(testTable[i].format, testTable[i].inputs);
        if (StringHelper::Compare(testTable[i].expectedResult, string.Buffer()) != 0) {
            AnyType data = testTable[i].inputs[i];
            printf("\n%s %s %d %d\n", string.Buffer(), testTable[i].expectedResult, i, *((uint8*) data.GetDataPointer()));
            return false;
        }
        i++;
    }
    return true;
}

bool StreamStringTest::TestGetToken(const TokenTestTableRow *table) {
    StreamString myStream;

    uint32 i = 0u;
    const TokenTestTableRow *row = &table[i];
    bool result = true;

    while (result && (row->toTokenize != NULL)) {
        myStream = row->toTokenize;
        myStream.Seek(0);
        char saveTerminator;
        uint32 t = 0u;

        uint32 outBuffSize = 64;
        char8 outputBuff[64] = { 0 };

        while (myStream.GetToken(outputBuff, row->terminators, outBuffSize, saveTerminator, row->skipCharacters)) {
            if (StringHelper::Compare(outputBuff, row->expectedResult[t]) != 0) {
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
