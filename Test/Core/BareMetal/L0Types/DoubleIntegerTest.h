/**
 * @file DoubleIntegerTest.h
 * @brief Header file for class DoubleIntegerTest
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

 * @details This header file contains the declaration of the class DoubleIntegerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DOUBLEINTEGERTEST_H_
#define DOUBLEINTEGERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DoubleInteger.h"
#include "FormatDescriptor.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

template<typename T>
class testTableShiftDI {
public:
    DoubleInteger<T> number;
    uint8 shift;
    DoubleInteger<T> result;

    testTableShiftDI(DoubleInteger<T> numberIn,
                     uint8 shiftIn,
                     DoubleInteger<T> resultIn) :
            number(numberIn),
            result(resultIn) {

        shift = shiftIn;

    }
};

template<typename T>
class testTableLogicDI {
public:
    DoubleInteger<T> number1;
    DoubleInteger<T> number2;
    DoubleInteger<T> result;
    bool isValid;

    testTableLogicDI(DoubleInteger<T> number1In,
                     DoubleInteger<T> number2In,
                     DoubleInteger<T> resultIn,
                     bool isValidIn) :
            number1(number1In),
            number2(number2In),
            result(resultIn) {
        isValid = isValidIn;
    }
};

/** @brief Class for testing of DoubleInteger functions. */
template<typename T>
class DoubleIntegerTest {

public:
    /** @brief Tests the shift operations.
     * @return true if all operations return the expected result. */
    bool TestRightShift(const testTableShiftDI<T> *table);

    bool TestLeftShift(const testTableShiftDI<T> *table);

    bool TestAndOperator(const testTableLogicDI<T> *table);

    bool TestOrOperator(const testTableLogicDI<T> *table);

    bool TestInvertOperator(const testTableLogicDI<T> *table);

    /** @brief Tests the Logical operations.
     * @return true if all operations return the expected result. */
    bool TestLogicalOperators();

    /** @brief Tests the Mathematic operations.
     * @return true of all operations return the expected result. */
    bool TestMathematicOperators();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool DoubleIntegerTest<T>::TestRightShift(const testTableShiftDI<T> *table) {
    uint32 i = 0;
    while (table[i].shift != 0xff) {

        if ((table[i].number >> table[i].shift) != table[i].result) {
            T lower = (table[i].number >> table[i].shift).GetLower();
            printf("\n%d %d %d %d\n", (int32) lower, i, sizeof(T), table[i].shift);
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestLeftShift(const testTableShiftDI<T> *table) {
    uint32 i = 0;
    while (table[i].shift != 0xff) {

        if ((table[i].number << table[i].shift) != table[i].result) {
            T lower = (table[i].number >> table[i].shift).GetLower();
            printf("\n%d %d %d %d\n", (int32) lower, i, sizeof(T), table[i].shift);
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestAndOperator(const testTableLogicDI<T> *table) {

    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 & table[i].number2) != table[i].result) {
            T lower = (table[i].number1 & table[i].number2).GetLower();
            printf("\n%d %d %d %d\n", (int32) lower, i, sizeof(T), table[i].result);
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestOrOperator(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 | table[i].number2) != table[i].result) {
            T lower = (table[i].number1 & table[i].number2).GetLower();
            printf("\n%d %d %d %d\n", (int32) lower, i, sizeof(T), table[i].result);
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestInvertOperator(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if (~(table[i].number1) != table[i].result) {
            T lower = (table[i].number1 & table[i].number2).GetLower();
            printf("\n%d %d %d %d\n", (int32) lower, i, sizeof(T), table[i].result);
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestLogicalOperators() {

    DoubleInteger<int32> test1(0x00ffffffffffffff);
    DoubleInteger<int32> test2(0xff00000000000000);

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

    DoubleInteger<int32> zero;

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

template<typename T>
bool DoubleIntegerTest<T>::TestMathematicOperators() {

    DoubleInteger<int32> sbit64;
    DoubleInteger<uint32> ubit64;
/////////////Subtraction

//Subtraction with positive-negative lowers, GetUpper()--
    ubit64 = 0xf7fffffff;
    DoubleInteger<uint32> uAddend(0xefffffffe);

    ubit64 -= uAddend;

    sbit64 = 0xf7fffffff;
    DoubleInteger<int32> sAddend(0xefffffffe);

    sbit64 -= sAddend;

    if ((int32) (ubit64.GetUpper()) != sbit64.GetUpper() || ubit64.GetLower() != (uint32) sbit64.GetLower() || ubit64.GetUpper() != 0
            || sbit64.GetUpper() != 0) {
        return false;
    }

    //Subtraction with negative-positive lowers, uppers should not decrement.
    ubit64 = 0xfffffffff;
    uAddend = 0xe7ffffffe;

    DoubleInteger<uint32> test1 = ubit64 - uAddend;

    sbit64 = 0xfffffffff;
    sAddend = 0xe7ffffffe;

    DoubleInteger<int32> test2 = sbit64 - sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper() || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 1 || test2.GetUpper() != 1) {
        return false;
    }

    //Subtraction with positive-positive lowers, GetUpper() decrement.
    ubit64 = 0xf6fffffff;
    uAddend = 0xe7ffffffe;

    test1 = ubit64 - uAddend;

    sbit64 = 0xf6fffffff;
    sAddend = 0xe7ffffffe;

    test2 = sbit64 - sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper() || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 0 || test2.GetUpper() != 0) {
        return false;
    }

    //Subtraction with negative-negative lowers, GetUpper() decrement.
    ubit64 = 0xfefffffff;
    uAddend = 0xefffffffe;

    test1 = ubit64 - uAddend;

    sbit64 = 0xfefffffff;
    sAddend = 0xefffffffe;

    test2 = sbit64 - sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper() || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 0 || test2.GetUpper() != 0) {
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

    if ((int32) (ubit64.GetUpper()) != sbit64.GetUpper() || ubit64.GetLower() != (uint32) sbit64.GetLower() || ubit64.GetUpper() != 1
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

    if ((int32) (test1.GetUpper()) != test2.GetUpper() || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 1 || test2.GetUpper() != 1) {

        return false;
    }

    //Subtraction with positive-positive lowers.
    ubit64 = 0x7fffffff;
    uAddend = 0x7ffffffe;

    test1 = ubit64 + uAddend;

    sbit64 = 0x7fffffff;
    sAddend = 0x7ffffffe;

    test2 = sbit64 + sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper() || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 0 || test2.GetUpper() != 0) {

        return false;
    }

    //Subtraction with negative-negative lowers, GetUpper() ++.
    ubit64 = 0xefffffff;
    uAddend = 0xfffffffe;

    test1 = ubit64 + uAddend;

    sbit64 = 0xefffffff;
    sAddend = 0xfffffffe;

    test2 = sbit64 + sAddend;

    if ((int32) (test1.GetUpper()) != test2.GetUpper() || test1.GetLower() != (uint32) test2.GetLower() || test1.GetUpper() != 1 || test2.GetUpper() != 1) {
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

#endif /* DOUBLEINTEGERTEST_H_ */
