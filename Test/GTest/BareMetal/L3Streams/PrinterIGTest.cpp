/**
 * @file PrinterIGTest.cpp
 * @brief Source file for class PrinterIGTest
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
 * the class PrinterIGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "PrinterITest.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L3Streams_PrinterIGTest, TestConstructor) {
    PrinterITest myPrinterITest;
    ASSERT_TRUE(myPrinterITest.TestConstructor());
}

TEST(BareMetal_L3Streams_PrinterIGTest, TestFullConstructor) {
    PrinterITest myPrinterITest;
    ASSERT_TRUE(myPrinterITest.TestFullConstructor());
}

TEST(BareMetal_L3Streams_PrinterIGTest, TestSetStream) {
    PrinterITest myPrinterITest;
    ASSERT_TRUE(myPrinterITest.TestSetStream());
}

TEST(BareMetal_L3Streams_PrinterIGTest, TestPrintBegin) {
    PrinterITest myPrinterITest;
    ASSERT_TRUE(myPrinterITest.TestPrintBegin());
}

TEST(BareMetal_L3Streams_PrinterIGTest, TestPrintEnd) {
    PrinterITest myPrinterITest;
    ASSERT_TRUE(myPrinterITest.TestPrintEnd());
}

TEST(BareMetal_L3Streams_PrinterIGTest, TestPrintStructuredDataI) {
    PrinterITest myPrinterITest;
    ASSERT_TRUE(myPrinterITest.TestPrintStructuredDataI());
}

