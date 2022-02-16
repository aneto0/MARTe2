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
#include "MARTe2UTest.h"

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
TEST(BareMetal_L6App_LoaderGTest,TestConstructor) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_CPUs) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_CPUs());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_Json) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_Json());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_Xml) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_Xml());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_False_BadParser) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_BadParser());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_False_FailedConfiguration) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_FailedConfiguration());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_False_FailedParser) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_FailedParser());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_False_NoParser) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_NoParser());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_False_FailedMessageFunction) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_False_FailedMessageFunction());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_BuildTokens) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_BuildTokens());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_BuildTokens_False) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_BuildTokens_False());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_DomainTokens) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_DomainTokens());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_DomainTokens_False) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_DomainTokens_False());
}

TEST(BareMetal_L6App_LoaderGTest,TestStart) {
    LoaderTest test;
    ASSERT_TRUE(test.TestStart());
}

TEST(BareMetal_L6App_LoaderGTest,TestStart_NoMessage) {
    LoaderTest test;
    ASSERT_TRUE(test.TestStart_NoMessage());
}

TEST(BareMetal_L6App_LoaderGTest,TestStart_WrongMessage) {
    LoaderTest test;
    ASSERT_TRUE(test.TestStart_WrongMessage());
}

TEST(BareMetal_L6App_LoaderGTest,TestStop) {
    LoaderTest test;
    ASSERT_TRUE(test.TestStop());
}

TEST(BareMetal_L6App_LoaderGTest,TestConfigure_SchedulerGranularity) {
    LoaderTest test;
    ASSERT_TRUE(test.TestConfigure_SchedulerGranularity());
}

TEST(BareMetal_L6App_LoaderGTest,TestReconfigure) {
    LoaderTest test;
    ASSERT_TRUE(test.TestReconfigure());
}

TEST(BareMetal_L6App_LoaderGTest,TestReconfigure_StructuredDataI) {
    LoaderTest test;
    ASSERT_TRUE(test.TestReconfigure_StructuredDataI());
}

TEST(BareMetal_L6App_LoaderGTest,TestReconfigure_Hash) {
    LoaderTest test;
    ASSERT_TRUE(test.TestReconfigure_Hash());
}

TEST(BareMetal_L6App_LoaderGTest,TestGetLastValidConfiguration) {
    LoaderTest test;
    ASSERT_TRUE(test.TestGetLastValidConfiguration());
}

TEST(BareMetal_L6App_LoaderGTest,TestGetLastValidConfiguration_AfterFailure) {
    LoaderTest test;
    ASSERT_TRUE(test.TestGetLastValidConfiguration_AfterFailure());
}

TEST(BareMetal_L6App_LoaderGTest,TestReloadLastValidConfiguration) {
    LoaderTest test;
    ASSERT_TRUE(test.TestReloadLastValidConfiguration());
}

TEST(BareMetal_L6App_LoaderGTest,TestLoaderPostInit) {
    LoaderTest test;
    ASSERT_TRUE(test.TestLoaderPostInit());
}

TEST(BareMetal_L6App_LoaderGTest,TestLoaderPostInit_ReloadLast_true) {
    LoaderTest test;
    ASSERT_TRUE(test.TestLoaderPostInit_ReloadLast_true());
}

TEST(BareMetal_L6App_LoaderGTest,TestLoaderPostInit_ReloadLast_false) {
    LoaderTest test;
    ASSERT_TRUE(test.TestLoaderPostInit_ReloadLast_false());
}

TEST(BareMetal_L6App_LoaderGTest,TestLoaderPostInit_ReloadLast_invalid) {
    LoaderTest test;
    ASSERT_TRUE(test.TestLoaderPostInit_ReloadLast_invalid());
}

TEST(BareMetal_L6App_LoaderGTest,TestLoaderPostInit_KeepAlive) {
    LoaderTest test;
    ASSERT_TRUE(test.TestLoaderPostInit_KeepAlive());
}

TEST(BareMetal_L6App_LoaderGTest,TestMessage_PreConfiguration) {
    LoaderTest test;
    ASSERT_TRUE(test.TestMessage_PreConfiguration());
}

TEST(BareMetal_L6App_LoaderGTest,TestMessage_PostConfiguration) {
    LoaderTest test;
    ASSERT_TRUE(test.TestMessage_PostConfiguration());
}

TEST(BareMetal_L6App_LoaderGTest,TestMessage_FailedConfiguration) {
    LoaderTest test;
    ASSERT_TRUE(test.TestMessage_FailedConfiguration());
}

TEST(BareMetal_L6App_LoaderGTest,TestMessage_ReloadedConfiguration) {
    LoaderTest test;
    ASSERT_TRUE(test.TestMessage_ReloadedConfiguration());
}

TEST(BareMetal_L6App_LoaderGTest,TestGetSeed) {
    LoaderTest test;
    ASSERT_TRUE(test.TestGetSeed());
}


