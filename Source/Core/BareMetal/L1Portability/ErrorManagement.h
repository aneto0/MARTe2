/**
 * @file ErrorManagement.h
 * @brief Header file for module ErrorManagement
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

 * @details This header file contains the declaration of the module ErrorManagement
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

#include "ErrorInformation.h"
#include "GeneralDefinitions.h"
#include "StreamI.h"
#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


namespace ErrorManagement {

/**
 * @brief The type of a user provided ErrorProcessing function
 */
typedef void (*ErrorProcessFunctionType)(const ErrorInformation &errorInfo, CCString const errorDescription);

/**
 * @brief A pointer to the function that will process the errors.
 */
extern DLL_API ErrorProcessFunctionType errorMessageProcessFunction;

/**
 * @brief A null ErrorProcessing function
 * @details This function is compatible with the function prototype defined
 * as LogMessageProcessFunctionType. Its purpose is to do nothing.
 */
DLL_API void NullErrorProcessFunction(const ErrorInformation &errorInfo,CCString const errorDescription);

/**
 * @brief Converts ErrorType to stream.
 * @param[in] errorCode is the error code.
 * @param[out] stream the full error description will be streamed into.
 */
DLL_API void ErrorCodeToStream (const ErrorType &errorCode,StreamI &stream );

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
DLL_API void ReportError(const ErrorType &code,
                         CCString const errorDescription,
                         CCString const fileName = static_cast<const char8 *>(NULL),
                         const int16 lineNumber = 0,
                         CCString const functionName = static_cast<const char8 *>(NULL));

/**
 * @brief Stores the error informations in an ErrorInformation structure, then calls a predefined routine.
 * @details The thread identifier is always stored in the ErrorInformation structure.
 * @param[in] code is the error code.
 * @param[in] errorDescription is the error description.
 * @param[in] fileName is the file name where the error was triggered.
 * @param[in] lineNumber is the line number where the error was triggered.
 * @param[in] functionName is the name of the function where the error is triggered.
 */
DLL_API void ReportErrorFullContext(const ErrorType &code,
                                    CCString const errorDescription,
                                    CCString const fileName = static_cast<const char8 *>(NULL),
                                    const int16 lineNumber = 0,
                                    CCString const functionName = static_cast<const char8 *>(NULL));

/**
 * @brief Sets the routine for error managing.
 * @param[in] userFun is a pointer to the function called by ReportError.
 */
DLL_API void SetErrorProcessFunction(const ErrorProcessFunctionType userFun);

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * @brief The function to call in case of errors.
 * @details Calls ErrorManagement::ReportError with the file name, the function and the line number of the error as inputs.
 * @param[in] code is the ErrorType code error.
 * @param[in] message is the description associated to the error.
 */
/*lint -save -e9026
 * 9026: function-like macro defined.
 */
#define REPORT_ERROR(code,message)\
MARTe::ErrorManagement::ReportError(code,message,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__)

#if 0
/**
 * @brief Checks a condition, reports error and sets an error variable
 * @details If condition true, Calls ErrorManagement::ReportError with the file name, the function and the line number of the error as inputs and sets the errorVariable
 * @param[in] errorCode is the ErrorType code error and the errorVariable error field.
 * @param[in] message is the description associated to the error.
 * @param[in] condition is the boolean condition that has to be true
 * @param[in] errorVariable is the variable to be set
 */
/*lint -save -e9026
 * 9026: function-like macro defined.
 */
#define MANAGE_ERROR(errorVariable,errorCode,condition,message)\
	if (condition ){\
		errorVariable. ## errorCode = true; \
		MARTe::ErrorManagement::ReportError(ErrorManagement:: ## errorCode,message,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \
	}

#endif
/**
 * @brief The function to call in case of errors.
 * @details Calls ErrorManagement::ReportErrorFullContext with the file name, the function and the line number of the error as inputs.
 * @param[in] code is the ErrorType code error.
 * @param[in] message is the description associated to the error.
 */
#define REPORT_ERROR_FULL(code,message)\
MARTe::ErrorManagement::ReportErrorFullContext(code,message,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__)

}
#endif /* ERRORMANAGEMENT_H_ */

