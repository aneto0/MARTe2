/**
 * @file HttpMessageInterfaceGTestGTest.cpp
 * @brief Source file for class HttpMessageInterfaceGTest
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
 * the class HttpMessageInterfaceGTest (public, protected, and private). Be aware that some
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
#include "HttpMessageInterfaceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(FileSystem_L4HttpService_HttpMessageInterfaceGTest, TestInitialise) {
    HttpMessageInterfaceTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FileSystem_L4HttpService_HttpMessageInterfaceGTest, TestInitialise_False_Not_Only_Messages) {
    HttpMessageInterfaceTest test;
    ASSERT_TRUE(test.TestInitialise_False_Not_Only_Messages());
}

TEST(FileSystem_L4HttpService_HttpMessageInterfaceGTest, TestGetAsStructuredData) {
    HttpMessageInterfaceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData());
}

TEST(FileSystem_L4HttpService_HttpMessageInterfaceGTest, TestGetAsText) {
    HttpMessageInterfaceTest test;
    ASSERT_TRUE(test.TestGetAsText());
}

TEST(FileSystem_L4HttpService_HttpMessageInterfaceGTest, TestGetAsStructuredData_Message_Cannot_Be_Sent) {
    HttpMessageInterfaceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Message_Cannot_Be_Sent());
}

TEST(FileSystem_L4HttpService_HttpMessageInterfaceGTest, TestGetAsText_Message_Cannot_Be_Sent) {
    HttpMessageInterfaceTest test;
    ASSERT_TRUE(test.TestGetAsText_Message_Cannot_Be_Sent());
}

TEST(FileSystem_L4HttpService_HttpMessageInterfaceGTest, TestGetAsStructuredData_Message_Does_Not_Exist) {
    HttpMessageInterfaceTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Message_Does_Not_Exist());
}

TEST(FileSystem_L4HttpService_HttpMessageInterfaceGTest, TestGetAsText_Message_Does_Not_Exist) {
    HttpMessageInterfaceTest test;
    ASSERT_TRUE(test.TestGetAsText_Message_Does_Not_Exist());
}
