/**
 * @file LogManagementTest.cpp
 * @brief Source file for class LogManagementTest
 * @date 24/ago/2015
 * @author pc
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
 * the class LogManagementTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LogManagementTest.h"
#include "StringHelper.h"
#include "Sleep.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
//define static members
ErrorType LogManagementTest::expectedErrorCode;
const char8* LogManagementTest::expectedErrorDescription;
const char8* LogManagementTest::expectedErrorFilename;
uint16 LogManagementTest::expectedErrorLine;
const char8* LogManagementTest::expectedErrorFunction;
const char8* LogManagementTest::expectedErrorName;
bool LogManagementTest::fullContext = false;
bool LogManagementTest::retVal = false;

void DummyErrorFunction(const LogManagement::LogInformation& errorInfo,
                        const char * const description) {

    LogManagementTest newEM;
    newEM.retVal = true;

}

void ReportTestFunction(const LogManagement::LogInformation& errorInfo,
                        const char * const description) {

    //shared static attributes
    LogManagementTest newEM;
    //checks if the structure is filled correctly
    newEM.CheckParameters(errorInfo, description);

}

void ThreadErrorTestFunction(LogManagementTest& t) {
    //launches error report functions.
    t.fullContext = false;

    LogManagement::ReportLogMessage(t.expectedErrorCode, t.expectedErrorDescription, t.expectedErrorFilename, t.expectedErrorLine, t.expectedErrorFunction);
    t.fullContext = true;
    LogManagement::ReportLogMessageFullContext(t.expectedErrorCode, t.expectedErrorDescription, t.expectedErrorFilename, t.expectedErrorLine,
                                            t.expectedErrorFunction);

    t.syncFlag = true;
}

void ThreadErrorTestFunctionMacro(LogManagementTest &t) {
    //launches error report functions.
    t.fullContext = false;
    t.expectedErrorFunction = __DECORATED_FUNCTION_NAME__;
    t.expectedErrorLine = __LINE__ + 1;
    REPORT_LOG_MESSAGE(t.expectedErrorCode, t.expectedErrorDescription);
    t.fullContext = true;
    t.expectedErrorLine = __LINE__ + 1;
    REPORT_LOG_MESSAGE_FULL(t.expectedErrorCode, t.expectedErrorDescription);

    t.syncFlag = true;
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool LogManagementTest::TestSetLogMessageProcessFunction() {
    retVal = false;

    LogManagement::SetLogMessageProcessFunction(DummyErrorFunction);

    LogManagement::ReportLogMessage(Information, "");

    return retVal;

}

bool LogManagementTest::TestToName() {

    ErrorType all[] = { NoError, Debug, Information, Warning, FatalError, RecoverableError, InitialisationError, OSError, ParametersError, IllegalOperation,
            ErrorSharing, ErrorAccessDenied, Exception, Timeout, CommunicationError, SyntaxError, UnsupportedFeature };

    const char8 *names[] = { "NoError", "Debug Information", "Information", "Warning", "FatalError", "RecoverableError", "InitialisationError", "OSError",
            "ParametersError", "IllegalOperation", "ErrorSharing", "ErrorAccessDenied", "Exception", "Timeout", "CommunicationError", "SyntaxError",
            "UnsupportedError" };

    uint32 i = 0;
    while (all[i] != UnsupportedFeature) {
        if (StringHelper::Compare(LogManagement::ToName(all[i]), names[i]) != 0) {
            return false;
        }
        i++;
    }

    return StringHelper::Compare(LogManagement::ToName(UnsupportedFeature), "UnsupportedError") == 0;
}

bool LogManagementTest::TestReportLogMessage(ErrorType code,
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

    LogManagement::SetLogMessageProcessFunction(ReportTestFunction);
    LogManagement::ReportLogMessage(code, errorDescription, errorFileName, errorLineNumber, errorFunctionName);

    return retVal;
}

bool LogManagementTest::TestReportLogMessageFullContext(ErrorType code,
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

    LogManagement::SetLogMessageProcessFunction(ReportTestFunction);

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

bool LogManagementTest::TestReportLogMessageMacro(ErrorType code,
                                                  const char8 *errorDescription,
                                                  const char8 *errorName) {

    expectedErrorCode = code;
    expectedErrorDescription = errorDescription;
    expectedErrorFilename = __FILE__;
    expectedErrorFunction = __DECORATED_FUNCTION_NAME__;
    expectedErrorName = errorName;
    LogManagement::SetLogMessageProcessFunction(ReportTestFunction);

    //put always the report error at the next line otherwise the test will fail!
    expectedErrorLine = __LINE__ + 1;
    REPORT_LOG_MESSAGE(code, errorDescription);

    return retVal;

}

bool LogManagementTest::TestReportLogMessageMacroFullContext(ErrorType code,
                                                             const char8 *errorDescription,
                                                             const char8 *errorName,
                                                             uint32 numThreads) {

    expectedErrorCode = code;
    expectedErrorDescription = errorDescription;
    expectedErrorFilename = __FILE__;
    expectedErrorName = errorName;
    LogManagement::SetLogMessageProcessFunction(ReportTestFunction);
    nThreads = numThreads;

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

void LogManagementTest::CheckParameters(const LogManagement::LogInformation& errorInfo,
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
    if (StringHelper::Compare(LogManagement::ToName(expectedErrorCode), expectedErrorName) != 0) {
        retVal = false;
        return;
    }

    retVal = true;
}

