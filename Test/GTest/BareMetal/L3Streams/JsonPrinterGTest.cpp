/**
 * @file JsonPrinterGTest.cpp
 * @brief Source file for class JsonPrinterGTest
 * @date 18/09/2018
 * @author Giuseppe Ferro
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
 * the class JsonPrinterGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "JsonPrinterTest.h"
#include "gtest/gtest.h"

TEST(JsonPrinterGTest,TestPrintOpenMatrix) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenMatrix());
}

TEST(JsonPrinterGTest,TestPrintCloseMatrix) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseMatrix());
}

TEST(JsonPrinterGTest,TestPrintScalarSeparator) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintScalarSeparator());
}

TEST(JsonPrinterGTest,TestPrintVectorSeparator) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVectorSeparator());
}

TEST(JsonPrinterGTest,TestPrintVariableSeparator) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVariableSeparator());
}

TEST(JsonPrinterGTest,TestPrintBlockSeparator) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintBlockSeparator());
}

TEST(JsonPrinterGTest,TestPrintOpenVector) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenVector());
}

TEST(JsonPrinterGTest,TestPrintCloseVector) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseVector());
}

TEST(JsonPrinterGTest,TestPrintOpenBlock) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenBlock());
}

TEST(JsonPrinterGTest,TestPrintCloseBlock) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseBlock());
}

TEST(JsonPrinterGTest,TestPrintOpenAssignment) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenAssignment());
}

TEST(JsonPrinterGTest,TestPrintCloseAssignment) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseAssignment());
}

TEST(JsonPrinterGTest,TestPrintVariable) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVariable());
}

TEST(JsonPrinterGTest,TestPrintBegin) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintBegin());
}

TEST(JsonPrinterGTest,TestPrintEnd) {
    JsonPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintEnd());
}
