/**
 * @file DirectoryGTest.cpp
 * @brief Source file for class DirectoryGTest
 * @date Nov 5, 2015
 * @author Frank Perez Paz
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
 * the class DirectoryGTest (public, protected, and private). Be aware that some 
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

#include "DirectoryTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

class DirectoryGTest: public ::testing::Test {

protected:
    // Per-test-case set-up.
    // Called before the first test in this test case.
    // Can be omitted if not needed.
    static void SetUpTestCase() {
        DirectoryTest myClassTest;
        myClassTest.Delete_Directory();
        myClassTest.Create_Directory();
    }

    // Per-test-case tear-down.
    // Called after the last test in this test case.
    // Can be omitted if not needed.
    static void TearDownTestCase() {
        DirectoryTest myClassTest;
        myClassTest.Delete_Directory();
    }
};


TEST_F(DirectoryGTest, TestDirectoryTest) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDirectoryTest("TestDirectoryTest"));
}
TEST_F(DirectoryGTest, TestDirectoryTest_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDirectoryTest(NULL));
}

TEST_F(DirectoryGTest, TestSetByName_Valid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_Valid());
}

TEST_F(DirectoryGTest, TestSetByName_Valid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_Valid());
}

TEST_F(DirectoryGTest, TestSetByName_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_NULL());
}

TEST_F(DirectoryGTest, TestSetByName_NULL2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_NULL());
}

TEST_F(DirectoryGTest, TestSetByName_Invalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_Invalid());
}

TEST_F(DirectoryGTest, TestSetByName_Invalid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_Invalid());
}

TEST_F(DirectoryGTest, TestGetName_Invalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetName_Invalid());
}

TEST_F(DirectoryGTest, TestGetName_Invalid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetName_Invalid());
}

TEST_F(DirectoryGTest, TestName_Valid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetName_Valid());
}

TEST_F(DirectoryGTest, TestName_Valid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetName_Valid());
}

TEST_F(DirectoryGTest, TestIsDirectory_Invalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Invalid());
}

TEST_F(DirectoryGTest, TestIsDirectory_Invalid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Invalid());
}

TEST_F(DirectoryGTest, TestIsDirectory_Valid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Valid());
}

TEST_F(DirectoryGTest, TestIsDirectory_Valid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Valid());
}

TEST_F(DirectoryGTest, TestIsFile_No) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_No());
}

TEST_F(DirectoryGTest, TestIsFile_No2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_No());
}

TEST_F(DirectoryGTest, TestIsFile_Yes) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_Yes());
}

TEST_F(DirectoryGTest, TestIsFile_Yes2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_Yes());
}

TEST_F(DirectoryGTest, TestGetSize_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_Dir());
}

TEST_F(DirectoryGTest, TestGetSize_Dir2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_Dir());
}

TEST_F(DirectoryGTest, TestGetSize_FileCorrect) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_FileCorrect());
}

TEST_F(DirectoryGTest, TestGetSize_FileCorrect2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_FileCorrect());
}

TEST_F(DirectoryGTest, TestGetSize_FileIncorrect) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_FileIncorrect());
}

TEST_F(DirectoryGTest, TestGetSize_FileIncorrect2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_FileIncorrect());
}

TEST_F(DirectoryGTest, TestCreate_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateDir",false));
}

TEST_F(DirectoryGTest, TestCreate_Dir1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateDir",false));
}

TEST_F(DirectoryGTest, TestCreate_Dir2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("",false));
}

TEST_F(DirectoryGTest, TestCreate_Dir3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("",false));
}

TEST_F(DirectoryGTest, TestCreate_DirNull) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,false));
}

TEST_F(DirectoryGTest, TestCreate_DirNull2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,false));
}

TEST_F(DirectoryGTest, TestCreate_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateFile.txt",true));
}

TEST_F(DirectoryGTest, TestCreate_File1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateFile.txt",true));
}

TEST_F(DirectoryGTest, TestCreate_File2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("",true));
}

TEST_F(DirectoryGTest, TestCreate_File3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("",true));
}

TEST_F(DirectoryGTest, TestCreate_FileNull) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,true));
}

TEST_F(DirectoryGTest, TestCreate_FileNull2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,true));
}

TEST_F(DirectoryGTest, TestDelete_DirNULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete(NULL,false));
}

TEST_F(DirectoryGTest, TestDelete_FileNULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete(NULL,true));
}

TEST_F(DirectoryGTest, TestDelete_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteFile.txt",true));
}

TEST_F(DirectoryGTest, TestDelete_File1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteFile.txt",true));
}

TEST_F(DirectoryGTest, TestDelete_File2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("",true));
}

TEST_F(DirectoryGTest, TestDelete_File3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("",true));
}

TEST_F(DirectoryGTest, TestDelete_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteDir",false));
}

TEST_F(DirectoryGTest, TestDelete_Dir1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteDir",false));
}

TEST_F(DirectoryGTest, TestDelete_Dir2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("",false));
}

TEST_F(DirectoryGTest, TestDelete_Dir3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("",false));
}

TEST_F(DirectoryGTest, TestExists_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists(NULL,false));
}

TEST_F(DirectoryGTest, TestExists_NULL1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists(NULL,true));
}

TEST_F(DirectoryGTest, TestExists_NULL2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("",false));
}

TEST_F(DirectoryGTest, TestExists_NULL3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("",true));
}

TEST_F(DirectoryGTest, TestExists_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("TestExists_Dir",false));
}

TEST_F(DirectoryGTest, TestExists_Dir2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("TestExists_Dir",false));
}

TEST_F(DirectoryGTest, TestExists_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("TestExists_File",true));
}

TEST_F(DirectoryGTest, TestExists_File2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("TestExists_File",true));
}

TEST_F(DirectoryGTest, TestGetLastAccessTime) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime());
}

TEST_F(DirectoryGTest, TestGetLastAccessTime2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime());
}

TEST_F(DirectoryGTest, TestGetLastAccessTime_ReRead) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime_ReRead());
}

TEST_F(DirectoryGTest, TestGetLastAccessTime_ReRead2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime_ReRead());
}

TEST_F(DirectoryGTest, TestGetLastWriteTime) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime());
}

TEST_F(DirectoryGTest, TestGetLastWriteTime2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime());
}

TEST_F(DirectoryGTest, TestGetLastWriteTime_ReWrite) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime_ReWrite());
}

TEST_F(DirectoryGTest, TestGetLastWriteTime_ReWrite2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime_ReWrite());
}

