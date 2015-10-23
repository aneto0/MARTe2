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
                     bool isValidIn = true) :
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

    /**
     * @brief Tests if the number is initialized to zero.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the number is copied correctly by input.
     */
    bool TestCopyConstructor(DoubleInteger<T> input);

    /**
     * @brief Tests if the upper and lower setting goes well.
     */
    bool TestConstructorByParts(T upper,
                                T lower);

    /**
     * @brief Tests if the constructor by generic type initializes the number correctly.
     */
    template<typename T2>
    bool TestConstructorByType(T2 input);


    bool TestGetLower(T lowerIn);

    bool TestGetUpper(T upperIn);

    bool TestSetLower(T lowerIn);

    bool TestSetUpper(T upperIn);

    bool TestRightShift(const testTableShiftDI<T> *table);

    bool TestLeftShift(const testTableShiftDI<T> *table);

    bool TestAndOperator(const testTableLogicDI<T> *table);

    bool TestOrOperator(const testTableLogicDI<T> *table);

    bool TestInvertOperator(const testTableLogicDI<T> *table);

    bool TestIsMajorOperator(const testTableLogicDI<T> *table);

    bool TestIsMinorOperator(const testTableLogicDI<T> *table);

    bool TestIsMajorEqualOperator(const testTableLogicDI<T> *table);

    bool TestIsMinorEqualOperator(const testTableLogicDI<T> *table);

    bool TestSumOperator(const testTableLogicDI<T> *table);

    bool TestSubOperator(const testTableLogicDI<T> *table);

    bool TestIsEqual(const testTableLogicDI<T> *table);

    bool TestIsDifferent(const testTableLogicDI<T> *table);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool DoubleIntegerTest<T>::TestDefaultConstructor() {

    DoubleInteger<T> x;
    return x == 0;
}

template<typename T>
bool DoubleIntegerTest<T>::TestCopyConstructor(DoubleInteger<T> input) {
    DoubleInteger<T> x(input);
    return x == input;
}

template<typename T>
bool DoubleIntegerTest<T>::TestConstructorByParts(T upper,
                                                  T lower) {
    DoubleInteger<T> x(upper, lower);
    return (x.GetLower() == lower) && (x.GetUpper() == upper);
}

template<typename T>
template<typename T2>
bool DoubleIntegerTest<T>::TestConstructorByType(T2 input) {
    DoubleInteger<T> x(input);
    return x == input;
}

template<typename T>
bool DoubleIntegerTest<T>::TestGetLower(T lowerIn) {
    DoubleInteger<T> x;
    x.SetLower(lowerIn);
    return x.GetLower() == lowerIn;
}

template<typename T>
bool DoubleIntegerTest<T>::TestGetUpper(T upperIn) {
    DoubleInteger<T> x;
    x.SetUpper(upperIn);
    return x.GetUpper() == upperIn;
}

template<typename T>
bool DoubleIntegerTest<T>::TestSetLower(T lowerIn) {
    DoubleInteger<T> x;
    x.SetLower(lowerIn);
    return x.GetLower() == lowerIn;
}

template<typename T>
bool DoubleIntegerTest<T>::TestSetUpper(T upperIn) {
    DoubleInteger<T> x;
    x.SetUpper(upperIn);
    return x.GetUpper() == upperIn;
}

template<typename T>
bool DoubleIntegerTest<T>::TestRightShift(const testTableShiftDI<T> *table) {
    uint32 i = 0;
    while (table[i].shift != 0xff) {

        if ((table[i].number >> table[i].shift) != table[i].result) {
            return false;
        }
        i++;
    }

    DoubleInteger<T> testN = table[i].number;
    testN >>= table[i].shift;
    if (testN != table[i].result) {
        return false;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestLeftShift(const testTableShiftDI<T> *table) {
    uint32 i = 0;
    while (table[i].shift != 0xff) {

        if ((table[i].number << table[i].shift) != table[i].result) {
            return false;
        }

        DoubleInteger<T> testN = table[i].number;
        testN <<= table[i].shift;
        if (testN != table[i].result) {
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
            return false;
        }

        DoubleInteger<T> testN = table[i].number1;
        testN &= table[i].number2;
        if (testN != table[i].result) {
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
            return false;
        }
        DoubleInteger<T> testN = table[i].number1;
        testN |= table[i].number2;
        if (testN != table[i].result) {
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
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestIsMajorOperator(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 > table[i].number2) != (table[i].result == 1)) {
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestIsMinorOperator(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 < table[i].number2) != (table[i].result == 1)) {

            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestIsMajorEqualOperator(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 >= table[i].number2) != (table[i].result == 1)) {

            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestIsMinorEqualOperator(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 <= table[i].number2) != (table[i].result == 1)) {

            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestSumOperator(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 + table[i].number2) != table[i].result) {

            return false;
        }
        DoubleInteger<T> testN = table[i].number1;
        testN += table[i].number2;
        if (testN != table[i].result) {
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestSubOperator(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 - table[i].number2) != table[i].result) {

            return false;
        }
        DoubleInteger<T> testN = table[i].number1;
        testN -= table[i].number2;
        if (testN != table[i].result) {
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestIsDifferent(const testTableLogicDI<T> *table) {
    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 != table[i].number2) != (table[i].result == 1)) {
            return false;
        }
        i++;
    }

    return true;
}

template<typename T>
bool DoubleIntegerTest<T>::TestIsEqual(const testTableLogicDI<T> *table) {

    uint32 i = 0;
    while (table[i].isValid) {

        if ((table[i].number1 == table[i].number2) != (table[i].result == 1)) {
            return false;
        }
        i++;
    }

    return true;
}

#endif /* DOUBLEINTEGERTEST_H_ */
