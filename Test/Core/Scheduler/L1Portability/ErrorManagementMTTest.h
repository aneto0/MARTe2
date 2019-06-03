/**
 * @file ErrorManagementTest.h
 * @brief Header file for class ErrorManagementTest
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

 * @details This header file contains the declaration of the class ErrorManagementTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ERRORMANAGEMENT_MT_TEST_H_
#define ERRORMANAGEMENT_MT_TEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagementTest.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class ErrorManagementMTTest:public ErrorManagementTest {

public:
    /**
     * @brief Tests the ReportError function.
     * @details The error function checks if the ErrorInfo structure passed by the ReportError function is filled correctly with the
     * right error informations. Launches a certain number of threads checking if also the thread id field is correct.
     * @param[in] code is the desired error code.
     * @param[in] expected is the expected error code string.
     * @param[in] errorDescription is the desired error description.
     * @param[in] errorFileName is the desired error file name.
     * @param[in] errorLineNumber is the desired error line number.
     * @param[in] errorFunctionName is the desired error function name.
     * @param[in] numThreads is the number of threads to launch.
     * @return true if the ErrorInfo structure passed by the ReportError function is filled correctly with the
     * right error informations, false otherwise.
     */
    bool TestReportErrorFullContext(ErrorManagement::ErrorType code,
                                    const char8* expected,
                                    const char8* errorDescription,
                                    const char8* errorFileName = "",
                                    int16 errorLineNumber = 0,
                                    const char8* errorFunctionName = "",
                                    uint32 numThreads = 0);


    /**
     * @brief Tests the REPORT_ERROR_FULL macro.
     * @param[in] code is the desired error code.
     * @param[in] errorDescription is the desired error description.
     * @param[in] errorName is the expected error code string.
     * @param[in] numThreads is the number of threads to launch.
     * @return true if the ErrorInfo Structure is filled with the file name, the line number and the function name where the error is triggered.
     * Launches also a certain number of threads and returns true if the thread is field is equal to the id of the thread which calls the macro.
     */
    bool TestReportErrorMacroFullContext(ErrorManagement::ErrorType code,
                                         const char8 *errorDescription,
                                         const char8 *errorName,
                                         uint32 numThreads);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ERRORMANAGEMENTTEST_H_ */

