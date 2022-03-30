/**
 * @file JsonPrinter.cpp
 * @brief Source file for class JsonPrinter
 * @date 07/09/2018
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
 * the class JsonPrinter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "JsonPrinter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

JsonPrinter::JsonPrinter(BufferedStreamI & streamIn) :
        PrinterI(streamIn) {
}

JsonPrinter::JsonPrinter() :
        PrinterI() {
}

JsonPrinter::~JsonPrinter() {

}

bool JsonPrinter::PrintOpenMatrix() {
    return stream->Printf("%s", "[");
}

bool JsonPrinter::PrintCloseMatrix() {
    return stream->Printf("%s", "]");
}

bool JsonPrinter::PrintScalarSeparator() {
    return stream->Printf("%s", ",");
}

bool JsonPrinter::PrintVectorSeparator() {
    return stream->Printf("%s", ",");
}

bool JsonPrinter::PrintVariableSeparator() {
    return stream->Printf("%s", ",");
}

bool JsonPrinter::PrintBlockSeparator() {
    return stream->Printf("%s", ",");
}

bool JsonPrinter::PrintOpenVector() {
    return stream->Printf("%s", "[");
}

bool JsonPrinter::PrintCloseVector() {
    return stream->Printf("%s", "]");
}

bool JsonPrinter::PrintOpenBlock(const char8 * const blockName) {
    return stream->Printf("\"%s\": {", blockName);
}

/*lint -e{715} parameter not used in this function*/
bool JsonPrinter::PrintCloseBlock(const char8 * const blockName) {
    return stream->Printf("%s", "}");
}

bool JsonPrinter::PrintOpenAssignment(const char8 * const varName) {
    return stream->Printf("\"%s\": ", varName);
}

/*lint -e{715} parameter not used in this function*/
bool JsonPrinter::PrintCloseAssignment(const char8 * const varName) {
    return true;
}

bool JsonPrinter::PrintVariable(const AnyType &var) {
    bool ok = true;
    TypeDescriptor td = var.GetTypeDescriptor();
    bool isNumber = (td.type == SignedInteger);
    if (!isNumber) {
        isNumber = (td.type == UnsignedInteger);
    }
    if (!isNumber) {
        isNumber = (td.type == Float);
    }
    if (isNumber) {
        ok = stream->Printf("%J!", var);
    }
    else {
        ok = stream->Printf("%#J!", var);
    }
    return ok;
}

bool JsonPrinter::PrintBegin() {
    return stream->Printf("%s", "{");
}

bool JsonPrinter::PrintEnd() {
    return stream->Printf("%s", "}");
}

bool JsonPrinter::PrintOpenArrayOfBlocks(const char8 * const blockName) {
    return stream->Printf("\"%s\" : [", blockName);
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. blockName is not required by the JSON implementation.*/
bool JsonPrinter::PrintCloseArrayOfBlocks(const char8 * const blockName) {
    return stream->Printf("%s", "]");
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. blockName is not required by the JSON implementation.*/
bool JsonPrinter::PrintOpenInsideArrayOfBlocks(const char8 * const blockName) {
    return stream->Printf("%s", "{");
}

/*lint -e{715} [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. blockName is not required by the JSON implementation.*/
bool JsonPrinter::PrintCloseInsideOfArrayOfBlocks(const char8 * const blockName) {
    return stream->Printf("%s", "}");
}

}

