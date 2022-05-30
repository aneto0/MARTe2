/**
 * @file ConfigurationDatabaseGTest.cpp
 * @brief Source file for class ConfigurationDatabaseGTest
 * @date 19/11/2015
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
 * the class ConfigurationDatabaseGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

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
TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestConstructor) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCreateAbsolute_ValidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateAbsolute_ValidPath());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCreateAbsolute_InvalidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateAbsolute_InvalidPath());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCreateRelative_ValidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateRelative_ValidPath());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCreateRelative_InvalidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateRelative_InvalidPath());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestMoveToRoot) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToRoot());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestMoveToAncestor_ValidNumberOfGenerations) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToAncestor_ValidNumberOfGenerations());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestMoveToAncestor_InvalidNumberOfGenerations) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToAncestor_InvalidNumberOfGenerations());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestMoveAbsolute_ValidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveAbsolute_ValidPaths());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestMoveAbsolute_InvalidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveAbsolute_InvalidPaths());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestMoveRelative_ValidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveRelative_ValidPaths());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestMoveRelative_InvalidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveRelative_InvalidPaths());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestDelete) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestDelete());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_UInt8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_Int8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_UInt16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_Int16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_UInt32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_Bool) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(bool(true)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_Int32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_UInt64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_Int64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_Float32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Valid_Float64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Invalid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Invalid());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_StructuredData) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_StructuredData());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestRead_Object) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Object());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestAddToCurrentNode) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestAddToCurrentNode());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestAddToCurrentNode_InvalidReference) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestAddToCurrentNode_InvalidReference());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_UInt8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_Int8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_UInt16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_Int16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_UInt32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_Bool) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(bool(true)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_Int32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_UInt64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_Int64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_Float32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Valid_Float64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Overwrite) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Overwrite());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Invalid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Invalid());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_StructuredData) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_StructuredData());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestWrite_Object) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Object());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Invalid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Invalid());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_UInt8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_Int8) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_UInt16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_Int16) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_UInt32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_Bool) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(bool(true)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_Int32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_UInt64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_Int64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_Float32) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetType_Valid_Float64) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetType_Valid(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetChildName) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetChildName());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetName) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetName());
}
TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetNumberOfChildren) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetNumberOfChildren());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCopy) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCopy());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestLock) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestLock());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestUnlock) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestUnlock());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestInitialise) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestInitialise());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCleanUp) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCleanUp());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestGetCurrentNode) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetCurrentNode());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCopyConstructor) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCopyConstructor());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCopyOperatorEqual) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCopyOperatorEqual());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestMoveToChild) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToChild());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestSetCurrentNodeAsRootNode) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestSetCurrentNodeAsRootNode());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestPurge) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestPurge());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseGTest,TestCopyOperatorEqualBug) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCopyOperatorEqualBug());
}

