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
 * Logging management functions
 */
#ifndef ADVANCED_ERROR_MANAGEMENT_H
#define ADVANCED_ERROR_MANAGEMENT_H

#include "ErrorManagement.h"
#include "StreamMemoryReference.h"

#define MAX_ERROR_MESSAGE_SIZE 200

#define REPORT_ERROR_PARAMETERS(code,message,...) \
{ \
	char buffer[MAX_ERROR_MESSAGE_SIZE+1];\
	StreamMemoryReference smr(buffer,MAX_ERROR_MESSAGE_SIZE);\
	smr.Printf(message,__VA_ARGS__);\
	buffer[smr.Size()]=0;\
	ErrorManagement::ReportError(code,buffer,__FILE__,__LINE__,__FUNCTION_NAME__);\
}

//		ErrorManagement::ReportErrorFullContext(code,message,__FILE__,__LINE__,__FUNCTION_NAME__);


#endif
