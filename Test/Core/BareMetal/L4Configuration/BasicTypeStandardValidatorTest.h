/**
 * @file BasicTypeStandardValidatorTest.h
 * @brief Header file for class BasicTypeStandardValidatorTest
 * @date 26/01/2016
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

 * @details This header file contains the declaration of the class BasicTypeStandardValidatorTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef VALIDATEBASICTYPETEST_H_
#define VALIDATEBASICTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Validate.h"
#include "BasicTypeStandardValidator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
#include "stdio.h"
using namespace MARTe;

/**
 * @brief Utility for Validate tests
 */
template<typename T>
struct BasicTypeStandardValidatorTestTable {
    const char8 * attributes;
    T value;
    bool expected;
};

/**
 * @brief Utility for Validate tests
 */
template<typename T, uint32 nElements>
struct BasicTypeStandardValidatorTestTableVector {
    const char8 * attributes;
    T value[nElements];
    bool expected;
};

/**
 * @brief Utility for Validate tests
 */
template<typename T, uint32 nRows, uint32 nCols>
struct BasicTypeStandardValidatorTestTableMatrix {
    const char8 * attributes;
    T value[nRows][nCols];
    bool expected;
};

/**
 * @brief Tests all the Validate functions
 */
class BasicTypeStandardValidatorTest {
public:

    /**
     * @brief Test the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Validate with a scalar AnyType
     */
    template<typename T>
    bool TestValidate(const BasicTypeStandardValidatorTestTable<T>* table);

    /**
     * @brief Tests the Validate with a vector AnyType
     */
    template<typename T, uint32 nElements>
    bool TestValidateVector(const BasicTypeStandardValidatorTestTableVector<T, nElements>* table);

    /**
     * @brief Tests the Validate with a matrix AnyType
     */
    template<typename T, uint32 nRows, uint32 nCols>
    bool TestValidateMatrix(const BasicTypeStandardValidatorTestTableMatrix<T, nRows, nCols>* table);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool BasicTypeStandardValidatorTest::TestValidate(const BasicTypeStandardValidatorTestTable<T>* table) {

    uint32 i = 0;
    while (table[i].attributes != NULL) {
        AnyType at(table[i].value);
        if (Validate(at, table[i].attributes) != table[i].expected) {
            printf("\n%d\n", i);
            return false;
        }
        i++;
    }

    return true;
}

template<typename T, uint32 nElements>
bool BasicTypeStandardValidatorTest::TestValidateVector(const BasicTypeStandardValidatorTestTableVector<T, nElements>* table) {
    uint32 i = 0;
    while (table[i].attributes != NULL) {
        AnyType at(table[i].value);
        if (Validate(at, table[i].attributes) != table[i].expected) {
            printf("\n%d\n", i);
            return false;
        }
        i++;
    }

    return true;
}

template<typename T, uint32 nRows, uint32 nCols>
bool BasicTypeStandardValidatorTest::TestValidateMatrix(const BasicTypeStandardValidatorTestTableMatrix<T, nRows, nCols>* table) {
    uint32 i = 0;
    while (table[i].attributes != NULL) {
        AnyType at(table[i].value);
        if (Validate(at, table[i].attributes) != table[i].expected) {
            printf("\n%d\n", i);
            return false;
        }
        i++;
    }

    return true;
}

#endif /* VALIDATEBASICTYPETEST_H_ */

