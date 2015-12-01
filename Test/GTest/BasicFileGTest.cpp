/**
 * @file BasicFile.cpp
 * @brief Source file for class BasicFile
 * @date 04/11/2015
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
 * the class BasicFile (public, protected, and private). Be aware that some 
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

#include "BasicFileTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicFileGTest, TestDefaultConstructor) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestDefaultConstructor());
}

TEST(BasicFileGTest, TestCopyConstructor_withOpenFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCopyConstructor_withOpenFile());
}

TEST(BasicFileGTest, TestCopyConstructor_withClosedFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCopyConstructor_withClosedFile());
}

TEST(BasicFileGTest, TestAssignmentOperator_withOpenFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestAssignmentOperator_withOpenFile());
}

TEST(BasicFileGTest, TestAssignmentOperator_withClosedFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestAssignmentOperator_withClosedFile());
}

TEST(BasicFileGTest, TestAssignmentOperator_checkPointsToSameFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestAssignmentOperator_checkPointsToSameFile());
}

TEST(BasicFileGTest, TestSetFlags) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSetFlags());
}

TEST(BasicFileGTest, TestSetFlags_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSetFlags_Close());
}

TEST(BasicFileGTest, TestGetFlags) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetFlags());
}

TEST(BasicFileGTest, TestGetFlags_defaultWFlags) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetFlags_defaultWFlags());
}

TEST(BasicFileGTest, TestGetFlags_defaultRFlags) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetFlags_defaultRFlags());
}

TEST(BasicFileGTest, TestCanRead_OnClosedFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanRead_OnClosedFile());
}

TEST(BasicFileGTest, TestCanRead_OnOpenedReadbleFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanRead_OnOpenedReadbleFile());
}

TEST(BasicFileGTest, TestCanRead_OnOpenWritableFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanRead_OnOpenWritableFile());
}

TEST(BasicFileGTest, TestCanWrite_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_Close());
}

TEST(BasicFileGTest, TestCanWrite_OpenRead) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_OpenRead());
}

TEST(BasicFileGTest, TestCanWrite_OpenWrite) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_OpenWrite());
}

TEST(BasicFileGTest, TestCanSeek_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_OpenWrite());
}

TEST(BasicFileGTest, TestCanSeek_Open) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_OpenWrite());
}

TEST(BasicFileGTest, TestOpen) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen());
}

TEST(BasicFileGTest, TestOpen_FLAG_CREAT_EXCLUSIVE) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_FLAG_CREAT_EXCLUSIVE());
}

TEST(BasicFileGTest, TestOpen_twice) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_twice());
}

TEST(BasicFileGTest, TestOpen_IncompatibleFlags1) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_IncompatibleFlags1());
}

TEST(BasicFileGTest, TestOpen_IncompatibleFlags2) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_IncompatibleFlags2());
}

TEST(BasicFileGTest, TestOpen_IncompatibleFlags3) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_IncompatibleFlags3());
}

TEST(BasicFileGTest, TestOpen_IncompatibleFlags4) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_IncompatibleFlags4());
}

TEST(BasicFileGTest, TestIsOpen) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestIsOpen());
}

TEST(BasicFileGTest, TestClose) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestClose());
}

TEST(BasicFileGTest, TestWrite) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestWrite());
}

TEST(BasicFileGTest, TestWrite_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestWrite_close());
}

TEST(BasicFileGTest, TestWrite_timeout) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestWrite_timeout());
}

TEST(BasicFileGTest, TestWrite_timeout_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestWrite_timeout_close());
}

TEST(BasicFileGTest, TestRead) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRead());
}

TEST(BasicFileGTest, TestRead_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRead_close());
}

TEST(BasicFileGTest, TestRead_timeout) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRead_timeout());
}

TEST(BasicFileGTest, TestRead_timeout_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRead_timeout_close());
}

TEST(BasicFileGTest, TestSize) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSize());
}

TEST(BasicFileGTest, TestSize_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSize_close());
}

TEST(BasicFileGTest, TestSeek_small) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSeek_small());
}

TEST(BasicFileGTest, TestSeek_large) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSeek_large());
}

TEST(BasicFileGTest, TestSeek_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSeek_Close());
}

TEST(BasicFileGTest, TestRelativeSeek_small) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRelativeSeek_small());
}

TEST(BasicFileGTest, TestRelativeSeek_larger) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRelativeSeek_larger());
}

TEST(BasicFileGTest, TestRelativeSeek_NegativePosition) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRelativeSeek_NegativePosition());
}

TEST(BasicFileGTest, TestRelativeSeek_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRelativeSeek_Close());
}

TEST(BasicFileGTest, TestPosition) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestPosition());
}

TEST(BasicFileGTest, TestSetSize_enlarging) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSetSize_enlarging());
}

TEST(BasicFileGTest, TestSetSize_reducing) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSetSize_reducing());
}

TEST(BasicFileGTest, TestGetPathName) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetPathName());
}

TEST(BasicFileGTest, TestGetReadHandle) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetReadHandle());
}

TEST(BasicFileGTest, TestGetWriteHandle) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetWriteHandle());
}
