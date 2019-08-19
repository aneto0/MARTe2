/**
 * @file ErrorManagementConsoleFunction.cpp
 * @brief Header file for class AnyType
 * @date 24 Jan 2018
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

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <stdio.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "HighResolutionTimer.h"
#include "ErrorInformation.h"
#include "ErrorTypeLookup.h"
#include "CString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe {

namespace ErrorManagement {

FILE *errorFile = NULL;

ErrorManagement::ErrorType PrintError(ErrorManagement::ErrorType e){
    if (!e){
        ErrorManagement::ErrorTypeLookup *etl = &ErrorManagement::errorTypeLookup[0];
        while (!etl->name.IsNullPtr()){
            if ((etl->errorBitSet &  e.format_as_integer)!= 0){
                fprintf(errorFile,"(err=%s)",etl->name.GetList());
            }
            etl++;
        }
    }

    return e;
}

ErrorInformation x;


void NullErrorProcessFunction(const ErrorInformation &errorInfo,CCString const errorDescription){
	if (errorFile == NULL){
		errorFile = fopen("MARTe_Error.log","w");
	}

	if (errorFile != NULL){
		fprintf (errorFile,"(%s ",errorInfo.fileName.GetList());
		fprintf (errorFile,", @%i): ",errorInfo.header.lineNumber);
		fprintf (errorFile,"error ");
		PrintError(errorInfo.header.errorType);
		fprintf (errorFile," %s",errorDescription.GetList());
		fprintf (errorFile,"   fn = %s ",errorInfo.functionName.GetList());
		fprintf (errorFile,"\n");
		fflush(errorFile);
	}

}

}


} //MARTe
