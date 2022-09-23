/**
 * @file PrinterITest.cpp
 * @brief Source file for class PrinterITest
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
 * the class PrinterITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ConfigurationDatabase.h"
#include "PrinterITest.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class PrinterITestClass: public PrinterI {

public:
    PrinterITestClass();

    PrinterITestClass(BufferedStreamI & streamIn);

    virtual ~PrinterITestClass();

    virtual bool PrintOpenMatrix();

    virtual bool PrintCloseMatrix();

    virtual bool PrintScalarSeparator();

    virtual bool PrintVectorSeparator();

    virtual bool PrintVariableSeparator();

    virtual bool PrintBlockSeparator();

    virtual bool PrintOpenVector();

    virtual bool PrintCloseVector();

    virtual bool PrintOpenBlock(const char8 *const blockName);

    virtual bool PrintCloseBlock(const char8 *const blockName);

    virtual bool PrintOpenAssignment(const char8 *const varName);

    virtual bool PrintCloseAssignment(const char8 *const varName);

    virtual bool PrintVariable(const AnyType &var);

    BufferedStreamI *GetStream();

};

PrinterITestClass::PrinterITestClass() :
        PrinterI() {

}

PrinterITestClass::PrinterITestClass(BufferedStreamI & streamIn) :
        PrinterI(streamIn) {

}

PrinterITestClass::~PrinterITestClass() {

}

bool PrinterITestClass::PrintOpenMatrix() {
    return true;
}

bool PrinterITestClass::PrintCloseMatrix() {
    return true;
}

bool PrinterITestClass::PrintScalarSeparator() {
    return true;
}

bool PrinterITestClass::PrintVectorSeparator() {
    return true;
}

bool PrinterITestClass::PrintVariableSeparator() {
    return true;
}

bool PrinterITestClass::PrintBlockSeparator() {
    return true;
}

bool PrinterITestClass::PrintOpenVector() {
    return true;
}

bool PrinterITestClass::PrintCloseVector() {
    return true;
}

bool PrinterITestClass::PrintOpenBlock(const char8 *const blockName) {
    return true;
}

bool PrinterITestClass::PrintCloseBlock(const char8 *const blockName) {
    return true;
}

bool PrinterITestClass::PrintOpenAssignment(const char8 *const varName) {
    return true;
}

bool PrinterITestClass::PrintCloseAssignment(const char8 *const varName) {
    return true;
}

bool PrinterITestClass::PrintVariable(const AnyType &var) {
    return true;
}

BufferedStreamI *PrinterITestClass::GetStream() {
    return stream;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

PrinterITest::PrinterITest() {

}

PrinterITest::~PrinterITest() {

}

bool PrinterITest::TestConstructor() {
    PrinterITestClass test;
    return test.GetStream() == NULL;
}

bool PrinterITest::TestFullConstructor() {
    StreamString stream;
    PrinterITestClass test(stream);
    return test.GetStream() == &stream;
}

bool PrinterITest::TestSetStream() {
    StreamString stream1;
    PrinterITestClass test(stream1);
    bool ret = test.GetStream() == &stream1;
    StreamString stream2;
    test.SetStream(stream2);
    ret &= (test.GetStream() == &stream2);
    return ret;
}

bool PrinterITest::TestPrintBegin() {
    StreamString stream1;
    PrinterITestClass test(stream1);
    return test.PrintBegin();
}

bool PrinterITest::TestPrintEnd() {
    StreamString stream1;
    PrinterITestClass test(stream1);
    return test.PrintEnd();
}

bool PrinterITest::TestPrintStructuredDataI() {
    StreamString stream1;
    PrinterITestClass test(stream1);
    ConfigurationDatabase cdb;
    bool ok = test.PrintStructuredDataI(cdb, false);
    if (ok) {
        ok = test.PrintStructuredDataI(cdb, true);
    }
    return ok;
}

