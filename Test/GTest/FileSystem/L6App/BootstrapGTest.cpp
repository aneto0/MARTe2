/**
 * @file BootstrapGTest.cpp
 * @brief Source file for class BootstrapGTest
 * @date 05/04/2018
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
 * the class BootstrapGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "BootstrapTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(FileSystem_L6App_BootstrapGTest, TestConstructor) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(FileSystem_L6App_BootstrapGTest, TestReadParameters) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestReadParameters());
}

TEST(FileSystem_L6App_BootstrapGTest, TestReadParameters_FalseNoLoader) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestReadParameters_FalseNoLoader());
}

TEST(FileSystem_L6App_BootstrapGTest, TestReadParameters_FalseNoFilename) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestReadParameters_FalseNoFilename());
}

TEST(FileSystem_L6App_BootstrapGTest, TestGetConfigurationStream) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestGetConfigurationStream());
}

TEST(FileSystem_L6App_BootstrapGTest, TestGetConfigurationStream_False) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestGetConfigurationStream_False());
}

TEST(FileSystem_L6App_BootstrapGTest, TestCloseConfigurationStream) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestCloseConfigurationStream());
}

TEST(FileSystem_L6App_BootstrapGTest, TestCloseConfigurationStream_False) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestCloseConfigurationStream_False());
}

TEST(FileSystem_L6App_BootstrapGTest, TestPrintf) {
    BootstrapTest test;
    ASSERT_TRUE(test.TestPrintf());
}
