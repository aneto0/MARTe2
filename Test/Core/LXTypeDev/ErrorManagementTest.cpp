#include "ErrorManagementTest.h"

void ReportTestFunction(ErrorInformation& errorInfo, const char* description) {

    //shared static attributes
    ErrorManagementTest newEM;
    //checks if the structure is filled correctly
    newEM.CheckParameters(errorInfo, description);

}

void ReportParametersTestFunction(ErrorInformation& errorInfo,
                                  const char* description) {
    //shared static attributes
    ErrorManagementTest newEM;

    const char* expected = newEM.expectedErrorName;
    uint32 lineNumber = newEM.errLine;
    const char* functionName = newEM.errFunction;
    const char* fileName = newEM.errFilename;

    //builds the expected result.
    StreamString test;
    test = "";
    test.Printf("Type:%s File:%s Line:%d Fun:%s", expected, fileName,
                lineNumber, functionName);

    //checks if the description and the expected results are equal.
    if (test != description) {
        newEM.retVal = False;
        return;
    }

    //checks if the function field contains the function which calls the error.
    if (StringHelper::Compare(errorInfo.functionName, "TestReportError") != 0) {
        newEM.retVal = False;
        return;
    }

    newEM.retVal = True;
}

void ThreadErrorTestFunction(ErrorManagementTest& t) {
    //launchs error report functions.
    ErrorManagement::ReportError(t.errCode, t.errDescription, t.errFilename,
                                 t.errLine, t.errFunction);
    ErrorManagement::ReportErrorFullContext(t.errCode, t.errDescription,
                                            t.errFilename, t.errLine,
                                            t.errFunction);
    t.syncFlag = True;
}

//declare static members
ErrorType ErrorManagementTest::errCode;
const char* ErrorManagementTest::errDescription;
const char* ErrorManagementTest::errFilename;
uint32 ErrorManagementTest::errLine;
const char* ErrorManagementTest::errFunction;
uint32 ErrorManagementTest::nThreads;
bool ErrorManagementTest::retVal;
const char* ErrorManagementTest::expectedErrorName;

