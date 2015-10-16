/**
 * @file IOBufferGTest.cpp
 * @brief Source file for class IOBufferGTest
 * @date 09/10/2015
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
 * the class IOBufferGTest (public, protected, and private). Be aware that some 
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
#include "IOBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(IOBufferGTest, TestDefaultConstructor) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestDefaultConstructor());
}

TEST(IOBufferGTest, TestFullConstructor_0) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestFullConstructor(0));
}

TEST(IOBufferGTest, TestFullConstructor_100) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestFullConstructor(100));
}

TEST(IOBufferGTest, TestFullConstructor_MAX) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestFullConstructor(0xffffffff));
}

TEST(IOBufferGTest, TestDestructor) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestDestructor());
}

TEST(IOBufferGTest, TestSize_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBufferSize(10, 32));
}

TEST(IOBufferGTest, TestSize_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBufferSize(10, 16));
}

TEST(IOBufferGTest, TestSize_zero_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBufferSize(0, 16));
}

TEST(IOBufferGTest, TestMaxUsableAmount_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestMaxUsableAmount(8, 2, 32));
}

TEST(IOBufferGTest, TestMaxUsableAmount_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestMaxUsableAmount(32, 2, 8));
}

TEST(IOBufferGTest, TestMaxUsableAmount_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestMaxUsableAmount(10, 12, 8));
}

TEST(IOBufferGTest, TestMaxUsableAmount_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestMaxUsableAmount(8, 12, 8));
}

TEST(IOBufferGTest, TestAmountLeft_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAmountLeft(8, 2, 32));
}

TEST(IOBufferGTest, TestAmountLeft_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAmountLeft(32, 2, 8));
}

TEST(IOBufferGTest, TestAmountLeft_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAmountLeft(10, 12, 8));
}

TEST(IOBufferGTest, TestAmountLeft_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAmountLeft(8, 12, 8));
}

TEST(IOBufferGTest, TestUsedAmountLeft_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(8, 2, 32));
}

TEST(IOBufferGTest, TestUsedAmountLeft_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(32, 2, 8));
}

TEST(IOBufferGTest, TestUsedAmountLeft_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(10, 12, 8));
}

TEST(IOBufferGTest, TestUsedAmountLeft_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(8, 12, 8));
}

TEST(IOBufferGTest, TestBuffer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBuffer());
}

TEST(IOBufferGTest, TestBufferReference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBufferReference());
}

TEST(IOBufferGTest, TestPosition) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPosition());
}

TEST(IOBufferGTest, TestUsedSize_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedSize(8, 2, 32));
}

TEST(IOBufferGTest, TestUsedSize_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedSize(32, 2, 8));
}

TEST(IOBufferGTest, TestUsedSize_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedSize(10, 12, 8));
}

TEST(IOBufferGTest, TestUsedSize_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(8, 12, 8));
}

TEST(IOBufferGTest, TestAllocationGranularity_0) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAllocationGranularity(0, 1));
}

TEST(IOBufferGTest, TestAllocationGranularity_127) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAllocationGranularity(127, 64));
}

TEST(IOBufferGTest, TestAllocationGranularity_MAX) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAllocationGranularity(0xffffffff, 0x80000000));
}

TEST(IOBufferGTest, TestUndoLevel_0) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUndoLevel(0));
}

TEST(IOBufferGTest, TestUndoLevel_100) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUndoLevel(100));
}

TEST(IOBufferGTest, TestUndoLevel_MAX) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUndoLevel(0xffffffff));
}

TEST(IOBufferGTest, TestCanWrite) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestCanWrite());
}


TEST(IOBufferGTest, TestSetBufferHeapMemory_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(8, 2, 32));
}

TEST(IOBufferGTest, TestSetBufferHeapMemory_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(32, 2, 8));
}

TEST(IOBufferGTest, TestSetBufferHeapMemory_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(10, 12, 8));
}

TEST(IOBufferGTest, TestSetBufferHeapMemory_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(8, 12, 8));
}

TEST(IOBufferGTest, TestSetBufferHeapMemory_zero_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(0, 12, 8));
}

TEST(IOBufferGTest, TestSetBufferHeapMemoryIncrease) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemoryIncrease());
}

TEST(IOBufferGTest, TestSetBufferHeapMemoryDecrease) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemoryDecrease());
}
TEST(IOBufferGTest, TestSetBufferReadOnlyReferencedMemory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferReadOnlyReferencedMemory());
}
TEST(IOBufferGTest, TestSetBufferReferencedMemory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferReferencedMemory());
}

TEST(IOBufferGTest, TestResync) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestResync());
}

TEST(IOBufferGTest, TestSeek_in_range) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSeek(32, 16, true));
}

TEST(IOBufferGTest, TestSeek_out_range) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSeek(32, 33, false));
}

TEST(IOBufferGTest, TestRelativeSeek_in_range) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRelativeSeek(16, 8, true));
}

TEST(IOBufferGTest, TestRelativeSeek_left_boundary) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRelativeSeek(16, -17, false));
}

TEST(IOBufferGTest, TestRelativeSeek_right_boundary) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRelativeSeek(16, 17, false));
}

TEST(IOBufferGTest, TestPutC_Heap) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPutC_Heap());
}

TEST(IOBufferGTest, TestPutC_MemoryReference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPutC_MemoryReference());
}

TEST(IOBufferGTest, TestPutC_MemoryReferenceRO) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPutC_MemoryReferenceRO());
}

TEST(IOBufferGTest, TestGetC_Heap) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetC_Heap());
}

TEST(IOBufferGTest, TestGetC_MemoryReference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetC_MemoryReference());
}

TEST(IOBufferGTest, TestGetC_MemoryReferenceRO) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetC_MemoryReferenceRO());
}

TEST(IOBufferGTest, TestUnPutC) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUnPutC());
}

TEST(IOBufferGTest, TestUnGetC) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUnGetC());
}

TEST(IOBufferGTest, TestSetUsedSize) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetUsedSize(32, 31));
}

TEST(IOBufferGTest, TestSetUsedSize_clipped) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetUsedSize(32, 33));
}
TEST(IOBufferGTest, TestWrite_Heap_size_in_allocated) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_Heap(32, 10, "HelloWorld"));
}

TEST(IOBufferGTest, TestWrite_Heap_allocated_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_Heap(8, 32, "HelloWorld"));
}

TEST(IOBufferGTest, TestWrite_Memoryreference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_Memoryreference());
}

TEST(IOBufferGTest, TestWrite_NULL_Buffer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_NULL_Buffer());
}

TEST(IOBufferGTest, TestRead_Heap_allocated_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRead_Heap(8, 32));
}

TEST(IOBufferGTest, TestRead_Heap_size_in_allocated) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRead_Heap(32, 10));
}

TEST(IOBufferGTest, TestRead_MemoryReference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRead_MemoryReference());
}

TEST(IOBufferGTest, TestRead_NULL_Buffer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRead_NULL_Buffer());
}

TEST(IOBufferGTest, TestWriteAll_size_in_allocated) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWriteAll(10, 5, "HelloWorld"));
}

TEST(IOBufferGTest, TestWriteAll_allocated_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWriteAll(5, 10, "HelloWorld"));
}

TEST(IOBufferGTest, TestEmpty) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestEmpty());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Integer_Decimal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Integer_Decimal());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Integer_Hexadecimal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Integer_Hexadecimal());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Integer_Octal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Integer_Octal());
}
TEST(IOBufferGTest, TestPrintFormattedToStream_Integer_Binary) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Integer_Binary());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Float_FixedPoint) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Float_FixedPoint());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Float_FixedRelativePoint) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Float_FixedRelativePoint());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Float_Exponential) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Float_Exponential());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Float_Engeneering) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Float_Engeneering());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Float_Smart) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Float_Smart());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Float_Compact) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Float_Compact());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_CCString) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_CCString());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Pointer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Pointer());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_Stream) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_Stream());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_BitSet_Unsigned) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_BitSet_Unsigned());
}

TEST(IOBufferGTest, TestPrintFormattedToStream_BitSet_Signed) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedToStream_BitSet_Signed());
}

TEST(IOBufferGTest, TestPrintFormattedMultiple) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedMultiple());
}

TEST(IOBufferGTest, TestGetTokenFromStream_ConstCharOutput) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetTokenFromStream_ConstCharOutput());
}

TEST(IOBufferGTest, TestGetTokenFromStream_IOBufferOutput) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetTokenFromStream_IOBufferOutput());
}

TEST(IOBufferGTest, TestSkipTokenInStream) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSkipTokenInStream());
}

TEST(IOBufferGTest, TestSkipTokenInStream_NULL_Terminator) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSkipTokenInStream_NULL_Terminator());
}





