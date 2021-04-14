/**
 * @file BitSetTest.h
 * @brief Header file for class BitSetTest
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

 * @details This header file contains the declaration of the class BitTestTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BITSETTEST_H_
#define BITSETTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "BitSet.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the BitRange functions.
 * @tparam T TODO Define T
 */
class BitSetTest {

public:

    /**
     * @brief Test the constructors.
     */
    bool TestConstructors();

    /**
     * @brief Test ByteSize.
     */
    bool TestGetNumberOfElements();
    
    /**
     * @brief Test the Set and Bit methods.
     */
    bool TestSetBitMethods();

    /**
     * @brief Thest the `int` cast operators.
     */
    bool TestIntCastOperator();

    /**
     * @brief Test the assing `=` operators.
     */
    bool TestAssingOperator();

    /**
     * @brief Test the bitwise `or` operators.
     */
    bool TestOrOperator();

    /**
     * @brief Test the bitwise `and` operators.
     */
    bool TestAndOperator();

    /**
     * @brief Test the bitwise `xor` operators.
     */
    bool TestXorOperator();

    /**
     * @brief Test the bitwise `not` operators.
     */
    bool TestNotOperator();

    /**
     * @brief Test boolean equality operators.
     */
    bool TestEquality();

    /**
     * @brief Test boolean disequality operators.
     */
    bool TestDisequality();

    /**
     * @brief Test left shift operator.
     */
    bool TestLeftShift();

    /**
     * @brief Test right shift operator.
     */
    bool TestRightShift();

    /**
     * @brief Test GetArray method.
     */
    bool TestGetArray();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif
