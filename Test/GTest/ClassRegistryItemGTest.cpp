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

#include <limits.h>
#include "gtest/gtest.h"

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

TEST(ClassRegistryItemGTest,TestConstructor) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestConstructor());
}

TEST(ClassRegistryItemGTest,TestDestructor) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestDestructor());
}

TEST(ClassRegistryItemGTest,TestIncrementNumberOfInstances) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestIncrementNumberOfInstances());
}

TEST(ClassRegistryItemGTest,TestDecrementNumberOfInstances) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestDecrementNumberOfInstances());
}

TEST(ClassRegistryItemGTest,TestGetNumberOfInstances) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetNumberOfInstances(32));
}

TEST(ClassRegistryItemGTest,TestGetClassPropertiesCopy) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetClassPropertiesCopy("Hello","World"));
}

TEST(ClassRegistryItemGTest,TestGetClassPropertiesCopyNull) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetClassPropertiesCopy(NULL,NULL));
}

TEST(ClassRegistryItemGTest,TestGetClassProperties) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetClassProperties("Hello","World"));
}

TEST(ClassRegistryItemGTest,TestGetClassPropertiesNull) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetClassProperties(NULL,"World"));
}


TEST(ClassRegistryItemGTest,TestSetGetLoadableLibrary) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestSetGetLoadableLibrary("Hello"));
}

TEST(ClassRegistryItemGTest,TestGetObjectBuildFunction) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestGetObjectBuildFunction());
}

TEST(ClassRegistryItemGTest,TestSetUniqueIdentifier) {
    ClassRegistryItemTest myClassRegistryItemTest;
    ASSERT_TRUE(myClassRegistryItemTest.TestSetUniqueIdentifier(0xfffff1));
}
