/**
 * @file MainGTest.cpp
 * @brief Source file for class MainGTest
 * @date 08/01/2015
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
 * the class MainGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <string.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "Bootstrap.h"
#include "ConfigurationDatabase.h"
#include "ErrorManagement.h"
#include "MARTe2UTest.h"
#include "Object.h"
#include "StreamI.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
#define MAX_ERROR_PROCESS_FUNCTION_LENGTH (2 * MARTe::MAX_ERROR_MESSAGE_SIZE)
MARTe::Bootstrap bootstrap;

//TODO Verify max string length
char tempPrintBuffer[MAX_ERROR_PROCESS_FUNCTION_LENGTH];
MARTe::StreamMemoryReference printSMR(&tempPrintBuffer[0], MAX_ERROR_PROCESS_FUNCTION_LENGTH);

void MainGTestErrorProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo,
                                   const char * const errorDescription) {
    MARTe::StreamString errorCodeStr;
    MARTe::ErrorManagement::ErrorCodeToStream(errorInfo.header.errorType, errorCodeStr);
    //size_t writeLen = snprintf(tempPrintBuffer, 256, "[%s - %s:%d]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    //bootstrap.Printf("[%s - %s:%d]: %s\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    //bootstrap.Printf(tempPrintBuffer);
    memset(tempPrintBuffer, 0, MAX_ERROR_PROCESS_FUNCTION_LENGTH);
    printSMR.Seek(0);
    printSMR.Printf("[%s - %s:%d]: %s\r\n", errorCodeStr.Buffer(), errorInfo.fileName, errorInfo.header.lineNumber, errorDescription);
    tempPrintBuffer[printSMR.Size()] = '\0';          
    bootstrap.Printf(printSMR.Buffer());
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
int _main(int argc, char** argv) {
    MARTe::ErrorManagement::ErrorType ret;

    ret.fatalError = !(MARTe::UnitTest::PrepareTestEnvironment(argc, argv));
    
    if (ret) {
        ret.fatalError = !MARTe::UnitTest::Run();
    }

    MARTe::UnitTest::CleanTestEnvironment();
    return (ret ? 0 : -1);
}

int main(int argc, char **argv) {
    using namespace MARTe;
    SetErrorProcessFunction(&MainGTestErrorProcessFunction);
    
    ConfigurationDatabase loaderParameters;
    StreamI *configurationStream = NULL_PTR(StreamI *);

    bootstrap.Main(_main, argc, argv);
    
    return 0;
}

