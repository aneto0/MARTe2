/**
 * @file ConfigurationLoaderTCPGTest.cpp
 * @brief Source file for class ConfigurationLoaderTCPGTest
 * @date 16/02/2022
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
 * the class ConfigurationLoaderTCPGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationLoaderTCPTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(FileSystem_L6App_ConfigurationLoaderTCPGTest,TestConstructor) {
    ConfigurationLoaderTCPTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileSystem_L6App_ConfigurationLoaderTCPGTest,TestPostInit) {
    ConfigurationLoaderTCPTest test;
    ASSERT_TRUE(test.TestPostInit());
}

TEST(FileSystem_L6App_ConfigurationLoaderTCPGTest,TestPostInit_NoPort) {
    ConfigurationLoaderTCPTest test;
    ASSERT_TRUE(test.TestPostInit_NoPort());
}

TEST(FileSystem_L6App_ConfigurationLoaderTCPGTest,TestPostInit_InvalidPort) {
    ConfigurationLoaderTCPTest test;
    ASSERT_TRUE(test.TestPostInit_InvalidPort());
}

TEST(FileSystem_L6App_ConfigurationLoaderTCPGTest,TestExecute) {
    ConfigurationLoaderTCPTest test;
    ASSERT_TRUE(test.TestExecute());
}

TEST(FileSystem_L6App_ConfigurationLoaderTCPGTest,TestExecuteErr) {
    ConfigurationLoaderTCPTest test;
    ASSERT_TRUE(test.TestExecuteErr());
}


