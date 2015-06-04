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
 * @file BitSetToIntegerTest.h
 * @brief Tests the BitSetToInteger functions.
 *
 * The test consists in observe the results of the various functions which manipulate bitSet data.
 */

#ifndef BITSET_TO_INTEGER_TEST_H
#define BITSET_TO_INTEGER_TEST_H

#include "BitSetToInteger.h"
#include "FormatDescriptor.h"
#define MAX_DIMENSION 128 

class BitSetToIntegerTest {

private:

public:
    /**
     * @brief Tests the write from source bitSet to destination bitSet.
     * @return true if succesful, false otherwise. */
    bool TestBitSetToBitSet();

    /**
     * @brief Tests the write from a bitSet to an integer.
     * @return true if succesful, false otherwise. */
    bool TestBitSetToInteger();

    /**
     * @brief Tests the write from an integer to a bitSet.
     * @return true if succesful, false otherwise. */
    bool TestIntegerToBitSet();
};

#endif
