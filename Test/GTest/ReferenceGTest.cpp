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

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceTest.h"
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(ReferenceGTest,TestDefaultConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestDefaultConstructor());
}

TEST(ReferenceGTest,TestCopyConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyConstructor());
}

TEST(ReferenceGTest,TestCopyConstructorNullPtr) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyConstructorNullPtr());
}

TEST(ReferenceGTest,TestBuildObjectConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestBuildObjectConstructor());
}

TEST(ReferenceGTest,TestBuildFakeObjectConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestBuildFakeObjectConstructor());
}


TEST(ReferenceGTest,TestCopyFromObjPtrConstructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyFromObjPtrConstructor());
}

TEST(ReferenceGTest,TestCopyFromObjPtrConstructorNullPtr) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyFromObjPtrConstructorNullPtr());
}

TEST(ReferenceGTest,TestDestructor) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestDestructor());
}

TEST(ReferenceGTest,TestInitialise) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInitialise());
}

TEST(ReferenceGTest,TestRemoveReference) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestRemoveReference());
}

TEST(ReferenceGTest,TestCopyOperatorReference) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyOperatorReference());
}


TEST(ReferenceGTest,TestCopyOperatorReferenceNull) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyOperatorReferenceNull());
}

TEST(ReferenceGTest,TestCopyOperatorObject) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyOperatorObject());
}


TEST(ReferenceGTest,TestCopyOperatorObjectNull) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestCopyOperatorObjectNull());
}

TEST(ReferenceGTest,TestIsValid) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestIsValid());
}

TEST(ReferenceGTest,TestEqualOperator) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestEqualOperator());
}

TEST(ReferenceGTest,TestDifferentOperator) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestDifferentOperator());
}


TEST(ReferenceGTest,TestNumberOfReferences) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestNumberOfReferences());
}

TEST(ReferenceGTest,TestInFunctionOnStack) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInFunctionOnStack());
}

TEST(ReferenceGTest,TestInFunctionOnHeap) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestInFunctionOnHeap(32));
}


TEST(ReferenceGTest,TestRightInherithance) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestRightInherithance());
}

TEST(ReferenceGTest,TestWrongInherithance) {
    ReferenceTest referenceTest;
    ASSERT_TRUE(referenceTest.TestWrongInherithance());
}

