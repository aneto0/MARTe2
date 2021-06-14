/**
 * @file ObjectRegistryDatabaseGTest.cpp
 * @brief Source file for class ObjectRegistryDatabaseGTest
 * @date 18/02/2016
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
 * the class ObjectRegistryDatabaseGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ObjectRegistryDatabaseTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ObjectRegistryDatabaseGTest,TestInstance) {
    ObjectRegistryDatabaseTest objRegTest;
    ASSERT_TRUE(objRegTest.TestInstance());
}

TEST(BareMetal_L2Objects_ObjectRegistryDatabaseGTest,TestFind) {
    ObjectRegistryDatabaseTest objRegTest;
    ASSERT_TRUE(objRegTest.TestFind());
}

TEST(BareMetal_L2Objects_ObjectRegistryDatabaseGTest,TestFind_Relative) {
    ObjectRegistryDatabaseTest objRegTest;
    ASSERT_TRUE(objRegTest.TestFind_Relative());
}

TEST(BareMetal_L2Objects_ObjectRegistryDatabaseGTest,TestFind_Absolute) {
    ObjectRegistryDatabaseTest objRegTest;
    ASSERT_TRUE(objRegTest.TestFind_Absolute());
}

TEST(BareMetal_L2Objects_ObjectRegistryDatabaseGTest,TestFindTooManyBackSteps) {
    ObjectRegistryDatabaseTest objRegTest;
    ASSERT_TRUE(objRegTest.TestFindTooManyBackSteps());
}
TEST(BareMetal_L2Objects_ObjectRegistryDatabaseGTest,TestGetClassName) {
    ObjectRegistryDatabaseTest objRegTest;
    ASSERT_TRUE(objRegTest.TestGetClassName());
}
