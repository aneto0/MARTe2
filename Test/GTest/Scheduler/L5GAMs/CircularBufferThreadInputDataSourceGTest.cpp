/**
 * @file CircularBufferThreadInputDataSourceGTest.cpp
 * @brief Source file for class CircularBufferThreadInputDataSourceGTest
 * @date 27/04/2018
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
 * the class CircularBufferThreadInputDataSourceGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "CircularBufferThreadInputDataSourceTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(CircularBufferThreadInputDataSourceGTest,TestConstructor) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestInitialise) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSynchronise) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSynchronise_FullRolling) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_FullRolling());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSynchronise_GetLatest) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSynchronise_GetLatest());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestGetBrokerName) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestGetInputBrokers) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputBrokers());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestGetOutputBrokers) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputBrokers());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_False_WrittenSignal) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_False_WrittenSignal());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestPrepareNextState) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestGetInputOffset) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetInputOffset());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestGetOutputOffset) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestGetOutputOffset());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute_SameSignalDifferentModes) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_SameSignalDifferentModes());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute_ErrorCheck) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_ErrorCheck());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute_ErrorCheck_Overwrite) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_ErrorCheck_Overwrite());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute_ErrorCheck_DriverRead) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_ErrorCheck_DriverRead());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute_ErrorCheck_Both) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_ErrorCheck_Both());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_ErrorCheck_False_BadType) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_ErrorCheck_False_BadType());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_ErrorCheck_False_BadSize) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_ErrorCheck_False_BadSize());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute_TimeStamp) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_TimeStamp());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute_TimeStamp_NoRead) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_TimeStamp_NoRead());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestExecute_TimeStamp_FalseDriverRead) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestExecute_TimeStamp_FalseDriverRead());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_TimeStamp_False_BadSize) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_TimeStamp_False_BadSize());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestSetConfiguredDatabase_TimeStamp_False_BadType) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase_TimeStamp_False_BadType());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestPurge) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestPurge());
}

TEST(CircularBufferThreadInputDataSourceGTest,TestTerminateInputCopy) {
    CircularBufferThreadInputDataSourceTest test;
    ASSERT_TRUE(test.TestTerminateInputCopy());
}

