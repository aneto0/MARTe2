/**
 * @file DoubleIntegerTest.cpp
 * @brief Source file for class DoubleIntegerTest
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
 * the class DoubleIntegerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


#include "GeneralDefinitions.h"
#include "DoubleIntegerTest.h"

#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

/*
using namespace MARTe;

bool DoubleIntegerTest::TestShift() {
    //init a 64 bit integer.
    int64 assign = -1;

    DoubleInteger< int32 > number = assign;

    //== operator
    if (!(number == -1)) {
        return false;
    }

    DoubleInteger< int32 > sbit64(0xf000000000000000);

    //!= operator
    if (sbit64 != 0xf000000000000000) {
        return false;
    }

    //Math shift with sign extension.
    if ((sbit64 >> 60) != -1) {
        return false;
    }

    //Copy bit a bit.
    DoubleInteger< uint32 > ubit64(0xf000000000000000);

    //Math shift without sign extension.
    if ((ubit64 >> 60) != 0xf) {
        return false;
    }

    sbit64 = 0xf;

    //left shift.
    if ((sbit64 << 4) != 0xf0) {
        return false;
    }

    //left shift.
    if ((sbit64 << 63) != 0x8000000000000000) {
        return false;
    }

    return true;
}

bool DoubleIntegerTest::TestLogicalOperators() {

    DoubleInteger< int32 > test1(0x00ffffffffffffff);
    DoubleInteger< int32 > test2(0xff00000000000000);

    //Test or
    if ((test1 | test2) != 0xffffffffffffffff) {
        return false;
    }

    test1 |= test2;

    if (test1 != 0xffffffffffffffff) {
        return false;
    }

    //Test and
    if ((test1 & test2) != test2) {
        return false;
    }

    test1 &= 0;

    if (test1 != 0) {
        return false;
    }

    test1 = test2 + 1;

    //Test <
    if (test1 < test2) {
        return false;
    }

    DoubleInteger< int32 > zero;

    //Test <=
    if (test1 <= test2) {
        return false;
    }

    if (!(zero <= 0)) {
        return false;
    }

    //Test >
    if (test2 > test1) {
        return false;
    }

    //Test >=
    if (test2 >= test1) {
        return false;
    }

    if (!(zero >= 0)) {
        return false;
    }

    test1 = -1;
    test2 = -2;

    if (test2 >= test1) {
        return false;
    }

    return true;
}

bool DoubleIntegerTest::TestMathematicOperators() {

    DoubleInteger< int32 > sbit64;
    DoubleInteger< uint32 > ubit64;
/////////////Subtraction

//Subtraction with positive-negative lowers, GetUpper()--
    ubit64 = 0xf7fffffff;
    DoubleInteger< uint32 > uAddend(0xefffffffe);

    ubit64 -= uAddend;

    sbit64 = 0xf7fffffff;
    DoubleInteger< int32 > sAddend(0xefffffffe);

    sbit64 -= sAddend;

    if ((int32) (ubit64.GetUpper()) != sbit64.GetUpper()
            || ubit64.GetLower() != (uint32) sbit64.GetLower() || ubit64.GetUpper() != 0
            || sbit64.GetUpper() != 0) {
        return false;
    }

    //Subtraction with negative-positive lowers, uppers should not decrement.
    ubit64 = 0xfffffffff;
    uAddend = 0xe7ffffffe;

    DoubleInteger< uint32 > test1 = ubit64 - uAddend;

    sbit64 = 0xfffffffff;
    sAddend = 0xe7ffffffe;

    DoubleInteger< int32 > test2 = sbit64 - sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper()
            || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 1
            || test2.GetUpper() != 1) {
        return false;
    }

    //Subtraction with positive-positive lowers, GetUpper() decrement.
    ubit64 = 0xf6fffffff;
    uAddend = 0xe7ffffffe;

    test1 = ubit64 - uAddend;

    sbit64 = 0xf6fffffff;
    sAddend = 0xe7ffffffe;

    test2 = sbit64 - sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper()
            || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 0
            || test2.GetUpper() != 0) {
        return false;
    }

    //Subtraction with negative-negative lowers, GetUpper() decrement.
    ubit64 = 0xfefffffff;
    uAddend = 0xefffffffe;

    test1 = ubit64 - uAddend;

    sbit64 = 0xfefffffff;
    sAddend = 0xefffffffe;

    test2 = sbit64 - sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper()
            || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 0
            || test2.GetUpper() != 0) {
        return false;
    }

/////////////Sum

//Addiction with positive-negative lowers, GetUpper()++
    ubit64 = 0x7fffffff;
    uAddend = (0xfffffffe);

    ubit64 += uAddend;

    sbit64 = 0x7fffffff;
    sAddend = 0xfffffffe;

    sbit64 += sAddend;

    if ((int32) (ubit64.GetUpper()) != sbit64.GetUpper()
            || ubit64.GetLower() != (uint32) sbit64.GetLower() || ubit64.GetUpper() != 1
            || sbit64.GetUpper() != 1) {
        return false;
    }

    //Subtraction with negative-positive lowers, uppers ++.
    ubit64 = 0xffffffff;
    uAddend = 0x7ffffffe;

    test1 = ubit64 + uAddend;

    sbit64 = 0xffffffff;
    sAddend = 0x7ffffffe;

    test2 = sbit64 + sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper()
            || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 1
            || test2.GetUpper() != 1) {

        return false;
    }

    //Subtraction with positive-positive lowers.
    ubit64 = 0x7fffffff;
    uAddend = 0x7ffffffe;

    test1 = ubit64 + uAddend;

    sbit64 = 0x7fffffff;
    sAddend = 0x7ffffffe;

    test2 = sbit64 + sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper()
            || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 0
            || test2.GetUpper() != 0) {

        return false;
    }

    //Subtraction with negative-negative lowers, GetUpper() ++.
    ubit64 = 0xefffffff;
    uAddend = 0xfffffffe;

    test1 = ubit64 + uAddend;

    sbit64 = 0xefffffff;
    sAddend = 0xfffffffe;

    test2 = sbit64 + sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper()
            || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 1
            || test2.GetUpper() != 1) {
        return false;
    }

    //Test operation with negative numbers.
    sbit64 = -1;
    test2 = -2;

    if (sbit64 + test2 != -3) {
        return false;
    }

    if (test2 - sbit64 != -1) {
        return false;
    }

    return true;
}
*/
