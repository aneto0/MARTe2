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
 * @file IntegerToStreamTest.h
 * @brief Tests the IOBufferIntegerPrint functions.
 *
 * The test consists in observing the results of the conversion from integer or bitSet to a string, and its corrent print
 * on a generic stream which implements a generic PutC(c) function.
 */

#ifndef INTEGER_TO_STREAM_TEST_H
#define INTEGER_TO_STREAM_TEST_H

#include "IOBufferIntegerPrint.h"
#include "FormatDescriptor.h"
#define MAX_DIMENSION 128 

/** @brief Class for testing of IOBufferIntegerPrint functions. */
class IntegerToStreamTest {

private:

public:

    /** 
     * @brief Tests the goodness of the function which returns the exponent of a decimal number.
     * @return true if the magnitude is equal to the number or digits minus one.*/
    bool TestDecimalMagnitude();

    /** 
     * @brief Tests the goodness of the function which returns the number of digits for an exadecimal notation.
     * @return true if the function returns the number of digits of hex numbers.*/
    bool TestHexadecimalMagnitude();

    /**
     * @brief Tests the goodness of the function which returns the number of digits for an octal notation.
     * @return true if the function returns the number of digits of octal numbers. */
    bool TestOctalMagnitude();

    /**
     * @brief Tests the goodness of the function which returns the number of digits for a binary notation.
     * @return true if the function returns the number of digits of binary numbers. */
    bool TestBinaryMagnitude();

    /**
     * @brief Tests the print of an integer on a generic stream using the decimal notation.
     * @return false if something fails.
     * 
     * Check the print on a stream in decimal notation try to trigger all cases. */
    bool TestDecimalStream();

    /**
     * @brief Tests the print of an integer on a generic stream using the exadecimal notation.
     * @return false if something fails.
     * 
     * @Check the print on a stream in exadecimal notation try to trigger all cases. */
    bool TestHexadecimalStream();

    /**
     * @brief Tests the print of an integer on a generic stream using the octal notation. 
     * @return false if something fails.
     * 
     * @Check the print on a stream in octal notation try to trigger all cases. */
    bool TestOctalStream();

    /**
     * @brief Tests the print of an integer on a generic stream using the binary notation.
     * @return false if something fails.
     * 
     * @Check the print on a stream in binary notation try to trigger all cases. */
    bool TestBinaryStream();

    /**
     * @brief Tests the print of an integer on a generic stream using different notations.
     * @return false if something fails. */
    bool TestIntegerToStream();

    /**
     * @brief Tests the print of a bitSet on a generic stream.
     * @return false if something fails. */
    bool TestBitSetToStream();
};

#endif
