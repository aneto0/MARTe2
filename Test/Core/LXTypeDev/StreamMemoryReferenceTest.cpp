/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:$
 *
 **/

#include "GeneralDefinitions.h"
#include "StreamMemoryReferenceTest.h"
#include "StringHelper.h"
#include "StreamTestHelper.h"
#include "DoubleInteger.h"
#include "stdio.h"

void cleanBuffer(char* buffer, int32 size) {
    for (int32 i = 0; i < size; i++) {
        buffer[i] = 0;
    }
}

bool StreamMemoryReferenceTest::TestGetC() {
    char buffer[10];
    StreamMemoryReference mBReference(buffer, 10);
    const char* inputString = "Hello World";
    uint32 size = StringHelper::Length(inputString);

    mBReference.Write(inputString, size);
    mBReference.Seek(0);

    char retC;

    for (int32 i = 0; i < 10; i++) {
        mBReference.GetC(retC);
        if (retC != inputString[i]) {
            return False;
        }
    }

    //out of range, retC remains the same!
    mBReference.GetC(retC);

    if (retC != 'l') {
        return False;
    }

    return True;
}

bool StreamMemoryReferenceTest::TestPutC() {

    char buffer[10];
    StreamMemoryReference mBReference(buffer, 10);
    const char* toPut = "Hello World";

    //Put all inputString on the StreamMemoryReference
    for (int32 i = 0; i < 10; i++) {
        mBReference.PutC(*toPut);
        toPut++;
    }

    //Size finished	
    mBReference.PutC(*toPut);

    mBReference.Seek(0);

    //Check the result.	
    if (StringHelper::Compare(mBReference.Buffer(), "Hello Worl") != 0) {
        return False;
    }

    return True;
}

bool StreamMemoryReferenceTest::TestReadAndWrite() {

    char* nullPointer = NULL;
    StreamMemoryReference nullReference(nullPointer, 10);

    //canSeek is always true
    if (nullReference.CanRead() || nullReference.CanWrite()
            || !nullReference.CanSeek()) {
        return False;
    }

    char buffer[10];
    uint32 buffSize = 10;
    StreamMemoryReference mBReference(buffer, buffSize);

    //buffer is empty.
    if (mBReference.Size() != 0) {
        return False;
    }

    //set the used size.
    mBReference.SetSize(buffSize);
    if (mBReference.Size() != buffSize) {
        return False;
    }

    //return to empty.
    mBReference.SetSize(0);

    const char* inputString = "Hello World";

    //Write only 7
    uint32 tempSize = 7;
    mBReference.Write(inputString, tempSize);

    mBReference.Seek(0);
    char outputBuffer[32];
    cleanBuffer(outputBuffer, 32);

    tempSize = 32;
    mBReference.Read(outputBuffer, tempSize);

    if (StringHelper::Compare(outputBuffer, "Hello W") != 0) {
        return False;
    }

    cleanBuffer(outputBuffer, 32);

    //reWrite
    tempSize = 32;
    mBReference.Seek(0);
    mBReference.Write(inputString, tempSize);

    //reRead
    tempSize = 32;
    mBReference.Seek(0);
    mBReference.Read(outputBuffer, tempSize);
    if (tempSize != 10) {
        return False;
    }

    //already read all, tempSize becomes 0.
    mBReference.Read(outputBuffer, tempSize);
    if (tempSize != 0) {
        return False;
    }

    if (StringHelper::Compare(outputBuffer, "Hello Worl") != 0) {
        return False;
    }

    //create a read only buffer
    StreamMemoryReference readOnly(inputString, 10);
    cleanBuffer(outputBuffer, 32);

    tempSize = 32;
    readOnly.Read(outputBuffer, tempSize);

    if (StringHelper::Compare(outputBuffer, "Hello Worl") != 0) {
        return False;
    }

    return True;
}

bool StreamMemoryReferenceTest::TestSeek(const char *inputString) {

    char buffer[10];
    StreamMemoryReference mBReference(buffer, 10);

    char test[11];
    uint32 bufferSize = 10;
    StringHelper::CopyN(test, inputString, bufferSize);
    uint32 size = StringHelper::Length(inputString);

    if (size > bufferSize) {
        size = bufferSize;
    }

    mBReference.Write(inputString, size);

    //Test position
    if (mBReference.Position() != size) {
        return False;
    }

    //seek to zero
    if (!mBReference.Seek(0)) {
        return False;
    }

    if (StringHelper::CompareN(test, mBReference.Buffer(), size) != 0) {
        return False;
    }

    //seek greater than the dimension
    if (mBReference.Seek(size + 10)) {
        return False;
    }

    if (mBReference.Position() != size) {
        return False;
    }

    //relative seek
    if (!mBReference.RelativeSeek(-1)) {
        return False;
    }

    char outputBuffer[32];
    uint32 dummySize = 1;
    mBReference.Read(outputBuffer, dummySize);

    //read a char. It increments the position.
    if (mBReference.Position() != size
            || (*outputBuffer) != inputString[size - 1]) {
        return False;
    }

    //position under 0
    if (mBReference.RelativeSeek(-size - 1)) {
        return False;
    }

    if (StringHelper::CompareN(test, mBReference.Buffer(), size) != 0) {
        return False;
    }

    //position over the size
    if (mBReference.RelativeSeek(size + 10)) {
        return False;
    }

    if (mBReference.Position() != size) {
        return False;
    }

    return True;
}

bool StreamMemoryReferenceTest::TestPrint() {

    char buffer[32];

    //Create a read&write streamable.
    StreamMemoryReference mBReference(buffer, 32);

    uint8 ubit8 = 1;
    uint16 ubit16 = 2;
    uint32 ubit32 = 3;
    uint64 ubit64 = 4;

    int8 sbit8 = -1;
    int16 sbit16 = -2;
    int32 sbit32 = -3;
    int64 sbit64 = -4;

    float fbit32 = 1.2;
    double dbit64 = 3.4;

    //Use the unbuffered PutC, 4 parameters.	
    //For integer the letter is useless
    mBReference.Printf("% 3u % 3f % 3d % 3x\n", sbit8, fbit32, sbit16, sbit8);

    //Use the unbuffered PutC, 4 parameters.	
    //For integer the letter is useless
    mBReference.Printf("% 3u % 3f % 3d % 3x\n", ubit64, fbit32, ubit16, sbit8);

    //size expired

    //Use the unbuffered PutC, 3 parameters.
    mBReference.Printf("% 3u % 3c % 3d\n", sbit64, ubit32, fbit32);

    //Use the unbuffered PutC, 3 parameters.
    mBReference.Printf("% 3u % 3c % 3d\n", sbit8, ubit32, fbit32);

    //Use the unbuffered PutC, 2 parameters.
    mBReference.Printf("% 3c % 3F\n", ubit8, dbit64);
    //Use the unbuffered PutC, 2 parameters.
    mBReference.Printf("% 3c % 3F\n", ubit8, dbit64);

    //Use the unbuffered PutC, 1 parameter.
    mBReference.Printf("% 3o\n", sbit32);

    if (StringHelper::CompareN(" -1 1.2  -2  FF\n  4 1.2   2  FF\n",
                               mBReference.Buffer(), 32) != 0) {
        return False;
    }

    return True;
}

bool StreamMemoryReferenceTest::TestToken() {

    const char* inputString = "Nome::: Giuseppe. Cognome: Ferrò:)";
    uint32 size = StringHelper::Length(inputString);

    char inputBuffer[40];
    StreamMemoryReference mBReference(inputBuffer, 40);
    mBReference.Write(inputString, size);

    char buffer[32];
    char saveTerminator;

    mBReference.Seek(0);

    //TESTS ON TOKEN FUNCTIONS BETWEEN A STREAM AND A STRING
    mBReference.GetToken(buffer, ".:", size, &saveTerminator, NULL);
    if (StringHelper::Compare(buffer, "Nome") != 0 || saveTerminator != ':') {
        return False;
    }

    //Without skip chars, the function skips consecutive terminators.
    mBReference.GetToken(buffer, ".:", size, &saveTerminator, NULL);
    if (StringHelper::Compare(buffer, " Giuseppe") != 0
            || saveTerminator != '.') {
        return False;
    }

    //return before the :::
    mBReference.Seek(4);

    //The function skips correctly the second ":"
    mBReference.GetToken(buffer, ".:", size, &saveTerminator, ":");
    if (StringHelper::Compare(buffer, " Giuseppe") != 0
            || saveTerminator != '.') {
        return False;
    }

    //The function does not skips because the terminator character is not correct
    mBReference.Seek(4);

    mBReference.GetToken(buffer, ".:", size, &saveTerminator, ".");
    if (StringHelper::Compare(buffer, "") != 0 || saveTerminator != ':') {
        return False;
    }

    //Test with a minor maximum size passed by argument. The terminated char is calculated in the size
    mBReference.Seek(4);

    size = 4;
    mBReference.GetToken(buffer, ".:", size, &saveTerminator, ":");
    if (StringHelper::Compare(buffer, " Gi") != 0 || saveTerminator != 'i') {
        return False;
    }
    size = StringHelper::Length(inputString);

    //Arrive to the next terminator
    mBReference.GetToken(buffer, ".:", size, &saveTerminator, ":");

    //Test if the functions terminate when the string is terminated
    for (uint32 i = 0; i < 3; i++) {
        mBReference.GetToken(buffer, ".:", size, &saveTerminator, ":");
    }
    if (StringHelper::Compare(buffer, ")") != 0) {
        return False;
    }

    mBReference.Seek(0);

    mBReference.SkipTokens(3, ":.");
    mBReference.GetToken(buffer, ")", size, &saveTerminator, NULL);
    if (StringHelper::Compare(buffer, " Ferrò:") != 0) {
        return False;
    }

    return True;

}

