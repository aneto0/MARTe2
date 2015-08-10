/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
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
 * See the Licence for the specific language governing 
   permissions and limitations under the Licence. 
 *
 * $Id: ErrorManagement.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file
 * @brief Error management functions
 */
#ifndef _ERROR_MANAGEMENT_H
#define _ERROR_MANAGEMENT_H

#include "GeneralDefinitions.h"
#include "HighResolutionTimer.h"
#include "Threads.h"

 struct ErrorInformation;
/** values to be used in ErrorManagementFunction */
enum ErrorType {
	/** Debug Information (should never be found in production code) */
	Debug               =   2,
	/** Application Information to be used to understand the working of the code */
	Information         =   1,
	/** Application warns of suspicious conditions */
	Warning             =   0,
	/** Application reports a fatal error */
	FatalError          =  -1,
	/** Application reports an error that allows recovery */
	RecoverableError    =  -2,
	/** Application reports an error during initialisation */
	InitialisationError =  -3,
	/** Error while calling an operating system function */
	OSError             =  -4,
	/** Unexpected parameter value that was passed to a function */
	ParametersError     =  -5,
	/** The operation was illegal in the run time context */
	IllegalOperation    =  -6,
	/** The operation failed because of a sharing problen */
	ErrorSharing        =  -7,
	/** The operation failed because of a sharing problen */
	ErrorAccessDenied   =  -8,
	/** an exception has occurred */
	Exception           =  -9,
	/** a Timeout has occurred */
	Timeout             = -10,
	/** error during a communication */
	CommunicationError  = -11,
	/** error while parsing */
	SyntaxError         = -12,
	/**	something that should be possible but still it is not supported	 */
	UnsupportedError    = -13
};

/** 
 	 Information associated with the error
 	 to better be able to debug what happened and/or 
 	 to disambiguate among possible sources 
 */
struct ErrorInformation{
	
	/// first to be transmitted is the header;
	struct {
		///
		ErrorType 	errorType:8;

		///
		uint16      lineNumber;

		/// implies that ObjectPointer and ClassName are sent as well
		bool        isObject:1;

	} header;


	/**
	 *  time as high resoution timer ticks
	*/  
	int64     		hrtTime;
	
	/** a pointer to a const char * which is persistent
	 * so a real constant, not a char * relabeled as const char *
	 * scope should be global to the application and persistent 
	 */ 
	const char * 	fileName;

	/** a pointer to a const char * which is persistent
	 * so a real constant, not a char * relabeled as const char *
	 * scope should be global to the application and persistent 
	 */ 
	const char * 	functionName;
	
	/// thread ID
	TID		    	threadId;
	
	/**
	 * address of the object that produced the error
	 * object may be temporary in memory
	 * as objectPointer will only be printed, not used
	 */
	void *      	objectPointer;

	/** a pointer to a const char * which is persistent
	 * so a real constant, not a char * relabeled as const char *
	 * scope should be global to the application and persistent 
	 */ 
	const char * 	className;
	
	/// initialise to 0 all parts that are not set by all the reporting functions
	ErrorInformation(){
		threadId 		= (TID)0;
		objectPointer 	= NULL;
		className       = NULL;
	}
};

/** the type of an user provided ErrorProcessing function */
typedef void (*ErrorMessageProcessFunctionType)(
		ErrorInformation &			errorInfo,
		const char *				errorDescription);

/**
 * pointer to the function that will process the errors
 */
extern ErrorMessageProcessFunctionType errorMessageProcessFunction; 

extern "C" {
	/// translates ErrorType to string 
	const char *ErrorManagement_ErrorName(ErrorType errorCode);

	/// installs a user defined Error Handling function
	void ErrorManagement_SetErrorMessageProcessFunction(ErrorMessageProcessFunctionType userFun=NULL);

}

/**
 * @brief Collection of functions and  types to manage error reporting.
 *
 * These functions allows an error reporting mechanism. The user should only pass the code of the error
 * and a description but automatically the name of the file, the line number and the function 
 * are stored in the ErrorInformation structure. The user can implement a routine that will be called
 * by the report error function to manage errors in specific ways.
 */
class ErrorManagement{

public:    
    /** 
     * @brief Returns the name string associated to the error code.
     * @param errorCode is the error code. */
	static inline const char *ErrorName(ErrorType errorCode){
		return ErrorManagement_ErrorName(errorCode);
	}

    /**       
     * @brief Store the error informations in the ErrorInformation structure, then calls a predefined routine.
     * @param code is the error code.
     * @param errorDescription is the error description.
     * @param fileName is the file name where the error was triggered.
     * @param lineNumber is the line number where the error was triggered.
     * @param functionName is the function name in witch the error was triggered.
     *
     * The thread id is stored in the structure only if interrupts are disabled, because
     * it is not possible get the thread id in an interrupt routine. */
 	static inline void ReportError(
				ErrorType 			code, 
				const char *		errorDescription,
				const char *        fileName			= NULL,
				unsigned int		lineNumber  		= 0,
				const char *		functionName        = NULL
				){
		ErrorInformation errorInfo;
		errorInfo.header.errorType  = code;
		errorInfo.header.lineNumber = lineNumber;
		errorInfo.fileName 	 		= fileName;
		errorInfo.functionName      = functionName;
		errorInfo.hrtTime 			= HighResolutionTimer::Counter();
#if !INTERRUPT_SUPPORTED		
		errorInfo.threadId 			= Threads::Id();
#endif		
		errorMessageProcessFunction(errorInfo, errorDescription);
	}

    /** 
     * @brief Report completely the error within the thread. 
     * @param code is the error code.
     * @param errorDescription is the error description.
     * @param fileName is the file name where the error was triggered.
     * @param lineNumber is the line number where the error was triggered.
     * @param functionName is the function name where the error was triggered.
     */
	static inline void ReportErrorFullContext(
			ErrorType 			code, 
			const char *		errorDescription,             /// memory volatile - will not be touched but is not expected to be persistent
			const char *        fileName			= NULL,   /// permanent in memory- accessible to all threads
			unsigned int		lineNumber  		= 0,
			const char *		functionName        = NULL    /// permanent in memory- accessible to all threads
			){

		ErrorInformation errorInfo;
		errorInfo.header.errorType  = code;
		errorInfo.header.lineNumber = lineNumber;
		errorInfo.fileName 	 		= fileName;
		errorInfo.functionName      = functionName;
		errorInfo.hrtTime 			= HighResolutionTimer::Counter();		
		errorInfo.threadId 			= Threads::Id();	
		errorMessageProcessFunction(errorInfo, errorDescription);
	}

public:    
    /** 
     * @brief Sets the routine for error messages.
     * @param ErrorMessageProcessFunctionType is a pointer to the function called by ReportError. */
	static inline void SetErrorMessageProcessFunction(ErrorMessageProcessFunctionType userFun=NULL){
		ErrorManagement_SetErrorMessageProcessFunction(userFun);
	}
};

#ifndef __FUNCTION_NAME__
    #if  defined __FUNCTION__ 
        // Undeclared
        #define __FUNCTION_NAME__   __FUNCTION__  
    #elif defined __PRETTY_FUNCTION__
        // Undeclared
        #define __FUNCTION_NAME__   __PRETTY_FUNCTION__
    #else
        // Declared 
		#define __FUNCTION_NAME__   __func__ 
    #endif // __func__ 

#endif // __FUNCTION_NAME__

/** @brief The function to call in case of errors: passed automatically to ReportError the file name and the line number where the error was triggered.*/
#define REPORT_ERROR(code,message)\
		ErrorManagement::ReportError(code,message,__FILE__,__LINE__,__FUNCTION_NAME__);
/** @brief The function to call in case of errors: passed automatically to ReportError the file name and the line number where the error was triggered.*/
#define REPORT_ERROR_FULL(code,message)\
		ErrorManagement::ReportErrorFullContext(code,message,__FILE__,__LINE__,__FUNCTION_NAME__);

#endif
