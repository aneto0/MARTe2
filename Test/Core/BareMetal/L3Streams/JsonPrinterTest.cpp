/**
 * @file JsonPrinterTest.cpp
 * @brief Source file for class JsonPrinterTest
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
 * the class JsonPrinterTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "JsonPrinterTest.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

JsonPrinterTest::JsonPrinterTest() {

}

JsonPrinterTest::~JsonPrinterTest() {

}

bool JsonPrinterTest::TestPrintOpenMatrix() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintOpenMatrix();

    return ret && (stream == "[");
}

bool JsonPrinterTest::TestPrintCloseMatrix() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintCloseMatrix();

    return ret && (stream == "]");
}

bool JsonPrinterTest::TestPrintScalarSeparator() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintScalarSeparator();

    return ret && (stream == ",");

}

bool JsonPrinterTest::TestPrintVectorSeparator() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintVectorSeparator();

    return ret && (stream == ",");

}

bool JsonPrinterTest::TestPrintVariableSeparator() {

    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintVariableSeparator();

    return ret && (stream == ",");

}

bool JsonPrinterTest::TestPrintBlockSeparator() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintBlockSeparator();

    return ret && (stream == ",");

}

bool JsonPrinterTest::TestPrintOpenVector() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintOpenVector();

    return ret && (stream == "[");

}

bool JsonPrinterTest::TestPrintCloseVector() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintCloseVector();

    return ret && (stream == "]");

}

bool JsonPrinterTest::TestPrintOpenBlock() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintOpenBlock("newblock");

    return ret && (stream == "\"newblock\": {");

}

bool JsonPrinterTest::TestPrintCloseBlock() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintCloseBlock("boh");

    return ret && (stream == "}");
}

bool JsonPrinterTest::TestPrintOpenAssignment() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintOpenAssignment("newvar");

    return ret && (stream == "\"newvar\": ");

}

bool JsonPrinterTest::TestPrintCloseAssignment() {
    StreamString stream;
    JsonPrinter test(stream);

    bool ret = test.PrintCloseAssignment("boh");

    return ret && (stream == "");

}

bool JsonPrinterTest::TestPrintVariable() {
    StreamString stream;
    JsonPrinter test(stream);

    int32 x=-258;
    bool ret = test.PrintVariable(x);

    return ret && (stream == "-258");

}

bool JsonPrinterTest::TestPrintBegin() {
    StreamString stream;
    JsonPrinter test(stream);
    bool ret = test.PrintBegin();

    return ret && (stream == "{");
}

bool JsonPrinterTest::TestPrintEnd() {
    StreamString stream;
    JsonPrinter test(stream);
    bool ret = test.PrintEnd();

    return ret && (stream == "}");
}

bool JsonPrinterTest::TestPrintStructuredDataI() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A");
    cdb.CreateRelative("B");
    cdb.Write("UI32", 5);
    cdb.CreateAbsolute("B[0]");
    cdb.CreateRelative("B[0]");
    cdb.Write("UI32", 3);
    cdb.Write("STR", "A");
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("B[1]");
    cdb.Write("UI32", 4);
    cdb.Write("STR", "B");
    cdb.CreateAbsolute("B[1]");
    cdb.CreateRelative("C[0]");
    cdb.Write("UI32", 4);
    cdb.Write("STR", "B");
    cdb.CreateRelative("D");
    cdb.Write("UI32", 4);
    cdb.Write("STR", "B");
    cdb.CreateAbsolute("B[2]");
    cdb.CreateRelative("C[0]");
    cdb.Write("UI32", 5);
    cdb.Write("STR", "C");
    cdb.CreateRelative("F");
    cdb.Write("UI32", 1);
    cdb.Write("STR", "A");


    cdb.MoveToRoot();
    StreamString stream;
    JsonPrinter test(stream);
    bool ok = test.PrintStructuredDataI(cdb, false);
    if (ok) {
        const char8 * const expected = "{\"A\": {\"B\": {\"UI32\": 5}},\"B[0]\": {\"B[0]\": {\"UI32\": 3,\"STR\": \"A\"},\"B[1]\": {\"UI32\": 4,\"STR\": \"B\"}},\"B[1]\": {\"C[0]\": {\"UI32\": 4,\"STR\": \"B\",\"D\": {\"UI32\": 4,\"STR\": \"B\"}}},\"B[2]\": {\"C[0]\": {\"UI32\": 5,\"STR\": \"C\",\"F\": {\"UI32\": 1,\"STR\": \"A\"}}}}";
        ok = (stream == expected);
    }
    stream = "";
    if (ok) {
        ok = test.PrintStructuredDataI(cdb, true);
    }
    if (ok) {
        const char8 * const expected = "{\"A\": {\"B\": {\"UI32\": 5}},\"B\" : [{\"B\" : [{\"UI32\": 3,\"STR\": \"A\"},{\"UI32\": 4,\"STR\": \"B\"}]},{\"C\" : [{\"UI32\": 4,\"STR\": \"B\",\"D\": {\"UI32\": 4,\"STR\": \"B\"}}]},{\"C\" : [{\"UI32\": 5,\"STR\": \"C\",\"F\": {\"UI32\": 1,\"STR\": \"A\"}}]}]}";
        ok = (stream == expected);
    }
    return ok;
}

