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

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicFileTest.h"
#include "DirectoryScanner.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void CleanEnvironment(){
    DirectoryScanner toDelete;
    toDelete.Scan("./", "*_Test.*");
    for(uint32 i=0u; i<toDelete.ListSize(); i++){
        Directory* entry=(Directory*)(toDelete.ListPeek(i));
        entry->Delete();
    }
}


TEST(FileSystem_L1Portability_BasicFileGTest, TestDefaultConstructor) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestDefaultConstructor());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCopyConstructor_withOpenFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCopyConstructor_withOpenFile());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCopyConstructor_withClosedFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCopyConstructor_withClosedFile());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestAssignmentOperator_withOpenFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestAssignmentOperator_withOpenFile());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestAssignmentOperator_withClosedFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestAssignmentOperator_withClosedFile());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestAssignmentOperator_checkPointsToSameFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestAssignmentOperator_checkPointsToSameFile());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSetFlags) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSetFlags());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSetFlags_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSetFlags_Close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestGetFlags) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetFlags());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestGetFlags_defaultWFlags) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetFlags_defaultWFlags());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestGetFlags_defaultRFlags) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetFlags_defaultRFlags());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCanRead_OnClosedFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanRead_OnClosedFile());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCanRead_OnOpenedReadbleFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanRead_OnOpenedReadbleFile());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCanRead_OnOpenWritableFile) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanRead_OnOpenWritableFile());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCanWrite_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_Close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCanWrite_OpenRead) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_OpenRead());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCanWrite_OpenWrite) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_OpenWrite());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCanSeek_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_OpenWrite());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestCanSeek_Open) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestCanWrite_OpenWrite());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestOpen) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestOpen_FLAG_CREAT_EXCLUSIVE) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_FLAG_CREAT_EXCLUSIVE());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestOpen_twice) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_twice());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestOpen_IncompatibleFlags1) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_IncompatibleFlags1());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestOpen_IncompatibleFlags2) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_IncompatibleFlags2());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestOpen_IncompatibleFlags3) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_IncompatibleFlags3());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestOpen_IncompatibleFlags4) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestOpen_IncompatibleFlags4());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestIsOpen) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestIsOpen());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestClose) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestClose());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestWrite) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestWrite());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestWrite_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestWrite_close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestWrite_timeout) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestWrite_timeout());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestWrite_timeout_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestWrite_timeout_close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestRead) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRead());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestRead_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRead_close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestRead_timeout) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRead_timeout());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestRead_timeout_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRead_timeout_close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSize) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSize());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSize_close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSize_close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSeek_small) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSeek_small());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSeek_large) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSeek_large());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSeek_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSeek_Close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestRelativeSeek_small) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRelativeSeek_small());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestRelativeSeek_larger) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRelativeSeek_larger());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestRelativeSeek_NegativePosition) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRelativeSeek_NegativePosition());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestRelativeSeek_Close) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestRelativeSeek_Close());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestPosition) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestPosition());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSetSize_enlarging) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSetSize_enlarging());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestSetSize_reducing) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestSetSize_reducing());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestGetPathName) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetPathName());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestGetReadHandle) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetReadHandle());
    CleanEnvironment();
}

TEST(FileSystem_L1Portability_BasicFileGTest, TestGetWriteHandle) {
    BasicFileTest myBasicFileTest;
    ASSERT_TRUE(myBasicFileTest.TestGetWriteHandle());
    CleanEnvironment();
}

