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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardPrinterTest.h"
#include "MARTe2UTest.h"

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintOpenMatrix) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenMatrix());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintCloseMatrix) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseMatrix());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintScalarSeparator) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintScalarSeparator());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintVectorSeparator) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVectorSeparator());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintVariableSeparator) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVariableSeparator());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintBlockSeparator) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintBlockSeparator());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintOpenVector) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenVector());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintCloseVector) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseVector());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintOpenBlock) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenBlock());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintCloseBlock) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseBlock());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintOpenAssignment) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenAssignment());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintCloseAssignment) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseAssignment());
}

TEST(BareMetal_L3Streams_StandardPrinterGTest,TestPrintVariable) {
    StandardPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVariable());
}
