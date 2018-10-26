/**
 * @file StandardPrinterTest.cpp
 * @brief Source file for class StandardPrinterTest
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
 * the class StandardPrinterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardPrinterTest.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

StandardPrinterTest::StandardPrinterTest() {

}

StandardPrinterTest::~StandardPrinterTest() {

}

bool StandardPrinterTest::TestPrintOpenMatrix() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintOpenMatrix();

    return ret && (stream == "{");
}

bool StandardPrinterTest::TestPrintCloseMatrix() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintCloseMatrix();

    return ret && (stream == "}");
}

bool StandardPrinterTest::TestPrintScalarSeparator() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintScalarSeparator();

    return ret && (stream == " ");

}

bool StandardPrinterTest::TestPrintVectorSeparator() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintVectorSeparator();

    return ret && (stream == "");

}

bool StandardPrinterTest::TestPrintVariableSeparator() {

    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintVariableSeparator();

    return ret && (stream == "");

}

bool StandardPrinterTest::TestPrintBlockSeparator() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintBlockSeparator();

    return ret && (stream == "");

}

bool StandardPrinterTest::TestPrintOpenVector() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintOpenVector();

    return ret && (stream == "{");

}

bool StandardPrinterTest::TestPrintCloseVector() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintCloseVector();

    return ret && (stream == "}");

}

bool StandardPrinterTest::TestPrintOpenBlock() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintOpenBlock("newblock");

    return ret && (stream == "newblock = {");

}

bool StandardPrinterTest::TestPrintCloseBlock() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintCloseBlock("boh");

    return ret && (stream == "}");
}

bool StandardPrinterTest::TestPrintOpenAssignment() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintOpenAssignment("newvar");

    return ret && (stream == "newvar =");

}

bool StandardPrinterTest::TestPrintCloseAssignment() {
    StreamString stream;
    StandardPrinter test(stream);

    bool ret = test.PrintCloseAssignment("boh");

    return ret && (stream == "");

}

bool StandardPrinterTest::TestPrintVariable() {
    StreamString stream;
    StandardPrinter test(stream);

    int32 x=-258;
    bool ret = test.PrintVariable(x);

    return ret && (stream == "-258");

}
