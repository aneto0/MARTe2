/**
 * @file ReferenceContainerFilterObjectNameGTest.cpp
 * @brief Source file for class ReferenceContainerFilterObjectNameGTest
 * @date 19/08/2015
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
 * the class ReferenceContainerFilterObjectNameGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainerFilterObjectNameTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestDefaultConstructor) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestDefaultConstructor());
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestFullConstructorOneNode) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestFullConstructor(5, "Hello"));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestFullConstructorMoreNodes) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestFullConstructor(5, "Hello.World"));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestFullConstructorMoreNodesSearchAll) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestFullConstructor(-1, "Hello.World"));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestFullConstructorFakeNodeSearchAll) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestFullConstructor(-1, ".Hello."));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestFullConstructorInvalidNodeSearchAll) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestFullConstructor(-1, "He..llo"));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestCopyConstructor) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestCopyConstructor(20, "Hello"));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestCopyConstructorSearchAll) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestCopyConstructor(-1, "Hello"));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestTestFullPath) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ReferenceContainer previouslyFound;
    Reference element1("Object");
    element1->SetName("A");
    Reference element2("Object");
    element2->SetName("B");
    previouslyFound.Insert(element1);
    previouslyFound.Insert(element2);

    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestTest(previouslyFound, "A.B.C", true));
}
/*

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestTestWrongPath) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ReferenceContainer previouslyFound;
    Reference element1("Object");
    element1->SetName("A");
    Reference element2("Object");
    element2->SetName("B");
    previouslyFound.Insert(element1);
    previouslyFound.Insert(element2);
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestTEST(BareMetal_L2Objects_previouslyFound, "A.C", false));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestTestPathNoRoot) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ReferenceContainer previouslyFound;
    Reference element1("Object");
    element1->SetName("A");
    Reference element2("Object");
    element2->SetName("B");
    previouslyFound.Insert(element1);
    previouslyFound.Insert(element2);
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestTEST(BareMetal_L2Objects_previouslyFound, "B.C", false));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestTestNoPath) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ReferenceContainer previouslyFound;
    Reference element1("Object");
    element1->SetName("A");
    previouslyFound.Insert(element1);
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestTEST(BareMetal_L2Objects_previouslyFound, "B", true));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestTestWrongPath2) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ReferenceContainer previouslyFound;
    Reference element1("Object");
    element1->SetName("A");
    previouslyFound.Insert(element1);
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestTEST(BareMetal_L2Objects_previouslyFound, "B.C", false));
}
*/
TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestAssignOperator) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestAssignOperator(20, "Hello"));
}

TEST(BareMetal_L2Objects_ReferenceContainerFilterObjectNameGTest,TestAssignOperatorSearchAll) {
    ReferenceContainerFilterObjectNameTest referenceContainerFilterObjectNameTest;
    ASSERT_TRUE(referenceContainerFilterObjectNameTest.TestAssignOperator(-1, "Hello"));
}

