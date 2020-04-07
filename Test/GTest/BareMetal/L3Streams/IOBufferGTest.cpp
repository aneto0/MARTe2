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

#include "TestSupport.h"
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
    ASSERT_TRUE(myIOBufferTest.TestWrite_Memoryreference("HelloWorld",10));
}

TEST(IOBufferGTest, TestWrite_Memoryreference_Clip_Size) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestWrite_Memoryreference("HelloWorld",5));
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

TEST(IOBufferGTest, TestPrintFormatted_Integer_Decimal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedDecimalTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_Integer_Hexadecimal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedHexadecimalTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_Integer_Octal) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedOctalTable()));
}
TEST(IOBufferGTest, TestPrintFormatted_Integer_Binary) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(128, GeneratePrintFormattedBinaryTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_Float_FixedPoint) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatFPTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_Float_FixedRelativePoint) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatFPRTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_Float_Exponential) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatExpTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_Float_Engineering) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatEngTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_Float_Smart) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatSmartTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_Float_Compact) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedFloatCompactTable()));
}

TEST(IOBufferGTest, TestPrintFormatted_CCString) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_CCString());
}

TEST(IOBufferGTest, TestPrintFormatted_Pointer) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_Pointer());
}

TEST(IOBufferGTest, TestPrintFormatted_Stream) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_Stream());
}

TEST(IOBufferGTest, TestPrintFormatted_TooBigStream) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_TooBigStream());
}

TEST(IOBufferGTest, TestPrintFormatted_Stream_NotSeekable) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_Stream_NotSeekable());
}


TEST(IOBufferGTest, TestPrintFormatted_BitSet_Unsigned) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_BitSet_Unsigned());
}

TEST(IOBufferGTest, TestPrintFormatted_BitSet_Signed) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted_BitSet_Signed());
}

TEST(IOBufferGTest, TestPrintFormattedMultiple) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormatted(64, GeneratePrintFormattedGenericTable()));
}

#if 0
TEST(IOBufferGTest, TestGetToken_ConstCharOutput_TableTerminators) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetToken_ConstCharOutput(&TokenTestTableTerminators[0]));
}

TEST(IOBufferGTest, TestGetToken_ConstCharOutput_TableSkip) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetToken_ConstCharOutput(&TokenTestTableSkipCharacters[0]));
}


TEST(IOBufferGTest, GetToken_ConstCharOutput_ClipSize) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.GetToken_ConstCharOutput_ClipSize());
}
#endif

TEST(IOBufferGTest, TestGetToken_IOBufferOutput_TableTerminators) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetToken_IOBufferOutput(&TokenTestTableTerminators[0]));
}

TEST(IOBufferGTest, TestGetToken_IOBufferOutput_TableSkip) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestGetToken_IOBufferOutput(&TokenTestTableSkipCharacters[0]));
}

TEST(IOBufferGTest, TestSkipToken) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestSkipToken(&SkipTokensTestTable[0]));
}

TEST(IOBufferGTest, TestPrintFormattedObject) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedObject());
}

#if 0
TEST(IOBufferGTest, TestPrintStructuredDataInterface) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintStructuredDataInterface());
}
#endif

TEST(IOBufferGTest, TestPrintFormattedIntrospection) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedIntrospection());
}

TEST(IOBufferGTest, TestPrintVector_Int32) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableVector<int32, 3> table[]={
            {"%d" ,{1, 2 , 3}, "{ 1 2 3 } "},
            {"%-#5x" ,{1, 2 , 3}, "{ 0x1   0x2   0x3   } "},
            {0, {0}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintVector(table));
}


TEST(IOBufferGTest, TestPrintVector_Float32) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableVector<float32, 3> table[]={
            {"%f" ,{1.5, 2.5 , 3.25}, "{ 1.500000 2.500000 3.250000 } "},
            {"%3f" ,{1.5, 2.5 , 3.25}, "{ 1.5 2.5 3.2 } "},
            {0, {0}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintVector(table));
}


TEST(IOBufferGTest, TestPrintVector_CString) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableVector<const char8*, 3> table[]={
            {"%s" ,{"Hello", "Hola", "Ciao"}, "{ Hello Hola Ciao } "},
            {"%-6s" ,{"laaaaaaaarge", "short", "miiiddle"}, "{ laaaaa short  miiidd } "},
            {0, {0}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintVector(table));
}

TEST(IOBufferGTest, TestPrintVector_CArray) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableVector<const char8*, 3> table[]={
            {"%s" ,{"Hello", "Hola", "Ciao"}, "{ Hello Hola Ciao } "},
            {"%-6s" ,{"laaaaaaaarge", "short", "miiiddle"}, "{ laaaaa short  miiidd } "},
            {0, {0}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintCArrayVector(table));
}


TEST(IOBufferGTest, TestPrintMatrix_Int32) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableMatrix<int32, 2, 3 > table[]={
            {"%d" ,{{1, 2 , 3},{4, 5, 6}}, "{ { 1 2 3 } { 4 5 6 } } "},
            {"%-#5x" ,{{1, 2 , 3},{4, 5, 6}}, "{ { 0x1   0x2   0x3   } { 0x4   0x5   0x6   } } "},
            {0, {{0}}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintMatrix(table));
}


TEST(IOBufferGTest, TestPrintMatrix_Float32) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableMatrix<float32, 2, 3> table[]={
            {"%f" ,{{1.5, 2.5 , 3.25}, {0.1, 0.2 , 0.5}}, "{ { 1.500000 2.500000 3.250000 } { 0.100000 0.200000 0.500000 } } "},
            {"%3f" ,{{1.5, 2.5 , 3.25}, {0.1, 0.2 , 0.5}}, "{ { 1.5 2.5 3.2 } { 0.1 0.2 0.5 } } "},
            {0, {{0}}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintMatrix(table));
}


TEST(IOBufferGTest, TestPrintMatrix_CString) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableMatrix<const char8*, 2,3> table[]={
            {"%s" ,{{"Hello", "Hola", "Ciao"}, {"Bye", "Adieu", "Ciao"}}, "{ { Hello Hola Ciao } { Bye Adieu Ciao } } "},
            {0, {{0}}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintMatrix(table));
}

TEST(IOBufferGTest, TestPrintMatrix_CArray) {
    IOBufferTest myIOBufferTest;

    const TestPrintFormattedTableMatrix<const char8*, 2,3> table[]={
            {"%s" ,{{"Hello", "Hola", "Ciao"}, {"Bye", "Adieu", "Ciao"}}, "{ { Hello Hola Ciao } { Bye Adieu Ciao } } "},
            {0, {{0}}, 0},
    };

    ASSERT_TRUE(myIOBufferTest.TestPrintCArrayMatrix(table));
}

TEST(IOBufferGTest, TestPrintTooMuchDimensions) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintTooMuchDimensions());
}

TEST(IOBufferGTest, TestPrintFormattedIntrospection_NotIntrospectable) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedIntrospection_NotIntrospectable());
}

TEST(IOBufferGTest, TestPrintFormattedObject_NotIntrospectable) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintFormattedObject_NotIntrospectable());
}

TEST(IOBufferGTest, TestPrintPointerVector) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintPointerVector());
}


TEST(IOBufferGTest, TestPrintPointerMatrix) {
    IOBufferTest myIOBufferTest;
    ASSERT_TRUE(myIOBufferTest.TestPrintPointerMatrix());
}


