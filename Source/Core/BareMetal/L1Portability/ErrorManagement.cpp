/**
 * @file ErrorManagement.cpp
 * @brief Source file for module ErrorManagement
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

 * @details This source file contains the definition of all the methods for
 * the module ErrorManagement (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "HighResolutionTimer.h"
#include "ErrorInformation.h"
#include "ErrorTypeLookup.h"
#include "StringHelper.h"
#include "CString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe {

namespace ErrorManagement {

extern void NullErrorProcessFunction(const ErrorInformation &errorInfo,CCString const errorDescription);


ErrorProcessFunctionType errorMessageProcessFunction = &NullErrorProcessFunction;


void ErrorCodeToStream(const ErrorType &errorCode,StreamI &stream) {
    //Skip the NoError
    uint32 i = 1u;
    bool firstErrorWritten = false;
    bool ok = true;
    while (ok && (!errorTypeLookup[i].name.IsNullPtr())) {
        if (errorCode.Contains(errorTypeLookup[i].errorBitSet)) {
            uint32 size = 1u;
            if (firstErrorWritten) {
                ok = stream.Write("+", size);
            }
            else {
                firstErrorWritten = true;
            }

            // TODO  why +1
//            size = StringHelper::Length(errorTypeLookup[i].name) + 1u;
            size = errorTypeLookup[i].name.GetSize();
            if (ok) {
                ok = stream.Write(errorTypeLookup[i].name.GetList(), size);
            }
        }
        i++;
    }

    if (ok && (!firstErrorWritten)) {
        uint32 size = 7u;
        /*lint -e{534} write to stream failure is ignored.*/
        stream.Write("NoError", size);
    }
}
 
void ReportError(const ErrorType &code,
                 CCString const errorDescription,
                 CCString const fileName,
                 const int16 lineNumber,
                 CCString const functionName) {
    ErrorInformation errorInfo;
//    errorInfo.threadId = InvalidThreadIdentifier;
    errorInfo.objectPointer = static_cast<void*>(NULL);
    errorInfo.className = static_cast<const char8 *>(NULL);
    errorInfo.header.errorType = code;

    errorInfo.header.lineNumber = lineNumber;
    errorInfo.fileName  = fileName.GetList();
    errorInfo.functionName = functionName.GetList();
    errorInfo.hrtTime = HighResolutionTimer::Counter();
#ifndef INTERRUPT_SUPPORTED
//    errorInfo.threadId = Threads::Id();
#endif
    errorMessageProcessFunction(errorInfo, errorDescription);
}

void ReportErrorFullContext(const ErrorType &code,
                            CCString const errorDescription,
							CCString const className,
							CCString const objectName,
	                        const void * const objectPointer,
                            CCString const fileName,
                            const int16 lineNumber,
                            CCString const functionName) {
    ErrorInformation errorInfo;
//    errorInfo.threadId = InvalidThreadIdentifier;
    errorInfo.objectPointer = objectPointer;
    errorInfo.className = className;
    errorInfo.objectName = objectName;
    errorInfo.header.errorType = code;
    errorInfo.header.lineNumber = lineNumber;
    errorInfo.fileName = fileName.GetList();
    errorInfo.functionName = functionName.GetList();
    errorInfo.hrtTime = HighResolutionTimer::Counter();
//    errorInfo.threadId = Threads::Id();
    errorMessageProcessFunction(errorInfo, errorDescription);
}

void SetErrorProcessFunction(const ErrorProcessFunctionType userFun) {
    if (userFun != NULL) {
        errorMessageProcessFunction = userFun;
    }
    else {
        errorMessageProcessFunction = &NullErrorProcessFunction;
    }
}

}

}
