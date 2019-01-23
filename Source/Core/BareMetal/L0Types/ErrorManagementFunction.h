/**
 * @file ErrorManagementFunction.h
 * @brief Header file for class AnyType
 * @date 3 Jan 2019
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef ERRORMANAGEMENTFUNCTION_H_
#define ERRORMANAGEMENTFUNCTION_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorInformation.h"
#include "ErrorType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{
namespace ErrorManagement {

/**
 * @brief The type of a user provided ErrorProcessing function
 * @param[in] errorDescription is the error description. Will be copied. Does not need to be valid after the call.
 */
typedef void (*ErrorProcessFunctionType)(const ErrorInformation &errorInfo, CCString const errorDescription);

/**
 * @brief A pointer to the function that will process the errors.
 */
extern DLL_API ErrorProcessFunctionType errorMessageProcessFunction;

/**
 * @brief Stores the error informations in an ErrorInformation structure, then calls a predefined routine.
 * @details The thread identifier is stored in the ErrorInformation structure only if interrupts are disabled, because
 * it is not possible get the thread id in an interrupt routine.
 * @param[in] code is the error code.
 * @param[in] errorDescription is the error description. Will be copied. Does not need to be valid after the call.
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
 * @param[in] errorDescription is the error description. Will be copied. Does not need to be valid after the call.
 * @param[in] fileName is the file name where the error was triggered.
 * @param[in] lineNumber is the line number where the error was triggered.
 * @param[in] functionName is the name of the function where the error is triggered.
 * @param[in] className is the name of the class (if relevant).
 * @param[in] objectName is the name of the Object (if relevant).
 * @param[in] objectPointer is the address of the Object (if relevant).
 */
DLL_API void ReportErrorFullContext(const ErrorType &code,
                         CCString const errorDescription,
                         CCString const className,
						 CCString const objectName,
                         const void * const objectPointer,
                         CCString const fileName,
                         const int16 lineNumber,
                         CCString const functionName
                         );



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} //ErrorManagement
} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L0TYPES_ERRORMANAGEMENTFUNCTION_H_ */
