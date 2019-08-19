/**
 * @file StringsGTest.cpp
 * @brief Source file for class StringsGTest
 * @date 30/06/2015
 * @author Giuseppe Ferr�
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
 * the class StringsGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestSupport.h"
#include "StringsTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(StringsGTest,TestLength) {
    ASSERT_TRUE(StringsTest::TestLength("Hello",5));
}

TEST(StringsGTest,TestLength0) {
    ASSERT_TRUE(StringsTest::TestLength("",0));
}

TEST(StringsGTest,TestLengthNULL) {
    ASSERT_TRUE(StringsTest::TestLength(emptyString,0));
}

TEST(StringsGTest,TestCompareT) {
    ASSERT_TRUE(StringsTest::TestCompare("Hello", "Hello",true));
}

TEST(StringsGTest,TestCompareF) {
    ASSERT_TRUE(StringsTest::TestCompare("Hella", "Hello",false));
}

TEST(StringsGTest,TestCompareFx) {
    ASSERT_TRUE(StringsTest::TestCompare("Hella", "Hello",true,4));
}

TEST(StringsGTest,TestCompareFE) {
    ASSERT_TRUE(StringsTest::TestCompare("Hella", "",false));
}

TEST(StringsGTest,TestCompareFN) {
    ASSERT_TRUE(StringsTest::TestCompare("Hella", emptyString,false));
}

TEST(StringsGTest,TestCompareContent) {
    ASSERT_TRUE(StringsTest::TestCompareContent("Hello", "Hello",0));
}

TEST(StringsGTest,TestCompareContentG) {
    ASSERT_TRUE(StringsTest::TestCompareContent("Hella", "Hello",-1));
}

TEST(StringsGTest,TestCompareContentGx) {
    ASSERT_TRUE(StringsTest::TestCompareContent("Hella", "Hello",4,0));
}

TEST(StringsGTest,TestCompareContentL) {
    ASSERT_TRUE(StringsTest::TestCompareContent("Hello", "Hella",1));
}

TEST(StringsGTest,TestCompareContentLx) {
    ASSERT_TRUE(StringsTest::TestCompareContent("Hello", "Hella",4,0));
}

TEST(StringsGTest,TestCompareContentG2) {
    ASSERT_TRUE(StringsTest::TestCompareContent("Hell", "Hello",-1));
}

TEST(StringsGTest,TestCompareContentL2) {
    ASSERT_TRUE(StringsTest::TestCompareContent("Hello", "Hell",1));
}

TEST(StringsGTest,TestCompareContentG3) {
    ASSERT_TRUE(StringsTest::TestCompareContent(emptyString, "Hello",-1));
}

TEST(StringsGTest,TestCompareContentL3) {
    ASSERT_TRUE(StringsTest::TestCompareContent("Hello", emptyString,1));
}

TEST(StringsGTest,TestCompareContent2) {
    ASSERT_TRUE(StringsTest::TestCompareContent(emptyString, emptyString,0));
}

TEST(StringsGTest,TestAppend) {
    ASSERT_TRUE(StringsTest::TestAppend("Hello"));
}

TEST(StringsGTest,TestAppend2) {
    ASSERT_TRUE(StringsTest::TestAppend("Hello",3));
}

TEST(StringsGTest,TestAppend3) {
    ASSERT_TRUE(StringsTest::TestAppend(emptyString,3,1,false));
}

TEST(StringsGTest,TestAppend4) {
    ASSERT_TRUE(StringsTest::TestAppend("Hello",0xFFFFFFFF,3));
}

TEST(StringsGTest,TestAppend5) {
    ASSERT_TRUE(StringsTest::TestAppend("Hello",3,5));
}

TEST(StringsGTest,TestFindXXX) {
    ASSERT_TRUE(StringsTest::TestFindXXX("Draconomicus","cono",3));
}

TEST(StringsGTest,TestFindXXX1) {
    ASSERT_TRUE(StringsTest::TestFindXXX("Draconomicus","m",7));
}

TEST(StringsGTest,TestFindXXX2) {
    ASSERT_TRUE(StringsTest::TestFindXXX("Draconomicus","noco",0,false));
}

TEST(StringsGTest,TestFindXXX3) {
    ASSERT_TRUE(StringsTest::TestFindXXX("Draconomicus","y",0,false));
}

TEST(StringsGTest,TestFindXXX4) {
    ASSERT_TRUE(StringsTest::TestFindXXX("Draconomicus",emptyString,0,false));
}

TEST(StringsGTest,TestFindXXX5) {
    ASSERT_TRUE(StringsTest::TestFindXXX(emptyString,"noco",0,false));
}

TEST(StringsGTest,TestFindXXX6) {
    ASSERT_TRUE(StringsTest::TestFindXXX(emptyString,"y",0,false));
}

TEST(StringsGTest,TestTokenizeChar) {
	CCString expectedTokens[] = {
			"Alice",
			"nel",
			"paese",
			"delle",
			"meraviglie",
			emptyString
	};
    ASSERT_TRUE(StringsTest::TestTokenize("Alice nel paese ,delle ;meraviglie",expectedTokens," ",",;"));
}

TEST(StringsGTest,TestTokenizeChar2) {
	CCString expectedTokens[] = {
			"paese",
			"",
			"delle",
			"meraviglie",
			emptyString
	};
    ASSERT_TRUE(StringsTest::TestTokenize("paese ,delle ;meraviglie",expectedTokens," ,",",;"));
}

TEST(StringsGTest,TestTokenizeString) {
	CCString expectedTokens[] = {
			"a",
			"",
			"b",
			"c",
			"",
			emptyString
	};
	CCString tokens[]={
			"AND",
			"OR",
			"NOT",
			"XOR",
			"(",
			")",
			emptyString
	};
    ASSERT_TRUE(StringsTest::TestTokenizeString("a AND (b XOR c)",expectedTokens,tokens," "));
}


