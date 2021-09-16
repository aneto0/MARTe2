/**
 * @file ObjectRegistryDatabaseMessageFilterGTest.cpp
 * @brief Source file for class ObjectRegistryDatabaseMessageFilterGTest
 * @date 19/06/2018
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
 * the class ObjectRegistryDatabaseMessageFilterGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "ObjectRegistryDatabaseMessageFilterTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestDefaultConstructor) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestDefaultConstructor());
}

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestConsumeMessage_Purge) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_Purge());
}

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestConsumeMessage_PurgeAll) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_PurgeAll());
}

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestConsumeMessage_Load) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_Load());
}

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestConsumeMessage_LoadSubTree) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_LoadSubTree());
}

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestConsumeMessage_False_InvalidNode) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_False_InvalidNode());
}

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestConsumeMessage_False_InvalidFunction) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_False_InvalidFunction());
}

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestConsumeMessage_Load_False_EmptyConfiguration) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_Load_False_EmptyConfiguration());
}

TEST(BareMetal_L4Messages_ObjectRegistryDatabaseMessageFilterGTest,TestConsumeMessage_Load_False_Not_ReferenceContainer) {
    ObjectRegistryDatabaseMessageFilterTest target;
    ASSERT_TRUE(target.TestConsumeMessage_Load_False_Not_ReferenceContainer());
}

