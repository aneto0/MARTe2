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
#include "StreamTestHelper.h"

#define MAX_DIMENSION 128 
using namespace MARTe;

template<typename T>
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

template<typename T>
bool BitSetToIntegerTest<T>::TestBitSetToBitSet() {

    uint32 dest[] = { 0, 0, 0, 0 };
    const BitSetToBitSetTableRow *table = GeneratedBitSetToBitSetTable();

    uint32 i = 0;
    while (table[i].sPointer != NULL) {
        T *destPointer = (T*) dest;
        uint8 destShift = table[i].destShift;
        uint8 destSize = table[i].destSize;
        bool destIsSigned = table[i].destIsSigned;
        uint8 sourceShift = table[i].sourceShift;
        uint8 sourceSize = table[i].sourceSize;
        bool sourceIsSigned = table[i].sourceIsSigned;
        T* source = (T*) (table[i].sPointer);
        BitSetToBitSet(destPointer, destShift, destSize, destIsSigned, source, sourceShift, sourceSize, sourceIsSigned);

        if (dest[table[i].destArrayIndex] != table[i].result) {
            return false;
        }

        i++;
    }
    return true;

}

template<typename T>
bool BitSetToIntegerTest<T>::TestBitSetToInteger() {

    //128 bit source  [4294967221 -68, 4369, 572666675, 0]
    uint32 source[] = { 0xffffffb5, 0x00001111, 0x22223333, 0x0 };
    //128 bit dest
    int32 sDest;
    uint32 uDest;

    uint8 sourceShift = 0;
    uint8 sourceSize = 32;
    uint32 *sPointer = source;

    //Copy a signed 32 bit number.
    BitSetToInteger(sDest, sPointer, sourceShift, sourceSize, true);
    if (sDest != (int32) source[0]) {
        return false;
    }

    sourceShift = 0;
    sPointer = source;
    //Copy an unsigned 32 bit number, with source signed it should be saturated to 0.
    BitSetToInteger(uDest, sPointer, sourceShift, sourceSize, true);
    if (uDest != 0) {
        return false;
    }

    sourceShift = 0;
    sPointer = source;
    //Copy an unsigned 32 bit number, with source unsigned.
    BitSetToInteger(uDest, sPointer, sourceShift, sourceSize, false);
    if (uDest != source[0]) {
        return false;
    }

    sourceShift = 16;
    sPointer = source;
    //Source shift
    BitSetToInteger(sDest, sPointer, sourceShift, sourceSize, true);
    if (sDest != 0x1111ffff) {
        return false;
    }

    //Source automatic shift
    BitSetToInteger(sDest, sPointer, sourceShift, sourceSize, true);
    if (sDest != 0x33330000) {
        return false;
    }
/*---------------------------------------*/
/*
    uint32 source[] = { 0xffffffb5, 0x00001111, 0x22223333, 0x0 };

    uint8 uDest16;
    int8 sDest16;

    sourceShift = 48;
    sourceSize = 8;
    sPointer = source;
    //Sign padding for signed source.
    BitSetToInteger(sDest16, sPointer, sourceShift, sourceSize, true);
    if (sDest16 != 0x22) {
        printf("\n1\n");
        return false;
    }

    sourceShift = 36;
    sourceSize = 5;
    sPointer = source;
    //Remains the same for unsigned source.
    BitSetToInteger(sDest16, sPointer, sourceShift, sourceSize, false);
    if (sDest16 != 0x11) {
        return false;
    }

    sourceShift = 32;
    sourceSize = 5;
    sPointer = source;
    //Saturation to zero for signed source and unsigned dest.
    BitSetToInteger(uDest16, sPointer, sourceShift, sourceSize, true);
    if (uDest16 != 0) {
        return false;
    }

    sourceShift = 32;
    sourceSize = 9;
    sPointer = source;
    //Saturation to minimum possible for 8 bits.
    BitSetToInteger(sDest16, sPointer, sourceShift, sourceSize, true);
    if (sDest16 != (int8) 0x80) {
        return false;
    }
    */
    /*---------------------------------------*/

    uint8 uDest8;
    int8 sDest8;

    sourceShift = 32;
    sourceSize = 5;
    sPointer = source;
    //Sign padding for signed source.
    BitSetToInteger(sDest8, sPointer, sourceShift, sourceSize, true);
    if (sDest8 != (int8) 0xf1) {
        return false;
    }

    sourceShift = 32;
    sourceSize = 5;
    sPointer = source;
    //Remains the same for unsigned source.
    BitSetToInteger(sDest8, sPointer, sourceShift, sourceSize, false);
    if (sDest8 != (int8) 0x11) {
        return false;
    }

    sourceShift = 32;
    sourceSize = 5;
    sPointer = source;
    //Saturation to zero for signed source and unsigned dest.
    BitSetToInteger(uDest8, sPointer, sourceShift, sourceSize, true);
    if (uDest8 != 0) {
        return false;
    }

    sourceShift = 32;
    sourceSize = 9;
    sPointer = source;
    //Saturation to minimum possible for 8 bits.
    BitSetToInteger(sDest8, sPointer, sourceShift, sourceSize, true);
    if (sDest8 != (int8) 0x80) {
        return false;
    }

    return true;
}

template<typename T>
bool BitSetToIntegerTest<T>::TestIntegerToBitSet() {

    uint32 dest[] = { 0x12345678, 0x12345678, 0x87654321, 0x12348765 };
    int32 sSource = -1;
    uint32 uSource = 0xdfffffff;

    uint8 destShift = 0;
    uint8 destSize = 32;
    //pointer to dest[1]
    uint32 *dPointer = dest + 1;

    //Copy a signed 32 bit number in the stream.
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource);
    if (sSource != (int32) dest[1]) {
        return false;
    }

    //Automatic shift to dest[2]. Copy a signed 32 bit number, with dest unsigned it should be saturated to 0.
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource);
    if ((int32) dest[2] != 0) {
        return false;
    }

    destShift = 0;
    dPointer = dest;
    //Copy an unsigned 32 bit number, with dest signed. Saturation to the max possible.
    IntegerToBitSet(dPointer, destShift, destSize, true, uSource);
    if ((int32) dest[0] != 0x7fffffff) {
        return false;
    }

    dest[0] = 0;
    destSize = 16;
    destShift = 16;
    //Another saturation because of the minor size of destination.
    sSource = 0x121234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource);

    if ((int32) dest[0] != 0x7fff0000) {
        return false;
    }

    destSize = 16;
    destShift = 0;
    //Saturation also with unsigned dest.
    sSource = 0x121234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource);
    if ((int32) dest[0] != 0x7fffffff) {
        return false;
    }

    int8 sSource8 = -1;
    destSize = 28;
    destShift = 96;
    dPointer = dest;
    dest[3] = 0;
    //Sign padding
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource8);
    if ((int32) dest[3] != 0x0fffffff) {
        return false;
    }

    return true;
}
