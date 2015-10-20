/**
 * @file CharBufferGTest.cpp
 * @brief Source file for class CharBufferGTest
 * @date 08/10/2015
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
 * the class CharBufferGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "CharBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(CharBufferGTest, TestDefaultConstructor) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestDefaultConstructor());
}

TEST(CharBufferGTest, TestGranularityConstructor_1) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(1, 1));
}

TEST(CharBufferGTest, TestGranularityConstructor_2) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(2, 2));
}

TEST(CharBufferGTest, TestGranularityConstructor_3) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(3, 2));
}

TEST(CharBufferGTest, TestGranularityConstructor_5) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(5, 4));
}

TEST(CharBufferGTest, TestGranularityConstructor_10) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(10, 8));
}

TEST(CharBufferGTest, TestGranularityConstructor_16) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(16, 16));
}

TEST(CharBufferGTest, TestGranularityConstructor_20) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(20, 16));
}

TEST(CharBufferGTest, TestGranularityConstructor_33) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(33, 32));
}

TEST(CharBufferGTest, TestGranularityConstructor_127) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(127, 64));
}

TEST(CharBufferGTest, TestGranularityConstructor_0) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(0, 1));
}

TEST(CharBufferGTest, TestGranularityConstructor_MAX) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(0xffffffff, 0x80000000));
}

TEST(CharBufferGTest, TestDestructor) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestDestructor());
}

TEST(CharBufferGTest, TestSetBufferSize_size_in_granularity) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferSize(10, 32, true));
}

TEST(CharBufferGTest, TestSetBufferSize_granularity_in_size) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferSize(32, 8, true));
}

TEST(CharBufferGTest, TestSetBufferSize_zero_size) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferSize(0, 8, false));
}

TEST(CharBufferGTest, TestSetBufferSize_out_of_boundary) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferSize(0xfffffff1, 16, false));
}

TEST(CharBufferGTest, TestSetBufferReferenceConst_SameSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceConst("HelloWorld", 10));
}

TEST(CharBufferGTest, TestSetBufferReferenceConst_GreaterSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceConst("HelloWorld", 12));
}

TEST(CharBufferGTest, TestSetBufferReferenceConst_MinorSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceConst("HelloWorld", 8));
}

TEST(CharBufferGTest, TestSetBufferReferenceNonConst_SameSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceNonConst("HelloWorld", 10));
}

TEST(CharBufferGTest, TestSetBufferReferenceNonConst_GreaterSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceNonConst("HelloWorld", 12));
}

TEST(CharBufferGTest, TestSetBufferReferenceNonConst_MinorSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceNonConst("HelloWorld", 8));
}

TEST(CharBufferGTest, TestBuffer) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestBuffer());
}

TEST(CharBufferGTest, TestBufferReference) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestBufferReference());
}

TEST(CharBufferGTest, TestSize_StaticAllocation_0) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_StaticAllocation(0));
}

TEST(CharBufferGTest, TestSize_StaticAllocation_32) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_StaticAllocation(32));
}

TEST(CharBufferGTest, TestSize_DynamicAllocation_zero_size) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_DynamicAllocation(0, 0));
}

TEST(CharBufferGTest, TestSize_DynamicAllocation_size_in_granularity) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_DynamicAllocation(10, 32));
}

TEST(CharBufferGTest, TestSize_DynamicAllocation_granularity_in_size) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_DynamicAllocation(23, 4));
}

TEST(CharBufferGTest, TestIsAllocated) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestIsAllocated());
}

TEST(CharBufferGTest, TestAllocationGranularity_129) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(129, 128));
}

TEST(CharBufferGTest, TestAllocationGranularity_255) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(255, 128));
}

TEST(CharBufferGTest, TestAllocationGranularity_530) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(530, 512));
}

TEST(CharBufferGTest, TestAllocationGranularity_3500) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(3500, 2048));
}

TEST(CharBufferGTest, TestAllocationGranularity_MAX_GRAN) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(0x80000000, 0x80000000));
}

TEST(CharBufferGTest, TestAllocationGranularity_20) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(20, 16));
}

TEST(CharBufferGTest, TestAllocationGranularity_33) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(33, 32));
}

TEST(CharBufferGTest, TestAllocationGranularity_127) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(127, 64));
}
TEST(CharBufferGTest, TestCanWrite) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestCanWrite());
}

