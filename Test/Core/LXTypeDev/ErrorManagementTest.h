/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:$
 *
 **/
/**
 * @file ErrorManagementTest.h
 * @brief Tests the ErrorManagement functions.
 *
 * Testing of the error management reporting checking if the structure with the error informations is filled correctly. 
 * Test the function which builds the error description using printf. */

#ifndef ERROR_MANAGEMENT_TEST_H
#define ERROR_MANAGEMENT_TEST_H

#include "AdvancedErrorManagement.h"
#include "StreamString.h"
#include "StringHelper.h"
#include "Threads.h"
#include "stdio.h"
#include "Sleep.h"

#define BUFF_SIZE 128
/** @brief Class for testing of ErrorManagement functions */
class ErrorManagementTest;

/** 
 * @brief Checks if the ErrorInformation structure is filled correctly.
 * @param errorInfo is the ErrorInformation structure.
 * @param description is the error description.
 */
void ReportTestFunction(ErrorInformation& errorInfo, const char* description);

/** 
 * @brief Checks if the description built using printf is equal to the expected. 
 * @param errorInfo is the ErrorInformation structure.
 * @param description is the error description.
 */
void ReportParametersTestFunction(ErrorInformation& errorInfo,
                                  const char* description);

/** 
 * @brief The function executed by threads which launchs the report error functions.
 * @param ErrorManagementTest is the current test class to obtain parameters. */
void ThreadErrorTestFunction(ErrorManagementTest&);

/** @brief Class for testing of ErrorManagement functions. */
class ErrorManagementTest {

public:

    /** Use static members only for an access from the extern report error functions. */
    static ErrorType errCode;
    static const char* errDescription;
    static const char* errFilename;
    static uint32 errLine;
    static const char* errFunction;
    static uint32 nThreads;
    static bool retVal;
    static const char* expectedErrorName;
    bool syncFlag;

public:

    /** @brief Default constructor: initialize the return value to false. */
    ErrorManagementTest() {
        retVal = False;
    }

    /** 
     * @brief Checks if the errorInfo fields contains right datas.
     * @param errorInfo is the structure which contains the error informations.
     * @param description is the desired error description. */
    void CheckParameters(ErrorInformation& errorInfo, const char* description) {

        //Checks the error code
        if (errorInfo.header.errorType != errCode) {
            retVal = False;
            return;
        }

        //Checks the error line number
        if (errorInfo.header.lineNumber != errLine) {
            retVal = False;
            return;
        }

        //Checks the error file name. */
        if (StringHelper::Compare(errorInfo.fileName, errFilename) != 0) {
            retVal = False;
            return;
        }

        //Checks the error function name. */
        if (StringHelper::Compare(errorInfo.functionName, errFunction) != 0) {
            retVal = False;
            return;
        }

        //Checks the error description. */
        if (StringHelper::Compare(description, errDescription) != 0) {
            retVal = False;
            return;
        }

        //Checks the thread id. */
        if (nThreads > 0) {
            if (errorInfo.threadId != Threads::Id()) {
                retVal = False;
                return;
            }
        }

        //Tests the errorName function.
        if (StringHelper::Compare(ErrorManagement::ErrorName(errCode),
                                  expectedErrorName) != 0) {
            retVal = False;
            return;
        }

        retVal = True;
    }

    /** 
     * @brief Initialize class attributes with desired error informations for the test ant tests the error reporting with parameters. 
     * @param code is the desired error code.
     * @param expected is the expected error code string.
     * @param errorDescription is the desired error description.
     * @param filename is the desired error file name.
     * @param lineNumber is the desired error line number. 
     * @param functionName is the desired error function name.
     * @param nOfThreads is the desired number of threads to be launched.
     * @return true if succesful, false otherwise.
     *
     * Tests first the printf error reporting checking if the description is equal to the expected, then it launchs nOfThreads threads 
     * and compare the information in ErrorInformation structure with the parameters in input.  */
    bool TestReportError(ErrorType code, const char* expected,
                         const char* errorDescription,
                         const char* filename = "", uint32 lineNumber = 0,
                         const char* functionName = "", int32 nOfThreads = 0) {
        //Fill the class attributes
        errCode = code;
        expectedErrorName = expected;
        errDescription = errorDescription;
        errFilename = filename;
        errLine = lineNumber;
        errFunction = functionName;
        nThreads = nOfThreads;


	//Set NULL as the error function
	ErrorManagement_SetErrorMessageProcessFunction(NULL);


        //Set the error function. 
        ErrorManagement_SetErrorMessageProcessFunction(
                ReportParametersTestFunction);

        //Checks that the description is equal to the expected.
        REPORT_ERROR_PARAMETERS(code, "Type:%s File:%s Line:%d Fun:%s",
                                expected, filename, lineNumber, functionName);

        if (!retVal)
            return False;

        //Set another function.
        ErrorManagement_SetErrorMessageProcessFunction(ReportTestFunction);

        for (int32 i = 0; i < nOfThreads; i++) {
            retVal = False;
            syncFlag = False;

            //Launchs the thread
            int32 tid = Threads::BeginThread(
                    (ThreadFunctionType) ThreadErrorTestFunction, this);

            //Wait that syncFlag becomes true or the timeout(about 100ms)
            for (int32 j = 0; j < 100; j++) {
                if (j == 100) {
                    //In case of timeout something wrong then return false
                    Threads::Kill(tid);
                    return False;
                }
                SleepSec(1e-3);
                if (syncFlag)
                    break;
            }

            //if the test fails return false
            SleepSec(1e-3);
            if (!retVal)
                return False;

        }
	
        //succesful, return true.
        return retVal;
    }

};

#endif
