/**
 * @file StandardPrinter.cpp
 * @brief Source file for class StandardPrinter
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
 * the class StandardPrinter (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardPrinter.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

StandardPrinter::StandardPrinter(BufferedStreamI & streamIn): PrinterI(streamIn) {
}

StandardPrinter::StandardPrinter():PrinterI(){

}

StandardPrinter::~StandardPrinter() {
}


bool StandardPrinter::PrintOpenMatrix(){
    return stream->Printf("%s", "{");
}

bool StandardPrinter::PrintCloseMatrix(){
    return stream->Printf("%s", "}");
}

bool StandardPrinter::PrintScalarSeparator(){
    return stream->Printf("%s", " ");
}

bool StandardPrinter::PrintVectorSeparator(){
    return true;
}

bool StandardPrinter::PrintVariableSeparator(){
    return true;
}

bool StandardPrinter::PrintBlockSeparator(){
    return true;
}

bool StandardPrinter::PrintOpenVector(){
    return stream->Printf("%s", "{");
}

bool StandardPrinter::PrintCloseVector(){
    return stream->Printf("%s", "}");
}

bool StandardPrinter::PrintOpenBlock(const char8 *const blockName){
    return stream->Printf("%s = {", blockName);
}

/*lint -e{715} parameter not used in this function*/
bool StandardPrinter::PrintCloseBlock(const char8 *const blockName){
    return stream->Printf("%s", "}");
}

bool StandardPrinter::PrintOpenAssignment(const char8 *const varName){
    return stream->Printf("%s =", varName);
}

/*lint -e{715} parameter not used in this function*/
bool StandardPrinter::PrintCloseAssignment(const char8 *const varName){
    return true;
}

bool StandardPrinter::PrintVariable(const AnyType &var){
    return stream->Printf("%#!", var);
}
}
