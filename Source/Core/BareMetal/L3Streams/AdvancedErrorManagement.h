/**
 * @file AdvancedErrorManagement.h
 * @brief Header file for class AdvancedErrorManagement
 * @date 21/10/2015
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

 * @details This header file contains the declaration of the class AdvancedErrorManagement
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ADVANCEDERRORMANAGEMENT_H_
#define ADVANCEDERRORMANAGEMENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompositeErrorManagement.h"
#include "StreamMemoryReference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace ErrorManagement {

template <class T1>
inline void FormattedReportError(		     const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
								 	 	 	 CCString const format,
                         	 	 	 	 	 T1 part1
											 );

template <class T1,class T2>
inline void FormattedReportError(		     const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
								 	 	 	 CCString const format,
                         	 	 	 	 	 T1 part1,
                         	 	 	 	 	 T2 part2
											 );

template <class T1,class T2,class T3>
inline void FormattedReportError(			 const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
                         	 	 	 	 	 T1 part1,
                         	 	 	 	 	 T2 part2,
                         	 	 	 	 	 T3 part3
											 );

template <class T1,class T2,class T3,class T4>
inline void FormattedReportError(			 const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4
											 );

template <class T1,class T2,class T3,class T4,class T5>
inline void FormattedReportError(			 const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5
											 );

template <class T1,class T2,class T3,class T4,class T5,class T6>
inline void FormattedReportError(			 const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
                         	 	 	 	 	 T4 part4,
                         	 	 	 	 	 T5 part5,
                         	 	 	 	 	 T6 part6
											 );
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

static const uint32 MAX_ERROR_MESSAGE_SIZE = 200u;

template <class T1>
void ErrorManagement::FormattedReportError(const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1
											 ){
	if (!code) {
	    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];
	    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);
	    smr.Printf(format,part1);
	    buffer[smr.Size()]='\0';
	    ReportError(code,&buffer[0],__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);
	}
}

template <class T1,class T2>
void ErrorManagement::FormattedReportError(  const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
	 	 	 	 	 	 	 	 	 	 	 T2 part2
											 ){
	if (!code) {
	    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];
	    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);
	    smr.Printf(format,part1,part2);
	    buffer[smr.Size()]='\0';
	    ReportError(code,&buffer[0],__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);
	}
}


template <class T1,class T2,class T3>
void ErrorManagement::FormattedReportError(const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
										     CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3
											 ){
	if (!code) {
	    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];
	    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);
	    smr.Printf(format,part1,part2,part3);
	    buffer[smr.Size()]='\0';
	    ReportError(code,&buffer[0],__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);
	}
}

template <class T1,class T2,class T3, class T4>
void ErrorManagement::FormattedReportError(const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
											 T4 part4
											 ){
	if (!code) {
	    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];
	    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);
	    smr.Printf(format,part1,part2,part3,part4);
	    buffer[smr.Size()]='\0';
	    ReportError(code,&buffer[0],__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);
	}
}

template <class T1,class T2,class T3, class T4, class T5>
void ErrorManagement::FormattedReportError(const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
											 T4 part4,
											 T5 part5
											 ){
	if (!code) {
	    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];
	    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);
	    smr.Printf(format,part1,part2,part3,part4,part5);
	    buffer[smr.Size()]='\0';
	    ReportError(code,&buffer[0],__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);
	}
}

template <class T1,class T2,class T3, class T4, class T5, class T6>
void ErrorManagement::FormattedReportError(const ErrorType &code,
											 CCString const fileName,
											 const int16 lineNumber,
											 CCString const functionName,
											 CCString const format,
	 	 	 	 	 	 	 	 	 	 	 T1 part1,
											 T2 part2,
											 T3 part3,
											 T4 part4,
											 T5 part5,
											 T6 part6
											 ){
	if (!code) {
	    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];
	    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);
	    smr.Printf(format,part1,part2,part3,part4,part5,part6);
	    buffer[smr.Size()]='\0';
	    ReportError(code,&buffer[0],__FILE__,__LINE__,__ERROR_FUNCTION_NAME__);
	}
}

 
#define FORMATTED_REPORT_ERROR(code, format,...)                                           \
ErrorManagement::FormattedReportError(code,__FILE__,__LINE__,__ERROR_FUNCTION_NAME__,format,__VA_ARGS__);
 
}

#endif /* ADVANCEDERRORMANAGEMENT_H_ */

