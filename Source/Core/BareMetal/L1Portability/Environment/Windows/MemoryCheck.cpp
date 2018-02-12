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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe{

namespace MemoryCheck {

/*lint -e{715}  [MISRA C++ Rule 0-1-11], [MISRA C++ Rule 0-1-12]. Justification: This implementation does not require
 all the input parameters.*/
bool Check(const void * const address,
           const uint8  accessMode,
           const uint32 size) {

//	printf ("%016p\n",address);
	bool ret = (address!= NULL);

    MEMORY_BASIC_INFORMATION mbi = {0};
    if (ret) {
    	ret = VirtualQuery(address, &mbi, sizeof(mbi));
//printf("protect = %x address = %p\n",mbi.Protect,address);
    }
    if (ret){
		ret = ((mbi.Protect & (PAGE_GUARD|PAGE_NOACCESS))==0);
    }
    if (ret){
    	if (accessMode & ExecuteAccessMode){
    		ret = ret && ((mbi.Protect  & (PAGE_EXECUTE_READ|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE))!=0);
    	}
    	if (accessMode & ReadAccessMode){
    		ret = ret && ((mbi.Protect  & (PAGE_EXECUTE_READ|PAGE_READONLY|PAGE_READWRITE|PAGE_WRITECOPY))!=0);
    	}
        if (accessMode & WriteAccessMode){
        	ret = ret && ((mbi.Protect  & (PAGE_EXECUTE_READWRITE|PAGE_READWRITE|PAGE_WRITECOPY))!=0);
        }
    }
//if (!ret){
//	printf("{prot=%x,%i,%i}\n",mbi.Protect,ret,accessMode);
//}
    return ret;
}





}
}
