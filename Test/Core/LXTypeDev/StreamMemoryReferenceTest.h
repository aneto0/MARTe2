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
 * @file StreamMemoryReferenceTest.h
 * @brief Tests the StreamMemoryReferenceTest functions.
 *
 * Tests the StreamMemoryReference functions. */

#ifndef STREAM_STRING_TEST_H
#define STREAM_STRING_TEST_H

#include "StreamMemoryReference.h"
#include "FormatDescriptor.h"

/** @brief Class for testing of StreamMemoryReference functions. */
class StreamMemoryReferenceTest {

private:

public:

    /**
     * @brief Tests the StreamMemoryReference GetC function.
     * @return true if successful, false otherwise.
     *
     * Gets chars from the stream and checks that they are equal to the expected ones.*/
    bool TestGetC();

    /** 
     * @brief Test the StreamMemoryReference PutC function.
     * @return true if successful, false otherwise.
     *
     * Puts chars on the stream and checks at the end if all chars until the stream size are on the stream. */
    bool TestPutC();

    /**
     * @brief Tests the StreamMemoryReference read function.
     * @return true if succesful, false otherwise.
     * 
     * Uses read and write functions with different sizes to test their behavior. */
    bool TestReadAndWrite();

    /**
     * @brief Tests the StreamMemoryReference Seek and RelativeSeek.
     * @param inputString is the string already on the stream.
     * @return true if the seek operations returns the correct result. 
     *
     * Tests the seek functions in different conditions, for example using a position which falls out of bounds. */
    bool TestSeek(const char* inputString);

    /**
     * @brief Tests the printf function.
     * @return true if succesful, false otherwise.
     *
     * tests the printf function. It must write until the stream size. */
    bool TestPrint();

    /**
     * @brief Tests the GetToken functions.
     * @return true if successful, false otherwise. */
    bool TestToken();

};

#endif
