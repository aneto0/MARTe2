/**
 * @file LoaderGTest.cpp
 * @brief Source file for class LoaderGTest
 * @date 05/04/2018
 * @author Andre Neto
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
 * the class LoaderGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LoaderTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(LoaderGTest,TestConstructor) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(LoaderGTest,TestConfigure) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure());
}

TEST(LoaderGTest,TestConfigure_CPUs) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_CPUs());
}

TEST(LoaderGTest,TestConfigure_Json) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_Json());
}

TEST(LoaderGTest,TestConfigure_Xml) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_Xml());
}

TEST(LoaderGTest,TestConfigure_False_BadParser) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_BadParser());
}

TEST(LoaderGTest,TestConfigure_False_FailedConfiguration) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_FailedConfiguration());
}

TEST(LoaderGTest,TestConfigure_False_FailedParser) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_FailedParser());
}

TEST(LoaderGTest,TestConfigure_False_NoParser) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_NoParser());
}

TEST(LoaderGTest,TestConfigure_False_FailedMessageFunction) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_FailedMessageFunction());
}

TEST(LoaderGTest,TestStart) {
    LoaderTest test;
    ASSERT_TRUE(test.TestStart());
}

TEST(LoaderGTest,TestStart_NoMessage) {
    LoaderTest test;
    ASSERT_TRUE(test.TestStart_NoMessage());
}

TEST(LoaderGTest,TestStart_WrongMessage) {
    LoaderTest test;
    ASSERT_TRUE(test.TestStart_WrongMessage());
}

TEST(LoaderGTest,TestStop) {
    LoaderTest test;
    ASSERT_TRUE(test.TestStop());
}
