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
#include "DoubleIntegerTest.h"
#include "StringTestHelper.h"
#include "StreamTestHelper.h"
#include "stdio.h"

bool DoubleIntegerTest::TestShift() {
    //init a 64 bit integer.
    int64 assign = -1;

    DoubleInteger< int32 > number = assign;

    //== operator
    if (!(number == -1)) {
        return False;
    }

    DoubleInteger< int32 > sbit64(0xf000000000000000);

    //!= operator
    if (sbit64 != 0xf000000000000000) {
        return False;
    }

    //Math shift with sign extension.
    if ((sbit64 >> 60) != -1) {
        return False;
    }

    //Copy bit a bit.
    DoubleInteger< uint32 > ubit64(0xf000000000000000);

    //Math shift without sign extension.
    if ((ubit64 >> 60) != 0xf) {
        return False;
    }

    sbit64 = 0xf;

    //left shift.
    if ((sbit64 << 4) != 0xf0) {
        return False;
    }

    //left shift.
    if ((sbit64 << 63) != 0x8000000000000000) {
        return False;
    }

    return True;
}

bool DoubleIntegerTest::TestLogicalOperators() {

    DoubleInteger< int32 > test1(0x00ffffffffffffff);
    DoubleInteger< int32 > test2(0xff00000000000000);

    //Test or
    if ((test1 | test2) != 0xffffffffffffffff) {
        return False;
    }

    test1 |= test2;

    if (test1 != 0xffffffffffffffff) {
        return False;
    }

    //Test and	
    if ((test1 & test2) != test2) {
        return False;
    }

    test1 &= 0;

    if (test1 != 0) {
        return False;
    }

    test1 = test2 + 1;

    //Test <
    if (test1 < test2) {
        return False;
    }

    DoubleInteger< int32 > zero;

    //Test <=
    if (test1 <= test2) {
        return False;
    }

    if (!(zero <= 0)) {
        return False;
    }

    //Test >
    if (test2 > test1) {
        return False;
    }

    //Test >=
    if (test2 >= test1) {
        return False;
    }

    if (!(zero >= 0)) {
        return False;
    }

    test1 = -1;
    test2 = -2;

    if (test2 >= test1) {
        return False;
    }

    return True;
}

bool DoubleIntegerTest::TestMathematicOperators() {

    DoubleInteger< int32 > sbit64;
    DoubleInteger< uint32 > ubit64;
/////////////Subtraction

//Subtraction with positive-negative lowers, upper--
    ubit64 = 0xf7fffffff;
    DoubleInteger< uint32 > uAddend(0xefffffffe);

    ubit64 -= uAddend;

    sbit64 = 0xf7fffffff;
    DoubleInteger< int32 > sAddend(0xefffffffe);

    sbit64 -= sAddend;

    if ((int32) (ubit64.upper) != sbit64.upper
            || ubit64.lower != (uint32) sbit64.lower || ubit64.upper != 0
            || sbit64.upper != 0) {
        return False;
    }

    //Subtraction with negative-positive lowers, uppers should not decrement.
    ubit64 = 0xfffffffff;
    uAddend = 0xe7ffffffe;

    DoubleInteger< uint32 > test1 = ubit64 - uAddend;

    sbit64 = 0xfffffffff;
    sAddend = 0xe7ffffffe;

    DoubleInteger< int32 > test2 = sbit64 - sAddend;

    if ((int32) (test1.upper) != test2.upper
            || test1.lower != (uint32) test2.lower || test1.upper != 1
            || test2.upper != 1) {
        return False;
    }

    //Subtraction with positive-positive lowers, upper decrement.
    ubit64 = 0xf6fffffff;
    uAddend = 0xe7ffffffe;

    test1 = ubit64 - uAddend;

    sbit64 = 0xf6fffffff;
    sAddend = 0xe7ffffffe;

    test2 = sbit64 - sAddend;

    if ((int32) (test1.upper) != test2.upper
            || test1.lower != (uint32) test2.lower || test1.upper != 0
            || test2.upper != 0) {
        return False;
    }

    //Subtraction with negative-negative lowers, upper decrement.
    ubit64 = 0xfefffffff;
    uAddend = 0xefffffffe;

    test1 = ubit64 - uAddend;

    sbit64 = 0xfefffffff;
    sAddend = 0xefffffffe;

    test2 = sbit64 - sAddend;

    if ((int32) (test1.upper) != test2.upper
            || test1.lower != (uint32) test2.lower || test1.upper != 0
            || test2.upper != 0) {
        return False;
    }

/////////////Sum

//Addiction with positive-negative lowers, upper++
    ubit64 = 0x7fffffff;
    uAddend = (0xfffffffe);

    ubit64 += uAddend;

    sbit64 = 0x7fffffff;
    sAddend = 0xfffffffe;

    sbit64 += sAddend;

    if ((int32) (ubit64.upper) != sbit64.upper
            || ubit64.lower != (uint32) sbit64.lower || ubit64.upper != 1
            || sbit64.upper != 1) {
        return False;
    }

    //Subtraction with negative-positive lowers, uppers ++.
    ubit64 = 0xffffffff;
    uAddend = 0x7ffffffe;

    test1 = ubit64 + uAddend;

    sbit64 = 0xffffffff;
    sAddend = 0x7ffffffe;

    test2 = sbit64 + sAddend;

    if ((int32) (test1.upper) != test2.upper
            || test1.lower != (uint32) test2.lower || test1.upper != 1
            || test2.upper != 1) {

        return False;
    }

    //Subtraction with positive-positive lowers.
    ubit64 = 0x7fffffff;
    uAddend = 0x7ffffffe;

    test1 = ubit64 + uAddend;

    sbit64 = 0x7fffffff;
    sAddend = 0x7ffffffe;

    test2 = sbit64 + sAddend;

    if ((int32) (test1.upper) != test2.upper
            || test1.lower != (uint32) test2.lower || test1.upper != 0
            || test2.upper != 0) {

        return False;
    }

    //Subtraction with negative-negative lowers, upper ++.
    ubit64 = 0xefffffff;
    uAddend = 0xfffffffe;

    test1 = ubit64 + uAddend;

    sbit64 = 0xefffffff;
    sAddend = 0xfffffffe;

    test2 = sbit64 + sAddend;

    if ((int32) (test1.upper) != test2.upper
            || test1.lower != (uint32) test2.lower || test1.upper != 1
            || test2.upper != 1) {
        return False;
    }

    //Test operation with negative numbers.
    sbit64 = -1;
    test2 = -2;

    if (sbit64 + test2 != -3) {
        return False;
    }

    if (test2 - sbit64 != -1) {
        return False;
    }

    return True;
}
