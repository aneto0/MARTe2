/**
 * @file StreamStringTest.cpp
 * @brief Source file for class StreamStringTest
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

#include "GeneralDefinitions.h"
#include "StreamStringTest.h"
#include "StringHelper.h"
#include "StreamTestHelper.h"
#include "DoubleInteger.h"
#include "stdio.h"
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

    printf("\n|%s| |%s|\n", myString.Buffer(), outputBuffer);

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

    printf("\n|%s| |%s|\n", myString.Buffer(), inputBuffer);

    return (sizeToWrite > size) ?
            (StringHelper::Compare(inputString, myString.Buffer()) == 0) : (StringHelper::CompareN(inputString, myString.Buffer(), sizeToWrite) == 0);

}

bool StreamStringTest::TestUnbufferedWrite() {
    StreamString dummy;
    uint32 dummySize = 1;
    return !dummy.UnbufferedWrite("", dummySize);
}

bool StreamStringTest::TestUnbufferedRead() {
    StreamString dummy;
    uint32 dummySize = 1;
    return !dummy.UnbufferedRead("", dummySize);
}

uint64 StreamStringTest::TestUnbufferedSize() {
    StreamString dummy;
    return dummy.UnbufferedSize() == 0;
}

bool StreamStringTest::TestUnbufferedSeek() {
    StreamString dummy;
    return !dummy.UnbufferedSeek(0);
}

bool StreamStringTest::TestUnbufferedRelativeSeek() {
    StreamString dummy;
    return !dummy.UnbufferedRelativeSeek(0);
}

uint64 StreamStringTest::TestUnbufferedPosition() {
    StreamString dummy;
    return dummy.UnbufferedPosition() == 0;
}

bool StreamStringTest::TestUnbufferedSetSize() {
    StreamString dummy;
    return !dummy.UnbufferedSetSize(1);
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
    if(myString.Position()!=0){
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

bool StreamStringTest::TestOperators(const char8* firstString,
        const char8* secondString) {
    /* StreamString myString1;
     StreamString myString2;

     //string parameters must be different each other

     //Assignment operator
     myString1 = firstString;

     //return at the beginning.
     myString1.Seek(0);

     //Is different operator.
     if (myString1 != firstString || !(myString1 == firstString) || myString1 == secondString) {
     return false;
     }

     char8* s = NULL;

     //Null assignment return false
     if (myString1 = s) {
     return false;
     }

     if (myString1 == s) {
     return false;
     }

     //Different sizes.
     myString2 = secondString;
     if (myString1 == myString2) {
     return false;
     }

     myString2 = firstString;
     myString1.BufferReference()[0] = 'a';

     //Same sizes but different strings
     if (myString1 == myString2) {
     return false;
     }

     //Another assignment
     myString1 = secondString;

     myString1.Seek(0);
     //Is equal operator
     if (myString1 == firstString || myString1 != secondString) {
     return false;
     }

     //Same operators between two StreamStrings
     myString1 = firstString;
     myString2 = myString1;

     myString1.Seek(0);
     myString1.Seek(0);
     if (myString1 != myString2 || !(myString1 == myString2)) {
     return false;
     }

     //Append operation
     myString1 = secondString;
     myString1 += firstString;
     char8 test[100];

     StringHelper::Copy(test, secondString);
     StringHelper::Concatenate(test, firstString);

     myString1.Seek(0);
     if (myString1 != test) {
     return false;
     }

     //Append another StreamString
     myString2 = firstString;

     //Try to make it fail with a seek.
     myString1.Seek(0);

     myString1 += myString2;
     StringHelper::Concatenate(test, firstString);

     myString1.Seek(0);
     if (myString1 != test) {
     return false;
     }

     //Use char8
     char8 c = 'c';
     myString2 = c;
     myString2 += c;
     if (myString2 != "cc") {
     return false;
     }

     //[] operator

     myString1 = secondString;
     if (secondString[1] != myString1[1]) {
     return false;
     }

     //Should return 0 if the begin is greater than size.
     uint32 size = StringHelper::Length(secondString);
     if (myString1[size + 1] != 0) {
     return false;
     }

     //Size function
     if (myString1.Size() != size) {
     return false;
     }

     //Locate function with char8

     //empty string return false.
     myString1 = "";
     if (myString1.Locate('\0') != -1) {
     return false;
     }

     myString1 = "HelloWorld";

     if (myString1.Locate('W') != 5) {
     return false;
     }

     //Locate function with another StreamString

     myString2 = "World";
     if (myString1.Locate(myString2) != 5) {
     return false;
     }

     //not found
     myString2 = "ImNotIn";

     if (myString1.Locate(myString2) != -1) {
     return false;
     }

     //greater string
     myString2 = "HelloWorld!";
     if (myString1.Locate(myString2) != -1) {
     return false;
     }

     //empty string
     myString2 = "";
     if (myString1.Locate(myString2) != -1) {
     return false;
     }*/

    return true;
}

bool StreamStringTest::TestPrint() {
    /*
     //Create a read&write streamable.
     StreamString myString;

     uint8 ubit8 = 1;
     uint16 ubit16 = 2;
     uint32 ubit32 = 3;
     uint64 ubit64 = 4;
     DoubleInteger<uint64> ubit128Tmp = 5;
     //ubit128.dataPointer = &ubit128Tmp;
     const TypeDescriptor UnsignedInt128(false, UnsignedInteger, 128);
     //ubit128.dataDescriptor = UnsignedInt128;
     //ubit128.bitAddress = 0;
     AnyType ubit128(UnsignedInt128, 0, &ubit128Tmp);

     int8 sbit8 = -1;
     int16 sbit16 = -2;
     int32 sbit32 = -3;
     int64 sbit64 = -4;

     float fbit32 = 1.2;
     double dbit64 = 3.4;
     __float128 fbit128Tmp = 5.6Q;

     //fbit128.dataPointer = &fbit128Tmp;
     const TypeDescriptor Float128(false, Float, 128);
     //fbit128.dataDescriptor = Float128;
     //fbit128.bitAddress = 0;

     AnyType fbit128(Float128, 0, &fbit128Tmp);

     int64 shifted64bitNumber = 0xffffffffffffb234;
     //  shifted64bit.dataPointer = &shifted64bitNumber;
     //shifted64bit.bitAddress = 12;
     const TypeDescriptor SignedShift(false, SignedInteger, 52);
     //    shifted64bit.dataDescriptor = SignedShift;
     AnyType shifted64bit(SignedShift, 12, &shifted64bitNumber);

     //Use the unbuffered PutC, 4 parameters.
     //For integer the letter is useless
     myString.Printf("% 3u % 3f % 3d % 3x\n", sbit8, fbit32, sbit16, sbit8);

     //Use the unbuffered PutC, 4 parameters.
     //For integer the letter is useless
     myString.Printf("% 3u % 3f % 3d % 3x\n", ubit64, fbit32, ubit16, sbit8);

     //Use the unbuffered PutC, 3 parameters.
     myString.Printf("% 3u % 3c % 3d\n", sbit64, ubit32, fbit32);

     //Use the unbuffered PutC, 3 parameters.
     myString.Printf("% 3i % 3c % 3d\n", shifted64bit, ubit128, fbit32);

     //Use the unbuffered PutC, 2 parameters.
     myString.Printf("% 3c % 3F\n", ubit8, dbit64);
     //Use the unbuffered PutC, 2 parameters.
     myString.Printf("% 3c % 3F\n", ubit8, dbit64);

     //Use the unbuffered PutC, 1 parameter.
     myString.Printf("% 3o\n", sbit32);

     if (StringHelper::Compare(" -1 1.2  -2  FF\n  4 1.2   2  FF\n -4   3 1.2\n -5   5 1.2\n  1 3.4\n  1 3.4\n  ?\n", myString.Buffer()) != 0) {
     return false;
     }

     //Use the print function
     myString = "";
     FormatDescriptor fd;
     const char8* pformat = "d";
     fd.InitialiseFromString(pformat);
     myString.Print(ubit8, fd);
     if (myString != "1") {
     return false;
     }

     //Unsupported 128 float numbers
     if (myString.Printf("%f", fbit128)) {
     return false;
     }

     //structured data anytype

     //wrong type in the format
     if (myString.Printf("%3l", fbit32)) {
     return false;
     }

     const char8* Hello = "HelloWorld";

     //cast const char8* to anytype.
     myString = "";
     myString.Printf("string:%s, number:%3d", Hello, dbit64);
     if (myString != "string:HelloWorld, number:3.4") {
     return false;
     }
     //Clip the string
     myString = "";
     myString.Printf("string:%5s, number:%3d", Hello, dbit64);
     if (myString != "string:Hello, number:3.4") {
     return false;
     }
     //Padd right the string
     myString = "";
     myString.Printf("string:% 11s, number:%3d", Hello, dbit64);
     if (myString != "string: HelloWorld, number:3.4") {
     return false;
     }

     //Padd left the string
     myString = "";
     myString.Printf("string:%- 11s, number:%3d", Hello, dbit64);
     if (myString != "string:HelloWorld , number:3.4") {
     return false;
     }

     //cast StreamString to anytype.
     myString = "";
     StreamString input = "HelloWorld";
     myString.Printf("string:%s, number:%3d", input, dbit64);
     if (myString != "string:HelloWorld, number:3.4") {
     return false;
     }

     //Clip the stream.
     myString = "";
     input = "HelloWorld";
     myString.Printf("string:%5s, number:%3d", input, dbit64);
     if (myString != "string:Hello, number:3.4") {
     return false;
     }

     //right padd
     myString = "";
     input = "HelloWorld";
     myString.Printf("string:% 12s, number:%3d", input, dbit64);
     if (myString != "string:  HelloWorld, number:3.4") {
     return false;
     }

     //left padd
     myString = "";
     input = "HelloWorld";
     myString.Printf("string:%-12s, number:%3d", input, dbit64);
     if (myString != "string:HelloWorld  , number:3.4") {
     return false;
     }

     //return the pointer thanks to hex notation.
     myString = "";
     input = "";
     const char8 *charPointer = "Hello";
     uint64 pointer = (uint64) charPointer;
     myString.Printf("%x", charPointer);
     input.Printf("%x", pointer);
     if (myString != input) {
     return false;
     }

     //%p format as the complete 32 bit pointer with header
     myString = "";
     input = "";
     myString.Printf("%p", (void*) charPointer);
     input.Printf("% #0x", pointer);
     if (myString != input) {
     return false;
     }*/

    return true;
}

bool StreamStringTest::TestToken() {
    /*
     const char8* inputString = "Nome::: Giuseppe. Cognome: Ferrò:)";
     uint32 size = StringHelper::Length(inputString);

     StreamString myString;
     myString = inputString;

     char8 buffer[32];
     char8 saveTerminator;

     myString.Seek(0);

     //TESTS ON TOKEN FUNCTIONS BETWEEN A STREAM AND A STRING
     myString.GetToken(buffer, ".:", size, &saveTerminator, NULL);
     if (StringHelper::Compare(buffer, "Nome") != 0 || saveTerminator != ':') {
     return false;
     }

     //Without skip chars, the function skips consecutive terminators.
     myString.GetToken(buffer, ".:", size, &saveTerminator, NULL);
     if (StringHelper::Compare(buffer, " Giuseppe") != 0 || saveTerminator != '.') {
     return false;
     }

     //return before the :::
     myString.Seek(4);

     //The function skips correctly the second ":"
     myString.GetToken(buffer, ".:", size, &saveTerminator, ":");
     if (StringHelper::Compare(buffer, " Giuseppe") != 0 || saveTerminator != '.') {
     return false;
     }

     //return before the :::
     myString.Seek(7);

     //skip "u" and "p"
     myString.GetToken(buffer, ".:", size, &saveTerminator, "up");
     if (StringHelper::Compare(buffer, " Gisee") != 0 || saveTerminator != '.') {
     return false;
     }

     //The function does not skips because the terminator character is not correct
     myString.Seek(4);

     myString.GetToken(buffer, ".:", size, &saveTerminator, ".");
     if (StringHelper::Compare(buffer, "") != 0 || saveTerminator != ':') {
     return false;
     }

     //Test with a minor maximum size passed by argument. The terminated char8 is calculated in the size
     myString.Seek(4);

     size = 4;
     myString.GetToken(buffer, ".:", size, &saveTerminator, ":");
     if (StringHelper::Compare(buffer, " Gi") != 0 || saveTerminator != 'i') {
     return false;
     }
     size = StringHelper::Length(inputString);

     //Arrive to the next terminator
     myString.GetToken(buffer, ".:", size, &saveTerminator, ":");

     //Test if the functions terminate when the string is terminated
     for (uint32 i = 0; i < 3; i++) {
     myString.GetToken(buffer, ".:", size, &saveTerminator, ":");
     }
     if (StringHelper::Compare(buffer, ")") != 0) {
     return false;
     }

     myString.Seek(0);

     myString.SkipTokens(3, ":.");
     myString.GetToken(buffer, ")", size, &saveTerminator, NULL);
     if (StringHelper::Compare(buffer, " Ferrò:") != 0) {
     return false;
     }

     //TESTS ON TOKEN FUNCTIONS BETWEEN A STRING AND STREAM

     myString.Seek(0);

     SimpleBufferedStream outputStream;
     outputStream.seekable = true;

     uint32 buffOutputSize = 1;
     if (!outputStream.SetBuffered(buffOutputSize)) {
     return false;
     }

     myString.GetToken(outputStream, ".:", &saveTerminator, NULL);
     outputStream.Seek(0);
     if (StringHelper::Compare(outputStream.buffer, "Nome") != 0 || saveTerminator != ':') {
     return false;
     }
     outputStream.Clear();

     //Without skip chars, the function skips consecutive terminators.
     myString.GetToken(outputStream, ".:", &saveTerminator, NULL);
     outputStream.Seek(0);
     if (StringHelper::Compare(outputStream.buffer, " Giuseppe") != 0 || saveTerminator != '.') {
     return false;
     }
     outputStream.Clear();
     //return before the :::
     myString.Seek(4);

     //The function skips correctly the second ":"
     myString.GetToken(outputStream, ".:", &saveTerminator, ":");
     outputStream.Seek(0);
     if (StringHelper::Compare(outputStream.buffer, " Giuseppe") != 0 || saveTerminator != '.') {
     return false;
     }

     outputStream.Clear();
     //return after the :::
     myString.Seek(7);

     //skips "u" and "p"
     myString.GetToken(outputStream, ".:", &saveTerminator, "up");
     outputStream.Seek(0);
     if (StringHelper::Compare(outputStream.buffer, " Gisee") != 0 || saveTerminator != '.') {
     return false;
     }

     outputStream.Clear();
     //The function does not skips because the terminator character is not correct
     myString.Seek(4);

     myString.GetToken(outputStream, ".:", &saveTerminator, ".");
     outputStream.Seek(0);
     if (StringHelper::Compare(outputStream.buffer, "") != 0 || saveTerminator != ':') {
     return false;
     }

     //Test if the functions terminate when the string is terminated
     for (uint32 i = 0; i < 3; i++) {
     myString.GetToken(outputStream, ".:", &saveTerminator, ":");
     }

     outputStream.Seek(0);
     if (StringHelper::Compare(outputStream.buffer, " Giuseppe Cognome Ferrò") != 0) {
     return false;
     }
     */
    return true;

}
