/**
 * @file ErrorManagement.h
 * @brief Header file for class ErrorManagement
 * @date 13/08/2015
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

 * @details This header file contains the declaration of the class ErrorManagement
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ERRORMANAGEMENT_H_
#define ERRORMANAGEMENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "ErrorType.h"
#include "Threads.h"
#include "HighResolutionTimer.h"
#include "ErrorInformation.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Collection of functions and  types to manage error reporting.
 * @details These functions allows an error reporting mechanism. The user should only pass the code of the error
 * and a description but automatically the name of the file, the line number and the function
 * are stored in the ErrorInformation structure. The user can implement a routine that will be called
 * by the report error function to manage errors in specific ways.
 */
namespace ErrorManagement {

/**
 * @brief The type of an user provided ErrorProcessing function
 */
typedef void (*ErrorMessageProcessFunctionType)(const ErrorInformation &errorInfo,
                                                const char8 * const errorDescription);

/**
 * @brief A pointer to the function that will process the errors.
 */
extern ErrorMessageProcessFunctionType errorMessageProcessFunction;

/**
 * @brief Returns the name string associated to the error code.
 * @param[in] errorCode is the error code.
 */
const char8 *ErrorName(const ErrorType errorCode);

/**
 * @brief Stores the error informations in an ErrorInformation structure, then calls a predefined routine.
 * @details The thread identifier is stored in the ErrorInformation structure only if interrupts are disabled, because
 * it is not possible get the thread id in an interrupt routine.
 * @param[in] code is the error code.
 * @param[in] errorDescription is the error description.
 * @param[in] fileName is the file name where the error was triggered.
 * @param[in] lineNumber is the line number where the error was triggered.
 * @param[in] functionName is the name of the function where the error is triggered.
 */
void ReportError(const ErrorType code,
                 const char8 * const errorDescription,
                 const char8 * const fileName = static_cast<const char8 *>(NULL),
                 const uint16 lineNumber = static_cast<uint16>(0u),
                 const char8 * const functionName = static_cast<const char8 *>(NULL));

/**
 * @brief Stores the error informations in an ErrorInformation structure, then calls a predefined routine.
 * @details The thread identifier is always stored in the ErrorInformation structure.
 * @param[in] code is the error code.
 * @param[in] errorDescription is the error description.
 * @param[in] fileName is the file name where the error was triggered.
 * @param[in] lineNumber is the line number where the error was triggered.
 * @param[in] functionName is the name of the function where the error is triggered.
 */
void ReportErrorFullContext(const ErrorType code,
                            const char8 * const errorDescription,
                            const char8 * const fileName = static_cast<const char8 *>(NULL),
                            const uint16 lineNumber = static_cast<uint16>(0u),
                            const char8 * const functionName = static_cast<const char8 *>(NULL));

/**
 * @brief Sets the routine for error managing.
 * @param[in] ErrorMessageProcessFunctionType is a pointer to the function called by ReportError.
 */
void SetErrorMessageProcessFunction(const ErrorMessageProcessFunctionType userFun = static_cast<ErrorMessageProcessFunctionType>(NULL));

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

}

/**
 * @brief The function to call in case of errors.
 * @details Calls ErrorManagement::ReportError with the file name, the function and the line number of the error as inputs.
 */
/*lint -save -e9026
 * 9026: function-like macro defined.
 */
#define REPORT_ERROR(code,message)\
ErrorManagement::ReportError(code,message,__FILE__,__LINE__,__DECORATED_FUNCTION_NAME__);
/**
 * @brief The function to call in case of errors.
 * @details Calls ErrorManagement::ReportErrorFullContext with the file name, the function and the line number of the error as inputs.
 */
#define REPORT_ERROR_FULL(code,message)\
ErrorManagement::ReportErrorFullContext(code,message,__FILE__,__LINE__,__DECORATED_FUNCTION_NAME__);

#endif /* ERRORMANAGEMENT_H_ */

