/**
 * @file ReferenceTGTest.cpp
 * @brief Source file for class ReferenceTGTest
 * @date 10/08/2015
 * @author Giusepper Ferrò
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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceTTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ReferenceTGTest,TestDefaultConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestDefaultConstructor());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyConstructorReference) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReference());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyConstructorReferenceParentToChild) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceParentToChild());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyConstructorReferenceChildToParent) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceChildToParent());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyConstructorReferenceT) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceT());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyConstructorReferenceTParentToChild) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceTParentToChild());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyConstructorReferenceTChildToParent) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorReferenceTChildToParent());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyConstructorNullPtr) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyConstructorNullPtr());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCreateConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCreateConstructor());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestBuildObjectConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestBuildObjectConstructor());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestBuildFakeObjectConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestBuildFakeObjectConstructor());
}


TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyFromObjPtrConstructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyFromObjPtrConstructor());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyFromObjPtrConstructorNullPtr) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyFromObjPtrConstructorNullPtr());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestDestructor) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestDestructor());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestInitialiseCreation) {
    ReferenceTTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseCreation());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestInitialiseNoCreation) {
    ReferenceTTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseNoCreation());
}


TEST(BareMetal_L2Objects_ReferenceTGTest,TestInitialiseNoObject) {
    ReferenceTTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseNoObject());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestInitialiseIncompatibleCast) {
    ReferenceTTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseNoObject());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestInitialiseNoClassName) {
    ReferenceTTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseNoClassName());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestRemoveReference) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestRemoveReference());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyOperatorReference) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorReference());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyOperatorReferenceNull) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorReferenceNull());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyOperatorReferenceT) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorReferenceT());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyOperatorReferenceTNull) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorReferenceTNull());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyOperatorObject) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorObject());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestCopyOperatorObjectNull) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestCopyOperatorObjectNull());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestIsValid) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestIsValid());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestEqualOperator) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestEqualOperator());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestDifferentOperator) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestDifferentOperator());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestNumberOfReferences) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestNumberOfReferences());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestInFunctionOnStack) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestInFunctionOnStack());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestInFunctionOnHeap) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestInFunctionOnHeap(32));
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestRightInherithance) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestRightInherithance());
}

TEST(BareMetal_L2Objects_ReferenceTGTest,TestWrongInherithance) {
    ReferenceTTest referenceTTest;
    ASSERT_TRUE(referenceTTest.TestWrongInherithance());
}
