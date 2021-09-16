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


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "IOBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L3Streams_IOBufferGTest, TestDefaultConstructor) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestDefaultConstructor());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestFullConstructor_0) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestFullConstructor(0));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestFullConstructor_100) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestFullConstructor(100));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestFullConstructor_MAX) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestFullConstructor(0xffffffff));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestDestructor) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestDestructor());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSize_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBufferSize(10, 32));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSize_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBufferSize(10, 16));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSize_zero_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBufferSize(0, 16));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestMaxUsableAmount_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestMaxUsableAmount(8, 2, 32));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestMaxUsableAmount_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestMaxUsableAmount(32, 2, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestMaxUsableAmount_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestMaxUsableAmount(10, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestMaxUsableAmount_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestMaxUsableAmount(8, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestAmountLeft_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAmountLeft(8, 2, 32));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestAmountLeft_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAmountLeft(32, 2, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestAmountLeft_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAmountLeft(10, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestAmountLeft_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAmountLeft(8, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUsedAmountLeft_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(8, 2, 32));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUsedAmountLeft_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(32, 2, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUsedAmountLeft_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(10, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUsedAmountLeft_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(8, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestBuffer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBuffer());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestBufferReference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestBufferReference());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPosition) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPosition());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUsedSize_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedSize(8, 2, 32));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUsedSize_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedSize(32, 2, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUsedSize_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedSize(10, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUsedSize_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUsedAmountLeft(8, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestAllocationGranularity_0) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAllocationGranularity(0, 1));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestAllocationGranularity_127) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAllocationGranularity(127, 64));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestAllocationGranularity_MAX) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestAllocationGranularity(0xffffffff, 0x80000000));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUndoLevel_0) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUndoLevel(0));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUndoLevel_100) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUndoLevel(100));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUndoLevel_MAX) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUndoLevel(0xffffffff));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestCanWrite) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestCanWrite());
}


TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferHeapMemory_size_in_granularity) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(8, 2, 32));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferHeapMemory_granularity_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(32, 2, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferHeapMemory_bigEndSpace_in_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(10, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferHeapMemory_bigEndSpace_out_memory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(8, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferHeapMemory_zero_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemory(0, 12, 8));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferHeapMemoryIncrease) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemoryIncrease());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferHeapMemoryDecrease) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferHeapMemoryDecrease());
}
TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferReadOnlyReferencedMemory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferReadOnlyReferencedMemory());
}
TEST(BareMetal_L3Streams_IOBufferGTest, TestSetBufferReferencedMemory) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetBufferReferencedMemory());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestResync) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestResync());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSeek_in_range) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSeek(32, 16, true));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSeek_out_range) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSeek(32, 33, false));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestRelativeSeek_in_range) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRelativeSeek(16, 8, true));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestRelativeSeek_left_boundary) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRelativeSeek(16, -17, false));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestRelativeSeek_right_boundary) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRelativeSeek(16, 17, false));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPutC_Heap) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPutC_Heap());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPutC_MemoryReference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPutC_MemoryReference());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPutC_MemoryReferenceRO) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPutC_MemoryReferenceRO());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestGetC_Heap) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetC_Heap());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestGetC_MemoryReference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetC_MemoryReference());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestGetC_MemoryReferenceRO) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetC_MemoryReferenceRO());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUnPutC) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUnPutC());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestUnGetC) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestUnGetC());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSetUsedSize) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetUsedSize(32, 31));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSetUsedSize_clipped) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSetUsedSize(32, 33));
}
TEST(BareMetal_L3Streams_IOBufferGTest, TestWrite_Heap_size_in_allocated) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_Heap(32, 10, "HelloWorld"));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestWrite_Heap_allocated_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_Heap(8, 32, "HelloWorld"));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestWrite_Memoryreference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_Memoryreference("HelloWorld",10));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestWrite_Memoryreference_Clip_Size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_Memoryreference("HelloWorld",5));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestWrite_NULL_Buffer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_NULL_Buffer());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestRead_Heap_allocated_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRead_Heap(8, 32));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestRead_Heap_size_in_allocated) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRead_Heap(32, 10));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestRead_MemoryReference) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRead_MemoryReference());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestRead_NULL_Buffer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRead_NULL_Buffer());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestWriteAll_size_in_allocated) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWriteAll(10, 5, "HelloWorld"));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestWriteAll_allocated_in_size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWriteAll(5, 10, "HelloWorld"));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestEmpty) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestEmpty());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Integer_Decimal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedDecimalTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Integer_Hexadecimal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedHexadecimalTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Integer_Octal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedOctalTable()));
}
TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Integer_Binary) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(128, GeneratePrintFormattedBinaryTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Float_FixedPoint) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatFPTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Float_FixedRelativePoint) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatFPRTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Float_Exponential) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatExpTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Float_Engineering) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatEngTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Float_Smart) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatSmartTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Float_Compact) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatCompactTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_CCString) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_CCString());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Pointer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_Pointer());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Stream) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_Stream());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_TooBigStream) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_TooBigStream());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_Stream_NotSeekable) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_Stream_NotSeekable());
}


TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_BitSet_Unsigned) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_BitSet_Unsigned());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormatted_BitSet_Signed) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_BitSet_Signed());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormattedMultiple) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedGenericTable()));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestGetToken_ConstCharOutput_TableTerminators) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetToken_ConstCharOutput(&TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestGetToken_ConstCharOutput_TableSkip) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetToken_ConstCharOutput(&TokenTestTableSkipCharacters[0]));
}


TEST(BareMetal_L3Streams_IOBufferGTest, GetToken_ConstCharOutput_ClipSize) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.GetToken_ConstCharOutput_ClipSize());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestGetToken_IOBufferOutput_TableTerminators) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetToken_IOBufferOutput(&TokenTestTableTerminators[0]));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestGetToken_IOBufferOutput_TableSkip) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetToken_IOBufferOutput(&TokenTestTableSkipCharacters[0]));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestSkipToken) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSkipToken(&SkipTokensTestTable[0]));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormattedObject) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedObject());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintStructuredDataInterface) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintStructuredDataInterface());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormattedIntrospection) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedIntrospection());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintVector_Int32) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableVector<int32, 3> table[]={
            {"%d" ,{1, 2 , 3}, "{ 1 2 3 } "},
            {"%-#5x" ,{1, 2 , 3}, "{ 0x1   0x2   0x3   } "},
            {0, {0}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintVector(table));
}


TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintVector_Float32) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableVector<float32, 3> table[]={
            {"%f" ,{1.5, 2.5 , 3.25}, "{ 1.500000 2.500000 3.250000 } "},
            {"%3f" ,{1.5, 2.5 , 3.25}, "{ 1.5 2.5 3.2 } "},
            {0, {0}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintVector(table));
}


TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintVector_CString) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableVector<const char8*, 3> table[]={
            {"%s" ,{"Hello", "Hola", "Ciao"}, "{ Hello Hola Ciao } "},
            {"%-6s" ,{"laaaaaaaarge", "short", "miiiddle"}, "{ laaaaa short  miiidd } "},
            {0, {0}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintVector(table));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintVector_CArray) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableVector<const char8*, 3> table[]={
            {"%s" ,{"Hello", "Hola", "Ciao"}, "{ Hello Hola Ciao } "},
            {"%-6s" ,{"laaaaaaaarge", "short", "miiiddle"}, "{ laaaaa short  miiidd } "},
            {0, {0}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintCArrayVector(table));
}


TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintMatrix_Int32) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableMatrix<int32, 2, 3 > table[]={
            {"%d" ,{{1, 2 , 3},{4, 5, 6}}, "{ { 1 2 3 } { 4 5 6 } } "},
            {"%-#5x" ,{{1, 2 , 3},{4, 5, 6}}, "{ { 0x1   0x2   0x3   } { 0x4   0x5   0x6   } } "},
            {0, {{0}}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintMatrix(table));
}


TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintMatrix_Float32) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableMatrix<float32, 2, 3> table[]={
            {"%f" ,{{1.5, 2.5 , 3.25}, {0.1, 0.2 , 0.5}}, "{ { 1.500000 2.500000 3.250000 } { 0.100000 0.200000 0.500000 } } "},
            {"%3f" ,{{1.5, 2.5 , 3.25}, {0.1, 0.2 , 0.5}}, "{ { 1.5 2.5 3.2 } { 0.1 0.2 0.5 } } "},
            {0, {{0}}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintMatrix(table));
}


TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintMatrix_CString) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableMatrix<const char8*, 2,3> table[]={
            {"%s" ,{{"Hello", "Hola", "Ciao"}, {"Bye", "Adieu", "Ciao"}}, "{ { Hello Hola Ciao } { Bye Adieu Ciao } } "},
            {0, {{0}}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintMatrix(table));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintMatrix_CArray) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableMatrix<const char8*, 2,3> table[]={
            {"%s" ,{{"Hello", "Hola", "Ciao"}, {"Bye", "Adieu", "Ciao"}}, "{ { Hello Hola Ciao } { Bye Adieu Ciao } } "},
            {0, {{0}}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintCArrayMatrix(table));
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintTooMuchDimensions) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintTooMuchDimensions());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormattedIntrospection_NotIntrospectable) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedIntrospection_NotIntrospectable());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintFormattedObject_NotIntrospectable) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedObject_NotIntrospectable());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintPointerVector) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintPointerVector());
}


TEST(BareMetal_L3Streams_IOBufferGTest, TestPrintPointerMatrix) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintPointerMatrix());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestRefill) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestRefill());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestFlush) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestFlush());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestNoMoreSpaceToWriteArg) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestNoMoreSpaceToWriteArg());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestNoMoreSpaceToWrite) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestNoMoreSpaceToWrite());
}

TEST(BareMetal_L3Streams_IOBufferGTest, TestNoMoreDataToRead) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestNoMoreDataToRead());
}

