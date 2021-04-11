/**
 * @file BitSetTest.cpp
 * @brief Source file for class BitSetTest
 * @date 05/02/2021
 * @author Martino Ferrari
 *
 * @copyright Copyright 2021 ITER
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
 * the class BitSetTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BitSetTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#if ENVIRONMENT==Linux && ARCHITECTURE==x86_gcc
#include <cstdio>
#define LOGERROR(X) printf("[%s:%d] ERROR: %s\n", __FILE__, __LINE__, #X)
#define ASSERT(X) if (!(X)) { LOGERROR(X); return false;}
#else
#define ASSERT(X) if (!(X)) return false;
#endif

using namespace MARTe;

bool Check(BitSet bitset, uint32 value) {
    ASSERT(bitset.GetNumberOfElements() == 1);
    for (uint32 i = 0; i < 32; i++) {
        bool ref = value & (1u << i);
        ASSERT(bitset.Bit(i) == ref);
    }
    return true;
}

bool Check(BitSet bitset, uint64 value) {
    ASSERT(bitset.GetNumberOfElements() == 2);
    for (uint32 i = 0; i < 64; i++) {
        bool ref = value & ((uint64) 1u << i);
        ASSERT(bitset.Bit(i) == ref);
    }
    return true;
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool BitSetTest::TestConstructors() {
    BitSet bitsetA; // default constructor
    ASSERT(Check(bitsetA, 0u));

    BitSet bitsetB(0b11011u); // uint32 constructor
    ASSERT(Check(bitsetB, 27u));

    uint64 value = (uint64) 1u << 32; // 0x100000000
    BitSet bitsetC(value); // uint64 constructor
    ASSERT(Check(bitsetC, value));

    BitSet bitsetD(bitsetB); // copy constructor
    ASSERT(Check(bitsetD, 27u));

    uint32 array[]={1u, 2u, 3u, 4u}; //generic constructor
    BitSet bitset(array, 4u);
    uint32 *retArray=bitset.GetArray();
    ASSERT(Check(retArray[0], array[0]));
    ASSERT(Check(retArray[1], array[1]));
    ASSERT(Check(retArray[2], array[2]));
    ASSERT(Check(retArray[3], array[3]));

    return true;
}

bool BitSetTest::TestGetNumberOfElements() {
    BitSet bitset;
    ASSERT(bitset.GetNumberOfElements() == 1u);
    BitSet bitsetU32(1u);
    ASSERT(bitsetU32.GetNumberOfElements() == 1u);
    BitSet bitsetU64(static_cast<uint64>(1u));
    ASSERT(bitsetU64.GetNumberOfElements() == 2u);
    bitset.Set(32, 1);
    ASSERT(bitset.GetNumberOfElements() == 2u);
    bitset.Set(64, 1);
    ASSERT(bitset.GetNumberOfElements() == 3u);

    return true;
}

bool BitSetTest::TestSetBitMethods() {
    BitSet bs(0b111u); // initialize
    ASSERT(Check(bs, 7u));

    bs.Set(0, 0); // Set a bit to 0;
    ASSERT(Check(bs, 6u));

    bs.Set(5, 1); // set a bit to 1;
    ASSERT(Check(bs, 38u));

    ASSERT(bs.GetNumberOfElements() == 1);
    bs.Set(32, 1); // it should increase the size of the array.
    ASSERT(bs.GetNumberOfElements() == 2);
    uint64 ref = 0x100000026;
    ASSERT(Check(bs, ref));

    bs.Set(32, 0); // it should maintain the same size.
    ASSERT(bs.GetNumberOfElements() == 2);
    ref = 0x26;
    ASSERT(Check(bs, ref));

    return true;
}

bool BitSetTest::TestAssingOperator() {
    BitSet bs = 0b1001u; // Assign 32 bit uint
    ASSERT(Check(bs, 9u));

    uint64 ref = 0x1100000000;
    bs = ref; // Assign 64 bit uint
    ASSERT(Check(bs, ref));

    BitSet bsB(11u);
    ASSERT(Check(bsB, 11u));
    bs = bsB; // Assign bitset
    ASSERT(Check(bs, 11u));
    return true;
}

bool BitSetTest::TestIntCastOperator() {
    BitSet bs(10u);
    ASSERT(Check(bs, 0xAu));
    uint32 val32 = bs;
    ASSERT(val32 == 0xAu);

    bs.Set(32, 1);
    ASSERT(Check(bs, (uint64 )0x10000000A));
    uint64 val64 = bs;
    ASSERT(val64 == (uint64 )0x10000000A);
    return true;
}

bool BitSetTest::TestOrOperator() {
    BitSet bsA(0b101u);
    ASSERT(Check(bsA, 5u));
    BitSet bsB(0b010u);
    ASSERT(Check(bsB, 2u));
    BitSet bsC = bsA | bsB; // bitset | bitset
    ASSERT(Check(bsC, 7u));

    bsB = (uint64) 0x100000002;
    bsC = bsA | bsB;
    ASSERT(bsC.GetNumberOfElements() == 2);
    ASSERT(Check(bsC, (uint64 )0x100000007));

    bsA |= BitSet(0b1000u); // |= bitset
    ASSERT(Check(bsA, 13u));

    return true;
}

bool BitSetTest::TestAndOperator() {
    BitSet bsA(0b101u);
    ASSERT(Check(bsA, 5u));
    BitSet bsB(0b010u);
    ASSERT(Check(bsB, 2u));
    BitSet bsC = bsA & bsB; // bitset & bitset
    ASSERT(Check(bsC, 0u));

    bsB = (uint64) 0x100000002;
    ASSERT(Check(bsA & bsB, (uint64 )0));

    bsA &= BitSet(0b100u); // &= bitset
    ASSERT(Check(bsA, 4u));

    return true;
}

bool BitSetTest::TestXorOperator() {
    BitSet bsA(0b101u);
    ASSERT(Check(bsA, 5u));
    BitSet bsB(0b011u);
    ASSERT(Check(bsB, 3u));
    BitSet bsC = bsA ^ bsB; // bitset ^ bitset
    ASSERT(Check(bsC, 6u));

    bsB = (uint64)0x100000002;
    ASSERT(Check(bsA ^ bsB, (uint64)0x100000007)); 

    bsA ^= BitSet(0b100u); // ^= bitset
    ASSERT(Check(bsA, 1u));
    return true;
}

bool BitSetTest::TestNotOperator() {
    BitSet bsA(0b101u);
    ASSERT(Check(bsA, 5u));
    BitSet bsB = ~bsA;
    ASSERT(bsB.GetNumberOfElements() == 1);
    ASSERT(bsB != bsA);
    ASSERT(Check(bsB, (uint32 )0xFFFFFFFA));

    bsA.Set(32, true);
    ASSERT(Check(bsA, (uint64 )0x100000005));
    bsB = ~bsA;
    ASSERT(bsB.GetNumberOfElements() == 2);
    ASSERT(Check(bsB, (uint64 )0xFFFFFFFEFFFFFFFA));
    return true;
}

bool BitSetTest::TestEquality() {
    BitSet bsA(0b101u);
    ASSERT(bsA == 0b101u);

    BitSet bsB(0b101u);
    ASSERT(bsA == bsB);

    bsB.Set(32, 1);
    bsB.Set(32, 0);
    ASSERT(bsB.GetNumberOfElements() == 2);
    ASSERT(bsA == bsB);
    bsB.Set(32, 1);
    ASSERT(!(bsA == bsB));
    uint32 v32 = 6u;
    ASSERT(!(bsA == v32));
    uint64 v64 = 6u;
    ASSERT(!(bsA == v64));
    v32 = 5u;
    v64 = 5u;
    ASSERT(!(bsB == v32));
    ASSERT(!(bsB == v64));
    
    // Check longer bitmasks.
    v64 = bsB;
    ASSERT(bsB == v64);
    bsB.Set(64, 1);
    bsB.Set(64, 0);
    ASSERT(bsB == v64);
    bsB.Set(64, 1);
    ASSERT(!(bsB == v64));

    return true;
}

bool BitSetTest::TestDisequality() {
    BitSet bsA(0b101u);
    ASSERT(bsA != 0b111u);

    BitSet bsB(0b111u);
    ASSERT(bsA != bsB);

    bsB.Set(1, 0);
    bsB.Set(32, 1);
    ASSERT(bsB.GetNumberOfElements() == 2);
    ASSERT(bsA != bsB);

    ASSERT(bsA != (uint64 )7);
    return true;
}

bool BitSetTest::TestLeftShift() {
    BitSet bs(0b1u);
    ASSERT(bs == 0b1u);
    ASSERT(bs.GetNumberOfElements() == 1);
    bs = bs << 1u;
    ASSERT(bs.GetNumberOfElements() == 2);
    ASSERT(bs == 0b10u);
    BitSet bsB(0b10u);
    ASSERT(bs == bsB);
    return true;
}

bool BitSetTest::TestRightShift() {
    BitSet bs;
    bs.Set(1, 1);
    ASSERT(bs == 2u);
    bs.Set(0, 1);
    ASSERT(bs == 3u);
    bs = bs >> 1u;
    ASSERT(bs.Bit(0) && !bs.Bit(1) && !bs.Bit(2));
    bs = bs << 1u;
    ASSERT(!bs.Bit(0) && bs.Bit(1) && !bs.Bit(2));
    return true;
}

bool BitSetTest::TestGetArray(){

    uint32 array[]={1u, 2u, 3u, 4u}; //generic constructor
    BitSet bitset(array, 4u);
    uint32 *retArray=bitset.GetArray();
    ASSERT(Check(retArray[0], array[0]));
    ASSERT(Check(retArray[1], array[1]));
    ASSERT(Check(retArray[2], array[2]));
    ASSERT(Check(retArray[3], array[3]));
    return true;
}


