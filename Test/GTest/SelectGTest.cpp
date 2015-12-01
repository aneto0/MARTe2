/**
 * @file SelectGTest.cpp
 * @brief Source file for class SelectGTest
 * @date 19/11/2015
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
 * the class SelectGTest (public, protected, and private). Be aware that some 
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
#include "SelectTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(SelectGTest, TestDefaultConstructor) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestDefaultConstructor());
}

TEST(SelectGTest, TestAddReadHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddReadHandle());
}

TEST(SelectGTest, TestAddReadHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddReadHandle_SameHandle());
}

TEST(SelectGTest, TestAddReadHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddReadHandle_InvalidHandle());
}

TEST(SelectGTest, TestAddWriteHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddWriteHandle());
}

TEST(SelectGTest, TestAddWritedHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddWritedHandle_SameHandle());
}

TEST(SelectGTest, TestAddWritedHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddWritedHandle_InvalidHandle());
}

TEST(SelectGTest, TestAddExceptionHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddExceptionHandle());
}

TEST(SelectGTest, TestAddExceptionHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddExceptionHandle_SameHandle());
}

TEST(SelectGTest, TestAddExceptionHandle_Invalidle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddExceptionHandle_Invalidle());
}

TEST(SelectGTest, TestRemoveReadHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveReadHandle());
}

TEST(SelectGTest, TestRemoveReadHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveReadHandle_SameHandle());
}

TEST(SelectGTest, TestRemoveReadHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveReadHandle_InvalidHandle());
}

TEST(SelectGTest, TestRemoveWriteHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveWriteHandle());
}

TEST(SelectGTest, TestRemoveWriteHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveWriteHandle_SameHandle());
}

TEST(SelectGTest, TestRemoveWriteHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveWriteHandle_InvalidHandle());
}

TEST(SelectGTest, TestRemoveExceptionHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveExceptionHandle());
}

TEST(SelectGTest, TestRemoveExceptionHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveExceptionHandle_SameHandle());
}

TEST(SelectGTest, TestRemoveExceptionHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveExceptionHandle_InvalidHandle());
}

TEST(SelectGTest, TesClearAllHandles) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TesClearAllHandles());
}

TEST(SelectGTest, TestIsSet) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestIsSet());
}

TEST(SelectGTest, TestWaitUntil_waitTimeout) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_waitTimeout());
}

TEST(SelectGTest, TestWaitUntil_waitRead) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_waitRead());
}

TEST(SelectGTest, TestWaitUntil_severaDifferentWaitRead) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_severaDifferentWaitRead());
}

TEST(SelectGTest, TestWaitUntil_removeSomeWaitRead) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_removeSomeWaitRead());
}

TEST(SelectGTest, TestWaitUntil_emptyList) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_emptyList());
}
