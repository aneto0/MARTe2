/**
 * @file StringIOBufferGTest.cpp
 * @brief Source file for class StringIOBufferGTest
 * @date 26/10/2015
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
 * the class StringIOBufferGTest (public, protected, and private). Be aware that some 
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
#include "StringIOBufferTest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(StringIOBufferGTest, TestDefaultConstructor) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestDefaultConstructor());
}

TEST(StringIOBufferGTest, TestGranularityConstructor_1) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(1, 1));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_2) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(2, 2));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_3) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(3, 2));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_5) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(5, 4));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_10) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(10, 8));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_16) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(16, 16));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_20) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(20, 16));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_33) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(33, 32));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_127) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(127, 64));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_0) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(0, 1));
}

TEST(StringIOBufferGTest, TestGranularityConstructor_MAX) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestGranularityConstructor(0xffffffff, 0x80000000));
}


TEST(StringIOBufferGTest, TestSetBufferAllocationSize_size_in_granularity) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestSetBufferAllocationSize(10, 32));
}

TEST(StringIOBufferGTest, TestSetBufferAllocationSize_granularity_in_size) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestSetBufferAllocationSize(10, 16));
}

TEST(StringIOBufferGTest, TestSetBufferAllocationSize_zero_size) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestSetBufferAllocationSize(0, 16));
}


TEST(StringIOBufferGTest, TestWrite_minor_size) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestWrite(5, "HelloWorld"));
}

TEST(StringIOBufferGTest, TestWrite_equal_size) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestWrite(11, "HelloWorld"));
}

TEST(StringIOBufferGTest, TestTerminate_minor_size) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestTerminate(3, "HelloWorld"));
}

TEST(StringIOBufferGTest, TestTerminate_equal_size) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestTerminate(11, "HelloWorld"));
}


TEST(StringIOBufferGTest, TestWriteAll_minor_size) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestWriteAll(5, "HelloWorld"));
}

TEST(StringIOBufferGTest, TestWriteAll_equal_size) {
    StringIOBufferTest myStringIOBufferTest;
    ASSERT_TRUE(myStringIOBufferTest.TestWriteAll(11, "HelloWorld"));
}

	
