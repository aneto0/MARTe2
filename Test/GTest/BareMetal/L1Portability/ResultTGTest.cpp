/**
 * @file ResultTGTest.cpp
 * @brief Source file for class ResultTGTest
 * @date 02/12/2015
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
 * the class ResultTGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ResultTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_ResultTGTest,TestConstructor) {
    ResultTTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L4Configuration_ResultTGTest,TestCopyConstructor) {
    ResultTTest test;
    ASSERT_TRUE(test.TestCopyConstructor());
}

TEST(BareMetal_L4Configuration_ResultTGTest,TestDestructor) {
    ResultTTest test;
    ASSERT_TRUE(test.TestDestructor());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestSetError) {
    ResultTTest test;
    ASSERT_TRUE(test.TestSetError());
}


TEST(BareMetal_L4Configuration_ResultTGTest, TestPrintError) {
    ResultTTest test;
    ASSERT_TRUE(test.TestPrintError());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestGetValue) {
    ResultTTest test;
    ASSERT_TRUE(test.TestGetValue());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestBoolOperator) {
    ResultTTest test;
    ASSERT_TRUE(test.TestBoolOperator());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestAssignOperator_Type) {
    ResultTTest test;
    ASSERT_TRUE(test.TestAssignOperator_Type());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestAssignOperator_Result) {
    ResultTTest test;
    ASSERT_TRUE(test.TestAssignOperator_Result());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestEqualOperator_Type) {
    ResultTTest test;
    ASSERT_TRUE(test.TestEqualOperator_Type());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestEqualOperator_Result) {
    ResultTTest test;
    ASSERT_TRUE(test.TestEqualOperator_Result());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestDifferentOperator_Type) {
    ResultTTest test;
    ASSERT_TRUE(test.TestDifferentOperator_Type());
}

TEST(BareMetal_L4Configuration_ResultTGTest, TestDifferentOperator_Result) {
    ResultTTest test;
    ASSERT_TRUE(test.TestDifferentOperator_Result());
}





