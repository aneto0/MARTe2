/**
 * @file ClassRegistryDatabaseGTest.cpp
 * @brief Source file for class ClassRegistryDatabaseGTest
 * @date Aug 6, 2015
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
 * the class ClassRegistryDatabaseGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>

#include "ClassRegistryDatabaseTest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/




TEST(ClassRegistryDatabaseGTest,TestInstance) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestInstance());
}

TEST(ClassRegistryDatabaseGTest,TestDeleteTrue) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestDeleteTrue());
}

TEST(ClassRegistryDatabaseGTest,TestDeleteFalse) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestDeleteFalse());
}

TEST(ClassRegistryDatabaseGTest,TestAdd) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestAdd());
}

TEST(ClassRegistryDatabaseGTest,TestAddTheSameName) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestAddTheSameName());
}


TEST(ClassRegistryDatabaseGTest,TestFind) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFind("abcdefg"));
}

TEST(ClassRegistryDatabaseGTest,TestFindNull) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFind(NULL));
}

TEST(ClassRegistryDatabaseGTest,TestFindDLL) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestFindDLL("abcdefg"));
}

TEST(ClassRegistryDatabaseGTest,TestList) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestList());
}

TEST(ClassRegistryDatabaseGTest,TestSize) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestSize(32));
}


TEST(ClassRegistryDatabaseGTest,TestElementAt) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestElementAt());
}

TEST(ClassRegistryDatabaseGTest,TestInstances) {
    ClassRegistryDatabaseTest classRegistryDatabaseTest;
    ASSERT_TRUE(classRegistryDatabaseTest.TestInstances());
}
