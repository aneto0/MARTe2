/**
 * @file ObjectGTest.cpp
 * @brief Source file for class ObjectGTest
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
 * the class ObjectGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ObjectGTest,TestConstructor) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestConstructor());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestDestructor) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestDestructor());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestGetIntrospectionCopy) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestGetIntrospectionCopy());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestInitialise) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestInitialise());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestNumberOfReferences) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestNumberOfReferences());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestGetProperties) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestGetProperties());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestSetGetName) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestSetGetName("Hello"));
}

TEST(BareMetal_L2Objects_ObjectGTest,TestSetGetNameNull) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestSetGetName(NULL));
}

TEST(BareMetal_L2Objects_ObjectGTest,TestDuplicateName) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestDuplicateName());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestGetUniqueName) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestGetUniqueName("Hello", 128));
}

TEST(BareMetal_L2Objects_ObjectGTest,TestGetUniqueNameClipPointer) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestGetUniqueName("Hello", 2));
}

TEST(BareMetal_L2Objects_ObjectGTest,TestGetUniqueNameClipName) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestGetUniqueName("Hello0000000000000000000000000000000000", 20));
}

TEST(BareMetal_L2Objects_ObjectGTest,TestGetUniqueName2) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestGetUniqueName2());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestExportData) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestExportData());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestExportMetadata) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestExportMetadata());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestCallRegisteredMethod) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestCallRegisteredMethod());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestCallRegisteredMethod_InvalidMethod) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestCallRegisteredMethod_InvalidMethod());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestCallRegisteredMethod_StructuredDataI) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestCallRegisteredMethod_StructuredDataI());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestCallRegisteredMethod_ReferenceContainer) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestCallRegisteredMethod_ReferenceContainer());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestCallRegisteredMethod_StreamI) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestCallRegisteredMethod_StreamI());
}

TEST(BareMetal_L2Objects_ObjectGTest,TestIsReferenceContainer) {
    ObjectTest objectTest;
    ASSERT_TRUE(objectTest.TestIsReferenceContainer());
}

