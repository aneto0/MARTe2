/**
 * @file BitSetGTest.cpp
 * @brief Source file for class BitSetGTest
 * @date 09/02/2021
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
 * the class BitSetGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "BitSetTest.h"

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L0Types_BitSetGTest,TestConstructors) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestConstructors());
}

TEST(BareMetal_L0Types_BitSetGTest,TestSetBitMethods) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestSetBitMethods());
}

TEST(BareMetal_L0Types_BitSetGTest,TestAssignOperator) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestAssingOperator());
}

TEST(BareMetal_L0Types_BitSetGTest,TestIntCastOperator) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestIntCastOperator());
}

TEST(BareMetal_L0Types_BitSetGTest,TestOrOperator) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestOrOperator());
}

TEST(BareMetal_L0Types_BitSetGTest,TestAndOperator) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestAndOperator());
}

TEST(BareMetal_L0Types_BitSetGTest,TestNotOperator) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestNotOperator());
}

TEST(BareMetal_L0Types_BitSetGTest,TestLeftShiftOperator) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestLeftShift());
}

TEST(BareMetal_L0Types_BitSetGTest,TestRightShiftOperator) {
    BitSetTest myBasicTypeTest;
    ASSERT_TRUE(myBasicTypeTest.TestRightShift());
}
