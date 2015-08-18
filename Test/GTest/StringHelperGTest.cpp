/**
 * @file StringHelperGTest.cpp
 * @brief Source file for class StringHelperGTest
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
 * the class StringHelperGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "StringHelperTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(StringHelperGTest,TestLength) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestLength("Hello"));
}

TEST(StringHelperGTest,TestCompare) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompare("Hello", "Hello"));
}

TEST(StringHelperGTest,TestCompareN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompareN());
}

TEST(StringHelperGTest,TestCopy) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopy("Hello"));
}

TEST(StringHelperGTest,TestCopyN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopyN());
}

TEST(StringHelperGTest,TestConcatenate) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenate());
}

TEST(StringHelperGTest,TestConcatenateN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenateN());
}

TEST(StringHelperGTest,TestSearchChars) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchChars());
}

TEST(StringHelperGTest,TestSearchChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchChar());
}

TEST(StringHelperGTest,TestSearchLastChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchLastChar());
}
TEST(StringHelperGTest,TestSearchString) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchString());
}

TEST(StringHelperGTest,TestSearchIndex) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchIndex());
}

TEST(StringHelperGTest,TestSetChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetChar());
}
TEST(StringHelperGTest,TestTokenByChars) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByChars());
}

TEST(StringHelperGTest,TestTokenByString) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByString());
}

TEST(StringHelperGTest,TestSubstr) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSubstr());
}

TEST(StringHelperGTest,TestSetChar0Size) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetChar0Size());
}

TEST(StringHelperGTest,TestSetCharNull) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetCharNull());
}
