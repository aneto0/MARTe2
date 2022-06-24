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

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadValidated) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadValidated());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadValidated_InvalidCondition) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadValidated_InvalidCondition());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadArray) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadArray());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadArray_False) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadArray_False());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadMatrix) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadMatrix());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadMatrix_False) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadMatrix_False());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestReadMatrix_False_Dimensions) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestReadMatrix_False_Dimensions());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestWrite) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestWrite());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCopy) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCopy());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetType) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetType());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveToRoot) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveToRoot());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveToAncestor) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveToAncestor());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveAbsolute) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveAbsolute());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveRelative) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveRelative());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestMoveToChild) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestMoveToChild());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCreateAbsolute) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCreateAbsolute());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestCreateRelative) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestCreateRelative());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestAddToCurrentNode) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestAddToCurrentNode());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestDelete) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestDelete());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetName) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetName());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetChildName) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetChildName());
}

TEST(BareMetal_L4Configuration_StructuredDataIHelperGTest,TestGetNumberOfChildren) {
    StructuredDataIHelperTest sdiTest;
    ASSERT_TRUE(sdiTest.TestGetNumberOfChildren());
}

