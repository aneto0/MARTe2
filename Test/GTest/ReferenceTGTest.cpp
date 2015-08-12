/**
 * @file ReferenceTGTest.cpp
 * @brief Source file for class ReferenceTGTest
 * @date 10/ago/2015
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
 * the class ReferenceTGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceTTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ReferenceTGTest,TestDefaultConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestDefaultConstructor());
}

TEST(ReferenceTGTest,TestCopyConstructorReference) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReference());
}

TEST(ReferenceTGTest,TestCopyConstructorReferenceParentToChild) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceParentToChild());
}

TEST(ReferenceTGTest,TestCopyConstructorReferenceChildToParent) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceChildToParent());
}

TEST(ReferenceTGTest,TestCopyConstructorReferenceT) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceT());
}

TEST(ReferenceTGTest,TestCopyConstructorReferenceTParentToChild) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceTParentToChild());
}

TEST(ReferenceTGTest,TestCopyConstructorReferenceTChildToParent) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceTChildToParent());
}

TEST(ReferenceTGTest,TestCopyConstructorNullPtr) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorNullPtr());
}

TEST(ReferenceTGTest,TestCreateConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCreateConstructor());
}

TEST(ReferenceTGTest,TestBuildObjectConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestBuildObjectConstructor());
}

TEST(ReferenceTGTest,TestBuildFakeObjectConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestBuildFakeObjectConstructor());
}

TEST(ReferenceTGTest,TestCopyFromObjPtrConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyFromObjPtrConstructor());
}

TEST(ReferenceTGTest,TestCopyFromObjPtrConstructorNullPtr) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyFromObjPtrConstructorNullPtr());
}

TEST(ReferenceTGTest,TestDestructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestDestructor());
}

TEST(ReferenceTGTest,TestInitialise) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestInitialise());
}

TEST(ReferenceTGTest,TestRemoveReference) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestRemoveReference());
}

TEST(ReferenceTGTest,TestCopyOperatorReference) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorReference());
}

TEST(ReferenceTGTest,TestCopyOperatorReferenceNull) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorReferenceNull());
}

TEST(ReferenceTGTest,TestCopyOperatorReferenceT) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorReferenceT());
}

TEST(ReferenceTGTest,TestCopyOperatorReferenceTNull) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorReferenceTNull());
}

TEST(ReferenceTGTest,TestCopyOperatorObject) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorObject());
}

TEST(ReferenceTGTest,TestCopyOperatorObjectNull) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorObjectNull());
}

TEST(ReferenceTGTest,TestIsValid) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestIsValid());
}

TEST(ReferenceTGTest,TestEqualOperator) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestEqualOperator());
}

TEST(ReferenceTGTest,TestDifferentOperator) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestDifferentOperator());
}

TEST(ReferenceTGTest,TestNumberOfReferences) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestNumberOfReferences());
}

TEST(ReferenceTGTest,TestInFunctionOnStack) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestInFunctionOnStack());
}

TEST(ReferenceTGTest,TestInFunctionOnHeap) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestInFunctionOnHeap(32));
}

TEST(ReferenceTGTest,TestRightInherithance) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestRightInherithance());
}

TEST(ReferenceTGTest,TestWrongInherithance) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestWrongInherithance());
}
