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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "SelectTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FileSystem_L1Portability_SelectGTest, TestDefaultConstructor) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestDefaultConstructor());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddReadHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddReadHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddReadHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddReadHandle_SameHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddReadHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddReadHandle_InvalidHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddWriteHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddWriteHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddWritedHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddWritedHandle_SameHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddWritedHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddWritedHandle_InvalidHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddExceptionHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddExceptionHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddExceptionHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddExceptionHandle_SameHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestAddExceptionHandle_Invalidle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestAddExceptionHandle_Invalidle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveReadHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveReadHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveReadHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveReadHandle_SameHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveReadHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveReadHandle_InvalidHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveWriteHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveWriteHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveWriteHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveWriteHandle_SameHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveWriteHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveWriteHandle_InvalidHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveExceptionHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveExceptionHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveExceptionHandle_SameHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveExceptionHandle_SameHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestRemoveExceptionHandle_InvalidHandle) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestRemoveExceptionHandle_InvalidHandle());
}

TEST(FileSystem_L1Portability_SelectGTest, TestClearAllHandles) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestClearAllHandles());
}

TEST(FileSystem_L1Portability_SelectGTest, TestIsSet) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestIsSet());
}

TEST(FileSystem_L1Portability_SelectGTest, TestWaitUntil_waitTimeout) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_waitTimeout());
}

TEST(FileSystem_L1Portability_SelectGTest, TestWaitUntil_waitRead) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_waitRead(2000));
}

TEST(FileSystem_L1Portability_SelectGTest, TestWaitUntil_waitRead_Infinite) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_waitRead(TTInfiniteWait));
}

TEST(FileSystem_L1Portability_SelectGTest, TestWaitUntil_severaDifferentWaitRead) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_severaDifferentWaitRead());
}

TEST(FileSystem_L1Portability_SelectGTest, TestWaitUntil_removeSomeWaitRead) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_removeSomeWaitRead());
}

TEST(FileSystem_L1Portability_SelectGTest, TestWaitUntil_emptyList) {
    SelectTest myTest;
    ASSERT_TRUE(myTest.TestWaitUntil_emptyList());
}
