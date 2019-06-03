/**
 * @file ErrorManagementTest.cpp
 * @brief Source file for class ErrorManagementTest
 * @date 25/08/2015
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

#include "ErrorManagementMTTest.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

static void ThreadErrorTestFunction(ErrorManagementTest& t) {
    //launches error report functions.
    t.fullContext = false;

    ErrorManagement::ReportError(t.expectedErrorCode, t.expectedErrorDescription,  t.expectedErrorFilename, t.expectedErrorLine, t.expectedErrorFunction);
    t.fullContext = true;
    ErrorManagement::ReportErrorFullContext(t.expectedErrorCode, t.expectedErrorDescription, emptyString,emptyString ,NULL,t.expectedErrorFilename, t.expectedErrorLine,
                                            t.expectedErrorFunction);

    t.syncFlag = true;
}

static void ThreadErrorTestFunctionMacro(ErrorManagementTest &t) {
    //launches error report functions.
    t.fullContext = false;
    t.expectedErrorFunction = __ERROR_FUNCTION_NAME__;
    t.expectedErrorLine = __LINE__ + 1;
    REPORT_ERROR(t.expectedErrorCode, t.expectedErrorDescription);
    t.fullContext = true;
    t.expectedErrorLine = __LINE__ + 1;
    REPORT_ERROR(t.expectedErrorCode, t.expectedErrorDescription);

    t.syncFlag = true;
}


bool ErrorManagementTest::TestReportErrorFullContext(ErrorManagement::ErrorType code,
                                                     const char8* errorName,
                                                     const char8* errorDescription,
                                                     const char8* errorFileName,
                                                     int16 errorLineNumber,
                                                     const char8* errorFunctionName,
                                                     uint32 numThreads) {

//Fill the class attributes
    expectedErrorCode = code;
    expectedErrorDescription = errorDescription;
    expectedErrorFilename = errorFileName;
    expectedErrorLine = errorLineNumber;
    expectedErrorFunction = errorFunctionName;
    expectedErrorName = errorName;
    expectedHRTCounter = HighResolutionTimer::Counter();
    nThreads = numThreads;

    ErrorManagement::ErrorProcessFunctionType saveFun;
    ErrorManagement::GetErrorProcessFunction(saveFun);
    ErrorManagement::SetErrorProcessFunction(ReportTestFunction);

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
            Sleep::Long(1,Units::ms);
        }

        //if the test fails return false
        Sleep::Long(1,Units::ms);
        if (!retVal) {
            return false;
        }
    }

//success, return true.
    return retVal;
}

bool ErrorManagementTest::TestReportErrorMacroFullContext(ErrorManagement::ErrorType code,
                                                          const char8 *errorDescription,
                                                          const char8 *errorName,
                                                          uint32 numThreads) {

    expectedErrorCode = code;
    expectedErrorDescription = errorDescription;
    expectedErrorFilename = __FILE__;
    expectedErrorName = errorName;
    expectedHRTCounter = HighResolutionTimer::Counter();
    ErrorManagement::SetErrorProcessFunction(ReportTestFunction);
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
            Sleep::Long(1,Units::ms);
        }

        //if the test fails return false
        Sleep::Long(1,Units::ms);
        if (!retVal) {
            return false;
        }

    }

//succes, return true.
    return retVal;

}
