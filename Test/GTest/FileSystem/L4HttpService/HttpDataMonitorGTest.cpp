/**
 * @file HttpDataMonitorGTestGTest.cpp
 * @brief Source file for class HttpDataMonitorGTest
 * @date 05/08/2025
 * @author Giuseppe Ferro
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
 * the class HttpDataMonitorGTest (public, protected, and private). Be aware that some
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
#include "HttpDataMonitorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestConstructor) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestDestructor) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestDestructor());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestInitialise) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestInitialise_False_NoSignals) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoSignals());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestInitialise_False_NoComponents) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestInitialise_False_NoComponents());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestInitialise_False_SignalsComponentsMismatch) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestInitialise_False_SignalsComponentsMismatch());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestGetAsStructuredData) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestGetAsStructuredData_Dir) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_Dir());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestGetAsStructuredData_False_InvalidComponent) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestGetAsStructuredData_False_InvalidComponent());
}

TEST(FileSystem_L4HttpService_HttpDataMonitorGTest, TestGetAsText) {
    HttpDataMonitorTest test;
    ASSERT_TRUE(test.TestGetAsText());
}
