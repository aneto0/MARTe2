/**
 * @file StructuredDataIHelperGTest.cpp
 * @brief Source file for class StructuredDataIHelperGTest
 * @date 18/06/2022
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
 * the class StructuredDataIHelperGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "StructuredDataIHelperTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestConstructor) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestResetErrors) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestResetErrors());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestHasErrors) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestHasErrors());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_False) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_False());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_DefaultValue) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_DefaultValue());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_DefaultValue_BadConversion) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_DefaultValue_BadConversion());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadEnum) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadEnum());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadEnum_DefaultValue) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadEnum_DefaultValue());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadEnum_False_NoDefaultValue) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadEnum_False_NoDefaultValue());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadEnum_False_InvalidOption) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadEnum_False_InvalidOption());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadEnum_False_OptionSizeMismatch) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadEnum_False_OptionSizeMismatch());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadEnum_False_BadConversion) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadEnum_False_BadConversion());
}





#if 0
TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCreateAbsolute_ValidPath) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCreateAbsolute_ValidPath());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCreateAbsolute_InvalidPath) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCreateAbsolute_InvalidPath());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCreateRelative_ValidPath) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCreateRelative_ValidPath());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCreateRelative_InvalidPath) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCreateRelative_InvalidPath());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveToRoot) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveToRoot());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveToAncestor_ValidNumberOfGenerations) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveToAncestor_ValidNumberOfGenerations());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveToAncestor_InvalidNumberOfGenerations) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveToAncestor_InvalidNumberOfGenerations());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveAbsolute_ValidPaths) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveAbsolute_ValidPaths());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveAbsolute_InvalidPaths) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveAbsolute_InvalidPaths());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveRelative_ValidPaths) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveRelative_ValidPaths());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveRelative_InvalidPaths) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveRelative_InvalidPaths());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestDelete) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestDelete());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_UInt8) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_Int8) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_UInt16) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_Int16) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_UInt32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_Bool) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(bool(true)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_Int32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_UInt64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_Int64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_Float32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Valid_Float64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Valid(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Invalid) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Invalid());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_StructuredData) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_StructuredData());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestRead_Object) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestRead_Object());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestAddToCurrentNode) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestAddToCurrentNode());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestAddToCurrentNode_InvalidReference) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestAddToCurrentNode_InvalidReference());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_UInt8) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_Int8) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_UInt16) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_Int16) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_UInt32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_Bool) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(bool(true)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_Int32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_UInt64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_Int64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_Float32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Valid_Float64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Valid(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Overwrite) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Overwrite());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Invalid) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Invalid());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_StructuredData) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_StructuredData());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite_Object) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite_Object());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Invalid) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Invalid());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_UInt8) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::uint8(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_Int8) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::int8(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_UInt16) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::uint16(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_Int16) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::int16(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_UInt32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::uint32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_Bool) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(bool(true)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_Int32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::int32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_UInt64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::uint64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_Int64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::int64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_Float32) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::float32(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType_Valid_Float64) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType_Valid(MARTe::float64(7)));
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetChildName) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetChildName());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetName) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetName());
}
TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetNumberOfChildren) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetNumberOfChildren());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCopy) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCopy());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestLock) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestLock());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestUnlock) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestUnlock());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestInitialise) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestInitialise());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCleanUp) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCleanUp());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetCurrentNode) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetCurrentNode());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCopyConstructor) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCopyConstructor());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCopyOperatorEqual) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCopyOperatorEqual());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveToChild) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveToChild());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestSetCurrentNodeAsRootNode) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestSetCurrentNodeAsRootNode());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestPurge) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestPurge());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCopyOperatorEqualBug) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCopyOperatorEqualBug());
}
#endif

