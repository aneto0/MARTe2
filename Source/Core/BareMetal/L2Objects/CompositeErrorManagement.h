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

template <class T1,class T2,class T3,class T4,class T5>
inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);

template <class T1,class T2,class T3,class T4,class T5,class T6>
inline void CompositeReportError(const ErrorType &code,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
                         	 	 	 	 	 T6 part6,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName);
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

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

template <class T1,class T2,class T3,class T4,class T5>
void ErrorManagement::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											T5 part5,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		DynamicCString err;
		err.Append(part1);
		err.Append(part2);
		err.Append(part3);
		err.Append(part4);
		err.Append(part5);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}


template <class T1,class T2,class T3,class T4,class T5,class T6>
void ErrorManagement::CompositeReportError(const ErrorType &code,
											T1 part1,
											T2 part2,
											T3 part3,
											T4 part4,
											T5 part5,
											T6 part6,
											CCString const fileName,
											const int16 lineNumber,
											CCString const functionName){
	if (!code) {
		DynamicCString err;
		err.Append(part1);
		err.Append(part2);
		err.Append(part3);
		err.Append(part4);
		err.Append(part5);
		err.Append(part6);
		ConditionalReportError(code,err,fileName,lineNumber,functionName);
	}
}



#define COMPOSITE_REPORT_ERROR(code,part1,...)															  \
MARTe::ErrorManagement::CompositeReportError(code,part1, __VA_ARGS__,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);



}
#endif /* ERRORMANAGEMENT_H_ */

