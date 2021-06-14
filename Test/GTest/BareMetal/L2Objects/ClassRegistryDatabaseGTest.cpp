/**
 * @file ClassRegistryDatabaseGTest.cpp
 * @brief Source file for class ClassRegistryDatabaseGTest
 * @date 06/08/2015
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
 * the class ClassRegistryDatabaseGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryDatabaseTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestInstance) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestInstance());
}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestFind) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFind());
}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestFindByTypeIdName) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFindTypeIdName());
}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestFindDLLOnlyClassNameTrue) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFindDLL("boh","IntegerObject",true));
}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestFindDLLOnlyClassNameFalse) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFindDLL("boh","boh",false));
}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestGetSize) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestGetSize());
}

//TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestPeek) {
//    ClassRegistryDatabaseTest classRegistryDatabaseTest;
//    ASSERT_TRUE(classRegistryDatabaseTest.TestPeek());
//}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestCreateInstances) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestCreateInstances());
}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestPolimorphismChild2Father) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestPolimorphismChild2Father());
}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestPolimorphismFather2Child) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestPolimorphismFather2Child());
}

TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestGetClassName) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestGetClassName());
}


TEST(BareMetal_L2Objects_ClassRegistryDatabaseGTest,TestCleanUp) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestCleanUp());
}
