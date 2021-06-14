/**
 * @file XMLPrinterGTest.cpp
 * @brief Source file for class XMLPrinterGTest
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
 * the class XMLPrinterGTest (public, protected, and private). Be aware that some 
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

#include "XMLPrinterTest.h"
#include "MARTe2UTest.h"

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintOpenMatrix) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenMatrix());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintCloseMatrix) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseMatrix());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintScalarSeparator) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintScalarSeparator());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintVectorSeparator) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVectorSeparator());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintVariableSeparator) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVariableSeparator());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintBlockSeparator) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintBlockSeparator());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintOpenVector) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenVector());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintCloseVector) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseVector());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintOpenBlock) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenBlock());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintCloseBlock) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseBlock());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintOpenAssignment) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintOpenAssignment());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintCloseAssignment) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintCloseAssignment());
}

TEST(BareMetal_L3Streams_XMLPrinterGTest,TestPrintVariable) {
    XMLPrinterTest streamtest;
    ASSERT_TRUE(streamtest.TestPrintVariable());
}
