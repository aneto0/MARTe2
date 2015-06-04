/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
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

#include "GeneralDefinitions.h"
#include "BitSetToIntegerTest.h"
#include "stdio.h"

bool BitSetToIntegerTest::TestBitSetToBitSet() {

    //128 bit source
    uint32 source[] = { 0xffffffb5, 0x00001111, 0x22223333, 0x0 };
    //128 bit dest
    uint32 dest[] = { 0, 0, 0, 0 };

    uint8 destShift = 0;
    uint8 sourceShift = 0;
    uint8 destSize = 32;
    uint8 sourceSize = 32;
    uint32 *sPointer = source;
    uint32 *dPointer = dest;

    //Copy the first position in the first position
    BitSetToBitSet(dPointer, destShift, destSize, true, sPointer, sourceShift,
                   sourceSize, true);
    if (dest[0] != source[0]) {
        return False;
    }

    destShift = 32;
    sourceShift = 0;
    sPointer = source;
    dPointer = dest;
    //Copy the first position in the second position
    BitSetToBitSet(dPointer, destShift, destSize, true, sPointer, sourceShift,
                   sourceSize, true);
    if (dest[1] != source[0]) {
        return False;
    }

    //Copy 0x0000ffff (between first and second, the second is up) in the third position.
    destShift = 64;
    sourceShift = 16;
    sPointer = source;
    dPointer = dest;
    uint32 test = 0x1111ffff;
    BitSetToBitSet(dPointer, destShift, destSize, true, sPointer, sourceShift,
                   sourceSize, true);
    if (dest[2] != test) {
        return False;
    }

    //Take only 16 bit size. Sign padding should work.
    destShift = 64;
    sourceShift = 16;
    sourceSize = 16;
    destSize = 32;
    sPointer = source;
    dPointer = dest;
    test = 0xffffffff;
    BitSetToBitSet(dPointer, destShift, destSize, true, sPointer, sourceShift,
                   sourceSize, true);
    if (dest[2] != test) {
        return False;
    }

    //Take only 3 bit size with shift=1. No sign padding
    destShift = 64;
    sourceShift = 1;
    sourceSize = 3;
    destSize = 32;
    sPointer = source;
    dPointer = dest;
    test = 0x2;
    BitSetToBitSet(dPointer, destShift, destSize, true, sPointer, sourceShift,
                   sourceSize, true);
    if (dest[2] != test) {
        return False;
    }

    //destSize < sourceSize, saturation.
    destShift = 96;
    sourceShift = 0;
    sourceSize = 16;
    destSize = 5;
    sPointer = source;
    dPointer = dest;
    test = (int32) 0x10;
    BitSetToBitSet(dPointer, destShift, destSize, true, sPointer, sourceShift,
                   sourceSize, true);
    if (dest[3] != test) {
        return False;
    }

    //saturation for unsigned source and signed destination.
    destShift = 104;
    sourceShift = 16;
    sourceSize = 16;
    destSize = 5;
    sPointer = source;
    dPointer = dest;
    test = (int32) 0xf10;
    BitSetToBitSet(dPointer, destShift, destSize, true, sPointer, sourceShift,
                   sourceSize, false);
    if (dest[3] != test) {
        return False;
    }

    //saturation to zero for negative source and unsigned destination.
    destShift = 104;
    sourceShift = 16;
    sourceSize = 16;
    destSize = 5;
    sPointer = source;
    dPointer = dest;
    test = (int32) 0x10;
    BitSetToBitSet(dPointer, destShift, destSize, false, sPointer, sourceShift,
                   sourceSize, true);
    if (dest[3] != test) {
        return False;
    }

    //8 bit pointer
    destShift = 104;
    sourceShift = 64;
    sourceSize = 3;
    destSize = 8;
    uint8 *sPointer8 = (uint8*) source;
    uint8 *dPointer8 = (uint8*) dest;
    test = (int32) 0x310;
    BitSetToBitSet(dPointer8, destShift, destSize, false, sPointer8,
                   sourceShift, sourceSize, false);
    if (dest[3] != test) {
        return False;
    }

    //16 bit pointer. Not padding because source is unsigned. 
    destShift = 96;
    sourceShift = 80; //64+16
    sourceSize = 10;
    destSize = 16;
    uint16 *sPointer16 = (uint16*) source;
    uint16 *dPointer16 = (uint16*) dest;
    test = (int32) 0x222;
    BitSetToBitSet(dPointer16, destShift, destSize, false, sPointer16,
                   sourceShift, sourceSize, false);
    if (dest[3] != test) {
        return False;
    }

    //16 bit pointer to 32. Sign padding. 
    destShift = 104;
    sourceShift = 77; //64+13
    sourceSize = 5;
    destSize = 16;
    sPointer16 = (uint16*) source;
    dPointer16 = (uint16*) dest;
    test = (int32) 0xfff122;
    BitSetToBitSet(dPointer16, destShift, destSize, true, sPointer16,
                   sourceShift, sourceSize, true);
    if (dest[3] != test) {
        return False;
    }

    //64 bit pointer to 128. Padding 
    destShift = 32;
    sourceShift = 48; //64+13
    sourceSize = 30;
    destSize = 32;
    int64 *sPointer64 = (int64*) source;
    int64 *dPointer64 = (int64*) dest;
    test = (int32) 0xf3330000;
    BitSetToBitSet(dPointer64, destShift, destSize, true, sPointer64,
                   sourceShift, sourceSize, true);
    if (dest[1] != test) {
        return False;
    }

    return True;

}

bool BitSetToIntegerTest::TestBitSetToInteger() {
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
        return False;
    }

    sourceShift = 0;
    sPointer = source;
    //Copy an unsigned 32 bit number, with source signed it should be saturated to 0.
    BitSetToInteger(uDest, sPointer, sourceShift, sourceSize, true);
    if (uDest != 0) {
        return False;
    }

    sourceShift = 0;
    sPointer = source;
    //Copy an unsigned 32 bit number, with source unsigned.
    BitSetToInteger(uDest, sPointer, sourceShift, sourceSize, false);
    if (uDest != source[0]) {
        return False;
    }

    sourceShift = 16;
    sPointer = source;
    //Source shift
    BitSetToInteger(sDest, sPointer, sourceShift, sourceSize, true);
    if (sDest != 0x1111ffff) {
        return False;
    }

    //Source automatic shift
    BitSetToInteger(sDest, sPointer, sourceShift, sourceSize, true);
    if (sDest != 0x33330000) {
        return False;
    }

    uint8 uDest8;
    int8 sDest8;

    sourceShift = 32;
    sourceSize = 5;
    sPointer = source;
    //Sign padding for signed source.
    BitSetToInteger(sDest8, sPointer, sourceShift, sourceSize, true);
    if (sDest8 != (int8) 0xf1) {
        return False;
    }

    sourceShift = 32;
    sourceSize = 5;
    sPointer = source;
    //Remains the same for unsigned source.
    BitSetToInteger(sDest8, sPointer, sourceShift, sourceSize, false);
    if (sDest8 != (int8) 0x11) {
        return False;
    }

    sourceShift = 32;
    sourceSize = 5;
    sPointer = source;
    //Saturation to zero for signed source and unsigned dest.
    BitSetToInteger(uDest8, sPointer, sourceShift, sourceSize, true);
    if (uDest8 != 0) {
        return False;
    }

    sourceShift = 32;
    sourceSize = 9;
    sPointer = source;
    //Saturation to minimum possible for 8 bits.
    BitSetToInteger(sDest8, sPointer, sourceShift, sourceSize, true);
    if (sDest8 != (int8) 0x80) {
        return False;
    }

    return True;
}

bool BitSetToIntegerTest::TestIntegerToBitSet() {

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
        return False;
    }

    //Automatic shift to dest[2]. Copy a signed 32 bit number, with dest unsigned it should be saturated to 0.
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource);
    if ((int32) dest[2] != 0) {
        return False;
    }

    destShift = 0;
    dPointer = dest;
    //Copy an unsigned 32 bit number, with dest signed. Saturation to the max possible.
    IntegerToBitSet(dPointer, destShift, destSize, true, uSource);
    if ((int32) dest[0] != 0x7fffffff) {
        return False;
    }

    dest[0] = 0;
    destSize = 16;
    destShift = 16;
    //Another saturation because of the minor size of destination.
    sSource = 0x121234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource);

    if ((int32) dest[0] != 0x7fff0000) {
        return False;
    }

    destSize = 16;
    destShift = 0;
    //Saturation also with unsigned dest.
    sSource = 0x121234;
    dPointer = dest;
    IntegerToBitSet(dPointer, destShift, destSize, false, sSource);
    if ((int32) dest[0] != 0x7fffffff) {
        return False;
    }

    int8 sSource8 = -1;
    destSize = 28;
    destShift = 96;
    dPointer = dest;
    dest[3] = 0;
    //Sign padding
    IntegerToBitSet(dPointer, destShift, destSize, true, sSource8);
    if ((int32) dest[3] != 0x0fffffff) {
        return False;
    }

    return True;
}
