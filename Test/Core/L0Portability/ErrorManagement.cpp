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

#include "ErrorManagement.h"


static inline void NULLErrorMessageProcessFunction(ErrorInformation &errorInfo,const char *errorDescription){}

/**
 * @brief Pointer to the function that will process the errors
 */
ErrorMessageProcessFunctionType errorMessageProcessFunction = NULLErrorMessageProcessFunction; 

void ErrorManagement_SetErrorMessageProcessFunction(ErrorMessageProcessFunctionType userFun){
    if (userFun != NULL)  errorMessageProcessFunction = userFun;
    else errorMessageProcessFunction = NULLErrorMessageProcessFunction;
}


/** a structure pairing an error code with its explanation. */
static const 
struct {
    const char *	name;
    ErrorType 		error;
} errorNames[] = {
	    {"Debug Information",     Debug},
	    {"Information",           Information },
	    {"Warning",               Warning },
	    {"FatalError",            FatalError },
	    {"RecoverableError",      RecoverableError },
	    {"InitialisationError",   InitialisationError },
	    {"OSError",               OSError },
	    {"ParametersError",       ParametersError },
	    {"IllegalOperation",      IllegalOperation },
	    {"ErrorSharing",          ErrorSharing },
	    {"ErrorAccessDenied",     ErrorAccessDenied},
	    {"Exception",             Exception},
	    {"Timeout",               Timeout},
	    {"CommunicationError",    CommunicationError},
	    {"SyntaxError",           SyntaxError},
	    {"UnsupportedError",      UnsupportedError},
	    {NULL,                    SyntaxError},
};


/** @brief Translate errorCode to ErrorName */
const char *ErrorManagement_ErrorName(ErrorType errorCode ){
    int i=0;
    while ( errorNames[i].name != NULL){
        if (errorNames[i].error == errorCode) return errorNames[i].name;
        i++;
    }
    return "Unrecognised Error";
}
