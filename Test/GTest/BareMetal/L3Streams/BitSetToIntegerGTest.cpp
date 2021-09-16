/**
 * @file BitSetToIntegerGTest.cpp
 * @brief Source file for class BitSetToIntegerGTest
 * @date 21/10/2015
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
 * the class BitSetToIntegerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include "BitSetToIntegerTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestBitSetToBitSet_64) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet<uint64>());
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet<int64>());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestBitSetToBitSet_32) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet<uint32>());
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet<int32>());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestBitSetToBitSet_16) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet<uint16>());
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet<int16>());}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestBitSetToBitSet_8) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet<uint8>());
    ASSERT_TRUE(bitsettest.TestBitSetToBitSet<int8>());}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestBitSetToInteger_8) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToInteger_8());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestBitSetToInteger_16) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToInteger_16());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestBitSetToInteger_32) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToInteger_32());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestBitSetToInteger_64) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestBitSetToInteger_64());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestIntegerToBitSet_8) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestIntegerToBitSet_8());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestIntegerToBitSet_16) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestIntegerToBitSet_16());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestIntegerToBitSet_32) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestIntegerToBitSet_32());
}

TEST(BareMetal_L3Streams_BitSetToIntegerGTest,TestIntegerToBitSet_64) {
    BitSetToIntegerTest bitsettest;
    ASSERT_TRUE(bitsettest.TestIntegerToBitSet_64());
}
