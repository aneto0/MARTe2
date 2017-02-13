/**
 * @file MemoryMapAsyncTriggerOutputBrokerGTest.cpp
 * @brief Source file for class MemoryMapAsyncTriggerOutputBrokerGTest
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
 * the class MemoryMapAsyncTriggerOutputBrokerGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include <MemoryMapAsyncTriggerOutputBrokerTest.h>
#include "gtest/gtest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestConstructor) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInit) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInit());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInitWithTriggerParameters) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithTriggerParameters());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInitWithTriggerParameters_False_0_NumberOfBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithTriggerParameters_False_0_NumberOfBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInitWithTriggerParameters_False_MoreThanOneGAM) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithTriggerParameters_False_MoreThanOneGAM());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInitWithTriggerParameters_False_PostTriggerGreaterThanNumberOfBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithTriggerParameters_False_PostTriggerGreaterThanNumberOfBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInitWithTriggerParameters_False_PrePlusPostTriggerGreaterThanNumberOfBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithTriggerParameters_False_PrePlusPostTriggerGreaterThanNumberOfBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInitWithTriggerParameters_False_PreTriggerGreaterThanNumberOfBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithTriggerParameters_False_PreTriggerGreaterThanNumberOfBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInitWithTriggerParameters_False_SamplesGreaterThanOne) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithTriggerParameters_False_SamplesGreaterThanOne());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestInitWithTriggerParameters_False_WrongTriggerSignalType) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithTriggerParameters_False_WrongTriggerSignalType());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_ManyCycles) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_ManyCycles());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_1_PreTriggerBuffers_0_PostTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_1_PreTriggerBuffers_0_PostTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_0_PreTriggerBuffers_1_PostTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0_PreTriggerBuffers_1_PostTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_1_PreTriggerBuffers_1_PostTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_1_PreTriggerBuffers_1_PostTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers());
}
	
TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers_AlwaysTriggering) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0_PreTriggerBuffers_0_PostTriggerBuffers_AlwaysTriggering());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_AlwaysTriggering) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_AlwaysTriggering());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_0_PostTriggerBuffers_OverwritingPreTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_0_PostTriggerBuffers_OverwritingPreTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_0_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPostTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_0_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPostTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPreAndPostTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_PreTriggerBuffers_N_PostTriggerBuffers_OverwritingPreAndPostTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_1_Buffer_AlwaysTriggering) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_1_Buffer_AlwaysTriggering());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestExecute_Buffer_Overrun) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_Buffer_Overrun());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestGetCPUMask) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestGetNumberOfBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestGetNumberOfBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestGetPreTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestGetPreTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestGetPostTriggerBuffers) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestGetPostTriggerBuffers());
}

TEST(MemoryMapAsyncTriggerOutputBrokerGTest,TestGetStackSize) {
    MemoryMapAsyncTriggerOutputBrokerTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}
