/**
 * @file UDPLoggerGTest.cpp
 * @brief Source file for class UDPLoggerGTest
 * @date 15/03/2017
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
 * the class UDPLoggerGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "UDPLoggerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(FileSystem_L4LoggerService_UDPLoggerGTest, TestConstructor) {
    UDPLoggerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileSystem_L4LoggerService_UDPLoggerGTest, TestInitialise) {
    UDPLoggerTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(FileSystem_L4LoggerService_UDPLoggerGTest, TestInitialise_False_Address) {
    UDPLoggerTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address());
}

TEST(FileSystem_L4LoggerService_UDPLoggerGTest, TestInitialise_False_Port) {
    UDPLoggerTest test;
    ASSERT_TRUE(test.TestInitialise_False_Port());
}

TEST(FileSystem_L4LoggerService_UDPLoggerGTest, TestInitialise_False_Address_Port) {
    UDPLoggerTest test;
    ASSERT_TRUE(test.TestInitialise_False_Address_Port());
}

TEST(FileSystem_L4LoggerService_UDPLoggerGTest, TestConsumeLogMessage) {
    UDPLoggerTest test;
    ASSERT_TRUE(test.TestConsumeLogMessage());
}
	
