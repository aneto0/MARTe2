/**
 * @file MemoryCheck.cpp
 * @brief Source file for module MemoryCheck
 * @date 27/07/2015
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

 * @details This source file contains the definition of all the methods for
 * the module MemoryCheck (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#ifndef LINT

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#else
#include "lint-linux.h"
#endif

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "../../MemoryCheck.h"
#include "ErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


class SystemInfoRecord{
public:
	static SYSTEM_INFO &Access(){
		static SystemInfoRecord sir;
		return sir.si;
	}

private:
	SYSTEM_INFO si;
	SystemInfoRecord(){
		GetSystemInfo(&si);
	}
};

//TODO check the full range

namespace MARTe{
namespace MemoryCheck {

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: This implementation does not require
 all the input parameters.*/
bool Check(const void * const address,
           const uint8  accessMode,
           const uint32 size) {

	ErrorManagement::ErrorType ret;
	bool returnValue = false;
	MEMORY_BASIC_INFORMATION mbi;

   	ret.illegalOperation = (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) ;
   	REPORT_ERROR(ret,"VirtualQuery failed");

    if (ret){

		returnValue = ((mbi.Protect & (PAGE_GUARD|PAGE_NOACCESS))==0);

    	if (accessMode & ExecuteAccessMode){
    		returnValue = returnValue && ((mbi.Protect  & (PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE))!=0);
    	}
    	if (accessMode & ReadAccessMode){
    		returnValue = returnValue && ((mbi.Protect  & (PAGE_EXECUTE_READ|PAGE_READONLY|PAGE_READWRITE|PAGE_WRITECOPY))!=0);
    	}
        if (accessMode & WriteAccessMode){
        	returnValue = returnValue && ((mbi.Protect  & (PAGE_EXECUTE_READWRITE|PAGE_READWRITE|PAGE_WRITECOPY))!=0);
        }
    }


    return returnValue;
}





}
}
