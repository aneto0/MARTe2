/**
 * @file ConsoleLogger.cpp
 * @brief Source file for class ConsoleLogger
 * @date 10/03/2017
 * @author Andre Neto
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
 * the class ConsoleLogger (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConsoleLogger.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ConsoleLogger::ConsoleLogger() : Object(), LoggerConsumerI() {
    (void) console.Open(BasicConsoleMode::Default);
}

/*lint -e{1551} the destructor must guarantee that the console is closed.*/
ConsoleLogger::~ConsoleLogger() {
    (void) console.Close();
}

bool ConsoleLogger::Initialise(StructuredDataI &data) {
    bool ok = Object::Initialise(data);
    if (ok) {
        LoadPrintPreferences(data);
    }
    return ok;

}

void ConsoleLogger::ConsumeLogMessage(LoggerPage * const logPage) {
    if (logPage != NULL_PTR(LoggerPage *)) {
        StreamString err;
        PrintToStream(logPage, err);
        err += "\n";
        uint32 size32 = static_cast<uint32>(err.Size());
        (void) console.Write(err.Buffer(), size32);
    }
}

CLASS_REGISTER(ConsoleLogger, "1.0")
}

