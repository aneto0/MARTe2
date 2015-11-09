/**
 * @file DirectoryGTest.cpp
 * @brief Source file for class DirectoryGTest
 * @date Nov 5, 2015
 * @author fperez
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


TEST(DirectoryGTest, TestSetByName) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSetByName());
}


TEST(DirectoryGTest, TestName_Invalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestName_Invalid());
}
TEST(DirectoryGTest, TestName_Valid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestName_Valid());
}

TEST(DirectoryGTest, TestIsDirectory_Invalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Invalid());
}
TEST(DirectoryGTest, TestIsDirectory_Valid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectory_Valid());
}

TEST(DirectoryGTest, TestIsFile_No) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_No());
}

TEST(DirectoryGTest, TestIsFile_Yes) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile_Yes());
}

TEST(DirectoryGTest, TestGetSize_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_Dir());
}

TEST(DirectoryGTest, TestGetSize_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetSize_File());
}

TEST(DirectoryGTest, TestCreate_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateDir",false));
}
TEST(DirectoryGTest, TestCreate_DirNull) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,false));
}
TEST(DirectoryGTest, TestCreate_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("TestCreateFile.txt",true));
}
TEST(DirectoryGTest, TestCreate_FileNull) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,true));
}

TEST(DirectoryGTest, TestDelete_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete(NULL,false));
}
TEST(DirectoryGTest, TestDelete_File) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteFile.txt",true));
}
TEST(DirectoryGTest, TestDelete_Dir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("TestDeleteDir",false));
}

TEST(DirectoryGTest, TestExists_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("GTestDirectoryTest/NULL"));
}
TEST(DirectoryGTest, TestExists_Exist) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("GTestDirectoryTest/TestExists_Exist"));
}

TEST(DirectoryGTest, TestGetLastAccessTime) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastAccessTime());
}

TEST(DirectoryGTest, TestGetLastWriteTime) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestGetLastWriteTime());
}
