/**
 * @file TokenGTest.cpp
 * @brief Source file for class TokenGTest
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
 * the class TokenGTest (public, protected, and private). Be aware that some 
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
#include "TokenTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Configuration_TokenGTest,TestDefaultConstructor) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_TokenGTest,TestFullConstructor) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestFullConstructor(1, "Hello", "World", 1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestFullConstructor_0id) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestFullConstructor(0, "Hello", "World", 1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestFullConstructor_NullDes) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestFullConstructor(1,NULL, "World",1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestFullConstructor_NullData) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestFullConstructor(1,"Hello", NULL,1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestFullConstructor_0LineNumber) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestFullConstructor(1, "Hello", "World", 0));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestFullConstructor_All0) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestFullConstructor(0,NULL, NULL,0));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestConstructorFromTokenInfo) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestConstructorFromTokenInfo(1, "Hello", "World", 1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestConstructorFromTokenInfo_0id) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestConstructorFromTokenInfo(0, "Hello", "World", 1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestConstructorFromTokenInfo_NullDes) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestConstructorFromTokenInfo(1,NULL, "World",1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestConstructorFromTokenInfo_NullData) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestConstructorFromTokenInfo(1,"Hello", NULL,1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestConstructorFromTokenInfo_0LineNumber) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestConstructorFromTokenInfo(1, "Hello", "World", 0));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestConstructorFromTokenInfo_All0) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestConstructorFromTokenInfo(0,NULL, NULL,0));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestGetId) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestGetId(1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestGetId_0) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestGetId(0));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestGetLineNumber) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestGetLineNumber(1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestGetLineNumber_0) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestGetLineNumber(0));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestAssignOperator) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestAssignOperator(1, "Hello", "World", 1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestAssignOperator_0id) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestAssignOperator(0, "Hello", "World", 1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestAssignOperator_NullDes) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestConstructorFromTokenInfo(1,NULL, "World",1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestAssignOperator_NullData) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestAssignOperator(1,"Hello", NULL,1));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestAssignOperator_0LineNumber) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestAssignOperator(1, "Hello", "World", 0));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestAssignOperator_All0) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestAssignOperator(0,NULL, NULL,0));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestGetDescription) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestGetDescription("Hello"));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestGetDescription_NULL) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestGetDescription(NULL));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestGetData) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestGetData("Hello"));
}

TEST(BareMetal_L4Configuration_TokenGTest,TestGetData_NULL) {
    TokenTest tokenTest;
    ASSERT_TRUE(tokenTest.TestGetData(NULL));
}

