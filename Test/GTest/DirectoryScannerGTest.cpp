/**
 * @file DirectoryScannerGTest.cpp
 * @brief Source file for class DirectoryScannerGTest
 * @date Nov 10, 2015
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
 * the class DirectoryScannerGTest (public, protected, and private). Be aware that some 
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
#include "DirectoryScannerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(DirectoryGTest, CreateDirectoryFolder) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.CreateDirectoryFolder());
}

TEST(DirectoryGTest, TestDirectorySize) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDirectorySize());
}

TEST(DirectoryGTest, TestScan) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan("TestScan"));
}

TEST(DirectoryGTest, TestScan2) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan("TestScan1",NULL));
}

TEST(DirectoryGTest, TestScan_NULL) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NULL(NULL,NULL));
}

TEST(DirectoryGTest, TestScan_Empy) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NULL("",""));
}

TEST(DirectoryGTest, TestScan_FailMix) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NULL(NULL,""));
}

TEST(DirectoryGTest, TestScan_FailMix2) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NULL("",NULL));
}

TEST(DirectoryGTest, TestScan_FailMix3) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NoExist("abc",NULL));
}

TEST(DirectoryGTest, TestScan_Mask) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_Mask());
}

TEST(DirectoryGTest, TestScan_Filter) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_Filter());
}

TEST(DirectoryGTest, TestBasePath) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestBasePath("TestBasePath"));
}

TEST(DirectoryGTest, TestCleanUp) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCleanUp());
}
