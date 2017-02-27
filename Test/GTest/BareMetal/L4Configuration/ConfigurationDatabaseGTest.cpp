/**
 * @file ConfigurationDatabaseGTest.cpp
 * @brief Source file for class ConfigurationDatabaseGTest
 * @date Nov 19, 2015
 * @author aneto
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
 * the class ConfigurationDatabaseGTest (public, protected, and private). Be aware that some 
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
#include "ConfigurationDatabaseTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(ConfigurationDatabaseGTest,TestConstructor) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestDefaultConstructor());
}

TEST(ConfigurationDatabaseGTest,TestCreateAbsolute_ValidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateAbsolute_ValidPath());
}

TEST(ConfigurationDatabaseGTest,TestCreateAbsolute_InvalidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateAbsolute_InvalidPath());
}

TEST(ConfigurationDatabaseGTest,TestCreateRelative_ValidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateRelative_ValidPath());
}

TEST(ConfigurationDatabaseGTest,TestCreateRelative_InvalidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateRelative_InvalidPath());
}

TEST(ConfigurationDatabaseGTest,TestMoveToRoot) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToRoot());
}

TEST(ConfigurationDatabaseGTest,TestMoveToAncestor_ValidNumberOfGenerations) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToAncestor_ValidNumberOfGenerations());
}

TEST(ConfigurationDatabaseGTest,TestMoveToAncestor_InvalidNumberOfGenerations) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToAncestor_InvalidNumberOfGenerations());
}

TEST(ConfigurationDatabaseGTest,TestMoveAbsolute_ValidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveAbsolute_ValidPaths());
}

TEST(ConfigurationDatabaseGTest,TestMoveAbsolute_InvalidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveAbsolute_InvalidPaths());
}

TEST(ConfigurationDatabaseGTest,TestMoveRelative_ValidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveRelative_ValidPaths());
}

TEST(ConfigurationDatabaseGTest,TestMoveRelative_InvalidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveRelative_InvalidPaths());
}

TEST(ConfigurationDatabaseGTest,TestDelete) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestDelete());
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_UInt8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::uint8(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_Int8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::int8(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_UInt16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::uint16(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_Int16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::int16(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_UInt32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::uint32(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_Int32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::int32(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_UInt64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::uint64(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_Int64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::int64(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_Float32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::float32(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid_Float64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::float64(7)));
}

TEST(ConfigurationDatabaseGTest,TestRead_Invalid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Invalid());
}

TEST(ConfigurationDatabaseGTest,TestRead_StructuredData) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_StructuredData());
}

TEST(ConfigurationDatabaseGTest,TestRead_Object) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Object());
}

TEST(ConfigurationDatabaseGTest,TestAddToCurrentNode) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestAddToCurrentNode());
}

TEST(ConfigurationDatabaseGTest,TestAddToCurrentNode_InvalidReference) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestAddToCurrentNode_InvalidReference());
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_UInt8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::uint8(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_Int8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::int8(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_UInt16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::uint16(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_Int16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::int16(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_UInt32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::uint32(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_Int32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::int32(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_UInt64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::uint64(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_Int64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::int64(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_Float32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::float32(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid_Float64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::float64(7)));
}

TEST(ConfigurationDatabaseGTest,TestWrite_Overwrite) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Overwrite());
}

TEST(ConfigurationDatabaseGTest,TestWrite_Invalid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Invalid());
}

TEST(ConfigurationDatabaseGTest,TestWrite_StructuredData) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_StructuredData());
}

TEST(ConfigurationDatabaseGTest,TestWrite_Object) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Object());
}

TEST(ConfigurationDatabaseGTest,TestGetType_Invalid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Invalid());
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_UInt8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::uint8(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_Int8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::int8(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_UInt16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::uint16(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_Int16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::int16(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_UInt32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::uint32(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_Int32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::int32(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_UInt64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::uint64(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_Int64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::int64(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_Float32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::float32(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetType_Valid_Float64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::float64(7)));
}

TEST(ConfigurationDatabaseGTest,TestGetChildName) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetChildName());
}

TEST(ConfigurationDatabaseGTest,TestGetName) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetName());
}
TEST(ConfigurationDatabaseGTest,TestGetNumberOfChildren) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetNumberOfChildren());
}

TEST(ConfigurationDatabaseGTest,TestCopy) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCopy());
}

TEST(ConfigurationDatabaseGTest,TestLock) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestLock());
}

TEST(ConfigurationDatabaseGTest,TestUnlock) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestUnlock());
}

TEST(ConfigurationDatabaseGTest,TestInitialise) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestInitialise());
}

TEST(ConfigurationDatabaseGTest,TestCleanUp) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCleanUp());
}

TEST(ConfigurationDatabaseGTest,TestGetCurrentNode) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetCurrentNode());
}

TEST(ConfigurationDatabaseGTest,TestCopyConstructor) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCopyConstructor());
}

TEST(ConfigurationDatabaseGTest,TestCopyOperatorEqual) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCopyOperatorEqual());
}

TEST(ConfigurationDatabaseGTest,TestMoveToChild) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToChild());
}

TEST(ConfigurationDatabaseGTest,TestSetCurrentNodeAsRootNode) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestSetCurrentNodeAsRootNode());
}
