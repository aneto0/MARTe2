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
#include "HardwareI.h"
#include "HighResolutionTimer.h"
#include "ErrorInformation.h"
#include "Sleep.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace ErrorManagement {

void NullErrorProcessFunction(const ErrorInformation &errorInfo,
                              const char8 * const errorDescription) {
}

ErrorProcessFunctionType errorMessageProcessFunction = &NullErrorProcessFunction;

/**
 * @brief A structure pairing an error code with its explanation.
 */
const struct {
    const char8 * const name;
    ErrorIntegerFormat errorBitSet;
} errorNames[] = {
        { "NoError", NoError },
        { "Debug", Debug },
        { "Information", Information },
        { "Warning", Warning },
        { "FatalError", FatalError },
        { "RecoverableError", RecoverableError },
        { "InitialisationError", InitialisationError },
        { "OSError", OSError },
        { "ParametersError", ParametersError },
        { "IllegalOperation", IllegalOperation },
        { "ErrorSharing", ErrorSharing },
        { "ErrorAccessDenied", ErrorAccessDenied },
        { "Exception", Exception },
        { "Timeout", Timeout },
        { "CommunicationError", CommunicationError },
        { "SyntaxError", SyntaxError },
        { "UnsupportedFeature", UnsupportedFeature },
        { "InternalSetupError", InternalSetupError },
        { "Completed", Completed },
        { "NotCompleted", NotCompleted },
        { static_cast<const char8 *>(NULL), NoError }, };

void ErrorCodeToStream(const ErrorType &errorCode,
                       StreamI &stream) {
    //Skip the NoError
    uint32 i = 1u;
    bool firstErrorWritten = false;
    bool ok = true;
    while (ok && (errorNames[i].name != NULL)) {
        if (errorCode.Contains(errorNames[i].errorBitSet)) {
            uint32 size = 1u;
            if (firstErrorWritten) {
                ok = stream.Write("+", size);
            }
            else {
                firstErrorWritten = true;
            }

            size = StringHelper::Length(errorNames[i].name) + 1u;
            if (ok) {
                ok = stream.Write(errorNames[i].name, size);
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
                 const char8 * const errorDescription,
                 const char8 * const clsName,
                 const char8 * const objName,
                 const void * const objPtr,
                 const char8 * const fileName,
                 const int16 lineNumber,
                 const char8 * const functionName) {
    ErrorInformation errorInfo;
//    errorInfo.threadId = InvalidThreadIdentifier;
    errorInfo.objectPointer = static_cast<void*>(NULL);
    errorInfo.className = static_cast<const char8 *>(NULL);
    errorInfo.header.errorType = code;
    errorInfo.header.lineNumber = lineNumber;
    errorInfo.header.isObject = (objPtr != static_cast<const char8 *>(NULL));
    errorInfo.className = clsName;
    errorInfo.objectName = objName;
    errorInfo.objectPointer = objPtr;
    errorInfo.fileName = fileName;
    errorInfo.functionName = functionName;
    errorInfo.hrtTime = HighResolutionTimer::Counter();
    errorInfo.timeSeconds = Sleep::GetDateSeconds();
    errorInfo.platformSpecificString = HardwareI::GetPlatformSpecificString();
#ifndef INTERRUPT_SUPPORTED
//    errorInfo.threadId = Threads::Id();
#endif
    errorMessageProcessFunction(errorInfo, errorDescription);
}

void ReportErrorFullContext(const ErrorType &code,
                            const char8 * const errorDescription,
                            const char8 * const fileName,
                            const int16 lineNumber,
                            const char8 * const functionName) {
    ErrorInformation errorInfo;
//    errorInfo.threadId = InvalidThreadIdentifier;
    errorInfo.objectPointer = static_cast<void*>(NULL);
    errorInfo.className = static_cast<const char8 *>(NULL);
    errorInfo.header.errorType = code;
    errorInfo.header.lineNumber = lineNumber;
    errorInfo.fileName = fileName;
    errorInfo.functionName = functionName;
    errorInfo.hrtTime = HighResolutionTimer::Counter();
//    errorInfo.threadId = Threads::Id();
    errorInfo.timeSeconds = Sleep::GetDateSeconds();
    errorInfo.platformSpecificString = HardwareI::GetPlatformSpecificString();
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
