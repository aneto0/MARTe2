/**
 * @file ClassRegistryItemGTest.cpp
 * @brief Source file for class ClassRegistryItemGTest
 * @date 11/08/2015
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
 * the class ClassRegistryItemGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryItemTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestConstructor) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestConstructor());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestIntrospectionConstructor) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestIntrospectionConstructor());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestFullConstructor) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestFullConstructor());
}

//TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestDestructor) {
//    ClassRegistryItemTest myClassRegistryItemTest;
//    ASSERT_TRUE(myClassRegistryItemTest.TestDestructor());
//}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestAddMethod) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestAddMethod());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestFindMethod) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestFindMethod());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestIncrementNumberOfInstances) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestIncrementNumberOfInstances());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestDecrementNumberOfInstances) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestDecrementNumberOfInstances());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestGetNumberOfInstances) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetNumberOfInstances(32));
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestGetClassPropertiesCopy) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetClassPropertiesCopy());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestGetClassProperties) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetClassProperties());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestSetGetLoadableLibrary) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestSetGetLoadableLibrary("Hello"));
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestGetObjectBuildFunction) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetObjectBuildFunction());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestGetIntrospection) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetIntrospection());
}

TEST(BareMetal_L2Objects_ClassRegistryItemGTest,TestSetUniqueIdentifier) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestSetUniqueIdentifier(0xfffff1));
}
