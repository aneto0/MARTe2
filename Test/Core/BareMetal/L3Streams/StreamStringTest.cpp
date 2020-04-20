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

    if (string.Buffer().IsNullPtr()) {
        return false;
    }

    return (string.CanWrite()) && (string.CanRead()) && (string.CanSeek());
}

bool StreamStringTest::TestConstructor_CCString(CCString initializationString) {
    StreamString string(initializationString);

    uint32 size = initializationString.GetSize();

    if (string.Position() != size) {
        return false;
    }

    if (string.Size() != size) {
        return false;
    }

    if (string != initializationString) {

        return false;
    }

    return (string.CanWrite()) && (string.CanRead()) && (string.CanSeek());

}

bool StreamStringTest::TestCopyConstructor(const char8 * initializationString) {
    StreamString toCopy(initializationString);
    StreamString string(toCopy);

    uint32 size = toCopy.Size();

    if (string.Position() != size) {
        return false;
    }

    if (string.Size() != size) {
        return false;
    }

    if (toCopy != string.Buffer()) {

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

    TypeDescriptor td = test.GetFullVariableDescriptor().GetSummaryTypeDescriptor();


    if (td.DataIsConstant()) {
        return false;
    }

    if (!td.IsStreamString()) {
        return false;
    }

    if (test.GetVariablePointer() != &string) {
        return false;
    }
    return true;
}

bool StreamStringTest::TestRead(const char8* inputString, uint32 sizeToRead) {
    StreamString myString;
    myString = inputString;

    myString.Seek(0);

    char8 outputBuffer[32];
    cleanOutputBuffer(outputBuffer, 32);

    //size equal to inputString length (+1 for the terminated char8).
    uint32 size = CCString(inputString).GetSize();
    myString.Read(outputBuffer, sizeToRead);

    return (sizeToRead > size) ? (CCString(inputString) == outputBuffer) : (CCString(inputString).CompareContent(outputBuffer, sizeToRead) == 0);
}

bool StreamStringTest::TestReadTimeout(const char8* inputString, uint32 sizeToRead) {
    StreamString myString;
    myString = inputString;

    myString.Seek(0);

    char8 outputBuffer[32];
    cleanOutputBuffer(outputBuffer, 32);

    //size equal to inputString length (+1 for the terminated char8).
    uint32 size = CCString(inputString).GetSize();
    myString.Read(outputBuffer, sizeToRead, MilliSeconds(500,Units::ms));

    return (sizeToRead > size) ? (CCString(inputString) == outputBuffer) : (CCString(inputString).CompareContent(outputBuffer, sizeToRead) == 0);
}


bool StreamStringTest::TestWrite(const char8* inputString,  uint32 sizeToWrite) {

    StreamString myString;
    myString = inputString;

    myString.Seek(0);

    //size equal to inputString length (+1 for the terminated char8).
    uint32 size = CCString(inputString).GetSize();
    myString.Write(inputString, sizeToWrite);

    return (sizeToWrite > size) ? (myString == inputString) : (CCString(inputString).CompareContent(myString.Buffer(), sizeToWrite) == 0);

}

bool StreamStringTest::TestWriteTimeout(const char* inputString, uint32 sizeToWrite) {
    StreamString myString;
    myString.Seek(0);

    DynamicCString buffer(inputString);

    //size equal to inputString length (+1 for the terminated char8).
    uint32 size = CCString(inputString).GetSize();
    myString.Write(inputString, sizeToWrite, MilliSeconds(500,Units::ms));

    return (sizeToWrite > size) ? (myString == inputString) : (CCString(inputString).CompareContent(myString.Buffer(), sizeToWrite) == 0);
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

    return string.Size() == CCString(input).GetSize();
}

bool StreamStringTest::TestSetSize(uint32 size) {
    StreamString string;

    // set the size of the buffer
    string.SetSize(size);

    return string.Buffer() != NULL;

}

bool StreamStringTest::TestBuffer(const char8 * input) {

    StreamString string(input);

    return input == string.Buffer();

}

bool StreamStringTest::TestBufferReference(const char8 * input) {

    StreamString string(input);

    CString buff = string.BufferReference();
    if (buff !=  input) {
        return false;
    }

    buff[0] = '\0';
    return (CCString(string.Buffer()).GetSize() == 0);
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



bool StreamStringTest::TestRelativeSeek_OutOfInt32Range(){
    StreamString string("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH");

    int64 toMuchSeek=MAX_INT32;
    toMuchSeek++;

    return !string.RelativeSeek(toMuchSeek);
}

bool StreamStringTest::TestPosition() {

    StreamString string;

    if (string.Position() != 0) {
        return false;
    }

    CCString toWrite = "HelloWorld";
    uint32 expectedPosition = toWrite.GetSize();
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

bool StreamStringTest::TestTail(const char8* input, uint32 index) {
    StreamString string(input);
    uint32 size = CCString(input).GetSize();

    CCString ret = string.Tail(index);

    if (ret.GetSize() == 0) {
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

    StreamString string("DUMMY");

    string = input;

    return string.Buffer()[0] == input;

}

bool StreamStringTest::TestCopyOperator_CCString(const char8 * input) {

    StreamString string = CCString("DUMMY");

    string = CCString(input);

    return string == input;

}

bool StreamStringTest::TestCopyOperator_String(const char8 * input) {

    StreamString inputString(input);

    StreamString outputString("DUMMY");

    outputString = inputString;

    return outputString == inputString;

}

bool StreamStringTest::TestConcatenateOperator_Char(char8 input) {

    DynamicCString initial("DUMMY");
    StreamString string = CCString(initial);
    string += input;
    initial().Append(input);

    return string == CCString(initial);
}

bool StreamStringTest::TestConcatenateOperator_CCString(const char8 *input) {
    DynamicCString initial("DUMMY");
    StreamString string = CCString(initial);

    string += input;
    initial().Append(input);

    return string == CCString(initial);
}

bool StreamStringTest::TestConcatenateOperator_String(const char8 *input) {
    DynamicCString initial("DUMMY");
    StreamString string = CCString(initial);

    StreamString toConcatenate(input);
    string += toConcatenate;
    initial().Append(input);

    return string == CCString(initial);
}

bool StreamStringTest::TestIsEqualOperator_CCString(const char8 *input) {
    StreamString string(input);

    DynamicCString testFalse(input);
    testFalse().Append('a');

    if (string == testFalse) {
        return false;
    }
    return string == input;
}

bool StreamStringTest::TestIsEqualOperator_String(const char8 *input) {
    StreamString inputString(input);
    StreamString string = inputString;
    StreamString testFalse(input);
    testFalse += "a";

    if (string == testFalse) {
        return false;
    }

    return string == inputString;
}

bool StreamStringTest::TestIsDifferentOperator_CCString(const char8 *input) {
    StreamString string(input);

    if (string != input) {
        return false;
    }
    DynamicCString testFalse(input);
    testFalse().Append('a');

    return string != testFalse;

}

bool StreamStringTest::TestIsDifferentOperator_String(const char8 *input) {
    StreamString inputString(input);
    StreamString string = inputString;

    if (string != input) {
        return false;
    }
    StreamString testFalse(input);
    testFalse += "a";

    return string != testFalse;

}

bool StreamStringTest::TestGetCharacterOperator(const char8 *input, uint32 index) {
    StreamString string(input);
    uint32 size = string.Size();

    return (index > size) ? (string[index] == 0) : string[index] == input[index];

}

bool StreamStringTest::TestLocate_Char(const char8 *input,
                                 char8 c,
                                 int32 expected) {
    StreamString string(input);
    return (string.Locate(c) == expected);
}

bool StreamStringTest::TestLocate_String(const char8 *input,  const char8* toSearch, int32 expected) {
    StreamString string(input);
    return (string.Locate(CCString(toSearch)) == expected);
}

bool StreamStringTest::TestPrintFormatted(const PrintfNode testTable[]) {
    uint32 i = 0;
    while (CCString(testTable[i].format).GetSize() > 0) {
        StreamString string;

        string.PrintFormatted(testTable[i].format, testTable[i].inputs);

        if (string != testTable[i].expectedResult) {
            AnyType data = testTable[i].inputs[i];
            printf("\n%s %s %d %d\n", string.Buffer().GetList(), testTable[i].expectedResult, i, *((uint8*) data.GetVariablePointer()));
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

        StreamString outputBuff;

        while (myStream.GetToken(outputBuff, row->terminators, saveTerminator, row->skipCharacters)) {
            if (outputBuff != row->expectedResult[t]) {
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
