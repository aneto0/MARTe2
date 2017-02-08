/**
 * @file MemoryMapTriggerOutputBrokerGTest.cpp
 * @brief Source file for class MemoryMapTriggerOutputBrokerGTest
 * @date 24/01/2017
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
 * the class MemoryMapTriggerOutputBrokerGTest (public, protected, and private). Be aware that some 
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
#include "MemoryMapTriggerOutputBrokerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MemoryMapTriggerOutputBrokerGTest,TestConstructor) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestInit) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInit());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_1_PreTriggerBuffers_0_PostTriggerBuffers) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_1_PreTriggerBuffers_0_PostTriggerBuffers());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_0_PreTriggerBuffers_1_PostTriggerBuffers) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0_PreTriggerBuffers_1_PostTriggerBuffers());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_1_PreTriggerBuffers_1_PostTriggerBuffers) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_1_PreTriggerBuffers_1_PostTriggerBuffers());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers());
}
	
TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers_AlwaysTriggering) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers_AlwaysTriggering());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_AlwaysTriggering) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_AlwaysTriggering());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_0_PostTriggerBuffers_OverwritingPreTriggerBuffers) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_0_PostTriggerBuffers_OverwritingPreTriggerBuffers());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_0_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPostTriggerBuffers) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPostTriggerBuffers());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPreAndPostTriggerBuffers) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPreAndPostTriggerBuffers());
}

TEST(MemoryMapTriggerOutputBrokerGTest,TestExecute_1_Buffer_AlwaysTriggering) {
    MemoryMapTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_1_Buffer_AlwaysTriggering());
}
