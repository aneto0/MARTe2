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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "BitSetTest.h"

#define TESTSET_NAME BareMetal_L0Types_BitSetGTest

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(TESTSET_NAME,TestConstructors) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestConstructors());
}

TEST(TESTSET_NAME, TestGetNumberOfElements) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestGetNumberOfElements());
}

TEST(TESTSET_NAME,TestSetBitMethods) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestSetBitMethods());
}

TEST(TESTSET_NAME,TestAssignOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestAssingOperator());
}

TEST(TESTSET_NAME,TestIntCastOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestIntCastOperator());
}

TEST(TESTSET_NAME,TestOrOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestOrOperator());
}

TEST(TESTSET_NAME,TestAndOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestAndOperator());
}

TEST(TESTSET_NAME,TestXorOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestXorOperator());
}

TEST(TESTSET_NAME,TestNotOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestNotOperator());
}

TEST(TESTSET_NAME,TestLeftShiftOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestLeftShift());
}

TEST(TESTSET_NAME,TestRightShiftOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestRightShift());
}

TEST(TESTSET_NAME, TestEqualityOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestEquality());
}

TEST(TESTSET_NAME, TestDisequalityOperator) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestDisequality());
}

TEST(TESTSET_NAME, TestGetArray) {
     BitSetTest myBasicTypeTest;
     ASSERT_TRUE(myBasicTypeTest.TestGetArray());
}
