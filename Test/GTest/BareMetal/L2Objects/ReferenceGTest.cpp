/**
 * @file ReferenceTest.cpp
 * @brief Source file for class ReferenceTest
 * @date 07/08/2015
 * @author Giuseppe Ferrò
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
 * the class ReferenceTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ReferenceGTest,TestDefaultConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestDefaultConstructor());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestCopyConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyConstructor());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestCopyConstructorNullPtr) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyConstructorNullPtr());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestBuildObjectConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestBuildObjectConstructor());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestBuildFakeObjectConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestBuildFakeObjectConstructor());
}


TEST(BareMetal_L2Objects_ReferenceGTest,TestCopyFromObjPtrConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyFromObjPtrConstructor());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestCopyFromObjPtrConstructorNullPtr) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyFromObjPtrConstructorNullPtr());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestDestructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestDestructor());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestInitialiseCreation) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseCreation());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestInitialiseNoCreation) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseNoCreation());
}


TEST(BareMetal_L2Objects_ReferenceGTest,TestInitialiseNoObject) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseNoObject());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestInitialiseNoClassName) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialiseNoClassName());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestRemoveReference) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestRemoveReference());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestCopyOperatorReference) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyOperatorReference());
}


TEST(BareMetal_L2Objects_ReferenceGTest,TestCopyOperatorReferenceNull) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyOperatorReferenceNull());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestCopyOperatorObject) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyOperatorObject());
}


TEST(BareMetal_L2Objects_ReferenceGTest,TestCopyOperatorObjectNull) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyOperatorObjectNull());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestIsValid) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestIsValid());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestEqualOperator) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestEqualOperator());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestDifferentOperator) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestDifferentOperator());
}


TEST(BareMetal_L2Objects_ReferenceGTest,TestNumberOfReferences) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestNumberOfReferences());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestInFunctionOnStack) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInFunctionOnStack());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestInFunctionOnHeap) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInFunctionOnHeap(32));
}


TEST(BareMetal_L2Objects_ReferenceGTest,TestRightInherithance) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestRightInherithance());
}

TEST(BareMetal_L2Objects_ReferenceGTest,TestWrongInherithance) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestWrongInherithance());
}

TEST(BareMetal_L2Objects_ReferenceGTest,HugeTest) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.HugeTest(64));
}

