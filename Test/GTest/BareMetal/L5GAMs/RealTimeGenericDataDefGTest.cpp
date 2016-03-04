/**
 * @file RealTimeGenericDataDefGTest.cpp
 * @brief Source file for class RealTimeGenericDataDefGTest
 * @date 03/mar/2016
 * @author pc
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
 * the class RealTimeGenericDataDefGTest (public, protected, and private). Be aware that some 
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

#include "RealTimeGenericDataDefTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(RealTimeGenericDataDefGTest,TestConstructor) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestConstructor());
}

TEST(RealTimeGenericDataDefGTest,TestInitialiseTrue) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestInitialise());
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocal) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocal());
}


TEST(RealTimeGenericDataDefGTest,TestMergeWithLocalFalse_FinalGlobal) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocalFalse_FinalGlobal());
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocalFalse_BothFinal) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocalFalse_BothFinal());
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocal_AddType) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocal_AddType());
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocal_AddPath) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocal_AddPath());
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocal_AddDefaultValues) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocal_AddDefaultValues());
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocal_DifferentTypes) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocal_DifferentTypes());
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocal_DifferentPaths) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocal_DifferentPaths());
}

TEST(RealTimeGenericDataDefGTest,TestGetDefaultValue) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestGetDefaultValue());
}

TEST(RealTimeGenericDataDefGTest,TestToStructuredData) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestToStructuredData());
}

TEST(RealTimeGenericDataDefGTest,TestVerify) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestVerify());
}

TEST(RealTimeGenericDataDefGTest,TestVerifyFalse_UnregisteredType) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestVerifyFalse_UnregisteredType());
}

TEST(RealTimeGenericDataDefGTest,TestVerifyFalse_UnintrospectableType) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestVerifyFalse_UnintrospectableType());
}

TEST(RealTimeGenericDataDefGTest,TestVerifyFalse_InvalidMemberName) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestVerifyFalse_InvalidMemberName());
}

TEST(RealTimeGenericDataDefGTest,TestVerifyFalse_InvalidMemberType) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestVerifyFalse_InvalidMemberType());
}

TEST(RealTimeGenericDataDefGTest,TestMergeWithLocal_DifferentDefaultValues) {
    RealTimeGenericDataDefTest rtgdTest;
    ASSERT_TRUE(rtgdTest.TestMergeWithLocal_DifferentDefaultValues());
}

