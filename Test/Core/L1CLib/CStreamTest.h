/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
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
/**
 * @class CStreamTest
 * @brief Tests the CStream functions.
 *
 * The test consists in observe the results of the various operators between BStrings, const char* and char.
 */

#ifndef CSTREAM_TEST_H
#define CSTREAM_TEST_H

#include "CStream.h"
#include "Memory.h"

/** A struct used from CStream to create a new buffer memory. **/
struct CStreamContext {
    int32 counter;
    int32 buffGranularity;
    char* begin;
};

/** A function to create a new buffer of memory **/
void CreateNewBuffer(CStream* p);

class CStreamTest {

private:

public:
    struct CStreamContext myContext;
    struct CStream myCStream;

    CStreamTest() {
        /** Initialize the context struct. **/
        myContext.counter = 0;
        myContext.buffGranularity = 350;
        myContext.begin = NULL;

        /** Initialize the CStream struct. **/
        myCStream.context = (void*) (&myContext);
        myCStream.bufferPtr = NULL;
        myCStream.NewBuffer = CreateNewBuffer;
        myCStream.sizeLeft = 0;
    }

    /** 
     * Tests the CPut() function writing a char on the CStream.
     * @param c is the character to write on the CStream.
     * @return true if the char written on the stream is equal to the char passed by argument. **/
    bool TestCPut(char c);

    /**
     * Tests the CGet() function reading a char from the CStream.
     * @param c is the character written in the beginning on the CStream and then read with CGet().
     * @return true if the char read from the stream is equal to the char passed by argument. **/
    bool TestCGet(char c);

    /**
     * Tests if a string is correctly read from the CStream. The CStream contains the string passed by argument, then it is read by the CRead() function on a buffer.
     * @param string is the string on the CStream.
     * @return true if the read string is equal to the string passed by argument. **/
    bool TestCRead(const char* string);

    /**
     * Tests if a string is correctly written on the CStream.
     * @param string is the string to write on the CStream.
     * @return true if the string written on the CStream is equal to the string passed by argument. **/
    bool TestCWrite(const char* string);

    /**
     * Tests the write on the CStream of a int32 number in different formats.
     * @param sDec is the signed integer.
     * @param uDec is the unsigned integer.
     * @param hex is the integer in hexadecimal base.
     * @param oct is the integer in octal base.
     * @param number is the number to print on the CStream.
     * @return true if the strings written on CStream for different formats are equal to the strings passed by argument. **/
    bool TestPrintInt32(const char* sDec, const char* uDec, const char* hex,
                        const char* oct, int32 number);

    /**
     * Tests the write on the CStream of a int64 number in different formats.
     * @param sDec is the signed integer.
     * @param hex is the integer in hexadecimal base.
     * @param oct is the integer in octal base.
     * @param number is the number to print on the CStream.
     * @return true if the strings written on CStream for different formats are equal to the strings passed by argument. **/
    bool TestPrintInt64(const char* sDec, const char* hex, const char* oct,
                        int64 number);

    /**
     * Tests the write on a CStream of a double number with different numbers after the point and approximations.
     * @return true if the strings written on the CStream are equal to the expected strings. **/
    bool TestPrintDouble();

    /**
     * Tests the write on a CStream of a string with left and right justify.
     * @param string is the string to write on the CStream.
     * @return true if the strings written on the CStream are equal to the expected strings. **/
    bool TestPrintString(const char* string);

    /** 
     * Tests the CPrintf() function which replies the printf() on the CStream.
     * @return true if the CPrintf() acts as expected. **/
    bool TestCPrintf();

    /**
     * Tests the different functions to tokenize strings, with strings as input and output, a CStream as input and a string as output, and two CStreams as input and output.
     * @return true if all tokenize functions acts like expected. **/
    bool TokenTest();
};

#endif
