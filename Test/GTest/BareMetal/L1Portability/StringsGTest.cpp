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
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestLength("Hello"));
}

TEST(StringsGTest,TestCompare) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompare("Hello", "Hello"));
}

TEST(StringsGTest,TestCompareN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompareN());
}

TEST(StringsGTest,TestCopy) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopy("Hello"));
}

TEST(StringsGTest,TestCopyN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopyN());
}

TEST(StringsGTest,TestConcatenate) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenate());
}

TEST(StringsGTest,TestConcatenateN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenateN());
}

TEST(StringsGTest,TestSearchChars) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchChars());
}

TEST(StringsGTest,TestSearchChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchChar());
}

TEST(StringsGTest,TestSearchLastChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchLastChar());
}
TEST(StringsGTest,TestSearchString) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchString());
}

TEST(StringsGTest,TestSearchIndex) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchIndex());
}

TEST(StringsGTest,TestSetChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetChar());
}
TEST(StringsGTest,TestTokenByChars) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByChars());
}

TEST(StringsGTest,TestTokenByString) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByString());
}

TEST(StringsGTest,TestSubstr) {
    StringHelperTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSubstr());
}

TEST(StringsGTest,TestSetChar0Size) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetChar0Size());
}

TEST(StringsGTest,TestSetCharNull) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSetCharNull());
}

TEST(StringPortableGTest,TestLength) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestLength("Hello"));
}

TEST(StringPortableGTest,TestCompare) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCompare("Hello", "Hello"));
}

TEST(StringPortableGTest,TestCompareN) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCompareN());
}

TEST(StringPortableGTest,TestCopy) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCopy("Hello"));
}

TEST(StringPortableGTest,TestCopyN) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCopyN());
}

TEST(StringPortableGTest,TestConcatenateNoResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNoResult());
}
TEST(StringPortableGTest,TestConcatenateNNoResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNNoResult());
}

TEST(StringPortableGTest,TestConcatenateWithResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateWithResult());
}
TEST(StringPortableGTest,TestConcatenateNWithResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNWithResult());
}

TEST(StringPortableGTest,TestSearchChars) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchChars());
}

TEST(StringPortableGTest,TestSearchChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchChar());
}

TEST(StringPortableGTest,TestSearchLastChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchLastChar());
}
TEST(StringPortableGTest,TestSearchString) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchString());
}

TEST(StringPortableGTest,TestSearchIndex) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchIndex());
}

TEST(StringPortableGTest,TestTokenByChars) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByChars());
}

TEST(StringPortableGTest,TestTokenByString) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByString());
}

TEST(StringPortableGTest,TestSubstr) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSubstr());
}

TEST(StringPortableGTest,TestSetChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSetChar());
}

