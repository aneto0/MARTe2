/**
 * @file ErrorLogger.cpp
 * @brief Source file for class ErrorLogger
 * @date 27/03/2018
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
 * the class ErrorLogger (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "ErrorLoggerExample.h"

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
void ErrorProcessExampleFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
    using namespace MARTe;
    const char8 * RED = "\x1B[31m";
    const char8 * GRN = "\x1B[32m";
    const char8 * RST = "\x1B[0m";

    StreamString errorCodeStr;
    ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errorCodeStr);
    if (errorInfo.header.errorType == ErrorManagement::Information) {
        printf("%s", GRN);
    }
    else if (errorInfo.header.errorType == ErrorManagement::NoError) {
        printf("%s", GRN);
    }
    else {
        printf("%s", RED);
    }
    if (errorInfo.objectName) {
        printf("[%s - %s:%d - %s]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorInfo.objectName, errorDescription);
    }
    else {
        printf("[%s - %s:%d]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    }
    printf("%s", RST);
}
