/**
 * @file StringHelperGTest.cpp
 * @brief Source file for class StringHelperGTest
 * @date 30/06/2015
 * @author Giuseppe Ferro
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
 * the class StringHelperGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "StringHelperTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_StringHelperGTest,TestLength) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestLength("Hello"));
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestCompare) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompare("Hello", "Hello"));
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestCompareN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompareN());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestToUpper) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestToUpper());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestToLower) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestToLower());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestCopy) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopy("Hello"));
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestCopyN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopyN());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestConcatenate) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenate());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestConcatenateN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenateN());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestSearchChars) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchChars());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestSearchChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchChar());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestSearchLastChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchLastChar());
}
TEST(BareMetal_L1Portability_StringHelperGTest,TestSearchString) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchString());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestSearchIndex) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchIndex());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestSetChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetChar());
}
TEST(BareMetal_L1Portability_StringHelperGTest,TestTokenByChars) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByChars());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestTokenByString) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByString());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestSubstr) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSubstr());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestSetChar0Size) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetChar0Size());
}

TEST(BareMetal_L1Portability_StringHelperGTest,TestSetCharNull) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetCharNull());
}
