/**
 * @file DirectoryGTest.cpp
 * @brief Source file for class DirectoryGTest
 * @date 05/11/2015
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

#include "MARTe2UTest.h"

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

TEST(FileSystem_L1Portability_DirectoryGTest, TestDirectoryTest) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDirectoryTest("TestDirectoryTest"));
}
TEST(FileSystem_L1Portability_DirectoryGTest, TestDirectoryTest_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDirectoryTest(NULL));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestSetByName_Valid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_Valid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestSetByName_Valid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_Valid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestSetByName_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_NULL());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestSetByName_NULL2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_NULL());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestSetByName_Invalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_Invalid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestSetByName_Invalid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName_Invalid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetName_Invalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetName_Invalid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetName_Invalid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetName_Invalid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestName_Valid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetName_Valid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestName_Valid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetName_Valid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestIsDirectory_Invalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Invalid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestIsDirectory_Invalid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Invalid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestIsDirectory_Valid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Valid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestIsDirectory_Valid2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Valid());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestIsFile_No) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_No());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestIsFile_No2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_No());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestIsFile_Yes) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_Yes());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestIsFile_Yes2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_Yes());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetSize_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_Dir());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetSize_Dir2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_Dir());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetSize_FileCorrect) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_FileCorrect());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetSize_FileCorrect2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_FileCorrect());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetSize_FileIncorrect) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_FileIncorrect());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetSize_FileIncorrect2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_FileIncorrect());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateDir",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_Dir1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateDir",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_Dir2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_Dir3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_DirNull) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_DirNull2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateFile.txt",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_File1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateFile.txt",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_File2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_File3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_FileNull) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestCreate_FileNull2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_DirNULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete(NULL,false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_FileNULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete(NULL,true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteFile.txt",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_File1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteFile.txt",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_File2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_File3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteDir",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_Dir1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteDir",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_Dir2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestDelete_Dir3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestExists_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists(NULL,false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestExists_NULL1) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists(NULL,true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestExists_NULL2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestExists_NULL3) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestExists_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("TestExists_Dir",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestExists_Dir2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("TestExists_Dir",false));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestExists_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("TestExists_File",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestExists_File2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("TestExists_File",true));
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetLastAccessTime) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetLastAccessTime2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetLastAccessTime_ReRead) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime_ReRead());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetLastAccessTime_ReRead2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime_ReRead());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetLastWriteTime) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetLastWriteTime2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetLastWriteTime_ReWrite) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime_ReWrite());
}

TEST(FileSystem_L1Portability_DirectoryGTest, TestGetLastWriteTime_ReWrite2) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime_ReWrite());
}

