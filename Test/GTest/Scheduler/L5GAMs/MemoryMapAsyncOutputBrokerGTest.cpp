/**
 * @file MemoryMapAsyncOutputBrokerGTest.cpp
 * @brief Source file for class MemoryMapAsyncOutputBrokerGTest
 * @date 13/02/2017
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
 * the class MemoryMapAsyncOutputBrokerGTest (public, protected, and private). Be aware that some
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
#include "MemoryMapAsyncOutputBrokerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(MemoryMapAsyncOutputBrokerGTest,TestConstructor) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestInit) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestInit());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestInitWithBufferParameters) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithBufferParameters());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestInitWithBufferParameters_False_0_NumberOfBuffers) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithBufferParameters_False_0_NumberOfBuffers());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestInitWithBufferParameters_False_MoreThanOneGAM) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithBufferParameters_False_MoreThanOneGAM());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestInitWithBufferParameters_False_SamplesGreaterThanOne) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestInitWithBufferParameters_False_SamplesGreaterThanOne());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestExecute_1_Buffer) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_1_Buffer());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestExecute_N_Buffers) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_N_Buffers());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestExecute_Buffer_Overrun) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestExecute_Buffer_Overrun());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestGetCPUMask) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestGetCPUMask());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestGetNumberOfBuffers) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestGetNumberOfBuffers());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestGetStackSize) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestGetStackSize());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestIsIgnoringBufferOverrun) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestIsIgnoringBufferOverrun());
}

TEST(MemoryMapAsyncOutputBrokerGTest,TestSetIgnoreBufferOverrun) {
    MemoryMapAsyncOutputBrokerTest test;
    ASSERT_TRUE(test.TestSetIgnoreBufferOverrun());
}
