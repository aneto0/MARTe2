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
 * @file StreamStringTest.h
 * @brief Tests the StreamString functions.
 *
 * Tests the StreamString functions and operators. */

#ifndef STREAM_STRING_TEST_H
#define STREAM_STRING_TEST_H

#include "StreamString.h"
#include "FormatDescriptor.h"

/** @brief Class for testing of StreamString functions. */
class StreamStringTest {

private:

public:

    /**
     * @brief Tests the streamString GetC function.
     * @param inputString is the string.
     * @return true if the read string is equal to the parameter.
     *
     * The inputString is copied on the streamString and then read using GetC.*/
    bool TestGetC(const char* inputString);

    /** 
     * @brief Test the streamString PutC function.
     * @param inputString is the string to put on the streamString. 
     * @return true if on the stream is copied the parameter.
     *
     * The inputString is written on the stream using the PutC function. */
    bool TestPutC(const char* inputString);

    /**
     * @brief Tests the streamString read function.
     * @param inputString is the string on the streamString.
     * @return true if the read string is equal to inputString using Read function. */
    bool TestRead(const char* inputString);

    /**
     * @brief Tests the streamString write function.
     * @param inputString is the string to write on the streamString.
     * @return true if the string written on the streamString using Write function is equal to inputString.*/
    bool TestWrite(const char* inputString);

    /**
     * @brief Tests the streamString Seek and RelativeSeek.
     * @param stringToRead is the string already on the stream.
     * @param stringToWrite is the string to write on the stream.
     * @return true if the seek operations returns the correct result. 
     *
     * Test the seek functions in different conditions, for example using a positions which falls out of bounds. */
    bool TestSeek(const char* inputString);

    /** 
     * @brief Tests the streamString operators. 
     * @param firstString is the first string parameter.
     * @param secondString is the second string string parameter.
     * @return true if tests goes fine.
     *
     * Using the string parameters the streamString operators are tested. */
    bool TestOperators(const char* firstString, const char* secondString);

    /** 
     * @brief Tests the printf functions.
     * @return true if successful, false otherwise.
     * 
     * Tests the printf function with different types and formats.*/
    bool TestPrint();

    /**
     * @brief Tests the GetToken function.
     * @return true if successful, false otherwise.
     * 
     * @Tests the GetToken function between stream-string and stream-stream. */
    bool TestToken();

};

#endif
