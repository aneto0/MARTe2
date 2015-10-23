/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they 
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
 * $Id: BasicSocket.cpp 3 2012-01-15 16:26:07Z aneto $
 *
**/

#include "BasicSocket.h"
#include "BasicTCPSocket.h"
#include "BasicUDPSocket.h"


#if  defined(_VXWORKS)
/** Sets the error status and depending on setup does appropriate action */
void BasicSocket::AssertSocketErrorCondition(EMFErrorType errorCode,const char *errorDescription/*=NULL*/,...){
    va_list argList;
    va_start(argList,errorDescription);
    VCAssertSocketErrorCondition(errorCode,this,"BasicSocket",errorDescription,argList);
    va_end(argList);
}
#endif

