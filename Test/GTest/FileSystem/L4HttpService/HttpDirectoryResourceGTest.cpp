/**
 * @file HttpDirectoryResourceGTest.cpp
 * @brief Source file for class HttpDirectoryResourceGTest.cpp
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
 * the class HttpDirectoryResourceGTest.cpp (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

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

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestInitialise_False_No_BaseDir) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestInitialise_False_No_BaseDir());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_Directory) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Directory());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsStructuredData_File) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_File());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_File_Head) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_File_Head());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestGetAsText_MimeTypes) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestGetAsText_MimeTypes());
}

TEST(FileSystem_L4HttpService_HttpDirectoryResource, TestSetReplyNotFound) {
    HttpDirectoryResourceTest test;
    ASSERT_TRUE(test.TestSetReplyNotFound());
}
