/**
 * @file ConfigurationDatabaseGTest.cpp
 * @brief Source file for class ConfigurationDatabaseGTest
 * @date Nov 19, 2015
 * @author aneto
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
 * the class ConfigurationDatabaseGTest (public, protected, and private). Be aware that some 
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
#include "ConfigurationDatabaseTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(ConfigurationDatabaseGTest,TestConstructor) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestDefaultConstructor());
}

TEST(ConfigurationDatabaseGTest,TestCreateAbsolute_ValidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateAbsolute_ValidPath());
}

TEST(ConfigurationDatabaseGTest,TestCreateAbsolute_InvalidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateAbsolute_InvalidPath());
}

TEST(ConfigurationDatabaseGTest,TestCreateRelative_ValidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateRelative_ValidPath());
}

TEST(ConfigurationDatabaseGTest,TestCreateRelative_InvalidPath) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestCreateRelative_InvalidPath());
}

TEST(ConfigurationDatabaseGTest,TestMoveToRoot) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToRoot());
}

TEST(ConfigurationDatabaseGTest,TestMoveToAncestor_ValidNumberOfGenerations) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToAncestor_ValidNumberOfGenerations());
}

TEST(ConfigurationDatabaseGTest,TestMoveToAncestor_InvalidNumberOfGenerations) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveToAncestor_InvalidNumberOfGenerations());
}

TEST(ConfigurationDatabaseGTest,TestMoveAbsolute_ValidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveAbsolute_ValidPaths());
}

TEST(ConfigurationDatabaseGTest,TestMoveAbsolute_InvalidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveAbsolute_InvalidPaths());
}

TEST(ConfigurationDatabaseGTest,TestMoveRelative_ValidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveRelative_ValidPaths());
}

TEST(ConfigurationDatabaseGTest,TestMoveRelative_InvalidPaths) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestMoveRelative_InvalidPaths());
}

TEST(ConfigurationDatabaseGTest,TestDelete) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestDelete());
}

TEST(ConfigurationDatabaseGTest,TestRead_Valid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Valid());
}

TEST(ConfigurationDatabaseGTest,TestRead_Invalid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestRead_Invalid());
}

TEST(ConfigurationDatabaseGTest,TestAddToCurrentNode) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestAddToCurrentNode());
}

TEST(ConfigurationDatabaseGTest,TestWrite_Valid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Valid());
}

TEST(ConfigurationDatabaseGTest,TestWrite_Overwrite) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Overwrite());
}

TEST(ConfigurationDatabaseGTest,TestWrite_Invalid) {
    ConfigurationDatabaseTest cdbTest;
    ASSERT_TRUE(cdbTest.TestWrite_Invalid());
}
