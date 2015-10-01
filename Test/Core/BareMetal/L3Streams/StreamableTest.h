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
 * @file StreamableTest.h
 * @brief Tests the Streamable functions.
 *
 * Tests the Streamable functions using the StreamWrapperIOBuffer class functions. */

#ifndef STREAM_STRING_TEST_H
#define STREAM_STRING_TEST_H

#include "Streamable.h"
#include "FormatDescriptor.h"

/** @brief Class for testing of Streamable functions. */
class StreamableTest {

private:

public:

    /**
     * @brief Assign a StreamWrapperIOBuffer to a streamable and test the seek and resync functions.
     * @return true if succesful, false otherwise.
     *
     * Using SimpleStreamable class defined in StreamTestHelper, assigns to it a StreamWrapperIOBuffer
     * and then, after a buffered getToken, resyncronizes the position. */
    bool TestSeek();

    /**
     * @brief Tests the printf function when the streamable does not have an IOBuffer. 
     * @return true if succesful, false otherwise.
     *
     * Uses printf with a string greater then 64 bytes to tests if it flushes on the streamable. */
    bool TestPrint();

    /**
     * @brief Tests GetToken in different conditions.
     * @return true if succesful, false otherwise.
     *
     * Uses GetToken against two streamable without IOBuffers, and against a streamable without IOBuffer and one provided. */
    bool TestToken();

};

#endif
