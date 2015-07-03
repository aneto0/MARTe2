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

class StringHelperGTest: public ::testing::Test {
protected:
    virtual void SetUp() {
        // Code here will be called immediately after the constructor
        // (right before each test).
    }

    virtual void TearDown() {
        // Code here will be called immediately after each test
        // (right before the destructor).
    }
};



TEST_F(StringHelperGTest,TestLength) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestLength("Hello"));
}

TEST_F(StringHelperGTest,TestCompare) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompare("Hello", "Hello"));
}

TEST_F(StringHelperGTest,TestCompareN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCompareN());
}

TEST_F(StringHelperGTest,TestCopy) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopy("Hello"));
}

TEST_F(StringHelperGTest,TestCopyN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestCopyN());
}

TEST_F(StringHelperGTest,TestConcatenate) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenate());
}
TEST_F(StringHelperGTest,TestConcatenateN) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestConcatenateN());
}

TEST_F(StringHelperGTest,TestSearchChars) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchChars());
}

TEST_F(StringHelperGTest,TestSearchChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchChar());
}

TEST_F(StringHelperGTest,TestSearchLastChar) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchLastChar());
}
TEST_F(StringHelperGTest,TestSearchString) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchString());
}

TEST_F(StringHelperGTest,TestSearchIndex) {
    StringHelperTest mystringhelper;
    ASSERT_TRUE(mystringhelper.TestSearchIndex());
}
