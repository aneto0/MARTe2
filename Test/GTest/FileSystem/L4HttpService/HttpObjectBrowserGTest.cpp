/**
 * @file HttpObjectBrowserGTest.cpp
 * @brief Source file for class HttpObjectBrowserGTest
 * @date 18/03/2019
 * @author Andre Neto
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
 * the class HttpObjectBrowserGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "HttpObjectBrowserTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(HttpObjectBrowserGTest, TestInitialise) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(HttpObjectBrowserGTest, TestInitialise_ObjectRegistryDatabase_Root) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise_ObjectRegistryDatabase_Root());
}

TEST(HttpObjectBrowserGTest, TestInitialise_Self_Root) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise_Self_Root());
}

TEST(HttpObjectBrowserGTest, TestInitialise_Other_Root) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise_Other_Root());
}

TEST(HttpObjectBrowserGTest, TestInitialise_False_No_Root) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise_False_No_Root());
}

TEST(HttpObjectBrowserGTest, TestInitialise_False_Bad_Root_1) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise_False_Bad_Root_1());
}

TEST(HttpObjectBrowserGTest, TestInitialise_False_Bad_Root_2) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise_False_Bad_Root_2());
}

TEST(HttpObjectBrowserGTest, TestInitialise_Realm) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise_Realm());
}

TEST(HttpObjectBrowserGTest, TestInitialise_False_Realm) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestInitialise_False_Realm());
}

TEST(HttpObjectBrowserGTest, TestGetAsStructuredData_This_Self) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_This_Self());
}

TEST(HttpObjectBrowserGTest, TestGetAsStructuredData_This_ObjectRegistryDatabase) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_This_ObjectRegistryDatabase());
}

TEST(HttpObjectBrowserGTest, TestGetAsStructuredData_This_Other) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_This_Other());
}

TEST(HttpObjectBrowserGTest, TestGetAsStructuredData_Other_Not_HttpObjectBrowser) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Other_Not_HttpObjectBrowser());
}

TEST(HttpObjectBrowserGTest, TestGetAsStructuredData_Other_Not_HttpObjectBrowser_Long_Path) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Other_Not_HttpObjectBrowser_Long_Path());
}

TEST(HttpObjectBrowserGTest, TestGetAsStructuredData_InvalidTarget) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_InvalidTarget());
}

TEST(HttpObjectBrowserGTest, TestGetAsText_This) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsText_This());
}

TEST(HttpObjectBrowserGTest, TestGetAsText_This_FileNotFound) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsText_This_FileNotFound());
}

TEST(HttpObjectBrowserGTest, TestGetAsText_Other) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsText_Other());
}

TEST(HttpObjectBrowserGTest, TestGetAsText_Other_Not_HttpDataExportI) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsText_Other_Not_HttpDataExportI());
}

TEST(HttpObjectBrowserGTest, TestGetAsText_InvalidObject) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsText_InvalidObject());
}

TEST(HttpObjectBrowserGTest, TestGetAsStructuredData_Realm) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Realm());
}

TEST(HttpObjectBrowserGTest, TestGetAsText_Realm) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsText_Realm());
}

TEST(HttpObjectBrowserGTest, TestGetAsStructuredData_Realm_False) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Realm_False());
}

TEST(HttpObjectBrowserGTest, TestGetAsText_Realm_False) {
    HttpObjectBrowserTest test;
    ASSERT_TRUE(test.TestGetAsText_Realm_False());
}
