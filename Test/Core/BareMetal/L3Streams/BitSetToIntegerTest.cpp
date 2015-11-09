/**
 * @file BitSetToIntegerTest.cpp
 * @brief Source file for class BitSetToIntegerTest
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
 * the class BitSetToIntegerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

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

bool BitSetToIntegerTest::TestBitSetToInteger_64() {

    //128 bit source  [4294967221 -68, 4369, 572666675, 0]
    uint32 source[] = { 0xffffffb5, 0x00001111, 0x22223333, 0x0 };
    //128 bit dest
    int64 sDest;
    uint64 uDest;

    uint8 sourceShift = 0;
    uint8 sourceSize = 64;
    uint32 *sPointer = source;

    //Copy a signed 32 bit number.
    BitSetToInteger(sDest, sPointer, sourceShift, sourceSize, true);
    uint64 test = source[1];
    test <<= 32;
    test += source[0];
    if (sDest != (int64) test) {
        return false;
    }

    sourceShift = 0;
    sourceSize = 33;
    sPointer = source;
    //Copy an unsigned 32 bit number, with source signed it should be saturated to 0.
    BitSetToInteger(uDest, sPointer, sourceShift, sourceSize, true);
    if (uDest != 0) {

        return false;
    }

    sourceShift = 0;
    sPointer = source;
    //Copy an unsigned 32 bit number, with source unsigned.
    test = 1;
    test <<= 32;
    test += source[0];
    BitSetToInteger(uDest, sPointer, sourceShift, sourceSize, false);
    if (uDest != test) {

        return false;
    }

    sourceShift = 16;
    sourceSize = 32;

    sPointer = source;
    //Source shift
    BitSetToInteger(sDest, sPointer, sourceShift, sourceSize, true);
    if (sDest != 0x1111ffff) {

        return false;
    }

    sourceSize = 17;

    //Source automatic shift
    BitSetToInteger(sDest, sPointer, sourceShift, sourceSize, true);
    if (sDest !=(int64) 0xffffffffffff0000) {

        return false;
    }

    return true;
}

bool BitSetToIntegerTest::TestBitSetToInteger_32() {

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

    return true;
}

bool BitSetToIntegerTest::TestBitSetToInteger_16() {

    /*---------------------------------------*/

    uint32 source[] = { 0xffffffb5, 0x00001111, 0x22223333, 0x0 };
    uint16 uDest16;
    int16 sDest16;

    uint8 sourceShift = 65;
    uint8 sourceSize = 8;
    uint32* sPointer = source;
    //Sign padding for signed source.
    BitSetToInteger(sDest16, sPointer, sourceShift, sourceSize, true);
    if (sDest16 != (int16) 0xff99) {
        return false;
    }

    sourceShift = 65;
    sourceSize = 8;
    sPointer = source;
    //Remains the same for unsigned source.
    BitSetToInteger(sDest16, sPointer, sourceShift, sourceSize, false);
    if (sDest16 != (int16) 0x99) {
        return false;
    }

    sourceShift = 5;
    sourceSize = 7;
    sPointer = source;
    //Saturation to zero for signed source and unsigned dest.
    BitSetToInteger(uDest16, sPointer, sourceShift, sourceSize, true);
    if (uDest16 != 0) {
        return false;
    }

    sourceShift = 15;
    sourceSize = 16;
    sPointer = source;
    //Saturation to maximum possible for 8 bits.
    BitSetToInteger(sDest16, sPointer, sourceShift, sourceSize, false);
    if (sDest16 != (int16) 0x7fff) {
        return false;
    }

    sourceShift = 17;
    sourceSize = 17;
    sPointer = source;
    //Saturation to maximum possible for 8 bits.
    BitSetToInteger(uDest16, sPointer, sourceShift, sourceSize, false);
    if (uDest16 != 0xffff) {
        return false;
    }

    sourceShift = 65;
    sourceSize = 17;
    sPointer = source;
    //Saturation to minimum possible for 8 bits.
    BitSetToInteger(sDest16, sPointer, sourceShift, sourceSize, true);
    if (sDest16 != (int16) 0x8000) {
        return false;
    }
    return true;
}

bool BitSetToIntegerTest::TestBitSetToInteger_8() {

    /*---------------------------------------*/
    uint32 source[] = { 0xffffffb5, 0x00001111, 0x22223333, 0x0 };

    uint8 uDest8;
    int8 sDest8;

    uint8 sourceShift = 32;
    uint8 sourceSize = 5;
    uint32 *sPointer = source;
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

bool BitSetToIntegerTest::TestIntegerToBitSet_64() {

    uint32 dest[] = { 0x12345678, 0x12345678, 0x87654321, 0x12348765 };
    int64 sSource64 = -1;
    uint64 uSource64 = 0xdfffffffffffffff;

    uint8 destShift = 32;
    uint8 destSize = 64;
    //pointer to dest[1]
    uint32 *dPointer = dest;

    //Copy a signed 64 bit number in the stream.
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource64);
    uint64 test = dest[2];
    test <<= 32;
    test += dest[1];
    if (sSource64 != (int64) test) {

        return false;
    }

    //Automatic shift to dest[3]. Copy a signed 64 bit number, with dest unsigned it should be saturated to 0.
    destSize = 16;

    IntegerToBitSet(dPointer, destShift, destSize, false, sSource64);
    if ((int32) dest[3] != 0x12340000) {

        return false;
    }

    destShift = 0;
    dPointer = dest;
    destSize = 64;
    //Copy an unsigned 64 bit number, with dest signed. Saturation to the max possible.
    IntegerToBitSet(dPointer, destShift, destSize, true, uSource64);
    test = dest[1];
    test <<= 32;
    test += dest[0];
    if (test != 0x7fffffffffffffff) {

        return false;
    }

    dest[0] = 0;
    destSize = 32;
    destShift = 32;
    //Another saturation because of the minor size of destination.
    sSource64 = (int64) 0x121234121234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource64);
    test = dest[1];
    test <<= 32;
    test += dest[0];
    if (test != 0x7fffffff00000000) {

        return false;
    }

    destSize = 32;
    destShift = 0;
    //Saturation also with unsigned dest.
    sSource64 = (int64) 0x121234121234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource64);
    if ((int32) dest[0] != -1) {

        return false;
    }
    return true;
}

bool BitSetToIntegerTest::TestIntegerToBitSet_32() {

    uint32 dest[] = { 0x12345678, 0x12345678, 0x87654321, 0x12348765 };
    int32 sSource32 = -1;
    uint32 uSource32 = 0xdfffffff;

    uint8 destShift = 0;
    uint8 destSize = 32;
    //pointer to dest[1]
    uint32 *dPointer = dest + 1;

    //Copy a signed 32 bit number in the stream.
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource32);
    if (sSource32 != (int32) dest[1]) {
        return false;
    }

    //Automatic shift to dest[2]. Copy a signed 32 bit number, with dest unsigned it should be saturated to 0.
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource32);
    if ((int32) dest[2] != 0) {
        return false;
    }

    destShift = 0;
    dPointer = dest;
    //Copy an unsigned 32 bit number, with dest signed. Saturation to the max possible.
    IntegerToBitSet(dPointer, destShift, destSize, true, uSource32);
    if ((int32) dest[0] != 0x7fffffff) {
        return false;
    }

    dest[0] = 0;
    destSize = 16;
    destShift = 16;
    //Another saturation because of the minor size of destination.
    sSource32 = (int32) 0x121234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource32);

    if ((int32) dest[0] != 0x7fff0000) {
        return false;
    }

    destSize = 16;
    destShift = 0;
    //Saturation also with unsigned dest.
    sSource32 = (int32) 0x121234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource32);
    if ((int32) dest[0] != 0x7fffffff) {
        return false;
    }
    return true;
}

bool BitSetToIntegerTest::TestIntegerToBitSet_16() {

    uint32 dest[] = { 0x12345678, 0x12345678, 0x87654321, 0x12348765 };

    /*--------------------------------------------------*/

    //pointer to dest[1]
    uint32 *dPointer = dest + 1;

    int16 sSource16 = -1;
    uint16 uSource16 = 0xdfff;

    uint8 destShift = 0;
    uint8 destSize = 32;

    //Copy a signed 16 bit number in the stream.
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource16);
    if (sSource16 != (int16)dest[1]) {

        return false;
    }

    //Automatic shift. Copy a signed 16 bit number, with dest unsigned it should be saturated to 0.
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource16);
    if (dest[2] != 0) {
        return false;
    }

    destShift = 0;
    dPointer = dest;
    destSize = 16;
    //Copy an unsigned 16 bit number, with dest signed. Saturation to the max possible.
    IntegerToBitSet(dPointer, destShift, destSize, true, uSource16);
    if (dest[0] != (int32) 0x12347fff) {
        return false;
    }

    dest[0] = 0;
    destSize = 15;
    destShift = 16;
    //Another saturation because of the minor size of destination.
    sSource16 = (int16) 0x7234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource16);

    if (dest[0] != (int32) 0x3fff0000) {
        return false;
    }

    destSize = 8;
    destShift = 71;
    //Saturation also with unsigned dest.
    sSource16 = (int16) 0xff80;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource16);
    if ((int8) dest[2] >> 7 != 0) {
        return false;
    }

    return true;
}

bool BitSetToIntegerTest::TestIntegerToBitSet_8() {

    uint32 dest = 0;

    /*--------------------------------------------------*/

    int8 sSource8 = -1;
    uint8 destSize = 28;
    uint8 destShift = 0;
    uint32* dPointer = &dest;
    dest = 0;
    //Sign padding
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource8);
    if (dest != (int32) 0x0fffffff) {
        return false;
    }

    sSource8 = 0x80;
    dest = 0x12300456;
    destSize = 7;
    destShift = 12;
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource8);
    if (dest != (int32) 0x12340456) {
        return false;
    }

    sSource8 = 0x80;
    dest = 0x12300456;
    destSize = 0;
    destShift = 12;
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource8);
    if (dest != (int32) 0x12300456) {
        return false;
    }

    sSource8 = 0x80;
    dest = 0x12300456;
    destSize = 1;
    destShift = 12;
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource8);
    if (dest != (int32) 0x12301456) {
        return false;
    }

    uint8 uSource8 = 0x1;
    dest = 0x12300456;
    destSize = 1;
    destShift = 12;
    IntegerToBitSet(dPointer, destShift, destSize, true, uSource8);
    if (dest != (int32) 0x12300456) {
        return false;
    }

    uSource8 = 0x1;
    dest = 0x12300456;
    destSize = 1;
    destShift = 12;
    IntegerToBitSet(dPointer, destShift, destSize, false, uSource8);
    if (dest != (int32) 0x12301456) {
        return false;
    }

    uSource8 = 0x1;
    dest = 0x12300456;
    destSize = 0;
    destShift = 12;
    IntegerToBitSet(dPointer, destShift, destSize, false, uSource8);
    if (dest != (int32) 0x12300456) {
        return false;
    }

    return true;
}
