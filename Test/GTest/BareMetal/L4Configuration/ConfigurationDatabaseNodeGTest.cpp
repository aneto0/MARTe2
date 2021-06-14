/**
 * @file ConfigurationDatabaseNodeGTest.cpp
 * @brief Source file for class ConfigurationDatabaseNodeGTest
 * @date 09/01/2020
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
 * the class ConfigurationDatabaseNodeGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabaseNodeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestConstructor) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestDefaultConstructor());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestPurge) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestPurge());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestPurge_ReferenceContainer) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestPurge_ReferenceContainer());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestInsert) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestInsert());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestSize) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestSize());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestGet) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGet());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestFind) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestFind());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestFindLeaf) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestFindLeaf());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestDelete) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestDelete());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestGetParent) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestGetParent());
}

TEST(BareMetal_L4Configuration_ConfigurationDatabaseNodeGTest,TestSetParent) {
    ConfigurationDatabaseNodeTest cdbTest;
    ASSERT_TRUE(cdbTest.TestSetParent());
}
