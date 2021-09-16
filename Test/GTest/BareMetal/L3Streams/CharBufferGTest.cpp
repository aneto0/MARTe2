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


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "CharBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L3Streams_CharBufferGTest, TestDefaultConstructor) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestDefaultConstructor());
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_1) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(1, 1));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_2) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(2, 2));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_3) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(3, 2));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_5) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(5, 4));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_10) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(10, 8));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_16) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(16, 16));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_20) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(20, 16));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_33) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(33, 32));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_127) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(127, 64));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_0) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(0, 1));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestGranularityConstructor_MAX) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestGranularityConstructor(0xffffffff, 0x80000000));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestDestructor) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestDestructor());
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferSize_size_in_granularity) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferSize(10, 32, true));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferSize_granularity_in_size) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferSize(32, 8, true));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferSize_zero_size) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferSize(0, 8, false));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferSize_out_of_boundary) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferSize(0xfffffff1, 16, false));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferReferenceConst_SameSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceConst("HelloWorld", 10));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferReferenceConst_GreaterSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceConst("HelloWorld", 12));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferReferenceConst_MinorSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceConst("HelloWorld", 8));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferReferenceNonConst_SameSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceNonConst("HelloWorld", 10));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferReferenceNonConst_GreaterSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceNonConst("HelloWorld", 12));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSetBufferReferenceNonConst_MinorSize) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSetBufferReferenceNonConst("HelloWorld", 8));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestBuffer) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestBuffer());
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestBufferReference) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestBufferReference());
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSize_StaticAllocation_0) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_StaticAllocation(0));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSize_StaticAllocation_32) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_StaticAllocation(32));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSize_DynamicAllocation_zero_size) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_DynamicAllocation(0, 0));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSize_DynamicAllocation_size_in_granularity) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_DynamicAllocation(10, 32));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestSize_DynamicAllocation_granularity_in_size) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestSize_DynamicAllocation(23, 4));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestIsAllocated) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestIsAllocated());
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestAllocationGranularity_129) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(129, 128));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestAllocationGranularity_255) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(255, 128));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestAllocationGranularity_530) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(530, 512));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestAllocationGranularity_3500) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(3500, 2048));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestAllocationGranularity_MAX_GRAN) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(0x80000000, 0x80000000));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestAllocationGranularity_20) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(20, 16));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestAllocationGranularity_33) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(33, 32));
}

TEST(BareMetal_L3Streams_CharBufferGTest, TestAllocationGranularity_127) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestAllocationGranularity(127, 64));
}
TEST(BareMetal_L3Streams_CharBufferGTest, TestCanWrite) {
    CharBufferTest myCharBufferTest;
    ASSERT_TRUE(myCharBufferTest.TestCanWrite());
}

