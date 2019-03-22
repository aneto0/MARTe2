/**
 * @file HttpDirectoryResource.cpp
 * @brief Source file for class HttpDirectoryResource
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
 * the class HttpDirectoryResource (public, protected, and private). Be aware that some
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
#include "HttpDirectoryResourceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

#if 0
TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_ObjectRegistryDatabase_Root) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_ObjectRegistryDatabase_Root());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_Self_Root) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_Self_Root());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_Other_Root) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_Other_Root());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_False_No_Root) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_No_Root());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_False_Bad_Root_1) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Bad_Root_1());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_False_Bad_Root_2) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Bad_Root_2());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_Realm) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_Realm());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_False_Realm) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Realm());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_This_Self) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_This_Self());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_This_ObjectRegistryDatabase) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_This_ObjectRegistryDatabase());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_This_Other) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_This_Other());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_Other_Not_HttpObjectBrowser) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Other_Not_HttpObjectBrowser());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_Other_Not_HttpObjectBrowser_Long_Path) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Other_Not_HttpObjectBrowser_Long_Path());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_InvalidTarget) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_InvalidTarget());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_This) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_This());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_This_FileNotFound) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_This_FileNotFound());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_Other) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_Other());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_Other_Not_HttpDataExportI) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_Other_Not_HttpDataExportI());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_InvalidObject) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_InvalidObject());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_Realm) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Realm());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_Realm) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_Realm());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_Realm_False) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Realm_False());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_Realm_False) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_Realm_False());
}
#endif
