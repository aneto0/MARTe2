/**
 * @file StringPortableGTest.cpp
 * @brief Source file for class StringPortableGTest
 * @date 27/07/2015
 * @author Llorenç Capellà
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
 * the class StringPortableGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StringPortableTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_StringPortableGTest,TestLength) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestLength("Hello"));
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestCompare) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCompare("Hello", "Hello"));
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestCompareN) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCompareN());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestCopy) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCopy("Hello"));
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestCopyN) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestCopyN());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestConcatenateNoResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNoResult());
}
TEST(BareMetal_L1Portability_StringPortableGTest,TestConcatenateNNoResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNNoResult());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestConcatenateWithResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateWithResult());
}
TEST(BareMetal_L1Portability_StringPortableGTest,TestConcatenateNWithResult) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestConcatenateNWithResult());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestSearchChars) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchChars());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestSearchChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchChar());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestSearchLastChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchLastChar());
}
TEST(BareMetal_L1Portability_StringPortableGTest,TestSearchString) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchString());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestSearchIndex) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSearchIndex());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestTokenByChars) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByChars());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestTokenByString) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestTokenizeByString());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestSubstr) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSubstr());
}

TEST(BareMetal_L1Portability_StringPortableGTest,TestSetChar) {
    StringPortableTest mystringportable;
    ASSERT_TRUE(mystringportable.TestSetChar());
}
