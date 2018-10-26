/**
 * @file StandardPrinterGTest.cpp
 * @brief Source file for class StandardPrinterGTest
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
 * the class StandardPrinterGTest (public, protected, and private). Be aware that some 
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

#include "StandardPrinterTest.h"
#include "gtest/gtest.h"

TEST(StandardPrinterGTest,TestPrintOpenMatrix) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenMatrix());
}

TEST(StandardPrinterGTest,TestPrintCloseMatrix) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseMatrix());
}

TEST(StandardPrinterGTest,TestPrintScalarSeparator) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintScalarSeparator());
}

TEST(StandardPrinterGTest,TestPrintVectorSeparator) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVectorSeparator());
}

TEST(StandardPrinterGTest,TestPrintVariableSeparator) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVariableSeparator());
}

TEST(StandardPrinterGTest,TestPrintBlockSeparator) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintBlockSeparator());
}

TEST(StandardPrinterGTest,TestPrintOpenVector) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenVector());
}

TEST(StandardPrinterGTest,TestPrintCloseVector) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseVector());
}

TEST(StandardPrinterGTest,TestPrintOpenBlock) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenBlock());
}

TEST(StandardPrinterGTest,TestPrintCloseBlock) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseBlock());
}

TEST(StandardPrinterGTest,TestPrintOpenAssignment) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenAssignment());
}

TEST(StandardPrinterGTest,TestPrintCloseAssignment) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseAssignment());
}

TEST(StandardPrinterGTest,TestPrintVariable) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVariable());
}
