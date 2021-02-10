
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
//#if LOCAL_TEST
#include <cstdlib>
#define LOG_ERROR(X) printf("[%s:%d] ERROR: %s\n", __FILE__, __LINE__, #X)
#define ASSERT(X) if (!(X)) { LOG_ERROR(X); return false;}
//#else
//#define ASSERT(X) if (!(X)) return false;
//#endif

using namespace MARTe;

bool check(BitSet bitset, uint32 value) {
    ASSERT(bitset.GetByteSize() == 1);
    for (uint32 i = 0; i < 32; i++) {
        bool ref = value & (1u << i);
        ASSERT(bitset.Bit(i) == ref);
    }
    return true;
}

bool check(BitSet bitset, uint64 value) {
    ASSERT(bitset.GetByteSize() == 2);
    for (uint32 i = 0; i < 64; i++) {
        bool ref = value & ((uint64)1u << i);
        ASSERT(bitset.Bit(i) == ref);
    }
    return true;
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool BitSetTest::TestConstructors(){
    BitSet bitset_a; // default constructor
    ASSERT(check(bitset_a , 0u));
    
    BitSet bitset_b(0b11011u); // uint32 constructor
    ASSERT(check(bitset_b, 27u)); 

    uint64 value = (uint64)1u << 32; // 0x100000000
    BitSet bitset_c(value); // uint64 constructor
    ASSERT(check(bitset_c, value));

    BitSet bitset_d(bitset_b); // copy constructor
    ASSERT(check(bitset_d, 27u));
    return true;
}

bool BitSetTest::TestSetBitMethods() {
    BitSet bs(0b111u); // initialize
    ASSERT(check(bs, 7u));
    
    bs.Set(0, 0); // Set a bit to 0;
    ASSERT(check(bs, 6u));

    bs.Set(5, 1); // set a bit to 1;
    ASSERT(check(bs, 38u));

    ASSERT(bs.GetByteSize() == 1);
    bs.Set(32, 1); // it should increase the size of the array.
    ASSERT(bs.GetByteSize() == 2);
    uint64 ref = 0x100000026;
    ASSERT(check(bs, ref));

    bs.Set(32, 0); // it should maintain the same size.
    ASSERT(bs.GetByteSize() == 2);
    ref = 0x26;
    ASSERT(check(bs, ref));

    return true;
}

bool BitSetTest::TestAssingOperator() {
    BitSet bs = 0b1001u; // Assign 32 bit uint
    ASSERT(check(bs, 9u));

    uint64 ref = 0x1100000000;
    bs = ref; // Assign 64 bit uint
    ASSERT(check(bs, ref));

    BitSet bs_b(11u);
    ASSERT(check(bs_b, 11u));
    bs = bs_b; // Assign bitset
    ASSERT(check(bs, 11u));
    return true;   
}

bool BitSetTest::TestIntCastOperator(){
    BitSet bs(10u);
    ASSERT(check(bs, 0xAu));
    uint32 val32 = bs;
    ASSERT(val32 == 0xAu);

    bs.Set(32, 1);
    ASSERT(check(bs, (uint64)0x10000000A));
    uint64 val64 = bs;
    ASSERT(val64 == (uint64)0x10000000A); 
    return true;
}

bool  BitSetTest::TestOrOperator(){
    BitSet bs_a(0b101u);
    ASSERT(check(bs_a, 5u));
    BitSet bs_b(0b010u);
    ASSERT(check(bs_b, 2u));
    BitSet bs_c = bs_a | bs_b; // bitset | bitset
    ASSERT(check(bs_c, 7u));

    bs_b = (uint64)0x100000002;
    bs_c = bs_a | bs_b;
    ASSERT(bs_c.GetByteSize() == 2);
    ASSERT(check(bs_c, (uint64)0x100000007)); 

    bs_a |= BitSet(0b1000u); // |= bitset
    ASSERT(check(bs_a, 13u));

    return true;
}

bool BitSetTest::TestAndOperator() {
    BitSet bs_a(0b101u);
    ASSERT(check(bs_a, 5u));
    BitSet bs_b(0b010u);
    ASSERT(check(bs_b, 2u));
    BitSet bs_c = bs_a & bs_b; // bitset & bitset
    ASSERT(check(bs_c, 0u));

    bs_b = (uint64)0x100000002;
    ASSERT(check(bs_a & bs_b, (uint64)0)); 

    bs_a &= BitSet(0b100u); // &= bitset
    ASSERT(check(bs_a, 4u));

    return true;
}

bool BitSetTest::TestXorOperator(){
    BitSet bs_a(0b101u);
    ASSERT(check(bs_a, 5u));
    BitSet bs_b(0b011u);
    ASSERT(check(bs_b, 2u));
    BitSet bs_c = bs_a ^ bs_b; // bitset ^ bitset
    ASSERT(check(bs_c, 6u));

    bs_b = (uint64)0x100000002;
    ASSERT(check(bs_a ^ bs_b, (uint64)0x100000006)); 

    bs_a ^= BitSet(0b100u); // ^= bitset
    ASSERT(check(bs_a, 1u));
    return true;
}

bool BitSetTest::TestNotOperator(){
    BitSet bs_a(0b101u);
    ASSERT(check(bs_a, 5u));
    BitSet bs_b = ~bs_a;
    ASSERT(bs_b.GetByteSize() == 1);
    ASSERT(bs_b != bs_a);
    ASSERT(check(bs_b, (uint32)0xFFFFFFFA));

    bs_a.Set(32, true);
    ASSERT(check(bs_a, (uint64)0x100000005));
    bs_b = ~bs_a;
    ASSERT(bs_b.GetByteSize() == 2);
    ASSERT(check(bs_b, (uint64)0xFFFFFFFEFFFFFFFA));
    return true;
}

bool BitSetTest::TestEquality(){
    BitSet bs_a(0b101u);
    ASSERT(bs_a == 0b101u);

    BitSet bs_b(0b101u);
    ASSERT(bs_a == bs_b);

    bs_b.Set(32, 1);
    bs_b.Set(32, 0);
    ASSERT(bs_b.GetByteSize() == 2);
    ASSERT(bs_a == bs_b);
    
    ASSERT(bs_a == (uint64)5);
    return true;
}

bool BitSetTest::TestDisequality(){
    BitSet bs_a(0b101u);
    ASSERT(bs_a != 0b111u);

    BitSet bs_b(0b111u);
    ASSERT(bs_a != bs_b);

    bs_b.Set(1, 0);
    bs_b.Set(32, 1);
    ASSERT(bs_b.GetByteSize() == 2);
    ASSERT(bs_a != bs_b);
    
    ASSERT(bs_a != (uint64)7);
    return true;
}

bool BitSetTest::TestLeftShift(){
    BitSet bs(0b1u);
    ASSERT(bs == 0b1u);
    ASSERT(bs.GetByteSize() == 1);
    bs = bs << 1u;
    ASSERT(bs.GetByteSize() == 2);
    ASSERT(bs == 0b10u);
    BitSet bs_b(0b10u);
    ASSERT(bs == bs_b);
    return true;
}

bool BitSetTest::TestRightShift(){
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