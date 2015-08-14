/**
 * @file ErrorManagementTest.cpp
 * @brief Source file for class ErrorManagementTest
 * @date 14/08/2015
 * @author Giuseppe Ferrò
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
 * the class ErrorManagementTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagementTest.h"
#include "StringHelper.h"
#include "Sleep.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//define static members
ErrorType ErrorManagementTest::expectedErrorCode;
const char8* ErrorManagementTest::expectedErrorDescription;
const char8* ErrorManagementTest::expectedErrorFilename;
uint16 ErrorManagementTest::expectedErrorLine;
const char8* ErrorManagementTest::expectedErrorFunction;
const char8* ErrorManagementTest::expectedErrorName;
bool ErrorManagementTest::fullContext = false;
bool ErrorManagementTest::retVal = false;

void DummyErrorFunction(ErrorInformation& errorInfo,
                        const char* description) {

    ErrorManagementTest newEM;
    newEM.retVal = true;

}

void ReportTestFunction(ErrorInformation& errorInfo,
                        const char* description) {

    //shared static attributes
    ErrorManagementTest newEM;
    //checks if the structure is filled correctly
    newEM.CheckParameters(errorInfo, description);

}

void ThreadErrorTestFunction(ErrorManagementTest& t) {
    //launches error report functions.
    t.fullContext = false;

    ErrorManagement::ReportError(t.expectedErrorCode, t.expectedErrorDescription, t.expectedErrorFilename, t.expectedErrorLine, t.expectedErrorFunction);
    t.fullContext = true;
    ErrorManagement::ReportErrorFullContext(t.expectedErrorCode, t.expectedErrorDescription, t.expectedErrorFilename, t.expectedErrorLine,
                                            t.expectedErrorFunction);

    t.syncFlag = true;
}

void ThreadErrorTestFunctionMacro(ErrorManagementTest& t) {
    //launches error report functions.
    t.fullContext = false;
    t.expectedErrorFunction = __FUNCTION_NAME__;
    t.expectedErrorLine = __LINE__ + 1;
    ErrorManagement::REPORT_ERROR(t.expectedErrorCode, t.expectedErrorDescription);
    t.fullContext = true;
    t.expectedErrorLine = __LINE__ + 1;
    ErrorManagement::REPORT_ERROR_FULL(t.expectedErrorCode, t.expectedErrorDescription);

    t.syncFlag = true;
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ErrorManagementTest::TestSetErrorMessageProcessFunction() {
    retVal = false;

    ErrorManagement::SetErrorMessageProcessFunction(DummyErrorFunction);

    ErrorManagement::ReportError(Information, "");

    return retVal;

}

bool ErrorManagementTest::TestErrorName() {

    ErrorType all[] = { NoError, Debug, Information, Warning, FatalError, RecoverableError, InitialisationError, OSError, ParametersError, IllegalOperation,
            ErrorSharing, ErrorAccessDenied, Exception, Timeout, CommunicationError, SyntaxError, UnsupportedFeature };

    uint32 i = 0;
    while (all[i] != UnsupportedFeature) {
        if (StringHelper::Compare(ErrorManagement::ErrorName(all[i]), errorNames[i].name) != 0) {
            return false;
        }
        i++;
    }

    return StringHelper::Compare(ErrorManagement::ErrorName(UnsupportedFeature), "UnsupportedError") == 0;
}

bool ErrorManagementTest::TestReportError(ErrorType code,
                                          const char8* errorName,
                                          const char8* errorDescription,
                                          const char8* errorFileName,
                                          uint16 errorLineNumber,
                                          const char8* errorFunctionName) {

    expectedErrorCode = code;
    expectedErrorDescription = errorDescription;
    expectedErrorFilename = errorFileName;
    expectedErrorLine = errorLineNumber;
    expectedErrorFunction = errorFunctionName;
    expectedErrorName = errorName;

    ErrorManagement::SetErrorMessageProcessFunction(ReportTestFunction);
    ErrorManagement::ReportError(code, errorDescription, errorFileName, errorLineNumber, errorFunctionName);

    return retVal;
}

bool ErrorManagementTest::TestReportErrorFullContext(ErrorType code,
                                                     const char8* errorName,
                                                     const char8* errorDescription,
                                                     const char8* errorFileName,
                                                     uint16 errorLineNumber,
                                                     const char8* errorFunctionName,
                                                     uint32 numThreads) {

    //Fill the class attributes
    expectedErrorCode = code;
    expectedErrorDescription = errorDescription;
    expectedErrorFilename = errorFileName;
    expectedErrorLine = errorLineNumber;
    expectedErrorFunction = errorFunctionName;
    expectedErrorName = errorName;
    nThreads = numThreads;

    ErrorManagement::SetErrorMessageProcessFunction(ReportTestFunction);

    for (uint32 i = 0; i < nThreads; i++) {
        retVal = false;
        syncFlag = false;

        //Launches the thread
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) ThreadErrorTestFunction, this);

        //Wait that syncFlag becomes true or the timeout(about 100ms)
        uint32 j = 0;
        while (!syncFlag) {
            if (j++ > 100) {
                //In case of timeout something wrong then return false
                Threads::Kill(tid);
                return false;
            }
            Sleep::Sec(1e-3);
        }

        //if the test fails return false
        Sleep::Sec(1e-3);
        if (!retVal) {
            return false;
        }

    }

    //success, return true.
    return retVal;
}

bool ErrorManagementTest::TestReportErrorMacro(ErrorType code,
                                               const char8 *errorDescription,
                                               const char8 *errorName) {

    expectedErrorCode = code;
    expectedErrorDescription = errorDescription;
    expectedErrorFilename = __FILE__;
    expectedErrorFunction = __FUNCTION_NAME__;
    expectedErrorName = errorName;
    ErrorManagement::SetErrorMessageProcessFunction(ReportTestFunction);

    //put always the report error at the next line otherwise the test will fail!
    expectedErrorLine = __LINE__ + 1;
    REPORT_ERROR(code, errorDescription);

    return retVal;

}

bool ErrorManagementTest::TestReportErrorMacroFullContext(ErrorType code,
                                                          const char8 *errorDescription,
                                                          const char8 *errorName,
                                                          uint32 numThreads) {

    expectedErrorCode = code;
    expectedErrorDescription = errorDescription;
    expectedErrorFilename = __FILE__;
    expectedErrorName = errorName;
    ErrorManagement::SetErrorMessageProcessFunction(ReportTestFunction);
    nThreads=numThreads;

    for (uint32 i = 0; i < nThreads; i++) {
        retVal = false;
        syncFlag = false;

        //Launches the thread
        ThreadIdentifier tid = Threads::BeginThread((ThreadFunctionType) ThreadErrorTestFunctionMacro, this);

        //Wait that syncFlag becomes true or the timeout(about 100ms)
        uint32 j = 0;
        while (!syncFlag) {
            if (j++ > 100) {
                //In case of timeout something wrong then return false
                Threads::Kill(tid);
                return false;
            }
            Sleep::Sec(1e-3);
        }

        //if the test fails return false
        Sleep::Sec(1e-3);
        if (!retVal) {
            return false;
        }

    }

    //succes, return true.
    return retVal;

}

void ErrorManagementTest::CheckParameters(ErrorInformation& errorInfo,
                                          const char* description) {

    //Checks the error code
    if (errorInfo.header.errorType != expectedErrorCode) {
        retVal = false;
        return;
    }

    //Checks the error line number
    if (errorInfo.header.lineNumber != expectedErrorLine) {
        retVal = false;
        return;
    }

    //Checks the error file name. */
    if (StringHelper::Compare(errorInfo.fileName, expectedErrorFilename) != 0) {
        retVal = false;
        return;
    }

    //Checks the error function name. */
    if (StringHelper::Compare(errorInfo.functionName, expectedErrorFunction) != 0) {
        retVal = false;
        return;
    }

    //Checks the error description. */
    if (StringHelper::Compare(description, expectedErrorDescription) != 0) {
        retVal = false;
        return;
    }

    if (fullContext) {
        //Checks the thread id. */
        if (errorInfo.threadId != Threads::Id()) {
            retVal = false;
            return;
        }

    }
    //Tests the errorName function.
    if (StringHelper::Compare(ErrorManagement::ErrorName(expectedErrorCode), expectedErrorName) != 0) {
        retVal = false;
        return;
    }

    retVal = true;
}

