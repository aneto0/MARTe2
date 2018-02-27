/**
 * @file CompoisteErrorManagement.h
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

#ifndef COMPOSITEERRORMANAGEMENT_H_
#define COMPOSITEERRORMANAGEMENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "DynamicCString.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


namespace ErrorManagement {


/**
 * @brief Stores the error informations in an ErrorInformation structure, then calls a predefined routine.
 * Only acts if code contains a valid error code!
 * @details The thread identifier is stored in the ErrorInformation structure only if interrupts are disabled, because
 * it is not possible get the thread id in an interrupt routine.
 * @param[in] code is the error code. Must be set for the routine to execute
 * @param[in] errorDescription is the error description. Will be copied. Does not need to be valid after the call.
 * @param[in] fileName is the file name where the error was triggered.
 * @param[in] lineNumber is the line number where the error was triggered.
 * @param[in] functionName is the name of the function where the error is triggered.
 */
inline void ConditionalReportError(const ErrorType &code,
                         CCString const errorDescription,
                         CCString const fileName = static_cast<const char8 *>(NULL),
                         const int16 lineNumber = 0,
                         CCString const functionName = static_cast<const char8 *>(NULL));


template <class T1,class T2>
inline void CompositeReportError(const ErrorType &code,
                         	 	 	 	 	 T1 part1,
                         	 	 	 	 	 T2 part2,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

template <class T1,class T2,class T3>
inline void CompositeReportError(const ErrorType &code,
                         	 	 	 	 	 T1 part1,
                         	 	 	 	 	 T2 part2,
                         	 	 	 	 	 T3 part3,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

template <class T1,class T2,class T3,class T4>
inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void ErrorManagement::ConditionalReportError(const ErrorType &code,
                         	 	 	 	 	 CCString const errorDescription,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName){
	if (!code){
		ReportError(code,errorDescription,fileName,lineNumber,functionName);
	}
}

template <class T1,class T2>
void ErrorManagement::CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName){
	if (!code) {
		DynamicCString err;
		err.Append(part1);
		err.Append(part2);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}

template <class T1,class T2,class T3>
void ErrorManagement::CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName){
	if (!code) {
		DynamicCString err;
		err.Append(part1);
		err.Append(part2);
		err.Append(part3);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}
template <class T1,class T2,class T3,class T4>
void ErrorManagement::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		DynamicCString err;
		err.Append(part1);
		err.Append(part2);
		err.Append(part3);
		err.Append(part4);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}
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
#define CONDITIONAL_REPORT_ERROR(code,message)\
MARTe::ErrorManagement::ConditionalReportError(code,message,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \


#define COMPOSITE_REPORT_ERROR(code,part1,...)															  \
MARTe::ErrorManagement::CompositeReportError(code,part1, __VA_ARGS__,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \



}
#endif /* ERRORMANAGEMENT_H_ */

