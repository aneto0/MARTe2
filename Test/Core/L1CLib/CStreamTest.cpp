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
#include "CStreamTest.h"
#include "StringTestHelper.h"



//This function allocate memory for the stream.
void CreateNewBuffer(CStream* p) {
    struct CStreamContext *context = (struct CStreamContext*) (p->context);

    if ((context->counter) == 0) {
        p->bufferPtr = (char*) MemoryMalloc(context->buffGranularity);
        (context->counter) = 1;
        context->begin = p->bufferPtr;
    }
    else {

        int32 n = ++(context->counter);
        int32 newSize = n * (context->buffGranularity);
        char* temp = (char*) MemoryRealloc((void*&) (context->begin), newSize);
        context->begin = temp;
    }
    p->sizeLeft += context->buffGranularity;
}

//It's a false new buffer to simulate a stream.
void FakeNewBuffer(CStream* p) {
    struct CStreamContext *context = (struct CStreamContext*) (p->context);
    p->sizeLeft += context->buffGranularity;
}

//Free memory allocate for the stream
void FreeAll(CStream* p) {
    struct CStreamContext *context = (struct CStreamContext*) (p->context);
    if (context->begin != NULL) {
        MemoryFree((void *&) context->begin);
    }
}

//Test of the CPut function
bool CStreamTest::TestCPut(char c) {
    myContext.buffGranularity = 4;

    //Put a char on the stream, the function itself allocates new memory.
    CPut(&myCStream, c);
    char* begin = myCStream.bufferPtr - 1;
    //Check if the char on the stream is correct
    bool cond = begin[0] == c;
    FreeAll(&myCStream);
    return cond;
}

//Test the CGet function
bool CStreamTest::TestCGet(char c) {
    myContext.buffGranularity = 4;
    myCStream.NewBuffer(&myCStream);

    //Write a char on the stream
    *(myCStream.bufferPtr) = c;
    char cRet;

    //Get a char from the stream
    CGet(&myCStream, cRet);

    //Check if the char on the stream is correct
    bool cond = cRet == c;
    FreeAll(&myCStream);
    return cond;
}

//Read from the stream to a buffer
bool CStreamTest::TestCRead(const char* string) {
    myContext.buffGranularity = 4;

    //Allocate space (not necessary because bufferPtr point to the string argument but increase the sizeLeft value)
    myCStream.NewBuffer(&myCStream);
    uint32 size = StringTestHelper::Size((char*) string);
    myCStream.bufferPtr = (char*) string;
    size++;

    //Create a buffer due to read from CStream
    char result[size];
    char* begin = myCStream.bufferPtr;
    if (!CRead(&myCStream, (void*) result, size)) {
        FreeAll(&myCStream);
        return False;
    }

    //Check if the read string is correct.
    bool cond = StringTestHelper::Compare(begin, (char*) result);
    FreeAll(&myCStream);
    return cond;
}

//Write from a buffer to the stream
bool CStreamTest::TestCWrite(const char* string) {

    //Allocate memory for the stream
    myContext.buffGranularity = 4;
    myCStream.NewBuffer(&myCStream);
    uint32 size = StringTestHelper::Size((char*) string);
    size++;
    char* begin = myCStream.bufferPtr;
    //Write the string argument on the stream
    if (!CWrite(&myCStream, (void*) string, size)) {
        FreeAll(&myCStream);
        return False;
    }

    //Check if the string on stream is correct
    bool cond = StringTestHelper::Compare(begin, (char*) string);
    FreeAll(&myCStream);
    return cond;
}

//Write on stream an int32 number with different formats
bool CStreamTest::TestPrintInt32(const char* sDec, const char* uDec,
                                 const char* hex, const char* oct,
                                 int32 number) {

    //Allocate new memory
    myCStream.NewBuffer(&myCStream);
    char* begin = myCStream.bufferPtr;

    //Print the maximum negative number 0x8000000. In complement 2 it's -0x7fffffff = -2^31.

    //The '-' going before the space requested for the desired size
    CPrintInt32(&myCStream, -2147483648, 12, 0, 'd');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) "- 2147483648")) {
        FreeAll(&myCStream);
        return False;
    }

    //In hexadecimal mode.
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, -2147483648, 9, 0, 'x');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) " 80000000")) {
        FreeAll(&myCStream);
        return False;
    }

    //In octal mode.
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, -2147483648, 13, 0, 'o');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) "  20000000000")) {
        FreeAll(&myCStream);
        return False;
    }

    //The '-' going before the padding
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, -2147483648, 12, '*', 'd');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) "-*2147483648")) {
        FreeAll(&myCStream);
        return False;
    }

    //In hexadecimal mode.
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, -2147483648, 9, 'x', 'x');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) "x80000000")) {
        FreeAll(&myCStream);
        return False;
    }

    //In octal mode.
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, -2147483648, 12, 'x', 'o');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) "x20000000000")) {
        FreeAll(&myCStream);
        return False;
    }

    //Write an integer in signed mode on the stream
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, number, 0, ' ', 'd');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) sDec)) {
        FreeAll(&myCStream);
        return False;
    }

    //Write an integer in unsigned mode on the stream
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, number, 0, ' ', 'u');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) uDec)) {
        FreeAll(&myCStream);
        return False;
    }

    //Write an integer in hex mode on the stream
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, number, 0, ' ', 'X');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) hex)) {
        FreeAll(&myCStream);
        return False;
    }

    //Write an integer in oct mode	
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, number, 0, ' ', 'o');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) oct)) {
        FreeAll(&myCStream);
        return False;
    }

    //Test the zero write on stream for different formats
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, 0, 0, ' ', 'd');
    if (begin[0] != '0') {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, 0, 0, ' ', 'u');
    if (begin[0] != '0') {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, 0, 0, ' ', 'X');
    if (begin[0] != '0') {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, 0, 0, ' ', 'o');
    if (begin[0] != '0') {
        FreeAll(&myCStream);
        return False;
    }

    //Test the padding modality. The padding is added before the number and the final size is >= of desiredSize
    uint32 desiredSize = 12;

    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, number, desiredSize, '*', 'd');
    CPut(&myCStream, '\0');
    if (StringTestHelper::Size(begin) != desiredSize) {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, number, desiredSize, '*', 'u');
    CPut(&myCStream, '\0');
    if (StringTestHelper::Size(begin) != desiredSize) {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, number, desiredSize, '*', 'X');
    CPut(&myCStream, '\0');
    if (StringTestHelper::Size(begin) != desiredSize) {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintInt32(&myCStream, number, desiredSize, '*', 'o');
    CPut(&myCStream, '\0');
    if (StringTestHelper::Size(begin) != desiredSize) {
        FreeAll(&myCStream);
        return False;
    }
    FreeAll(&myCStream);

    return True;
}

//Write on stream an int64 number with different formats
bool CStreamTest::TestPrintInt64(const char* sDec, const char* hex,
                                 const char* oct, int64 number) {

    //Write a long in signed mode
    myCStream.NewBuffer(&myCStream);
    char* begin = myCStream.bufferPtr;
    CPrintInt64(&myCStream, number, 0, ' ', 'd');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) sDec)) {
        FreeAll(&myCStream);
        return False;
    }

    //Write a long in hex mode
    begin = myCStream.bufferPtr;
    CPrintInt64(&myCStream, number, 0, ' ', 'X');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) hex)) {
        FreeAll(&myCStream);
        return False;
    }

    //Write a long in oct mode
    begin = myCStream.bufferPtr;
    CPrintInt64(&myCStream, number, 0, 0, 'o');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) oct)) {
        FreeAll(&myCStream);
        return False;
    }

    //Test zero write on the stream
    begin = myCStream.bufferPtr;
    CPrintInt64(&myCStream, 0, 0, ' ', 'd');
    if (begin[0] != '0') {
        FreeAll(&myCStream);
        return False;
    }

    begin = myCStream.bufferPtr;
    CPrintInt64(&myCStream, 0, 0, ' ', 'X');
    if (begin[0] != '0') {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintInt64(&myCStream, 0, 0, ' ', 'o');
    if (begin[0] != '0') {
        FreeAll(&myCStream);
        return False;
    }

    //Test the padding modality. The padding is added before the number.
    uint32 desiredSize = 23;

    begin = myCStream.bufferPtr;
    CPrintInt64(&myCStream, number, desiredSize, '*', 'd');
    CPut(&myCStream, '\0');
    if (StringTestHelper::Size(begin) != desiredSize) {
        FreeAll(&myCStream);
        return False;
    }
    CPrintInt64(&myCStream, number, desiredSize, '*', 'X');
    CPut(&myCStream, '\0');
    if (StringTestHelper::Size(begin) != desiredSize) {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintInt64(&myCStream, number, desiredSize, '*', 'o');
    CPut(&myCStream, '\0');
    if (StringTestHelper::Size(begin) != desiredSize) {
        FreeAll(&myCStream);
        return False;
    }

    FreeAll(&myCStream);
    return True;
}

bool CStreamTest::TestPrintDouble() {

    //Allocare memory and initialize a big and a small double
    myCStream.NewBuffer(&myCStream);

    //Test the default printing
    double myDouble = 2.321;
    const char* string = "2.3e+000";
    char* begin = myCStream.bufferPtr;
    CPrintDouble(&myCStream, myDouble, 0, 1, 0, 'e');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //Test the approximation
    myDouble = 99.9999;
    string = "1.0e+002";
    begin = myCStream.bufferPtr;
    CPrintDouble(&myCStream, myDouble, 0, 1, 0, 'e');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    double myBigDouble = 139665.99554;
    double myLittleDouble = 0.0395;

    //Test the padding and the e-modality with the big number. Furthermore it tests also the 
    //approximation at the 7th number after the point
    string = "*****1.3966600e+005";
    begin = myCStream.bufferPtr;
    CPrintDouble(&myCStream, myBigDouble, 19, 7, '*', 'e');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //Test the padding and the e-modality with the small number. It tests also the approximation at the 1st number
    //after the point
    string = "**4.0e-002";
    begin = myCStream.bufferPtr;
    CPrintDouble(&myCStream, myLittleDouble, 10, 1, '*', 'e');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //Create an Inf number.
    double inf = 1.0 / 0.0;

    //Check if it recognizes the inf number. The padding is not considered, but it's added a space for the desired size.
    begin = myCStream.bufferPtr;
    CPrintDouble(&myCStream, inf, 5, 7, '*', 'e');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) "+Inf ")) {
        FreeAll(&myCStream);
        return False;
    }

    //With a negative sign.
    inf = -1.0 / 0.0;

    begin = myCStream.bufferPtr;
    CPrintDouble(&myCStream, inf, 6, 7, '*', 'e');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) "-Inf  ")) {
        FreeAll(&myCStream);
        return False;
    }

    //Create a Nan number.
    double nan = 0.0 / 0.0;

    //Test the print of a Nan number.
    begin = myCStream.bufferPtr;
    CPrintDouble(&myCStream, nan, 5, 7, '*', 'f');
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) "NaN  ")) {
        FreeAll(&myCStream);
        return False;
    }

    FreeAll(&myCStream);
    return True;

}

//Test the write of strings on the stream
bool CStreamTest::TestPrintString(const char* string) {

    //Allocate new memory
    myCStream.NewBuffer(&myCStream);
    char* begin = myCStream.bufferPtr;

    //Write string on stream with right and left justify but no padding. It must be the same
    CPrintString(&myCStream, string, 0, 0, True);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }
    begin = myCStream.bufferPtr;
    CPrintString(&myCStream, string, 0, 0, False);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //Add three stars as padding and test right and left justify
    uint32 size = StringTestHelper::Size((char*) string);

    uint32 desiredSize = size + 3;
    char result[32];
    StringTestHelper::Append((char*) string, (char*) "***", result);

    begin = myCStream.bufferPtr;
    CPrintString(&myCStream, string, desiredSize, '*', False);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) result)) {
        FreeAll(&myCStream);
        return False;
    }
    StringTestHelper::Append((char*) "***", (char*) string, result);

    begin = myCStream.bufferPtr;
    CPrintString(&myCStream, string, desiredSize, '*', True);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) result)) {
        FreeAll(&myCStream);
        return False;
    }
    FreeAll(&myCStream);
    return True;
}

//Test the Cprintf function. There are other tests on double
bool CStreamTest::TestCPrintf() {

    //print an int32 on stream
    myCStream.NewBuffer(&myCStream);
    char* begin = myCStream.bufferPtr;
    const char* string = "Int32 number:   13    d   15";
    CPrintf(&myCStream, "Int32 number: %4i %4x %4o", 13, 13, 13);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //print an int64 on stream
    begin = myCStream.bufferPtr;
    string = "Int64 number: 1099500000000 FFFF4E9300 37723511400";
    CPrintf(&myCStream, "Int64 number: %lli %6Lx %3lo", 1099500000000,
            1099500000000, 1099500000000);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //print a double on stream. The default numbers after the point are 6
    string = "Double number: 199.900000";
    begin = myCStream.bufferPtr;
    CPrintf(&myCStream, "Double number: %f", 199.9);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //approximation with f-modality of double
    string = "Double number: 200.0";
    begin = myCStream.bufferPtr;
    CPrintf(&myCStream, "Double number: %.1f", 199.9989);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //if the desired numbers after the point are greather than nine they returns to the default (6)
    //it tests also the % print
    string = "Double number: 199.900000 %";
    begin = myCStream.bufferPtr;
    CPrintf(&myCStream, "Double number: %10f %%", 199.9);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //space and numbers after the point changes for each new entry
    string = "Double number: @@@@@@@@@199.90 1.999000e+002";
    begin = myCStream.bufferPtr;
    CPrintf(&myCStream, "Double number: %@15.2f %e", 199.9, 199.9);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //characters are not affected by desired space. Default left justify for strings
    begin = myCStream.bufferPtr;
    const char* origin = "print this string?";	//18
    char c = 'y';
    string = "   print this string? y";
    CPrintf(&myCStream, "%21s %9c", origin, c);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //Padding on the left
    begin = myCStream.bufferPtr;
    string = "***print this string? y";	//18
    CPrintf(&myCStream, "%*21s %9c", origin, c);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //To impose the right justify for strings add -
    begin = myCStream.bufferPtr;
    string = "print this string?    y";	//18
    CPrintf(&myCStream, "%-21s %9c", origin, c);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //A double - impose '-' as padding
    begin = myCStream.bufferPtr;
    string = "print this string?--- y";	//18
    CPrintf(&myCStream, "%--21s %9c", origin, c);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    //Padding on the right
    begin = myCStream.bufferPtr;
    string = "%--- print this string?***     r y";	//18
    CPrintf(&myCStream, "%--- %*-21s %5s %9c", origin, "r", c);
    CPut(&myCStream, '\0');
    if (!StringTestHelper::Compare(begin, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    string = "print this string?    y";	//18
    //Test the sprintf function with size passed by argument (incluse the terminated char)
    char myBuffer[32];
    bl2_snprintf(myBuffer, 18, "%-21s %9c", origin, c);
    if (!StringTestHelper::Compare(myBuffer, (char*) "print this string")) {
        FreeAll(&myCStream);
        return False;
    }

    //Test the sprintf function without size.
    bl2_sprintf(myBuffer, "%-21s %9c", origin, c);
    if (!StringTestHelper::Compare(myBuffer, (char*) string)) {
        FreeAll(&myCStream);
        return False;
    }

    FreeAll(&myCStream);
    return True;
}

bool CStreamTest::TokenTest() {

    //TESTS ON TOKEN FUNCTIONS BETWEEN STRINGS

    //Define a string (caution: don't use const char to avoid segmentation fault for the destructiveToken function
    char input1[] = "Nome::: Giuseppe. Cognome: Ferrò:)";
    char* input = input1;
    char* begin = input;
    uint32 size = 15;
    char buffer[15];

    //This function returns the token using as terminators chars passed in the argument (the terminator is a char)
    CGetCStringToken((const char*&) input, buffer, ":.", size);

    if (!StringTestHelper::Compare((char*) buffer, (char*) "Nome")) {
        return False;
    }
    CGetCStringToken((const char*&) input, buffer, ":.", size);
    if (!StringTestHelper::Compare((char*) buffer, (char*) " Giuseppe")) {
        return False;
    }
    CGetCStringToken((const char*&) input, buffer, ":.", size);
    if (!StringTestHelper::Compare((char*) buffer, (char*) " Cognome")) {
        return False;
    }
    char saveTerminator;

    //return to the initial string and get the token with the distructive function. A '\0' char is written at the position of the terminator
    input = begin;
    char* p = CDestructiveGetCStringToken((char *&) input, ":.",
                                          &saveTerminator, ":");
    if (!StringTestHelper::Compare((char*) p, (char*) "Nome") || saveTerminator != ':') {
        return False;
    }

    //Restore the terminator char in the string
    input--;
    if (*input != 0) {
        return False;
    }
    *input = saveTerminator;
    input++;

    //Save this point of the string to test the skip
    char* newBegin = input;

    //Impose null skip. The function skips consecutive terminators automatically
    p = CDestructiveGetCStringToken((char *&) input, ":.", &saveTerminator,
                                    NULL);
    if (!StringTestHelper::Compare((char*) p, (char*) " Giuseppe") || saveTerminator != '.') {
        return False;
    }
    input--;
    *input = saveTerminator;
    input++;

    //Imposing ":" as skip char, the function skips the second ":" terminator
    input = newBegin;
    p = CDestructiveGetCStringToken((char *&) input, ":.", &saveTerminator,
                                    ":");
    if (!StringTestHelper::Compare((char*) p, (char*) " Giuseppe") || saveTerminator != '.') {
        return False;
    }

    //Imposing "." as skip char, the function doesn't skips because it finds ":" and return an empty string
    input--;
    *input = saveTerminator;
    input++;
    input = newBegin;
    p = CDestructiveGetCStringToken((char *&) input, ":.", &saveTerminator,
                                    ".");
    if (!StringTestHelper::Compare((char*) p, (char*) "")) {
        return False;
    }

    //Test if the functions terminate when the string is terminated
    for (uint32 i = 0; i < 4; i++) {
        input--;
        *input = saveTerminator;
        input++;
        p = CDestructiveGetCStringToken((char *&) newBegin, ":.",
                                        &saveTerminator, NULL);
    }
    if (!StringTestHelper::Compare((char*) p, (char*) ")")) {
        return False;
    }

    //TESTS ON TOKEN FUNCTIONS BETWEEN A CSTREAM AND A STRING
    myCStream.NewBuffer = FakeNewBuffer;
    myCStream.NewBuffer(&myCStream);
    myCStream.bufferPtr = (char*) "Nome:: Giuseppe. Cognome: Ferrò:)";

    begin = myCStream.bufferPtr;
    CGetToken(&myCStream, (char*) buffer, ".:", size, &saveTerminator, NULL);
    if (!StringTestHelper::Compare((char*) buffer, (char*) "Nome") || saveTerminator != ':') {
        FreeAll(&myCStream);
        return False;
    }

    //Without skip chars, the function skips consecutive terminators.
    newBegin = myCStream.bufferPtr;

    CGetToken(&myCStream, (char*) buffer, ".:", size, &saveTerminator, NULL);
    if (!StringTestHelper::Compare((char*) buffer, (char*) " Giuseppe")
            || saveTerminator != '.') {
        FreeAll(&myCStream);
        return False;
    }
    myCStream.bufferPtr = newBegin;

    //The function skips correctly the second ":"
    CGetToken(&myCStream, (char*) buffer, ".:", size, &saveTerminator, ":");
    if (!StringTestHelper::Compare((char*) buffer, (char*) " Giuseppe")
            || saveTerminator != '.') {
        FreeAll(&myCStream);
        return False;
    }

    //The function does not skips because the terminator character is not correct
    myCStream.bufferPtr = newBegin;

    CGetToken(&myCStream, (char*) buffer, ".:", size, &saveTerminator, ".");
    if (!StringTestHelper::Compare((char*) buffer, (char*) "") || saveTerminator != ':') {
        FreeAll(&myCStream);
        return False;
    }

    //Test with a minor maximum size passed by argument. The terminated char is calculated in the size
    myCStream.bufferPtr = newBegin;
    size = 4;
    CGetToken(&myCStream, (char*) buffer, ".:", size, &saveTerminator, ":");
    if (!StringTestHelper::Compare((char*) buffer, (char*) " Gi") || saveTerminator != 'i') {
        FreeAll(&myCStream);
        return False;
    }
    size = 15;
    //Arrive to the next terminator
    CGetToken(&myCStream, (char*) buffer, ".:", size, &saveTerminator, ":");

    //Test if the functions terminate when the string is terminated
    for (uint32 i = 0; i < 3; i++) {
        CGetToken(&myCStream, (char*) buffer, ".:", size, &saveTerminator, ":");
    }
    if (!StringTestHelper::Compare((char*) buffer, (char*) ")")) {
        FreeAll(&myCStream);
        return False;
    }

    //TESTS ON TOKEN FUNCTIONS BETWEEN TWO CSTREAMS

    //Create two CStreams. The first in input does not allocate memory because points to a string, the second allocates memory
    struct CStream inputCStream;
    struct CStream outputCStream;
    struct CStreamContext inputContext;
    struct CStreamContext outputContext;

    inputContext.counter = 0;
    outputContext.counter = 0;

    inputContext.buffGranularity = 0;
    outputContext.buffGranularity = 100;

    inputCStream.context = (void*) (&inputContext);
    inputCStream.bufferPtr = NULL;
    //The input CStream does not allocate memory
    inputCStream.NewBuffer = FakeNewBuffer;
    inputCStream.sizeLeft = 0;

    outputCStream.context = (void*) (&outputContext);
    outputCStream.bufferPtr = NULL;
    outputCStream.NewBuffer = CreateNewBuffer;
    outputCStream.sizeLeft = 0;

    inputCStream.NewBuffer(&inputCStream);
    outputCStream.NewBuffer(&outputCStream);

    //Create the string to tokenize
    inputCStream.bufferPtr = (char*) "Nome:: Giuseppe. Cognome:: Ferrò:)";
    begin = inputCStream.bufferPtr;

    uint32 totalSize = 0;
    char* output = outputCStream.bufferPtr;

    //Use a string as a terminator  with this function
    CGetStringToken(&inputCStream, &outputCStream, ":: ", totalSize);
    CPut(&outputCStream, '\0');
    if (!StringTestHelper::Compare((char*) output, (char*) "Nome")) {
        FreeAll(&outputCStream);
        return False;
    }

    output = outputCStream.bufferPtr;
    CGetStringToken(&inputCStream, &outputCStream, ":: ", totalSize);
    CPut(&outputCStream, '\0');
    if (!StringTestHelper::Compare((char*) output, (char*) "Giuseppe. Cognome")) {
        FreeAll(&outputCStream);
        return False;
    }

    //If the sizeLeft of inputCStream (3 because the fake newBuffer function for inputCStream increases sizeleft of buffGranularity and CStrinToken calls the newBuffer function)
    //is different than totalSize passed by argument, the function write on output sizeLeft chars.
    output = outputCStream.bufferPtr;
    inputCStream.sizeLeft = 0;
    inputContext.buffGranularity = 3;
    totalSize = 5;
    CGetStringToken(&inputCStream, &outputCStream, ":: ", totalSize);
    CPut(&outputCStream, '\0');
    if (!StringTestHelper::Compare((char*) output, (char*) "Fer")) {
        FreeAll(&outputCStream);
        return False;
    }

    //Test the Token with chars as terminators between two strings
    inputCStream.bufferPtr = begin;
    output = outputCStream.bufferPtr;
    CGetCSToken(&inputCStream, &outputCStream, ":.", &saveTerminator, ":");
    CPut(&outputCStream, '\0');
    if (!StringTestHelper::Compare((char*) output, (char*) "Nome")) {
        FreeAll(&outputCStream);
        return False;
    }

    //The function skips the second ":"
    output = outputCStream.bufferPtr;
    CGetCSToken(&inputCStream, &outputCStream, ":.", &saveTerminator, ":");
    CPut(&outputCStream, '\0');
    if (!StringTestHelper::Compare((char*) output, (char*) " Giuseppe")) {
        FreeAll(&outputCStream);
        return False;
    }
    CGetCSToken(&inputCStream, &outputCStream, ":", &saveTerminator, NULL);

    //The function does not skips because the terminator is different than the second ":"
    output = outputCStream.bufferPtr;
    CGetCSToken(&inputCStream, &outputCStream, ":.", &saveTerminator, ".");
    CPut(&outputCStream, '\0');
    if (!StringTestHelper::Compare((char*) output, (char*) "")) {
        FreeAll(&outputCStream);
        return False;
    }

    //Tests if the functions write the final of the string without terminators.
    output = outputCStream.bufferPtr;
    CGetCSToken(&inputCStream, &outputCStream, "", &saveTerminator, NULL);
    CPut(&outputCStream, '\0');
    if (!StringTestHelper::Compare((char*) output, (char*) " Ferrò:)")) {
        FreeAll(&outputCStream);
        return False;
    }

    //Test the skip function that can skip a number of terminetors. Consecutive terminetors are calculated once.
    inputCStream.bufferPtr = begin;
    CSkipTokens(&inputCStream, 3, ":.");
    output = outputCStream.bufferPtr;
    CGetCSToken(&inputCStream, &outputCStream, ")", &saveTerminator, NULL);
    CPut(&outputCStream, '\0');
    if (!StringTestHelper::Compare((char*) output, (char*) ": Ferrò:")) {
        FreeAll(&outputCStream);
        return False;
    }
    FreeAll(&outputCStream);
    return True;
}

