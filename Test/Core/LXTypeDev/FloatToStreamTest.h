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
 * @file FloatToStreamTest.h
 * @brief Tests the IOBufferFloatPrint functions.
 *
 * The test consists in observing the results of the conversions from float (or equivalent) numbers and strings and their correct
 * print on a generic stream which implements a PutC(c) function.
 */

#ifndef FLOAT_TO_STREAM_TEST_H
#define FLOAT_TO_STREAM_TEST_H

#include "IOBufferFloatPrint.h"
#include "FormatDescriptor.h"

/** @brief Class for testing of IOBufferFloatPrint functions. */
class FloatToStreamTest {

private:

public:

    /** 
     * @brief Tests the print of a float on a stream in fixed point notation (precision = #decimals)
     * @return false if something fails.
     *
     * Test the print of float number on stream try to trigger all special cases. */
    bool TestFixedPoint();

    /**
     * @brief Tests the print of a float on a stream in fixed point relative notation (precision = #significative digits).   
     * @return false if something fails.
     *
     * Test the print of float number on stream try to trigger all special cases. */
    bool TestFixedRelativePoint();

    /**
     * @brief Tests the print of a float on a stream in engineering notation (precision = #significative digits).
     * @return false if something fails.
     *
     * Test the print of float number on stream try to trigger all special cases. */
    bool TestEngeneering();

    /**
     * @brief Tests the print of a float on a stream in smart notation (precision = #significative digits).
     * @return false if something fails.
     *
     * Test the print of float number on stream try to trigger all special cases. */
    bool TestSmart();

    /**
     * @brief Tests the print of a float on a stream in exponential notation (precision = #significative digits).
     * @return false if something fails.
     *
     * Test the print of float number on stream try to trigger all special cases. */
    bool TestExponential();

    /**
     * @brief Tests the print of a float on a stream in compact notation (precision = #significative digits).
     * @return false if something fails.
     *
     * Test the print of float number on stream try to trigger all special cases. */
    bool TestCompact();
};

#endif
