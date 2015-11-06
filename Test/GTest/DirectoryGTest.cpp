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


TEST(DirectoryGTest, TestNameInvalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestNameInvalid());
}
TEST(DirectoryGTest, TestNameValid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestNameValid());
}

TEST(DirectoryGTest, TestIsDirectoryInvalid) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsDirectoryInvalid());
}
TEST(DirectoryGTest, TestIsNoFile) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsNoFile());
}

TEST(DirectoryGTest, TestIsFile) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestIsFile());
}

TEST(DirectoryGTest, TestReadOnly) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestReadOnly());
}

TEST(DirectoryGTest, TestSizeDir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSizeDir("WWWW/TestSizeDir"));
}
TEST(DirectoryGTest, TestSizeDirNULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSizeDir("WWWW/"));
}


TEST(DirectoryGTest, TestSizeFileFile) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSizeFile("WWWW/TestSizeFileFile.txt"));
}
TEST(DirectoryGTest, TestSizeFileFileC) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestSizeFile("WWWW/TestSizeFileFileC.c"));
}

TEST(DirectoryGTest, TestCreateDir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("WWWW/TestCreateDir",false));
}
TEST(DirectoryGTest, TestCreateDir0) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,false));
}
TEST(DirectoryGTest, TestCreateFile) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate("WWWW/TestCreateFile.txt",true));
}
TEST(DirectoryGTest, TestCreateFileNull) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCreate(NULL,true));
}

TEST(DirectoryGTest, TestDeleteNULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete(NULL,false));
}
TEST(DirectoryGTest, TestDeleteFile) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("WWWW/TestDeleteFile.txt",true));
}
TEST(DirectoryGTest, TestDeleteDir) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDelete("WWWW/TestDeleteDir",false));
}

TEST(DirectoryGTest, TestExists_NULL) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("WWWW/NULL"));
}
TEST(DirectoryGTest, TestExists_Exist) {
    DirectoryTest myClassTest;
    ASSERT_TRUE(myClassTest.TestExists("WWWW/TestExists_Exist"));
}
