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
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "DirectoryScannerTest.h"
#include "TestSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(DirectoryScannerGTest, CreateDirectoryFolder) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.CreateDirectoryFolder());
}

TEST(DirectoryScannerGTest, TestDirectorySize) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestDirectorySize());
}

TEST(DirectoryScannerGTest, TestScan) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan("TestScan"));
}

TEST(DirectoryScannerGTest, TestScan2) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan("TestScan1",emptyString));
}

TEST(DirectoryScannerGTest, TestScan_NULL) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NULL(emptyString,emptyString));
}

TEST(DirectoryScannerGTest, TestScan_Empy) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NULL("",""));
}

TEST(DirectoryScannerGTest, TestScan_FailMix) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NULL(emptyString,""));
}

TEST(DirectoryScannerGTest, TestScan_FailMix2) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NULL("",emptyString));
}

TEST(DirectoryScannerGTest, TestScan_FailMix3) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_NoExist("abc",emptyString));
}

TEST(DirectoryScannerGTest, TestScan_Mask) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_Mask());
}

TEST(DirectoryScannerGTest, TestScan_Filter) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestScan_Filter());
}

TEST(DirectoryScannerGTest, TestBasePath) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestBasePath("TestBasePath"));
}

TEST(DirectoryScannerGTest, TestCleanUp) {
    DirectoryScannerTest myClassTest;
    ASSERT_TRUE(myClassTest.TestCleanUp());
}
